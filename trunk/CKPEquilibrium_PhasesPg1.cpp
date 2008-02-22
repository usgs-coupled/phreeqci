// CKPEquilibrium_PhasesPg1.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "CKPEquilibrium_PhasesPg1.h"

#include "RichDocIn.h"
#include "Database.h"
#include "Util.h"
#include "DDX_DDV.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CCKPEquilibrium_PhasesPg1, baseCKPEquilibrium_PhasesPg1)


/////////////////////////////////////////////////////////////////////////////
// CCKPEquilibrium_PhasesPg1 property page

CCKPEquilibrium_PhasesPg1::CCKPEquilibrium_PhasesPg1()
 : baseCKPEquilibrium_PhasesPg1(CCKPEquilibrium_PhasesPg1::IDD)
 , m_glDoc(&m_ctrlPhaseEditGrid, &m_ctrlPhaseCheckList, 1)
// COMMENT: {8/28/2001 1:29:00 PM} , m_bComboFilledOnce(false)
{
	//{{AFX_DATA_INIT(CCKPEquilibrium_PhasesPg1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCKPEquilibrium_PhasesPg1::~CCKPEquilibrium_PhasesPg1()
{
}

void CCKPEquilibrium_PhasesPg1::DoDataExchange(CDataExchange* pDX)
{
	baseCKPEquilibrium_PhasesPg1::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCKPEquilibrium_PhasesPg1)
	DDX_Control(pDX, IDC_CL_PHASE, m_ctrlPhaseCheckList);
	DDX_Control(pDX, IDC_MSHFG_PHASE, m_ctrlPhaseEditGrid);
	//}}AFX_DATA_MAP
	DDX_PurePhaseList(pDX, IDC_MSHFG_PHASE);
}

BEGIN_MESSAGE_MAP(CCKPEquilibrium_PhasesPg1, baseCKPEquilibrium_PhasesPg1)
	//{{AFX_MSG_MAP(CCKPEquilibrium_PhasesPg1)
	ON_NOTIFY(NM_SETFOCUS, IDC_CL_PHASE, OnSetfocusClPhase)
	ON_WM_CONTEXTMENU()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CL_PHASE, OnItemchangedClPhase)
	ON_WM_SIZE()
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP

	// custom edit grid notifications
	ON_MESSAGE(EGN_BEGINCELLEDIT, OnBeginCellEdit)
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
	ON_MESSAGE(EGN_SETFOCUS, OnSetfocusGrid)

	// custom check list notifications
	ON_COMMAND_RANGE(ID_VIEW_AS_LIST, ID_LIST_CLEAR_ALL, OnViewChange)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CCKPEquilibrium_PhasesPg1, baseCKPEquilibrium_PhasesPg1)
    //{{AFX_EVENTSINK_MAP(CCKPEquilibrium_PhasesPg1)
	ON_EVENT(CCKPEquilibrium_PhasesPg1, IDC_MSHFG_NUM_DESC, 71 /* EnterCell */, OnEnterCellMshfgNumDesc, VTS_NONE)
	ON_EVENT(CCKPEquilibrium_PhasesPg1, IDC_MSHFG_PHASE, 71 /* EnterCell */, OnEnterCellMshfgPhase, VTS_NONE)
	ON_EVENT(CCKPEquilibrium_PhasesPg1, IDC_MSHFG_PHASE, -602 /* KeyDown */, OnKeyDownMshfgPhase, VTS_PI2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BOOL CCKPEquilibrium_PhasesPg1::OnInitDialog() 
{
	baseCKPEquilibrium_PhasesPg1::OnInitDialog();
	
	// Add extra initialization here

	// set layout
	CreateRoot(VERTICAL, 5, 6) 
		<< item(IDC_MSHFG_NUM_DESC, ABSOLUTE_VERT)

		<< item(IDC_S_DEF_PHASE, NORESIZE | ALIGN_BOTTOM)

		<< item(IDC_CL_PHASE, GREEDY)

		<< item(IDC_MSHFG_PHASE, GREEDY)

		<< ( paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, GREEDY, nDefaultBorder, 10, 10)
			<< item(IDC_E_DESC_INPUT, ABSOLUTE_VERT)
		);
	UpdateLayout();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCKPEquilibrium_PhasesPg1::InitPhaseCheckList()
{
	CUtil::InsertPhases(&m_ctrlPhaseCheckList, GetDatabase());

	m_ctrlPhaseCheckList.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);

	return TRUE;
}

BOOL CCKPEquilibrium_PhasesPg1::InitPhaseEditGrid()
{
	//{{ new support for dissolve only
	m_ctrlPhaseEditGrid.SetCols(0, 6);
	//}} new support for dissolve only

	// set column titles
	m_ctrlPhaseEditGrid.SetTextMatrix(0, 1, _T("Name"));
	m_ctrlPhaseEditGrid.SetTextMatrix(0, 2, _T("Sat. index"));
	m_ctrlPhaseEditGrid.SetTextMatrix(0, 3, _T("Amount (moles)"));
	m_ctrlPhaseEditGrid.SetTextMatrix(0, 4, _T("[Alt. formula]"));
	//{{ new support for dissolve only
	m_ctrlPhaseEditGrid.SetTextMatrix(0, 5, _T("[Dissolve only]"));
	//}} new support for dissolve only

	// set alignment
	m_ctrlPhaseEditGrid.SetColAlignment(1, flexAlignLeftCenter);

	// set bold titles
	m_ctrlPhaseEditGrid.SetRow(0), m_ctrlPhaseEditGrid.SetCol(1);
	m_ctrlPhaseEditGrid.SetCellFontBold(TRUE);
	m_ctrlPhaseEditGrid.SetRow(0), m_ctrlPhaseEditGrid.SetCol(2);
	m_ctrlPhaseEditGrid.SetCellFontBold(TRUE);
	m_ctrlPhaseEditGrid.SetRow(0), m_ctrlPhaseEditGrid.SetCol(3);
	m_ctrlPhaseEditGrid.SetCellFontBold(TRUE);

	// center align the column headers
	m_ctrlPhaseEditGrid.SetFillStyle(flexFillRepeat);
	m_ctrlPhaseEditGrid.SetRow(0);
	m_ctrlPhaseEditGrid.SetCol(1);
	m_ctrlPhaseEditGrid.SetColSel(m_ctrlPhaseEditGrid.GetCols(0) - 1);
	m_ctrlPhaseEditGrid.SetCellAlignment(flexAlignCenterCenter);
	m_ctrlPhaseEditGrid.SetFillStyle(flexFillSingle);

	// set column widths
// COMMENT: {8/28/2001 1:05:19 PM}	m_ctrlPhaseEditGrid.SetColWidth(0, 0, 500);
// COMMENT: {8/28/2001 1:05:19 PM}	m_ctrlPhaseEditGrid.SetColWidth(1, 0, 1440);
// COMMENT: {8/28/2001 1:05:19 PM}	m_ctrlPhaseEditGrid.SetColWidth(2, 0, 1440);
// COMMENT: {8/28/2001 1:05:19 PM}	m_ctrlPhaseEditGrid.SetColWidth(3, 0, 1440);
// COMMENT: {8/28/2001 1:05:19 PM}	m_ctrlPhaseEditGrid.SetColWidth(4, 0, 1600);

	m_ctrlPhaseEditGrid.SetColWidth(0, 0, 500);
	m_ctrlPhaseEditGrid.SetColWidth(1, 0, 1240);
	m_ctrlPhaseEditGrid.SetColWidth(2, 0, 1240);
	m_ctrlPhaseEditGrid.SetColWidth(3, 0, 1440);
	m_ctrlPhaseEditGrid.SetColWidth(4, 0, 1300);
	m_ctrlPhaseEditGrid.SetColWidth(5, 0, 1300);

	// set row titles
	m_ctrlPhaseEditGrid.AddRowHeaders();

	// center align row titles
	m_ctrlPhaseEditGrid.SetColAlignmentFixed(0, flexAlignCenterCenter);

	// scroll to top of grid
	m_ctrlPhaseEditGrid.SetTopRow(1); m_ctrlPhaseEditGrid.SetLeftCol(1);

	// set initial cell
	m_ctrlPhaseEditGrid.SetRow(1); m_ctrlPhaseEditGrid.SetCol(1);

	return TRUE;
}

void CCKPEquilibrium_PhasesPg1::OnEnterCellMshfgNumDesc() 
{
	CString strRes;

	switch (m_egNumDesc.GetRow())
	{
	case 0 :
		AfxFormatString1(strRes, IDS_N_USER_140, _T("phase assemblage"));
		break;
	case 1 :
		AfxFormatString1(strRes, IDS_N_USER_END_141, _T("phase"));
		break;
	case 2 :
		AfxFormatString1(strRes, IDS_DESC_STR_142, _T("phase assemblage"));
		break;
	default :
		ASSERT(FALSE);
		break;
	}	
	m_eInputDesc.SetWindowText(strRes);
}

void CCKPEquilibrium_PhasesPg1::OnEnterCellMshfgPhase() 
{
	CString strRes;
	CString strRes2;

	switch (m_ctrlPhaseEditGrid.GetCol())
	{
	case 1 :
		strRes.LoadString(IDS_EQ_PH_1_143);
		break;
	case 2 :
		strRes.LoadString(IDS_EQ_PH_2_144);
		break;
	case 3 :
		strRes.LoadString(IDS_EQ_PH_3_145);
		break;
	case 4 :
		strRes.LoadString(IDS_EQ_PH_4A_146);
		strRes2.LoadString(IDS_EQ_PH_4B_147);
		strRes += strRes2;
		break;
	case 5:
		strRes.LoadString(IDS_STRING615);
		break;
	default :
		ASSERT(FALSE);
		break;
	}
	m_eInputDesc.SetWindowText(strRes);
}

void CCKPEquilibrium_PhasesPg1::OnSetfocusClPhase(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ASSERT(pNMHDR->hwndFrom == m_ctrlPhaseCheckList.m_hWnd);
	UNUSED(pNMHDR);   // unused in release builds

	CString strRes;

	strRes.LoadString(IDS_EQ_PH_CL_148);

	m_eInputDesc.SetWindowText(strRes);
	
	*pResult = 0;
}

LRESULT CCKPEquilibrium_PhasesPg1::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
{
	UINT nID = wParam;
	UNUSED_ALWAYS(lParam);

	switch (nID)
	{
	case IDC_MSHFG_NUM_DESC :
		OnEnterCellMshfgNumDesc();
		break;
	case IDC_MSHFG_PHASE :
		OnEnterCellMshfgPhase();
		break;
	}
	return 0;
}

void CCKPEquilibrium_PhasesPg1::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	if (pWnd == &m_ctrlPhaseCheckList) 
	{
		if (point.x == -1 && point.y == -1)
		{
			//keystroke invocation
			CRect rect;

			pWnd->GetClientRect(rect);
			pWnd->ClientToScreen(rect);

			point = rect.TopLeft();
			point.Offset(5, 5);
		}

		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_CHECKLIST));

		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);

		pPopup->CheckMenuRadioItem(ID_VIEW_AS_LIST, ID_VIEW_AS_REPORT,
			(m_ctrlPhaseCheckList.GetStyle() & LVS_TYPEMASK) == LVS_LIST ? ID_VIEW_AS_LIST : ID_VIEW_AS_REPORT,
			MF_BYCOMMAND );
		
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
			this/*pWndPopupOwner*/);
	}
}

void CCKPEquilibrium_PhasesPg1::OnViewChange(UINT uID)
{
	CWaitCursor wait;
	int i, nElements;
	
	switch (uID)
	{
	case ID_VIEW_AS_LIST :
		m_ctrlPhaseCheckList.ModifyStyle(LVS_REPORT, LVS_LIST);
		break;

	case ID_VIEW_AS_REPORT :
		m_ctrlPhaseCheckList.ModifyStyle(LVS_LIST, LVS_REPORT);
		m_ctrlPhaseCheckList.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
		m_ctrlPhaseCheckList.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
		m_ctrlPhaseCheckList.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);
		m_ctrlPhaseCheckList.RedrawWindow();
		break;

	case ID_LIST_CLEAR_ALL :
		nElements = m_ctrlPhaseCheckList.GetItemCount();
		for (i = 0; i < nElements; ++i)
		{
			if (m_ctrlPhaseCheckList.GetCheckBox(i) == BST_CHECKED)
			{
				m_ctrlPhaseCheckList.SetCheckBox(i, BST_UNCHECKED);
			}
		}
		break;

	default :
		ASSERT(FALSE);
		break;
	}
}

LRESULT CCKPEquilibrium_PhasesPg1::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bDenyEdit = FALSE;	// allow edit

	if ( nID == IDC_MSHFG_PHASE )
	{
		switch ( pInfo->item.iCol )
		{
		case 1 :
			// set to phase list
			pInfo->item.bUseCombo = (CUtil::InsertPhases(pInfo->item.hWndCombo, GetDatabase()) > 0);
// COMMENT: {8/28/2001 1:29:34 PM}			if (!m_bComboFilledOnce)
// COMMENT: {8/28/2001 1:29:34 PM}			{
// COMMENT: {8/28/2001 1:29:34 PM}				m_bComboFilledOnce = true;
// COMMENT: {8/28/2001 1:29:34 PM}				pInfo->item.bUseCombo = (CUtil::InsertPhases(pInfo->item.hWndCombo, GetDatabase()) > 0);
// COMMENT: {8/28/2001 1:29:34 PM}			}
// COMMENT: {8/28/2001 1:29:34 PM}			else
// COMMENT: {8/28/2001 1:29:34 PM}			{
// COMMENT: {8/28/2001 1:29:34 PM}				if (pInfo->item.hWndCombo)
// COMMENT: {8/28/2001 1:29:34 PM}					pInfo->item.bUseCombo = (::SendMessage(pInfo->item.hWndCombo, CB_GETCOUNT, 0, 0) > 0);
// COMMENT: {8/28/2001 1:29:34 PM}				else
// COMMENT: {8/28/2001 1:29:34 PM}					pInfo->item.bUseCombo = (CUtil::InsertPhases(pInfo->item.hWndCombo, GetDatabase()) > 0);
// COMMENT: {8/28/2001 1:29:34 PM}
// COMMENT: {8/28/2001 1:29:34 PM}			}
			break;
		case 5:
			if (pInfo->item.hWndCombo)
			{
				CComboBox* pCombo = (CComboBox*)CWnd::FromHandle(pInfo->item.hWndCombo);
				ASSERT_KINDOF(CComboBox, pCombo);	// must be combobox
				if (pCombo)
				{
					// remove prev items
					pCombo->ResetContent();
					pCombo->AddString(_T(""));
					pCombo->AddString(_T("true"));

					// set drop width
					CDC* pDC = pCombo->GetDC();
					CSize size = pDC->GetTextExtent(_T("true"));
					pCombo->SetDroppedWidth(size.cx);
					pInfo->item.bUseCombo = (pCombo->GetCount() > 0);
				}
			}
			else
			{
				pInfo->item.bUseCombo = TRUE;
			}
			break;

		default :
			break;

		}
	}

	return bDenyEdit;
}

LRESULT CCKPEquilibrium_PhasesPg1::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	UNUSED(pInfo);

	BOOL bMakeChange = TRUE;

	// ignore if edit is cancelled
	if ( pInfo->item.pszText == NULL ) return bMakeChange;

	switch ( nID )
	{
	case IDC_MSHFG_PHASE :
		if (_tcslen(pInfo->item.pszText))
		{
			// fill in defaults
			if (m_ctrlPhaseEditGrid.GetTextMatrix(pInfo->item.iRow, 2).IsEmpty())
			{
				m_ctrlPhaseEditGrid.SetTextMatrix(pInfo->item.iRow, 2, _T("0.0"));
			}
			if (m_ctrlPhaseEditGrid.GetTextMatrix(pInfo->item.iRow, 3).IsEmpty())
			{
				m_ctrlPhaseEditGrid.SetTextMatrix(pInfo->item.iRow, 3, _T("10.0"));
			}
		}
		return m_glDoc.OnEndCellEdit(wParam, lParam);
		break;

	case IDC_MSHFG_NUM_DESC :
		break;
	}

	return bMakeChange;
}

void CCKPEquilibrium_PhasesPg1::OnItemchangedClPhase(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// do default
	long row = m_glDoc.OnItemChanged(pNMHDR, pResult);
	if (row != -1)
	{
		if (m_ctrlPhaseEditGrid.GetTextMatrix(row, 2).IsEmpty())
			m_ctrlPhaseEditGrid.SetTextMatrix(row, 2, _T("0.0"));
		if (m_ctrlPhaseEditGrid.GetTextMatrix(row, 3).IsEmpty())
			m_ctrlPhaseEditGrid.SetTextMatrix(row, 3, _T("10.0"));
	}
}

void CCKPEquilibrium_PhasesPg1::OnKeyDownMshfgPhase(short FAR* KeyCode, short Shift) 
{
	// mustn't have any disabled cells
	m_glDoc.OnKeyDown(KeyCode, Shift);
}

void CCKPEquilibrium_PhasesPg1::DDX_PurePhaseList(CDataExchange *pDX, int nIDC)
{
	ASSERT(nIDC == IDC_MSHFG_PHASE);

	// do only on initialization
	if (m_bFirstSetActive)
	{
		InitPhaseCheckList();
		InitPhaseEditGrid();
	}

	if (pDX->m_bSaveAndValidate)
	{
		std::list<CPurePhase> listPurePhase;
		CString strDummy;
		for (long nRow = 1; nRow < m_ctrlPhaseEditGrid.GetRows(); ++nRow)
		{
			CPurePhase purePhase;

			// phase name
			DDX_GridText(pDX, nIDC, nRow, 1, purePhase.m_strName);
			if (purePhase.m_strName.IsEmpty())
				continue;

			// saturation index
			DDX_GridText(pDX, nIDC, nRow, 2, purePhase.m_dSI);

			// alternative formula or phase
			DDX_GridText(pDX, nIDC, nRow, 4, purePhase.m_strAlt);

			// amount
			DDX_GridText(pDX, nIDC, nRow, 3, purePhase.m_dAmount);

			// dissolve only
			DDX_GridText(pDX, nIDC, nRow, 5, strDummy);
			strDummy.TrimLeft();
			if (!strDummy.IsEmpty())
			{
				switch (strDummy[0])
				{
				case _T('T') : case _T('t') : case _T('Y') : case _T('y') :
					purePhase.m_bDissolveOnly = true;
					break;

				case _T('F') : case _T('f') : case _T('N') : case _T('n') :
					purePhase.m_bDissolveOnly = false;
					break;

				default :
					::AfxMessageBox(_T("The dissolve only option should be \"true\", \"false\" or empty."));
					pDX->Fail();
					break;
				}
			}

			if (purePhase.m_bDissolveOnly && !purePhase.m_strAlt.IsEmpty())
			{
				::AfxMessageBox(_T("The dissolve only option cannot be used when adding an alternative reaction."));
				pDX->Fail();
			}

			// if here no exception has occured so add to list
			listPurePhase.push_back(purePhase);
		}
		// if here listPurePhase is valid and can be assigned to the member variable
		m_listPurePhase.empty();
		m_listPurePhase.assign(listPurePhase.begin(), listPurePhase.end());
	}
	else
	{
		std::list<CPurePhase>::const_iterator iter = m_listPurePhase.begin();		
		for (; iter != m_listPurePhase.end(); ++iter)
		{
			// phase name [saturation index [(alternative formula or alternative phase) [amount]]]
			CPurePhase purePhase(*iter);

			// phase name
			// add to grid list doc
			int nRow = m_glDoc.AddItem(purePhase.m_strName).m_nRow;

			// saturation index
			DDX_GridText(pDX, nIDC, nRow, 2, purePhase.m_dSI);

			// alternative formula or phase
			if ( !(*iter).m_strAlt.IsEmpty() )
			{
				DDX_GridText(pDX, nIDC, nRow, 4, purePhase.m_strAlt);
			}

			// amount
			DDX_GridText(pDX, nIDC, nRow, 3, purePhase.m_dAmount);

			// dissolve only
			if (purePhase.m_bDissolveOnly)
			{
				CString str = _T("true");
				DDX_GridText(pDX, nIDC, nRow, 5, str);
			}
		}
	}
}

void CCKPEquilibrium_PhasesPg1::OnSize(UINT nType, int cx, int cy) 
{
	baseCKPEquilibrium_PhasesPg1::OnSize(nType, cx, cy);
	
	// Add your message handler code here
	if (::GetKeyState(VK_CONTROL) < 0)
	{
	}
	
}

BOOL CCKPEquilibrium_PhasesPg1::OnHelpInfo(HELPINFO* pHelpInfo) 
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
	CString strResEx;

	switch (pHelpInfo->iCtrlId)
	{
	case IDC_CL_PHASE: case IDC_S_DEF_PHASE:
		strRes.LoadString(IDS_EQ_PH_CL_148);
		break;
	case IDC_MSHFG_PHASE:
		if (!IsContextHelp())
		{
			if (!(m_ctrlPhaseEditGrid.GetRowIsVisible(m_ctrlPhaseEditGrid.GetRow()) && m_ctrlPhaseEditGrid.GetColIsVisible(m_ctrlPhaseEditGrid.GetCol())))
			{
				::MessageBeep((UINT)-1);
				return TRUE;
			}

			// modify placement
			CDC* pDC = m_ctrlPhaseEditGrid.GetDC();
			int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
			int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

			long nLeft = ::MulDiv(m_ctrlPhaseEditGrid.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
			long nTop  = ::MulDiv(m_ctrlPhaseEditGrid.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

			CRect rc;
			m_ctrlPhaseEditGrid.GetWindowRect(rc);

			myPopup.pt.x = rc.left + nLeft;
			myPopup.pt.y = rc.top + nTop + 10;
		}
		switch (IsContextHelp() ? m_ctrlPhaseEditGrid.GetMouseCol() : m_ctrlPhaseEditGrid.GetCol())
		{
		case 1 :
			strRes.LoadString(IDS_EQ_PH_1_143);
			break;
		case 2 :
			strRes.LoadString(IDS_EQ_PH_2_144);
			break;
		case 3 :
			strRes.LoadString(IDS_EQ_PH_3_145);
			break;
		case 4 :
			strRes.LoadString(IDS_EQ_PH_4A_146);
			strResEx.LoadString(IDS_EQ_PH_4B_147);
			strRes += strResEx;
			break;
		case 5:
			strRes.LoadString(IDS_STRING615);
			break;
		default :
			// No help topic is associated with this item. 
			strRes.LoadString(IDS_STRING441);
			break;
		}
		break;
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
			AfxFormatString1(strRes, IDS_N_USER_140, _T("phase assemblage"));
			break;
		case 1 :
			AfxFormatString1(strRes, IDS_N_USER_END_141, _T("phase"));
			break;
		case 2 :
			AfxFormatString1(strRes, IDS_DESC_STR_142, _T("phase assemblage"));
			break;
		default :
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
