// RunDlg.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "RunDlg.h"

#include "Util.h"

extern "C" {
#include "phreeqc/src/input.h"
#include "phreeqc/src/output.h"
#include "phreeqc/src/phrqproto.h"	
#include "phreeqci_files.h"

	int output_message(const int type, const char *err_str, const int stop, const char *format, va_list args);


	/* C globals */
	struct h_status g_status;
	char g_szLineBuf[160];
	HANDLE g_hKill;
}

/* C++ globals */
CEvent g_eventKill;


extern CEvent g_eventKill;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRunDlg dialog


CRunDlg::CRunDlg(CWnd* pParent /*=NULL*/)
	: baseRunDlg(CRunDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRunDlg)
	m_bCloseOnFinish = FALSE;
	m_bSetAsDefault = FALSE;
	//}}AFX_DATA_INIT

	m_bFirstSetActive = TRUE;
	m_bErrors = false;
	m_strError = GetErrorFile();
	ASSERT( CUtil::IsFileWriteable(m_strError) );
}

CRunDlg::~CRunDlg()
{
	::input_error = 0;
	RemoveErrorFile();
	CDialog::~CDialog();
}

void CRunDlg::DoDataExchange(CDataExchange* pDX)
{
	baseRunDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRunDlg)
	DDX_Control(pDX, IDC_SET_AS_DEFAULT, m_btnSetAsDefault);
	DDX_Control(pDX, IDC_CBO_DATABASE, m_cboDatabase);
	DDX_Check(pDX, IDC_CHECK1, m_bCloseOnFinish);
	DDX_Check(pDX, IDC_SET_AS_DEFAULT, m_bSetAsDefault);
	//}}AFX_DATA_MAP

	// Input file name
	if (pDX->m_bSaveAndValidate)
	{
		DDX_Text(pDX, IDC_E_INPUT, m_strInput);
	}
	else
	{
		if (m_bFirstSetActive)
		{
			m_strInput = m_props.m_pDoc->GetPathName();
		}
		DDX_Text(pDX, IDC_E_INPUT, m_strInput);
	}


	// Output file name
	DDX_Text(pDX, IDC_E_OUTPUT, m_props.m_strOutPathName);

	// initialize sim and trans string
	CString str;
	DDX_Text(pDX, IDC_S_SIM, str);
	DDX_Text(pDX, IDC_S_TRANS, str);


	// database file name:
	if (pDX->m_bSaveAndValidate)
	{
		// save database
		ASSERT(m_cboDatabase.GetCurSel() != CB_ERR);
		m_cboDatabase.GetLBText(m_cboDatabase.GetCurSel(), m_props.m_strDBPathName);
		ASSERT(!m_props.m_strDBPathName.IsEmpty());
		ASSERT(CUtil::FileExists(m_props.m_strDBPathName));
		// validate database
		if (!CUtil::FileExists(m_props.m_strDBPathName))
		{
			CString strMsg;
			pDX->PrepareCtrl(IDC_CBO_DATABASE);
			strMsg.Format(_T("%s was not found"), m_props.m_strDBPathName);
			MessageBox(strMsg, _T("Missing File"), MB_OK|MB_ICONSTOP);
			pDX->Fail();
		}
	}
	else
	{
		if (m_bFirstSetActive)
		{
			// fill database combo
			m_cboDatabase.ResetContent();
			CRecentFileList* pList = ((CPhreeqciApp*)AfxGetApp())->m_pRecentDBFileList;
			CString strPathName;
			for (int i = 0; i < pList->GetSize(); ++i)
			{
				strPathName = (*pList)[i];
				if ( !(strPathName.IsEmpty()) && CUtil::FileExists(strPathName) )
					m_cboDatabase.AddString(strPathName);
			}

			// set Set as default checkbox
			UpdateSetAsDefault();
		}

		// set database combo
		ASSERT( !m_props.m_strDBPathName.IsEmpty() );
		int nFind = m_cboDatabase.FindString(-1, m_props.m_strDBPathName);
		if (nFind == CB_ERR)
		{
			// database not in combo so add to list
			CRecentFileList* pList = ((CPhreeqciApp*)AfxGetApp())->m_pRecentDBFileList;
			nFind = m_cboDatabase.AddString(m_props.m_strDBPathName);
			pList->Add(m_props.m_strDBPathName);
		}
		m_cboDatabase.SetCurSel(nFind);
	}
	//}}
}


BEGIN_MESSAGE_MAP(CRunDlg, baseRunDlg)
	//{{AFX_MSG_MAP(CRunDlg)
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_BROWSE_OUTPUT, OnBrowseOutput)
	ON_BN_CLICKED(IDC_BROWSE_DATABASE, OnBrowseDatabase)
	ON_BN_CLICKED(IDC_BROWSE_INPUT, OnBrowseInput)
	ON_CBN_SELCHANGE(IDC_CBO_DATABASE, OnSelchangeCboDatabase)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_THREADFINISHED, OnThreadFinished)
	ON_MESSAGE(WM_THREADCANCELED, OnThreadCanceled)
	ON_MESSAGE(WM_THREADERRORS, OnThreadErrors)
	ON_MESSAGE(WM_THREADHARDWARE_EXCEPTION, OnThreadHardware)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRunDlg message handlers

BOOL CRunDlg::OnInitDialog() 
{
	baseRunDlg::OnInitDialog();
	
	// Add extra initialization here
	CreateLayout();

	m_bFirstSetActive = FALSE;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRunDlg::CreateLayout()
{
	// save initial sizes of anything that is resizable
	SaveInitialSize(IDC_S_INPUT);
	SaveInitialSize(IDC_E_INPUT);
	SaveInitialSize(IDC_STATIC_1);
	SaveInitialSize(IDC_E_OUTPUT);
	SaveInitialSize(IDC_STATIC_2);
	SaveInitialSize(IDC_CBO_DATABASE);
	SaveInitialSize(IDC_CHECK1);

	CreateRoot(HORIZONTAL, 5, 6)
		// left
		<< ( pane(VERTICAL, ABSOLUTE_VERT)
			<< item(IDC_S_INPUT, ABSOLUTE_VERT)
			<< ( pane(HORIZONTAL, ABSOLUTE_VERT)
				<< item(IDC_E_INPUT, ABSOLUTE_VERT)
				<< item(IDC_BROWSE_INPUT, NORESIZE)
			)

			<< itemFixed(VERTICAL, 6)

			<< item(IDC_STATIC_1, ABSOLUTE_VERT)
			<< ( pane(HORIZONTAL, ABSOLUTE_VERT)
				<< item(IDC_E_OUTPUT, ABSOLUTE_VERT)
				<< item(IDC_BROWSE_OUTPUT, NORESIZE)
			)

			<< itemFixed(VERTICAL, 6)

			<< item(IDC_STATIC_2, ABSOLUTE_VERT)
			<< ( pane(HORIZONTAL, ABSOLUTE_VERT)
				<< item(IDC_CBO_DATABASE, ABSOLUTE_VERT)
				<< item(IDC_BROWSE_DATABASE, NORESIZE)
			)

			<< ( pane(HORIZONTAL, ABSOLUTE_VERT)
				<< itemGrowing(HORIZONTAL)
				<< item(IDC_SET_AS_DEFAULT, NORESIZE)
				<< itemSpaceLike(HORIZONTAL, IDC_BROWSE_DATABASE)
			)

			<< itemFixed(VERTICAL, 6)

			<< item(IDC_CHECK1, ABSOLUTE_VERT)
		)

		<< itemFixed(HORIZONTAL, 20)

		// right
		<< ( pane(VERTICAL, NORESIZE)
			<< item(IDC_START, NORESIZE)
			<< item(IDCANCEL, NORESIZE)
		);

	UpdateLayout();
}

CSize CRunDlg::GetMinSize(int IDD)
{
	CWnd* pWnd    = GetDlgItem(IDD);
	HWND hwndCtrl = pWnd->GetSafeHwnd();
	ASSERT( hwndCtrl );

	CRect rcControl;
	::GetWindowRect(hwndCtrl, &rcControl);

	CSize size;
	size.cx = rcControl.Width();
	size.cy = rcControl.Height();

	ASSERT(size.cx > 0);
	ASSERT(size.cy > 0);

	return size;
}

bool CRunDlg::SaveInitialSize(int IDD)
{
	m_mapSizes.insert(std::pair<int, CSize>(IDD, GetMinSize(IDD)));
	return true;
}

void CRunDlg::OnStart() 
{
	CString strStartBtn;
	CWnd* pWndStartBtn = GetDlgItem(IDC_START);
	pWndStartBtn->GetWindowText(strStartBtn);

	if (strStartBtn.Compare(_T("Start")) == 0) 
	{
		if (!UpdateData(TRUE))
		{
			return;
		}

		// make sure input and output are different
		if (m_strInput == m_props.m_strOutPathName)
		{
			CString strMsg;
			strMsg.Format(_T("Output file must be different from input file."));
			MessageBox(strMsg, _T("Invalid Output File"), MB_OK|MB_ICONSTOP);
			return;
		}

		// check if output file is writable
		if (!CUtil::IsFileWriteable(m_props.m_strOutPathName)) 
		{
			CString strMsg;
			strMsg.Format(_T("Cannot open output file \"%s\" for writing."), m_props.m_strOutPathName);
			MessageBox(strMsg, _T("File Error"), MB_OK|MB_ICONSTOP);
			return;
		}

		// check if output file is writable
		if (((CPhreeqciApp*)AfxGetApp())->m_settings.m_bWarnOverwriteOutput
			&& 
			CUtil::FileExists(m_props.m_strOutPathName)) 
		{
			CString strMsg;
			strMsg.Format(_T("%s already exists.  Do you want to replace it?"), m_props.m_strOutPathName);
			if (MessageBox(strMsg, _T("Overwrite Output"), MB_YESNO|MB_ICONEXCLAMATION) == IDNO)
			{
				return;
			}
		}

		// check for database existence
		if (!CUtil::FileExists(m_props.m_strDBPathName)) 
		{
			CString strMsg;
			strMsg.Format(_T("%s was not found"), m_props.m_strDBPathName);
			MessageBox(strMsg, _T("File Error"), MB_OK|MB_ICONSTOP);
			return;
		}


		if (((CRichDocIn*)m_props.m_pDoc)->m_props.m_strDBPathName.CompareNoCase(m_props.m_strDBPathName) != 0)
		{
			((CRichDocIn*)m_props.m_pDoc)->m_props.m_strDBPathName = m_props.m_strDBPathName;
			m_props.m_pDoc->SetModifiedFlag(TRUE);
		}
		ASSERT(((CRichDocIn*)m_props.m_pDoc)->m_props.m_strDBPathName.CompareNoCase(m_props.m_strDBPathName) == 0);

		if (m_props.m_pDoc->GetPathName().CompareNoCase(m_strInput) != 0)
		{
			m_props.m_pDoc->DoSave(m_strInput);
			ASSERT(m_props.m_pDoc->IsModified() == FALSE);
			ASSERT(m_props.m_pDoc->GetPathName().CompareNoCase(m_strInput) == 0);
		}
		ASSERT(m_props.m_pDoc->GetPathName().CompareNoCase(m_strInput) == 0);

		// check if file needs saving
		if (m_props.m_pDoc->IsModified())
		{
			if (((CPhreeqciApp*)AfxGetApp())->m_settings.m_bWarnOverwriteInput) 
			{
				ASSERT(CUtil::FileExists(m_props.m_pDoc->GetPathName()));
				CString strMsg;
				strMsg.Format(_T("%s already exists.  Do you want to replace it?"), m_props.m_pDoc->GetPathName());
				if (MessageBox(strMsg, _T("Overwrite Input"), MB_YESNO|MB_ICONEXCLAMATION) == IDNO)
				{
					return;
				}
			}
			if (!m_props.m_pDoc->DoFileSave())
			{
				return;
			}
		}

		pWndStartBtn->EnableWindow(FALSE);
		GetDlgItem(IDC_CBO_DATABASE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BROWSE_INPUT)->EnableWindow(FALSE);
		GetDlgItem(IDC_SET_AS_DEFAULT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BROWSE_OUTPUT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BROWSE_DATABASE)->EnableWindow(FALSE);
		GetDlgItem(IDC_S_SIM)->SetWindowText(_T(""));
		GetDlgItem(IDC_S_TRANS)->SetWindowText(_T(""));
		GetDlgItem(IDC_EDIT1)->SetWindowText(_T(""));
		ExpandDialog();
		g_eventKill.ResetEvent();
		AfxBeginThread(RunThreadProc, this);
	}
	else
	{
		// dismiss button pressed
		CDialog::OnOK();
	}	
}

void CRunDlg::OnCancel() 
{
	CString strLabel;

	if (!GetDlgItem(IDC_START)->IsWindowEnabled())
	{
		// phreeqc is still running halt it
		g_eventKill.SetEvent();
	}
	else
	{
		GetDlgItem(IDC_START)->GetWindowText(strLabel);
		if ( strLabel.Compare(_T("Dismiss")) == 0 )
		{
			CDialog::OnOK();
		}
		else
		{
			baseRunDlg::OnCancel();
		}
	}
}

UINT CRunDlg::RunThreadProc(LPVOID pParam)
{
	TRACE("Starting run\n");

	TCHAR szDrive[_MAX_DRIVE];
	TCHAR szDir[_MAX_DIR];
	TCHAR szFname[_MAX_FNAME];
	TCHAR szExt[_MAX_EXT];
	TCHAR szPath[_MAX_PATH];

	CRunDlg* pThis = (CRunDlg*)pParam;


	ASSERT(!pThis->m_strInput.IsEmpty());
	ASSERT(!pThis->m_props.m_strOutPathName.IsEmpty());
	ASSERT(!pThis->m_props.m_strDBPathName.IsEmpty());

	// set args for phreeqc
	char * argv[5];
	argv[0] = NULL;
	argv[1] = pThis->m_strInput.GetBuffer(pThis->m_strInput.GetLength() + 1);
	argv[2] = pThis->m_props.m_strOutPathName.GetBuffer(pThis->m_props.m_strOutPathName.GetLength() + 1);
	argv[3] = pThis->m_props.m_strDBPathName.GetBuffer(pThis->m_props.m_strDBPathName.GetLength() + 1);
	argv[4] = pThis->m_strError.GetBuffer(pThis->m_strError.GetLength() + 1);

	// change current directory to that of output file
	_tsplitpath(argv[2], szDrive, szDir, szFname, szExt);
	_tmakepath(szPath, szDrive, szDir, NULL, NULL);
	VERIFY(SetCurrentDirectory(szPath));

	g_status.hSim   = pThis->GetDlgItem(IDC_S_SIM)->GetSafeHwnd();
	g_status.hTrans = pThis->GetDlgItem(IDC_S_TRANS)->GetSafeHwnd();
	g_status.hText  = pThis->GetDlgItem(IDC_EDIT1)->GetSafeHwnd();
	g_hKill         = (HANDLE)g_eventKill;


	ASSERT( g_status.hSim );
	ASSERT( g_status.hTrans );
	ASSERT( g_status.hText );

	// start phreeqc
	// ::input_errors = 0;
	DWORD dwVal = gui_main(5, argv, pThis, WriteCallBack);

	pThis->m_strInput.ReleaseBuffer();
	pThis->m_props.m_strOutPathName.ReleaseBuffer();
	pThis->m_props.m_strDBPathName.ReleaseBuffer();

	// let dialog know that phreeqc has finished running
	// this message causes OnThreadFinished to be called
	switch (dwVal)
	{
	case 0 :
		// main finished normally
		pThis->PostMessage(WM_THREADFINISHED);
		break;
	case USER_CANCELED_RUN :
		pThis->PostMessage(WM_THREADCANCELED);
		break;

	case INPUT_CONTAINS_ERRORS :
		pThis->PostMessage(WM_THREADERRORS);
		break;

	default :
		//a hardware exception has occured
		pThis->PostMessage(WM_THREADHARDWARE_EXCEPTION, dwVal);
		break;
	}
	return 0;
}

void CRunDlg::ExpandDialog()
{
	GetDlgItem(IDC_S_SIM)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_S_TRANS)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT1)->ShowWindow(SW_SHOW);

	CreateRoot(HORIZONTAL, 5, 6)
		// left
		<< ( pane(VERTICAL, ABSOLUTE_VERT)
			<< item(IDC_S_INPUT, ABSOLUTE_VERT)
			<< ( pane(HORIZONTAL, ABSOLUTE_VERT)
				<< item(IDC_E_INPUT, ABSOLUTE_VERT)
				<< item(IDC_BROWSE_INPUT, NORESIZE)
			)

			<< itemFixed(VERTICAL, 6)

			<< item(IDC_STATIC_1, ABSOLUTE_VERT)
			<< ( pane(HORIZONTAL, ABSOLUTE_VERT)
				<< item(IDC_E_OUTPUT, ABSOLUTE_VERT)
				<< item(IDC_BROWSE_OUTPUT, NORESIZE)
			)

			<< itemFixed(VERTICAL, 6)

			<< item(IDC_STATIC_2, ABSOLUTE_VERT)
			<< ( pane(HORIZONTAL, ABSOLUTE_VERT)
				<< item(IDC_CBO_DATABASE, ABSOLUTE_VERT)
				<< item(IDC_BROWSE_DATABASE, NORESIZE)
			)

			<< ( pane(HORIZONTAL, ABSOLUTE_VERT)
				<< itemGrowing(HORIZONTAL)
				<< item(IDC_SET_AS_DEFAULT, NORESIZE)
				<< itemSpaceLike(HORIZONTAL, IDC_BROWSE_DATABASE)
			)

			<< itemFixed(VERTICAL, 6)

			<< item(IDC_CHECK1, ABSOLUTE_VERT)

			<< itemFixed(VERTICAL, 6)

			<< item(IDC_S_SIM, ABSOLUTE_VERT, 0, 0, GetMinSize(IDC_S_SIM).cx, GetMinSize(IDC_S_SIM).cy)
			<< itemFixed(VERTICAL, 8)
			<< item(IDC_S_TRANS, ABSOLUTE_VERT, 0, 0, GetMinSize(IDC_S_TRANS).cx, GetMinSize(IDC_S_TRANS).cy)
			<< itemFixed(VERTICAL, 8)
			<< item(IDC_EDIT1, GREEDY, 0, 0, GetMinSize(IDC_EDIT1).cx, GetMinSize(IDC_EDIT1).cy)

		)

		<< itemFixed(HORIZONTAL, 20)

		// right
		<< ( pane(VERTICAL, NORESIZE)
			<< item(IDC_START, NORESIZE)
			<< item(IDCANCEL, NORESIZE)
		);

	UpdateLayout();
}

LRESULT CRunDlg::OnThreadFinished(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	GetDlgItem(IDC_START)->SetWindowText(_T("Dismiss"));
	GetDlgItem(IDC_START)->EnableWindow(TRUE);
	GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
	MessageBeep(MB_OK);

	UpdateData(TRUE);
	if (m_bCloseOnFinish)
	{
		CDialog::OnOK();
	}
    
    return 0;
}

LRESULT CRunDlg::OnThreadCanceled(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	MessageBeep(MB_OK);
	CDialog::OnOK();
   
    return 0;
}

LRESULT CRunDlg::OnThreadErrors(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	MessageBeep(MB_OK);

	m_bErrors = true;

	CDialog::OnOK();

	::MessageBox(NULL, _T("The input file contains errors.  Look at the errors tab for an explanation."), _T("File contains errors"), MB_OK|MB_ICONERROR);

   
    return 0;
}

LRESULT CRunDlg::OnThreadHardware(WPARAM wParam, LPARAM lParam)
{
	DWORD dwError = wParam;
	UNREFERENCED_PARAMETER(lParam);

	MessageBeep(MB_OK);

	CString strResource;

	CDialog::OnOK();

	switch (dwError)
	{
	case EXCEPTION_ACCESS_VIOLATION :
		strResource.LoadString(IDS_EXCEPTION_ACCESS_VIOLATION);
		::MessageBox(NULL, strResource, _T("Hardware Exception"), MB_OK|MB_ICONERROR);
		break;
	default :
		ASSERT( FALSE );	// operating system should handle (see phreeqci_gui.c ExceptionFilter())
		break;
	}   
    return 0;
}


void CRunDlg::OnBrowseOutput() 
{
	// Show file Dialog box
	CFileDialog dlg(
		FALSE,					// bOpenFileDialog
		_T("pqo"),				// lpszDefExt
		m_props.m_strOutPathName,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST,
		_T("Phreeqci Output Files (*.pqo)|*.pqo|All Files (*.*)|*.*||")
		);
	dlg.m_ofn.lpstrTitle = _T("Save Output As");
	if (dlg.DoModal() == IDOK)
	{
		m_props.m_strOutPathName = dlg.GetPathName();
		UpdateData(FALSE);
	}	
}

void CRunDlg::OnBrowseDatabase() 
{
	// Show file Dialog box
	CFileDialog dlg(
		TRUE,					// bOpenFileDialog
		NULL,					// lpszDefExt
		m_props.m_strDBPathName,
		OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, 
		_T("Database Files (*.dat)|*.dat|All Files (*.*)|*.*||")
		);
	dlg.m_ofn.lpstrTitle = _T("Select a Database");
	
	if (dlg.DoModal() == IDOK)
	{
		m_props.m_strDBPathName = dlg.GetPathName();
		UpdateData(FALSE);
	}	
}

CString CRunDlg::GetErrorFile()
{
	// get temp directory
	TCHAR szTempDirectory[MAX_PATH];
	GetTempPath(MAX_PATH, szTempDirectory);

	TCHAR szFile[MAX_PATH];

	GetTempFileName(szTempDirectory, "err", 0, szFile);	
	CStdioFile errFile;
	VERIFY( errFile.Open(szFile, CFile::modeWrite | CFile::modeCreate) );
	errFile.Abort();

	ASSERT( CUtil::IsFileWriteable(szFile) );

	return szFile;
}

void CRunDlg::RemoveErrorFile()
{
	// remove error file
	try
	{
		CFile::Remove(m_strError);
	}
	catch (CFileException* e)
	{
		e->Delete();
	}
}

void CRunDlg::UpdateSetAsDefault()
{
	if (m_props.m_strDBPathName.CompareNoCase(((CPhreeqciApp*)AfxGetApp())->m_settings.m_strDefDBPathName) == 0)
	{
		m_btnSetAsDefault.EnableWindow(FALSE);
		m_btnSetAsDefault.SetCheck(BST_CHECKED);
	}
	else
	{
		m_btnSetAsDefault.EnableWindow(TRUE);
		m_btnSetAsDefault.SetCheck(BST_UNCHECKED);
	}
}

void CRunDlg::OnBrowseInput() 
{
	// Show file Dialog box
	CFileDialog dlg(
		FALSE,					// bOpenFileDialog
		_T("pqi"),				// lpszDefExt
		m_strInput,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST,
		_T("Phreeqci Input Files (*.pqi)|*.pqi|All Files (*.*)|*.*||")
		);
	dlg.m_ofn.lpstrTitle = _T("Save Input As");
	if (dlg.DoModal() == IDOK)
	{
		m_strInput = dlg.GetPathName();
		m_props.m_strOutPathName = CUtil::SetFileExtension(m_strInput, _T("pqo"));
		UpdateData(FALSE);
	}	
}

void CRunDlg::OnSelchangeCboDatabase() 
{
	// update m_props.m_strDBPathName before calling UpdateSetAsDefault
	UpdateData(TRUE);
	UpdateSetAsDefault();
}

int CRunDlg::DoModal() 
{
	int nDoModal = baseRunDlg::DoModal();

	if (nDoModal == IDOK && m_bSetAsDefault)
	{
		// set and save default database if nec
		if (m_props.m_strDBPathName.CompareNoCase(((CPhreeqciApp*)AfxGetApp())->m_settings.m_strDefDBPathName) != 0)
		{
			// set in app
			((CPhreeqciApp*)AfxGetApp())->m_settings.m_strDefDBPathName = m_props.m_strDBPathName;
			// save in registry
			((CPhreeqciApp*)AfxGetApp())->SaveSettings();
		}
	}
	return nDoModal;
}

int CRunDlg::WriteCallBack(const int action, const int type, const char *err_str, const int stop, void *cookie, const char *format, va_list args)
{
	UNREFERENCED_PARAMETER(stop);
	UNREFERENCED_PARAMETER(err_str);
	UNREFERENCED_PARAMETER(cookie);

	static char buffer[180];
	static CString str;

	switch(action)
	{
	case ACTION_OUTPUT:
		switch (type)
		{
		case OUTPUT_ERROR:
		case OUTPUT_WARNING:
			break;

		case OUTPUT_SCREEN:
			str = format;
			str.Replace("\n", "\r\n");
			VERIFY(::_vsnprintf(buffer, 180, str, args) != -1);
			::SendMessage(g_status.hText, EM_REPLACESEL, (WPARAM)0, (LPARAM)buffer);
			break;

		case OUTPUT_GUI_ERROR:
			output_message(OUTPUT_SCREEN, err_str, stop, format, args);
			break;

		default:
			break;
		}

		if (::WaitForSingleObject(g_hKill /*g_eventKill*/, 0) == WAIT_OBJECT_0) {
			::error_msg("Execution canceled by user.", CONTINUE);
			::RaiseException(USER_CANCELED_RUN, 0, 0, NULL);
		}

		if (stop == STOP) {
			::RaiseException(INPUT_CONTAINS_ERRORS, 0, 0, NULL);
		}
		break;

	case ACTION_OPEN:
		return(OK);
		break;
	}

	return 0;
}
