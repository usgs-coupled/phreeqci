// KPPitzerB0.cpp : implementation file
//

#include "stdafx.h"
#include "phreeqci2.h"
#include "KPPitzerAlphas.h"
#include "Util.h"

// CKPPitzerAlphas dialog

// IMPLEMENT_DYNAMIC(CKPPitzerAlphas, CPropertyPage)
IMPLEMENT_DYNCREATE(CKPPitzerAlphas, baseCKPPitzerAlphas)

const long NCOL_IMAGE  = 0;
const long NCOL_CATION = 1;
const long NCOL_ANION  = 2;
const long NCOL_ALPHA1 = 3;
const long NCOL_ALPHA2 = 4;

const bool ADD_MISSING = false;

CKPPitzerAlphas::CKPPitzerAlphas(UINT nIDTemplate, UINT nIDCaption) : baseCKPPitzerAlphas(nIDTemplate, nIDCaption)
{
	this->m_pictHolderRightArrow.CreateFromIcon(IDI_RIGHT_ARROW2);
	this->m_pictHolderStar.CreateFromIcon(IDI_STAR);

	this->m_hWndLastControl  = NULL;
	this->m_bEditLastControl = FALSE;

	this->m_bIgnoreChanges = false;
}

CKPPitzerAlphas::~CKPPitzerAlphas()
{
}

void CKPPitzerAlphas::DoDataExchange(CDataExchange* pDX)
{
	baseCKPPitzerAlphas::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CB_CATION,    this->m_ctrlCation);
	DDX_Control(pDX, IDC_CB_ANION,     this->m_ctrlAnion);

	DDX_Control(pDX, IDC_EDIT_ALPHA1,  this->m_ctrlAlpha1);
	DDX_Control(pDX, IDC_EDIT_ALPHA2,  this->m_ctrlAlpha2);

	DDX_Control(pDX, IDC_SRCDBPGCTRL1, this->m_pager);
	DDX_Control(pDX, IDC_MSHFLEXGRID1, this->m_ctrlGrid);
	DDX_Control(pDX, IDC_DESCRIPTION,  this->m_eInputDesc);

	if (this->m_bFirstSetActive)
	{
		CUtil::InsertCations(this->m_ctrlCation.GetSafeHwnd(), this->GetDatabase());
		CUtil::InsertAnions(this->m_ctrlAnion.GetSafeHwnd(), this->GetDatabase());
		this->InitGrid(pDX, IDC_MSHFLEXGRID1);
	}

	if (pDX->m_bSaveAndValidate)
	{
		CWaitCursor wait;
		std::list<CPitzParam> listParams;
		int nCurrentTextBox = 0;
		for (long nRow = 1; nRow < m_ctrlGrid.GetRows(); ++nRow)
		{
			OnLeaveCellGrid();

			CPitzParam p(TYPE_ALPHAS);

			if (m_ctrlGrid.GetTextMatrix(nRow, NCOL_CATION).IsEmpty()
				&&
				m_ctrlGrid.GetTextMatrix(nRow, NCOL_ANION).IsEmpty())
			{
				continue;
			}

			// cation
			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_CATION, p.species[0]);
			if (p.species[0].IsEmpty())
			{
				pDX->PrepareCtrl(IDC_CB_CATION);
				::AfxMessageBox("Please define a cation");
				pDX->Fail();
			}

			// anion
			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_ANION, p.species[1]);
			if (p.species[1].IsEmpty())
			{
				pDX->PrepareCtrl(IDC_CB_ANION);
				::AfxMessageBox("Please define an anion");
				pDX->Fail();
			}

			try
			{
				nCurrentTextBox = IDC_EDIT_ALPHA1;
				DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_ALPHA1, p.a[0]);
				nCurrentTextBox = IDC_EDIT_ALPHA2;
				DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_ALPHA2, p.a[1]);
			}
			catch(CUserException* pE)
			{
				OnLeaveCellGrid();
				pE->Delete();
				m_ctrlGrid.SetRow(nRow);
				OnRowColChangeGrid();
				pDX->PrepareEditCtrl(nCurrentTextBox);
				pDX->Fail();
			}

			listParams.push_back(p);
		}
		// if here list is valid and can be assigned to the member variable
		this->m_listParams.assign(listParams.begin(), listParams.end());
	}
	else
	{
		// size grid
		m_ctrlGrid.SetRows(m_listParams.size() + 2);

		std::list<CPitzParam>::const_iterator cIter = m_listParams.begin();
		for (long nRow = 1; cIter != m_listParams.end(); ++cIter, ++nRow)
		{
			CPitzParam p(*cIter);

			// cation
			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_CATION, p.species[0]);

			// anion
			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_ANION, p.species[1]);

			// -analytical_expression
			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_ALPHA1, p.a[0]);
			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_ALPHA2, p.a[1]);
		}

		m_ctrlGrid.SetCol(0);
		m_ctrlGrid.SetRow(m_listParams.size() + 1);
		m_ctrlGrid.SetCellPictureAlignment(flexAlignCenterCenter);
		m_ctrlGrid.SetRefCellPicture(m_pictHolderStar.GetPictureDispatch());

		// set initial pos
		m_ctrlGrid.SetRow(1);
		m_ctrlGrid.SetCol(1);
		OnRowColChangeGrid();

		// set initial pos
		m_ctrlGrid.SetRow(1);
		m_ctrlGrid.SetCol(1);
		OnRowColChangeGrid();
	}
}


BEGIN_MESSAGE_MAP(CKPPitzerAlphas, baseCKPPitzerAlphas)
	// custom edit grid notifications
	ON_MESSAGE(EGN_BEGINCELLEDIT, &CKPPitzerAlphas::OnBeginCellEdit)
	ON_MESSAGE(EGN_ENDCELLEDIT, &CKPPitzerAlphas::OnEndCellEdit)
	ON_MESSAGE(EGN_SETFOCUS, &CKPPitzerAlphas::OnSetfocusGrid)
	ON_MESSAGE(EGN_CHANGE, &CKPPitzerAlphas::OnChange)
	ON_MESSAGE(EGN_ALLOW_INSERT_ROW, &CKPPitzerAlphas::OnAllowInsertRow)
	ON_MESSAGE(EGN_INSERT_ROW, &CKPPitzerAlphas::OnInsertRow)
	ON_MESSAGE(EGN_ALLOW_DELETE_ROW, &CKPPitzerAlphas::OnAllowDeleteRow)
	ON_MESSAGE(EGN_DELETE_ROW, &CKPPitzerAlphas::OnDeleteRow)
	ON_CBN_EDITCHANGE(IDC_CB_CATION, &CKPPitzerAlphas::OnCbnEditchangeCbCation)
	ON_CBN_SELCHANGE(IDC_CB_CATION, &CKPPitzerAlphas::OnCbnSelchangeCbCation)
	ON_CBN_EDITCHANGE(IDC_CB_ANION, &CKPPitzerAlphas::OnCbnEditchangeCbAnion)
	ON_CBN_SELCHANGE(IDC_CB_ANION, &CKPPitzerAlphas::OnCbnSelchangeCbAnion)
END_MESSAGE_MAP()


// CKPPitzerAlphas message handlers
void CKPPitzerAlphas::InitGrid(CDataExchange* pDX, int nIDC)
{
	UNUSED(pDX);
	ASSERT(nIDC == this->m_ctrlGrid.GetDlgCtrlID());
	UNUSED(nIDC);   // unused in release builds

	// set column widths
	this->m_ctrlGrid.SetColWidth(NCOL_IMAGE, 0, 300);

	this->m_ctrlGrid.SetColWidth(NCOL_CATION, 0, 1095);
	this->m_ctrlGrid.SetColWidth(NCOL_ANION,  0, 1095);
	this->m_ctrlGrid.SetColWidth(NCOL_ALPHA1, 0, 1095);
	this->m_ctrlGrid.SetColWidth(NCOL_ALPHA2, 0, 1095);

	// set grid titles
    this->m_ctrlGrid.SetTextMatrix(0, NCOL_CATION, _T("Cation"));
    this->m_ctrlGrid.SetTextMatrix(0, NCOL_ANION,  _T("Anion"));
    this->m_ctrlGrid.SetTextMatrix(0, NCOL_ALPHA1, _T("Alpha1"));
    this->m_ctrlGrid.SetTextMatrix(0, NCOL_ALPHA2, _T("Alpha2"));
// COMMENT: {11/3/2009 9:16:04 PM}	// set alignment
// COMMENT: {11/3/2009 9:16:04 PM}	m_ctrlGrid.SetColAlignment(NCOL_RXN, flexAlignLeftCenter);
// COMMENT: {11/3/2009 9:16:04 PM}
	// set starting position
	this->m_ctrlGrid.SetRow(1);
	this->m_ctrlGrid.SetCol(1);
}

BOOL CKPPitzerAlphas::OnInitDialog()
{
	baseCKPPitzerAlphas::OnInitDialog();

	// set layout
	CreateRoot(VERTICAL, 5, 6)

		<< (paneCtrl(IDC_STATIC_MAIN, HORIZONTAL, ABSOLUTE_VERT, nDefaultBorder, 10, 10)
			<< (pane(VERTICAL, GREEDY)

				<< itemFixed(VERTICAL, 10)

				<< (pane(HORIZONTAL, GREEDY)
					<< item(IDC_ST_CATION, NORESIZE | ALIGN_CENTER)
					<< item(IDC_CB_CATION, ABSOLUTE_VERT | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_ST_ANION, NORESIZE | ALIGN_CENTER)
					<< item(IDC_CB_ANION, ABSOLUTE_VERT | ALIGN_CENTER)
					)

				<< itemFixed(VERTICAL, 10)

				<< (pane(HORIZONTAL)
					<< item(IDC_STATIC_ALPHA1, NORESIZE | ALIGN_CENTER)
					<< item(IDC_EDIT_ALPHA1, ABSOLUTE_VERT | ALIGN_CENTER)
					<< itemFixed(VERTICAL, 10)
					<< item(IDC_STATIC_ALPHA2, NORESIZE | ALIGN_CENTER)
					<< item(IDC_EDIT_ALPHA2, ABSOLUTE_VERT | ALIGN_CENTER)
					)
				)
				<< itemFixed(VERTICAL, 100)
			)
		<< item(IDC_SRCDBPGCTRL1, ABSOLUTE_VERT | ALIGN_CENTER)
		<< item(IDC_MSHFLEXGRID1, GREEDY | ALIGN_CENTER)

		<< (paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, ABSOLUTE_VERT, nDefaultBorder, 10, 10)
			<< item(IDC_DESCRIPTION, GREEDY)
			)
	;
	UpdateLayout();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CKPPitzerAlphas::OnEnChangeEditAlpha1()
{
	TRACE("OnEnChangeEditAlpha1\n");
	if (!this->m_bIgnoreChanges)
	{
		CString str;
		this->m_ctrlAlpha1.GetWindowText(str);
		this->m_ctrlGrid.SetTextMatrix(this->m_ctrlGrid.GetRow(), NCOL_ALPHA1, str);
	}
}

void CKPPitzerAlphas::OnEnChangeEditAlpha2()
{
	TRACE("OnEnChangeEditAlpha2\n");
	if (!this->m_bIgnoreChanges)
	{
		CString str;
		this->m_ctrlAlpha2.GetWindowText(str);
		this->m_ctrlGrid.SetTextMatrix(this->m_ctrlGrid.GetRow(), NCOL_ALPHA2, str);
	}
}

void CKPPitzerAlphas::OnEnSetfocusEditAlpha1()
{
	//CString strRes;
	//strRes.LoadString(IDS_STRING551);
	//m_eInputDesc.SetWindowText(strRes);
}

void CKPPitzerAlphas::OnEnSetfocusEditAlpha2()
{
	//CString strRes;
	//strRes.LoadString(IDS_STRING551);
	//m_eInputDesc.SetWindowText(strRes);
}

void CKPPitzerAlphas::OnEnKillfocusEditAlpha1()
{
	this->m_hWndLastControl = this->m_ctrlAlpha1.m_hWnd;
	this->m_bEditLastControl = TRUE;
}

void CKPPitzerAlphas::OnEnKillfocusEditAlpha2()
{
	this->m_hWndLastControl = this->m_ctrlAlpha2.m_hWnd;
	this->m_bEditLastControl = TRUE;
}

BEGIN_EVENTSINK_MAP(CKPPitzerAlphas, CKeywordPage)
	ON_EVENT(CKPPitzerAlphas, IDC_MSHFLEXGRID1, 70, CKPPitzerAlphas::OnRowColChangeGrid, VTS_NONE)
	ON_EVENT(CKPPitzerAlphas, IDC_MSHFLEXGRID1, 71, CKPPitzerAlphas::OnEnterCellGrid, VTS_NONE)
	ON_EVENT(CKPPitzerAlphas, IDC_MSHFLEXGRID1, 72, CKPPitzerAlphas::OnLeaveCellGrid, VTS_NONE)
	ON_EVENT(CKPPitzerAlphas, IDC_MSHFLEXGRID1, DISPID_DBLCLICK, CKPPitzerAlphas::OnDblClickGrid, VTS_NONE)
	ON_EVENT(CKPPitzerAlphas, IDC_MSHFLEXGRID1, DISPID_KEYDOWN, CKPPitzerAlphas::OnKeyDownGrid, VTS_PI2 VTS_I2)
	ON_EVENT(CKPPitzerAlphas, IDC_SRCDBPGCTRL1, 1, CKPPitzerAlphas::OnGotoFirstClicked, VTS_NONE)
	ON_EVENT(CKPPitzerAlphas, IDC_SRCDBPGCTRL1, 4, CKPPitzerAlphas::OnGotoLastClicked, VTS_NONE)
	ON_EVENT(CKPPitzerAlphas, IDC_SRCDBPGCTRL1, 5, CKPPitzerAlphas::OnGotoNewClicked, VTS_NONE)
	ON_EVENT(CKPPitzerAlphas, IDC_SRCDBPGCTRL1, 3, CKPPitzerAlphas::OnGotoNextClicked, VTS_NONE)
	ON_EVENT(CKPPitzerAlphas, IDC_SRCDBPGCTRL1, 2, CKPPitzerAlphas::OnGotoPrevClicked, VTS_NONE)
END_EVENTSINK_MAP()

void CKPPitzerAlphas::OnRowColChangeGrid()
{
	TRACE("OnRowColChangeGrid\n");

	ASSERT(m_bIgnoreChanges == false);
	m_bIgnoreChanges = true;
	TRACE("Setting m_bIgnoreChanges = true in OnRowColChangeGrid\n");

	long nRow = m_ctrlGrid.GetRow();
	long nCol = m_ctrlGrid.GetCol();

	// cation
	CString cat(this->m_ctrlGrid.GetTextMatrix(nRow, NCOL_CATION));
	if (cat.IsEmpty())
	{
		this->m_ctrlCation.SetCurSel(-1);
	}
	else
	{
		if (ADD_MISSING)
		{
			if (this->m_ctrlCation.FindString(-1, cat) == CB_ERR)
			{
				this->m_ctrlCation.AddString(cat);
			}
		}
		this->m_ctrlCation.SelectString(0, cat);
	}

	// anion
	CString ani(this->m_ctrlGrid.GetTextMatrix(nRow, NCOL_ANION));
	if (ani.IsEmpty())
	{
		this->m_ctrlAnion.SetCurSel(-1);
	}
	else
	{
		if (ADD_MISSING)
		{
			if (this->m_ctrlAnion.FindString(-1, ani) == CB_ERR)
			{
				this->m_ctrlAnion.AddString(ani);
			}
		}
		VERIFY(this->m_ctrlAnion.SelectString(0, ani) != CB_ERR);
	}

	// analytical expression
	this->m_ctrlAlpha1.SetWindowText(this->m_ctrlGrid.GetTextMatrix(nRow, NCOL_ALPHA1)); // implicit OnChangeEditA0
	this->m_ctrlAlpha2.SetWindowText(this->m_ctrlGrid.GetTextMatrix(nRow, NCOL_ALPHA2)); // implicit OnChangeEditA1

	// set arrow image
	this->m_ctrlGrid.SetCol(0);
	this->m_ctrlGrid.SetRefCellPicture(this->m_pictHolderRightArrow.GetPictureDispatch());
	this->m_ctrlGrid.SetCellPictureAlignment(flexAlignCenterCenter);

	// reset column
	this->m_ctrlGrid.SetCol(nCol);

	// update pager
	CString str;
	str.Format(_T("%d"), nRow);
	this->m_pager.SetCaption(str);
	if (nRow == 1)
	{
		this->m_pager.SetGotoPrevEnabled(FALSE);
	}
	else
	{
		this->m_pager.SetGotoPrevEnabled(TRUE);
	}
	if (nRow == this->m_ctrlGrid.GetRows() - 1)
	{
		this->m_pager.SetGotoNextEnabled(FALSE);
	}
	else
	{
		this->m_pager.SetGotoNextEnabled(TRUE);
	}

	ASSERT(this->m_bIgnoreChanges == true);
	this->m_bIgnoreChanges = false;
	TRACE("Setting this->m_bIgnoreChanges = false in OnRowColChangeGrid\n");
}

void CKPPitzerAlphas::OnEnterCellGrid()
{
	UINT nResID = 0;
	switch (this->m_ctrlGrid.GetCol())
	{
	case NCOL_ALPHA1:
		//nResID = IDS_STRING551;
		break;
	case NCOL_ALPHA2:
		//nResID = IDS_STRING551;
		break;
	}

	if (nResID != 0)
	{
		CString strRes;
		strRes.LoadString(nResID);
		this->m_eInputDesc.SetWindowText(strRes);
	}
	else
	{
		this->m_eInputDesc.SetWindowText(_T(""));
	}

}

void CKPPitzerAlphas::OnLeaveCellGrid()
{
	TRACE("OnLeaveCellGrid\n");

	long nColSave = this->m_ctrlGrid.GetCol();

	CPictureHolder pictHolder;
	pictHolder.CreateEmpty();

	this->m_ctrlGrid.SetCol(0);
	if (this->m_ctrlGrid.GetRow() == this->m_ctrlGrid.GetRows() - 1)
	{
		this->m_ctrlGrid.SetCellPictureAlignment(flexAlignCenterCenter);
		this->m_ctrlGrid.SetRefCellPicture(this->m_pictHolderStar.GetPictureDispatch());
	}
	else
	{
		this->m_ctrlGrid.SetRefCellPicture(pictHolder.GetPictureDispatch());
	}
	this->m_ctrlGrid.SetCol(nColSave);
}

void CKPPitzerAlphas::OnDblClickGrid()
{
	if (!this->m_ctrlGrid.GetMouseRow())
	{
		// resize column to widest or to fit WWW
		long nCol = this->m_ctrlGrid.GetMouseCol();
		if (nCol == 0) return;

		CDC* pDC = this->m_ctrlGrid.GetDC();
		int nPixelsPerInchX = pDC->GetDeviceCaps(LOGPIXELSX);

		CSize size;
		long nWidest = pDC->GetTextExtent(_T("WWW"), 3).cx;
		for (long i = 0; i < this->m_ctrlGrid.GetRows(); ++i)
		{
			size = pDC->GetTextExtent(this->m_ctrlGrid.GetTextMatrix(i, nCol));
			if (size.cx > nWidest)
			{
				nWidest = size.cx;
			}
		}
		// limit col width to client rect minus imagewidth(col 0)
		CRect rect;
		this->m_ctrlGrid.GetClientRect(&rect);
		long nImageWidth = ::MulDiv(nPixelsPerInchX, this->m_ctrlGrid.GetColWidth(NCOL_IMAGE, 0), TWIPS_PER_INCH);
		nWidest = __min(nWidest, rect.right - 20 - nImageWidth);

		ASSERT(this->m_ctrlGrid.GetColWidth(NCOL_IMAGE, 0) >= 0);
		if (nWidest > 5)
		{
			this->m_ctrlGrid.SetColWidth(nCol, 0, ::MulDiv(nWidest, TWIPS_PER_INCH, nPixelsPerInchX));
		}
	}
}

void CKPPitzerAlphas::OnKeyDownGrid(short* KeyCode, short Shift)
{
	// Add your control notification handler code here
	UNUSED(Shift);

	switch (*KeyCode)
	{
	case VK_DELETE:
		this->m_ctrlGrid.ClearContents();
		break;
	}
}

void CKPPitzerAlphas::OnGotoFirstClicked()
{
	TRACE("OnGotoFirstClicked\n");

	this->OnLeaveCellGrid();
	this->m_ctrlGrid.SetRow(1);
	this->m_ctrlGrid.GetCellLeft();
	this->OnRowColChangeGrid();

	this->ResetFocus();
}

void CKPPitzerAlphas::OnGotoLastClicked()
{
	TRACE("OnGotoLastClicked\n");

	this->OnLeaveCellGrid();
	long nRow = 1;
	for (; nRow < this->m_ctrlGrid.GetRows(); ++nRow)
	{
		if (this->m_ctrlGrid.GetTextMatrix(nRow, NCOL_CATION).IsEmpty())
			break;
	}
	this->m_ctrlGrid.SetRow(__max(nRow - 1, 1));
	this->m_ctrlGrid.GetCellLeft();
	this->OnRowColChangeGrid();
	this->ResetFocus();
}

void CKPPitzerAlphas::OnGotoNewClicked()
{
	TRACE("OnGotoNewClicked\n");

	this->OnLeaveCellGrid();

	if (this->m_ctrlGrid.GetRows() == this->m_ctrlGrid.GetRow() + 1)
	{
		this->m_ctrlGrid.SetRows(this->m_ctrlGrid.GetRows() + 1);
	}
	long nRow = this->m_ctrlGrid.GetRows() - 1;

	this->OnLeaveCellGrid();
	this->m_ctrlGrid.SetRow(__max(nRow, 1));
	this->m_ctrlGrid.GetCellLeft();
	this->OnRowColChangeGrid();

	// TODO
// COMMENT: {11/3/2009 11:25:26 PM}	this->m_ctrlAssocRxn.SetFocus();
// COMMENT: {11/3/2009 11:25:26 PM}	this->m_ctrlAssocRxn.SetSel(0, -1);
}

void CKPPitzerAlphas::OnGotoNextClicked()
{
	TRACE("OnGotoNextClicked\n");

	long nRow = this->m_ctrlGrid.GetRow();
	if (nRow != this->m_ctrlGrid.GetRows() - 1)
	{
		this->OnLeaveCellGrid();
		this->m_ctrlGrid.SetRow(__min(nRow + 1, this->m_ctrlGrid.GetRows() - 1));
		this->m_ctrlGrid.GetCellLeft();
		this->OnRowColChangeGrid();
	}

	this->ResetFocus();
}

void CKPPitzerAlphas::OnGotoPrevClicked()
{
	TRACE("OnGotoPrevClicked\n");

	long nRow = this->m_ctrlGrid.GetRow();
	if (nRow != 1)
	{
		this->OnLeaveCellGrid();
		this->m_ctrlGrid.SetRow(__max(nRow - 1, 1));
		this->m_ctrlGrid.GetCellLeft(); // scroll into view
		this->OnRowColChangeGrid();
	}

	this->ResetFocus();
}

void CKPPitzerAlphas::ResetFocus()
{
	if (this->m_hWndLastControl != NULL)
	{
		// restore focus and selection last control
		::SetFocus(this->m_hWndLastControl);
		if (this->m_bEditLastControl)
		{
			// select edit item
			::SendMessage(this->m_hWndLastControl, EM_SETSEL, 0, -1);
		}
	}
}

LRESULT CKPPitzerAlphas::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
{
	TRACE("OnBeginCellEdit\n");

	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bDenyEdit = FALSE;	// allow edit

// COMMENT: {11/3/2009 11:39:35 PM}	const LPCTSTR arrStrCheckEqnOptions[] = { _T("Yes"), _T("No") };
// COMMENT: {11/3/2009 11:39:35 PM}
	if (pInfo->item.hWndCombo)
	{
		ASSERT(this->m_bIgnoreChanges == false);
		this->m_bIgnoreChanges = true;
		TRACE("Setting this->m_bIgnoreChanges = true in OnBeginCellEdit\n");
	}

	switch (nID)
	{
	case IDC_MSHFLEXGRID1 :
		switch ( pInfo->item.iCol )
		{
		case NCOL_CATION:
			pInfo->item.bUseCombo = (CUtil::InsertCations(pInfo->item.hWndCombo, GetDatabase()) > 0);
			break;

		case NCOL_ANION:
			pInfo->item.bUseCombo = (CUtil::InsertAnions(pInfo->item.hWndCombo, GetDatabase()) > 0);
			break;

		case NCOL_ALPHA1: case NCOL_ALPHA2:
			break;

		default :
			ASSERT(FALSE);
			break;
		}
		break;
	}

	return bDenyEdit;
}

LRESULT CKPPitzerAlphas::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
{
	TRACE("OnEndCellEdit\n");

	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	UNUSED(pInfo);

	BOOL bMakeChange = TRUE;

	if (!this->m_bIgnoreChanges)
	{
		// this is a cut or paste
		this->m_bIgnoreChanges = true;
	}

	if (pInfo->item.iRow != this->m_ctrlGrid.GetRow())
	{
		this->m_bIgnoreChanges = false;
		return bMakeChange;
	}

	switch (nID)
	{
	case IDC_MSHFLEXGRID1 :
		switch ( pInfo->item.iCol )
		{
		case NCOL_CATION:
			break;

		case NCOL_ANION:
			break;

		case NCOL_ALPHA1: // alpha1
			if (pInfo->item.pszText == NULL)
			{
				// edit cancelled
				this->m_ctrlAlpha1.SetWindowText(this->m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol));
			}
			else
			{
				this->m_ctrlAlpha1.SetWindowText(pInfo->item.pszText);
			}
			break;


		case NCOL_ALPHA2: // alpha2
			if (pInfo->item.pszText == NULL)
			{
				// edit cancelled
				this->m_ctrlAlpha2.SetWindowText(this->m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol));
			}
			else
			{
				this->m_ctrlAlpha2.SetWindowText(pInfo->item.pszText);
			}
			break;

		default:
			ASSERT(FALSE);
			break;
		}
		break;
	}

	ASSERT(this->m_bIgnoreChanges == true);
	this->m_bIgnoreChanges = false;
	TRACE("Setting this->m_bIgnoreChanges = false in OnEndCellEdit\n");

	return bMakeChange;
}

LRESULT CKPPitzerAlphas::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
{
	UINT nID = wParam;
	UNUSED_ALWAYS(lParam);
	switch (nID)
	{
	case IDC_MSHFLEXGRID1:
		OnEnterCellGrid();
		break;
	}
	return 0;
}

LRESULT CKPPitzerAlphas::OnChange(WPARAM wParam, LPARAM lParam)
{
	TRACE("CKPSolutionSpeciesPg1::OnChange\n");

	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	ASSERT(m_bIgnoreChanges == true); // should have been set in OnBeginCellEdit

	switch (nID)
	{
	case IDC_MSHFLEXGRID1 :
		switch ( pInfo->item.iCol )
		{
		case NCOL_ALPHA1:  // A0
			m_ctrlAlpha1.SetWindowText(pInfo->item.pszText);
			break;

		case NCOL_ALPHA2:  // A1
			m_ctrlAlpha2.SetWindowText(pInfo->item.pszText);
			break;

// COMMENT: {11/6/2009 6:56:46 PM}		default :
// COMMENT: {11/6/2009 6:56:46 PM}			ASSERT(FALSE);
// COMMENT: {11/6/2009 6:56:46 PM}			break;
		}
		break;
	}
	return 0;

}

LRESULT CKPPitzerAlphas::OnAllowDeleteRow(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	ASSERT(nID == IDC_MSHFLEXGRID1);

	return TRUE;  // allow
}

LRESULT CKPPitzerAlphas::OnDeleteRow(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	ASSERT(nID == IDC_MSHFLEXGRID1);

	m_ctrlGrid.DeleteRow(pInfo->item.iRow);
	return TRUE; // handled locally
}

LRESULT CKPPitzerAlphas::OnAllowInsertRow(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	ASSERT(nID == IDC_MSHFLEXGRID1);

	return FALSE;  // disallow for now
}

LRESULT CKPPitzerAlphas::OnInsertRow(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	ASSERT(nID == IDC_MSHFLEXGRID1);

	ASSERT(FALSE); // OnAllowInsertRow disallowed

	return TRUE; // handled locally
}

void CKPPitzerAlphas::OnCbnEditchangeCbCation()
{
	TRACE("OnCbnEditchangeCbCation\n");
	if (!this->m_bIgnoreChanges)
	{
		CString str;
		this->m_ctrlCation.GetWindowText(str);
		this->m_ctrlGrid.SetTextMatrix(this->m_ctrlGrid.GetRow(), NCOL_CATION, str);
	}
}

void CKPPitzerAlphas::OnCbnSelchangeCbCation()
{
	TRACE("OnCbnSelchangeCbCation\n");
	if (!this->m_bIgnoreChanges)
	{
		CString str;
		this->m_ctrlCation.GetLBText(this->m_ctrlCation.GetCurSel(), str);
		this->m_ctrlGrid.SetTextMatrix(this->m_ctrlGrid.GetRow(), NCOL_CATION, str);
	}
}

void CKPPitzerAlphas::OnCbnEditchangeCbAnion()
{
	TRACE("OnCbnEditchangeCbCation\n");
	if (!this->m_bIgnoreChanges)
	{
		CString str;
		this->m_ctrlAnion.GetWindowText(str);
		this->m_ctrlGrid.SetTextMatrix(this->m_ctrlGrid.GetRow(), NCOL_ANION, str);
	}
}

void CKPPitzerAlphas::OnCbnSelchangeCbAnion()
{
	TRACE("OnCbnSelchangeCbCation\n");
	if (!this->m_bIgnoreChanges)
	{
		CString str;
		this->m_ctrlAnion.GetLBText(this->m_ctrlAnion.GetCurSel(), str);
		this->m_ctrlGrid.SetTextMatrix(this->m_ctrlGrid.GetRow(), NCOL_ANION, str);
	}
}
