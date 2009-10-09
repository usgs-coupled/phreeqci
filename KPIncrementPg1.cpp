// KPIncrementPg1.CPP : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "KPIncrementPg1.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CKPIncrementPg1, baseCKPIncrementPg1)


/////////////////////////////////////////////////////////////////////////////
// CKPIncrementPg1 property page

CKPIncrementPg1::CKPIncrementPg1() : baseCKPIncrementPg1(CKPIncrementPg1::IDD)
{
	//{{AFX_DATA_INIT(CKPIncrementPg1)
	m_bTrueOrFalse = TRUE;
	//}}AFX_DATA_INIT
}

CKPIncrementPg1::~CKPIncrementPg1()
{
}

void CKPIncrementPg1::DoDataExchange(CDataExchange* pDX)
{
	baseCKPIncrementPg1::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKPIncrementPg1)
	DDX_Control(pDX, IDC_E_DESC_INPUT, m_eDesc);
	DDX_Check(pDX, IDC_TRUE_OR_FALSE, m_bTrueOrFalse);
	//}}AFX_DATA_MAP

	if (m_bFirstSetActive && !pDX->m_bSaveAndValidate)
	{
		CString strRes;
		strRes.LoadString(IDS_INCREMENTAL_357);
		m_eDesc.SetWindowText(strRes);
	}
}

BEGIN_MESSAGE_MAP(CKPIncrementPg1, baseCKPIncrementPg1)
	//{{AFX_MSG_MAP(CKPIncrementPg1)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CKPIncrementPg1::OnInitDialog() 
{
	baseCKPIncrementPg1::OnInitDialog();

	// Add extra initialization here
	// set layout
	CreateRoot(VERTICAL, 5, 6)
		<< item(IDC_TRUE_OR_FALSE, ABSOLUTE_VERT)

		<< ( paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, GREEDY, nDefaultBorder, 10, 10)
			<< item(IDC_E_DESC_INPUT, GREEDY)
		);
	UpdateLayout();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CKPIncrementPg1::OnHelpInfo(HELPINFO* pHelpInfo) 
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
	case IDC_TRUE_OR_FALSE:
		strRes.LoadString(IDS_INCREMENTAL_357);
		break;
	default:
		// No help topic is associated with this item. 
		strRes.LoadString(IDS_STRING441);
		break;
	}
	myPopup.pszText = strRes;
	return ::HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}
