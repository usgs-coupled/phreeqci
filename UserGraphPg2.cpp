// UserGraphPg2.cpp : implementation file
//

#include "stdafx.h"
#include "phreeqci2.h"
#include "UserGraphPg2.h"

#include "DDX_DDV.h"
#include "DelayUpdate.h"
#include "BasicObj.h"

////#import "phreeqc3/src/phreeqc/ZedGraph.dll"
//using ZedGraph;
//using namespace ZedGraph;
//using namespace System::Windows::Forms;
#include "phreeqc3/src/ChartObject.h"
#include "phreeqc3/src/Utils.h"


// CUserGraphPg2 dialog

//IMPLEMENT_DYNAMIC(CUserGraphPg2, CPropertyPage)
IMPLEMENT_DYNCREATE(CUserGraphPg2, baseUserGraphPg2)

CUserGraphPg2::CUserGraphPg2() : baseUserGraphPg2(CUserGraphPg2::IDD)
, m_basicDesc(GetDatabase(), IDC_LB_FUNCS, IDC_E_EXPLAN, IDC_TREE_ARGS)
{

}

CUserGraphPg2::~CUserGraphPg2()
{
}

void CUserGraphPg2::DoDataExchange(CDataExchange* pDX)
{
	baseUserGraphPg2::DoDataExchange(pDX);

	if (m_bFirstSetActive)
	{
		//
		// Init NumDesc (USER_GRAPH doesn't support a range)
		//
		m_egNumDesc.SetColWidth(0, 0, 1100);
		m_egNumDesc.SetTextMatrix(0, 0, _T("Number"));
		m_egNumDesc.SetRowHeight(1, 0);
	}

	DDX_Control(pDX, IDC_MSHFG_BASIC, m_egBasic);
// COMMENT: {5/1/2012 6:27:49 PM}	DDX_Text(pDX, IDC_E_HEAD, m_strHead);
// COMMENT: {5/1/2012 6:27:49 PM}
// COMMENT: {5/1/2012 6:27:49 PM}	// initial solutions
// COMMENT: {5/1/2012 6:27:49 PM}	if (pDX->m_bSaveAndValidate)
// COMMENT: {5/1/2012 6:27:49 PM}	{
// COMMENT: {5/1/2012 6:27:49 PM}		if (this->IsDlgButtonChecked(IDC_CHECK_INIT_SOLNS) == BST_CHECKED)
// COMMENT: {5/1/2012 6:27:49 PM}		{
// COMMENT: {5/1/2012 6:27:49 PM}			this->m_initial_solutions = true;
// COMMENT: {5/1/2012 6:27:49 PM}		}
// COMMENT: {5/1/2012 6:27:49 PM}		else
// COMMENT: {5/1/2012 6:27:49 PM}		{
// COMMENT: {5/1/2012 6:27:49 PM}			this->m_initial_solutions = false;
// COMMENT: {5/1/2012 6:27:49 PM}		}
// COMMENT: {5/1/2012 6:27:49 PM}	}
// COMMENT: {5/1/2012 6:27:49 PM}	else
// COMMENT: {5/1/2012 6:27:49 PM}	{
// COMMENT: {5/1/2012 6:27:49 PM}		if (this->m_initial_solutions)
// COMMENT: {5/1/2012 6:27:49 PM}		{
// COMMENT: {5/1/2012 6:27:49 PM}			this->CheckDlgButton(IDC_CHECK_INIT_SOLNS, BST_CHECKED);
// COMMENT: {5/1/2012 6:27:49 PM}		}
// COMMENT: {5/1/2012 6:27:49 PM}		else
// COMMENT: {5/1/2012 6:27:49 PM}		{
// COMMENT: {5/1/2012 6:27:49 PM}			this->CheckDlgButton(IDC_CHECK_INIT_SOLNS, BST_UNCHECKED);
// COMMENT: {5/1/2012 6:27:49 PM}		}
// COMMENT: {5/1/2012 6:27:49 PM}	}
// COMMENT: {5/1/2012 6:27:49 PM}
// COMMENT: {5/1/2012 6:27:49 PM}	// connect simulations
// COMMENT: {5/1/2012 6:27:49 PM}	if (pDX->m_bSaveAndValidate)
// COMMENT: {5/1/2012 6:27:49 PM}	{
// COMMENT: {5/1/2012 6:27:49 PM}		if (this->IsDlgButtonChecked(IDC_CHECK_CONNECT_SIMS) == BST_CHECKED)
// COMMENT: {5/1/2012 6:27:49 PM}		{
// COMMENT: {5/1/2012 6:27:49 PM}			this->m_connect_simulations = true;
// COMMENT: {5/1/2012 6:27:49 PM}		}
// COMMENT: {5/1/2012 6:27:49 PM}		else
// COMMENT: {5/1/2012 6:27:49 PM}		{
// COMMENT: {5/1/2012 6:27:49 PM}			this->m_connect_simulations = false;
// COMMENT: {5/1/2012 6:27:49 PM}		}
// COMMENT: {5/1/2012 6:27:49 PM}	}
// COMMENT: {5/1/2012 6:27:49 PM}	else
// COMMENT: {5/1/2012 6:27:49 PM}	{
// COMMENT: {5/1/2012 6:27:49 PM}		if (this->m_connect_simulations)
// COMMENT: {5/1/2012 6:27:49 PM}		{
// COMMENT: {5/1/2012 6:27:49 PM}			this->CheckDlgButton(IDC_CHECK_CONNECT_SIMS, BST_CHECKED);
// COMMENT: {5/1/2012 6:27:49 PM}		}
// COMMENT: {5/1/2012 6:27:49 PM}		else
// COMMENT: {5/1/2012 6:27:49 PM}		{
// COMMENT: {5/1/2012 6:27:49 PM}			this->CheckDlgButton(IDC_CHECK_CONNECT_SIMS, BST_UNCHECKED);
// COMMENT: {5/1/2012 6:27:49 PM}		}
// COMMENT: {5/1/2012 6:27:49 PM}	}
// COMMENT: {5/1/2012 6:27:49 PM}
// COMMENT: {5/1/2012 6:27:49 PM}	// detach
// COMMENT: {5/1/2012 6:27:49 PM}	if (pDX->m_bSaveAndValidate)
// COMMENT: {5/1/2012 6:27:49 PM}	{
// COMMENT: {5/1/2012 6:27:49 PM}		if (this->IsDlgButtonChecked(IDC_CHECK_DETACH) == BST_CHECKED)
// COMMENT: {5/1/2012 6:27:49 PM}		{
// COMMENT: {5/1/2012 6:27:49 PM}			this->m_detach = true;
// COMMENT: {5/1/2012 6:27:49 PM}		}
// COMMENT: {5/1/2012 6:27:49 PM}		else
// COMMENT: {5/1/2012 6:27:49 PM}		{
// COMMENT: {5/1/2012 6:27:49 PM}			this->m_detach = false;
// COMMENT: {5/1/2012 6:27:49 PM}		}
// COMMENT: {5/1/2012 6:27:49 PM}	}
// COMMENT: {5/1/2012 6:27:49 PM}	else
// COMMENT: {5/1/2012 6:27:49 PM}	{
// COMMENT: {5/1/2012 6:27:49 PM}		if (this->m_detach)
// COMMENT: {5/1/2012 6:27:49 PM}		{
// COMMENT: {5/1/2012 6:27:49 PM}			this->CheckDlgButton(IDC_CHECK_DETACH, BST_CHECKED);
// COMMENT: {5/1/2012 6:27:49 PM}		}
// COMMENT: {5/1/2012 6:27:49 PM}		else
// COMMENT: {5/1/2012 6:27:49 PM}		{
// COMMENT: {5/1/2012 6:27:49 PM}			this->CheckDlgButton(IDC_CHECK_DETACH, BST_UNCHECKED);
// COMMENT: {5/1/2012 6:27:49 PM}		}
// COMMENT: {5/1/2012 6:27:49 PM}	}
// COMMENT: {5/1/2012 6:27:49 PM}
// COMMENT: {5/1/2012 6:27:49 PM}	// active
// COMMENT: {5/1/2012 6:27:49 PM}	if (pDX->m_bSaveAndValidate)
// COMMENT: {5/1/2012 6:27:49 PM}	{
// COMMENT: {5/1/2012 6:27:49 PM}		if (this->IsDlgButtonChecked(IDC_CHECK_ACTIVE) == BST_CHECKED)
// COMMENT: {5/1/2012 6:27:49 PM}		{
// COMMENT: {5/1/2012 6:27:49 PM}			this->m_active = true;
// COMMENT: {5/1/2012 6:27:49 PM}		}
// COMMENT: {5/1/2012 6:27:49 PM}		else
// COMMENT: {5/1/2012 6:27:49 PM}		{
// COMMENT: {5/1/2012 6:27:49 PM}			this->m_active = false;
// COMMENT: {5/1/2012 6:27:49 PM}		}
// COMMENT: {5/1/2012 6:27:49 PM}	}
// COMMENT: {5/1/2012 6:27:49 PM}	else
// COMMENT: {5/1/2012 6:27:49 PM}	{
// COMMENT: {5/1/2012 6:27:49 PM}		if (this->m_active)
// COMMENT: {5/1/2012 6:27:49 PM}		{
// COMMENT: {5/1/2012 6:27:49 PM}			this->CheckDlgButton(IDC_CHECK_ACTIVE, BST_CHECKED);
// COMMENT: {5/1/2012 6:27:49 PM}		}
// COMMENT: {5/1/2012 6:27:49 PM}		else
// COMMENT: {5/1/2012 6:27:49 PM}		{
// COMMENT: {5/1/2012 6:27:49 PM}			this->CheckDlgButton(IDC_CHECK_ACTIVE, BST_UNCHECKED);
// COMMENT: {5/1/2012 6:27:49 PM}		}
// COMMENT: {5/1/2012 6:27:49 PM}	}


	m_basicDesc.DoDataExchange(pDX);
	DDX_ListCommands(pDX);
}


BEGIN_MESSAGE_MAP(CUserGraphPg2, baseUserGraphPg2)
	//{{AFX_MSG_MAP(CKPUserPunchPg1)
	ON_LBN_SELCHANGE(IDC_LB_FUNCS, OnSelchangeLbFuncs)
	ON_LBN_SETFOCUS(IDC_LB_FUNCS, OnSetfocusLbFuncs)
	ON_LBN_SETFOCUS(IDC_LB_ARGS, OnSetfocusLbArgs)
	ON_EN_SETFOCUS(IDC_E_EXPLAN, OnSetfocusEExplan)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_B_RENUMBER, OnBRenumber)
	ON_EN_SETFOCUS(IDC_E_HEAD, OnSetfocusEHead)
	ON_WM_HELPINFO()
	ON_NOTIFY(NM_SETFOCUS, IDC_TREE_ARGS, OnSetfocusTreeArgs)
	//}}AFX_MSG_MAP

	// custom edit grid notifications
	ON_MESSAGE(EGN_BEGINCELLEDIT, OnBeginCellEdit)
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
	ON_MESSAGE(EGN_SETFOCUS, OnSetfocusGrid)
END_MESSAGE_MAP()

// CUserGraphPg2 message handlers

BOOL CUserGraphPg2::OnInitDialog()
{
	baseUserGraphPg2::OnInitDialog();

	CreateRoot(VERTICAL, 5, 6)
		//{{
		<< (pane(HORIZONTAL, ABSOLUTE_VERT)
			<< item(IDC_MSHFG_NUM_DESC, ABSOLUTE_VERT)
			)
		//}}
		/***
		<< item(IDC_ST_HEAD, ABSOLUTE_VERT | ALIGN_BOTTOM)
		<< item(IDC_E_HEAD, ABSOLUTE_VERT)
		//{{
		<< item(IDC_CHECK_INIT_SOLNS, ABSOLUTE_VERT | ALIGN_BOTTOM)
		<< item(IDC_CHECK_CONNECT_SIMS, ABSOLUTE_VERT | ALIGN_BOTTOM)
		<< item(IDC_CHECK_DETACH, ABSOLUTE_VERT | ALIGN_BOTTOM)
		<< item(IDC_CHECK_ACTIVE, ABSOLUTE_VERT | ALIGN_BOTTOM)
		//}}
		***/
		<< (pane(HORIZONTAL, GREEDY)
			<< item(IDC_ST_BASIC, ABSOLUTE_VERT | ALIGN_BOTTOM)
			<< item(IDC_B_RENUMBER, NORESIZE)
			)
			<< item(IDC_MSHFG_BASIC, GREEDY)
			<< (pane(HORIZONTAL, GREEDY)
				<< (pane(VERTICAL, ABSOLUTE_VERT | ALIGN_BOTTOM)
					<< item(IDC_ST_FUNCS, ABSOLUTE_VERT | ALIGN_BOTTOM)
					<< item(IDC_LB_FUNCS, ABSOLUTE_VERT | ALIGN_BOTTOM)
					)
				<< (pane(VERTICAL, ABSOLUTE_VERT | ALIGN_BOTTOM)
					<< item(IDC_ST_EXPLAN, ABSOLUTE_VERT | ALIGN_BOTTOM)
					<< item(IDC_E_EXPLAN, ABSOLUTE_VERT | ALIGN_BOTTOM)
					)
				<< (pane(VERTICAL, ABSOLUTE_VERT | ALIGN_BOTTOM)
					<< item(IDC_ST_ARGS, ABSOLUTE_VERT | ALIGN_BOTTOM)
					<< item(IDC_TREE_ARGS, ABSOLUTE_VERT | ALIGN_BOTTOM)
					)
				)
		<< (paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, GREEDY, nDefaultBorder, 10, 10)
			<< item(IDC_E_DESC_INPUT, ABSOLUTE_VERT)
			);
	UpdateLayout();

	// allow indention
	m_egBasic.SetTrimOnEntry(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUserGraphPg2::OnSelchangeLbFuncs()
{
	m_basicDesc.OnSelchangeLbFuncs();
}

void CUserGraphPg2::OnSetfocusLbFuncs()
{
	CString strRes;
	strRes.LoadString(IDS_RATE_258);
	m_eInputDesc.SetWindowText(strRes);
}

void CUserGraphPg2::OnSetfocusLbArgs()
{
	CString strRes;
	strRes.LoadString(IDS_RATE_260);
	m_eInputDesc.SetWindowText(strRes);
}

void CUserGraphPg2::OnSetfocusEExplan()
{
	CString strRes;
	strRes.LoadString(IDS_RATE_259);
	m_eInputDesc.SetWindowText(strRes);
}

void CUserGraphPg2::OnSetfocusBRenumber()
{
	CString strRes;
	strRes.LoadString(IDS_STRING512);
	m_eInputDesc.SetWindowText(strRes);
}

void CUserGraphPg2::OnSize(UINT nType, int cx, int cy)
{
	baseUserGraphPg2::OnSize(nType, cx, cy);

// COMMENT: {10/5/2000 6:19:38 PM}	CDC* pDC = GetDC();
// COMMENT: {10/5/2000 6:19:38 PM}	int nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
// COMMENT: {10/5/2000 6:19:38 PM}
// COMMENT: {10/5/2000 6:19:38 PM}	// resize the column within the grid
// COMMENT: {10/5/2000 6:19:38 PM}	if (m_egHead.GetSafeHwnd())
// COMMENT: {10/5/2000 6:19:38 PM}	{
// COMMENT: {10/5/2000 6:19:38 PM}		CRect rect;
// COMMENT: {10/5/2000 6:19:38 PM}		m_egHead.GetClientRect(&rect);
// COMMENT: {10/5/2000 6:19:38 PM}		m_egHead.SetColWidth(0, 0, MulDiv(rect.right, TWIPS_PER_INCH, nLogX));
// COMMENT: {10/5/2000 6:19:38 PM}	}

	// resize the columns within the grid
	if (m_egBasic.GetSafeHwnd())
	{
		long nCol0 = m_egBasic.GetColWidth(0, 0);

		CRect rect;
		CDC* pDC = GetDC();
		int nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
		m_egBasic.GetClientRect(&rect);
		m_egBasic.SetColWidth(1, 0, MulDiv(rect.right, TWIPS_PER_INCH, nLogX) - nCol0/* - 50*/);
	}
}

void CUserGraphPg2::OnBRenumber()
{
	m_bRenumbering = true;

	if (UpdateData(TRUE))
	{
		CBasicObj basicCheck;
		if (basicCheck.Renumber(m_listCommands))
		{
			// update grid
			DisplayCommands();
		}
	}
	else
	{
		// do nothing BASIC contains errors
	}
	m_bRenumbering = false;
}

void CUserGraphPg2::DDX_ListCommands(CDataExchange *pDX)
{
	// validate  state
	ASSERT_VALID(this);

	// avoid flashing grid
	CDelayUpdate update(this, IDC_MSHFG_BASIC);

	if (pDX->m_bSaveAndValidate)
	{
		// create object to check basic
		CBasicObj basicCheck;

		std::list<basic_command> updatedList;
		std::list<basic_command> cleanedList;

		// save view
		long nRowSave = m_egBasic.GetRow();
		long nTopRowSave = m_egBasic.GetTopRow();

		for (long nRow = m_egBasic.GetFixedRows(); nRow < m_egBasic.GetRows(); ++nRow)
		{
			basic_command command;
			basicCheck.DDX_BasicCommand(pDX, IDC_MSHFG_BASIC, nRow, 0, command);

			if (command.strCommand.IsEmpty())
			{
				// don't add empty commands to list
				continue;
			}
			updatedList.push_back(command);

			// cleanup cmd
			std::string cmd(command.strCommand);
			std::string cmd_lower = cmd;
			Utilities::str_tolower(cmd_lower);
			if (cmd_lower.find("plot_xy") != std::string::npos)
			{
#if defined(__cplusplus_cli)
				ChartObject::ExtractCurveInfo(cmd); // truncates cmd
#endif
			}
			basic_command cleaned(command);
			cleaned.strCommand = cmd.c_str();
			cleanedList.push_back(cleaned);
		}

		// check by running
		basicCheck.DDV_BasicCommands(pDX, IDC_MSHFG_BASIC, cleanedList, m_bRenumbering);

		// restore view
		m_egBasic.SetRow(nRowSave);
		m_egBasic.SetTopRow(nTopRowSave);

		m_listCommands.assign(updatedList.begin(), updatedList.end());
	}
	else
	{
		if (m_bFirstSetActive)
		{
			// layout basic grid
			CRect rect;
			CDC* pDC = GetDC();
			int nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
			m_egBasic.GetClientRect(&rect);
			m_egBasic.SetColWidth(1, 0, MulDiv(rect.right, TWIPS_PER_INCH, nLogX) - 425 - 3);

			// set grid
			DisplayCommands();
		}
	}

	// validate  state
	ASSERT_VALID(this);
}

void CUserGraphPg2::DisplayCommands()
{
	ASSERT_VALID(this);

	// set line number column width
	m_egBasic.SetColWidth(0, 0, 625);

	// init m_egBasic rows
	long size = /*foundItem->rate.*/m_listCommands.size();
	if (m_egBasic.GetRows() < size + 5)
	{
		m_egBasic.SetRows(size + 5);
	}

	CString str;
	long nLastLineNumber = 0;
	long nRow = 0;

	std::list<basic_command>::iterator cmdIter = /*foundItem->rate.*/m_listCommands.begin();
	for (; cmdIter != /*foundItem->rate.*/m_listCommands.end(); ++cmdIter, ++nRow)
	{
		nLastLineNumber = cmdIter->nLine;
		str.Format(_T("%d"), cmdIter->nLine);
		m_egBasic.SetTextMatrix(nRow, 0, str);
		m_egBasic.SetTextMatrix(nRow, 1, cmdIter->strCommand);
	}

	// determine next line number
	long nLineNumber = ((nLastLineNumber / 10) + 1) * 10;
	for (; nRow < m_egBasic.GetRows(); nRow++, nLineNumber += 10)
	{
		str.Format(_T("%d"), nLineNumber);
		m_egBasic.SetTextMatrix(nRow, 0, str);
		m_egBasic.SetTextMatrix(nRow, 1, _T(""));
	}

	// reset grid
	m_egBasic.SetTopRow(0);
	m_egBasic.SetRow(0);
	m_egBasic.SetCol(1);

	ASSERT_VALID(this);
}

BEGIN_EVENTSINK_MAP(CUserGraphPg2, baseUserGraphPg2)
    //{{AFX_EVENTSINK_MAP(CUserGraphPg2)
	ON_EVENT(CUserGraphPg2, IDC_MSHFG_BASIC, 71 /* EnterCell */, OnEnterCellMshfgBasic, VTS_NONE)
	ON_EVENT(CUserGraphPg2, IDC_MSHFG_BASIC, -602 /* KeyDown */, OnKeyDownMshfgBasic, VTS_PI2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CUserGraphPg2::OnEnterCellMshfgBasic()
{
	CString strRes;
	switch (m_egBasic.GetCol())
	{
	case 0 :
		strRes.LoadString(IDS_RATE_261);
		break;
	case 1 :
		AfxFormatString1(strRes, IDS_USER_PRINT_297, _T("PUNCH"));
		break;
	}
	m_eInputDesc.SetWindowText(strRes);
}

void CUserGraphPg2::OnKeyDownMshfgBasic(short FAR* KeyCode, short Shift)
{
	UNUSED(Shift);

	switch (*KeyCode)
	{
	case VK_DELETE:
		{
			CWaitCursor wait;	// display hourglass cursor
			m_egBasic.SetRedraw(FALSE);
			long nCol    = m_egBasic.GetCol();
			long nColSel = m_egBasic.GetColSel();

			// check if entire row is selected
			if ((min(nCol, nColSel) == 0) && (1 == max(nCol, nColSel)))
			{
				long nRow    = m_egBasic.GetRow();
				long nRowSel = m_egBasic.GetRowSel();

				// determine how many rows to delete
				long cSelectedRows = abs(nRow - nRowSel) + 1;

				// determine first row to delete
				long iStartRow = min(nRow, nRowSel);

				// delete each row
				for (long i = 0; i < cSelectedRows; ++i)
				{
					m_egBasic.DeleteRow(iStartRow);
				}
			}
			else
			{
				// just fill with empty strings
				m_egBasic.SetFillStyle(flexFillRepeat);
				m_egBasic.SetText(_T(""));
				m_egBasic.SetFillStyle(flexFillSingle);
			}
			m_egBasic.SetRedraw(TRUE);
		}
		break;
	}
}

LRESULT CUserGraphPg2::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bDenyEdit = FALSE;	// allow edit

	return bDenyEdit;
}

LRESULT CUserGraphPg2::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bMakeChange = TRUE;

	// ignore if edit is cancelled
	if ( pInfo->item.pszText == NULL ) return bMakeChange;

	switch ( nID )
	{
	case IDC_MSHFG_NUM_DESC :
		break;

	case IDC_MSHFG_BASIC :
		// make sure grid has at least one empty row
		if (pInfo->item.iRow >= m_egBasic.GetRows() - m_egBasic.GetFixedRows() - 1)
		{
			m_egBasic.SetRows(m_egBasic.GetRows() + 5);
			// MAYBE number added lines
		}
		break;

	default:
		ASSERT(FALSE);
		break;
	}

	return bMakeChange;
}

LRESULT CUserGraphPg2::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
{
	UNUSED_ALWAYS(lParam);
	int nID = wParam;
	switch (nID)
	{
	case IDC_MSHFG_BASIC :
		OnEnterCellMshfgBasic();
		break;
	}
	return 0;
}

void CUserGraphPg2::OnSetfocusEHead()
{
	CString strRes;
	strRes.LoadString(IDS_USER_PUNCH_299);
	m_eInputDesc.SetWindowText(strRes);
}

BOOL CUserGraphPg2::OnHelpInfo(HELPINFO* pHelpInfo)
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
	case IDC_E_HEAD: case IDC_ST_HEAD:
		strRes.LoadString(IDS_USER_PUNCH_299);
		break;
	case IDC_B_RENUMBER:
		strRes.LoadString(IDS_STRING512);
		break;
	case IDC_LB_FUNCS: case IDC_ST_FUNCS:
		strRes.LoadString(IDS_RATE_258);
		break;
	case IDC_TREE_ARGS: case IDC_ST_ARGS:
		strRes.LoadString(IDS_RATE_260);
		break;
	case IDC_MSHFG_BASIC:
		if (!IsContextHelp())
		{
			if (!(m_egBasic.GetRowIsVisible(m_egBasic.GetRow()) && m_egBasic.GetColIsVisible(m_egBasic.GetCol())))
			{
				::MessageBeep((UINT)-1);
				return TRUE;
			}

			// modify placement
			CDC* pDC = m_egBasic.GetDC();
			int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
			int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

			long nLeft = ::MulDiv(m_egBasic.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
			long nTop  = ::MulDiv(m_egBasic.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

			CRect rc;
			m_egBasic.GetWindowRect(rc);

			myPopup.pt.x = rc.left + nLeft;
			myPopup.pt.y = rc.top + nTop + 10;
		}
		switch (IsContextHelp() ? m_egBasic.GetMouseCol() : m_egBasic.GetCol())
		{
		case 0 :
			strRes.LoadString(IDS_RATE_261);
			break;
		case 1 :
			AfxFormatString1(strRes, IDS_USER_PRINT_297, _T("PRINT"));
			break;
		}
		break;
	case IDC_E_EXPLAN: case IDC_ST_EXPLAN:
		strRes.LoadString(IDS_RATE_259);
		break;
	default:
		// No help topic is associated with this item.
		strRes.LoadString(IDS_STRING441);
		break;
	}

	myPopup.pszText = strRes;
	return ::HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}

void CUserGraphPg2::OnSetfocusTreeArgs(NMHDR* pNMHDR, LRESULT* pResult)
{
	UNUSED(pNMHDR);
	CString strRes;
	strRes.LoadString(IDS_RATE_260);
	m_eInputDesc.SetWindowText(strRes);

	*pResult = 0;
}

/////////////[[[[[[[[[[[[[[[[[[[
// COMMENT: {4/30/2012 3:34:45 PM}LRESULT CUserGraphPg2::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
// COMMENT: {4/30/2012 3:34:45 PM}{
// COMMENT: {4/30/2012 3:34:45 PM}	UINT nID = wParam;
// COMMENT: {4/30/2012 3:34:45 PM}	UNUSED_ALWAYS(lParam);
// COMMENT: {4/30/2012 3:34:45 PM}
// COMMENT: {4/30/2012 3:34:45 PM}	switch (nID)
// COMMENT: {4/30/2012 3:34:45 PM}	{
// COMMENT: {4/30/2012 3:34:45 PM}	case IDC_MSHFG_NUM_DESC :
// COMMENT: {4/30/2012 3:34:45 PM}		OnEnterCellMshfgNumDesc();
// COMMENT: {4/30/2012 3:34:45 PM}		break;
// COMMENT: {4/30/2012 3:34:45 PM}	case IDC_MSHFG_BASIC :
// COMMENT: {4/30/2012 3:34:45 PM}		OnEnterCellMshfgMix();
// COMMENT: {4/30/2012 3:34:45 PM}		break;
// COMMENT: {4/30/2012 3:34:45 PM}	default :
// COMMENT: {4/30/2012 3:34:45 PM}		ASSERT(FALSE);
// COMMENT: {4/30/2012 3:34:45 PM}		break;
// COMMENT: {4/30/2012 3:34:45 PM}	}
// COMMENT: {4/30/2012 3:34:45 PM}	return 0;
// COMMENT: {4/30/2012 3:34:45 PM}}
