// KPTitlePg1.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "KPTitlePg1.h"

#include "Keyword.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CKPTitlePg1, baseKPTitlePg1)


/////////////////////////////////////////////////////////////////////////////
// CKPTitlePg1 property page

CKPTitlePg1::CKPTitlePg1() : baseKPTitlePg1(CKPTitlePg1::IDD)
{
	//{{AFX_DATA_INIT(CKPTitlePg1)
	m_strTitle = _T("<Type title here>");
	//}}AFX_DATA_INIT
}

CKPTitlePg1::~CKPTitlePg1()
{
}

void CKPTitlePg1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CKPTitlePg1)
	DDX_Control(pDX, IDC_EDIT1, m_eTitle);
	//}}AFX_DATA_MAP
	DDX_Title(pDX);
}


BEGIN_MESSAGE_MAP(CKPTitlePg1, baseKPTitlePg1)
	//{{AFX_MSG_MAP(CKPTitlePg1)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//
// Validates and exchanges m_strTitle
//
void CKPTitlePg1::DDX_Title(CDataExchange *pDX)
{
	CString strNewLine(_T("\r\n"));
	if (pDX->m_bSaveAndValidate)	// get and format data from control
	{
		// Quotes keywords that begin on a new line
		// Adds spaces to beginning of new lines

		int nLines = m_eTitle.GetLineCount();

		// do nothing if only one line
		if (nLines == 1)
		{
			m_eTitle.GetWindowText(m_strTitle);
			return;
		}

		CString str;
		int nAllocLen = 128;
		LPTSTR lpsz;
		CString strIndent(_T("      "));
		CString strSpace(_T("\x009\x00A\x00B\x00C\x00D\x020")); // see isspace
		CString strQuote(_T("\""));
		for (int i = 0; i < nLines; ++i)
		{
			while (true)
			{
				lpsz = str.GetBuffer(nAllocLen);
				int nLen = m_eTitle.GetLine(i, lpsz, nAllocLen - 1);
				if (nLen < nAllocLen - 1)
				{
					lpsz[nLen] = NULL;
					str.ReleaseBuffer();
					break;
				}
				nAllocLen *= 2;
			}
			str.TrimLeft(strSpace);
			str.TrimRight(strSpace);

			// do for first line only
			if (i == 0)
			{
				m_strTitle = str + strNewLine;
				continue;
			}

			// do only for lines after first
			// check for keyword at beginning of line
			CString strFirstToken = str.SpanExcluding(strSpace);
			if (!strFirstToken.IsEmpty() && CKeyword::IsKeyword(strFirstToken))
			{
				// quote keyword
				str = strQuote + strFirstToken + strQuote + str.Mid(strFirstToken.GetLength());
			}
			// indent each line after first
			str = strIndent + str + strNewLine;
			m_strTitle += str;
		}
		m_strTitle.TrimRight(strSpace);
	}
	else		// load data into control
	{
		// remove extraneous space from the begining of each line after first
		CString strTitle = m_strTitle;
		CString strTemp;
		CString strText;
		while (!strTitle.IsEmpty())
		{
			strTemp = strTitle.SpanExcluding(strNewLine);
			strTitle = strTitle.Mid(strTemp.GetLength());
			strTitle.TrimLeft();
			strText += strTemp + strNewLine;
		}
		strText.TrimRight();
		m_eTitle.SetWindowText(strText);
	}
}

BOOL CKPTitlePg1::OnInitDialog() 
{
	baseKPTitlePg1::OnInitDialog();
	
	//{{Set Layout
	CreateRoot(VERTICAL, 5, 6) << item(IDC_EDIT1);
	UpdateLayout();
	//}}Set Layout
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CKPTitlePg1::OnHelpInfo(HELPINFO* pHelpInfo) 
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
	case IDC_EDIT1:
		strRes.LoadString(IDS_STRING511);
		break;
	default:
		// No help topic is associated with this item. 
		strRes.LoadString(IDS_STRING441);
		break;
	}
	myPopup.pszText = strRes;
	return HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}
