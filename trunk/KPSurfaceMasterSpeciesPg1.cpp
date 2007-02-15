// KPSurfaceMasterSpeciesPg1.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "KPSurfaceMasterSpeciesPg1.h"

#include "KSSurfaceMasterSpecies.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CKPSurfaceMasterSpeciesPg1, baseCKPSurfaceMasterSpeciesPg1)

#undef IDC_STATIC_EXCHANGE_NAME
#undef IDC_EDIT_EXCHANGE_NAME
#undef IDC_STATIC_EXCHANGE_MASTER_SPECIES
#undef IDC_EDIT_EXCHANGE_MASTER_SPECIES

/////////////////////////////////////////////////////////////////////////////
// CKPSurfaceMasterSpeciesPg1 property page

const long NCOL_IMAGE           = 0;
const long NCOL_SURFACE_NAME    = 1;
const long NCOL_SMASTER_SPECIES = 2;

CKPSurfaceMasterSpeciesPg1::CKPSurfaceMasterSpeciesPg1() : baseCKPSurfaceMasterSpeciesPg1(CKPSurfaceMasterSpeciesPg1::IDD)
{
	//{{AFX_DATA_INIT(CKPSurfaceMasterSpeciesPg1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pictHolderRightArrow.CreateFromIcon(IDI_RIGHT_ARROW2);
	m_pictHolderStar.CreateFromIcon(IDI_STAR);

	m_hWndLastControl  = NULL;
	m_bEditLastControl = FALSE;

	m_bIgnoreChanges = false;
}

CKPSurfaceMasterSpeciesPg1::~CKPSurfaceMasterSpeciesPg1()
{
}

void CKPSurfaceMasterSpeciesPg1::DoDataExchange(CDataExchange* pDX)
{
	baseCKPSurfaceMasterSpeciesPg1::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKPSurfaceMasterSpeciesPg1)
	DDX_Control(pDX, IDC_EDIT_SURFACE_NAME, m_ctrlSurfaceName);
	DDX_Control(pDX, IDC_EDIT_SURFACE_MASTER_SPECIES, m_ctrlSMasterSpecies);
	DDX_Control(pDX, IDC_DESCRIPTION, m_eInputDesc);
	DDX_Control(pDX, IDC_MSHFLEXGRID, m_ctrlGrid);
	DDX_Control(pDX, IDC_SRCDBPGCTRL, m_pager);
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

			// surface binding-site name
			nCurrentTextBox = IDC_EDIT_SURFACE_NAME;
			DDX_GridText(pDX, IDC_MSHFLEXGRID, nRow, NCOL_SURFACE_NAME, mast.m_strName);
			mast.m_strName.Replace(_T(' '), _T('_'));
			if (mast.m_strName.IsEmpty()) continue;

			if (!CUtil::IsValidElementName(mast.m_strName))
			{
				m_ctrlGrid.SetRow(nRow);
				OnRowColChangeGrid();
				pDX->PrepareEditCtrl(nCurrentTextBox);
				::AfxMessageBox(_T("The surface binding-site name must start with capital letter."));
				pDX->Fail();
			}

			// master species
			nCurrentTextBox = IDC_EDIT_SURFACE_MASTER_SPECIES;
			DDX_GridText(pDX, IDC_MSHFLEXGRID, nRow, NCOL_SMASTER_SPECIES, mast.m_strMasterSpecies);

			if (mast.m_strMasterSpecies.IsEmpty() || !CUtil::IsValidMasterSpeciesName(mast.m_strMasterSpecies))
			{
				m_ctrlGrid.SetRow(nRow);
				OnRowColChangeGrid();
				pDX->PrepareEditCtrl(nCurrentTextBox);
				::AfxMessageBox(_T("Master species must start with capital letter."));
				pDX->Fail();
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

			// exchange name
			DDX_GridText(pDX, IDC_MSHFLEXGRID, nRow, NCOL_SURFACE_NAME, mast.m_strName);

			// master species
			DDX_GridText(pDX, IDC_MSHFLEXGRID, nRow, NCOL_SMASTER_SPECIES, mast.m_strMasterSpecies);
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


BEGIN_MESSAGE_MAP(CKPSurfaceMasterSpeciesPg1, baseCKPSurfaceMasterSpeciesPg1)
	//{{AFX_MSG_MAP(CKPSurfaceMasterSpeciesPg1)
	ON_EN_CHANGE(IDC_EDIT_SURFACE_NAME, OnChangeEditSurfaceName)
	ON_EN_KILLFOCUS(IDC_EDIT_SURFACE_NAME, OnKillfocusEditExchangeName)
	ON_EN_SETFOCUS(IDC_EDIT_SURFACE_NAME, OnSetfocusEditExchangeName)
	ON_EN_CHANGE(IDC_EDIT_SURFACE_MASTER_SPECIES, OnChangeEditSurfaceMasterSpecies)
	ON_EN_KILLFOCUS(IDC_EDIT_SURFACE_MASTER_SPECIES, OnKillfocusEditExchangeMasterSpecies)
	ON_EN_SETFOCUS(IDC_EDIT_SURFACE_MASTER_SPECIES, OnSetfocusEditExchangeMasterSpecies)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
	// custom edit grid notifications
	ON_MESSAGE(EGN_BEGINCELLEDIT, OnBeginCellEdit)
	ON_MESSAGE(EGN_CHANGE, OnChange)
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
	ON_MESSAGE(EGN_SETFOCUS, OnSetfocusGrid)
END_MESSAGE_MAP()



BEGIN_EVENTSINK_MAP(CKPSurfaceMasterSpeciesPg1, baseCKPSurfaceMasterSpeciesPg1)
    //{{AFX_EVENTSINK_MAP(CKPSurfaceMasterSpeciesPg1)
	ON_EVENT(CKPSurfaceMasterSpeciesPg1, IDC_SRCDBPGCTRL, 1 /* GotoFirstClicked */, OnGotoFirstClicked, VTS_NONE)
	ON_EVENT(CKPSurfaceMasterSpeciesPg1, IDC_SRCDBPGCTRL, 4 /* GotoLastClicked */, OnGotoLastClicked, VTS_NONE)
	ON_EVENT(CKPSurfaceMasterSpeciesPg1, IDC_SRCDBPGCTRL, 3 /* GotoNextClicked */, OnGotoNextClicked, VTS_NONE)
	ON_EVENT(CKPSurfaceMasterSpeciesPg1, IDC_SRCDBPGCTRL, 5 /* GotoNewClicked */, OnGotoNewClicked, VTS_NONE)
	ON_EVENT(CKPSurfaceMasterSpeciesPg1, IDC_SRCDBPGCTRL, 2 /* GotoPrevClicked */, OnGotoPrevClicked, VTS_NONE)
	ON_EVENT(CKPSurfaceMasterSpeciesPg1, IDC_MSHFLEXGRID, 70 /* RowColChange */, OnRowColChangeGrid, VTS_NONE)
	ON_EVENT(CKPSurfaceMasterSpeciesPg1, IDC_MSHFLEXGRID, 72 /* LeaveCell */, OnLeaveCellGrid, VTS_NONE)
	ON_EVENT(CKPSurfaceMasterSpeciesPg1, IDC_MSHFLEXGRID, -601 /* DblClick */, OnDblClickGrid, VTS_NONE)
	ON_EVENT(CKPSurfaceMasterSpeciesPg1, IDC_MSHFLEXGRID, 71 /* EnterCell */, OnEnterCellGrid, VTS_NONE)
	ON_EVENT(CKPSurfaceMasterSpeciesPg1, IDC_MSHFLEXGRID, -602 /* KeyDown */, OnKeyDownGrid, VTS_PI2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CKPSurfaceMasterSpeciesPg1::InitGrid(CDataExchange* pDX, int nIDC)
{
	UNUSED(pDX);
	ASSERT(nIDC == m_ctrlGrid.GetDlgCtrlID());
	UNUSED(nIDC);   // unused in release builds

	// set column widths
	m_ctrlGrid.SetColWidth(NCOL_IMAGE,           0, 300);
	m_ctrlGrid.SetColWidth(NCOL_SURFACE_NAME,   0, 2100);
	m_ctrlGrid.SetColWidth(NCOL_SMASTER_SPECIES, 0, 2000);

	// set grid titles
    m_ctrlGrid.SetTextMatrix( 0, NCOL_SURFACE_NAME,   _T("Surface binding-site name"));
    m_ctrlGrid.SetTextMatrix( 0, NCOL_SMASTER_SPECIES, _T("Surface master species"));

	// set alignment
	m_ctrlGrid.SetColAlignment(NCOL_SURFACE_NAME, flexAlignLeftCenter);
	m_ctrlGrid.SetColAlignment(NCOL_SMASTER_SPECIES, flexAlignLeftCenter);

	// set starting position
	m_ctrlGrid.SetRow(1);
	m_ctrlGrid.SetCol(1);
}

LRESULT CKPSurfaceMasterSpeciesPg1::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
{
	TRACE("OnBeginCellEdit\n");

	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bDenyEdit = FALSE;	// allow edit

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
		case NCOL_SURFACE_NAME:
			break;
		case NCOL_SMASTER_SPECIES:
			break;
		default :
			ASSERT(FALSE);
			break;
		}
		break;
	}

	return bDenyEdit;
}

LRESULT CKPSurfaceMasterSpeciesPg1::OnChange(WPARAM wParam, LPARAM lParam)
{
	TRACE("OnChange\n");

	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	ASSERT(m_bIgnoreChanges == true); // should have vbeen set in OnBeginCellEdit

	switch (nID)
	{
	case IDC_MSHFLEXGRID :
		switch ( pInfo->item.iCol )
		{
		case NCOL_SURFACE_NAME:
			m_ctrlSurfaceName.SetWindowText(pInfo->item.pszText);
			break;
		case NCOL_SMASTER_SPECIES:
			m_ctrlSMasterSpecies.SetWindowText(pInfo->item.pszText);
			break;
		default :
			ASSERT(FALSE);
			break;
		}
		break;
	}
	return 0;

}

void CKPSurfaceMasterSpeciesPg1::OnChangeEditSurfaceMasterSpecies() 
{
	TRACE("OnChangeEditSurfaceMasterSpecies\n");

	if (!m_bIgnoreChanges)
	{
		CString str;
		m_ctrlSMasterSpecies.GetWindowText(str);
		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_SMASTER_SPECIES, str);
	}
}

void CKPSurfaceMasterSpeciesPg1::OnChangeEditSurfaceName() 
{
	TRACE("OnChangeEditSurfaceName\n");

	if (!m_bIgnoreChanges)
	{
		CString str;
		m_ctrlSurfaceName.GetWindowText(str);
		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_SURFACE_NAME, str);
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

void CKPSurfaceMasterSpeciesPg1::OnDblClickGrid() 
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

LRESULT CKPSurfaceMasterSpeciesPg1::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
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
		case NCOL_SURFACE_NAME:
			if (pInfo->item.pszText == NULL)
			{
				// edit cancelled
				m_ctrlSurfaceName.SetWindowText(m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol));
			}
			else
			{
				m_ctrlSurfaceName.SetWindowText(pInfo->item.pszText);
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
		case NCOL_SMASTER_SPECIES:
			if (pInfo->item.pszText == NULL)
			{
				// edit cancelled
				m_ctrlSMasterSpecies.SetWindowText(m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol));
			}
			else
			{
				m_ctrlSMasterSpecies.SetWindowText(pInfo->item.pszText);
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

void CKPSurfaceMasterSpeciesPg1::OnEnterCellGrid() 
{
	UINT nResID = 0;
	switch (m_ctrlGrid.GetCol())
	{
	case NCOL_SURFACE_NAME:
		nResID = IDS_STRING591;
		break;

	case NCOL_SMASTER_SPECIES:
		nResID = IDS_STRING592;
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

void CKPSurfaceMasterSpeciesPg1::OnGotoFirstClicked() 
{
	TRACE("OnGotoFirstClicked\n");

	OnLeaveCellGrid();
	m_ctrlGrid.SetRow(1);
	m_ctrlGrid.GetCellLeft();
	OnRowColChangeGrid();

	ResetFocus();
}

void CKPSurfaceMasterSpeciesPg1::OnGotoLastClicked() 
{
	TRACE("OnGotoLastClicked\n");

	OnLeaveCellGrid();
	for (long nRow = 1; nRow < m_ctrlGrid.GetRows(); ++nRow)
	{
		if (m_ctrlGrid.GetTextMatrix(nRow, NCOL_SURFACE_NAME).IsEmpty())
			break;
	}
	m_ctrlGrid.SetRow(__max(nRow - 1, 1));
	m_ctrlGrid.GetCellLeft();
	OnRowColChangeGrid();
	ResetFocus();
}

void CKPSurfaceMasterSpeciesPg1::OnGotoNewClicked() 
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

	m_ctrlSurfaceName.SetFocus();
	m_ctrlSurfaceName.SetSel(0, -1);
}

void CKPSurfaceMasterSpeciesPg1::OnGotoNextClicked() 
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

void CKPSurfaceMasterSpeciesPg1::OnGotoPrevClicked() 
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

BOOL CKPSurfaceMasterSpeciesPg1::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// Declare the popup structure and initialize it.
	HH_POPUP        myPopup;
	::memset(&myPopup, 0, sizeof(HH_POPUP));
	
	// Fill in the popup structure
	myPopup.cbStruct         = sizeof(HH_POPUP);
	myPopup.rcMargins.top    = 5;
	myPopup.rcMargins.bottom = 5;
	myPopup.rcMargins.left   = 5;
	myPopup.rcMargins.right  = 5;
	myPopup.clrForeground    = (COLORREF)-1;  // use default
	myPopup.clrBackground    = (COLORREF)-1;  // use default
	myPopup.idString         = 0;             // set the ID string to 0

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
	switch (pHelpInfo->iCtrlId)
	{
	case IDC_EDIT_SURFACE_NAME: case IDC_STATIC_SURFACE_NAME:
		strRes.LoadString(IDS_STRING591);
		break;
	case IDC_EDIT_SURFACE_MASTER_SPECIES: case IDC_STATIC_SURFACE_MASTER_SPECIES:
		strRes.LoadString(IDS_STRING592);
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
		case NCOL_SURFACE_NAME:
			strRes.LoadString(IDS_STRING591);
			break;
		case NCOL_SMASTER_SPECIES:
			strRes.LoadString(IDS_STRING592);
			break;
		default:
			// No help topic is associated with this item. 
			strRes.LoadString(IDS_STRING441);
			break;
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

BOOL CKPSurfaceMasterSpeciesPg1::OnInitDialog() 
{
	baseCKPSurfaceMasterSpeciesPg1::OnInitDialog();

	// TODO: Add extra initialization here
	// set layout
	CreateRoot(VERTICAL, 5, 6)
		<< (paneCtrl(IDC_STATIC_MAIN, HORIZONTAL, ABSOLUTE_VERT, nDefaultBorder, 10, 10)
			<< (pane(VERTICAL, GREEDY)
				<< (pane(HORIZONTAL, GREEDY)
					<< item(IDC_STATIC_SURFACE_NAME, NORESIZE)
					<< itemFixed(HORIZONTAL, 24)
					<< item(IDC_STATIC_SURFACE_MASTER_SPECIES, NORESIZE)
					)
				<< (pane(HORIZONTAL, GREEDY)
					<< item(IDC_EDIT_SURFACE_NAME, NORESIZE)
					<< itemFixed(HORIZONTAL, 24)
					<< item(IDC_EDIT_SURFACE_MASTER_SPECIES, NORESIZE)
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
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKPSurfaceMasterSpeciesPg1::OnKeyDownGrid(short FAR* KeyCode, short Shift) 
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

void CKPSurfaceMasterSpeciesPg1::OnKillfocusEditExchangeMasterSpecies() 
{
	m_hWndLastControl = m_ctrlSMasterSpecies.m_hWnd;
	m_bEditLastControl = TRUE;	
}

void CKPSurfaceMasterSpeciesPg1::OnKillfocusEditExchangeName() 
{
	m_hWndLastControl = m_ctrlSurfaceName.m_hWnd;
	m_bEditLastControl = TRUE;	
}

void CKPSurfaceMasterSpeciesPg1::OnLeaveCellGrid() 
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

void CKPSurfaceMasterSpeciesPg1::OnRowColChangeGrid() 
{
	TRACE("OnRowColChangeGrid\n");

	ASSERT(m_bIgnoreChanges == false);
	m_bIgnoreChanges = true;
	TRACE("Setting m_bIgnoreChanges = true in OnRowColChangeGrid\n");

	long nRow = m_ctrlGrid.GetRow();
	long nCol = m_ctrlGrid.GetCol();


	// Exchange name
	m_ctrlSurfaceName.SetWindowText(m_ctrlGrid.GetTextMatrix(nRow, NCOL_SURFACE_NAME));

	// Exchange master species
	m_ctrlSMasterSpecies.SetWindowText(m_ctrlGrid.GetTextMatrix(nRow, NCOL_SMASTER_SPECIES));

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

void CKPSurfaceMasterSpeciesPg1::OnSetfocusEditExchangeMasterSpecies() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING592);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPSurfaceMasterSpeciesPg1::OnSetfocusEditExchangeName() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING591);
	m_eInputDesc.SetWindowText(strRes);	
}

LRESULT CKPSurfaceMasterSpeciesPg1::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
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

void CKPSurfaceMasterSpeciesPg1::ResetFocus()
{
	if (m_hWndLastControl != NULL)
	{
		// restore focus and selection last control
		::SetFocus(m_hWndLastControl);
		if (m_bEditLastControl) // select edit item
			::SendMessage(m_hWndLastControl, EM_SETSEL, 0, -1);
	}
}

