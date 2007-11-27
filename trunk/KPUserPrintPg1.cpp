// KPUserPrintPg1.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "KPUserPrintPg1.h"

#include "DDX_DDV.h"
#include "DelayUpdate.h"
#include "BasicObj.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CKPUserPrintPg1, baseKPUserPrintPg1)

//#define IDC_LB_ARGS IDC_TREE_ARGS

/////////////////////////////////////////////////////////////////////////////
// CKPUserPrintPg1 property page

CKPUserPrintPg1::CKPUserPrintPg1() : baseKPUserPrintPg1(CKPUserPrintPg1::IDD)
, m_basicDesc(GetDatabase(), IDC_LB_FUNCS, IDC_E_EXPLAN, IDC_TREE_ARGS)
{
	m_bRenumbering = false;
	//{{AFX_DATA_INIT(CKPUserPrintPg1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CKPUserPrintPg1::~CKPUserPrintPg1()
{
}

void CKPUserPrintPg1::DoDataExchange(CDataExchange* pDX)
{
	baseKPUserPrintPg1::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKPUserPrintPg1)
	DDX_Control(pDX, IDC_E_DESC_INPUT, m_eInputDesc);
	DDX_Control(pDX, IDC_MSHFG_BASIC, m_egBasic);
	//}}AFX_DATA_MAP
	m_basicDesc.DoDataExchange(pDX);
	DDX_ListCommands(pDX);
}


BEGIN_MESSAGE_MAP(CKPUserPrintPg1, baseKPUserPrintPg1)
	//{{AFX_MSG_MAP(CKPUserPrintPg1)
	ON_LBN_SELCHANGE(IDC_LB_FUNCS, OnSelchangeLbFuncs)
	ON_LBN_SETFOCUS(IDC_LB_FUNCS, OnSetfocusLbFuncs)
	ON_EN_SETFOCUS(IDC_E_EXPLAN, OnSetfocusEExplan)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_B_RENUMBER, OnBRenumber)
	ON_WM_HELPINFO()
	ON_NOTIFY(NM_SETFOCUS, IDC_TREE_ARGS, OnSetfocusTreeArgs)
	//}}AFX_MSG_MAP
	// custom BN_SETFOCUS notifications
	ON_BN_SETFOCUS(IDC_B_RENUMBER, OnSetfocusBRenumber)
	// custom edit grid notifications
	ON_MESSAGE(EGN_BEGINCELLEDIT, OnBeginCellEdit)
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
	ON_MESSAGE(EGN_SETFOCUS, OnSetfocusGrid)
END_MESSAGE_MAP()



BOOL CKPUserPrintPg1::OnInitDialog()
{
	baseKPUserPrintPg1::OnInitDialog();

	CreateRoot(VERTICAL, 5, 6)
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

/////////////////////////////////////////////////////////////////////////////
// CKPUserPrintPg1 diagnostics

#ifdef _DEBUG
void CKPUserPrintPg1::AssertValid() const
{
	baseKPUserPrintPg1::AssertValid();
}

void CKPUserPrintPg1::Dump(CDumpContext& dc) const
{
	baseKPUserPrintPg1::Dump(dc);
}
#endif //_DEBUG

void CKPUserPrintPg1::OnSelchangeLbFuncs()
{
	m_basicDesc.OnSelchangeLbFuncs();
}

void CKPUserPrintPg1::OnSetfocusLbFuncs()
{
	CString strRes;
	strRes.LoadString(IDS_RATE_258);
	m_eInputDesc.SetWindowText(strRes);
}

void CKPUserPrintPg1::OnSetfocusLbArgs()
{
	CString strRes;
	strRes.LoadString(IDS_RATE_260);
	m_eInputDesc.SetWindowText(strRes);
}

void CKPUserPrintPg1::OnSetfocusEExplan()
{
	CString strRes;
	strRes.LoadString(IDS_RATE_259);
	m_eInputDesc.SetWindowText(strRes);
}

void CKPUserPrintPg1::OnSetfocusBRenumber()
{
	CString strRes;
	strRes.LoadString(IDS_STRING512);
	m_eInputDesc.SetWindowText(strRes);
}

void CKPUserPrintPg1::OnSize(UINT nType, int cx, int cy)
{
	baseKPUserPrintPg1::OnSize(nType, cx, cy);

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

void CKPUserPrintPg1::OnBRenumber()
{
	m_bRenumbering = true;

	if (UpdateData(TRUE))
	{
		CBasicObj basicCheck;
		if (basicCheck.Renumber(m_listCommands))
		{
			// update grid
			DisplayCommands();
			// DisplayTab(nRate);
		}
	}
	else
	{
		// do nothing BASIC contains errors
	}
	m_bRenumbering = false;
}

void CKPUserPrintPg1::DDX_ListCommands(CDataExchange *pDX)
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
		}

		// check by running
		basicCheck.DDV_BasicCommands(pDX, IDC_MSHFG_BASIC, updatedList, m_bRenumbering);

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

void CKPUserPrintPg1::DisplayCommands()
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

BEGIN_EVENTSINK_MAP(CKPUserPrintPg1, baseKPUserPrintPg1)
    //{{AFX_EVENTSINK_MAP(CKPUserPrintPg1)
	ON_EVENT(CKPUserPrintPg1, IDC_MSHFG_BASIC, 71 /* EnterCell */, OnEnterCellMshfgBasic, VTS_NONE)
	ON_EVENT(CKPUserPrintPg1, IDC_MSHFG_BASIC, -602 /* KeyDown */, OnKeyDownMshfgBasic, VTS_PI2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CKPUserPrintPg1::OnEnterCellMshfgBasic()
{
	CString strRes;
	switch (m_egBasic.GetCol())
	{
	case 0 :
		strRes.LoadString(IDS_RATE_261);
		break;
	case 1 :
		AfxFormatString1(strRes, IDS_USER_PRINT_297, _T("PRINT"));
		break;
	}
	m_eInputDesc.SetWindowText(strRes);
}

void CKPUserPrintPg1::OnKeyDownMshfgBasic(short FAR* KeyCode, short Shift)
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

LRESULT CKPUserPrintPg1::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bDenyEdit = FALSE;	// allow edit

	return bDenyEdit;
}

LRESULT CKPUserPrintPg1::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bMakeChange = TRUE;

	// ignore if edit is cancelled
	if ( pInfo->item.pszText == NULL ) return bMakeChange;

	switch ( nID )
	{
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

LRESULT CKPUserPrintPg1::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
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

BOOL CKPUserPrintPg1::OnHelpInfo(HELPINFO* pHelpInfo) 
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
	return HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}

void CKPUserPrintPg1::OnSetfocusTreeArgs(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNUSED(pNMHDR);
	CString strRes;
	strRes.LoadString(IDS_RATE_260);
	m_eInputDesc.SetWindowText(strRes);
	
	*pResult = 0;
}
