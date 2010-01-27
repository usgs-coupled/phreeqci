// KPSelectedOutputPg9.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "KPSelectedOutputPg9.h"

#include "Util.h"
#include "DelayUpdate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKPSelectedOutputPg9 property page

IMPLEMENT_DYNCREATE(CKPSelectedOutputPg9, baseCKPSelectedOutputPg9)

CKPSelectedOutputPg9::CKPSelectedOutputPg9() : baseCKPSelectedOutputPg9(CKPSelectedOutputPg9::IDD)
, m_glDoc(&m_egSolid, &m_clcSolid, 1)
{
	//{{AFX_DATA_INIT(CKPSelectedOutputPg9)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CKPSelectedOutputPg9::~CKPSelectedOutputPg9()
{
}

void CKPSelectedOutputPg9::DoDataExchange(CDataExchange* pDX)
{
	baseCKPSelectedOutputPg9::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKPSelectedOutputPg9)
	DDX_Control(pDX, IDC_DESCRIPTION, m_eInputDesc);
	DDX_Control(pDX, IDC_CL_SOLID, m_clcSolid);
	DDX_Control(pDX, IDC_MSHFG_SOLID, m_egSolid);
	//}}AFX_DATA_MAP
	CWaitCursor wait;

	// reduce grid flicker
	CDelayUpdate update(this, IDC_MSHFG_SOLID);

	if (m_bFirstSetActive)
	{
		CUtil::InsertSolidSolutions(&m_clcSolid, GetDatabase());
		m_egSolid.AddRowHeaders();
		m_egSolid.SetTextMatrix(0, 1, _T("Name"));
		m_egSolid.SetColWidth(1, 0, 1400);

		// center align the column headers
		m_egSolid.SetFillStyle(flexFillRepeat);
		m_egSolid.SetRow(0);
		m_egSolid.SetCol(0);
		m_egSolid.SetColSel(m_egSolid.GetCols(0) - 1);
		m_egSolid.SetCellAlignment(flexAlignCenterCenter);
		m_egSolid.SetFillStyle(flexFillSingle);


		// set bold titles
		m_egSolid.SetRow(0), m_egSolid.SetCol(1);
		m_egSolid.SetCellFontBold(TRUE);

		// set initial cell
		m_egSolid.SetRow(1), m_egSolid.SetCol(1);
	}
	if (pDX->m_bSaveAndValidate)
	{
		std::list<CString> listSolid;
		for (long nRow = m_egSolid.GetFixedRows(); nRow < m_egSolid.GetRows(); ++nRow)
		{
			CString strSolid;

			DDX_GridText(pDX, IDC_MSHFG_SOLID, nRow, 1, strSolid);
			if (strSolid.IsEmpty())
				continue;

			listSolid.push_back(strSolid);
		}


		// if here list is valid and can be assigned to the member variable
		m_listSolid.assign(listSolid.begin(), listSolid.end());

	}
	else
	{
		// see OnKillActive
		ASSERT(m_glDoc.GetCount() == 0);

		std::list<CString>::iterator iter = m_listSolid.begin();
		for (; iter != m_listSolid.end(); ++iter)
		{
			m_glDoc.AddItem(*iter);
		}
		ASSERT(m_listSolid.size() == m_glDoc.GetCount());
	}
}


BEGIN_MESSAGE_MAP(CKPSelectedOutputPg9, baseCKPSelectedOutputPg9)
	//{{AFX_MSG_MAP(CKPSelectedOutputPg9)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CL_SOLID, OnItemchangedClSolid)
	ON_NOTIFY(NM_SETFOCUS, IDC_CL_SOLID, OnSetfocusClSolid)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
	ON_MESSAGE(EGN_BEGINCELLEDIT, OnBeginCellEdit)
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
	ON_MESSAGE(EGN_SETFOCUS, OnSetfocusGrid)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKPSelectedOutputPg9 message handlers

BOOL CKPSelectedOutputPg9::OnKillActive() 
{
	if (baseCKPSelectedOutputPg9::OnKillActive())
	{
		// as last step remove all entries from m_glDoc;
		m_glDoc.DeleteAllRows();
		return TRUE;
	}
	return FALSE;
}

void CKPSelectedOutputPg9::OnItemchangedClSolid(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_glDoc.OnItemChanged(pNMHDR, pResult);
}

void CKPSelectedOutputPg9::OnSetfocusClSolid(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNUSED(pNMHDR);

	CString strRes;
	strRes.LoadString(IDS_STRING396);
	m_eInputDesc.SetWindowText(strRes);

	// set default selection
	// this gives user feedback when the list recieves the focus
	if (m_clcSolid.GetItemCount() && m_clcSolid.GetFirstSelectedItemPosition() == NULL)
	{
		m_clcSolid.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}
	
	*pResult = 0;
}

BEGIN_EVENTSINK_MAP(CKPSelectedOutputPg9, baseCKPSelectedOutputPg9)
    //{{AFX_EVENTSINK_MAP(CKPSelectedOutputPg9)
	ON_EVENT(CKPSelectedOutputPg9, IDC_MSHFG_SOLID, -602 /* KeyDown */, OnKeyDownMshfgSolid, VTS_PI2 VTS_I2)
	ON_EVENT(CKPSelectedOutputPg9, IDC_MSHFG_SOLID, 71 /* EnterCell */, OnEnterCellMshfgSolid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CKPSelectedOutputPg9::OnKeyDownMshfgSolid(short FAR* KeyCode, short Shift) 
{
	m_glDoc.OnKeyDown(KeyCode, Shift);	
}

BOOL CKPSelectedOutputPg9::OnInitDialog() 
{
	baseCKPSelectedOutputPg9::OnInitDialog();
	
	// set layout
	CreateRoot(VERTICAL, 5, 6) 
		<< item(IDC_S_DEFINED, NORESIZE)
		<< (pane(VERTICAL, GREEDY, 0, 0, 0)
			<< (pane(HORIZONTAL, RELATIVE_VERT, 20, 0, 80)
				<< item(IDC_CL_SOLID, GREEDY)
				<< item(IDC_MSHFG_SOLID, ABSOLUTE_HORZ)
				)
			<< itemFixed(VERTICAL, 7)
			<< (paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, RELATIVE_VERT, nDefaultBorder, 10, 10, 20)
				<< item(IDC_DESCRIPTION, GREEDY)
				)
			)
	;
	UpdateLayout();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CKPSelectedOutputPg9::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bDenyEdit = FALSE;	// allow edit	

	switch (nID)
	{
	case IDC_MSHFG_SOLID :
		switch ( pInfo->item.iCol )
		{
		case 1 :
			pInfo->item.bUseCombo = (CUtil::InsertSolidSolutions(pInfo->item.hWndCombo, GetDatabase()) > 0);
			break;
		default :
			break;
		}
		break;
	}

	return bDenyEdit;
}

LRESULT CKPSelectedOutputPg9::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
{
	return m_glDoc.OnEndCellEdit(wParam, lParam);
}

LRESULT CKPSelectedOutputPg9::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
{
	UINT nID = wParam;
	UNUSED_ALWAYS(lParam);
	switch (nID)
	{
	case IDC_MSHFG_SOLID:
		OnEnterCellMshfgSolid();
		break;
	}
	return 0;
}

void CKPSelectedOutputPg9::OnEnterCellMshfgSolid() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING520);
	m_eInputDesc.SetWindowText(strRes);	
}

BOOL CKPSelectedOutputPg9::OnHelpInfo(HELPINFO* pHelpInfo) 
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
	case IDC_CL_SOLID: case IDC_S_DEFINED:
		strRes.LoadString(IDS_STRING396);
		break;
	case IDC_MSHFG_SOLID:
		strRes.LoadString(IDS_STRING520);
		break;
	default:
		// No help topic is associated with this item. 
		strRes.LoadString(IDS_STRING441);
		break;
	}
	myPopup.pszText = strRes;
	return ::HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}
