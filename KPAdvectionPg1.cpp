// KPAdvectionPg1.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "KPAdvectionPg1.h"

#include "OCKSAdvection.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CKPAdvectionPg1, baseCKPAdvectionPg1)
IMPLEMENT_DYNCREATE(CKPAdvectionPg2, baseCKPAdvectionPg2)

/////////////////////////////////////////////////////////////////////////////
// CKPAdvectionPg1 property page

CKPAdvectionPg1::CKPAdvectionPg1() : baseCKPAdvectionPg1(CKPAdvectionPg1::IDD)
{
	//{{AFX_DATA_INIT(CKPAdvectionPg1)
	m_nCells = 1;
	m_nShifts = 1;
	m_bPrintWarnings = TRUE;
	m_dTimeStep = 0.0;
	m_dInitTime = 0.0;
	//}}AFX_DATA_INIT
	m_tuTimeStep = TU_SECONDS;
	m_tuInitTime = TU_SECONDS;
}

CKPAdvectionPg1::~CKPAdvectionPg1()
{
}

void CKPAdvectionPg1::DoDataExchange(CDataExchange* pDX)
{
	baseCKPAdvectionPg1::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKPAdvectionPg1)
	DDX_Control(pDX, IDC_DESCRIPTION, m_eInputDesc);
	DDX_Control(pDX, IDC_CBO_INIT_TIME_UNITS, m_cboInitTimeUnits);
	DDX_Control(pDX, IDC_CBO_TIME_STEP_UNITS, m_cboTimeStepUnits);
	DDX_Control(pDX, IDC_UD_SHIFTS, m_spinShifts);
	DDX_Control(pDX, IDC_UD_CELLS, m_spinCells);
	DDX_Text(pDX, IDC_E_CELLS, m_nCells);
	DDV_MinMaxUInt(pDX, m_nCells, 1, INT_MAX);
	DDX_Text(pDX, IDC_E_SHIFTS, m_nShifts);
	DDV_MinMaxUInt(pDX, m_nShifts, 1, INT_MAX);
	DDX_Check(pDX, IDC_C_DISP_WARN, m_bPrintWarnings);
	DDX_Text(pDX, IDC_E_TIME_STEP, m_dTimeStep);
	DDX_Text(pDX, IDC_E_INIT_TIME, m_dInitTime);
	//}}AFX_DATA_MAP
	if (m_bFirstSetActive)
	{
		UDACCEL uda[7];
		uda[0].nSec = 0;
		uda[0].nInc = 1;
		uda[1].nSec = 2;
		uda[1].nInc = 2;
		uda[2].nSec = 4;
		uda[2].nInc = 10;
		uda[3].nSec = 8;
		uda[3].nInc = 100;
		uda[4].nSec = 16;
		uda[4].nInc = 1000;
		uda[5].nSec = 32;
		uda[5].nInc = 10000;
		uda[6].nSec = 64;
		uda[6].nInc = 100000;

		// init spinners
		m_spinCells.SetRange32(1, INT_MAX - 1);
		m_spinCells.SetAccel(7, uda);
		m_spinShifts.SetRange32(1, INT_MAX - 1);
		m_spinShifts.SetAccel(7, uda);
	}
	if (pDX->m_bSaveAndValidate)
	{
		int n = m_cboTimeStepUnits.GetCurSel();
		m_tuTimeStep = (TimeUnit)n;

		n = m_cboInitTimeUnits.GetCurSel();
		m_tuInitTime = (TimeUnit)n;
	}
	else
	{
		m_cboTimeStepUnits.SetCurSel((int)m_tuTimeStep);
		m_cboInitTimeUnits.SetCurSel((int)m_tuInitTime);
	}
}


BEGIN_MESSAGE_MAP(CKPAdvectionPg1, baseCKPAdvectionPg1)
	//{{AFX_MSG_MAP(CKPAdvectionPg1)
	ON_EN_SETFOCUS(IDC_E_CELLS, OnSetfocusECells)
	ON_EN_SETFOCUS(IDC_E_SHIFTS, OnSetfocusEShifts)
	ON_EN_SETFOCUS(IDC_E_TIME_STEP, OnSetfocusETimeStep)
	ON_CBN_SETFOCUS(IDC_CBO_TIME_STEP_UNITS, OnSetfocusCboTimeStepUnits)
	ON_EN_SETFOCUS(IDC_E_INIT_TIME, OnSetfocusEInitTime)
	ON_CBN_SETFOCUS(IDC_CBO_INIT_TIME_UNITS, OnSetfocusCboInitTimeUnits)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
	ON_BN_SETFOCUS(IDC_C_DISP_WARN, OnSetfocusBWarn)
END_MESSAGE_MAP()


BOOL CKPAdvectionPg1::OnInitDialog() 
{
	baseCKPAdvectionPg1::OnInitDialog();

	// set layout
	CreateRoot(VERTICAL, 5, 6) 
			<< (paneCtrl(IDC_S_PARAMS, HORIZONTAL, ABSOLUTE_VERT, 0, 18, 8, 86)
				<< itemFixed(HORIZONTAL, 18)
				<< (pane(VERTICAL, ABSOLUTE_VERT, nDefaultBorder, 0, 0)
					<< item(IDC_S_CELLS, ABSOLUTE_VERT)
					<< (pane(HORIZONTAL, ABSOLUTE_VERT, 0, 0, 0)
						<< item(IDC_E_CELLS, ABSOLUTE_VERT|ALIGN_VCENTER)
						<< item(IDC_UD_CELLS, NORESIZE)
						<< itemFixed(HORIZONTAL, 50)
						)
					)
				<< itemFixed(HORIZONTAL, 63)
				<< (pane(VERTICAL, ABSOLUTE_VERT, nDefaultBorder, 0, 0)
					<< item(IDC_S_SHIFTS, ABSOLUTE_VERT)
					<< (pane(HORIZONTAL, ABSOLUTE_VERT, 0, 0, 0)
						<< item(IDC_E_SHIFTS, ABSOLUTE_VERT|ALIGN_VCENTER)
						<< item(IDC_UD_SHIFTS, NORESIZE)
						<< itemFixed(HORIZONTAL, 50)
						)
					)
				)
			<< (paneCtrl(IDC_S_KIN_OPTS, HORIZONTAL, ABSOLUTE_VERT, 0, 18, 8, 91)
				<< itemFixed(HORIZONTAL, 18)
				<< (pane(VERTICAL, ABSOLUTE_VERT, nDefaultBorder, 0, 0)
					<< item(IDC_S_TIME_STEP, ABSOLUTE_VERT)
					<< (pane(HORIZONTAL, ABSOLUTE_VERT, 0, 0, 0)
						<< item(IDC_E_TIME_STEP, ABSOLUTE_VERT|ALIGN_VCENTER)
						<< itemFixed(HORIZONTAL, 5)
						<< item(IDC_CBO_TIME_STEP_UNITS, NORESIZE)
						<< itemFixed(HORIZONTAL, 5)
						)
					)
				<< itemFixed(HORIZONTAL, 28)
				<< (pane(VERTICAL, ABSOLUTE_VERT, nDefaultBorder, 0, 0)
					<< item(IDC_S_INIT_TIME, ABSOLUTE_VERT)
					<< (pane(HORIZONTAL, ABSOLUTE_VERT, 0, 0, 0)
						<< item(IDC_E_INIT_TIME, ABSOLUTE_VERT|ALIGN_VCENTER)
						<< itemFixed(HORIZONTAL, 5)
						<< item(IDC_CBO_INIT_TIME_UNITS, NORESIZE)
						<< itemFixed(HORIZONTAL, 5)
						)
					)
				)
			<< (paneCtrl(IDC_S_OUT_OPTS, HORIZONTAL, ABSOLUTE_VERT, 0, 27, 0, 67)
				<< itemFixed(HORIZONTAL, 9)
				<< item(IDC_C_DISP_WARN, ABSOLUTE_VERT)
				)
			<< (paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, GREEDY, nDefaultBorder, 10, 10, 30)
				<< item(IDC_DESCRIPTION, GREEDY)
				)
	;
	UpdateLayout();	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKPAdvectionPg1::OnSetfocusECells() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING398);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPAdvectionPg1::OnSetfocusEShifts() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING399);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPAdvectionPg1::OnSetfocusETimeStep() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING400);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPAdvectionPg1::OnSetfocusCboTimeStepUnits() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING407);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPAdvectionPg1::OnSetfocusEInitTime() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING401);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPAdvectionPg1::OnSetfocusCboInitTimeUnits() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING407);
	m_eInputDesc.SetWindowText(strRes);	
}
void CKPAdvectionPg1::OnSetfocusBWarn() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING406);
	m_eInputDesc.SetWindowText(strRes);
}

BOOL CKPAdvectionPg1::OnHelpInfo(HELPINFO* pHelpInfo) 
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

	switch (pHelpInfo->iCtrlId)
	{
	case IDC_E_CELLS: case IDC_S_CELLS: case IDC_UD_CELLS:
		strRes.LoadString(IDS_STRING398);
		break;
	case IDC_E_SHIFTS: case IDC_S_SHIFTS: case IDC_UD_SHIFTS:
		strRes.LoadString(IDS_STRING399);
		break;
	case IDC_E_TIME_STEP: case IDC_S_TIME_STEP:
		strRes.LoadString(IDS_STRING400);
		break;
	case IDC_E_INIT_TIME: case IDC_S_INIT_TIME:
		strRes.LoadString(IDS_STRING401);
		break;
	case IDC_C_DISP_WARN:
		strRes.LoadString(IDS_STRING406);
		break;
	case IDC_CBO_TIME_STEP_UNITS: case IDC_CBO_INIT_TIME_UNITS:
		strRes.LoadString(IDS_STRING407);
		break;
	default:
		// No help topic is associated with this item. 
		strRes.LoadString(IDS_STRING441);
		// return baseCKPAdvectionPg1::OnHelpInfo(pHelpInfo);
	}
	myPopup.pszText = strRes;
	return ::HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}


/////////////////////////////////////////////////////////////////////////////
// CKPAdvectionPg2 property page

CKPAdvectionPg2::CKPAdvectionPg2() : baseCKPAdvectionPg2(CKPAdvectionPg2::IDD)
{
	//{{AFX_DATA_INIT(CKPAdvectionPg2)
	m_nPrintModulus = 1;
	m_nPunchModulus = 1;
	//}}AFX_DATA_INIT
}

CKPAdvectionPg2::~CKPAdvectionPg2()
{
}

void CKPAdvectionPg2::DDX_Ranges(CDataExchange* pDX, std::list<CRange>& listRange, CCheckListCtrl& checkListCtrl, CEditGrid& editGrid)
{
	if (pDX->m_bSaveAndValidate)
	{
		std::set<CRange> setRanges;
		for (int nRow = 1; nRow < editGrid.GetRows(); ++nRow)
		{
			CString str;
			DDX_GridText(pDX, editGrid.GetDlgCtrlID(), nRow, 1, str);
			CRange range(str);
			if (!range.IsEmpty())
			{
				if (!range.IsValid())
				{
					DDX_GridFail(pDX, _T("Invalid range."));
				}
				setRanges.insert(range);
			}
		}
		if (setRanges.size() == 0  && ((COCKSAdvection*)GetParent())->m_Page1.m_nCells > 0)
		{
			// Warning: Selecting no cells to print in effect prints all cells.  Do you want to continue printing all cells?
			if (AfxMessageBox(IDS_STRING440, MB_YESNO | MB_ICONEXCLAMATION ) == IDNO)
			{
				pDX->Fail();
			}
		}
		if (setRanges.size() == 1)
		{
			CRange range(*setRanges.begin());
			if (range.nMin == 1 && range.nMax == (int)((COCKSAdvection*)GetParent())->m_Page1.m_nCells)
			{
				setRanges.erase(setRanges.begin(), setRanges.end());
			}
		}
		listRange.erase(listRange.begin(), listRange.end());
		std::copy(setRanges.begin(), setRanges.end(), std::back_insert_iterator< std::list<CRange> >(listRange));
	}
	else
	{
		checkListCtrl.DeleteAllItems();
		CString strItem;
		for (UINT n = 1; n <= ((COCKSAdvection*)GetParent())->m_Page1.m_nCells; ++n)
		{
			strItem.Format(_T("%d"), n);
			checkListCtrl.InsertItem(n - 1, strItem);
		}
		VERIFY(checkListCtrl.SetColumnWidth(0, 60));

		if (listRange.empty())
		{
			for (int i = 0; i < checkListCtrl.GetItemCount(); ++i)
			{
				checkListCtrl.SetCheckBox(i, BST_CHECKED);
			}
			if (((COCKSAdvection*)GetParent())->m_Page1.m_nCells > 0)
			{
				CRange range(1, ((COCKSAdvection*)GetParent())->m_Page1.m_nCells);
				editGrid.SetTextMatrix(1, 1, range.toCString());
			}
		}
		else
		{
			// make space if nec
			editGrid.SetRows(max((std::list<CRange>::size_type)editGrid.GetRows(), listRange.size() + 5));

			// write ranges to grid and set checkmarks
			std::list<CRange>::const_iterator cIter = listRange.begin();
			for (int nRow = 1; nRow < editGrid.GetRows(); ++nRow)
			{
				if (cIter != listRange.end())
				{
					// Note: required to set checkbox and grid since
					// no messages are recieved within DoDataExchange
					editGrid.SetTextMatrix(nRow, 1, (*cIter).toCString());
					for (int i = cIter->nMin; i <= cIter->nMax; ++i)
					{
						checkListCtrl.SetCheckBox(i - 1, BST_CHECKED);
					}
					cIter++;
				}
				else
				{
					editGrid.SetTextMatrix(nRow, 1, _T(""));
				}
			}
		}
	}
}

void CKPAdvectionPg2::DoDataExchange(CDataExchange* pDX)
{
	baseCKPAdvectionPg2::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKPAdvectionPg2)
	DDX_Control(pDX, IDC_DESCRIPTION, m_eInputDesc);
	DDX_Control(pDX, IDC_SPIN_PUNCH, m_spinPunch);
	DDX_Control(pDX, IDC_SPIN_PRINT, m_spinPrint);
	DDX_Control(pDX, IDC_CL_PUNCH, m_clcPunch);
	DDX_Control(pDX, IDC_CL_PRINT, m_clcPrint);
	DDX_Control(pDX, IDC_MSHFG_PRINT, m_egPrint);
	DDX_Control(pDX, IDC_MSHFG_PUNCH, m_egPunch);
	DDX_Text(pDX, IDC_E_PRINT_MOD, m_nPrintModulus);
	DDV_MinMaxUInt(pDX, m_nPrintModulus, 1, INT_MAX);
	DDX_Text(pDX, IDC_E_PUNCH_MOD, m_nPunchModulus);
	DDV_MinMaxUInt(pDX, m_nPunchModulus, 1, INT_MAX);
	//}}AFX_DATA_MAP

	// rewrite ranges if nec.
	if (!pDX->m_bSaveAndValidate)
	{
		SetCells(((COCKSAdvection*)GetParent())->m_Page1.m_nCells);
	}

	DDX_Ranges(pDX, m_listPrintRange, m_clcPrint, m_egPrint);
	DDX_Ranges(pDX, m_listPunchRange, m_clcPunch, m_egPunch);

	if (m_bFirstSetActive)
	{
		InitSpinner(m_spinPrint);
		InitSpinner(m_spinPunch);
	}

	if (!pDX->m_bSaveAndValidate)
	{
		// do everytime
		InitEditGrid(m_egPrint);
		InitEditGrid(m_egPunch);
	}
}

bool CKPAdvectionPg2::SetCells(size_t stCells)
{
	ASSERT(stCells > 0);

	//
	// reset print ranges
	//
	if (!m_listPrintRange.empty())
	{
		std::list<CRange>::reverse_iterator lrrIter = m_listPrintRange.rbegin();
		for (; lrrIter != m_listPrintRange.rend(); )
		{
			if (stCells < (size_t)lrrIter->nMin)
			{
				// range is outside; remove it
				m_listPrintRange.pop_back();
				lrrIter = m_listPrintRange.rbegin();
			}
			else
			{
				if (lrrIter->Contains(stCells))
					lrrIter->nMax = stCells;
				break; // done
			}
		}

	}
	// check if list is needed
	if (m_listPrintRange.size() == 1)
	{
		if (m_listPrintRange.back().nMin == 1 && (size_t)m_listPrintRange.back().nMax == stCells)
		{
			m_listPrintRange.pop_back();
		}		
	}

	//
	// reset punch ranges
	//
	if (!m_listPrintRange.empty())
	{
		std::list<CRange>::reverse_iterator lrrIter = m_listPunchRange.rbegin();
		for (; lrrIter != m_listPunchRange.rend(); )
		{
			if (stCells < (size_t)lrrIter->nMin)
			{
				// range is outside; remove it
				m_listPunchRange.pop_back();
				lrrIter = m_listPunchRange.rbegin();
			}
			else
			{
				if (lrrIter->Contains(stCells))
					lrrIter->nMax = stCells;
				break; // done
			}
		}

	}
	// check if list is needed
	if (m_listPunchRange.size() == 1)
	{
		if (m_listPunchRange.back().nMin == 1 && (size_t)m_listPunchRange.back().nMax == stCells)
		{
			m_listPunchRange.pop_back();
		}		
	}
	return true;
}

void CKPAdvectionPg2::InitSpinner(CSpinButtonCtrl& spinner)
{
	UDACCEL uda[7];
	uda[0].nSec = 0;
	uda[0].nInc = 1;
	uda[1].nSec = 2;
	uda[1].nInc = 2;
	uda[2].nSec = 4;
	uda[2].nInc = 10;
	uda[3].nSec = 8;
	uda[3].nInc = 100;
	uda[4].nSec = 16;
	uda[4].nInc = 1000;
	uda[5].nSec = 32;
	uda[5].nInc = 10000;
	uda[6].nSec = 64;
	uda[6].nInc = 100000;

	// init spinners
	spinner.SetRange32(1, INT_MAX - 1);
	spinner.SetAccel(7, uda);
}

void CKPAdvectionPg2::InitEditGrid(CEditGrid& editGrid)
{
	// add col and row headings
	editGrid.AddRowHeaders();
	editGrid.SetTextMatrix(0, 1, _T("Cell"));

	// set col widths
	editGrid.SetColWidth(1, 0, 1400);

	// center align the column headers
	editGrid.SetFillStyle(flexFillRepeat);
	editGrid.SetRow(0);
	editGrid.SetCol(0);
	editGrid.SetColSel(editGrid.GetCols(0) - 1);
	editGrid.SetCellAlignment(flexAlignCenterCenter);
	editGrid.SetFillStyle(flexFillSingle);

	// set bold titles
	editGrid.SetRow(0), editGrid.SetCol(1);
	editGrid.SetCellFontBold(TRUE);

	// set initial cell
	editGrid.SetRow(1), editGrid.SetCol(1);
}


BEGIN_MESSAGE_MAP(CKPAdvectionPg2, baseCKPAdvectionPg2)
	//{{AFX_MSG_MAP(CKPAdvectionPg2)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CL_PRINT, OnItemchangedClPrint)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CL_PUNCH, OnItemchangedClPunch)
	ON_NOTIFY(NM_SETFOCUS, IDC_CL_PRINT, OnSetfocusClPrint)
	ON_EN_SETFOCUS(IDC_E_PRINT_MOD, OnSetfocusEPrintMod)
	ON_EN_SETFOCUS(IDC_E_PUNCH_MOD, OnSetfocusEPunchMod)
	ON_NOTIFY(NM_SETFOCUS, IDC_CL_PUNCH, OnSetfocusClPunch)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
	// custom edit grid notifications
	// ON_MESSAGE(EGN_BEGINCELLEDIT, OnBeginCellEdit)
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
	ON_MESSAGE(EGN_SETFOCUS, OnSetfocusGrid)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CKPAdvectionPg2, baseCKPAdvectionPg2)
    //{{AFX_EVENTSINK_MAP(CKPAdvectionPg2)
	ON_EVENT(CKPAdvectionPg2, IDC_MSHFG_PRINT, -602 /* KeyDown */, OnKeyDownMshfgPrint, VTS_PI2 VTS_I2)
	ON_EVENT(CKPAdvectionPg2, IDC_MSHFG_PUNCH, -602 /* KeyDown */, OnKeyDownMshfgPunch, VTS_PI2 VTS_I2)
	ON_EVENT(CKPAdvectionPg2, IDC_MSHFG_PRINT, 71 /* EnterCell */, OnEnterCellMshfgPrint, VTS_NONE)
	ON_EVENT(CKPAdvectionPg2, IDC_MSHFG_PUNCH, 71 /* EnterCell */, OnEnterCellMshfgPunch, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BOOL CKPAdvectionPg2::OnInitDialog() 
{
	baseCKPAdvectionPg2::OnInitDialog();
	
	// set layout
	CreateRoot(VERTICAL, 5, 6) 
			<< (paneCtrl(IDC_S_PRINT, VERTICAL, GREEDY, nDefaultBorder, 10, 10, 30)
				<< (pane(HORIZONTAL, GREEDY, 0, 0, 0)
					<< item(IDC_CL_PRINT, GREEDY)
					<< itemFixed(HORIZONTAL, 25)
					<< item(IDC_MSHFG_PRINT, ABSOLUTE_HORZ)
					)
				<< (pane(HORIZONTAL, ABSOLUTE_VERT, 0, 0, 0)
					<< itemGrowing(HORIZONTAL)
					<< item(IDC_S_PRINT_MOD, NORESIZE|ALIGN_VCENTER)
					<< item(IDC_E_PRINT_MOD, NORESIZE|ALIGN_VCENTER)
					<< item(IDC_SPIN_PRINT, NORESIZE)
					)
				)
			<< (paneCtrl(IDC_S_PUNCH, VERTICAL, GREEDY, nDefaultBorder, 10, 10, 30)
				<< (pane(HORIZONTAL, GREEDY, 0, 0, 0)
					<< item(IDC_CL_PUNCH, GREEDY)
					<< itemFixed(HORIZONTAL, 25)
					<< item(IDC_MSHFG_PUNCH, ABSOLUTE_HORZ)
					)
				<< (pane(HORIZONTAL, ABSOLUTE_VERT, 0, 0, 0)
					<< itemGrowing(HORIZONTAL)
					<< item(IDC_S_PUNCH_MOD, NORESIZE|ALIGN_VCENTER)
					<< item(IDC_E_PUNCH_MOD, NORESIZE|ALIGN_VCENTER)
					<< item(IDC_SPIN_PUNCH, NORESIZE)
					)
				)
			<< (paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, GREEDY, nDefaultBorder, 10, 10, 30)
				<< item(IDC_DESCRIPTION, GREEDY)
				)
	;
	UpdateLayout();	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CKPAdvectionPg2::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bMakeChange = TRUE;

	// ignore if edit is cancelled
	if ( pInfo->item.pszText == NULL ) return bMakeChange;

	switch (nID)
	{
	case IDC_MSHFG_PRINT :
		bMakeChange = OnEndCellEdit(pInfo, m_clcPrint, m_egPrint);
		break;
	case IDC_MSHFG_PUNCH :
		bMakeChange = OnEndCellEdit(pInfo, m_clcPunch, m_egPunch);
		break;
	}
	return bMakeChange;
}

LRESULT CKPAdvectionPg2::OnEndCellEdit(NMEGINFO* pInfo, CCheckListCtrl& checkListCtrl, CEditGrid& editGrid)
{
	BOOL bMakeChange = FALSE; // changes are made by OnItemchanged

	CRange rangeNew(pInfo->item.pszText);

	if (!rangeNew.IsValid() && !rangeNew.IsEmpty())
	{
		// Invalid range of cells.
		::AfxMessageBox(IDS_STRING439);
	}
	else
	{
		CRange rangeOld(editGrid.GetTextMatrix(pInfo->item.iRow, 1));
		if (!rangeOld.IsEmpty() && rangeOld.IsValid())
		{
			for (int i = rangeOld.nMin; i <= rangeOld.nMax; ++i)
			{
				checkListCtrl.SetCheckBox(i - 1, BST_UNCHECKED);
			}
		}
		if (!rangeNew.IsEmpty())
		{
			for (int i = rangeNew.nMin; i <= rangeNew.nMax; ++i)
			{
				checkListCtrl.SetCheckBox(i - 1, BST_CHECKED);
			}
		}
	}
	return bMakeChange;
}

void CKPAdvectionPg2::OnItemchangedClPrint(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnItemchanged((NM_LISTVIEW*)pNMHDR, m_clcPrint, m_egPrint);
	*pResult = 0;
}

void CKPAdvectionPg2::OnItemchangedClPunch(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnItemchanged((NM_LISTVIEW*)pNMHDR, m_clcPunch, m_egPunch);
	*pResult = 0;
}

void CKPAdvectionPg2::OnItemchanged(NM_LISTVIEW* pNMListView, CCheckListCtrl& checkListCtrl, CEditGrid& editGrid) 
{
	if ((pNMListView->uNewState ^ pNMListView->uOldState) & LVIS_STATEIMAGEMASK)
	{
		int nVal = atoi(checkListCtrl.GetItemText(pNMListView->iItem, 0));
		switch (checkListCtrl.GetCheckBox(pNMListView->iItem))
		{
		case BST_CHECKED :
			{
				bool bRangeFound = false;
				std::set<CRange> setRanges;
				int nRow = 1;
				for (; nRow < editGrid.GetRows(); ++nRow)
				{
					CRange range(editGrid.GetTextMatrix(nRow, 1));
					if (!range.IsEmpty() && range.IsValid())
					{
						if (!bRangeFound)
						{
							if (nVal == range.nMin - 1)
							{
								bRangeFound = true;
								range.nMin = nVal;
							}
							else if (nVal == range.nMax + 1)
							{
								bRangeFound = true;
								range.nMax = nVal;
							}
						}
						setRanges.insert(range);
					}
				}

				if (!bRangeFound)
				{
					// insert as new range
					CRange insert(nVal, nVal);
					setRanges.insert(insert);
				}

				// check for ranges that can be merged
				std::set<CRange>::iterator iter = setRanges.begin();
				std::set<CRange>::iterator prev = iter++;
				for (; iter != setRanges.end(); prev = ++iter)
				{
					if (prev->nMax + 1 == iter->nMin)
					{
						prev->nMax = iter->nMax;
						iter = setRanges.erase(iter);
					}
				}

				// write ranges to grid
				std::set<CRange>::const_iterator cIter = setRanges.begin();
				for (nRow = 1; nRow < editGrid.GetRows(); ++nRow)
				{
					if (cIter != setRanges.end())
					{
						editGrid.SetTextMatrix(nRow, 1, (*cIter).toCString());
						cIter++;
					}
					else
					{
						editGrid.SetTextMatrix(nRow, 1, _T(""));
					}
				}
			}
			break;

		case BST_UNCHECKED :
			{
				for (int nRow = 0; nRow < editGrid.GetRows(); ++nRow)
				{
					CRange range(editGrid.GetTextMatrix(nRow, 1));
					if (!range.IsEmpty() && range.IsValid() && range.Contains(nVal))
					{
						CRange newRange = range.Remove(nVal);
						if (range.IsEmpty())
						{
							if (newRange.IsEmpty())
							{
								editGrid.DeleteRow(nRow);
							}
							else
							{
								editGrid.SetTextMatrix(nRow, 1, newRange.toCString());
							}
						}
						else
						{
							editGrid.SetTextMatrix(nRow, 1, range.toCString());
							if (!newRange.IsEmpty())
							{
								// insert new row
								editGrid.SetRows(editGrid.GetRows() + 1);
								CString strRowTitle;
								strRowTitle.Format("%d", (int)editGrid.GetRows() - 1);
								editGrid.SetTextMatrix(editGrid.GetRows() - 1, 0, strRowTitle);
								for (int i = editGrid.GetRows() - 1; i > nRow; --i)
								{
									editGrid.SetTextMatrix(i, 1, editGrid.GetTextMatrix(i - 1, 1));
								}
								editGrid.SetTextMatrix(nRow + 1, 1, newRange.toCString());
							}
						}
						break;
					}
				}
			}
			break;
		}
	}	
}

void CKPAdvectionPg2::OnKeyDownMshfgPrint(short FAR* KeyCode, short Shift) 
{
	OnKeyDownMshfg(KeyCode, Shift, m_clcPrint, m_egPrint);	
}

void CKPAdvectionPg2::OnKeyDownMshfgPunch(short FAR* KeyCode, short Shift) 
{
	OnKeyDownMshfg(KeyCode, Shift, m_clcPunch, m_egPunch);	
}

void CKPAdvectionPg2::OnKeyDownMshfg(short FAR* KeyCode, short Shift, CCheckListCtrl& checkListCtrl, CEditGrid& editGrid)
{
	UNUSED(Shift);
	switch (*KeyCode)
	{
	case VK_DELETE :
		{
			std::list<CRange> ranges;
			for (long nRow = editGrid.GetRow(); nRow <= editGrid.GetRowSel(); ++nRow)
			{
				CRange range(editGrid.GetTextMatrix(nRow, editGrid.GetCol()));
				if (!range.IsEmpty() && range.IsValid())
				{
					ranges.push_back(range);
				}
			}
			std::list<CRange>::const_iterator iter = ranges.begin();
			for (; iter != ranges.end(); ++iter)
			{
				for (int i = iter->nMin; i <= iter->nMax; ++i)
				{
					checkListCtrl.SetCheckBox(i - 1, BST_UNCHECKED);
				}
			}
		}
		break;
	}	
}

void CKPAdvectionPg2::OnSetfocusClPrint(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNUSED(pNMHDR);

	CString strRes;
	strRes.LoadString(IDS_STRING408);
	m_eInputDesc.SetWindowText(strRes);

	// set default selection
	// this gives user feedback when the list recieves the focus
	if (m_clcPrint.GetItemCount() && m_clcPrint.GetFirstSelectedItemPosition() == NULL)
	{
		m_clcPrint.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}
	
	*pResult = 0;
}

void CKPAdvectionPg2::OnSetfocusEPrintMod() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING403);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPAdvectionPg2::OnSetfocusEPunchMod() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING405);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPAdvectionPg2::OnSetfocusClPunch(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNUSED(pNMHDR);

	CString strRes;
	strRes.LoadString(IDS_STRING409);
	m_eInputDesc.SetWindowText(strRes);

	// set default selection
	// this gives user feedback when the list recieves the focus
	if (m_clcPunch.GetItemCount() && m_clcPunch.GetFirstSelectedItemPosition() == NULL)
	{
		m_clcPunch.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}
	
	*pResult = 0;
}

LRESULT CKPAdvectionPg2::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
{
	UINT nID = wParam;
	UNUSED_ALWAYS(lParam);

	switch (nID)
	{
	case IDC_MSHFG_PRINT :
		OnEnterCellMshfgPrint();
		break;

	case IDC_MSHFG_PUNCH :
		OnEnterCellMshfgPunch();
		break;
	default :
		ASSERT(FALSE);
		break;
	}
	return 0;
}

void CKPAdvectionPg2::OnEnterCellMshfgPrint() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING402);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPAdvectionPg2::OnEnterCellMshfgPunch() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING404);
	m_eInputDesc.SetWindowText(strRes);	
}

BOOL CKPAdvectionPg2::OnHelpInfo(HELPINFO* pHelpInfo) 
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
	switch (pHelpInfo->iCtrlId)
	{
	case IDC_MSHFG_PRINT:
		strRes.LoadString(IDS_STRING402);
		break;
	case IDC_MSHFG_PUNCH:
		strRes.LoadString(IDS_STRING404);
		break;
	case IDC_CL_PRINT:
		strRes.LoadString(IDS_STRING408);
		break;
	case IDC_CL_PUNCH:
		strRes.LoadString(IDS_STRING409);
		break;
	case IDC_E_PRINT_MOD: case IDC_S_PRINT_MOD: case IDC_SPIN_PRINT:
		strRes.LoadString(IDS_STRING403);
		break;
	case IDC_E_PUNCH_MOD: case IDC_S_PUNCH_MOD: case IDC_SPIN_PUNCH:
		strRes.LoadString(IDS_STRING405);
		break;
	default:
		// No help topic is associated with this item. 
		strRes.LoadString(IDS_STRING441);
		// return baseCKPAdvectionPg2::OnHelpInfo(pHelpInfo);
	}
	myPopup.pszText = strRes;
	return ::HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}
