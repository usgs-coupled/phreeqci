// UserGraphPg2.cpp : implementation file
//

#include "stdafx.h"
#include "phreeqci2.h"
#include "UserGraphPg2.h"

#include "DDX_DDV.h"
#include "DelayUpdate.h"
#include "BasicObj.h"
#include "RichDocIn.h"
#include <Shlwapi.h>

#include "ChartObject.h"
#include "Utils.h"

#define METHOD1

// CUserGraphPg2 dialog

//IMPLEMENT_DYNAMIC(CUserGraphPg2, CPropertyPage)
IMPLEMENT_DYNCREATE(CUserGraphPg2, baseUserGraphPg2)

CUserGraphPg2::CUserGraphPg2() : baseUserGraphPg2(CUserGraphPg2::IDD)
, m_basicDesc(GetDatabase(), IDC_LB_FUNCS, IDC_E_EXPLAN, IDC_TREE_ARGS, true)
{

}

CUserGraphPg2::~CUserGraphPg2()
{
}

void CUserGraphPg2::DoDataExchange(CDataExchange* pDX)
{
	baseUserGraphPg2::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CVS, this->cvsListBox);

	DDX_Control(pDX, IDC_BUTTON_CVS_ADD, this->btnAdd);
	DDX_Control(pDX, IDC_BUTTON_CVS_DEL, this->btnDelete);
	

	if (m_bFirstSetActive)
	{
		//
		// Init NumDesc (USER_GRAPH doesn't support a range)
		//
		m_egNumDesc.SetColWidth(0, 0, 1100);
		m_egNumDesc.SetTextMatrix(0, 0, _T("Number"));
		m_egNumDesc.SetRowHeight(1, 0);

		for (size_t i = 0; i < this->csv_file_names.size(); ++i)
		{
			this->cvsListBox.InsertString(i, this->csv_file_names[i].c_str());
		}
#if defined(METHOD1)
		if (CWnd *pWnd = this->GetDlgItem(IDC_EDIT_CVS_FILE))
		{
			char buf[MAX_PATH];
			pWnd->GetWindowTextA(buf, MAX_PATH);
			this->btnAdd.EnableWindow(::strlen(buf) > 0);
		}
		this->btnDelete.EnableWindow(this->cvsListBox.GetCurSel() != LB_ERR);
#endif
	}

	//
	// plot_csv_file
	//
	if (pDX->m_bSaveAndValidate)
	{
		this->csv_file_names.clear();
		for (int j = 0; j < this->cvsListBox.GetCount(); ++j)
		{
			CString item;
			this->cvsListBox.GetText(j, item);
			this->csv_file_names.push_back((LPCTSTR)item);
		}
		ASSERT(this->cvsListBox.GetCount() == (int)this->csv_file_names.size());

		CString sval;
		DDX_Text(pDX, IDC_EDIT_CVS_FILE, sval);
		if (!sval.IsEmpty())
		{
			this->csv_file_names.push_back((LPCTSTR)sval);
		}
	}

	DDX_Control(pDX, IDC_MSHFG_BASIC, m_egBasic);
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

	ON_LBN_SELCHANGE(IDC_LIST_CVS, &CUserGraphPg2::OnLbnSelchangeListCvs)
	ON_BN_CLICKED(IDC_BUTTON_CVS_ADD, &CUserGraphPg2::OnBnClickedButtonCvsAdd)
	ON_BN_CLICKED(IDC_BUTTON_CVS_DEL, &CUserGraphPg2::OnBnClickedButtonCvsDelete)
	ON_EN_CHANGE(IDC_EDIT_CVS_FILE, &CUserGraphPg2::OnEnChangeEditCvsFile)
	ON_BN_CLICKED(IDC_BUTTON_CVS, &CUserGraphPg2::OnBnClickedButtonCvs)
	ON_EN_SETFOCUS(IDC_EDIT_CVS_FILE, &CUserGraphPg2::OnEnSetfocusEditCvsFile)
END_MESSAGE_MAP()

// CUserGraphPg2 message handlers

BOOL CUserGraphPg2::OnInitDialog()
{
	baseUserGraphPg2::OnInitDialog();

	CreateRoot(VERTICAL, 5, 6)
		<< (pane(HORIZONTAL, ABSOLUTE_VERT)
			<< item(IDC_MSHFG_NUM_DESC, ABSOLUTE_VERT)
			)
		<< (paneCtrl(IDC_STATIC_GB_CVS, HORIZONTAL, GREEDY, nDefaultBorder, 10, 10)
			<< item(IDC_STATIC_PLOT_CVS, NORESIZE | ALIGN_TOP)
			<< item(IDC_EDIT_CVS_FILE, ABSOLUTE_VERT | ALIGN_TOP)
			<< item(IDC_BUTTON_CVS, NORESIZE | ALIGN_TOP)
			<< item(IDC_LIST_CVS, NORESIZE | ALIGN_CENTER)
			<< item(IDC_BUTTON_CVS_ADD, NORESIZE | ALIGN_CENTER)
			<< item(IDC_BUTTON_CVS_DEL, NORESIZE | ALIGN_CENTER)			
			)
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
	case IDC_MSHFG_NUM_DESC:
		if (!IsContextHelp())
		{
			if (!(m_egNumDesc.GetRowIsVisible(m_egNumDesc.GetRow()) && m_egNumDesc.GetColIsVisible(m_egNumDesc.GetCol())))
			{
				::MessageBeep((UINT)-1);
				return TRUE;
			}

			// modify placement
			CDC* pDC = m_egNumDesc.GetDC();
			int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
			int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

			long nLeft = ::MulDiv(m_egNumDesc.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
			long nTop  = ::MulDiv(m_egNumDesc.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

			CRect rc;
			m_egNumDesc.GetWindowRect(rc);

			myPopup.pt.x = rc.left + nLeft;
			myPopup.pt.y = rc.top + nTop + 10;
		}
		switch (IsContextHelp() ? m_egNumDesc.GetMouseRow() : m_egNumDesc.GetRow())
		{
		case 0 :
			AfxFormatString1(strRes, IDS_STRING711, _T("user-graph") ); 
			break;
		case 1 :
			ASSERT(FALSE);
			break;
		case 2 :
			AfxFormatString1(strRes, IDS_STRING712, _T("user-graph chart") ); 
			break;
		default :
			// No help topic is associated with this item. 
			strRes.LoadString(IDS_STRING441);
			break;
		}	
		break;
	case IDC_STATIC_GB_CVS: case IDC_STATIC_PLOT_CVS: case IDC_EDIT_CVS_FILE: case IDC_BUTTON_CVS: case IDC_LIST_CVS: case IDC_BUTTON_CVS_ADD: case IDC_BUTTON_CVS_DEL:
		strRes.LoadString(IDS_STRING730);
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
			AfxFormatString1(strRes, IDS_STRING731, _T("PRINT"));
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

LRESULT CUserGraphPg2::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
{
	UINT nID = wParam;
	UNUSED_ALWAYS(lParam);

	switch (nID)
	{
	case IDC_MSHFG_NUM_DESC :
		OnEnterCellMshfgNumDesc();
		break;
	case IDC_MSHFG_BASIC :
		OnEnterCellMshfgBasic();
		break;
	default :
		ASSERT(FALSE);
		break;
	}
	return 0;
}

void CUserGraphPg2::OnEnterCellMshfgNumDesc() 
{
	CString strRes;
	switch (m_egNumDesc.GetRow())
	{
	case 0 :
		// Positive number to designate this %1 and its composition. Default is 1.
		AfxFormatString1(strRes, IDS_STRING711, _T("user-graph") ); 
		break;
	case 1 :
		ASSERT(FALSE);
		break;
	case 2 :
		// Optional comment that describes the %1. The description will appear
		// in the title of the chart window.
		AfxFormatString1(strRes, IDS_STRING712, _T("user-graph chart") ); 
		break;
	default :
		ASSERT(FALSE);
		break;
	}

	m_eInputDesc.SetWindowText(strRes);
}

void CUserGraphPg2::OnLbnSelchangeListCvs()
{
#if defined(METHOD1)
	int n = this->cvsListBox.GetCurSel();
	this->btnDelete.EnableWindow(n != LB_ERR);
#else
	int n = this->cvsListBox.GetCurSel();
	if (n == LB_ERR)
	{
		this->btnDelete.EnableWindow(FALSE);
	}
	else
	{
		CString name;
		this->cvsListBox.GetText(n, name);
		if (CWnd *pWnd = this->GetDlgItem(IDC_EDIT_CVS_FILE))
		{
			pWnd->SetWindowTextA(name);
		}
		this->btnDelete.EnableWindow(TRUE);
	}
#endif
}

void CUserGraphPg2::OnBnClickedButtonCvsAdd()
{
#if defined(METHOD1)
	if (CWnd *pWnd = this->GetDlgItem(IDC_EDIT_CVS_FILE))
	{
		char buf[MAX_PATH];
		pWnd->GetWindowTextA(buf, MAX_PATH);
		int n = this->cvsListBox.InsertString(-1, buf);
		this->cvsListBox.SetCurSel(n);
		pWnd->SetWindowTextA(_T(""));
		this->btnDelete.EnableWindow(TRUE);
		pWnd->SetFocus();
	}
#else
	int n = this->cvsListBox.GetCurSel();
	////if (n == LB_ERR)
	{
		if (CWnd *pWnd = this->GetDlgItem(IDC_EDIT_CVS_FILE))
		{
			char buf[MAX_PATH];
			pWnd->GetWindowTextA(buf, MAX_PATH);

			n = this->cvsListBox.GetCount();
			n = this->cvsListBox.InsertString(n, buf);
			//this->cvsListBox.SetCurSel(n);
			this->cvsListBox.SetCurSel(-1);
			pWnd->SetWindowTextA(_T(""));
		}
	}
#endif
}

void CUserGraphPg2::OnBnClickedButtonCvsDelete()
{
#if defined(METHOD1)
	int n = this->cvsListBox.GetCurSel();
	if (n == LB_ERR)
	{
		ASSERT(FALSE);
	}
	else
	{
		CString name;
		this->cvsListBox.GetText(n, name);

		if (CWnd *pWnd = this->GetDlgItem(IDC_EDIT_CVS_FILE))
		{
			pWnd->SetWindowTextA(name);
			pWnd->SetFocus();
			pWnd->SendMessage(EM_SETSEL, 0, -1);
		}

		int count = this->cvsListBox.DeleteString(n);
		if (count > n)
		{
			this->cvsListBox.SetCurSel(n);
		}
		else
		{
			this->cvsListBox.SetCurSel(-1);
			this->btnDelete.EnableWindow(FALSE);
		}
	}
#else
	int n = this->cvsListBox.GetCurSel();
	if (n == LB_ERR)
	{
		ASSERT(FALSE);
	}
	else
	{
		this->cvsListBox.DeleteString(n);
		this->cvsListBox.SetCurSel(-1);
		this->btnDelete.EnableWindow(FALSE);
		if (CWnd *pWnd = this->GetDlgItem(IDC_EDIT_CVS_FILE))
		{
			pWnd->SetWindowTextA(_T(""));
		}
	}
#endif
}

void CUserGraphPg2::OnEnChangeEditCvsFile()
{
#if defined(METHOD1)
	if (CWnd *pWnd = this->GetDlgItem(IDC_EDIT_CVS_FILE))
	{
		char buf[MAX_PATH];
		pWnd->GetWindowTextA(buf, MAX_PATH);
		this->btnAdd.EnableWindow(::strlen(buf) > 0);
	}
#else
	int n = this->cvsListBox.GetCurSel();
	if (n != LB_ERR)
	{
		if (CWnd *pWnd = this->GetDlgItem(IDC_EDIT_CVS_FILE))
		{
			char buf[MAX_PATH];
			pWnd->GetWindowTextA(buf, MAX_PATH);
			this->cvsListBox.DeleteString(n);
			this->cvsListBox.InsertString(n, buf);
			this->cvsListBox.SetCurSel(n);
		}
	}
#endif
}

void CUserGraphPg2::OnBnClickedButtonCvs()
{
	CString str;

	// Show file Dialog box
	CFileDialog dlg(
		TRUE,					// bOpenFileDialog
		NULL,					// lpszDefExt
		str,
		OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, 
		_T("CSV Files (*.csv)|*.csv|Dat Files (*.dat)|*.dat|All Files (*.*)|*.*||")
		);

	// set dialog caption
	dlg.m_ofn.lpstrTitle = _T("Select a CSV file");
	
	if (dlg.DoModal() == IDOK)
	{
		if (CWnd *pWnd = this->GetDlgItem(IDC_EDIT_CVS_FILE))
		{
			str = dlg.GetPathName();

			CRichDocIn *pDoc = this->GetActiveDocument();
			CString pathName(pDoc->GetPathName());

			TCHAR szOut[MAX_PATH] = "";
			if (::PathIsSameRoot(pathName, str))
			{
				VERIFY(::PathCanonicalize(szOut, str));
				str = szOut;
				VERIFY(::PathRelativePathTo(szOut, pathName, FILE_ATTRIBUTE_NORMAL, str, FILE_ATTRIBUTE_NORMAL));
				str = szOut;
				if (::strlen(szOut) > 2 && szOut[0] == '.' && szOut[1] == '\\')
				{
					str = szOut + 2;
				}
			}

			this->btnAdd.EnableWindow(str.GetLength());
			pWnd->SetWindowTextA(str);
			pWnd->SetFocus();
			pWnd->SendMessage(EM_SETSEL, 0, -1);
		}
	}	
}

void CUserGraphPg2::OnEnSetfocusEditCvsFile()
{
	CString strRes;
	// Identifier selects a file containing data to be plotted on the chart. The first line
	// of the file is a set of headings, one for the X axis, followed by one for each curve
	// to be plotted. All headings and data are tab delimited. It is possible to set curve
	// properties by special values in the first column beginning at line 2 in the file. Up
	// to five lines of special values may be defined; each special value is followed by 
	// settings for each curve (tab delimited). The special values are “color”, “symbol”,
	// “symbol_size”, “line_width”, and “y_axis”. Legal values for these settings are 
	// described in the explanation for Line 10. The data lines follow the lines that 
	// define special values; each data line has an X value in the first column followed by
	// Y values for each curve; missing values are indicated by consecutive tab characters.
	strRes.LoadString(IDS_STRING730);
	m_eInputDesc.SetWindowText(strRes);
}
