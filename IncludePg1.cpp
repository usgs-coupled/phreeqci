// IncludePg1.cpp : implementation file
//

#include "stdafx.h"
#include "phreeqci2.h"
#include "IncludePg1.h"

#include "SaveCurrentDirectory.h"

// CIncludePg1 dialog

IMPLEMENT_DYNAMIC(CIncludePg1, baseCIncludePg1)

CIncludePg1::CIncludePg1()
	: baseCIncludePg1(CIncludePg1::IDD)
{

}

CIncludePg1::~CIncludePg1()
{
}

void CIncludePg1::DoDataExchange(CDataExchange* pDX)
{
	baseCIncludePg1::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DESCRIPTION, m_eInputDesc);
	DDX_Text(pDX, IDC_INC_FILE_NAME, m_strFileName);
}

BEGIN_MESSAGE_MAP(CIncludePg1, baseCIncludePg1)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_EN_SETFOCUS(IDC_INC_FILE_NAME, OnSetfocusFileName)
	ON_WM_HELPINFO()
	ON_BN_SETFOCUS(IDC_BROWSE, OnSetfocusBrowse)
END_MESSAGE_MAP()

BOOL CIncludePg1::OnInitDialog() 
{
	baseCIncludePg1::OnInitDialog();
	
	// Add extra initialization here

	// set layout
	CreateRoot(VERTICAL, 5, 6) 
		<< (pane(HORIZONTAL, ABSOLUTE_VERT)
			<< itemFixed(HORIZONTAL, 4)
			<< item(IDC_S_FILENAME, NORESIZE | ALIGN_CENTER)
			<< item(IDC_FILE_NAME, ABSOLUTE_VERT | ALIGN_CENTER)
			<< item(IDC_BROWSE, NORESIZE)
			)

		<< (paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, GREEDY, nDefaultBorder, 10, 10)
			<< item(IDC_DESCRIPTION, GREEDY)
			)
	;
	UpdateLayout();

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// CIncludePg1 message handlers
void CIncludePg1::OnSetfocusBrowse()
{
	// in order for a check box to recieve this notification
	// it must have the notify style set
	CString strRes;
	strRes.LoadString(IDS_BROWSE_STRING162);
	m_eInputDesc.SetWindowText(strRes);
}

void CIncludePg1::OnBrowse() 
{
	CSaveCurrentDirectory scd;

	// Show file Dialog box
	CFileDialog dlg(
		TRUE,					// bOpenFileDialog
		_T(""),                 // lpszDefExt
		m_strFileName,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST,
		_T("All Files (*.*)|*.*||")
		);
	if (dlg.DoModal() == IDOK)
	{
		m_strFileName = dlg.GetPathName();
		UpdateData(FALSE);
	}	
}

void CIncludePg1::OnSetfocusFileName() 
{
	CString strRes;
	strRes.LoadString(IDS_INC_FILE_NAME_129);
	m_eInputDesc.SetWindowText(strRes);	
}

BOOL CIncludePg1::OnHelpInfo(HELPINFO* pHelpInfo) 
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
	case IDC_BROWSE:
		strRes.LoadString(IDS_BROWSE_STRING162);
		break;
	case IDC_INC_FILE_NAME: case IDC_S_FILENAME:
		strRes.LoadString(IDS_INC_FILE_NAME_129);
		break;
	default:
		// No help topic is associated with this item. 
		strRes.LoadString(IDS_STRING441);
		break;
	}
	myPopup.pszText = strRes;
	return ::HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}
