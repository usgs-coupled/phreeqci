// KPPitzerB0.cpp : implementation file
//

#include "stdafx.h"
#include "phreeqci2.h"
#include "KPPitzerCAN.h"
#include "Util.h"

// CKPPitzerCAN dialog

// IMPLEMENT_DYNAMIC(CKPPitzerCAN, CPropertyPage)
IMPLEMENT_DYNCREATE(CKPPitzerCAN, baseCKPPitzerCAN)

const long NCOL_IMAGE          = 0;
const long NCOL_C              = 1;
const long NCOL_A              = 2;
const long NCOL_N              = 3;
const long NCOL_A0             = 4;
const long NCOL_A1             = 5;
const long NCOL_A2             = 6;
const long NCOL_A3             = 7;
const long NCOL_A4             = 8;
const long NCOL_A5             = 9;

const bool ADD_MISSING = false;

CKPPitzerCAN::CKPPitzerCAN(UINT nIDTemplate, UINT nIDCaption) : baseCKPPitzerCAN(nIDTemplate, nIDCaption)
{
	this->m_pictHolderRightArrow.CreateFromIcon(IDI_RIGHT_ARROW2);
	this->m_pictHolderStar.CreateFromIcon(IDI_STAR);

	this->m_hWndLastControl  = NULL;
	this->m_bEditLastControl = FALSE;

	this->m_bIgnoreChanges = false;
}

CKPPitzerCAN::~CKPPitzerCAN()
{
}

void CKPPitzerCAN::DoDataExchange(CDataExchange* pDX)
{
	baseCKPPitzerCAN::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CB_CATION, this->m_ctrlC);
	DDX_Control(pDX, IDC_CB_ANION, this->m_ctrlA);
	DDX_Control(pDX, IDC_CB_NEUTRAL, this->m_ctrlN);

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
		CUtil::InsertCations(this->m_ctrlC.GetSafeHwnd(), this->GetDatabase());
		CUtil::InsertAnions(this->m_ctrlA.GetSafeHwnd(), this->GetDatabase());
		CUtil::InsertNeutral(this->m_ctrlN.GetSafeHwnd(), this->GetDatabase());
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

			CPitzParam p(TYPE_ZETA);

			if (m_ctrlGrid.GetTextMatrix(nRow, NCOL_C).IsEmpty()
				&&
				m_ctrlGrid.GetTextMatrix(nRow, NCOL_A).IsEmpty()
				&&
				m_ctrlGrid.GetTextMatrix(nRow, NCOL_N).IsEmpty())
			{
				continue;
			}

			// Cation
			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_C, p.species[0]);
			if (p.species[0].IsEmpty())
			{
				pDX->PrepareCtrl(IDC_CB_CATION);
				::AfxMessageBox("Please define a cation");
				pDX->Fail();
			}

			// Anion
			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_A, p.species[1]);
			if (p.species[1].IsEmpty())
			{
				pDX->PrepareCtrl(IDC_CB_ANION);
				::AfxMessageBox("Please define a anion");
				pDX->Fail();
			}


			// Neutral
			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_N, p.species[2]);
			if (p.species[2].IsEmpty())
			{
				pDX->PrepareCtrl(IDC_CB_NEUTRAL);
				::AfxMessageBox("Please define an neutral ion");
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

			// Cation
			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_C, p.species[0]);

			// Anion
			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_A, p.species[1]);

			// Neutral
			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_N, p.species[2]);

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


BEGIN_MESSAGE_MAP(CKPPitzerCAN, baseCKPPitzerCAN)
	ON_EN_CHANGE(IDC_EDIT_A0, &CKPPitzerCAN::OnEnChangeEditA0)
	ON_EN_CHANGE(IDC_EDIT_A1, &CKPPitzerCAN::OnEnChangeEditA1)
	ON_EN_CHANGE(IDC_EDIT_A2, &CKPPitzerCAN::OnEnChangeEditA2)
	ON_EN_CHANGE(IDC_EDIT_A3, &CKPPitzerCAN::OnEnChangeEditA3)
	ON_EN_CHANGE(IDC_EDIT_A4, &CKPPitzerCAN::OnEnChangeEditA4)
	ON_EN_CHANGE(IDC_EDIT_A5, &CKPPitzerCAN::OnEnChangeEditA5)
	ON_EN_SETFOCUS(IDC_EDIT_A0, &CKPPitzerCAN::OnEnSetfocusEditA0)
	ON_EN_SETFOCUS(IDC_EDIT_A1, &CKPPitzerCAN::OnEnSetfocusEditA1)
	ON_EN_SETFOCUS(IDC_EDIT_A2, &CKPPitzerCAN::OnEnSetfocusEditA2)
	ON_EN_SETFOCUS(IDC_EDIT_A3, &CKPPitzerCAN::OnEnSetfocusEditA3)
	ON_EN_SETFOCUS(IDC_EDIT_A4, &CKPPitzerCAN::OnEnSetfocusEditA4)
	ON_EN_SETFOCUS(IDC_EDIT_A5, &CKPPitzerCAN::OnEnSetfocusEditA5)
	ON_EN_KILLFOCUS(IDC_EDIT_A0, &CKPPitzerCAN::OnEnKillfocusEditA0)
	ON_EN_KILLFOCUS(IDC_EDIT_A1, &CKPPitzerCAN::OnEnKillfocusEditA1)
	ON_EN_KILLFOCUS(IDC_EDIT_A2, &CKPPitzerCAN::OnEnKillfocusEditA2)
	ON_EN_KILLFOCUS(IDC_EDIT_A3, &CKPPitzerCAN::OnEnKillfocusEditA3)
	ON_EN_KILLFOCUS(IDC_EDIT_A4, &CKPPitzerCAN::OnEnKillfocusEditA4)
	ON_EN_KILLFOCUS(IDC_EDIT_A5, &CKPPitzerCAN::OnEnKillfocusEditA5)
	// custom edit grid notifications
	ON_MESSAGE(EGN_BEGINCELLEDIT, &CKPPitzerCAN::OnBeginCellEdit)
	ON_MESSAGE(EGN_ENDCELLEDIT, &CKPPitzerCAN::OnEndCellEdit)
	ON_MESSAGE(EGN_SETFOCUS, &CKPPitzerCAN::OnSetfocusGrid)
	ON_MESSAGE(EGN_CHANGE, &CKPPitzerCAN::OnChange)
	ON_MESSAGE(EGN_ALLOW_INSERT_ROW, &CKPPitzerCAN::OnAllowInsertRow)
	ON_MESSAGE(EGN_INSERT_ROW, &CKPPitzerCAN::OnInsertRow)
	ON_MESSAGE(EGN_ALLOW_DELETE_ROW, &CKPPitzerCAN::OnAllowDeleteRow)
	ON_MESSAGE(EGN_DELETE_ROW, &CKPPitzerCAN::OnDeleteRow)
	// ComboBox
	ON_CBN_EDITCHANGE(IDC_CB_CATION, &CKPPitzerCAN::OnCbnEditchangeCbC)
	ON_CBN_SELCHANGE(IDC_CB_CATION, &CKPPitzerCAN::OnCbnSelchangeCbC)
	ON_CBN_EDITCHANGE(IDC_CB_ANION, &CKPPitzerCAN::OnCbnEditchangeCbA)
	ON_CBN_SELCHANGE(IDC_CB_ANION, &CKPPitzerCAN::OnCbnSelchangeCbA)
	ON_CBN_EDITCHANGE(IDC_CB_NEUTRAL, &CKPPitzerCAN::OnCbnEditchangeCbN)
	ON_CBN_SELCHANGE(IDC_CB_NEUTRAL, &CKPPitzerCAN::OnCbnSelchangeCbN)
	ON_CBN_SETFOCUS(IDC_CB_CATION, &CKPPitzerCAN::OnCbnSetfocusCbCation)
	ON_CBN_SETFOCUS(IDC_CB_ANION, &CKPPitzerCAN::OnCbnSetfocusCbAnion)
	ON_CBN_SETFOCUS(IDC_CB_NEUTRAL, &CKPPitzerCAN::OnCbnSetfocusCbNeutral)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CKPPitzerCAN message handlers
void CKPPitzerCAN::InitGrid(CDataExchange* pDX, int nIDC)
{
	UNUSED(pDX);
	ASSERT(nIDC == this->m_ctrlGrid.GetDlgCtrlID());
	UNUSED(nIDC);   // unused in release builds

	// set column widths
	this->m_ctrlGrid.SetColWidth(NCOL_IMAGE, 0, 300);

	this->m_ctrlGrid.SetColWidth(NCOL_C, 0, 1095);
	this->m_ctrlGrid.SetColWidth(NCOL_A, 0, 1095);
	this->m_ctrlGrid.SetColWidth(NCOL_N,  0, 1095);
	this->m_ctrlGrid.SetColWidth(NCOL_A0, 0, 1095);
	this->m_ctrlGrid.SetColWidth(NCOL_A1, 0, 1095);
	this->m_ctrlGrid.SetColWidth(NCOL_A2, 0, 1095);
	this->m_ctrlGrid.SetColWidth(NCOL_A3, 0, 1095);
	this->m_ctrlGrid.SetColWidth(NCOL_A4, 0, 1095);
	this->m_ctrlGrid.SetColWidth(NCOL_A4, 0, 1095);

	// set grid titles
    this->m_ctrlGrid.SetTextMatrix(0, NCOL_C, _T("Cation"));
    this->m_ctrlGrid.SetTextMatrix(0, NCOL_A, _T("Anion"));
    this->m_ctrlGrid.SetTextMatrix(0, NCOL_N,  _T("Neutral"));
    this->m_ctrlGrid.SetTextMatrix(0, NCOL_A0, _T("A0"));
    this->m_ctrlGrid.SetTextMatrix(0, NCOL_A1, _T("A1"));
    this->m_ctrlGrid.SetTextMatrix(0, NCOL_A2, _T("A2"));
    this->m_ctrlGrid.SetTextMatrix(0, NCOL_A3, _T("A3"));
    this->m_ctrlGrid.SetTextMatrix(0, NCOL_A4, _T("A4"));
    this->m_ctrlGrid.SetTextMatrix(0, NCOL_A5, _T("A5"));

	// set alignment
	this->m_ctrlGrid.SetColAlignment(NCOL_C, flexAlignLeftCenter);
	this->m_ctrlGrid.SetColAlignment(NCOL_A, flexAlignLeftCenter);
	this->m_ctrlGrid.SetColAlignment(NCOL_N, flexAlignLeftCenter);

	// set starting position
	this->m_ctrlGrid.SetRow(1);
	this->m_ctrlGrid.SetCol(1);
}

BOOL CKPPitzerCAN::OnInitDialog()
{
	baseCKPPitzerCAN::OnInitDialog();

	// set layout
	CreateRoot(VERTICAL, 5, 6)

		<< (paneCtrl(IDC_STATIC_MAIN, HORIZONTAL, ABSOLUTE_VERT, nDefaultBorder, 10, 10)
			<< (pane(VERTICAL, GREEDY)

				<< (pane(HORIZONTAL, GREEDY)
					<< item(IDC_ST_CATION, NORESIZE | ALIGN_CENTER)
					<< item(IDC_CB_CATION, ABSOLUTE_VERT | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_ST_ANION, NORESIZE | ALIGN_CENTER)
					<< item(IDC_CB_ANION, ABSOLUTE_VERT | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_ST_NEUTRAL, NORESIZE | ALIGN_CENTER)
					<< item(IDC_CB_NEUTRAL, ABSOLUTE_VERT | ALIGN_CENTER)
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

void CKPPitzerCAN::OnEnChangeEditA0()
{
	TRACE("OnEnChangeEditA0\n");
	if (!this->m_bIgnoreChanges)
	{
		CString str;
		this->m_ctrlA0.GetWindowText(str);
		this->m_ctrlGrid.SetTextMatrix(this->m_ctrlGrid.GetRow(), NCOL_A0, str);
	}
}

void CKPPitzerCAN::OnEnChangeEditA1()
{
	TRACE("OnEnChangeEditA1\n");
	if (!this->m_bIgnoreChanges)
	{
		CString str;
		this->m_ctrlA1.GetWindowText(str);
		this->m_ctrlGrid.SetTextMatrix(this->m_ctrlGrid.GetRow(), NCOL_A1, str);
	}
}

void CKPPitzerCAN::OnEnChangeEditA2()
{
	TRACE("OnEnChangeEditA2\n");
	if (!this->m_bIgnoreChanges)
	{
		CString str;
		this->m_ctrlA2.GetWindowText(str);
		this->m_ctrlGrid.SetTextMatrix(this->m_ctrlGrid.GetRow(), NCOL_A2, str);
	}
}

void CKPPitzerCAN::OnEnChangeEditA3()
{
	TRACE("OnEnChangeEditA3\n");
	if (!this->m_bIgnoreChanges)
	{
		CString str;
		this->m_ctrlA3.GetWindowText(str);
		this->m_ctrlGrid.SetTextMatrix(this->m_ctrlGrid.GetRow(), NCOL_A3, str);
	}
}

void CKPPitzerCAN::OnEnChangeEditA4()
{
	TRACE("OnEnChangeEditA4\n");
	if (!this->m_bIgnoreChanges)
	{
		CString str;
		this->m_ctrlA4.GetWindowText(str);
		this->m_ctrlGrid.SetTextMatrix(this->m_ctrlGrid.GetRow(), NCOL_A4, str);
	}
}

void CKPPitzerCAN::OnEnChangeEditA5()
{
	TRACE("OnEnChangeEditA5\n");
	if (!this->m_bIgnoreChanges)
	{
		CString str;
		this->m_ctrlA5.GetWindowText(str);
		this->m_ctrlGrid.SetTextMatrix(this->m_ctrlGrid.GetRow(), NCOL_A5, str);
	}
}

void CKPPitzerCAN::OnEnSetfocusEditA0()
{
	CString strRes;
	strRes.FormatMessage(IDS_STRING702, _T("A0"), this->m_strCaption);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPPitzerCAN::OnEnSetfocusEditA1()
{
	CString strRes;
	strRes.FormatMessage(IDS_STRING702, _T("A1"), this->m_strCaption);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPPitzerCAN::OnEnSetfocusEditA2()
{
	CString strRes;
	strRes.FormatMessage(IDS_STRING702, _T("A2"), this->m_strCaption);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPPitzerCAN::OnEnSetfocusEditA3()
{
	CString strRes;
	strRes.FormatMessage(IDS_STRING702, _T("A3"), this->m_strCaption);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPPitzerCAN::OnEnSetfocusEditA4()
{
	CString strRes;
	strRes.FormatMessage(IDS_STRING702, _T("A4"), this->m_strCaption);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPPitzerCAN::OnEnSetfocusEditA5()
{
	CString strRes;
	strRes.FormatMessage(IDS_STRING702, _T("A5"), this->m_strCaption);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPPitzerCAN::OnEnKillfocusEditA0()
{
	this->m_hWndLastControl = this->m_ctrlA0.m_hWnd;
	this->m_bEditLastControl = TRUE;
}

void CKPPitzerCAN::OnEnKillfocusEditA1()
{
	this->m_hWndLastControl = this->m_ctrlA1.m_hWnd;
	this->m_bEditLastControl = TRUE;
}

void CKPPitzerCAN::OnEnKillfocusEditA2()
{
	this->m_hWndLastControl = this->m_ctrlA2.m_hWnd;
	this->m_bEditLastControl = TRUE;
}

void CKPPitzerCAN::OnEnKillfocusEditA3()
{
	this->m_hWndLastControl = this->m_ctrlA3.m_hWnd;
	this->m_bEditLastControl = TRUE;
}

void CKPPitzerCAN::OnEnKillfocusEditA4()
{
	this->m_hWndLastControl = this->m_ctrlA4.m_hWnd;
	this->m_bEditLastControl = TRUE;
}

void CKPPitzerCAN::OnEnKillfocusEditA5()
{
	this->m_hWndLastControl = this->m_ctrlA5.m_hWnd;
	this->m_bEditLastControl = TRUE;
}

BEGIN_EVENTSINK_MAP(CKPPitzerCAN, CKeywordPage)
	ON_EVENT(CKPPitzerCAN, IDC_MSHFLEXGRID1, 70, CKPPitzerCAN::OnRowColChangeGrid, VTS_NONE)
	ON_EVENT(CKPPitzerCAN, IDC_MSHFLEXGRID1, 71, CKPPitzerCAN::OnEnterCellGrid, VTS_NONE)
	ON_EVENT(CKPPitzerCAN, IDC_MSHFLEXGRID1, 72, CKPPitzerCAN::OnLeaveCellGrid, VTS_NONE)
	ON_EVENT(CKPPitzerCAN, IDC_MSHFLEXGRID1, DISPID_DBLCLICK, CKPPitzerCAN::OnDblClickGrid, VTS_NONE)
	ON_EVENT(CKPPitzerCAN, IDC_MSHFLEXGRID1, DISPID_KEYDOWN, CKPPitzerCAN::OnKeyDownGrid, VTS_PI2 VTS_I2)
	ON_EVENT(CKPPitzerCAN, IDC_SRCDBPGCTRL1, 1, CKPPitzerCAN::OnGotoFirstClicked, VTS_NONE)
	ON_EVENT(CKPPitzerCAN, IDC_SRCDBPGCTRL1, 4, CKPPitzerCAN::OnGotoLastClicked, VTS_NONE)
	ON_EVENT(CKPPitzerCAN, IDC_SRCDBPGCTRL1, 5, CKPPitzerCAN::OnGotoNewClicked, VTS_NONE)
	ON_EVENT(CKPPitzerCAN, IDC_SRCDBPGCTRL1, 3, CKPPitzerCAN::OnGotoNextClicked, VTS_NONE)
	ON_EVENT(CKPPitzerCAN, IDC_SRCDBPGCTRL1, 2, CKPPitzerCAN::OnGotoPrevClicked, VTS_NONE)
END_EVENTSINK_MAP()

void CKPPitzerCAN::OnRowColChangeGrid()
{
	TRACE("OnRowColChangeGrid\n");

	ASSERT(m_bIgnoreChanges == false);
	m_bIgnoreChanges = true;
	TRACE("Setting m_bIgnoreChanges = true in OnRowColChangeGrid\n");

	long nRow = m_ctrlGrid.GetRow();
	long nCol = m_ctrlGrid.GetCol();

	// cation
	CString cation(this->m_ctrlGrid.GetTextMatrix(nRow, NCOL_C));
	if (cation.IsEmpty())
	{
		this->m_ctrlC.SetCurSel(-1);
	}
	else
	{
		if (ADD_MISSING)
		{
			if (this->m_ctrlC.FindString(-1, cation) == CB_ERR)
			{
				this->m_ctrlC.AddString(cation);
			}
		}
		this->m_ctrlC.SelectString(0, cation);
	}

	// anion
	CString anion(this->m_ctrlGrid.GetTextMatrix(nRow, NCOL_A));
	if (anion.IsEmpty())
	{
		this->m_ctrlA.SetCurSel(-1);
	}
	else
	{
		if (ADD_MISSING)
		{
			if (this->m_ctrlA.FindString(-1, anion) == CB_ERR)
			{
				this->m_ctrlA.AddString(anion);
			}
		}
		this->m_ctrlA.SelectString(0, anion);
	}

	// neutral
	CString neutral(this->m_ctrlGrid.GetTextMatrix(nRow, NCOL_N));
	if (neutral.IsEmpty())
	{
		this->m_ctrlN.SetCurSel(-1);
	}
	else
	{
		if (ADD_MISSING)
		{
			if (this->m_ctrlN.FindString(-1, neutral) == CB_ERR)
			{
				this->m_ctrlN.AddString(neutral);
			}
		}
		this->m_ctrlN.SelectString(0, neutral);
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

void CKPPitzerCAN::OnEnterCellGrid()
{
	UINT nResID = 0;
	switch (this->m_ctrlGrid.GetCol())
	{
	case NCOL_C:
		return this->OnCbnSetfocusCbCation();
		break;
	case NCOL_A:
		return this->OnCbnSetfocusCbAnion();
		break;
	case NCOL_N:
		return this->OnCbnSetfocusCbNeutral();
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

void CKPPitzerCAN::OnLeaveCellGrid()
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

void CKPPitzerCAN::OnDblClickGrid()
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

void CKPPitzerCAN::OnKeyDownGrid(short* KeyCode, short Shift)
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

void CKPPitzerCAN::OnGotoFirstClicked()
{
	TRACE("OnGotoFirstClicked\n");

	this->OnLeaveCellGrid();
	this->m_ctrlGrid.SetRow(1);
	this->m_ctrlGrid.GetCellLeft();
	this->OnRowColChangeGrid();

	this->ResetFocus();
}

void CKPPitzerCAN::OnGotoLastClicked()
{
	TRACE("OnGotoLastClicked\n");

	this->OnLeaveCellGrid();
	long nRow = 1;
	for (; nRow < this->m_ctrlGrid.GetRows(); ++nRow)
	{
		if (this->m_ctrlGrid.GetTextMatrix(nRow, NCOL_C).IsEmpty())
			break;
	}
	this->m_ctrlGrid.SetRow(__max(nRow - 1, 1));
	this->m_ctrlGrid.GetCellLeft();
	this->OnRowColChangeGrid();
	this->ResetFocus();
}

void CKPPitzerCAN::OnGotoNewClicked()
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

void CKPPitzerCAN::OnGotoNextClicked()
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

void CKPPitzerCAN::OnGotoPrevClicked()
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

void CKPPitzerCAN::ResetFocus()
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

LRESULT CKPPitzerCAN::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
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
		case NCOL_C:
			pInfo->item.bUseCombo = (CUtil::InsertCations(pInfo->item.hWndCombo, GetDatabase()) > 0);
			break;

		case NCOL_A:
			pInfo->item.bUseCombo = (CUtil::InsertAnions(pInfo->item.hWndCombo, GetDatabase()) > 0);
			break;

		case NCOL_N:
			pInfo->item.bUseCombo = (CUtil::InsertNeutral(pInfo->item.hWndCombo, GetDatabase()) > 0);
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

LRESULT CKPPitzerCAN::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
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
		case NCOL_C:
			break;

		case NCOL_A:
			break;

		case NCOL_N:
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

LRESULT CKPPitzerCAN::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
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

LRESULT CKPPitzerCAN::OnChange(WPARAM wParam, LPARAM lParam)
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

LRESULT CKPPitzerCAN::OnAllowDeleteRow(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	ASSERT(nID == IDC_MSHFLEXGRID1);

	return TRUE;  // allow
}

LRESULT CKPPitzerCAN::OnDeleteRow(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	ASSERT(nID == IDC_MSHFLEXGRID1);

	m_ctrlGrid.DeleteRow(pInfo->item.iRow);
	return TRUE; // handled locally
}

LRESULT CKPPitzerCAN::OnAllowInsertRow(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	ASSERT(nID == IDC_MSHFLEXGRID1);

	return FALSE;  // disallow for now
}

LRESULT CKPPitzerCAN::OnInsertRow(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	ASSERT(nID == IDC_MSHFLEXGRID1);

	ASSERT(FALSE); // OnAllowInsertRow disallowed

	return TRUE; // handled locally
}

void CKPPitzerCAN::OnCbnEditchangeCbC()
{
	TRACE("OnCbnEditchangeCbC\n");
	if (!this->m_bIgnoreChanges)
	{
		CString str;
		this->m_ctrlC.GetWindowText(str);
		this->m_ctrlGrid.SetTextMatrix(this->m_ctrlGrid.GetRow(), NCOL_C, str);
	}
}

void CKPPitzerCAN::OnCbnSelchangeCbC()
{
	TRACE("OnCbnSelchangeCbC\n");
	if (!this->m_bIgnoreChanges)
	{
		int n = this->m_ctrlC.GetCurSel();
		if (n >= 0)
		{
			CString str;
			this->m_ctrlC.GetLBText(n, str);
			this->m_ctrlGrid.SetTextMatrix(this->m_ctrlGrid.GetRow(), NCOL_C, str);
		}
		else
		{
			ASSERT(FALSE);
		}
	}
}

void CKPPitzerCAN::OnCbnEditchangeCbA()
{
	TRACE("OnCbnEditchangeCbA\n");
	if (!this->m_bIgnoreChanges)
	{
		CString str;
		this->m_ctrlA.GetWindowText(str);
		this->m_ctrlGrid.SetTextMatrix(this->m_ctrlGrid.GetRow(), NCOL_A, str);
	}
}

void CKPPitzerCAN::OnCbnSelchangeCbA()
{
	TRACE("OnCbnSelchangeCbA\n");
	if (!this->m_bIgnoreChanges)
	{
		int n = this->m_ctrlA.GetCurSel();
		if (n >= 0)
		{
			CString str;
			this->m_ctrlA.GetLBText(n, str);
			this->m_ctrlGrid.SetTextMatrix(this->m_ctrlGrid.GetRow(), NCOL_A, str);
		}
		else
		{
			ASSERT(FALSE);
		}
	}
}

void CKPPitzerCAN::OnCbnEditchangeCbN()
{
	TRACE("OnCbnEditchangeCbCation\n");
	if (!this->m_bIgnoreChanges)
	{
		CString str;
		this->m_ctrlN.GetWindowText(str);
		this->m_ctrlGrid.SetTextMatrix(this->m_ctrlGrid.GetRow(), NCOL_N, str);
	}
}

void CKPPitzerCAN::OnCbnSelchangeCbN()
{
	TRACE("OnCbnSelchangeCbCation\n");
	if (!this->m_bIgnoreChanges)
	{
		int n = this->m_ctrlN.GetCurSel();
		if (n >= 0)
		{
			CString str;
			this->m_ctrlN.GetLBText(n, str);
			this->m_ctrlGrid.SetTextMatrix(this->m_ctrlGrid.GetRow(), NCOL_N, str);
		}
		else
		{
			ASSERT(FALSE);
		}
	}
}

void CKPPitzerCAN::OnCbnSetfocusCbCation()
{
	CString strRes;
	strRes.Format(_T("Choose %s for this interaction coefficient."), _T("a cation"));
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPPitzerCAN::OnCbnSetfocusCbAnion()
{
	CString strRes;
	strRes.Format(_T("Choose %s for this interaction coefficient."), _T("an anion"));
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPPitzerCAN::OnCbnSetfocusCbNeutral()
{
	CString strRes;
	strRes.Format(_T("Choose %s for this interaction coefficient."), _T("a neutral ion"));
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPPitzerCAN::OnSize(UINT nType, int cx, int cy)
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
