// CustomizePage1.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "CustomizePage1.h"

#include "phreeqci2.h"
#include <Htmlhelp.h>
#include "SaveCurrentDirectory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CCustomizePage1, baseCustomizePage1)
IMPLEMENT_DYNCREATE(CCustomizePage2, baseCustomizePage2)


/////////////////////////////////////////////////////////////////////////////
// CCustomizePage1 property page

CCustomizePage1::CCustomizePage1() : baseCustomizePage1(CCustomizePage1::IDD)
{
	//{{AFX_DATA_INIT(CCustomizePage1)
	m_strDefaultDatabase = _T("");
	m_bApplyOpen = FALSE;
	m_bApplyOnlyDef = FALSE;
	//}}AFX_DATA_INIT
}

CCustomizePage1::~CCustomizePage1()
{
}

void CCustomizePage1::DoDataExchange(CDataExchange* pDX)
{
	baseCustomizePage1::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCustomizePage1)
	DDX_Control(pDX, IDC_CB_DB, m_cboDatabase);
	DDX_Check(pDX, IDC_B_APPLY_OPEN, m_bApplyOpen);
	DDX_Check(pDX, IDC_APPLY_ONLY_DEF, m_bApplyOnlyDef);
	//}}AFX_DATA_MAP
	if (pDX->m_bSaveAndValidate)
	{
		DDX_CBString(pDX, IDC_CB_DB, m_strDefaultDatabase);
		m_strDefaultDatabase.TrimLeft();
		m_strDefaultDatabase.TrimRight();
		if (m_strDefaultDatabase.IsEmpty())
		{
			::AfxMessageBox("Invalid default database.");
			pDX->Fail();
		}

		if (::GetFileAttributes(m_strDefaultDatabase) == -1)
		{
			DWORD dw = GetLastError();
			ASSERT(
				dw == ERROR_FILE_NOT_FOUND ||
				dw == ERROR_PATH_NOT_FOUND ||
				dw == ERROR_INVALID_DRIVE  ||
				dw == ERROR_BAD_NETPATH    ||
				dw == ERROR_BAD_NET_NAME   ||
				dw == ERROR_INVALID_NAME
				);
			// let system display error message
			LPVOID lpMsgBuf;
			::FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				FORMAT_MESSAGE_FROM_SYSTEM | 
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				dw,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
				(LPTSTR) &lpMsgBuf,
				0,
				NULL
				);
			::AfxMessageBox((LPCTSTR)lpMsgBuf);
			LocalFree(lpMsgBuf);
			pDX->Fail();
		}
		else
		{
			// GetStatus cleans up the file name
			// ie e:\\path\file.ext  gets  e:\path\file.ext

			CFileStatus status;
			VERIFY(CFile::GetStatus(m_strDefaultDatabase, status));
			m_strDefaultDatabase = status.m_szFullName;
		}
	}
	else
	{
		if (m_bFirstSetActive)
		{
			// fill database combo
			m_cboDatabase.ResetContent();
			CRecentFileList* pList = ((CPhreeqciApp*)AfxGetApp())->m_pRecentDBFileList;
			for (int i = 0; i < pList->GetSize(); ++i)
			{
				if ( !((*pList)[i].IsEmpty()) )
					m_cboDatabase.AddString((*pList)[i]);
			}
		}
		DDX_CBString(pDX, IDC_CB_DB, m_strDefaultDatabase);
		UpdateButtons();
	}
}


BEGIN_MESSAGE_MAP(CCustomizePage1, baseCustomizePage1)
	//{{AFX_MSG_MAP(CCustomizePage1)
	ON_BN_CLICKED(IDC_B_DB, OnBrowseDb)
	ON_CBN_EDITCHANGE(IDC_CB_DB, OnEditchangeCbDb)
	ON_CBN_SELCHANGE(IDC_CB_DB, OnSelchangeCbDb)
	ON_CBN_EDITUPDATE(IDC_CB_DB, OnEditupdateCbDb)
	ON_WM_HELPINFO()
	ON_BN_CLICKED(IDC_B_APPLY_OPEN, OnBApplyOpen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCustomizePage2 property page

CCustomizePage2::CCustomizePage2() : baseCustomizePage2(CCustomizePage2::IDD)
{
	//{{AFX_DATA_INIT(CCustomizePage2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCustomizePage2::~CCustomizePage2()
{
}

void CCustomizePage2::DoDataExchange(CDataExchange* pDX)
{
	baseCustomizePage2::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCustomizePage2)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCustomizePage2, baseCustomizePage2)
	//{{AFX_MSG_MAP(CCustomizePage2)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



void CCustomizePage1::OnBrowseDb() 
{
	CSaveCurrentDirectory scd;

	CDataExchange dx(this, TRUE);
	DDX_CBString(&dx, IDC_CB_DB, m_strDefaultDatabase);

	// Show file Dialog box
	CFileDialog dlg(
		TRUE,					// bOpenFileDialog
		NULL,					// lpszDefExt
		m_strDefaultDatabase,
		OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, 
		_T("Database Files (*.dat)|*.dat|All Files (*.*)|*.*||")
		);

	// set dialog caption
	dlg.m_ofn.lpstrTitle = _T("Select a Database");
	
	if (dlg.DoModal() == IDOK)
	{
		m_strDefaultDatabase = dlg.GetPathName();
		UpdateData(FALSE);		// set window
	}	
}

void CCustomizePage1::OnEditchangeCbDb() 
{
	// save but don't validate m_strDefaultDatabase
	CComboBox* pBox = (CComboBox*)GetDlgItem(IDC_CB_DB);
	pBox->GetWindowText(m_strDefaultDatabase);
	UpdateButtons();
}

void CCustomizePage1::OnSelchangeCbDb() 
{
	// save but don't validate m_strDefaultDatabase
	CComboBox* pBox = (CComboBox*)GetDlgItem(IDC_CB_DB);
	int index = pBox->GetCurSel();
	ASSERT(index != CB_ERR);
	if (index != CB_ERR)
	{
		pBox->GetLBText(index, m_strDefaultDatabase);
	}
	UpdateButtons();
}

void CCustomizePage1::OnEditupdateCbDb() 
{
	TRACE("OnEditupdateCbDb\n");
}

BOOL CCustomizePage1::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// Add your message handler code here and/or call default

    if (pHelpInfo->iContextType == HELPINFO_WINDOW)
    {
		HH_POPUP myPopup;
		memset(&myPopup, 0, sizeof(HH_POPUP));
		myPopup.cbStruct = sizeof(HH_POPUP);
		// use default colors
		myPopup.clrForeground = (COLORREF)-1;
		myPopup.clrBackground = (COLORREF)-1;
		myPopup.pt.x = pHelpInfo->MousePos.x;
		myPopup.pt.y = pHelpInfo->MousePos.y;
		myPopup.rcMargins.top    = -1;
		myPopup.rcMargins.bottom = -1;
		myPopup.rcMargins.left   = -1;
		myPopup.rcMargins.right  = -1;
		myPopup.idString =  0;

		switch (pHelpInfo->iCtrlId)
		{
		case IDC_B_APPLY_OPEN :
			myPopup.pszText = "When selected, all files currently open will be set to use the default database.";
			::HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup);
			break;
		case IDC_APPLY_ONLY_DEF :
			myPopup.pszText = "When selected, all files currently open will be set to use the default database only if it currently uses the previous default database.";
			::HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup);
			break;
		case IDC_CB_DB :
			myPopup.pszText = "Enter the database file to be used for new files and files that do not have an associated database.";
			::HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup);
			break;
		case IDC_B_DB :
			myPopup.pszText = "Click to browse through folders to find the file you want.";
			::HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup);
			break;
		}
		return TRUE;
    }

	
	return baseCustomizePage1::OnHelpInfo(pHelpInfo);
}

BOOL CCustomizePage1::OnInitDialog() 
{
	baseCustomizePage1::OnInitDialog();
	
	// Add extra initialization here
	CreateRoot(VERTICAL, 5, 6)
		<< (paneCtrl(IDC_GB_DATABASE, VERTICAL, GREEDY, nDefaultBorder, 16, 15, 40)
			<< (pane(HORIZONTAL, GREEDY)
				<< item(IDC_ST_3, NORESIZE)
				<< item(IDC_CB_DB, ABSOLUTE_VERT)
				<< item(IDC_B_DB, NORESIZE)
				)
			<< item(IDC_B_APPLY_OPEN, NORESIZE)
			<< item(IDC_APPLY_ONLY_DEF, NORESIZE)
			)
		;
	UpdateLayout();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCustomizePage1::OnBApplyOpen() 
{
	UpdateButtons();
}

void CCustomizePage1::UpdateButtons()
{
	CWnd* pWndApplyOpen = GetDlgItem(IDC_B_APPLY_OPEN);
	CWnd* pWndApplyOnlyDef = GetDlgItem(IDC_APPLY_ONLY_DEF);

	CString strOrigDB = ((CPhreeqciApp*)AfxGetApp())->m_settings.m_strDefDBPathName;
	if (m_strDefaultDatabase.IsEmpty() || strOrigDB.CompareNoCase(m_strDefaultDatabase) == 0)
	{
		pWndApplyOpen->EnableWindow(FALSE);
		pWndApplyOnlyDef->EnableWindow(FALSE);
	}
	else
	{
		pWndApplyOpen->EnableWindow(TRUE);
		if (((CButton*)pWndApplyOpen)->GetCheck() == BST_CHECKED)
		{
			pWndApplyOnlyDef->EnableWindow(TRUE);
		}
		else
		{
			pWndApplyOnlyDef->EnableWindow(FALSE);
		}
	}
}
