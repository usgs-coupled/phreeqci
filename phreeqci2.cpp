// phreeqci2.cpp : Defines the class behaviors for the application.
//
// $Date: 2004/11/03 06:56:14 $
// $Revision: 1.6 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"

#include "MainFrame.h"
#include "ChildFrame.h"
#include "RichDocIn.h"
#include "RichViewIn.h"

#include <Htmlhelp.h>

#include "RichDocOut.h"
#include "RichViewOut.h"

#include "MyRecentFileList.h"

#include "MultiDocTemplateIn.h"
#include "MultiDocTemplateOut.h"

#include "CustomizeSheet.h"

#include <io.h>
#include "Splash.h"

#include <../Mfc/Src/AfxImpl.h>	// for AfxFullPath & AfxResolveShortcut & _countof

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPhreeqciApp

BEGIN_MESSAGE_MAP(CPhreeqciApp, CWinApp)
	ON_COMMAND(CG_IDS_TIPOFTHEDAY, ShowTipOfTheDay)
	ON_COMMAND(CG_IDS_TIPOFTHEDAY_FIX, ShowTipOfTheDayFix)
	//{{AFX_MSG_MAP(CPhreeqciApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_UPDATE_COMMAND_UI(ID_FILE_MRU_INPUT1, OnUpdateRecentInFileMenu)
	ON_UPDATE_COMMAND_UI(ID_FILE_MRU_OUTPUT1, OnUpdateRecentOutFileMenu)
	ON_COMMAND(ID_OPTIONS_SETDEFAULTDATABASE, OnOptionsSetdefaultdatabase)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	//ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
	// MRU
	ON_COMMAND_EX_RANGE(ID_FILE_MRU_INPUT1, ID_FILE_MRU_INPUT16, OnOpenRecentInFile)
	ON_COMMAND_EX_RANGE(ID_FILE_MRU_OUTPUT1, ID_FILE_MRU_OUTPUT16, OnOpenRecentOutFile)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPhreeqciApp construction

CPhreeqciApp::CPhreeqciApp()
: CWinApp()
{
	// TODO: add construction code here,
	m_pRecentInFileList = NULL;
	m_pRecentOutFileList = NULL;
	// Place all significant initialization in InitInstance

	// Note: Can't use CString::LoadString here
	ASSERT(m_pszRegistryKey == NULL);
	m_pszRegistryKey = _tcsdup(_T("USGS"));

	ASSERT(m_pszProfileName == NULL);
	m_pszProfileName = _tcsdup(_T("PHREEQC Interactive\\2.11"));

	//
	// use to find memory leaks
	//
	// _CrtSetBreakAlloc(587);
	//
	// ie
	//
	// {587} normal block at 0x009FA470, 8 bytes long.
	// Data: <        > CD CD CD CD CD CD CD CD 
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CPhreeqciApp object

CPhreeqciApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CPhreeqciApp initialization

BOOL CPhreeqciApp::InitInstance()
{
	// CG: The following block was added by the Splash Screen component.
	{
		CCommandLineInfo cmdInfo;
		ParseCommandLine(cmdInfo);
		////CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);
		CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash && cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew);
	}
#ifdef _DEBUG
	// afxMemDF |= checkAlwaysMemDF;
	///{{

	///}}
#endif

	/*******************************************************
	if (!AfxSocketInit())
	{
		::AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}
	*******************************************************/

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		::AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// Set in Ctor SetRegistryKey(IDS_ORGANIZATION);

	LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)
	if (!LoadMoreProfileSettings(8))
	{
		return FALSE;
	}

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CMultiDocTemplate* pDocTemplate;
	// create input file template
	pDocTemplate = new CMultiDocTemplateIn(
		IDR_INPUT_TYPE,
		RUNTIME_CLASS(CRichDocIn),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CRichViewIn));
	pDocTemplate->SetContainerInfo(IDR_PHRQCTYPE_CNTR_IP);
	AddDocTemplate(pDocTemplate);

	// create output file template
	pDocTemplate = new CMultiDocTemplateOut(
		IDR_OUTPUT_TYPE,
		RUNTIME_CLASS(CRichDocOut),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CRichViewOut));
	pDocTemplate->SetContainerInfo(IDR_PHRQCTYPE_CNTR_IP);
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;


	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Remove Shell New for Phreeqci Output File
	UnRegisterShellNew(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CPhreeqciApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CPhreeqciApp message handlers


void CPhreeqciApp::OnFileNew() 
{
	// Add your command handler code here
	VERIFY( OpenNewDocument(_T("Phreeqci.Input")) );	
}

BOOL CPhreeqciApp::OpenNewDocument(const CString &strTarget)
{
	CString strDocName;
	CDocTemplate* pSelectedTemplate;
	POSITION pos = GetFirstDocTemplatePosition();
	while (pos != NULL)
	{
		pSelectedTemplate = (CDocTemplate*) GetNextDocTemplate(pos);
		ASSERT( pSelectedTemplate != NULL );
		ASSERT( pSelectedTemplate->IsKindOf(RUNTIME_CLASS(CDocTemplate)) );
		pSelectedTemplate->GetDocString(strDocName, CDocTemplate::regFileTypeId);
		if (strDocName == strTarget)
		{
			pSelectedTemplate->OpenDocumentFile(NULL);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CPhreeqciApp::LoadMoreProfileSettings(UINT nMaxMRU)
{
	ASSERT_VALID(this);

	TCHAR szOutput[_MAX_PATH];
	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
	TCHAR fname[_MAX_FNAME];
	TCHAR ext[_MAX_EXT];
	TCHAR szBuff[_MAX_PATH];

	// Get exe path
	VERIFY(::GetModuleFileName(m_hInstance, szBuff, _MAX_PATH));
	m_settings.m_strExePath = szBuff;
	
	// Create default database
	_tsplitpath(szBuff, drive, dir, fname, ext);
	_tmakepath(szOutput, drive, dir, _T("phreeqc.dat"), NULL);

	// Get default database from registry
	m_settings.m_strDefDBPathName = GetProfileString(_T("Settings"), _T("Database"), szOutput);

	if (_taccess(m_settings.m_strDefDBPathName, 0) != -1)
	{
		// m_db2.Load(m_strDefaultDatabase);
	}
	else if (_taccess(szOutput, 0) != -1) 
	{
		CString warning;
		warning.Format(_T("WARNING: The default database \"%s\" cannot be found.")
			_T("  Resetting default database to \"%s\"."),
			m_settings.m_strDefDBPathName,
			szOutput);
		MessageBox(NULL, warning, _T("Missing default database"), MB_OK | MB_ICONWARNING);
		m_settings.m_strDefDBPathName = szOutput;
		// m_db2.Load(m_strDefaultDatabase);
	}
	else
	{
		CString error;
		error.Format(_T("ERROR: The default database \"%s\" cannot be found.")
			_T("  Please reinstall software."), m_settings.m_strDefDBPathName);
		MessageBox(NULL, error, _T("Missing default database"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	PreLoadDatabase(m_settings.m_strDefDBPathName);

	// Get default close run dialog on finish setting
	m_settings.m_bCloseOnFinish = GetProfileInt(_T("Settings"), _T("CloseOnFinish"), m_settings.m_bCloseOnFinish);
	m_settings.m_bWarnOverwriteInput = GetProfileInt(_T("Settings"), _T("WarnOverwriteInput"), m_settings.m_bWarnOverwriteInput);
	m_settings.m_bWarnOverwriteOutput = GetProfileInt(_T("Settings"), _T("WarnOverwriteOutput"), m_settings.m_bWarnOverwriteOutput);

	// Create and load MRUs
	ASSERT(m_pRecentInFileList == NULL);

	if (nMaxMRU != 0)
	{
		// create file MRU since nMaxMRU not zero
		m_pRecentInFileList = new CMyRecentFileList(0,
			_T("MRU Input"), _T("Input%d"),
			nMaxMRU);
		m_pRecentInFileList->ReadList();
	}

	ASSERT(m_pRecentOutFileList == NULL);

	if (nMaxMRU != 0)
	{
		// create file MRU since nMaxMRU not zero
		m_pRecentOutFileList = new CMyRecentFileList(0,
			_T("MRU Output"), _T("Output%d"),
			nMaxMRU);
		m_pRecentOutFileList->ReadList();
	}

	ASSERT(m_pRecentDBFileList == NULL);

	if (nMaxMRU != 0)
	{
		// create file MRU since nMaxMRU not zero
		m_pRecentDBFileList = new CMyRecentFileList(0,
			_T("MRU DB"), _T("DB%d"),
			nMaxMRU);
		m_pRecentDBFileList->ReadList();
	}
	m_pRecentDBFileList->Add(m_settings.m_strDefDBPathName);

	return TRUE;

}

void CPhreeqciApp::SaveSettings()
{
	// Save default database to registry
	WriteProfileString(_T("Settings"), _T("Database"), m_settings.m_strDefDBPathName);

	// Save default close run dialog on finish setting
	WriteProfileInt(_T("Settings"), _T("CloseOnFinish"), m_settings.m_bCloseOnFinish);
	WriteProfileInt(_T("Settings"), _T("WarnOverwriteInput"), m_settings.m_bWarnOverwriteInput);
	WriteProfileInt(_T("Settings"), _T("WarnOverwriteOutput"), m_settings.m_bWarnOverwriteOutput);

	// Save input MRU list
	if (m_pRecentInFileList != NULL)
		m_pRecentInFileList->WriteList();

	// Save output MRU list
	if (m_pRecentOutFileList != NULL)
		m_pRecentOutFileList->WriteList();

	// Save database MRU list
	if (m_pRecentDBFileList != NULL)
		m_pRecentDBFileList->WriteList();
}

void CPhreeqciApp::AddToRecentInFileList(LPCTSTR lpszPathName)
{
	ASSERT_VALID(this);
	ASSERT(lpszPathName != NULL);
	ASSERT(AfxIsValidString(lpszPathName));

	if (m_pRecentInFileList != NULL)
		m_pRecentInFileList->Add(lpszPathName);
}

void CPhreeqciApp::AddToRecentOutFileList(LPCTSTR lpszPathName)
{
	ASSERT_VALID(this);
	ASSERT(lpszPathName != NULL);
	ASSERT(AfxIsValidString(lpszPathName));

	if (m_pRecentOutFileList != NULL)
		m_pRecentOutFileList->Add(lpszPathName);
}

void CPhreeqciApp::AddToRecentDBFileList(LPCTSTR lpszPathName)
{
	ASSERT_VALID(this);
	ASSERT(lpszPathName != NULL);
	ASSERT(AfxIsValidString(lpszPathName));

	if (m_pRecentDBFileList != NULL)
		m_pRecentDBFileList->Add(lpszPathName);
}

void CPhreeqciApp::OnUpdateRecentInFileMenu(CCmdUI* pCmdUI) 
{
	ASSERT_VALID(this);
	if (m_pRecentInFileList == NULL) // no MRU files
		pCmdUI->Enable(FALSE);
	else
		m_pRecentInFileList->UpdateMenu(pCmdUI);
}

void CPhreeqciApp::OnUpdateRecentOutFileMenu(CCmdUI* pCmdUI) 
{
	ASSERT_VALID(this);
	if (m_pRecentOutFileList == NULL) // no MRU files
		pCmdUI->Enable(FALSE);
	else
		m_pRecentOutFileList->UpdateMenu(pCmdUI);
}

BOOL CPhreeqciApp::OnOpenRecentInFile(UINT nID) 
{
	ASSERT_VALID(this);
	ASSERT(m_pRecentInFileList != NULL);

	ASSERT(nID >= ID_FILE_MRU_INPUT1);
	ASSERT(nID < ID_FILE_MRU_INPUT1 + (UINT)m_pRecentInFileList->GetSize());
	int nIndex = nID - ID_FILE_MRU_INPUT1;
	ASSERT((*m_pRecentInFileList)[nIndex].GetLength() != 0);

	TRACE2("MRU: open file (%d) '%s'.\n", (nIndex) + 1,
			(LPCTSTR)(*m_pRecentInFileList)[nIndex]);

	//
	// Before doing default processing check if file still exists
	//

	//{{ MOD CDocManager::OpenDocumentFile
	TCHAR szFileName[_MAX_PATH];
	TCHAR szPath[_MAX_PATH];

	ASSERT(lstrlen((LPCTSTR)(*m_pRecentInFileList)[nIndex]) < _countof(szPath));

	lstrcpyn(szFileName, (LPCTSTR)(*m_pRecentInFileList)[nIndex], _MAX_PATH);

	LPCTSTR lpszFileName = szFileName;

	TCHAR szTemp[_MAX_PATH];
	if (lpszFileName[0] == '\"')
		++lpszFileName;
	lstrcpyn(szTemp, lpszFileName, _MAX_PATH);
	LPTSTR lpszLast = _tcsrchr(szTemp, '\"');
	if (lpszLast != NULL)
		*lpszLast = 0;
	AfxFullPath(szPath, szTemp);
	TCHAR szLinkName[_MAX_PATH];
	if (AfxResolveShortcut(AfxGetMainWnd(), szPath, szLinkName, _MAX_PATH))
		lstrcpy(szPath, szLinkName);
	//}} MOD CDocManager::OpenDocumentFile

	//{{
	if (!CUtil::FileExists(szPath))
	{
		CString strMsg;
		strMsg.Format(_T("%s was not found"), szPath);
		::AfxMessageBox(strMsg, MB_OK | MB_ICONWARNING);
		m_pRecentInFileList->Remove(nIndex);
		return TRUE;
	}
	//}}

	if (CWinApp::OpenDocumentFile((*m_pRecentInFileList)[nIndex]) == NULL)
		m_pRecentInFileList->Remove(nIndex);

	return TRUE;
}

BOOL CPhreeqciApp::OnOpenRecentOutFile(UINT nID) 
{
	ASSERT_VALID(this);
	ASSERT(m_pRecentOutFileList != NULL);

	ASSERT(nID >= ID_FILE_MRU_OUTPUT1);
	ASSERT(nID < ID_FILE_MRU_OUTPUT1 + (UINT)m_pRecentOutFileList->GetSize());
	int nIndex = nID - ID_FILE_MRU_OUTPUT1;
	ASSERT((*m_pRecentOutFileList)[nIndex].GetLength() != 0);

	TRACE2("MRU: open file (%d) '%s'.\n", (nIndex) + 1,
			(LPCTSTR)(*m_pRecentOutFileList)[nIndex]);
	//
	// Before doing default processing check if file still exists
	//

	//{{ MOD CDocManager::OpenDocumentFile
	TCHAR szFileName[_MAX_PATH];
	TCHAR szPath[_MAX_PATH];

	ASSERT(lstrlen((LPCTSTR)(*m_pRecentOutFileList)[nIndex]) < _countof(szPath));

	lstrcpyn(szFileName, (LPCTSTR)(*m_pRecentOutFileList)[nIndex], _MAX_PATH);

	LPCTSTR lpszFileName = szFileName;

	TCHAR szTemp[_MAX_PATH];
	if (lpszFileName[0] == '\"')
		++lpszFileName;
	lstrcpyn(szTemp, lpszFileName, _MAX_PATH);
	LPTSTR lpszLast = _tcsrchr(szTemp, '\"');
	if (lpszLast != NULL)
		*lpszLast = 0;
	AfxFullPath(szPath, szTemp);
	TCHAR szLinkName[_MAX_PATH];
	if (AfxResolveShortcut(AfxGetMainWnd(), szPath, szLinkName, _MAX_PATH))
		lstrcpy(szPath, szLinkName);
	//}} MOD CDocManager::OpenDocumentFile

	//{{
	if (!CUtil::FileExists(szPath))
	{
		CString strMsg;
		strMsg.Format(_T("%s was not found"), szPath);
		::AfxMessageBox(strMsg, MB_OK | MB_ICONWARNING);
		m_pRecentOutFileList->Remove(nIndex);
		return TRUE;
	}
	//}}

	if (CWinApp::OpenDocumentFile((*m_pRecentOutFileList)[nIndex]) == NULL)
		m_pRecentOutFileList->Remove(nIndex);

	return TRUE;
}

int CPhreeqciApp::ExitInstance() 
{
	// Add your specialized code here and/or call the base class
	SaveSettings();

	if (m_pRecentInFileList != NULL)
		delete m_pRecentInFileList;

	if (m_pRecentOutFileList != NULL)
		delete m_pRecentOutFileList;

	if (m_pRecentDBFileList != NULL)
		delete m_pRecentDBFileList;

	::OutputDebugString("PHREEQCI2:About to call CWinApp::ExitInstance\n");
	
	return CWinApp::ExitInstance();
}

CDocument* CPhreeqciApp::OpenDocumentFile(LPCTSTR lpszFileName, const CString &rFileTypeId)
{
	CString strDocName;
	CDocTemplate* pSelectedTemplate;
	POSITION pos = GetFirstDocTemplatePosition();
	while (pos != NULL)
	{
		pSelectedTemplate = (CDocTemplate*) GetNextDocTemplate(pos);
		ASSERT( pSelectedTemplate != NULL );
		ASSERT( pSelectedTemplate->IsKindOf(RUNTIME_CLASS(CDocTemplate)) );
		pSelectedTemplate->GetDocString(strDocName, CDocTemplate::regFileTypeId);
		if (strDocName == rFileTypeId)
		{
			return pSelectedTemplate->OpenDocumentFile(lpszFileName);
		}
	}
	ASSERT( FALSE );	// unsupported type
	return NULL;
}

const CDatabase& CPhreeqciApp::GetMergeDatabase(LPCTSTR lpszPathName)
{
	return PreLoadDatabase_(lpszPathName);
}

void CPhreeqciApp::PreLoadDatabase(LPCTSTR lpszPathName)
{
	PreLoadDatabase_(lpszPathName);
}

const CDatabase& CPhreeqciApp::PreLoadDatabase_(LPCTSTR lpszPathName)
{
	// convert all file names to lower case
	CString str(lpszPathName);
	str.MakeLower();
	
	// check if database is already loaded
	if (m_databaseMap.find(str) == m_databaseMap.end())
	{
		// database has not been loaded
		m_databaseMap[str].Load(str);

		// lock database access
		//{{
		VERIFY(m_databaseMap[str].DenyWrite());
		//}}
	}
	return m_databaseMap[str];
}

void CPhreeqciApp::WinHelp(DWORD dwData, UINT nCmd) 
{
	// CWinApp::WinHelp(dwData, nCmd);

	::OutputDebugString("PHREEQCI2:In CPhreeqciApp::WinHelp\n");
	switch (nCmd)
	{
	case HELP_CONTEXT: 
		VERIFY(::HtmlHelp(::GetDesktopWindow(), _T("phreeqci.chm"), HH_HELP_CONTEXT, dwData));
		break;
	case HELP_FINDER: 
		VERIFY(::HtmlHelp(::GetDesktopWindow(), _T("phreeqci.chm"), HH_DISPLAY_TOPIC, 0));
		break;	
	}

}

void CPhreeqciApp::ShowTipOfTheDay(void)
{
	// CG: This function added by 'Tip of the Day' component.

	CTipDlg dlg;
	dlg.DoModal();

}

BOOL CPhreeqciApp::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following lines were added by the Splash Screen component.
	if (CSplashWnd::PreTranslateAppMessage(pMsg))
		return TRUE;

	return CWinApp::PreTranslateMessage(pMsg);
}

void CPhreeqciApp::ShowTipOfTheDayFix(void)
{
	CTipDlg dlg;
	if (dlg.m_bStartup)
		dlg.DoModal();

}

void CPhreeqciApp::OnOptionsSetdefaultdatabase() 
{
	// TODO: Add your command handler code here
	CCustomizeSheet props;
	props.m_Page1.m_strDefaultDatabase = m_settings.m_strDefDBPathName;
	if (props.DoModal() == IDOK)
	{
		if (m_settings.m_strDefDBPathName.CompareNoCase(props.m_Page1.m_strDefaultDatabase) == 0)
		{
			// no changes do nothing
			return;
		}
		
		if (props.m_Page1.m_bApplyOpen)
		{
			POSITION pos = GetFirstDocTemplatePosition();
			CMultiDocTemplateIn* pTemplate = (CMultiDocTemplateIn*)GetNextDocTemplate(pos);
			ASSERT_KINDOF(CMultiDocTemplateIn, pTemplate);
			pos = pTemplate->GetFirstDocPosition();

			// go through all documents
			pos = pTemplate->GetFirstDocPosition();
			while (pos != NULL)
			{
				CRichDocIn* pDoc = (CRichDocIn*)pTemplate->GetNextDoc(pos);
				ASSERT_KINDOF(CRichDocIn, pDoc);

				if (props.m_Page1.m_bApplyOnlyDef
					&&
					m_settings.m_strDefDBPathName.CompareNoCase(pDoc->m_props.m_strDBPathName) != 0)
				{
					// current database not default; do nothing
					continue;
				}

				if (props.m_Page1.m_strDefaultDatabase.CompareNoCase(pDoc->m_props.m_strDBPathName) != 0)
				{
					if (!pDoc->GetPathName().IsEmpty())
					{
						// only set if the file has been saved at least once
						pDoc->SetModifiedFlag(TRUE);
					}
				}
				pDoc->m_props.m_strDBPathName = props.m_Page1.m_strDefaultDatabase;
			}
		}

		m_settings.m_strDefDBPathName = props.m_Page1.m_strDefaultDatabase;
		PreLoadDatabase(props.m_Page1.m_strDefaultDatabase);

		// save to registry immediately
		SaveSettings();
	}
}

void CAboutDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting	

	CDC dcLogo;
	dcLogo.CreateCompatibleDC(NULL);
	
	CBitmap bmLogo;
	bmLogo.LoadBitmap(IDB_USGS_LOGO);

	CBitmap* pbmLogoOld = dcLogo.SelectObject(&bmLogo);

	// Get bitmap dims
	BITMAP bm;
	bmLogo.GetBitmap(&bm);

    // Create memory DC for image with inverted background (AND mask)
    // Create DC compatible with screen
	CDC dcImage;
	dcImage.CreateCompatibleDC(NULL);
	int nPlanes = dcImage.GetDeviceCaps(PLANES);
	int nBitcount = dcImage.GetDeviceCaps(BITSPIXEL);

	// Create color bitmap same as screen
	CBitmap bmImage;
	bmImage.CreateBitmap(bm.bmWidth, bm.bmHeight, nPlanes, nBitcount, NULL);

	CBitmap* pbmImageOld = dcImage.SelectObject(&bmImage);
	// Copy image (we'll invert its background later)
	dcImage.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &dcLogo, 0, 0, SRCCOPY);

    // Create DC for monochrome mask of image (XOR mask)
    // Create device context compatible with screen
	CDC dcMask;
	dcMask.CreateCompatibleDC(NULL);
	// Create bitmap (monochrome by default)
	CBitmap bmMask;
	bmMask.CreateCompatibleBitmap(&dcMask, bm.bmWidth, bm.bmHeight);
	CBitmap *pbmMaskOld = dcMask.SelectObject(&bmMask);
	// Copy color bitmap to monochrome DC to create mono mask
	dcMask.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &dcImage, 0, 0, SRCCOPY);

    // Invert background of image to create AND Mask
	dcImage.SetBkColor(RGB(0, 0, 0));
	dcImage.SetTextColor(RGB(255, 255, 255));
	dcImage.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &dcMask, 0, 0, SRCAND);

	// Create memory DCs for old background and cache
	CDC dcBack;
	dcBack.CreateCompatibleDC(NULL);
	CBitmap bmBack;
	bmBack.CreateBitmap(bm.bmWidth, bm.bmHeight, nPlanes, nBitcount, NULL);
	CBitmap* pbmBackOld = dcBack.SelectObject(&bmBack);
	dcBack.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &dcLogo, 0, 0, SRCCOPY);

	CDC dcCache;
	dcCache.CreateCompatibleDC(NULL);
	CBitmap bmCache;
	bmCache.CreateBitmap(bm.bmWidth, bm.bmHeight, nPlanes, nBitcount, NULL);
	CBitmap* pbmCacheOld = dcCache.SelectObject(&bmCache);
	dcCache.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &dcLogo, 0, 0, SRCCOPY);

	COLORREF crBack = dcCache.GetBkColor();
	COLORREF crFore = dcCache.GetTextColor();
	dcCache.SetBkColor(::GetSysColor(COLOR_3DFACE));
	dcCache.SetTextColor(RGB(0, 0, 0));

	// Mask the background
	dcCache.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &dcMask, 0, 0, SRCAND);

	// Put image in hole created by mask
	dcCache.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &dcImage, 0, 0, SRCPAINT);

	// Restore color
	dcCache.SetBkColor(crBack);
	dcCache.SetTextColor(crFore);

	// Put finished cache on screen
	dc.BitBlt(10, 10, bm.bmWidth + 10, bm.bmHeight + 10, &dcCache, 0, 0, SRCCOPY);

	// Delte all Objects and DCs that were created
	dcImage.SelectObject(pbmImageOld);
	dcMask.SelectObject(pbmMaskOld);
	dcBack.SelectObject(pbmBackOld);
	dcCache.SelectObject(pbmCacheOld);
	dcLogo.SelectObject(pbmLogoOld);

	// Do not call CDialog::OnPaint() for painting messages
}

// Modified from void CDocManager::UnregisterShellFileTypes()
AFX_STATIC_DATA const TCHAR _afxShellNewFmt[] = _T("%s\\ShellNew");
void CPhreeqciApp::UnRegisterShellNew(CMultiDocTemplate* pTemplate)
{
	ASSERT(pTemplate);
	if (pTemplate)
	{
		CString strTemp;
		CString strFilterExt, strFileTypeId, strFileTypeName;
		if (pTemplate->GetDocString(strFileTypeId,
		   CDocTemplate::regFileTypeId) && !strFileTypeId.IsEmpty())
		{
			pTemplate->GetDocString(strFilterExt, CDocTemplate::filterExt);
			if (!strFilterExt.IsEmpty())
			{
				ASSERT(strFilterExt[0] == '.');

				LONG lSize = _MAX_PATH * 2;
				LONG lResult = ::RegQueryValue(HKEY_CLASSES_ROOT, strFilterExt,
					strTemp.GetBuffer(lSize), &lSize);
				strTemp.ReleaseBuffer();

				if (lResult != ERROR_SUCCESS || strTemp.IsEmpty() ||
					strTemp == strFileTypeId)
				{
					strTemp.Format(_afxShellNewFmt, (LPCTSTR)strFilterExt);

					// try to open key
					HKEY hKey;
					if (::RegOpenKey(HKEY_CLASSES_ROOT, (LPCTSTR)strTemp, &hKey) != ERROR_SUCCESS)
						return;

					// close the key
					::RegCloseKey(hKey);

					// delete the key
					::RegDeleteKey(HKEY_CLASSES_ROOT, (LPCTSTR)strTemp);
				}
			}
		}
	}
}
