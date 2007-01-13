// KPSolutionMasterSpeciesPg1.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "KPSolutionMasterSpeciesPg1.h"

#include "DDX_DDV.h"
#include "Util.h"


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CKPSolutionMasterSpeciesPg1, baseCKPSolutionMasterSpeciesPg1)

#undef IDC_MSHFLEXGRID1
#undef IDC_SRCDBPGCTRL1
/////////////////////////////////////////////////////////////////////////////
// CKPSolutionMasterSpeciesPg1 property page

const long NCOL_IMAGE          = 0;
const long NCOL_ELEMENT_NAME   = 1;
const long NCOL_MASTER_SPECIES = 2;
const long NCOL_ALKALINITY     = 3;
const long NCOL_FORMULA_OR_GFW = 4;
const long NCOL_GFW_OF_ELEMENT = 5;

CKPSolutionMasterSpeciesPg1::CKPSolutionMasterSpeciesPg1() : baseCKPSolutionMasterSpeciesPg1(CKPSolutionMasterSpeciesPg1::IDD)
{
	//{{AFX_DATA_INIT(CKPSolutionMasterSpeciesPg1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pictHolderRightArrow.CreateFromIcon(IDI_RIGHT_ARROW2);
	m_pictHolderStar.CreateFromIcon(IDI_STAR);

	m_hWndLastControl  = NULL;
	m_bEditLastControl = FALSE;

	m_bIgnoreChanges = false;
}

CKPSolutionMasterSpeciesPg1::~CKPSolutionMasterSpeciesPg1()
{
}

void CKPSolutionMasterSpeciesPg1::DoDataExchange(CDataExchange* pDX)
{
	baseCKPSolutionMasterSpeciesPg1::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKPSolutionMasterSpeciesPg1)
	DDX_Control(pDX, IDC_EDIT_ELEMENT_NAME, m_ctrlElementName);
	DDX_Control(pDX, IDC_EDIT_MASTER_SPECIES, m_ctrlMasterSpecies);
	DDX_Control(pDX, IDC_EDIT_ALKALINITY, m_ctrlAlkalinity);
	DDX_Control(pDX, IDC_EDIT_FORMULA_OR_GFW, m_ctrlFormulaOrGFW);
	DDX_Control(pDX, IDC_EDIT_GFW_ELEMENT, m_ctrlGFWOfElement);	
	DDX_Control(pDX, IDC_SRCDBPGCTRL, m_pager);	
	DDX_Control(pDX, IDC_MSHFLEXGRID, m_ctrlGrid);
	DDX_Control(pDX, IDC_DESCRIPTION, m_eInputDesc);
	//}}AFX_DATA_MAP

	if (m_bFirstSetActive)
	{
		InitGrid(pDX, IDC_MSHFLEXGRID);
	}
	if (pDX->m_bSaveAndValidate)
	{
		CWaitCursor wait;
		int nCurrentTextBox = 0;
		std::list<CMaster> listMaster;
		for (long nRow = 1; nRow < m_ctrlGrid.GetRows(); ++nRow)
		{
			OnLeaveCellGrid();

			CMaster mast;

			// element name
			nCurrentTextBox = IDC_EDIT_ELEMENT_NAME;
			DDX_GridText(pDX, IDC_MSHFLEXGRID, nRow, NCOL_ELEMENT_NAME, mast.m_strName);
			mast.m_strName.Replace(_T(' '), _T('_'));
			if (mast.m_strName.IsEmpty()) continue;
			if (!CUtil::IsValidElementName(mast.m_strName))
			{
				m_ctrlGrid.SetRow(nRow);
				OnRowColChangeGrid();
				pDX->PrepareEditCtrl(nCurrentTextBox);
				::AfxMessageBox(_T("Element name must start with capital letter."));
				pDX->Fail();
			}

			// master species
			nCurrentTextBox = IDC_EDIT_MASTER_SPECIES;
			DDX_GridText(pDX, IDC_MSHFLEXGRID, nRow, NCOL_MASTER_SPECIES, mast.m_strMasterSpecies);

			if (mast.m_strMasterSpecies.IsEmpty() || !CUtil::IsValidMasterSpeciesName(mast.m_strMasterSpecies))
			{
				m_ctrlGrid.SetRow(nRow);
				OnRowColChangeGrid();
				pDX->PrepareEditCtrl(nCurrentTextBox);
				::AfxMessageBox(_T("Master species must start with capital letter."));
				pDX->Fail();
			}

			// alkalinity
			nCurrentTextBox = IDC_EDIT_ALKALINITY;
			DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID, nRow, NCOL_ALKALINITY, mast.m_dAlk);
			if (mast.m_dAlk == std::numeric_limits<double>::signaling_NaN())
			{
				m_ctrlGrid.SetRow(nRow);
				OnRowColChangeGrid();
				pDX->PrepareEditCtrl(nCurrentTextBox);
				::AfxMessageBox(_T("Expected alkalinity for master species"));
				pDX->Fail();
			}

			// ( gram formula weight or formula )
			nCurrentTextBox = IDC_EDIT_FORMULA_OR_GFW;
			DDX_GridText(pDX, IDC_MSHFLEXGRID, nRow, NCOL_FORMULA_OR_GFW, mast.m_strFormula);
			if (!mast.m_strFormula.IsEmpty() && ::_istdigit(mast.m_strFormula[0]) || mast.m_strFormula[0] == _T('.') ||  mast.m_strFormula[0] == _T('-'))
			{
				mast.m_strFormula = _T("");
				try 
				{
					DDX_GridText(pDX, IDC_MSHFLEXGRID, nRow, NCOL_FORMULA_OR_GFW, mast.m_dGFW);
				}
				catch (CUserException* pE)
				{
					OnLeaveCellGrid();
					pE->Delete();
					m_ctrlGrid.SetRow(nRow);
					OnRowColChangeGrid();
					pDX->PrepareEditCtrl(nCurrentTextBox);
					pDX->Fail();
				}
			}
			else if (!mast.m_strFormula.IsEmpty() && ::_istupper(mast.m_strFormula[0])) // TODO DAVID
			{
				// do nothing
			}
			else
			{
				m_ctrlGrid.SetRow(nRow);
				OnRowColChangeGrid();
				pDX->PrepareEditCtrl(nCurrentTextBox);
				CString strMsg;
				strMsg.Format(_T("Expected gram formula weight for master species, %s."), mast.m_strName);
				::AfxMessageBox(strMsg);
				pDX->Fail();
			}

			// gram formula weight of element
			mast.m_bPrimary = false;
			if (mast.m_strName.Find(_T('(')) == -1)
			{
				mast.m_bPrimary = true;
				if (mast.m_strName != _T("E"))
				{
					nCurrentTextBox = IDC_EDIT_GFW_ELEMENT;
					DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID, nRow, NCOL_GFW_OF_ELEMENT, mast.m_dGFW);
					if (mast.m_dGFW == std::numeric_limits<double>::signaling_NaN())
					{
						m_ctrlGrid.SetRow(nRow);
						OnRowColChangeGrid();
						pDX->PrepareEditCtrl(nCurrentTextBox);
						CString strMsg;
						strMsg.Format(_T("Expected gram formula weight for element, %s."), mast.m_strName);
						::AfxMessageBox(strMsg);
						pDX->Fail();
					}
				}
			}
			listMaster.push_back(mast);
		}

		// if here list is valid and can be assigned to the member variable
		m_listMaster.assign(listMaster.begin(), listMaster.end());
	}
	else
	{
		// size grid
		m_ctrlGrid.SetRows(m_listMaster.size() + 2);

		std::list<CMaster>::const_iterator cIter = m_listMaster.begin();
		for (long nRow = 1; cIter != m_listMaster.end(); ++cIter, ++nRow)
		{
			CMaster mast(*cIter);

			// element name
			DDX_GridText(pDX, IDC_MSHFLEXGRID, nRow, NCOL_ELEMENT_NAME, mast.m_strName);

			// master species
			DDX_GridText(pDX, IDC_MSHFLEXGRID, nRow, NCOL_MASTER_SPECIES, mast.m_strMasterSpecies);

			// alkalinity
			DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID, nRow, NCOL_ALKALINITY, mast.m_dAlk);

			// ( gram formula weight or formula )
			if (mast.m_strFormula.IsEmpty())
			{
				DDX_GridText(pDX, IDC_MSHFLEXGRID, nRow, NCOL_FORMULA_OR_GFW, mast.m_dGFW);
			}
			else
			{
				DDX_GridText(pDX, IDC_MSHFLEXGRID, nRow, NCOL_FORMULA_OR_GFW, mast.m_strFormula);
			}

			// gram formula weight of element
			if (mast.m_bPrimary /* && mast.m_strName != _T("E") */)
			{
				DDX_GridText(pDX, IDC_MSHFLEXGRID, nRow, NCOL_GFW_OF_ELEMENT, mast.m_dGFW);
			}
		}

		m_ctrlGrid.SetCol(0);
		m_ctrlGrid.SetRow(m_listMaster.size() + 1);
		m_ctrlGrid.SetCellPictureAlignment(flexAlignCenterCenter);
		m_ctrlGrid.SetRefCellPicture(m_pictHolderStar.GetPictureDispatch());
		// set initial pos
		m_ctrlGrid.SetRow(1);
		m_ctrlGrid.SetCol(1);
		OnRowColChangeGrid();
	}
}


BEGIN_MESSAGE_MAP(CKPSolutionMasterSpeciesPg1, baseCKPSolutionMasterSpeciesPg1)
	//{{AFX_MSG_MAP(CKPSolutionMasterSpeciesPg1)
	ON_EN_CHANGE(IDC_EDIT_ELEMENT_NAME, OnChangeEditElementName)
	ON_EN_KILLFOCUS(IDC_EDIT_ELEMENT_NAME, OnKillfocusEditElementName)
	ON_EN_SETFOCUS(IDC_EDIT_ELEMENT_NAME, OnSetfocusEditElementName)
	ON_EN_CHANGE(IDC_EDIT_MASTER_SPECIES, OnChangeEditMasterSpecies)
	ON_EN_KILLFOCUS(IDC_EDIT_MASTER_SPECIES, OnKillfocusEditMasterSpecies)
	ON_EN_SETFOCUS(IDC_EDIT_MASTER_SPECIES, OnSetfocusEditMasterSpecies)
	ON_EN_CHANGE(IDC_EDIT_ALKALINITY, OnChangeEditAlkalinity)
	ON_EN_KILLFOCUS(IDC_EDIT_ALKALINITY, OnKillfocusEditAlkalinity)
	ON_EN_SETFOCUS(IDC_EDIT_ALKALINITY, OnSetfocusEditAlkalinity)
	ON_EN_CHANGE(IDC_EDIT_FORMULA_OR_GFW, OnChangeEditFormulaOrGfw)
	ON_EN_KILLFOCUS(IDC_EDIT_FORMULA_OR_GFW, OnKillfocusEditFormulaOrGfw)
	ON_EN_SETFOCUS(IDC_EDIT_FORMULA_OR_GFW, OnSetfocusEditFormulaOrGfw)
	ON_EN_CHANGE(IDC_EDIT_GFW_ELEMENT, OnChangeEditGfwElement)
	ON_EN_KILLFOCUS(IDC_EDIT_GFW_ELEMENT, OnKillfocusEditGfwElement)
	ON_EN_SETFOCUS(IDC_EDIT_GFW_ELEMENT, OnSetfocusEditGfwElement)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
	// custom edit grid notifications
	ON_MESSAGE(EGN_BEGINCELLEDIT, OnBeginCellEdit)
	ON_MESSAGE(EGN_CHANGE, OnChange)
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
	ON_MESSAGE(EGN_SETFOCUS, OnSetfocusGrid)

	// custom radio notifications
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_FORMULA, IDC_RADIO_GFW, OnClickedRadio)
	ON_CONTROL_RANGE(BN_SETFOCUS, IDC_RADIO_FORMULA, IDC_RADIO_GFW, OnSetfocusRadio)

END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CKPSolutionMasterSpeciesPg1, baseCKPSolutionMasterSpeciesPg1)
    //{{AFX_EVENTSINK_MAP(CKPSolutionMasterSpeciesPg1)
	ON_EVENT(CKPSolutionMasterSpeciesPg1, IDC_SRCDBPGCTRL, 1 /* GotoFirstClicked */, OnGotoFirstClicked, VTS_NONE)
	ON_EVENT(CKPSolutionMasterSpeciesPg1, IDC_SRCDBPGCTRL, 4 /* GotoLastClicked */, OnGotoLastClicked, VTS_NONE)
	ON_EVENT(CKPSolutionMasterSpeciesPg1, IDC_SRCDBPGCTRL, 5 /* GotoNewClicked */, OnGotoNewClicked, VTS_NONE)
	ON_EVENT(CKPSolutionMasterSpeciesPg1, IDC_SRCDBPGCTRL, 3 /* GotoNextClicked */, OnGotoNextClicked, VTS_NONE)
	ON_EVENT(CKPSolutionMasterSpeciesPg1, IDC_SRCDBPGCTRL, 2 /* GotoPrevClicked */, OnGotoPrevClicked, VTS_NONE)
	ON_EVENT(CKPSolutionMasterSpeciesPg1, IDC_MSHFLEXGRID, 70 /* RowColChange */, OnRowColChangeGrid, VTS_NONE)
	ON_EVENT(CKPSolutionMasterSpeciesPg1, IDC_MSHFLEXGRID, 72 /* LeaveCell */, OnLeaveCellGrid, VTS_NONE)
	ON_EVENT(CKPSolutionMasterSpeciesPg1, IDC_MSHFLEXGRID, -601 /* DblClick */, OnDblClickGrid, VTS_NONE)
	ON_EVENT(CKPSolutionMasterSpeciesPg1, IDC_MSHFLEXGRID, 71 /* EnterCell */, OnEnterCellGrid, VTS_NONE)
	ON_EVENT(CKPSolutionMasterSpeciesPg1, IDC_MSHFLEXGRID, -602 /* KeyDown */, OnKeyDownGrid, VTS_PI2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CKPSolutionMasterSpeciesPg1::InitGrid(CDataExchange* pDX, int nIDC)
{
	UNUSED(pDX);
	ASSERT(nIDC == m_ctrlGrid.GetDlgCtrlID());
	UNUSED(nIDC);   // unused in release builds

	// set column widths
	m_ctrlGrid.SetColWidth(NCOL_IMAGE,          0, 300);
	m_ctrlGrid.SetColWidth(NCOL_ELEMENT_NAME,   0, 1200);
	m_ctrlGrid.SetColWidth(NCOL_MASTER_SPECIES, 0, 1200);
	m_ctrlGrid.SetColWidth(NCOL_ALKALINITY,     0, 1100);
	m_ctrlGrid.SetColWidth(NCOL_FORMULA_OR_GFW, 0, 1300);
	m_ctrlGrid.SetColWidth(NCOL_GFW_OF_ELEMENT, 0, 1300);

	// set grid titles
    m_ctrlGrid.SetTextMatrix( 0, NCOL_ELEMENT_NAME,   _T("Element name"));   // m_ctrlElementName
    m_ctrlGrid.SetTextMatrix( 0, NCOL_MASTER_SPECIES, _T("Master species")); // m_ctrlMasterSpecies
    m_ctrlGrid.SetTextMatrix( 0, NCOL_ALKALINITY,     _T("Alkalinity"));     // m_ctrlAlkalinity
    m_ctrlGrid.SetTextMatrix( 0, NCOL_FORMULA_OR_GFW, _T("Formula or GFW")); // m_ctrlFormulaOrGFW
    m_ctrlGrid.SetTextMatrix( 0, NCOL_GFW_OF_ELEMENT, _T("GFW of Element")); // m_ctrlGFWOfElement

	// set alignment
	//m_ctrlGrid.SetColAlignment(NCOL_PHASE_NAME, flexAlignLeftCenter);
	//m_ctrlGrid.SetColAlignment(NCOL_RXN, flexAlignLeftCenter);

	// set starting position
	m_ctrlGrid.SetRow(1);
	m_ctrlGrid.SetCol(1);
}

LRESULT CKPSolutionMasterSpeciesPg1::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
{
	TRACE("OnBeginCellEdit\n");

	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bDenyEdit = FALSE;	// allow edit

	const LPCTSTR arrStrCheckEqnOptions[] = { _T("Yes"), _T("No") };

	if (pInfo->item.hWndCombo)
	{
		ASSERT(m_bIgnoreChanges == false);
		m_bIgnoreChanges = true;
		TRACE("Setting m_bIgnoreChanges = true in OnBeginCellEdit\n");
	}

	switch (nID)
	{
	case IDC_MSHFLEXGRID :
		switch ( pInfo->item.iCol )
		{
		case NCOL_ELEMENT_NAME:
			break;
		case NCOL_MASTER_SPECIES:
			break;
		case NCOL_ALKALINITY:
			break;
		case NCOL_FORMULA_OR_GFW:
			break;
		case NCOL_GFW_OF_ELEMENT:
			break;
		default :
			ASSERT(FALSE);
			break;
		}
		break;
	}

	return bDenyEdit;
}

LRESULT CKPSolutionMasterSpeciesPg1::OnChange(WPARAM wParam, LPARAM lParam)
{
	TRACE("CKPSolutionMasterSpeciesPg1::OnChange\n");

	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	ASSERT(m_bIgnoreChanges == true); // should have vbeen set in OnBeginCellEdit

	switch (nID)
	{
	case IDC_MSHFLEXGRID :
		switch ( pInfo->item.iCol )
		{
		case NCOL_ELEMENT_NAME:
			m_ctrlElementName.SetWindowText(pInfo->item.pszText);
			break;
		case NCOL_MASTER_SPECIES:
			m_ctrlMasterSpecies.SetWindowText(pInfo->item.pszText);
			break;
		case NCOL_ALKALINITY:
			m_ctrlAlkalinity.SetWindowText(pInfo->item.pszText);
			break;
		case NCOL_FORMULA_OR_GFW:
			m_ctrlFormulaOrGFW.SetWindowText(pInfo->item.pszText);
			break;
		case NCOL_GFW_OF_ELEMENT:
			m_ctrlGFWOfElement.SetWindowText(pInfo->item.pszText);
			break;
		default :
			ASSERT(FALSE);
			break;
		}
		break;
	}
	return 0;

}

void CKPSolutionMasterSpeciesPg1::OnChangeEditAlkalinity() 
{
	TRACE("OnChangeEditAlkalinity\n");

	if (!m_bIgnoreChanges)
	{
		CString str;
		m_ctrlAlkalinity.GetWindowText(str);
		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_ALKALINITY, str);
	}
}

void CKPSolutionMasterSpeciesPg1::OnChangeEditElementName() 
{
	TRACE("OnChangeEditElementName\n");

	if (!m_bIgnoreChanges)
	{
		CString str;
		m_ctrlElementName.GetWindowText(str);
		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_ELEMENT_NAME, str);
		if (str.GetLength() > 0)
		{
			if (m_ctrlGrid.GetRows() - 1 == m_ctrlGrid.GetRow())
			{
				long nRow = m_ctrlGrid.GetRow();
				long nCol = m_ctrlGrid.GetCol();

				// add new row
				m_ctrlGrid.SetRows(m_ctrlGrid.GetRows() + 1);

				m_ctrlGrid.SetCol(0);
				m_ctrlGrid.SetRow(m_ctrlGrid.GetRows() - 1);
				m_ctrlGrid.SetCellPictureAlignment(flexAlignCenterCenter);
				m_ctrlGrid.SetRefCellPicture(m_pictHolderStar.GetPictureDispatch());
				m_ctrlGrid.SetRow(nRow);
				m_ctrlGrid.SetCol(nCol);
			}
		}
	}
}

void CKPSolutionMasterSpeciesPg1::OnChangeEditFormulaOrGfw() 
{
	TRACE("OnChangeEditFormulaOrGfw\n");

	if (!m_bIgnoreChanges)
	{
		CString str;
		m_ctrlFormulaOrGFW.GetWindowText(str);
		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_FORMULA_OR_GFW, str);
	}
}

void CKPSolutionMasterSpeciesPg1::OnChangeEditGfwElement() 
{
	TRACE("OnChangeEditGfwElement\n");

	if (!m_bIgnoreChanges)
	{
		CString str;
		m_ctrlGFWOfElement.GetWindowText(str);
		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_GFW_OF_ELEMENT, str);
	}
}

void CKPSolutionMasterSpeciesPg1::OnChangeEditMasterSpecies() 
{
	TRACE("OnChangeEditMasterSpecies\n");

	if (!m_bIgnoreChanges)
	{
		CString str;
		m_ctrlMasterSpecies.GetWindowText(str);
		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_MASTER_SPECIES, str);
	}
}

void CKPSolutionMasterSpeciesPg1::OnClickedRadio(UINT nID)
{
	TRACE("OnClickedRadio\n");

	long nRow = m_ctrlGrid.GetRow();
	UNUSED(nRow);
	switch (nID)
	{
	case IDC_RADIO_FORMULA:
		// nothing to do
		break;

	case IDC_RADIO_GFW:
		// nothing to do
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}

void CKPSolutionMasterSpeciesPg1::OnDblClickGrid() 
{
	if (!m_ctrlGrid.GetMouseRow())
	{
		// resize column to widest or to fit WWW
		long nCol = m_ctrlGrid.GetMouseCol();
		if (nCol == 0) return;

		CDC* pDC = m_ctrlGrid.GetDC();
		int nPixelsPerInchX = pDC->GetDeviceCaps(LOGPIXELSX);

		CSize size;
		long nWidest = pDC->GetTextExtent(_T("WWW"), 3).cx;
		for (long i = 0; i < m_ctrlGrid.GetRows(); ++i)
		{
			size = pDC->GetTextExtent(m_ctrlGrid.GetTextMatrix(i, nCol));
			if (size.cx > nWidest)
			{
				nWidest = size.cx;
			}
		}

		// limit col width to client rect minus imagewidth(col 0)
		CRect rect;
		m_ctrlGrid.GetClientRect(&rect);
		long nImageWidth = ::MulDiv(nPixelsPerInchX, m_ctrlGrid.GetColWidth(NCOL_IMAGE, 0), TWIPS_PER_INCH);
		nWidest = __min(nWidest, rect.right - 20 - nImageWidth);

		ASSERT(m_ctrlGrid.GetColWidth(NCOL_IMAGE, 0) >= 0);
		if (nWidest > 5)
		{
			m_ctrlGrid.SetColWidth(nCol, 0, ::MulDiv(nWidest, TWIPS_PER_INCH, nPixelsPerInchX));
		}
	}
}

LRESULT CKPSolutionMasterSpeciesPg1::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
{
	TRACE("OnEndCellEdit\n");

	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	UNUSED(pInfo);

	BOOL bMakeChange = TRUE;

	if (!m_bIgnoreChanges)
	{
		// this is a cut or paste
		m_bIgnoreChanges = true;
	}

	if (pInfo->item.iRow != m_ctrlGrid.GetRow())
	{
		m_bIgnoreChanges = false;
		return bMakeChange;
	}

	switch (nID)
	{
	case IDC_MSHFLEXGRID :
		switch ( pInfo->item.iCol )
		{
		case NCOL_ELEMENT_NAME:
			if (pInfo->item.pszText == NULL)
			{
				// edit cancelled
				m_ctrlElementName.SetWindowText(m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol));
			}
			else
			{
				m_ctrlElementName.SetWindowText(pInfo->item.pszText);
				if (strlen(pInfo->item.pszText) > 0)
				{
					if (m_ctrlGrid.GetRows() - 1 == pInfo->item.iRow)
					{
						// add new row
						m_ctrlGrid.SetRows(m_ctrlGrid.GetRows() + 1);

						m_ctrlGrid.SetCol(0);
						m_ctrlGrid.SetRow(m_ctrlGrid.GetRows() - 1);
						m_ctrlGrid.SetCellPictureAlignment(flexAlignCenterCenter);
						m_ctrlGrid.SetRefCellPicture(m_pictHolderStar.GetPictureDispatch());
						m_ctrlGrid.SetCol(pInfo->item.iCol);
						m_ctrlGrid.SetRow(pInfo->item.iRow);
					}
				}
			}
			break;

		case NCOL_MASTER_SPECIES:
			if (pInfo->item.pszText == NULL)
			{
				// edit cancelled
				m_ctrlMasterSpecies.SetWindowText(m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol));
			}
			else
			{
				m_ctrlMasterSpecies.SetWindowText(pInfo->item.pszText);
			}
			break;

		case NCOL_ALKALINITY:
			if (pInfo->item.pszText == NULL)
			{
				// edit cancelled
				m_ctrlAlkalinity.SetWindowText(m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol));
			}
			else
			{
				m_ctrlAlkalinity.SetWindowText(pInfo->item.pszText);
			}
			break;

		case NCOL_FORMULA_OR_GFW:
			if (pInfo->item.pszText == NULL)
			{
				// edit cancelled
				m_ctrlFormulaOrGFW.SetWindowText(m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol));
			}
			else
			{
				m_ctrlFormulaOrGFW.SetWindowText(pInfo->item.pszText);
			}
			break;

		case NCOL_GFW_OF_ELEMENT:
			if (pInfo->item.pszText == NULL)
			{
				// edit cancelled
				m_ctrlGFWOfElement.SetWindowText(m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol));
			}
			else
			{
				m_ctrlGFWOfElement.SetWindowText(pInfo->item.pszText);
			}
			break;
		default:
			ASSERT(FALSE);
			break;
		}
		break;
	}

	ASSERT(m_bIgnoreChanges == true);
	m_bIgnoreChanges = false;
	TRACE("Setting m_bIgnoreChanges = false in OnEndCellEdit\n");

	return bMakeChange;
}

void CKPSolutionMasterSpeciesPg1::OnEnterCellGrid() 
{
	UINT nResID = 0;
	switch (m_ctrlGrid.GetCol())
	{
	case NCOL_ELEMENT_NAME:
		nResID = IDS_STRING579;
		break;

	case NCOL_MASTER_SPECIES:
		nResID = IDS_STRING580;
		break;

	case NCOL_ALKALINITY:
		nResID = IDS_STRING581;
		break;

	case NCOL_FORMULA_OR_GFW:
		if (IsDlgButtonChecked(IDC_RADIO_FORMULA))
		{
			nResID = IDS_STRING582;
		}
		else
		{
			nResID = IDS_STRING583;
		}
		break;

	case NCOL_GFW_OF_ELEMENT:
		nResID = IDS_STRING584;
		break;

	default:
		ASSERT(FALSE);
		break;
	}

	if (nResID != 0)
	{
		CString strRes;
		strRes.LoadString(nResID);
		m_eInputDesc.SetWindowText(strRes);
	}
	else
	{
		m_eInputDesc.SetWindowText(_T(""));
	}
}

void CKPSolutionMasterSpeciesPg1::OnGotoFirstClicked() 
{
	TRACE("OnGotoFirstClicked\n");

	OnLeaveCellGrid();
	m_ctrlGrid.SetRow(1);
	m_ctrlGrid.GetCellLeft();
	OnRowColChangeGrid();

	ResetFocus();
}

void CKPSolutionMasterSpeciesPg1::OnGotoLastClicked() 
{
	TRACE("OnGotoLastClicked\n");

	OnLeaveCellGrid();
	for (long nRow = 1; nRow < m_ctrlGrid.GetRows(); ++nRow)
	{
		if (m_ctrlGrid.GetTextMatrix(nRow, NCOL_ELEMENT_NAME).IsEmpty())
			break;
	}
	m_ctrlGrid.SetRow(__max(nRow - 1, 1));
	m_ctrlGrid.GetCellLeft();
	OnRowColChangeGrid();
	ResetFocus();
}

void CKPSolutionMasterSpeciesPg1::OnGotoNewClicked() 
{
	TRACE("OnGotoNewClicked\n");

	OnLeaveCellGrid();

	if (m_ctrlGrid.GetRows() == m_ctrlGrid.GetRow() + 1)
	{
		m_ctrlGrid.SetRows(m_ctrlGrid.GetRows() + 1);
	}
	long nRow = m_ctrlGrid.GetRows() - 1;

	OnLeaveCellGrid();
	m_ctrlGrid.SetRow(__max(nRow, 1));
	m_ctrlGrid.GetCellLeft();
	OnRowColChangeGrid();

	m_ctrlElementName.SetFocus();
	m_ctrlElementName.SetSel(0, -1);
}

void CKPSolutionMasterSpeciesPg1::OnGotoNextClicked() 
{
	TRACE("OnGotoNextClicked\n");

	long nRow = m_ctrlGrid.GetRow();
	if (nRow != m_ctrlGrid.GetRows() - 1)
	{
		OnLeaveCellGrid();
		m_ctrlGrid.SetRow(__min(nRow + 1, m_ctrlGrid.GetRows() - 1));
		m_ctrlGrid.GetCellLeft();
		OnRowColChangeGrid();
	}

	ResetFocus();
}

void CKPSolutionMasterSpeciesPg1::OnGotoPrevClicked() 
{
	TRACE("OnGotoPrevClicked\n");

	long nRow = m_ctrlGrid.GetRow();
	if (nRow != 1)
	{
		OnLeaveCellGrid();
		m_ctrlGrid.SetRow(__max(nRow - 1, 1));
		m_ctrlGrid.GetCellLeft(); // scroll into view
		OnRowColChangeGrid();
	}

	ResetFocus();
}

BOOL CKPSolutionMasterSpeciesPg1::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// Declare the popup structure and initialize it.
	HH_POPUP        myPopup;
	memset(&myPopup, 0, sizeof(HH_POPUP));
	
	// Fill in the popup structure
	myPopup.cbStruct         = sizeof(HH_POPUP);
	myPopup.rcMargins.top    = 5;
	myPopup.rcMargins.bottom = 5;
	myPopup.rcMargins.left   = 5;
	myPopup.rcMargins.right  = 5;
	myPopup.clrForeground    = (COLORREF)-1;  // use default
	myPopup.clrBackground    = (COLORREF)-1;  // use default
	myPopup.idString         = 0;             // set the ID string to 0

	// Set placement
	if (IsContextHelp())
	{
		myPopup.pt.x = pHelpInfo->MousePos.x;
		myPopup.pt.y = pHelpInfo->MousePos.y;
	}
	else
	{
		if (pHelpInfo->hItemHandle)
		{
			// Center pt of window w/ offset
			CRect rc;
			::GetWindowRect((HWND)pHelpInfo->hItemHandle, &rc);
			myPopup.pt = rc.CenterPoint();
			myPopup.pt.y += 10;
		}
	}

	CString strRes;
	UINT nResID = 0;

	switch (pHelpInfo->iCtrlId)
	{
	case IDC_EDIT_ELEMENT_NAME: case IDC_STATIC_ELEMENT_NAME:
		strRes.LoadString(IDS_STRING579);
		break;
	case IDC_EDIT_MASTER_SPECIES: case IDC_STATIC_MASTER_SPECIES:
		strRes.LoadString(IDS_STRING580);
		break;
	case IDC_EDIT_ALKALINITY: case IDC_STATIC_ALKALINITY:
		strRes.LoadString(IDS_STRING581);
		break;
	case IDC_RADIO_FORMULA:
		strRes.LoadString(IDS_STRING585);
		break;
	case IDC_RADIO_GFW:
		strRes.LoadString(IDS_STRING586);
		break;
	case IDC_EDIT_FORMULA_OR_GFW:
		if (IsDlgButtonChecked(IDC_RADIO_FORMULA))
		{
			strRes.LoadString(IDS_STRING582);
		}
		else
		{
			strRes.LoadString(IDS_STRING583);
		}
		break;
	case IDC_EDIT_GFW_ELEMENT: case IDC_STATIC_GFW_ELEMENT:
		strRes.LoadString(IDS_STRING584);
		break;
	case IDC_MSHFLEXGRID:
		if (!IsContextHelp())
		{
			if (!(m_ctrlGrid.GetRowIsVisible(m_ctrlGrid.GetRow()) && m_ctrlGrid.GetColIsVisible(m_ctrlGrid.GetCol())))
			{
				::MessageBeep((UINT)-1);
				return TRUE;
			}

			// modify placement
			CDC* pDC = m_ctrlGrid.GetDC();
			int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
			int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

			long nLeft = ::MulDiv(m_ctrlGrid.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
			long nTop  = ::MulDiv(m_ctrlGrid.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

			CRect rc;
			m_ctrlGrid.GetWindowRect(rc);

			myPopup.pt.x = rc.left + nLeft;
			myPopup.pt.y = rc.top + nTop + 10;
		}
		switch (IsContextHelp() ? m_ctrlGrid.GetMouseCol() : m_ctrlGrid.GetCol())
		{
		case NCOL_ELEMENT_NAME:
			nResID = IDS_STRING579;
			break;
		case NCOL_MASTER_SPECIES:
			nResID = IDS_STRING580;
			break;
		case NCOL_ALKALINITY:
			nResID = IDS_STRING581;
			break;
		case NCOL_FORMULA_OR_GFW:
			if (IsDlgButtonChecked(IDC_RADIO_FORMULA))
			{
				nResID = IDS_STRING582;
			}
			else
			{
				nResID = IDS_STRING583;
			}
			break;
		case NCOL_GFW_OF_ELEMENT:
			nResID = IDS_STRING584;
			break;
		}
		if (nResID != 0)
		{
			strRes.LoadString(nResID);
		}
		else
		{
			// No help topic is associated with this item. 
			strRes.LoadString(IDS_STRING441);
		}
		break;
	default:
		// No help topic is associated with this item. 
		strRes.LoadString(IDS_STRING441);
		break;
	}
	myPopup.pszText = strRes;
	return HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}

BOOL CKPSolutionMasterSpeciesPg1::OnInitDialog() 
{
	baseCKPSolutionMasterSpeciesPg1::OnInitDialog();
	
	// Add extra initialization here

	// set layout
	CreateRoot(VERTICAL, 5, 6)
		<< (paneCtrl(IDC_STATIC_MAIN, HORIZONTAL, ABSOLUTE_VERT, nDefaultBorder, 10, 10)
			<< (pane(VERTICAL, GREEDY)
				<< (pane(HORIZONTAL, GREEDY)
					<< item(IDC_STATIC_ELEMENT_NAME, NORESIZE)
					<< itemFixed(HORIZONTAL, 24)
					<< item(IDC_STATIC_MASTER_SPECIES, NORESIZE)
					<< itemFixed(HORIZONTAL, 24)
					<< item(IDC_STATIC_ALKALINITY, NORESIZE)
					)
				<< (pane(HORIZONTAL, GREEDY)
					<< item(IDC_EDIT_ELEMENT_NAME, NORESIZE)
					<< itemFixed(HORIZONTAL, 24)
					<< item(IDC_EDIT_MASTER_SPECIES, NORESIZE)
					<< itemFixed(HORIZONTAL, 24)
					<< item(IDC_EDIT_ALKALINITY, NORESIZE)
					)
				<< (pane(HORIZONTAL, GREEDY)
					<< (paneCtrl(IDC_GB_MOLECULAR_WT, VERTICAL, ABSOLUTE_VERT, nDefaultBorder, 10, 10)
						<< (pane(HORIZONTAL, GREEDY)
							<< item(IDC_RADIO_FORMULA, NORESIZE)
							<< item(IDC_EDIT_FORMULA_OR_GFW, NORESIZE)
							)
						<< (pane(HORIZONTAL, GREEDY)
							<< item(IDC_RADIO_GFW, NORESIZE)
							)
						)
						<< itemFixed(HORIZONTAL, 54)
						<< (pane(VERTICAL, GREEDY)
							<< item(IDC_STATIC_GFW_ELEMENT, NORESIZE)
							<< item(IDC_EDIT_GFW_ELEMENT, NORESIZE)
							)
					)
				)
			)
		<< item(IDC_SRCDBPGCTRL, ABSOLUTE_VERT | ALIGN_CENTER)
		<< item(IDC_MSHFLEXGRID, GREEDY | ALIGN_CENTER)
		<< (paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, ABSOLUTE_VERT, nDefaultBorder, 10, 10)
			<< item(IDC_DESCRIPTION, GREEDY)
			)
	;
	UpdateLayout();


// COMMENT: {8/21/2001 6:40:08 PM}	// set layout
// COMMENT: {8/21/2001 6:40:08 PM}	CreateRoot(VERTICAL, 5, 6)
// COMMENT: {8/21/2001 6:40:08 PM}		<< (paneCtrl(IDC_STATIC_MAIN, HORIZONTAL, ABSOLUTE_VERT, nDefaultBorder, 10, 10)
// COMMENT: {8/21/2001 6:40:08 PM}			<< (pane(VERTICAL, GREEDY)
// COMMENT: {8/21/2001 6:40:08 PM}				<< (pane(HORIZONTAL, GREEDY)
// COMMENT: {8/21/2001 6:40:08 PM}					<< item(IDC_STATIC_ELEMENT_NAME, ABSOLUTE_VERT)
// COMMENT: {8/21/2001 6:40:08 PM}					<< item(IDC_STATIC_MASTER_SPECIES, ABSOLUTE_VERT)
// COMMENT: {8/21/2001 6:40:08 PM}					<< item(IDC_STATIC_ALKALINITY, ABSOLUTE_VERT)
// COMMENT: {8/21/2001 6:40:08 PM}					)
// COMMENT: {8/21/2001 6:40:08 PM}				<< (pane(HORIZONTAL, GREEDY)
// COMMENT: {8/21/2001 6:40:08 PM}					<< item(IDC_EDIT_ELEMENT_NAME, ABSOLUTE_VERT)
// COMMENT: {8/21/2001 6:40:08 PM}					<< item(IDC_EDIT_MASTER_SPECIES, ABSOLUTE_VERT)
// COMMENT: {8/21/2001 6:40:08 PM}					<< item(IDC_EDIT_ALKALINITY, ABSOLUTE_VERT)
// COMMENT: {8/21/2001 6:40:08 PM}					)
// COMMENT: {8/21/2001 6:40:08 PM}				<< (pane(HORIZONTAL, GREEDY)
// COMMENT: {8/21/2001 6:40:08 PM}					<< (paneCtrl(IDC_GB_MOLECULAR_WT, VERTICAL, ABSOLUTE_VERT, nDefaultBorder, 10, 10)
// COMMENT: {8/21/2001 6:40:08 PM}						<< (pane(HORIZONTAL, GREEDY)
// COMMENT: {8/21/2001 6:40:08 PM}							<< item(IDC_RADIO_FORMULA, ABSOLUTE_VERT)
// COMMENT: {8/21/2001 6:40:08 PM}							<< item(IDC_EDIT_FORMULA_OR_GFW, ABSOLUTE_VERT)
// COMMENT: {8/21/2001 6:40:08 PM}							)
// COMMENT: {8/21/2001 6:40:08 PM}						<< (pane(HORIZONTAL, GREEDY)
// COMMENT: {8/21/2001 6:40:08 PM}							<< item(IDC_RADIO_GFW, ABSOLUTE_VERT)
// COMMENT: {8/21/2001 6:40:08 PM}							)
// COMMENT: {8/21/2001 6:40:08 PM}						)
// COMMENT: {8/21/2001 6:40:08 PM}						<< (pane(VERTICAL, GREEDY)
// COMMENT: {8/21/2001 6:40:08 PM}							<< item(IDC_STATIC_GFW_ELEMENT, ABSOLUTE_VERT)
// COMMENT: {8/21/2001 6:40:08 PM}							<< item(IDC_EDIT_GFW_ELEMENT, ABSOLUTE_VERT)
// COMMENT: {8/21/2001 6:40:08 PM}							)
// COMMENT: {8/21/2001 6:40:08 PM}					)
// COMMENT: {8/21/2001 6:40:08 PM}				)
// COMMENT: {8/21/2001 6:40:08 PM}			)
// COMMENT: {8/21/2001 6:40:08 PM}		<< item(IDC_SRCDBPGCTRL, ABSOLUTE_VERT | ALIGN_CENTER)
// COMMENT: {8/21/2001 6:40:08 PM}		<< item(IDC_MSHFLEXGRID, GREEDY | ALIGN_CENTER)
// COMMENT: {8/21/2001 6:40:08 PM}		<< (paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, ABSOLUTE_VERT, nDefaultBorder, 10, 10)
// COMMENT: {8/21/2001 6:40:08 PM}			<< item(IDC_DESCRIPTION, GREEDY)
// COMMENT: {8/21/2001 6:40:08 PM}			)
// COMMENT: {8/21/2001 6:40:08 PM}	;
// COMMENT: {8/21/2001 6:40:08 PM}	UpdateLayout();

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKPSolutionMasterSpeciesPg1::OnKillfocusEditAlkalinity() 
{
	m_hWndLastControl = m_ctrlAlkalinity.m_hWnd;
	m_bEditLastControl = TRUE;	
}

void CKPSolutionMasterSpeciesPg1::OnKillfocusEditElementName() 
{
	m_hWndLastControl = m_ctrlElementName.m_hWnd;
	m_bEditLastControl = TRUE;	
}

void CKPSolutionMasterSpeciesPg1::OnKillfocusEditFormulaOrGfw() 
{
	m_hWndLastControl = m_ctrlFormulaOrGFW.m_hWnd;
	m_bEditLastControl = TRUE;	
}

void CKPSolutionMasterSpeciesPg1::OnKillfocusEditGfwElement() 
{
	m_hWndLastControl = m_ctrlGFWOfElement.m_hWnd;
	m_bEditLastControl = TRUE;	
}

void CKPSolutionMasterSpeciesPg1::OnKillfocusEditMasterSpecies() 
{
	m_hWndLastControl = m_ctrlMasterSpecies.m_hWnd;
	m_bEditLastControl = TRUE;	
}

void CKPSolutionMasterSpeciesPg1::OnLeaveCellGrid() 
{
	TRACE("OnLeaveCellGrid\n");

	long nColSave = m_ctrlGrid.GetCol();

	m_ctrlGrid.SetCol(0);
	if (m_ctrlGrid.GetRow() == m_ctrlGrid.GetRows() - 1)
	{
		m_ctrlGrid.SetCellPictureAlignment(flexAlignCenterCenter);
		m_ctrlGrid.SetRefCellPicture(m_pictHolderStar.GetPictureDispatch());
	}
	else
	{
		CPictureHolder pictHolder;
		pictHolder.CreateEmpty();
		m_ctrlGrid.SetRefCellPicture(pictHolder.GetPictureDispatch());
	}	
	m_ctrlGrid.SetCol(nColSave);
}

void CKPSolutionMasterSpeciesPg1::OnRowColChangeGrid() 
{
	TRACE("OnRowColChangeGrid\n");

	ASSERT(m_bIgnoreChanges == false);
	m_bIgnoreChanges = true;
	TRACE("Setting m_bIgnoreChanges = true in OnRowColChangeGrid\n");

	long nRow = m_ctrlGrid.GetRow();
	long nCol = m_ctrlGrid.GetCol();


	// Element name
	m_ctrlElementName.SetWindowText(m_ctrlGrid.GetTextMatrix(nRow, NCOL_ELEMENT_NAME));

	// Master species
	m_ctrlMasterSpecies.SetWindowText(m_ctrlGrid.GetTextMatrix(nRow, NCOL_MASTER_SPECIES));

	// Alkalinity
	m_ctrlAlkalinity.SetWindowText(m_ctrlGrid.GetTextMatrix(nRow, NCOL_ALKALINITY));

	// Formula or GFW
	CString strFormulaOrGFW = m_ctrlGrid.GetTextMatrix(nRow, NCOL_FORMULA_OR_GFW);
	strFormulaOrGFW.TrimLeft();
	strFormulaOrGFW.TrimRight();
	m_ctrlFormulaOrGFW.SetWindowText(strFormulaOrGFW);
	if (strFormulaOrGFW.IsEmpty())
	{
		CheckRadioButton(IDC_RADIO_FORMULA, IDC_RADIO_GFW, IDC_RADIO_FORMULA);				
	}
	else
	{
		if (::atof(strFormulaOrGFW) != 0.0 || strFormulaOrGFW[0] == _T('0'))
		{
			CheckRadioButton(IDC_RADIO_FORMULA, IDC_RADIO_GFW, IDC_RADIO_GFW);				
		}
		else
		{
			CheckRadioButton(IDC_RADIO_FORMULA, IDC_RADIO_GFW, IDC_RADIO_FORMULA);				
		}
	}

	// GFW of Element
	m_ctrlGFWOfElement.SetWindowText(m_ctrlGrid.GetTextMatrix(nRow, NCOL_GFW_OF_ELEMENT));


	// set arrow image
	m_ctrlGrid.SetCol(0);
	m_ctrlGrid.SetRefCellPicture(m_pictHolderRightArrow.GetPictureDispatch());
	m_ctrlGrid.SetCellPictureAlignment(flexAlignCenterCenter);


	// reset column
	m_ctrlGrid.SetCol(nCol);

	// update pager
	CString str;
	str.Format(_T("%d"), nRow);
	m_pager.SetCaption(str);
	if (nRow == 1)
	{
		m_pager.SetGotoPrevEnabled(FALSE);
	}
	else
	{
		m_pager.SetGotoPrevEnabled(TRUE);
	}
	if (nRow == m_ctrlGrid.GetRows() - 1)
	{
		m_pager.SetGotoNextEnabled(FALSE);
	}
	else
	{
		m_pager.SetGotoNextEnabled(TRUE);
	}

	ASSERT(m_bIgnoreChanges == true);
	m_bIgnoreChanges = false;
	TRACE("Setting m_bIgnoreChanges = false in OnRowColChangeGrid\n");
}

void CKPSolutionMasterSpeciesPg1::OnSetfocusEditAlkalinity() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING581);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPSolutionMasterSpeciesPg1::OnSetfocusEditElementName() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING579);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPSolutionMasterSpeciesPg1::OnSetfocusEditFormulaOrGfw() 
{
	CString strRes;

	if (IsDlgButtonChecked(IDC_RADIO_FORMULA))
	{
		strRes.LoadString(IDS_STRING582);
	}
	else
	{
		strRes.LoadString(IDS_STRING582);
	}
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPSolutionMasterSpeciesPg1::OnSetfocusEditGfwElement() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING584);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPSolutionMasterSpeciesPg1::OnSetfocusEditMasterSpecies() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING580);
	m_eInputDesc.SetWindowText(strRes);	
}

LRESULT CKPSolutionMasterSpeciesPg1::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
{
	UINT nID = wParam;
	UNUSED_ALWAYS(lParam);
	switch (nID)
	{
	case IDC_MSHFLEXGRID:
		OnEnterCellGrid();
		break;
	}
	return 0;
}

void CKPSolutionMasterSpeciesPg1::OnSetfocusRadio(UINT nID) 
{
	UINT nResID = nID - IDC_RADIO_FORMULA + IDS_STRING585;

	CString strRes;
	strRes.LoadString(nResID);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPSolutionMasterSpeciesPg1::ResetFocus()
{
	if (m_hWndLastControl != NULL)
	{
		// restore focus and selection last control
		::SetFocus(m_hWndLastControl);
		if (m_bEditLastControl) // select edit item
			::SendMessage(m_hWndLastControl, EM_SETSEL, 0, -1);
	}
}


void CKPSolutionMasterSpeciesPg1::OnKeyDownGrid(short FAR* KeyCode, short Shift) 
{
	// Add your control notification handler code here
	UNUSED(Shift);

	switch (*KeyCode)
	{
	case VK_DELETE:
		m_ctrlGrid.ClearContents();	
		break;
	}	
}
