// KPPitzerB0.cpp : implementation file
//

#include "stdafx.h"
#include "phreeqci2.h"
#include "KPPitzerXX.h"
#include "Util.h"

// CKPPitzerXX dialog

// IMPLEMENT_DYNAMIC(CKPPitzerXX, CPropertyPage)
IMPLEMENT_DYNCREATE(CKPPitzerXX, baseCKPPitzerXX)

const long NCOL_IMAGE          = 0;
const long NCOL_X1             = 1;
const long NCOL_X2             = 2;
const long NCOL_A0             = 3;
const long NCOL_A1             = 4;
const long NCOL_A2             = 5;
const long NCOL_A3             = 6;
const long NCOL_A4             = 7;
const long NCOL_A5             = 8;

const bool ADD_MISSING = false;

CKPPitzerXX::CKPPitzerXX(UINT nIDTemplate, UINT nIDCaption) : baseCKPPitzerXX(nIDTemplate, nIDCaption)
{
	this->m_pictHolderRightArrow.CreateFromIcon(IDI_RIGHT_ARROW2);
	this->m_pictHolderStar.CreateFromIcon(IDI_STAR);

	this->m_hWndLastControl  = NULL;
	this->m_bEditLastControl = FALSE;

	this->m_bIgnoreChanges = false;

	this->m_ctrlGrid.SetSortedCombos(true);
}

CKPPitzerXX::~CKPPitzerXX()
{
}

void CKPPitzerXX::DoDataExchange(CDataExchange* pDX)
{
	baseCKPPitzerXX::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CB_X1, this->m_ctrlX1);
	DDX_Control(pDX, IDC_CB_X2, this->m_ctrlX2);

	DDX_Control(pDX, IDC_EDIT_A0, this->m_ctrlA0);
	DDX_Control(pDX, IDC_EDIT_A1, this->m_ctrlA1);
	DDX_Control(pDX, IDC_EDIT_A2, this->m_ctrlA2);
	DDX_Control(pDX, IDC_EDIT_A3, this->m_ctrlA3);
	DDX_Control(pDX, IDC_EDIT_A4, this->m_ctrlA4);
	DDX_Control(pDX, IDC_EDIT_A5, this->m_ctrlA5);
	DDX_Control(pDX, IDC_SRCDBPGCTRL1, this->m_pager);
	DDX_Control(pDX, IDC_MSHFLEXGRID1, this->m_ctrlGrid);
	DDX_Control(pDX, IDC_DESCRIPTION, this->m_eInputDesc);

	if (this->m_bFirstSetActive)
	{
		CUtil::InsertCationsAnions(this->m_ctrlX1.GetSafeHwnd(), this->GetDatabase());
		CUtil::InsertCationsAnions(this->m_ctrlX2.GetSafeHwnd(), this->GetDatabase());
		this->InitGrid(pDX, IDC_MSHFLEXGRID1);

		if (CWnd* pWnd = this->GetDlgItem(IDC_STATIC_ANAL))
		{
			CString str;
			str.Format(_T("Analytical expression, %s = A0 + A1(1/TK - 1/TR) + A2log(TK/TR) + A3(TK-TR) + A4(TK^2 - TR^2) + A5(1/TK^2 - 1/TR^2)"), this->m_strCaption);
			pWnd->SetWindowTextA(str);
		}
	}

	if (pDX->m_bSaveAndValidate)
	{
		CWaitCursor wait;
		std::list<CPitzParam> listParams;
		int nCurrentTextBox = 0;
		for (long nRow = 1; nRow < m_ctrlGrid.GetRows(); ++nRow)
		{
			OnLeaveCellGrid();

			CPitzParam p(TYPE_THETA);

			if (m_ctrlGrid.GetTextMatrix(nRow, NCOL_X1).IsEmpty()
				&&
				m_ctrlGrid.GetTextMatrix(nRow, NCOL_X2).IsEmpty()
				)
			{
				continue;
			}

			// X1
			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_X1, p.species[0]);
			if (p.species[0].IsEmpty())
			{
				pDX->PrepareCtrl(IDC_CB_CATION);
				::AfxMessageBox("Please define a cation/anion");
				pDX->Fail();
			}

			// X2
			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_X2, p.species[1]);
			if (p.species[1].IsEmpty())
			{
				pDX->PrepareCtrl(IDC_CB_CATION);
				::AfxMessageBox("Please define a cation/anion");
				pDX->Fail();
			}

			try
			{
				nCurrentTextBox = IDC_EDIT_A0;
				DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_A0, p.a[0]);
				nCurrentTextBox = IDC_EDIT_A1;
				DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_A1, p.a[1]);
				nCurrentTextBox = IDC_EDIT_A2;
				DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_A2, p.a[2]);
				nCurrentTextBox = IDC_EDIT_A3;
				DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_A3, p.a[3]);
				nCurrentTextBox = IDC_EDIT_A4;
				DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_A4, p.a[4]);
				nCurrentTextBox = IDC_EDIT_A5;
				DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_A5, p.a[5]);
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

			// X1
			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_X1, p.species[0]);

			// X2
			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_X2, p.species[1]);

			// -analytical_expression
			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_A0, p.a[0]);
			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_A1, p.a[1]);
			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_A2, p.a[2]);
			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_A3, p.a[3]);
			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_A4, p.a[4]);
			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_A5, p.a[5]);
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


BEGIN_MESSAGE_MAP(CKPPitzerXX, baseCKPPitzerXX)
	ON_EN_CHANGE(IDC_EDIT_A0, &CKPPitzerXX::OnEnChangeEditA0)
	ON_EN_CHANGE(IDC_EDIT_A1, &CKPPitzerXX::OnEnChangeEditA1)
	ON_EN_CHANGE(IDC_EDIT_A2, &CKPPitzerXX::OnEnChangeEditA2)
	ON_EN_CHANGE(IDC_EDIT_A3, &CKPPitzerXX::OnEnChangeEditA3)
	ON_EN_CHANGE(IDC_EDIT_A4, &CKPPitzerXX::OnEnChangeEditA4)
	ON_EN_CHANGE(IDC_EDIT_A5, &CKPPitzerXX::OnEnChangeEditA5)
	ON_EN_SETFOCUS(IDC_EDIT_A0, &CKPPitzerXX::OnEnSetfocusEditA0)
	ON_EN_SETFOCUS(IDC_EDIT_A1, &CKPPitzerXX::OnEnSetfocusEditA1)
	ON_EN_SETFOCUS(IDC_EDIT_A2, &CKPPitzerXX::OnEnSetfocusEditA2)
	ON_EN_SETFOCUS(IDC_EDIT_A3, &CKPPitzerXX::OnEnSetfocusEditA3)
	ON_EN_SETFOCUS(IDC_EDIT_A4, &CKPPitzerXX::OnEnSetfocusEditA4)
	ON_EN_SETFOCUS(IDC_EDIT_A5, &CKPPitzerXX::OnEnSetfocusEditA5)
	ON_EN_KILLFOCUS(IDC_EDIT_A0, &CKPPitzerXX::OnEnKillfocusEditA0)
	ON_EN_KILLFOCUS(IDC_EDIT_A1, &CKPPitzerXX::OnEnKillfocusEditA1)
	ON_EN_KILLFOCUS(IDC_EDIT_A2, &CKPPitzerXX::OnEnKillfocusEditA2)
	ON_EN_KILLFOCUS(IDC_EDIT_A3, &CKPPitzerXX::OnEnKillfocusEditA3)
	ON_EN_KILLFOCUS(IDC_EDIT_A4, &CKPPitzerXX::OnEnKillfocusEditA4)
	ON_EN_KILLFOCUS(IDC_EDIT_A5, &CKPPitzerXX::OnEnKillfocusEditA5)
	// custom edit grid notifications
	ON_MESSAGE(EGN_BEGINCELLEDIT, &CKPPitzerXX::OnBeginCellEdit)
	ON_MESSAGE(EGN_ENDCELLEDIT, &CKPPitzerXX::OnEndCellEdit)
	ON_MESSAGE(EGN_SETFOCUS, &CKPPitzerXX::OnSetfocusGrid)
	ON_MESSAGE(EGN_CHANGE, &CKPPitzerXX::OnChange)
	ON_MESSAGE(EGN_ALLOW_INSERT_ROW, &CKPPitzerXX::OnAllowInsertRow)
	ON_MESSAGE(EGN_INSERT_ROW, &CKPPitzerXX::OnInsertRow)
	ON_MESSAGE(EGN_ALLOW_DELETE_ROW, &CKPPitzerXX::OnAllowDeleteRow)
	ON_MESSAGE(EGN_DELETE_ROW, &CKPPitzerXX::OnDeleteRow)
	// ComboBox notifications
	ON_CBN_EDITCHANGE(IDC_CB_X1, &CKPPitzerXX::OnCbnEditchangeCbX1)
	ON_CBN_SELCHANGE(IDC_CB_X1, &CKPPitzerXX::OnCbnSelchangeCbX1)
	ON_CBN_EDITCHANGE(IDC_CB_X2, &CKPPitzerXX::OnCbnEditchangeCbX2)
	ON_CBN_SELCHANGE(IDC_CB_X2, &CKPPitzerXX::OnCbnSelchangeCbX2)
	ON_CBN_SETFOCUS(IDC_CB_X1, &CKPPitzerXX::OnCbnSetfocusCbX1)
	ON_CBN_SETFOCUS(IDC_CB_X2, &CKPPitzerXX::OnCbnSetfocusCbX2)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CKPPitzerXX message handlers
void CKPPitzerXX::InitGrid(CDataExchange* pDX, int nIDC)
{
	UNUSED(pDX);
	ASSERT(nIDC == this->m_ctrlGrid.GetDlgCtrlID());
	UNUSED(nIDC);   // unused in release builds

	// set column widths
	this->m_ctrlGrid.SetColWidth(NCOL_IMAGE, 0, 300);

	this->m_ctrlGrid.SetColWidth(NCOL_X1, 0, 1095);
	this->m_ctrlGrid.SetColWidth(NCOL_X2, 0, 1095);
	this->m_ctrlGrid.SetColWidth(NCOL_A0, 0, 1095);
	this->m_ctrlGrid.SetColWidth(NCOL_A1, 0, 1095);
	this->m_ctrlGrid.SetColWidth(NCOL_A2, 0, 1095);
	this->m_ctrlGrid.SetColWidth(NCOL_A3, 0, 1095);
	this->m_ctrlGrid.SetColWidth(NCOL_A4, 0, 1095);
	this->m_ctrlGrid.SetColWidth(NCOL_A4, 0, 1095);

	// set grid titles
    this->m_ctrlGrid.SetTextMatrix(0, NCOL_X1, _T("C/A"));
    this->m_ctrlGrid.SetTextMatrix(0, NCOL_X2, _T("C/A"));
    this->m_ctrlGrid.SetTextMatrix(0, NCOL_A0, _T("A0"));
    this->m_ctrlGrid.SetTextMatrix(0, NCOL_A1, _T("A1"));
    this->m_ctrlGrid.SetTextMatrix(0, NCOL_A2, _T("A2"));
    this->m_ctrlGrid.SetTextMatrix(0, NCOL_A3, _T("A3"));
    this->m_ctrlGrid.SetTextMatrix(0, NCOL_A4, _T("A4"));
    this->m_ctrlGrid.SetTextMatrix(0, NCOL_A5, _T("A5"));

	// set starting position
	this->m_ctrlGrid.SetRow(1);
	this->m_ctrlGrid.SetCol(1);
}

BOOL CKPPitzerXX::OnInitDialog()
{
	baseCKPPitzerXX::OnInitDialog();

	// set layout
	CreateRoot(VERTICAL, 5, 6)

		<< (paneCtrl(IDC_STATIC_MAIN, HORIZONTAL, ABSOLUTE_VERT, nDefaultBorder, 10, 10)
			<< (pane(VERTICAL, GREEDY)

				<< (pane(HORIZONTAL, GREEDY)
					<< item(IDC_ST_X1, NORESIZE | ALIGN_CENTER)
					<< item(IDC_CB_X1, ABSOLUTE_VERT | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_ST_X2, NORESIZE | ALIGN_CENTER)
					<< item(IDC_CB_X2, ABSOLUTE_VERT | ALIGN_CENTER)
					)

				<< (paneCtrl(IDC_STATIC_ANAL, VERTICAL, ABSOLUTE_VERT, nDefaultBorder, /*sizeExtraBorder*/10, /*sizeTopExtra*/10, /*sizeSecondary*/0)
					<< (pane(HORIZONTAL)
						<< item(IDC_STATIC_A0, NORESIZE | ALIGN_CENTER)
						<< item(IDC_EDIT_A0, ABSOLUTE_VERT | ALIGN_CENTER)
						<< item(IDC_STATIC_A1, NORESIZE | ALIGN_CENTER)
						<< item(IDC_EDIT_A1, ABSOLUTE_VERT | ALIGN_CENTER)
						<< item(IDC_STATIC_A2, NORESIZE | ALIGN_CENTER)
						<< item(IDC_EDIT_A2, ABSOLUTE_VERT | ALIGN_CENTER)
						<< item(IDC_STATIC_A3, NORESIZE | ALIGN_CENTER)
						<< item(IDC_EDIT_A3, ABSOLUTE_VERT | ALIGN_CENTER)
						<< item(IDC_STATIC_A4, NORESIZE | ALIGN_CENTER)
						<< item(IDC_EDIT_A4, ABSOLUTE_VERT | ALIGN_CENTER)
						<< item(IDC_STATIC_A5, NORESIZE | ALIGN_CENTER)
						<< item(IDC_EDIT_A5, ABSOLUTE_VERT | ALIGN_CENTER)
						)
					)
				)
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

void CKPPitzerXX::OnEnChangeEditA0()
{
	TRACE("OnEnChangeEditA0\n");
	if (!this->m_bIgnoreChanges)
	{
		CString str;
		this->m_ctrlA0.GetWindowText(str);
		this->m_ctrlGrid.SetTextMatrix(this->m_ctrlGrid.GetRow(), NCOL_A0, str);
	}
}

void CKPPitzerXX::OnEnChangeEditA1()
{
	TRACE("OnEnChangeEditA1\n");
	if (!this->m_bIgnoreChanges)
	{
		CString str;
		this->m_ctrlA1.GetWindowText(str);
		this->m_ctrlGrid.SetTextMatrix(this->m_ctrlGrid.GetRow(), NCOL_A1, str);
	}
}

void CKPPitzerXX::OnEnChangeEditA2()
{
	TRACE("OnEnChangeEditA2\n");
	if (!this->m_bIgnoreChanges)
	{
		CString str;
		this->m_ctrlA2.GetWindowText(str);
		this->m_ctrlGrid.SetTextMatrix(this->m_ctrlGrid.GetRow(), NCOL_A2, str);
	}
}

void CKPPitzerXX::OnEnChangeEditA3()
{
	TRACE("OnEnChangeEditA3\n");
	if (!this->m_bIgnoreChanges)
	{
		CString str;
		this->m_ctrlA3.GetWindowText(str);
		this->m_ctrlGrid.SetTextMatrix(this->m_ctrlGrid.GetRow(), NCOL_A3, str);
	}
}

void CKPPitzerXX::OnEnChangeEditA4()
{
	TRACE("OnEnChangeEditA4\n");
	if (!this->m_bIgnoreChanges)
	{
		CString str;
		this->m_ctrlA4.GetWindowText(str);
		this->m_ctrlGrid.SetTextMatrix(this->m_ctrlGrid.GetRow(), NCOL_A4, str);
	}
}

void CKPPitzerXX::OnEnChangeEditA5()
{
	TRACE("OnEnChangeEditA5\n");
	if (!this->m_bIgnoreChanges)
	{
		CString str;
		this->m_ctrlA5.GetWindowText(str);
		this->m_ctrlGrid.SetTextMatrix(this->m_ctrlGrid.GetRow(), NCOL_A5, str);
	}
}

void CKPPitzerXX::OnEnSetfocusEditA0()
{
	CString strRes;
	strRes.FormatMessage(IDS_STRING702, _T("A0"), this->m_strCaption);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPPitzerXX::OnEnSetfocusEditA1()
{
	CString strRes;
	strRes.FormatMessage(IDS_STRING702, _T("A1"), this->m_strCaption);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPPitzerXX::OnEnSetfocusEditA2()
{
	CString strRes;
	strRes.FormatMessage(IDS_STRING702, _T("A2"), this->m_strCaption);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPPitzerXX::OnEnSetfocusEditA3()
{
	CString strRes;
	strRes.FormatMessage(IDS_STRING702, _T("A3"), this->m_strCaption);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPPitzerXX::OnEnSetfocusEditA4()
{
	CString strRes;
	strRes.FormatMessage(IDS_STRING702, _T("A4"), this->m_strCaption);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPPitzerXX::OnEnSetfocusEditA5()
{
	CString strRes;
	strRes.FormatMessage(IDS_STRING702, _T("A5"), this->m_strCaption);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPPitzerXX::OnEnKillfocusEditA0()
{
	this->m_hWndLastControl = this->m_ctrlA0.m_hWnd;
	this->m_bEditLastControl = TRUE;
}

void CKPPitzerXX::OnEnKillfocusEditA1()
{
	this->m_hWndLastControl = this->m_ctrlA1.m_hWnd;
	this->m_bEditLastControl = TRUE;
}

void CKPPitzerXX::OnEnKillfocusEditA2()
{
	this->m_hWndLastControl = this->m_ctrlA2.m_hWnd;
	this->m_bEditLastControl = TRUE;
}

void CKPPitzerXX::OnEnKillfocusEditA3()
{
	this->m_hWndLastControl = this->m_ctrlA3.m_hWnd;
	this->m_bEditLastControl = TRUE;
}

void CKPPitzerXX::OnEnKillfocusEditA4()
{
	this->m_hWndLastControl = this->m_ctrlA4.m_hWnd;
	this->m_bEditLastControl = TRUE;
}

void CKPPitzerXX::OnEnKillfocusEditA5()
{
	this->m_hWndLastControl = this->m_ctrlA5.m_hWnd;
	this->m_bEditLastControl = TRUE;
}

BEGIN_EVENTSINK_MAP(CKPPitzerXX, CKeywordPage)
	ON_EVENT(CKPPitzerXX, IDC_MSHFLEXGRID1, 70, CKPPitzerXX::OnRowColChangeGrid, VTS_NONE)
	ON_EVENT(CKPPitzerXX, IDC_MSHFLEXGRID1, 71, CKPPitzerXX::OnEnterCellGrid, VTS_NONE)
	ON_EVENT(CKPPitzerXX, IDC_MSHFLEXGRID1, 72, CKPPitzerXX::OnLeaveCellGrid, VTS_NONE)
	ON_EVENT(CKPPitzerXX, IDC_MSHFLEXGRID1, DISPID_DBLCLICK, CKPPitzerXX::OnDblClickGrid, VTS_NONE)
	ON_EVENT(CKPPitzerXX, IDC_MSHFLEXGRID1, DISPID_KEYDOWN, CKPPitzerXX::OnKeyDownGrid, VTS_PI2 VTS_I2)
	ON_EVENT(CKPPitzerXX, IDC_SRCDBPGCTRL1, 1, CKPPitzerXX::OnGotoFirstClicked, VTS_NONE)
	ON_EVENT(CKPPitzerXX, IDC_SRCDBPGCTRL1, 4, CKPPitzerXX::OnGotoLastClicked, VTS_NONE)
	ON_EVENT(CKPPitzerXX, IDC_SRCDBPGCTRL1, 5, CKPPitzerXX::OnGotoNewClicked, VTS_NONE)
	ON_EVENT(CKPPitzerXX, IDC_SRCDBPGCTRL1, 3, CKPPitzerXX::OnGotoNextClicked, VTS_NONE)
	ON_EVENT(CKPPitzerXX, IDC_SRCDBPGCTRL1, 2, CKPPitzerXX::OnGotoPrevClicked, VTS_NONE)
END_EVENTSINK_MAP()

void CKPPitzerXX::OnRowColChangeGrid()
{
	TRACE("OnRowColChangeGrid\n");

	ASSERT(m_bIgnoreChanges == false);
	m_bIgnoreChanges = true;
	TRACE("Setting m_bIgnoreChanges = true in OnRowColChangeGrid\n");

	long nRow = m_ctrlGrid.GetRow();
	long nCol = m_ctrlGrid.GetCol();

	// X1
	CString x1(this->m_ctrlGrid.GetTextMatrix(nRow, NCOL_X1));
	if (x1.IsEmpty())
	{
		this->m_ctrlX1.SetCurSel(-1);
	}
	else
	{
		if (ADD_MISSING)
		{
			if (this->m_ctrlX1.FindString(-1, x1) == CB_ERR)
			{
				this->m_ctrlX1.AddString(x1);
			}
		}
		this->m_ctrlX1.SelectString(0, x1);
	}

	// X2
	CString x2(this->m_ctrlGrid.GetTextMatrix(nRow, NCOL_X2));
	if (x2.IsEmpty())
	{
		this->m_ctrlX2.SetCurSel(-1);
	}
	else
	{
		if (ADD_MISSING)
		{
			if (this->m_ctrlX2.FindString(-1, x2) == CB_ERR)
			{
				this->m_ctrlX2.AddString(x2);
			}
		}
		this->m_ctrlX2.SelectString(0, x2);
	}

	// analytical expression
	this->m_ctrlA0.SetWindowText(this->m_ctrlGrid.GetTextMatrix(nRow, NCOL_A0)); // implicit OnChangeEditA0
	this->m_ctrlA1.SetWindowText(this->m_ctrlGrid.GetTextMatrix(nRow, NCOL_A1)); // implicit OnChangeEditA1
	this->m_ctrlA2.SetWindowText(this->m_ctrlGrid.GetTextMatrix(nRow, NCOL_A2)); // implicit OnChangeEditA2
	this->m_ctrlA3.SetWindowText(this->m_ctrlGrid.GetTextMatrix(nRow, NCOL_A3)); // implicit OnChangeEditA3
	this->m_ctrlA4.SetWindowText(this->m_ctrlGrid.GetTextMatrix(nRow, NCOL_A4)); // implicit OnChangeEditA4
	this->m_ctrlA5.SetWindowText(this->m_ctrlGrid.GetTextMatrix(nRow, NCOL_A5)); // implicit OnChangeEditA5

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

void CKPPitzerXX::OnEnterCellGrid()
{
	UINT nResID = 0;
	switch (this->m_ctrlGrid.GetCol())
	{
	case NCOL_X1:
		return this->OnCbnSetfocusCbX1();
		break;
	case NCOL_X2:
		return this->OnCbnSetfocusCbX2();
		break;
	case NCOL_A0:
		return this->OnEnSetfocusEditA0();
		break;
	case NCOL_A1:
		return this->OnEnSetfocusEditA1();
		break;
	case NCOL_A2:
		return this->OnEnSetfocusEditA2();
		break;
	case NCOL_A3:
		return this->OnEnSetfocusEditA3();
		break;
	case NCOL_A4:
		return this->OnEnSetfocusEditA4();
		break;
	case NCOL_A5:
		return this->OnEnSetfocusEditA5();
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

void CKPPitzerXX::OnLeaveCellGrid()
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

void CKPPitzerXX::OnDblClickGrid()
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

void CKPPitzerXX::OnKeyDownGrid(short* KeyCode, short Shift)
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

void CKPPitzerXX::OnGotoFirstClicked()
{
	TRACE("OnGotoFirstClicked\n");

	this->OnLeaveCellGrid();
	this->m_ctrlGrid.SetRow(1);
	this->m_ctrlGrid.GetCellLeft();
	this->OnRowColChangeGrid();

	this->ResetFocus();
}

void CKPPitzerXX::OnGotoLastClicked()
{
	TRACE("OnGotoLastClicked\n");

	this->OnLeaveCellGrid();
	long nRow = 1;
	for (; nRow < this->m_ctrlGrid.GetRows(); ++nRow)
	{
		if (this->m_ctrlGrid.GetTextMatrix(nRow, NCOL_X1).IsEmpty())
			break;
	}
	this->m_ctrlGrid.SetRow(__max(nRow - 1, 1));
	this->m_ctrlGrid.GetCellLeft();
	this->OnRowColChangeGrid();
	this->ResetFocus();
}

void CKPPitzerXX::OnGotoNewClicked()
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

void CKPPitzerXX::OnGotoNextClicked()
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

void CKPPitzerXX::OnGotoPrevClicked()
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

void CKPPitzerXX::ResetFocus()
{
	if (this->m_hWndLastControl != NULL)
	{
		// restore focus and selection last control
		::SetFocus(this->m_hWndLastControl);
		if (this->m_bEditLastControl) // select edit item
			::SendMessage(this->m_hWndLastControl, EM_SETSEL, 0, -1);
	}
}

LRESULT CKPPitzerXX::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
{
	TRACE("OnBeginCellEdit\n");

	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bDenyEdit = FALSE;	// allow edit

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
		case NCOL_X1:
			pInfo->item.bUseCombo = (CUtil::InsertCationsAnions(pInfo->item.hWndCombo, GetDatabase()) > 0);
			break;

		case NCOL_X2:
			pInfo->item.bUseCombo = (CUtil::InsertCationsAnions(pInfo->item.hWndCombo, GetDatabase()) > 0);
			break;

		case NCOL_A0: case NCOL_A1: case NCOL_A2: case NCOL_A3: case NCOL_A4: case NCOL_A5:
			break;

		default :
			ASSERT(FALSE);
			break;
		}
		break;
	}

	return bDenyEdit;
}

LRESULT CKPPitzerXX::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
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
		case NCOL_X1:
			break;

		case NCOL_X2:
			break;

		case NCOL_A0: // A0
			if (pInfo->item.pszText == NULL)
			{
				// edit cancelled
				this->m_ctrlA0.SetWindowText(this->m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol));
			}
			else
			{
				this->m_ctrlA0.SetWindowText(pInfo->item.pszText);
			}
			break;


		case NCOL_A1: // A1
			if (pInfo->item.pszText == NULL)
			{
				// edit cancelled
				this->m_ctrlA1.SetWindowText(this->m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol));
			}
			else
			{
				this->m_ctrlA1.SetWindowText(pInfo->item.pszText);
			}
			break;

		case NCOL_A2: // A2
			if (pInfo->item.pszText == NULL)
			{
				// edit cancelled
				this->m_ctrlA2.SetWindowText(this->m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol));
			}
			else
			{
				this->m_ctrlA2.SetWindowText(pInfo->item.pszText);
			}
			break;

		case NCOL_A3: // A3
			if (pInfo->item.pszText == NULL)
			{
				// edit cancelled
				this->m_ctrlA3.SetWindowText(this->m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol));
			}
			else
			{
				this->m_ctrlA3.SetWindowText(pInfo->item.pszText);
			}
			break;

		case NCOL_A4: // A4
			if (pInfo->item.pszText == NULL)
			{
				// edit cancelled
				this->m_ctrlA4.SetWindowText(this->m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol));
			}
			else
			{
				this->m_ctrlA4.SetWindowText(pInfo->item.pszText);
			}
			break;

		case NCOL_A5: // A5
			if (pInfo->item.pszText == NULL)
			{
				// edit cancelled
				this->m_ctrlA5.SetWindowText(this->m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol));
			}
			else
			{
				this->m_ctrlA5.SetWindowText(pInfo->item.pszText);
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

LRESULT CKPPitzerXX::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
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

LRESULT CKPPitzerXX::OnChange(WPARAM wParam, LPARAM lParam)
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
		case NCOL_A0:  // A0
			m_ctrlA0.SetWindowText(pInfo->item.pszText);
			break;

		case NCOL_A1:  // A1
			m_ctrlA1.SetWindowText(pInfo->item.pszText);
			break;

		case NCOL_A2:  // A2
			m_ctrlA2.SetWindowText(pInfo->item.pszText);
			break;

		case NCOL_A3:  // A3
			m_ctrlA3.SetWindowText(pInfo->item.pszText);
			break;

		case NCOL_A4:  // A4
			m_ctrlA4.SetWindowText(pInfo->item.pszText);
			break;

		case NCOL_A5:  // A5
			m_ctrlA5.SetWindowText(pInfo->item.pszText);
			break;

// COMMENT: {11/6/2009 6:56:46 PM}		default :
// COMMENT: {11/6/2009 6:56:46 PM}			ASSERT(FALSE);
// COMMENT: {11/6/2009 6:56:46 PM}			break;
		}
		break;
	}
	return 0;

}

LRESULT CKPPitzerXX::OnAllowDeleteRow(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	ASSERT(nID == IDC_MSHFLEXGRID1);

	return TRUE;  // allow
}

LRESULT CKPPitzerXX::OnDeleteRow(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	ASSERT(nID == IDC_MSHFLEXGRID1);

	m_ctrlGrid.DeleteRow(pInfo->item.iRow);
	return TRUE; // handled locally
}

LRESULT CKPPitzerXX::OnAllowInsertRow(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	ASSERT(nID == IDC_MSHFLEXGRID1);

	return FALSE;  // disallow for now
}

LRESULT CKPPitzerXX::OnInsertRow(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	ASSERT(nID == IDC_MSHFLEXGRID1);

	ASSERT(FALSE); // OnAllowInsertRow disallowed

	return TRUE; // handled locally
}

void CKPPitzerXX::OnCbnEditchangeCbX1()
{
	TRACE("OnCbnEditchangeCbX1\n");
	if (!this->m_bIgnoreChanges)
	{
		CString str;
		this->m_ctrlX1.GetWindowText(str);
		this->m_ctrlGrid.SetTextMatrix(this->m_ctrlGrid.GetRow(), NCOL_X1, str);
	}
}

void CKPPitzerXX::OnCbnSelchangeCbX1()
{
	TRACE("OnCbnSelchangeCbX1\n");
	if (!this->m_bIgnoreChanges)
	{
		int n = this->m_ctrlX1.GetCurSel();
		if (n >= 0)
		{
			CString str;
			this->m_ctrlX1.GetLBText(n, str);
			this->m_ctrlGrid.SetTextMatrix(this->m_ctrlGrid.GetRow(), NCOL_X1, str);
		}
		else
		{
			ASSERT(FALSE);
		}
	}
}

void CKPPitzerXX::OnCbnEditchangeCbX2()
{
	TRACE("OnCbnEditchangeCbX2\n");
	if (!this->m_bIgnoreChanges)
	{
		CString str;
		this->m_ctrlX2.GetWindowText(str);
		this->m_ctrlGrid.SetTextMatrix(this->m_ctrlGrid.GetRow(), NCOL_X2, str);
	}
}

void CKPPitzerXX::OnCbnSelchangeCbX2()
{
	TRACE("OnCbnSelchangeCbX2\n");
	if (!this->m_bIgnoreChanges)
	{
		int n = this->m_ctrlX2.GetCurSel();
		if (n >= 0)
		{
			CString str;
			this->m_ctrlX2.GetLBText(n, str);
			this->m_ctrlGrid.SetTextMatrix(this->m_ctrlGrid.GetRow(), NCOL_X2, str);
		}
		else
		{
			ASSERT(FALSE);
		}
	}
}

void CKPPitzerXX::OnCbnSetfocusCbX1()
{
	CString strRes;
	strRes.Format(_T("Choose %s for this interaction coefficient."), _T("a(n) cation/anion"));
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPPitzerXX::OnCbnSetfocusCbX2()
{
	CString strRes;
	strRes.Format(_T("Choose %s for this interaction coefficient."), _T("a(n) cation/anion"));
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPPitzerXX::OnSize(UINT nType, int cx, int cy)
{
	CKeywordPage::OnSize(nType, cx, cy);

	// resize the last column to fill the grid area
	if (this->m_ctrlGrid.GetSafeHwnd())
	{
		long width = 0;
		long col = 0;
		for (; col < this->m_ctrlGrid.GetCols(0) - 1; ++col)
		{
			width += this->m_ctrlGrid.GetColWidth(col, 0);
		}

		CRect rect;
		CDC* pDC = GetDC();
		int nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
		this->m_ctrlGrid.GetClientRect(&rect);
		this->m_ctrlGrid.SetColWidth(col, 0, MulDiv(rect.right, TWIPS_PER_INCH, nLogX) - width);
	}	
}
