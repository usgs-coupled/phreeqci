// KPSelectedOutputPg8.cpp : implementation file
//
// $Date: 2001/09/14 20:12:01 $
// $Revision: 1.1.1.8 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "KPSelectedOutputPg8.h"

#include "Util.h"
#include "DelayUpdate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKPSelectedOutputPg8 property page

IMPLEMENT_DYNCREATE(CKPSelectedOutputPg8, baseCKPSelectedOutputPg8)

CKPSelectedOutputPg8::CKPSelectedOutputPg8() : baseCKPSelectedOutputPg8(CKPSelectedOutputPg8::IDD)
, m_glDoc(&m_egKinetic, &m_clcKinetic, 1)
{
	//{{AFX_DATA_INIT(CKPSelectedOutputPg8)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CKPSelectedOutputPg8::~CKPSelectedOutputPg8()
{
}

void CKPSelectedOutputPg8::DoDataExchange(CDataExchange* pDX)
{
	baseCKPSelectedOutputPg8::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKPSelectedOutputPg8)
	DDX_Control(pDX, IDC_DESCRIPTION, m_eInputDesc);
	DDX_Control(pDX, IDC_CL_KINETIC, m_clcKinetic);
	DDX_Control(pDX, IDC_MSHFG_KINETIC, m_egKinetic);
	//}}AFX_DATA_MAP

	CWaitCursor wait;

	// reduce grid flicker
	CDelayUpdate update(this, IDC_MSHFG_KINETIC);

	if (m_bFirstSetActive)
	{
		CUtil::InsertRates(&m_clcKinetic, GetDatabase());
		m_egKinetic.AddRowHeaders();
		m_egKinetic.SetTextMatrix(0, 1, _T("Name"));
		m_egKinetic.SetColWidth(1, 0, 1400);

		// center align the column headers
		m_egKinetic.SetFillStyle(flexFillRepeat);
		m_egKinetic.SetRow(0);
		m_egKinetic.SetCol(0);
		m_egKinetic.SetColSel(m_egKinetic.GetCols(0) - 1);
		m_egKinetic.SetCellAlignment(flexAlignCenterCenter);
		m_egKinetic.SetFillStyle(flexFillSingle);


		// set bold titles
		m_egKinetic.SetRow(0), m_egKinetic.SetCol(1);
		m_egKinetic.SetCellFontBold(TRUE);

		// set initial cell
		m_egKinetic.SetRow(1), m_egKinetic.SetCol(1);
	}
	if (pDX->m_bSaveAndValidate)
	{
		std::list<CString> listKinetic;
		for (long nRow = m_egKinetic.GetFixedRows(); nRow < m_egKinetic.GetRows(); ++nRow)
		{
			CString strKinetic;

			DDX_GridText(pDX, IDC_MSHFG_KINETIC, nRow, 1, strKinetic);
			if (strKinetic.IsEmpty())
				continue;

			listKinetic.push_back(strKinetic);
		}


		// if here list is valid and can be assigned to the member variable
		m_listKinetic.assign(listKinetic.begin(), listKinetic.end());

	}
	else
	{
		// see OnKillActive
		ASSERT(m_glDoc.GetCount() == 0);

		std::list<CString>::iterator iter = m_listKinetic.begin();
		for (; iter != m_listKinetic.end(); ++iter)
		{
			m_glDoc.AddItem(*iter);
		}
		ASSERT(m_listKinetic.size() == m_glDoc.GetCount());
	}
}


BEGIN_MESSAGE_MAP(CKPSelectedOutputPg8, baseCKPSelectedOutputPg8)
	//{{AFX_MSG_MAP(CKPSelectedOutputPg8)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CL_KINETIC, OnItemchangedClKinetic)
	ON_NOTIFY(NM_SETFOCUS, IDC_CL_KINETIC, OnSetfocusClKinetic)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
	ON_MESSAGE(EGN_BEGINCELLEDIT, OnBeginCellEdit)
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
	ON_MESSAGE(EGN_SETFOCUS, OnSetfocusGrid)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKPSelectedOutputPg8 message handlers

BOOL CKPSelectedOutputPg8::OnInitDialog() 
{
	baseCKPSelectedOutputPg8::OnInitDialog();
	
	// set layout
	CreateRoot(VERTICAL, 5, 6) 
		<< item(IDC_S_DEFINED, NORESIZE)
		<< (pane(VERTICAL, GREEDY, 0, 0, 0)
			<< (pane(HORIZONTAL, RELATIVE_VERT, 20, 0, 80)
				<< item(IDC_CL_KINETIC, GREEDY)
				<< item(IDC_MSHFG_KINETIC, ABSOLUTE_HORZ)
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

BOOL CKPSelectedOutputPg8::OnKillActive() 
{
	if (baseCKPSelectedOutputPg8::OnKillActive())
	{
		// as last step remove all entries from m_glDoc;
		m_glDoc.DeleteAllRows();
		return TRUE;
	}
	return FALSE;
}

void CKPSelectedOutputPg8::OnItemchangedClKinetic(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_glDoc.OnItemChanged(pNMHDR, pResult);
}

void CKPSelectedOutputPg8::OnSetfocusClKinetic(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNUSED(pNMHDR);

	CString strRes;
	strRes.LoadString(IDS_STRING395);
	m_eInputDesc.SetWindowText(strRes);

	// set default selection
	// this gives user feedback when the list recieves the focus
	if (m_clcKinetic.GetItemCount() && m_clcKinetic.GetFirstSelectedItemPosition() == NULL)
	{
		m_clcKinetic.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}
	
	*pResult = 0;
}

BEGIN_EVENTSINK_MAP(CKPSelectedOutputPg8, baseCKPSelectedOutputPg8)
    //{{AFX_EVENTSINK_MAP(CKPSelectedOutputPg8)
	ON_EVENT(CKPSelectedOutputPg8, IDC_MSHFG_KINETIC, -602 /* KeyDown */, OnKeyDownMshfgKinetic, VTS_PI2 VTS_I2)
	ON_EVENT(CKPSelectedOutputPg8, IDC_MSHFG_KINETIC, 71 /* EnterCell */, OnEnterCellMshfgKinetic, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CKPSelectedOutputPg8::OnKeyDownMshfgKinetic(short FAR* KeyCode, short Shift) 
{
	m_glDoc.OnKeyDown(KeyCode, Shift);	
}

LRESULT CKPSelectedOutputPg8::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bDenyEdit = FALSE;	// allow edit	

	switch (nID)
	{
	case IDC_MSHFG_KINETIC :
		switch ( pInfo->item.iCol )
		{
		case 1 :
			pInfo->item.bUseCombo = (CUtil::InsertRates(pInfo->item.hWndCombo, GetDatabase()) > 0);
			break;
		default :
			break;
		}
		break;
	}

	return bDenyEdit;
}

LRESULT CKPSelectedOutputPg8::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
{
	return m_glDoc.OnEndCellEdit(wParam, lParam);
}

LRESULT CKPSelectedOutputPg8::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
{
	UINT nID = wParam;
	UNUSED_ALWAYS(lParam);
	switch (nID)
	{
	case IDC_MSHFG_KINETIC:
		OnEnterCellMshfgKinetic();
		break;
	}
	return 0;
}

void CKPSelectedOutputPg8::OnEnterCellMshfgKinetic() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING519);
	m_eInputDesc.SetWindowText(strRes);	
}

BOOL CKPSelectedOutputPg8::OnHelpInfo(HELPINFO* pHelpInfo) 
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
	case IDC_CL_KINETIC: case IDC_S_DEFINED:
		strRes.LoadString(IDS_STRING395);
		break;
	case IDC_MSHFG_KINETIC:
		strRes.LoadString(IDS_STRING519);
		break;
	default:
		// No help topic is associated with this item. 
		strRes.LoadString(IDS_STRING441);
		break;
	}
	myPopup.pszText = strRes;
	return HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}
