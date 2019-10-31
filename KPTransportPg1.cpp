// KPTransportPg1.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "phrqtype.h"          // LDBLE
#include "KPTransportPg1.h"

#include "OCKSTransport.h"
#include "SaveCurrentDirectory.h"


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CKPTransportPg1, baseCKPTransportPg1)
IMPLEMENT_DYNCREATE(CKPTransportPg2, baseCKPTransportPg2)
IMPLEMENT_DYNCREATE(CKPTransportPg3, baseCKPTransportPg3)
IMPLEMENT_DYNCREATE(CKPTransportPg4, baseCKPTransportPg4)
IMPLEMENT_DYNCREATE(CKPTransportPg5, baseCKPTransportPg5)


/////////////////////////////////////////////////////////////////////////////
// CKPTransportPg1 property page

CKPTransportPg1::CKPTransportPg1() : baseCKPTransportPg1(CKPTransportPg1::IDD)
{
	//{{AFX_DATA_INIT(CKPTransportPg1)
	m_nCells = 1;
	m_nShifts = 1;
	m_bPrintWarnings = TRUE;
	m_dTimeStep = 0.0;
	m_dInitTime = 0.0;
	//}}AFX_DATA_INIT
	m_tuTimeStep = TU_SECONDS;
	m_tuInitTime = TU_SECONDS;
}

CKPTransportPg1::~CKPTransportPg1()
{
}

void CKPTransportPg1::DoDataExchange(CDataExchange* pDX)
{
	baseCKPTransportPg1::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKPTransportPg1)
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

		// TODO: Update -lengths and - dispersivities
	}
	else
	{
		m_cboTimeStepUnits.SetCurSel((int)m_tuTimeStep);
		m_cboInitTimeUnits.SetCurSel((int)m_tuInitTime);
	}
}


BEGIN_MESSAGE_MAP(CKPTransportPg1, baseCKPTransportPg1)
	//{{AFX_MSG_MAP(CKPTransportPg1)
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

BOOL CKPTransportPg1::OnInitDialog() 
{
	baseCKPTransportPg1::OnInitDialog();
	
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
void CKPTransportPg1::OnSetfocusECells() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING411);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPTransportPg1::OnSetfocusEShifts() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING412);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPTransportPg1::OnSetfocusETimeStep() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING413);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPTransportPg1::OnSetfocusCboTimeStepUnits() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING420);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPTransportPg1::OnSetfocusEInitTime() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING414);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPTransportPg1::OnSetfocusCboInitTimeUnits() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING420);
	m_eInputDesc.SetWindowText(strRes);	
}
void CKPTransportPg1::OnSetfocusBWarn() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING419);
	m_eInputDesc.SetWindowText(strRes);
}

BOOL CKPTransportPg1::OnHelpInfo(HELPINFO* pHelpInfo) 
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
		strRes.LoadString(IDS_STRING411);
		break;
	case IDC_E_SHIFTS: case IDC_S_SHIFTS: case IDC_UD_SHIFTS:
		strRes.LoadString(IDS_STRING412);
		break;
	case IDC_E_TIME_STEP: case IDC_S_TIME_STEP:
		strRes.LoadString(IDS_STRING413);
		break;
	case IDC_E_INIT_TIME: case IDC_S_INIT_TIME:
		strRes.LoadString(IDS_STRING414);
		break;
	case IDC_C_DISP_WARN:
		strRes.LoadString(IDS_STRING419);
		break;
	case IDC_CBO_TIME_STEP_UNITS: case IDC_CBO_INIT_TIME_UNITS:
		strRes.LoadString(IDS_STRING420);
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
// CKPTransportPg2 property page

CKPTransportPg2::CKPTransportPg2() : baseCKPTransportPg2(CKPTransportPg2::IDD)
{
	//{{AFX_DATA_INIT(CKPTransportPg2)
	m_nPrintModulus = 1;
	m_nPunchModulus = 1;
	//}}AFX_DATA_INIT
}

CKPTransportPg2::~CKPTransportPg2()
{
}

void CKPTransportPg2::DDX_Ranges(CDataExchange* pDX, std::list<CRange>& listRange, CCheckListCtrl& checkListCtrl, CEditGrid& editGrid)
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
					// Invalid range of cells.
					DDX_GridFail(pDX, IDS_STRING439);
				}
				setRanges.insert(range);
			}
		}
		if (setRanges.size() == 0  && ((COCKSTransport*)GetParent())->m_Page1.m_nCells > 0)
		{
			// Warning: Selecting no cells to print in effect prints all cells.  Do you want to continue printing all cells?
			if (::AfxMessageBox(IDS_STRING440, MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
			{
				pDX->Fail();
			}
		}
		if (((COCKSTransport*)GetParent())->m_Page5.m_bUseStagnant)
		{
			if (setRanges.size() == 2)
			{
				int nCells = ((COCKSTransport*)GetParent())->m_Page1.m_nCells;
				int nStags = ((COCKSTransport*)GetParent())->m_Page5.m_nStagCells;
				ASSERT(nStags > 0);
				std::set<CRange>::iterator iter = setRanges.begin();
				CRange first(*iter);
				++iter;
				CRange second(*iter);
				if (first.nMin == 1 && first.nMax == nCells
					&&
					second.nMin == nCells + 2 && second.nMax == (nStags + 1) * nCells + 1
					)
				{
					setRanges.erase(setRanges.begin(), setRanges.end());
				}
			}
		}
		else
		{
			if (setRanges.size() == 1)
			{
				CRange range(*setRanges.begin());
				if (range.nMin == 1 && range.nMax == (int)((COCKSTransport*)GetParent())->m_Page1.m_nCells)
				{
					setRanges.erase(setRanges.begin(), setRanges.end());
				}
			}
		}
		listRange.erase(listRange.begin(), listRange.end());
		std::copy(setRanges.begin(), setRanges.end(), std::back_insert_iterator< std::list<CRange> >(listRange));
	}
	else
	{
		checkListCtrl.DeleteAllItems();
		CString strItem;
		for (UINT n = 1; n <= ((COCKSTransport*)GetParent())->m_Page1.m_nCells; ++n)
		{
			strItem.Format(_T("%d"), n);
			checkListCtrl.InsertItem(n - 1, strItem);
		}
		// add stagnant layers
		if (((COCKSTransport*)GetParent())->m_Page5.m_bUseStagnant) {
			for (UINT sl = 1; sl <= ((COCKSTransport*)GetParent())->m_Page5.m_nStagCells; ++sl)
			{
				UINT cells = ((COCKSTransport*)GetParent())->m_Page1.m_nCells;
				for (UINT n = 1; n <= cells; ++n)
				{
					strItem.Format(_T("sl%d %d"), sl, (sl * cells) + n + 1);
					checkListCtrl.InsertItem((sl * cells) + n, strItem);
				}
			}
		}
		VERIFY(checkListCtrl.SetColumnWidth(0, 60));

		if (listRange.empty())
		{
			for (int i = 0; i < checkListCtrl.GetItemCount(); ++i)
			{
				checkListCtrl.SetCheckBox(i, BST_CHECKED);
			}
			if (((COCKSTransport*)GetParent())->m_Page1.m_nCells > 0)
			{
				CRange range(1, ((COCKSTransport*)GetParent())->m_Page1.m_nCells);
				editGrid.SetTextMatrix(1, 1, range.toCString());
			}
			// add stagnant range
			if (((COCKSTransport*)GetParent())->m_Page5.m_bUseStagnant) {
				if (((COCKSTransport*)GetParent())->m_Page5.m_nStagCells > 0)
				{
					CRange range(((COCKSTransport*)GetParent())->m_Page1.m_nCells + 2,
						(((COCKSTransport*)GetParent())->m_Page5.m_nStagCells + 1) * ((COCKSTransport*)GetParent())->m_Page1.m_nCells + 1);
					editGrid.SetTextMatrix(2, 1, range.toCString());
				}
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
						int cells = ((COCKSTransport*)GetParent())->m_Page1.m_nCells;
						if (i <= cells)
						{
							checkListCtrl.SetCheckBox(i - 1, BST_CHECKED);
						}
						else if (i > cells + 1)
						{
							checkListCtrl.SetCheckBox(i - 2, BST_CHECKED);
						}
						else
						{
							TRACE("\n");
						}
					}
					++cIter;
				}
				else
				{
					editGrid.SetTextMatrix(nRow, 1, _T(""));
				}
			}
		}
	}
}

void CKPTransportPg2::DoDataExchange(CDataExchange* pDX)
{
	baseCKPTransportPg2::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKPTransportPg2)
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
		if (((COCKSTransport*)GetParent())->m_Page5.m_bUseStagnant)
		{
			SetCells(((COCKSTransport*)GetParent())->m_Page1.m_nCells, ((COCKSTransport*)GetParent())->m_Page5.m_nStagCells);
		}
		else
		{
			SetCells(((COCKSTransport*)GetParent())->m_Page1.m_nCells, 0);
		}
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
		// init everytime
		InitEditGrid(m_egPrint);
		InitEditGrid(m_egPunch);
	}

}

bool CKPTransportPg2::SetCells(size_t stCells, size_t stStags)
{
	ASSERT(stCells > 0);

	//
	// reset print ranges
	//
	if (!m_listPrintRange.empty())
	{
		if (stStags == 0)
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
		else
		{
			std::list<CRange>::reverse_iterator lrrIter = m_listPrintRange.rbegin();
			int dead = stCells + 1;
			for (; lrrIter != m_listPrintRange.rend(); ++lrrIter)
			{
				// check range for dead cell
				if (lrrIter->nMin == dead) {
					lrrIter->nMin = dead + 1;
				}
				if (lrrIter->nMax == dead) {
					lrrIter->nMax = dead - 1;
				}
			}

			lrrIter = m_listPrintRange.rbegin();
			for (; lrrIter != m_listPrintRange.rend(); )
			{

				if ((stStags + 1) * stCells + 1 < (size_t)lrrIter->nMin)
				{
					// range is outside; remove it
					m_listPrintRange.pop_back();
					lrrIter = m_listPrintRange.rbegin();
				}
				else
				{
					if (lrrIter->Contains((stStags + 1) * stCells + 1)) {
						lrrIter->nMax = (stStags + 1) * stCells + 1;
					}
					break; // done
				}
			}
		}
	}

	// check if list is needed
	if (stStags > 0)
	{
		if (m_listPrintRange.size() == 1)
		{
			// remove if it contains the entire range along with the dead cell
			if (m_listPrintRange.back().nMin == 1 && m_listPrintRange.back().nMax == (int)((stStags + 1) * stCells + 1))
			{
				m_listPrintRange.pop_back();
			}
		}
		else if (m_listPrintRange.size() == 2)
		{
			std::list<CRange>::iterator iter = m_listPrintRange.begin();
			CRange first(*iter);
			++iter;
			CRange second(*iter);
			if (first.nMin == 1 && first.nMax == (int)stCells
				&&
				second.nMin == (int)(stCells + 2) && second.nMax == (int)((stStags + 1) * stCells + 1)
				)
			{
				m_listPrintRange.erase(m_listPrintRange.begin(), m_listPrintRange.end());
			}
		}
	}
	else
	{
		if (m_listPrintRange.size() == 1)
		{
			if (m_listPrintRange.back().nMin == 1 && (size_t)m_listPrintRange.back().nMax == stCells)
			{
				m_listPrintRange.pop_back();
			}
		}
	}

	//
	// reset punch ranges
	//
	if (!m_listPrintRange.empty())
	{
		if (stStags == 0)
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
		else
		{
			std::list<CRange>::reverse_iterator lrrIter = m_listPunchRange.rbegin();
			int dead = stCells + 1;
			for (; lrrIter != m_listPunchRange.rend(); ++lrrIter)
			{
				// check range for dead cell
				if (lrrIter->nMin == dead) {
					lrrIter->nMin = dead + 1;
				}
				if (lrrIter->nMax == dead) {
					lrrIter->nMax = dead - 1;
				}
			}

			lrrIter = m_listPunchRange.rbegin();
			for (; lrrIter != m_listPunchRange.rend(); )
			{

				if ((stStags + 1) * stCells + 1 < (size_t)lrrIter->nMin)
				{
					// range is outside; remove it
					m_listPunchRange.pop_back();
					lrrIter = m_listPunchRange.rbegin();
				}
				else
				{
					if (lrrIter->Contains((stStags + 1) * stCells + 1)) {
						lrrIter->nMax = (stStags + 1) * stCells + 1;
					}
					break; // done
				}
			}
		}
	}
	// check if list is needed
	if (stStags > 0)
	{
		if (m_listPunchRange.size() == 1)
		{
			// remove if it contains the entire range along with the dead cell
			if (m_listPunchRange.back().nMin == 1 && m_listPunchRange.back().nMax == (int)((stStags + 1) * stCells + 1))
			{
				m_listPunchRange.pop_back();
			}
		}
		else if (m_listPunchRange.size() == 2)
		{
			std::list<CRange>::iterator iter = m_listPunchRange.begin();
			CRange first(*iter);
			++iter;
			CRange second(*iter);
			if (first.nMin == 1 && first.nMax == (int)stCells
				&&
				second.nMin == (int)(stCells + 2) && second.nMax == (int)((stStags + 1) * stCells + 1)
				)
			{
				m_listPunchRange.erase(m_listPunchRange.begin(), m_listPunchRange.end());
			}
		}
	}
	else
	{
		if (m_listPunchRange.size() == 1)
		{
			if (m_listPunchRange.back().nMin == 1 && (size_t)m_listPunchRange.back().nMax == stCells)
			{
				m_listPunchRange.pop_back();
			}
		}
	}

	return true;
}

void CKPTransportPg2::InitSpinner(CSpinButtonCtrl& spinner)
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

void CKPTransportPg2::InitEditGrid(CEditGrid& editGrid)
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

BEGIN_MESSAGE_MAP(CKPTransportPg2, baseCKPTransportPg2)
	//{{AFX_MSG_MAP(CKPTransportPg2)
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

BEGIN_EVENTSINK_MAP(CKPTransportPg2, baseCKPTransportPg2)
    //{{AFX_EVENTSINK_MAP(CKPTransportPg2)
	ON_EVENT(CKPTransportPg2, IDC_MSHFG_PRINT, -602 /* KeyDown */, OnKeyDownMshfgPrint, VTS_PI2 VTS_I2)
	ON_EVENT(CKPTransportPg2, IDC_MSHFG_PUNCH, -602 /* KeyDown */, OnKeyDownMshfgPunch, VTS_PI2 VTS_I2)
	ON_EVENT(CKPTransportPg2, IDC_MSHFG_PRINT, 71 /* EnterCell */, OnEnterCellMshfgPrint, VTS_NONE)
	ON_EVENT(CKPTransportPg2, IDC_MSHFG_PUNCH, 71 /* EnterCell */, OnEnterCellMshfgPunch, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BOOL CKPTransportPg2::OnInitDialog() 
{
	baseCKPTransportPg2::OnInitDialog();
	
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

LRESULT CKPTransportPg2::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
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

LRESULT CKPTransportPg2::OnEndCellEdit(NMEGINFO* pInfo, CCheckListCtrl& checkListCtrl, CEditGrid& editGrid)
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
		int cells = ((COCKSTransport*)GetParent())->m_Page1.m_nCells;
		CRange rangeOld(editGrid.GetTextMatrix(pInfo->item.iRow, 1));
		if (!rangeOld.IsEmpty() && rangeOld.IsValid())
		{
			if (((COCKSTransport*)GetParent())->m_Page5.m_bUseStagnant)
			{
				for (int i = rangeOld.nMin; i <= rangeOld.nMax; ++i)
				{
					if (i <= cells)
					{
						checkListCtrl.SetCheckBox(i - 1, BST_UNCHECKED);
					}
					else if (i > cells + 1)
					{
						checkListCtrl.SetCheckBox(i - 2, BST_UNCHECKED);
					}
				}
			}
			else
			{
				for (int i = rangeOld.nMin; i <= rangeOld.nMax; ++i)
				{
					checkListCtrl.SetCheckBox(i - 1, BST_UNCHECKED);
				}
			}
		}
		if (!rangeNew.IsEmpty())
		{
			if (((COCKSTransport*)GetParent())->m_Page5.m_bUseStagnant)
			{
				for (int i = rangeNew.nMin; i <= rangeNew.nMax; ++i)
				{
					if (i <= cells)
					{
						checkListCtrl.SetCheckBox(i - 1, BST_CHECKED);
					}
					else if (i > cells + 1)
					{
						checkListCtrl.SetCheckBox(i - 2, BST_CHECKED);
					}
				}
			}
			else
			{
				for (int i = rangeNew.nMin; i <= rangeNew.nMax; ++i)
				{
					checkListCtrl.SetCheckBox(i - 1, BST_CHECKED);
				}
			}
		}
	}
	return bMakeChange;
}

void CKPTransportPg2::OnItemchangedClPrint(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnItemchanged((NM_LISTVIEW*)pNMHDR, m_clcPrint, m_egPrint);
	*pResult = 0;
}

void CKPTransportPg2::OnItemchangedClPunch(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnItemchanged((NM_LISTVIEW*)pNMHDR, m_clcPunch, m_egPunch);
	*pResult = 0;
}

void CKPTransportPg2::OnItemchanged(NM_LISTVIEW* pNMListView, CCheckListCtrl& checkListCtrl, CEditGrid& editGrid) 
{
	if ((pNMListView->uNewState ^ pNMListView->uOldState) & LVIS_STATEIMAGEMASK)
	{
		int nVal = pNMListView->iItem + 1;
		if (nVal > (int)((COCKSTransport*)GetParent())->m_Page1.m_nCells)
		{
			ASSERT(((COCKSTransport*)GetParent())->m_Page5.m_bUseStagnant);
			++nVal;
		}
		switch (checkListCtrl.GetCheckBox(pNMListView->iItem))
		{
		case BST_CHECKED :
			{
				bool bRangeFound = false;
				std::set<CRange> setRanges;
				for (int nRow = 1; nRow < editGrid.GetRows(); ++nRow)
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
				for (; iter != setRanges.end(); prev = iter++)
				{
					if (prev->nMax + 1 == iter->nMin)
					{
						prev->nMax = iter->nMax;
						iter = setRanges.erase(iter);
						// Note: can't increment the end() iterator
						if (iter == setRanges.end()) break;
					}
				}

				// write ranges to grid
				std::set<CRange>::const_iterator cIter = setRanges.begin();
				for (int nRow = 1; nRow < editGrid.GetRows(); ++nRow)
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

void CKPTransportPg2::OnKeyDownMshfgPrint(short FAR* KeyCode, short Shift) 
{
	OnKeyDownMshfg(KeyCode, Shift, m_clcPrint, m_egPrint);	
}

void CKPTransportPg2::OnKeyDownMshfgPunch(short FAR* KeyCode, short Shift) 
{
	OnKeyDownMshfg(KeyCode, Shift, m_clcPunch, m_egPunch);	
}

void CKPTransportPg2::OnKeyDownMshfg(short FAR* KeyCode, short Shift, CCheckListCtrl& checkListCtrl, CEditGrid& editGrid)
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
				int cells = ((COCKSTransport*)GetParent())->m_Page1.m_nCells;
				for (int i = iter->nMin; i <= iter->nMax; ++i)
				{
					if (i <= cells)
					{
						checkListCtrl.SetCheckBox(i - 1, BST_UNCHECKED);
					}
					else if (i > cells + 1)
					{
						checkListCtrl.SetCheckBox(i - 2, BST_UNCHECKED);
					}
				}
			}
		}
		break;
	}	
}

void CKPTransportPg2::OnSetfocusClPrint(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNUSED(pNMHDR);

	CString strRes;
	strRes.LoadString(IDS_STRING421);
	m_eInputDesc.SetWindowText(strRes);

	// set default selection
	// this gives user feedback when the list recieves the focus
	if (m_clcPrint.GetItemCount() && m_clcPrint.GetFirstSelectedItemPosition() == NULL)
	{
		m_clcPrint.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}
	
	*pResult = 0;
}

void CKPTransportPg2::OnSetfocusEPrintMod() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING416);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPTransportPg2::OnSetfocusEPunchMod() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING418);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPTransportPg2::OnSetfocusClPunch(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNUSED(pNMHDR);

	CString strRes;
	strRes.LoadString(IDS_STRING422);
	m_eInputDesc.SetWindowText(strRes);

	// set default selection
	// this gives user feedback when the list recieves the focus
	if (m_clcPunch.GetItemCount() && m_clcPunch.GetFirstSelectedItemPosition() == NULL)
	{
		m_clcPunch.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}
	
	*pResult = 0;
}

LRESULT CKPTransportPg2::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
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

void CKPTransportPg2::OnEnterCellMshfgPrint() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING415);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPTransportPg2::OnEnterCellMshfgPunch() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING417);
	m_eInputDesc.SetWindowText(strRes);	
}

BOOL CKPTransportPg2::OnHelpInfo(HELPINFO* pHelpInfo) 
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
		strRes.LoadString(IDS_STRING415);
		break;
	case IDC_MSHFG_PUNCH:
		strRes.LoadString(IDS_STRING417);
		break;
	case IDC_CL_PRINT:
		strRes.LoadString(IDS_STRING421);
		break;
	case IDC_CL_PUNCH:
		strRes.LoadString(IDS_STRING422);
		break;
	case IDC_E_PRINT_MOD: case IDC_S_PRINT_MOD: case IDC_SPIN_PRINT:
		strRes.LoadString(IDS_STRING416);
		break;
	case IDC_E_PUNCH_MOD: case IDC_S_PUNCH_MOD: case IDC_SPIN_PUNCH:
		strRes.LoadString(IDS_STRING418);
		break;
	default:
		// No help topic is associated with this item. 
		strRes.LoadString(IDS_STRING441);
		// return baseCKPAdvectionPg2::OnHelpInfo(pHelpInfo);
	}
	myPopup.pszText = strRes;
	return ::HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CKPTransportPg3 property page

CKPTransportPg3::CKPTransportPg3() : baseCKPTransportPg3(CKPTransportPg3::IDD)
{
	m_bcFirst = BC_FLUX;
	m_bcLast  = BC_FLUX;

	m_fdFD    = FD_FORWARD;

	//{{AFX_DATA_INIT(CKPTransportPg3)
	m_bCorrectDisp = FALSE;
	m_dDiffCoef = 0.3e-9;
	m_dTDC = 0.3e-9;
// COMMENT: {11/19/2009 4:29:14 PM}	m_bUseThermal = FALSE;
	m_bUseThermal = TRUE;
	//}}AFX_DATA_INIT
	m_dTRF = 2.0;

	m_bSynchDiffCoefs = true;
	m_bSynching       = false;
}

CKPTransportPg3::~CKPTransportPg3()
{
}

void CKPTransportPg3::DoDataExchange(CDataExchange* pDX)
{
	baseCKPTransportPg3::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKPTransportPg3)
	DDX_Control(pDX, IDC_CBO_FLOW_DIR, m_cboFD);
	DDX_Control(pDX, IDC_CBO_LAST_CELL, m_cboBCLast);
	DDX_Control(pDX, IDC_CBO_FIRST_CELL, m_cboBCFirst);
	DDX_Control(pDX, IDC_DESCRIPTION, m_eInputDesc);
	DDX_Check(pDX, IDC_CB_CORR_DISP, m_bCorrectDisp);
	DDX_Text(pDX, IDC_E_DIFF_COEF, m_dDiffCoef);
	DDX_Text(pDX, IDC_E_TDC, m_dTDC);
	DDX_Check(pDX, IDC_CB_USE_THERM, m_bUseThermal);
	//}}AFX_DATA_MAP

	DDX_Text(pDX, IDC_E_TRF, m_dTRF);
	if (pDX->m_bSaveAndValidate)
	{
		// validate m_dThermRetardFactor
		if (m_dTRF < 1.0)
		{
			// Temperature retardation factor < 1 is not possible.
			::AfxMessageBox(IDS_STRING438, MB_ICONEXCLAMATION);
			pDX->Fail();
		}
	}
	else
	{
	}

	if (pDX->m_bSaveAndValidate)
	{
		int n = m_cboFD.GetCurSel();
		m_fdFD = static_cast<FD>(n);

		DDX_CBIndex(pDX, IDC_CBO_FIRST_CELL, n);
		m_bcFirst = static_cast<BC>(n);
		if (m_bcFirst == BC_CLOSED && m_fdFD != FD_DIFF_ONLY)
		{
			// Boundary condition = 'closed' is only possible when Flow direction = 'diffusion_only'.
			::AfxMessageBox(IDS_STRING437, MB_ICONEXCLAMATION);
			pDX->Fail();
		}

		DDX_CBIndex(pDX, IDC_CBO_LAST_CELL, n);
		m_bcLast = static_cast<BC>(n);
		if (m_bcLast == BC_CLOSED && m_fdFD != FD_DIFF_ONLY)
		{
			// Boundary condition = 'closed' is only possible when Flow direction = 'diffusion_only'.
			::AfxMessageBox(IDS_STRING437, MB_ICONEXCLAMATION);
			pDX->Fail();
		}

	}
	else
	{
		m_cboFD.SetCurSel(static_cast<int>(m_fdFD));
		m_cboBCFirst.SetCurSel(static_cast<int>(m_bcFirst));
		m_cboBCLast.SetCurSel(static_cast<int>(m_bcLast));

		// enable/disable thermal
		OnCbUseThermal();
	}
}


BEGIN_MESSAGE_MAP(CKPTransportPg3, baseCKPTransportPg3)
	//{{AFX_MSG_MAP(CKPTransportPg3)
	ON_CBN_SETFOCUS(IDC_CBO_FIRST_CELL, OnSetfocusCboFirstCell)
	ON_CBN_SETFOCUS(IDC_CBO_LAST_CELL, OnSetfocusCboLastCell)
	ON_CBN_SETFOCUS(IDC_CBO_FLOW_DIR, OnSetfocusCboFlowDir)
	ON_EN_SETFOCUS(IDC_E_DIFF_COEF, OnSetfocusEDiffCoef)
	ON_EN_SETFOCUS(IDC_E_TRF, OnSetfocusETrf)
	ON_EN_SETFOCUS(IDC_E_TDC, OnSetfocusETdc)
	ON_WM_HELPINFO()
	ON_EN_CHANGE(IDC_E_DIFF_COEF, OnChangeEDiffCoef)
	ON_EN_CHANGE(IDC_E_TDC, OnChangeETdc)
	ON_BN_CLICKED(IDC_CB_USE_THERM, OnCbUseThermal)
	ON_CBN_SELCHANGE(IDC_CBO_FIRST_CELL, OnSelchangeCboFirstCell)
	ON_CBN_SELCHANGE(IDC_CBO_LAST_CELL, OnSelchangeCboLastCell)
	//}}AFX_MSG_MAP
	ON_BN_SETFOCUS(IDC_CB_CORR_DISP, OnSetfocusCBCorrDisp)
	ON_BN_SETFOCUS(IDC_CB_USE_THERM, OnSetfocusCBUseTherm)
END_MESSAGE_MAP()

BOOL CKPTransportPg3::OnInitDialog() 
{
	baseCKPTransportPg3::OnInitDialog();
	
	// set layout
	CreateRoot(VERTICAL, 5, 6) 
			<< (paneCtrl(IDC_GB_BOUNDARY, HORIZONTAL, ABSOLUTE_VERT, 0, 18, 8, 86)
				<< (pane(VERTICAL, ABSOLUTE_VERT, nDefaultBorder, 0, 0)
					<< item(IDC_S_FIRST_CELL, ABSOLUTE_VERT)
					<< (pane(HORIZONTAL, ABSOLUTE_VERT, 0, 0, 0)
						<< item(IDC_CBO_FIRST_CELL, NORESIZE|ALIGN_VCENTER)
						<< itemFixed(HORIZONTAL, 0)
						)
					)
				<< (pane(VERTICAL, ABSOLUTE_VERT, nDefaultBorder, 0, 0)
					<< item(IDC_S_LAST_CELL, ABSOLUTE_VERT)
					<< (pane(HORIZONTAL, ABSOLUTE_VERT, 0, 0, 0)
						<< item(IDC_CBO_LAST_CELL, NORESIZE|ALIGN_VCENTER)
						<< itemFixed(HORIZONTAL, 0)
						)
					)
				<< (pane(VERTICAL, ABSOLUTE_VERT, nDefaultBorder, 0, 0)
					<< itemFixed(VERTICAL, 17)
					<< (pane(HORIZONTAL, ABSOLUTE_VERT, 0, 0, 0)
						<< item(IDC_CB_CORR_DISP, ABSOLUTE_VERT|ALIGN_BOTTOM)
						<< itemFixed(HORIZONTAL, 0)
						)
					)
				)
			<< (paneCtrl(IDC_GB_FC, HORIZONTAL, ABSOLUTE_VERT, 0, 18, 8, 86)
				<< (pane(VERTICAL, ABSOLUTE_VERT, nDefaultBorder, 0, 0)
					<< item(IDC_S_FLOW_DIR, ABSOLUTE_VERT)
					<< (pane(HORIZONTAL, ABSOLUTE_VERT, 0, 0, 0)
						<< item(IDC_CBO_FLOW_DIR, NORESIZE|ALIGN_VCENTER)
						<< itemFixed(HORIZONTAL, 0)
						)
					)
				<< itemFixed(HORIZONTAL, 28)
				<< (pane(VERTICAL, ABSOLUTE_VERT, nDefaultBorder, 0, 0)
					<< item(IDC_S_DIFF_COEF, ABSOLUTE_VERT)
					<< (pane(HORIZONTAL, ABSOLUTE_VERT, 0, 0, 0)
						<< item(IDC_E_DIFF_COEF, ABSOLUTE_VERT|ALIGN_VCENTER)
						<< itemFixed(HORIZONTAL, 4)
						<< item(IDC_S_DIFF_COEF_UNITS, NORESIZE|ALIGN_VCENTER)
						)
					)
				<< itemFixed(HORIZONTAL, 28)
				)
			// {{
			<< (paneCtrl(IDC_GB_THERM_DIFF, VERTICAL, GREEDY/*ABSOLUTE_VERT*/, 0, 18, 8, 86)
// COMMENT: {5/3/2001 3:12:48 PM}				<< itemFixed(VERTICAL, 4)
// COMMENT: {11/19/2009 4:32:23 PM}				<< (pane(HORIZONTAL, GREEDY, 0, 0, 0)
// COMMENT: {11/19/2009 4:32:23 PM}// COMMENT: {5/3/2001 3:14:06 PM}					<< itemFixed(HORIZONTAL, 11)
// COMMENT: {11/19/2009 4:32:23 PM}					<< item(IDC_CB_USE_THERM, ABSOLUTE_VERT|ALIGN_VCENTER)
// COMMENT: {11/19/2009 4:32:23 PM}					)
// COMMENT: {11/19/2009 4:32:23 PM}				<< itemFixed(VERTICAL, 8)
				<< (pane(HORIZONTAL, GREEDY, 0, 0, 0)
					<< itemFixed(HORIZONTAL, 11)
					<< (pane(VERTICAL, ABSOLUTE_VERT, nDefaultBorder, 0, 0)
						<< item(IDC_S_TRF, ABSOLUTE_VERT)
						<< (pane(HORIZONTAL, ABSOLUTE_VERT, 0, 0, 0)
							<< item(IDC_E_TRF, ABSOLUTE_VERT|ALIGN_VCENTER)
							<< itemFixed(HORIZONTAL, 40)
							)
						)
					<< itemFixed(HORIZONTAL, 28)
					<< (pane(VERTICAL, ABSOLUTE_VERT, nDefaultBorder, 0, 0)
						<< item(IDC_S_TDC, ABSOLUTE_VERT)
						<< (pane(HORIZONTAL, ABSOLUTE_VERT, 0, 0, 0)
							<< item(IDC_E_TDC, ABSOLUTE_VERT|ALIGN_VCENTER)
							<< itemFixed(HORIZONTAL, 4)
							<< item(IDC_S_DIFF_COEF_UNITS2, NORESIZE|ALIGN_VCENTER)
							<< itemFixed(HORIZONTAL, 5)
							)
						)
					<< itemFixed(HORIZONTAL, 28)
					)
				)
			//}}
			//{{ 
			/*
			<< (paneCtrl(IDC_GB_THERM_DIFF, HORIZONTAL, ABSOLUTE_VERT, 0, 18, 8, 86)
				<< (pane(VERTICAL, ABSOLUTE_VERT, nDefaultBorder, 0, 0)
					<< item(IDC_S_TRF, ABSOLUTE_VERT)
					<< (pane(HORIZONTAL, ABSOLUTE_VERT, 0, 0, 0)
						<< item(IDC_E_TRF, ABSOLUTE_VERT|ALIGN_VCENTER)
						<< itemFixed(HORIZONTAL, 40)
						)
					)
				<< itemFixed(HORIZONTAL, 28)
				<< (pane(VERTICAL, ABSOLUTE_VERT, nDefaultBorder, 0, 0)
					<< item(IDC_S_TDC, ABSOLUTE_VERT)
					<< (pane(HORIZONTAL, ABSOLUTE_VERT, 0, 0, 0)
						<< item(IDC_E_TDC, ABSOLUTE_VERT|ALIGN_VCENTER)
						<< itemFixed(HORIZONTAL, 4)
						<< item(IDC_S_DIFF_COEF_UNITS2, NORESIZE|ALIGN_VCENTER)
						<< itemFixed(HORIZONTAL, 5)
						)
					)
				<< itemFixed(HORIZONTAL, 28)
				)
			*/
			//}}
			<< (paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, GREEDY, nDefaultBorder, 10, 10, 30)
				<< item(IDC_DESCRIPTION, GREEDY)
				)
	;
	UpdateLayout();	

	m_bSynchDiffCoefs = m_dDiffCoef == m_dTDC;

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKPTransportPg3::OnChangeETdc() 
{
	if (!m_bSynching)
		m_bSynchDiffCoefs = false;
}

void CKPTransportPg3::OnChangeEDiffCoef() 
{
	if (m_bSynchDiffCoefs)
	{
		CString strDC;
		GetDlgItemText(IDC_E_DIFF_COEF, strDC);
		m_bSynching = true;
		SetDlgItemText(IDC_E_TDC, strDC);
		m_bSynching = false;
	}
}

void CKPTransportPg3::OnSelchangeCboFirstCell() 
{
	// MAYBE: enable/disable correct_disp	
}

void CKPTransportPg3::OnSelchangeCboLastCell() 
{
	// MAYBE: enable/disable correct_disp	
}

void CKPTransportPg3::OnSetfocusCboFirstCell() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING423);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPTransportPg3::OnSetfocusCboLastCell() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING424);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPTransportPg3::OnSetfocusCboFlowDir() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING425);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPTransportPg3::OnSetfocusEDiffCoef() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING426);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPTransportPg3::OnSetfocusETrf() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING427);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPTransportPg3::OnSetfocusETdc() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING428);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPTransportPg3::OnSetfocusCBCorrDisp() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING429);
	m_eInputDesc.SetWindowText(strRes);
}

void CKPTransportPg3::OnSetfocusCBUseTherm() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING450);

	CString strAdd;
	strAdd.LoadString(IDS_STRING451);
	strRes += strAdd;
	m_eInputDesc.SetWindowText(strRes);
}

void CKPTransportPg3::OnCbUseThermal() 
{
	// Add your control notification handler code here
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CB_USE_THERM);
	if (pBtn)
	{
		m_bUseThermal = (pBtn->GetCheck() == BST_CHECKED);
		CWnd* pWnd;
		if ((pWnd = GetDlgItem(IDC_S_TRF)) != NULL)
		{
			pWnd->EnableWindow(m_bUseThermal);
		}
		if ((pWnd = GetDlgItem(IDC_E_TRF)) != NULL)
		{
			pWnd->EnableWindow(m_bUseThermal);
		}
		if ((pWnd = GetDlgItem(IDC_S_TDC)) != NULL)
		{
			pWnd->EnableWindow(m_bUseThermal);
		}
		if ((pWnd = GetDlgItem(IDC_E_TDC)) != NULL)
		{
			pWnd->EnableWindow(m_bUseThermal);
		}
		if ((pWnd = GetDlgItem(IDC_S_DIFF_COEF_UNITS2)) != NULL)
		{
			pWnd->EnableWindow(m_bUseThermal);
		}
	}
	
}


BOOL CKPTransportPg3::OnHelpInfo(HELPINFO* pHelpInfo) 
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
	case IDC_CBO_FIRST_CELL: case IDC_S_FIRST_CELL:
		strRes.LoadString(IDS_STRING423);
		break;
	case IDC_CBO_LAST_CELL: case IDC_S_LAST_CELL:
		strRes.LoadString(IDS_STRING424);
		break;
	case IDC_CB_CORR_DISP:
		strRes.LoadString(IDS_STRING429);
		break;
	case IDC_CBO_FLOW_DIR: case IDC_S_FLOW_DIR:
		strRes.LoadString(IDS_STRING425);
		break;
	case IDC_E_DIFF_COEF: case IDC_S_DIFF_COEF: case IDC_S_DIFF_COEF_UNITS:
		strRes.LoadString(IDS_STRING426);
		break;
	case IDC_E_TRF: case IDC_S_TRF:
		strRes.LoadString(IDS_STRING427);
		break;
	case IDC_E_TDC: case IDC_S_TDC: case IDC_S_DIFF_COEF_UNITS2:
		strRes.LoadString(IDS_STRING428);
		break;
	case IDC_CB_USE_THERM:
		{
			strRes.LoadString(IDS_STRING450);
			CString strAdd;
			strAdd.LoadString(IDS_STRING451);
			strRes += strAdd;
		}
		break;
	default:
		// No help topic is associated with this item. 
		strRes.LoadString(IDS_STRING441);
		// return baseCKPTransportPg3::OnHelpInfo(pHelpInfo);
	}
	myPopup.pszText = strRes;
	return ::HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}


/////////////////////////////////////////////////////////////////////////////
// CKPTransportPg4 property page

CKPTransportPg4::CKPTransportPg4() : baseCKPTransportPg4(CKPTransportPg4::IDD)
{
	//{{AFX_DATA_INIT(CKPTransportPg4)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CKPTransportPg4::~CKPTransportPg4()
{
}

void CKPTransportPg4::DoDataExchange(CDataExchange* pDX)
{
	CWaitCursor wait;

	baseCKPTransportPg4::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKPTransportPg4)
	DDX_Control(pDX, IDC_DESCRIPTION, m_eInputDesc);
	DDX_Control(pDX, IDC_MSHFG_LEN, m_egLengths);
	DDX_Control(pDX, IDC_MSHFG_DISP, m_egDisps);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate)
	{
		//
		// Lengths
		//
		std::list<CRepeat> lrLengths;

		CString strValue;
		// DDX_GridText(pDX, IDC_MSHFG_LEN, 1, 1, strValue);
		strValue = m_egLengths.GetTextMatrix(1, 1);

		double dLen;
		if (strValue.IsEmpty())
		{
			dLen = 1.0; // default
		}
		else
		{
			DDX_GridText(pDX, IDC_MSHFG_LEN, 1, 1, dLen);
			if (dLen <= 0.0)
			{
				// Expected positive length.
				DDX_GridFail(pDX, IDS_STRING442);
			}
		}

		CRepeat rLength(dLen);
		for (long nRow = 2; nRow < m_egLengths.GetRows(); ++nRow)
		{
			// DDX_GridText(pDX, IDC_MSHFG_LEN, nRow, 1, strValue); this is SLOW!
			strValue = m_egLengths.GetTextMatrix(nRow, 1);
			if (strValue.IsEmpty() || strValue == rLength.GetStrValue())
			{
				rLength.Increment();
				continue;
			}

			DDX_GridText(pDX, IDC_MSHFG_LEN, nRow, 1, dLen);

			if (dLen <= 0.0)
			{
				// Expected positive length.
				DDX_GridFail(pDX, IDS_STRING442);
			}

			if (dLen == rLength.GetDValue())
			{
				rLength.Increment();
			}
			else
			{
				lrLengths.push_back(rLength);
				rLength = CRepeat(dLen);
			}
		}
		// add last length
		if (!(rLength.GetDValue() == 1.0 && rLength.GetCount() == ((COCKSTransport*)GetParent())->m_Page1.m_nCells))
		{
			lrLengths.push_back(rLength);
		}
		
		// if here all ok
		m_lrLengths = lrLengths;

		//
		// Disps
		//
		std::list<CRepeat> lrDisps;

		strValue = m_egDisps.GetTextMatrix(1, 1);

		LPCTSTR lpszErrDisp = _T("Expected positive dispersivity.");

		double dDisp;
		if (strValue.IsEmpty())
		{
			dDisp = 0.0; // default
		}
		else
		{
			DDX_GridText(pDX, IDC_MSHFG_DISP, 1, 1, dDisp);
			if (dDisp < 0.0)
			{
				DDX_GridFail(pDX, lpszErrDisp);
			}
		}

		CRepeat rDisp(dDisp);
		for (int nRow = 2; nRow < m_egDisps.GetRows(); ++nRow)
		{
			// DDX_GridText(pDX, IDC_MSHFG_LEN, nRow, 1, strValue); this is SLOW!
			strValue = m_egDisps.GetTextMatrix(nRow, 1);
			if (strValue.IsEmpty() || strValue == rDisp.GetStrValue())
			{
				rDisp.Increment();
				continue;
			}

			DDX_GridText(pDX, IDC_MSHFG_DISP, nRow, 1, dDisp);

			if (dDisp < 0.0)
			{
				DDX_GridFail(pDX, lpszErrDisp);
			}

			if (dDisp == rDisp.GetDValue())
			{
				rDisp.Increment();
			}
			else
			{
				lrDisps.push_back(rDisp);
				rDisp = CRepeat(dDisp);
			}
		}
		// add last length
		if (!(rDisp.GetDValue() == 0.0 && rDisp.GetCount() == ((COCKSTransport*)GetParent())->m_Page1.m_nCells))
		{
			lrDisps.push_back(rDisp);
		}
		
		// if here all ok
		m_lrDisps = lrDisps;
	}
	else
	{
		UINT nCells = ((COCKSTransport*)GetParent())->m_Page1.m_nCells;

		// m_egLengths.SetRows(nCells + 1);
		SetCells(nCells);

		if (m_bFirstSetActive)
		{
			m_egLengths.SetTextMatrix(0, 1, _T("Length(m)"));
			m_egDisps.SetTextMatrix(0, 1, _T("Dispersivity(m)"));
		}

		InitEditGrid(m_egLengths);
		InitEditGrid(m_egDisps);

		//
		// lengths
		//
		std::list<CRepeat>::const_iterator clrIterLength = m_lrLengths.begin();
		int nRow = 1;
		for (; clrIterLength != m_lrLengths.end() && nRow < m_egLengths.GetRows(); ++clrIterLength)
		{
			for (size_t count = 0; count < clrIterLength->GetCount(); ++count)
			{
				m_egLengths.SetTextMatrix(nRow++, 1,  clrIterLength->GetStrValue());
				if (nRow == m_egLengths.GetRows()) break;
			}
		}
		// fill remaining cells
		CString strFormat;
		if (m_lrLengths.empty())
		{
			// default value
			strFormat.Format(_T("%.*g"), DBL_DIG, 1.0);
		}
		else
		{
			// last value
			strFormat = m_lrLengths.back().GetStrValue();
		}
		for (; nRow < m_egLengths.GetRows(); ++nRow)
		{
			m_egLengths.SetTextMatrix(nRow, 1, strFormat);			
		}

		//
		// disps
		//
		clrIterLength = m_lrDisps.begin();
		nRow = 1;
		for (; clrIterLength != m_lrDisps.end() && nRow < m_egDisps.GetRows(); ++clrIterLength)
		{
			for (size_t count = 0; count < clrIterLength->GetCount(); ++count)
			{
				m_egDisps.SetTextMatrix(nRow++, 1,  clrIterLength->GetStrValue());
				if (nRow == m_egDisps.GetRows()) break;
			}
		}
		// fill remaining cells
		if (m_lrDisps.empty())
		{
			// default value
			strFormat.Format(_T("%.*g"), DBL_DIG, 0.0);
		}
		else
		{
			// last value
			strFormat = m_lrDisps.back().GetStrValue();
		}
		for (; nRow < m_egDisps.GetRows(); ++nRow)
		{
			m_egDisps.SetTextMatrix(nRow, 1, strFormat);			
		}
	}
}

void CKPTransportPg4::InitEditGrid(CEditGrid& editGrid)
{
	// add col and row headings
	editGrid.AddRowHeaders();
	editGrid.SetTextMatrix(0, 0, _T("Cell"));

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
	editGrid.SetRow(0), editGrid.SetCol(0);
	editGrid.SetCellFontBold(TRUE);

	editGrid.SetRow(0), editGrid.SetCol(1);
	editGrid.SetCellFontBold(TRUE);

	// set initial cell
	editGrid.SetRow(1), editGrid.SetCol(1);
}

BEGIN_MESSAGE_MAP(CKPTransportPg4, baseCKPTransportPg4)
	//{{AFX_MSG_MAP(CKPTransportPg4)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
	// custom edit grid notifications
	// ON_MESSAGE(EGN_BEGINCELLEDIT, OnBeginCellEdit)
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
	ON_MESSAGE(EGN_SETFOCUS, OnSetfocusGrid)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CKPTransportPg4, baseCKPTransportPg4)
    //{{AFX_EVENTSINK_MAP(CKPTransportPg4)
	ON_EVENT(CKPTransportPg4, IDC_MSHFG_LEN, 71 /* EnterCell */, OnEnterCellMshfgLen, VTS_NONE)
	ON_EVENT(CKPTransportPg4, IDC_MSHFG_LEN, -602 /* KeyDown */, OnKeyDownMshfgLen, VTS_PI2 VTS_I2)
	ON_EVENT(CKPTransportPg4, IDC_MSHFG_DISP, 71 /* EnterCell */, OnEnterCellMshfgDisp, VTS_NONE)
	ON_EVENT(CKPTransportPg4, IDC_MSHFG_DISP, -602 /* KeyDown */, OnKeyDownMshfgDisp, VTS_PI2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BOOL CKPTransportPg4::OnInitDialog() 
{
	baseCKPTransportPg4::OnInitDialog();
	
	// set layout
	CreateRoot(VERTICAL, 5, 6) 
			<< (paneCtrl(IDC_S_CELL_PROPS, HORIZONTAL, GREEDY, nDefaultBorder, 10, 10, 30)
				<< (pane(VERTICAL, GREEDY, 0, 0, 0)
					<< item(IDC_S_LEN, ABSOLUTE_VERT)
					<< item(IDC_MSHFG_LEN, GREEDY)
					)
				<< itemFixed(HORIZONTAL, 50)
				<< (pane(VERTICAL, GREEDY, 0, 0, 0)
					<< item(IDC_S_DISP, ABSOLUTE_VERT)
					<< item(IDC_MSHFG_DISP, GREEDY)
					)
				)
			<< (paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, ABSOLUTE_VERT, nDefaultBorder, 10, 10, 90)
				<< item(IDC_DESCRIPTION, GREEDY)
				)
	;
	UpdateLayout();	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKPTransportPg4::OnEnterCellMshfgLen() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING430);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPTransportPg4::OnKeyDownMshfgLen(short FAR* KeyCode, short Shift) 
{
	UNUSED(Shift);

	switch (*KeyCode)
	{
	case VK_DELETE:
		{
			CWaitCursor wait;	// display hourglass cursor
			m_egLengths.SetRedraw(FALSE);
			// just fill with empty strings
			m_egLengths.SetFillStyle(flexFillRepeat);
			m_egLengths.SetText(_T(""));
			m_egLengths.SetFillStyle(flexFillSingle);
			m_egLengths.SetRedraw(TRUE);
		}
		break;
	}
}

void CKPTransportPg4::OnEnterCellMshfgDisp() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING431);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPTransportPg4::OnKeyDownMshfgDisp(short FAR* KeyCode, short Shift) 
{
	UNUSED(Shift);

	switch (*KeyCode)
	{
	case VK_DELETE:
		{
			CWaitCursor wait;	// display hourglass cursor
			m_egDisps.SetRedraw(FALSE);
			// just fill with empty strings
			m_egDisps.SetFillStyle(flexFillRepeat);
			m_egDisps.SetText(_T(""));
			m_egDisps.SetFillStyle(flexFillSingle);
			m_egDisps.SetRedraw(TRUE);
		}
		break;
	}
}

LRESULT CKPTransportPg4::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
{
	UINT nID = wParam;
	UNUSED_ALWAYS(lParam);

	switch (nID)
	{
	case IDC_MSHFG_LEN :
		OnEnterCellMshfgLen();
		break;
	case IDC_MSHFG_DISP :
		OnEnterCellMshfgDisp();
		break;
	default :
		ASSERT(FALSE);
		break;
	}
	return 0;
}

BOOL CKPTransportPg4::OnHelpInfo(HELPINFO* pHelpInfo) 
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
	case IDC_MSHFG_LEN: case IDC_S_LEN:
		strRes.LoadString(IDS_STRING430);
		break;
	case IDC_MSHFG_DISP: case IDC_S_DISP:
		strRes.LoadString(IDS_STRING431);
		break;
	default:
		// No help topic is associated with this item. 
		strRes.LoadString(IDS_STRING441);
		// return baseCKPTransportPg4::OnHelpInfo(pHelpInfo);
	}
	myPopup.pszText = strRes;
	return ::HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}


LRESULT CKPTransportPg4::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bMakeChange = TRUE;

	// ignore if edit is cancelled
	if ( pInfo->item.pszText == NULL ) return bMakeChange;

	switch (nID)
	{
	case IDC_MSHFG_LEN :
		bMakeChange = OnEndCellEdit(pInfo, m_egLengths);
		break;
	case IDC_MSHFG_DISP :
		bMakeChange = OnEndCellEdit(pInfo, m_egDisps);
		break;
	}
	return bMakeChange;
}

LRESULT CKPTransportPg4::OnEndCellEdit(NMEGINFO* pInfo, CEditGrid& editGrid)
{
	BOOL bMakeChange = TRUE;

	CString strNew = pInfo->item.pszText;
	if (strNew.Replace("*", " "))
	{
		int n;
		double value;
		if (sscanf(strNew, "%d%lf", &n, &value) == 2)
		{
			CString strVal = strNew.Mid(strNew.Find(" ", 0) + 1);
			int nR;
			for (nR = 0; nR < n; ++nR)
			{
				if (pInfo->item.iRow + nR > editGrid.GetRows() - 1) break;
				editGrid.SetTextMatrix(pInfo->item.iRow + nR, pInfo->item.iCol, strVal);
			}
			editGrid.SetRow(pInfo->item.iRow + nR - 1);
			editGrid.SetCol(pInfo->item.iCol);
			editGrid.SetTopRow(pInfo->item.iRow + nR - 2);
			return FALSE;
		}

	}
	return bMakeChange;
}

bool CKPTransportPg4::SetCells(size_t stCells)
{
	ASSERT(stCells > 0);

	// reset lengths
	if (m_egLengths.GetSafeHwnd()) m_egLengths.SetRows(stCells + 1);
	if (!m_lrLengths.empty())
	{
		// count cells
		size_t nCellTotal = 0;
		std::list<CRepeat>::const_iterator clrIterLength = m_lrLengths.begin();
		for (; clrIterLength != m_lrLengths.end(); ++clrIterLength)
			nCellTotal += clrIterLength->GetCount();

		if (nCellTotal < stCells)
		{
			for (; nCellTotal < stCells; ++nCellTotal)
			{
				m_lrLengths.back().Increment();
			}
		}
		else if (nCellTotal > stCells)
		{
			for (; stCells < nCellTotal; --nCellTotal)
			{
				if (m_lrLengths.empty())
				{
					// this is probably not nec.
					break;
				}
				if (m_lrLengths.back().GetCount() > 1)
				{
					m_lrLengths.back().Decrement();
				}
				else
				{
					ASSERT(m_lrLengths.back().GetCount() == 1);
					m_lrLengths.pop_back();
				}
			}
		}
	}
	// check if list is needed
	if (m_lrLengths.size() == 1)
	{
		if (m_lrLengths.back().GetCount() == stCells && m_lrLengths.back().GetDValue() == 1.0)
		{
			m_lrLengths.pop_back();
		}		
	}

	// reset disps
	if (m_egDisps.GetSafeHwnd()) m_egDisps.SetRows(stCells + 1);
	if (!m_lrDisps.empty())
	{
		// count cells
		size_t nCellTotal = 0;
		std::list<CRepeat>::const_iterator clrIterLength = m_lrDisps.begin();
		for (; clrIterLength != m_lrDisps.end(); ++clrIterLength)
			nCellTotal += clrIterLength->GetCount();

		if (nCellTotal < stCells)
		{
			for (; nCellTotal < stCells; ++nCellTotal)
			{
				m_lrDisps.back().Increment();
			}
		}
		else if (nCellTotal > stCells)
		{
			for (; stCells < nCellTotal; --nCellTotal)
			{
				if (m_lrDisps.empty())
				{
					// this is probably not nec.
					break;
				}
				if (m_lrDisps.back().GetCount() > 1)
				{
					m_lrDisps.back().Decrement();
				}
				else
				{
					ASSERT(m_lrDisps.back().GetCount() == 1);
					m_lrDisps.pop_back();
				}
			}
		}
	}
	// check if list is needed
	if (m_lrDisps.size() == 1)
	{
		if (m_lrDisps.back().GetCount() == stCells && m_lrDisps.back().GetDValue() == 0.0)
		{
			m_lrDisps.pop_back();
		}		
	}

	return true;
}


/////////////////////////////////////////////////////////////////////////////
// CKPTransportPg5 property page

CKPTransportPg5::CKPTransportPg5() : baseCKPTransportPg5(CKPTransportPg5::IDD)
{
	//{{AFX_DATA_INIT(CKPTransportPg5)
	m_nStagCells = 0;
	m_dExchFactor = 0.0;
	m_dThetaM = 0.0;
	m_dThetaIM = 0.0;
	m_strDumpFileName = _T("phreeqc.dmp");
	m_bDumpToFile = FALSE;
	m_bUseStagnant = FALSE;
	//}}AFX_DATA_INIT
	m_nDumpModulus = 0;
	m_nDumpRestart = 1;
}

CKPTransportPg5::~CKPTransportPg5()
{
}

void CKPTransportPg5::DoDataExchange(CDataExchange* pDX)
{

	baseCKPTransportPg5::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKPTransportPg5)
	DDX_Control(pDX, IDC_SPIN_DUMP_RESTART, m_spinDumpRestart);
	DDX_Control(pDX, IDC_SPIN_DUMP_MOD, m_spinDumpModulus);
	DDX_Control(pDX, IDC_SPIN_STAG_CELLS, m_spinStagCells);
	DDX_Control(pDX, IDC_DESCRIPTION, m_eInputDesc);
	DDX_Text(pDX, IDC_E_STAG_CELLS, m_nStagCells);
	DDX_Check(pDX, IDC_CB_DUMP_TO_FILE, m_bDumpToFile);
	DDX_Check(pDX, IDC_CB_USE_STAG, m_bUseStagnant);
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
		m_spinStagCells.SetRange32(0, INT_MAX - 1);
		m_spinStagCells.SetAccel(7, uda);

		m_spinDumpModulus.SetRange32(1, INT_MAX - 1);
		m_spinDumpModulus.SetAccel(7, uda);

		m_spinDumpRestart.SetRange32(1, ((COCKSTransport*)GetParent())->m_Page1.m_nShifts);
		m_spinDumpRestart.SetAccel(7, uda);
	}

	if (pDX->m_bSaveAndValidate)
	{
		if (m_bUseStagnant)
		{
			if (m_nStagCells == 1)
			{
				DDX_Text(pDX, IDC_E_EXCH_FACT, m_dExchFactor);
				DDX_Text(pDX, IDC_E_MOB_POR, m_dThetaM);
				DDX_Text(pDX, IDC_E_IMMOB_POR, m_dThetaIM);
			}
		}
		else
		{
			m_nStagCells = 0;
		}
		if (m_bDumpToFile)
		{
			DDX_Text(pDX, IDC_E_DUMP_FILE, m_strDumpFileName);
			DDX_Text(pDX, IDC_E_DUMP_MOD, m_nDumpModulus);
			DDX_Text(pDX, IDC_E_DUMP_RESTART, m_nDumpRestart);
			if (m_nDumpRestart > ((COCKSTransport*)GetParent())->m_Page1.m_nShifts)
			{
				// "Dump restart" must be less than or equal to "(General Tab) Number of shifts"
				::AfxMessageBox(IDS_STRING436, MB_ICONEXCLAMATION);
				pDX->Fail();
			}
		}
		if (m_nStagCells == 0)
		{
			ASSERT(m_bUseStagnant == FALSE);
			m_bUseStagnant = FALSE;
		}
	}
	else
	{
		DDX_Text(pDX, IDC_E_EXCH_FACT, m_dExchFactor);
		DDX_Text(pDX, IDC_E_MOB_POR, m_dThetaM);
		DDX_Text(pDX, IDC_E_IMMOB_POR, m_dThetaIM);

		if (m_nDumpModulus == 0)
		{
			m_nDumpModulus = __max(1, ((COCKSTransport*)GetParent())->m_Page1.m_nShifts / 2);
		}
		DDX_Text(pDX, IDC_E_DUMP_FILE, m_strDumpFileName);
		DDX_Text(pDX, IDC_E_DUMP_MOD, m_nDumpModulus);
		DDX_Text(pDX, IDC_E_DUMP_RESTART, m_nDumpRestart);

		// enable/disable stagnant
		OnCbUseStagnant();
		//OnChangeEStagCells();

		// enable/disable dump
		OnCbDumpToFile();
	}
}


BEGIN_MESSAGE_MAP(CKPTransportPg5, baseCKPTransportPg5)
	//{{AFX_MSG_MAP(CKPTransportPg5)
	ON_EN_SETFOCUS(IDC_E_STAG_CELLS, OnSetfocusEStagCells)
	ON_WM_HELPINFO()
	ON_EN_CHANGE(IDC_E_STAG_CELLS, OnChangeEStagCells)
	ON_EN_SETFOCUS(IDC_E_EXCH_FACT, OnSetfocusEExchFact)
	ON_EN_SETFOCUS(IDC_E_MOB_POR, OnSetfocusEMobPor)
	ON_EN_SETFOCUS(IDC_E_IMMOB_POR, OnSetfocusEImmobPor)
	ON_BN_CLICKED(IDC_B_DUMP_FILE, OnBDumpFile)
	ON_BN_CLICKED(IDC_CB_DUMP_TO_FILE, OnCbDumpToFile)
	ON_EN_SETFOCUS(IDC_E_DUMP_FILE, OnSetfocusEDumpFile)
	ON_EN_SETFOCUS(IDC_E_DUMP_MOD, OnSetfocusEDumpMod)
	ON_EN_SETFOCUS(IDC_E_DUMP_RESTART, OnSetfocusEDumpRestart)
	ON_BN_CLICKED(IDC_CB_USE_STAG, OnCbUseStagnant)
	//}}AFX_MSG_MAP
	ON_BN_SETFOCUS(IDC_CB_DUMP_TO_FILE, OnSetfocusCBDumpToFile)
	ON_BN_SETFOCUS(IDC_CB_USE_STAG, OnSetfocusCBUseStagnant)
END_MESSAGE_MAP()

BOOL CKPTransportPg5::OnInitDialog() 
{
	baseCKPTransportPg5::OnInitDialog();
	
	// set layout
	CreateRoot(VERTICAL, 5, 6) 
			<< (paneCtrl(IDC_GB_STAG_PROP, VERTICAL, GREEDY, nDefaultBorder, 10, 10, 30)
				//{{
				<< itemFixed(VERTICAL, 4)
				<< (pane(HORIZONTAL, GREEDY, 0, 0, 0)
					<< itemFixed(HORIZONTAL, 11)
					<< item(IDC_CB_USE_STAG, ABSOLUTE_VERT|ALIGN_VCENTER)
					)
				//}}
				<< itemFixed(VERTICAL, 11)
				<< (pane(HORIZONTAL, GREEDY, 0, 0, 0)
					<< itemFixed(HORIZONTAL, 11)
					<< item(IDC_S_STAG_CELLS, NORESIZE|ALIGN_VCENTER)
					<< itemFixed(HORIZONTAL, 4)
					<< item(IDC_E_STAG_CELLS, ABSOLUTE_VERT|ALIGN_VCENTER)
					<< item(IDC_SPIN_STAG_CELLS, NORESIZE|ALIGN_VCENTER)
					<< itemFixed(HORIZONTAL, 25)
					<< item(IDC_S_EXCH_FACT, NORESIZE|ALIGN_VCENTER)
					<< itemFixed(HORIZONTAL, 4)
					<< item(IDC_E_EXCH_FACT, ABSOLUTE_VERT|ALIGN_VCENTER)
					<< itemFixed(HORIZONTAL, 2)
					<< item(IDC_S_EXCH_FACT_UNITS, NORESIZE|ALIGN_VCENTER)
					)
				<< itemFixed(VERTICAL, 11)
				<< (pane(HORIZONTAL, GREEDY, 0, 0, 0)
					<< itemFixed(HORIZONTAL, 11)
					<< item(IDC_S_MOB_POR, NORESIZE|ALIGN_VCENTER)
					<< itemFixed(HORIZONTAL, 4)
					<< item(IDC_E_MOB_POR, ABSOLUTE_VERT|ALIGN_VCENTER)
					<< itemSpaceLike(HORIZONTAL, IDC_SPIN_STAG_CELLS)
					<< itemFixed(HORIZONTAL, 25)
					<< item(IDC_S_IMMOB_POR, NORESIZE|ALIGN_VCENTER)
					<< itemFixed(HORIZONTAL, 4)
					<< item(IDC_E_IMMOB_POR, ABSOLUTE_VERT|ALIGN_VCENTER)
					<< itemFixed(HORIZONTAL, 2)
					<< itemSpaceLike(HORIZONTAL, IDC_S_EXCH_FACT_UNITS)
					)
				)
			<< (paneCtrl(IDC_GB_DO, VERTICAL, GREEDY, nDefaultBorder, 10, 10, 30)
				<< itemFixed(VERTICAL, 4)
				<< (pane(HORIZONTAL, GREEDY, 0, 0, 0)
					<< itemFixed(HORIZONTAL, 11)
					<< item(IDC_CB_DUMP_TO_FILE, ABSOLUTE_VERT|ALIGN_VCENTER)
					)
				<< itemFixed(VERTICAL, 11)
				<< (pane(HORIZONTAL, GREEDY, 0, 0, 0)
					<< itemFixed(HORIZONTAL, 15)
					<< item(IDC_S_DUMP_FILE, NORESIZE|ALIGN_VCENTER)
					<< item(IDC_E_DUMP_FILE, ABSOLUTE_VERT|ALIGN_VCENTER)
					<< itemFixed(HORIZONTAL, 6)
					<< item(IDC_B_DUMP_FILE, NORESIZE|ALIGN_VCENTER)
					)
				<< itemFixed(VERTICAL, 11)
				<< (pane(HORIZONTAL, GREEDY, 0, 0, 0)
					<< itemFixed(HORIZONTAL, 15)
					<< item(IDC_S_DUMP_MOD, NORESIZE|ALIGN_VCENTER)
					<< itemFixed(HORIZONTAL, 4)
					<< item(IDC_E_DUMP_MOD, ABSOLUTE_VERT|ALIGN_VCENTER)
					<< item(IDC_SPIN_DUMP_MOD, NORESIZE|ALIGN_VCENTER)
					<< itemFixed(HORIZONTAL, 25)
					<< item(IDC_S_DUMP_RESTART, NORESIZE|ALIGN_VCENTER)
					<< itemFixed(HORIZONTAL, 4)
					<< item(IDC_E_DUMP_RESTART, ABSOLUTE_VERT|ALIGN_VCENTER)
					<< item(IDC_SPIN_DUMP_RESTART, NORESIZE|ALIGN_VCENTER)
					)
				<< itemFixed(VERTICAL, 4)
				)
			<< (paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, GREEDY, nDefaultBorder, 10, 10, 90)
				<< item(IDC_DESCRIPTION, GREEDY)
				)
	;
	UpdateLayout();	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CKPTransportPg5::OnHelpInfo(HELPINFO* pHelpInfo) 
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
	case IDC_E_STAG_CELLS: case IDC_S_STAG_CELLS: case IDC_SPIN_STAG_CELLS:
		strRes.LoadString(IDS_STRING432);
		break;
	case IDC_E_EXCH_FACT: case IDC_S_EXCH_FACT: case IDC_S_EXCH_FACT_UNITS:
		strRes.LoadString(IDS_STRING433);
		break;
	case IDC_E_MOB_POR: case IDC_S_MOB_POR:
		strRes.LoadString(IDS_STRING434);
		break;
	case IDC_E_IMMOB_POR: case IDC_S_IMMOB_POR:
		strRes.LoadString(IDS_STRING435);
		break;
	case IDC_CB_DUMP_TO_FILE:
		strRes.LoadString(IDS_STRING443);
		break;
	case IDC_E_DUMP_FILE: case IDC_S_DUMP_FILE:
		strRes.LoadString(IDS_STRING444);
		break;
	case IDC_E_DUMP_MOD: case IDC_S_DUMP_MOD: case IDC_SPIN_DUMP_MOD:
		strRes.LoadString(IDS_STRING445);
		break;
	case IDC_E_DUMP_RESTART: case IDC_S_DUMP_RESTART: case IDC_SPIN_DUMP_RESTART:
		strRes.LoadString(IDS_STRING446);
		break;
	case IDC_B_DUMP_FILE:
		strRes.LoadString(IDS_STRING447);
		break;
	case IDC_CB_USE_STAG:
		{
			strRes.LoadString(IDS_STRING448);
			CString strAdd;
			strAdd.LoadString(IDS_STRING449);
			strRes += strAdd;
		}
		break;
	default:
		// No help topic is associated with this item. 
		strRes.LoadString(IDS_STRING441);
		// return baseCKPTransportPg5::OnHelpInfo(pHelpInfo);
	}
	myPopup.pszText = strRes;
	return ::HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}

void CKPTransportPg5::OnChangeEStagCells() 
{
	CString strStagCells;
	if (GetDlgItemText(IDC_E_STAG_CELLS, strStagCells) != 0)
	{
		strStagCells.TrimLeft(); strStagCells.TrimRight();

		LPCTSTR lsz = strStagCells;
		int nStagCells = _ttoi(lsz);

		BOOL bEnable = (nStagCells == 1  && m_bUseStagnant);
		CWnd* pWnd;

		pWnd = GetDlgItem(IDC_S_EXCH_FACT);
		if (pWnd) pWnd->EnableWindow(bEnable);
		pWnd = GetDlgItem(IDC_E_EXCH_FACT);
		if (pWnd) pWnd->EnableWindow(bEnable);
		pWnd = GetDlgItem(IDC_S_EXCH_FACT_UNITS);
		if (pWnd) pWnd->EnableWindow(bEnable);

		pWnd = GetDlgItem(IDC_S_MOB_POR);
		if (pWnd) pWnd->EnableWindow(bEnable);
		pWnd = GetDlgItem(IDC_E_MOB_POR);
		if (pWnd) pWnd->EnableWindow(bEnable);

		pWnd = GetDlgItem(IDC_S_IMMOB_POR);
		if (pWnd) pWnd->EnableWindow(bEnable);
		pWnd = GetDlgItem(IDC_E_IMMOB_POR);
		if (pWnd) pWnd->EnableWindow(bEnable);
	}
}

void CKPTransportPg5::OnSetfocusEStagCells() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING432);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPTransportPg5::OnSetfocusEExchFact() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING433);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPTransportPg5::OnSetfocusEMobPor() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING434);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPTransportPg5::OnSetfocusEImmobPor() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING435);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPTransportPg5::OnSetfocusCBDumpToFile() 
{
	// IDC_CB_DUMP_TO_FILE
	CString strRes;
	strRes.LoadString(IDS_STRING443);
	m_eInputDesc.SetWindowText(strRes);
}

void CKPTransportPg5::OnSetfocusCBUseStagnant() 
{
	// IDC_CB_USE_STAG
	CString strRes;
	CString strAdd;
	strRes.LoadString(IDS_STRING448);
	strAdd.LoadString(IDS_STRING449);
	strRes += strAdd;
	m_eInputDesc.SetWindowText(strRes);
}

void CKPTransportPg5::OnSetfocusEDumpFile() 
{
	// IDC_E_DUMP_FILE
	CString strRes;
	strRes.LoadString(IDS_STRING444);
	m_eInputDesc.SetWindowText(strRes);
}

void CKPTransportPg5::OnSetfocusEDumpMod() 
{
	// IDC_E_DUMP_MOD
	CString strRes;
	strRes.LoadString(IDS_STRING445);
	m_eInputDesc.SetWindowText(strRes);
}

void CKPTransportPg5::OnSetfocusEDumpRestart() 
{
	// IDC_E_DUMP_RESTART
	CString strRes;
	strRes.LoadString(IDS_STRING446);
	m_eInputDesc.SetWindowText(strRes);
}

void CKPTransportPg5::OnBDumpFile() 
{
	CSaveCurrentDirectory scd;

	// Show file Dialog box
	CFileDialog dlg(
		FALSE,					// bOpenFileDialog
		_T("dmp"),				// lpszDefExt
		m_strDumpFileName,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST,
		_T("Dump Files (*.dmp)|*.dmp|All Files (*.*)|*.*||")
		);
	dlg.m_ofn.lpstrTitle = _T("Save Dump File As");
	if (dlg.DoModal() == IDOK)
	{
		m_strDumpFileName = dlg.GetPathName();
		UpdateData(FALSE);
	}	
}

void CKPTransportPg5::OnCbDumpToFile() 
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CB_DUMP_TO_FILE);
	if (pBtn)
	{
		m_bDumpToFile = (pBtn->GetCheck() == BST_CHECKED);
		CWnd* pWnd;
		if ((pWnd = GetDlgItem(IDC_S_DUMP_FILE)) != NULL)
		{
			pWnd->EnableWindow(m_bDumpToFile);
		}
		if ((pWnd = GetDlgItem(IDC_E_DUMP_FILE)) != NULL)
		{
			pWnd->EnableWindow(m_bDumpToFile);
		}
		if ((pWnd = GetDlgItem(IDC_B_DUMP_FILE)) != NULL)
		{
			pWnd->EnableWindow(m_bDumpToFile);
		}
		if ((pWnd = GetDlgItem(IDC_S_DUMP_MOD)) != NULL)
		{
			pWnd->EnableWindow(m_bDumpToFile);
		}
		if ((pWnd = GetDlgItem(IDC_E_DUMP_MOD)) != NULL)
		{
			pWnd->EnableWindow(m_bDumpToFile);
		}
		if ((pWnd = GetDlgItem(IDC_SPIN_DUMP_MOD)) != NULL)
		{
			pWnd->EnableWindow(m_bDumpToFile);
		}
		if ((pWnd = GetDlgItem(IDC_S_DUMP_RESTART)) != NULL)
		{
			pWnd->EnableWindow(m_bDumpToFile);
		}
		if ((pWnd = GetDlgItem(IDC_E_DUMP_RESTART)) != NULL)
		{
			pWnd->EnableWindow(m_bDumpToFile);
		}
		if ((pWnd = GetDlgItem(IDC_SPIN_DUMP_RESTART)) != NULL)
		{
			pWnd->EnableWindow(m_bDumpToFile);
		}
	}
}


void CKPTransportPg5::OnCbUseStagnant() 
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CB_USE_STAG);
	if (pBtn)
	{
		m_bUseStagnant = (pBtn->GetCheck() == BST_CHECKED);
		CWnd* pWnd;
		if ((pWnd = GetDlgItem(IDC_S_STAG_CELLS)) != NULL)
		{
			pWnd->EnableWindow(m_bUseStagnant);
		}
		if ((pWnd = GetDlgItem(IDC_E_STAG_CELLS)) != NULL)
		{
			pWnd->EnableWindow(m_bUseStagnant);
		}
		if ((pWnd = GetDlgItem(IDC_SPIN_STAG_CELLS)) != NULL)
		{
			pWnd->EnableWindow(m_bUseStagnant);
		}
	}
	OnChangeEStagCells();
}


