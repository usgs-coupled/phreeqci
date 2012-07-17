#include "stdafx.h"
#include "phreeqci2.h"
#include "RunDlg2.h"

#include "Util.h"
#include "SaveCurrentDirectory.h"

//{{
#include "phreeqc3/src/Exchange.h"
#include "phreeqc3/src/Solution.h"
//}}

extern struct h_status g_status;
char g_szLineBuf[160];

extern CEvent g_eventKill;

CRunDlg2::CRunDlg2(void)
{
}

CRunDlg2::~CRunDlg2(void)
{
	this->clear_istream();
	this->close_ostreams();
}

BEGIN_MESSAGE_MAP(CRunDlg2, CRunDlg)
	//{{AFX_MSG_MAP(CRunDlg)
	ON_BN_CLICKED(IDC_START, OnStart)
// COMMENT: {3/7/2012 6:46:36 PM}	ON_BN_CLICKED(IDC_BROWSE_OUTPUT, OnBrowseOutput)
// COMMENT: {3/7/2012 6:46:36 PM}	ON_BN_CLICKED(IDC_BROWSE_DATABASE, OnBrowseDatabase)
// COMMENT: {3/7/2012 6:46:36 PM}	ON_BN_CLICKED(IDC_BROWSE_INPUT, OnBrowseInput)
// COMMENT: {3/7/2012 6:46:36 PM}	ON_CBN_SELCHANGE(IDC_CBO_DATABASE, OnSelchangeCboDatabase)
// COMMENT: {3/7/2012 6:46:36 PM}	//}}AFX_MSG_MAP
// COMMENT: {3/7/2012 6:46:36 PM}	ON_MESSAGE(WM_THREADFINISHED, OnThreadFinished)
// COMMENT: {3/7/2012 6:46:36 PM}	ON_MESSAGE(WM_THREADCANCELED, OnThreadCanceled)
// COMMENT: {3/7/2012 6:46:36 PM}	ON_MESSAGE(WM_THREADERRORS, OnThreadErrors)
// COMMENT: {3/7/2012 6:46:36 PM}	ON_MESSAGE(WM_THREADHARDWARE_EXCEPTION, OnThreadHardware)
// COMMENT: {4/30/2012 3:58:45 PM}	ON_BN_CLICKED(IDC_BROWSE_OUTPUT, OnBrowseOutput)
// COMMENT: {4/30/2012 3:58:45 PM}	ON_BN_CLICKED(IDC_BROWSE_DATABASE, OnBrowseDatabase)
// COMMENT: {4/30/2012 3:58:45 PM}	ON_BN_CLICKED(IDC_BROWSE_INPUT, OnBrowseInput)
// COMMENT: {4/30/2012 3:58:45 PM}	ON_CBN_SELCHANGE(IDC_CBO_DATABASE, OnSelchangeCboDatabase)
// COMMENT: {4/30/2012 3:58:45 PM}	//}}AFX_MSG_MAP
// COMMENT: {4/30/2012 3:58:45 PM}	ON_MESSAGE(WM_THREADFINISHED, OnThreadFinished)
// COMMENT: {4/30/2012 3:58:45 PM}	ON_MESSAGE(WM_THREADCANCELED, OnThreadCanceled)
// COMMENT: {4/30/2012 3:58:45 PM}	ON_MESSAGE(WM_THREADERRORS, OnThreadErrors)
// COMMENT: {4/30/2012 3:58:45 PM}	ON_MESSAGE(WM_THREADHARDWARE_EXCEPTION, OnThreadHardware)
END_MESSAGE_MAP()


int CRunDlg2::getc(void)
{
	return PHRQ_io::getc();
}

void CRunDlg2::warning_msg(const char *err_str)
{
	// no-op
	UNUSED_ALWAYS(err_str);
}

void CRunDlg2::error_msg(const char *err_str, bool stop)
{
	// no-op
	UNUSED_ALWAYS(err_str);

	if (stop == STOP)
	{
		::RaiseException(INPUT_CONTAINS_ERRORS, 0, 0, NULL);
	}
}

void CRunDlg2::screen_msg(const char *err_str)
{
	static CString str;
	str = err_str;
	str.Replace("\n", "\r\n");
	::SendMessage(g_status.hText, EM_REPLACESEL, (WPARAM)0, (LPARAM)str.GetBuffer());
}

void CRunDlg2::OnStart() 
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

UINT CRunDlg2::RunThreadProc(LPVOID pParam)
{
	TRACE("Starting run\n");

	TCHAR szDrive[_MAX_DRIVE];
	TCHAR szDir[_MAX_DIR];
	TCHAR szFname[_MAX_FNAME];
	TCHAR szExt[_MAX_EXT];
	TCHAR szPath[_MAX_PATH];

	CRunDlg2* pThis = (CRunDlg2*)pParam;

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

	CSaveCurrentDirectory save;
	VERIFY(save.SetCurrentDirectory(szPath));

	g_status.hSim   = pThis->GetDlgItem(IDC_S_SIM)->GetSafeHwnd();
	g_status.hTrans = pThis->GetDlgItem(IDC_S_TRANS)->GetSafeHwnd();
	g_status.hText  = pThis->GetDlgItem(IDC_EDIT1)->GetSafeHwnd();
	ASSERT( g_status.hSim );
	ASSERT( g_status.hTrans );
	ASSERT( g_status.hText );

	PhreeqcI p(5, argv, pThis);
	DWORD dwVal = p.GetReturnValue();

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

#if SKIP
/* ---------------------------------------------------------------------- */
int Phreeqc::status(int count, const char *str, bool kinetics)
/* ---------------------------------------------------------------------- */
{
// COMMENT: {3/8/2012 5:22:10 PM}	extern HANDLE g_hKill;

	static int spinner;
	char sim_str[20];
	char state_str[45];
	char spin_str[2];
#if defined(PHREEQCI_GUI)
	if (WaitForSingleObject(g_eventKill, 0) == WAIT_OBJECT_0)
	{
		error_msg("Execution canceled by user.", CONTINUE);
		RaiseException(USER_CANCELED_RUN, 0, 0, NULL);
	}
#endif

	if (pr.status == FALSE) return(OK);
	sprintf(sim_str,"Simulation %d.", simulation);
	sprintf(state_str," ");
	sprintf(spin_str," ");

	if (state == INITIALIZE)
	{
		screen_msg(sformatf("\n%-80s", "Initializing..."));
		return(OK);
	}
/*
 *   If str is defined print it
 */
	if (str != NULL)
	{
		if (status_on != TRUE)
		{
			status_on = TRUE;
		}
		if (state != TRANSPORT)
		{
			sprintf(g_szLineBuf, "%-80s\r\n", str); 
			PostMessage(g_status.hText, EM_REPLACESEL, (WPARAM)0, (LPARAM)g_szLineBuf);
		} 
		else
		{
			SetWindowText(g_status.hTrans, str);
		}
	}
	else if (state != TRANSPORT)
	{
		if (state == INITIAL_SOLUTION)
		{
			sprintf(state_str, "Initial solution %d.", use.Get_solution_ptr()->Get_n_user());
		}
		else if (state == INITIAL_EXCHANGE)
		{
			sprintf(state_str, "Initial exchange %d.", use.Get_exchange_ptr()->Get_n_user());
		}
		else if (state == INITIAL_SURFACE)
		{
			sprintf(state_str, "Initial surface %d.", use.Get_exchange_ptr()->Get_n_user());
		}
		else if (state == INVERSE)
		{
			sprintf(state_str, "Inverse %d. Models = %d.", use.Get_inverse_ptr()->n_user, count);
		}
		else if (state == REACTION)
		{
			if (use.Get_kinetics_in() == TRUE)
			{
				sprintf(state_str, "Kinetic step %d.", reaction_step);
			}
			else
			{
				sprintf(state_str, "Reaction step %d.", reaction_step);
			}
		}
		else if (state == ADVECTION || state == TRANSPORT)
		{
			if (state == ADVECTION)
			{
				sprintf(state_str, "Advection, shift %d.", advection_step);
			}
			else if (state == TRANSPORT)
			{
				sprintf(state_str, "Transport, shift %d.", transport_step);
			}
			spinner++;
			if (spinner == 1)
			{
				spin_str[0] = '/';
			}
			else if (spinner == 2)
			{
				spin_str[0] = '-';
			}
			else
			{
				spin_str[0] = '\\';
				spinner = 0;
			}
		}
		if (status_on != TRUE)
		{
			status_on = TRUE;
		}
		if (use.Get_kinetics_in() == TRUE)
		{
			sprintf(g_szLineBuf, "%-15s%-27s%38s", sim_str, state_str, " ");
			SetWindowText(g_status.hSim, g_szLineBuf);
		}
		else
		{
			sprintf(g_szLineBuf, "%-15s%-27s%1s%37s", sim_str, state_str, spin_str, " ");
			SetWindowText(g_status.hSim, g_szLineBuf);
		}
	}
	return(OK);
}
#endif
/* ---------------------------------------------------------------------- */
int Phreeqc::status(int count, const char *str, bool rk_string)
/* ---------------------------------------------------------------------- */
{
	static int spinner;
	char sim_str[20];
	char state_str[45];
	char spin_str[2];

#if defined(PHREEQCI_GUI)
	::PhreeqcIWait(this);
#endif
	if (pr.status == FALSE || phast == TRUE)
		return (OK);

	if (state == INITIALIZE)
	{
		screen_msg(sformatf("\n%-80s", "Initializing..."));
		return (OK);
	}

	switch (state)
	{
	case INITIALIZE:
		break;
	case TRANSPORT:
		if (str != NULL)
		{
			if (rk_string)
			{
				screen_string = screen_string.substr(0, 43);
				screen_string.append(str);
				screen_msg(screen_string.c_str());
				sprintf(g_szLineBuf, "%-80s\r\n", str);
				PostMessage(g_status.hText, EM_REPLACESEL, (WPARAM)0, (LPARAM)g_szLineBuf);
				SetWindowText(g_status.hTrans, screen_string.c_str());
			}
			else
			{
				screen_string = "\r";
				screen_string.clear();
				screen_string.append(str);
				screen_msg(screen_string.c_str());
				sprintf(g_szLineBuf, "%-80s\r\n", str);
				SetWindowText(g_status.hTrans, str);
			}
		}
	case PHAST:
		break;
	default:
		// if str not NULL, print it
		if (str != NULL && !rk_string)
		{
			//screen_string = "\r";
			//screen_string.append(str);
			//screen_msg(screen_string.c_str());
			sprintf(g_szLineBuf, "%-80s\r\n", str);
			TRACE("g_status.hText = \"%s\"\n", g_szLineBuf);
			PostMessage(g_status.hText, EM_REPLACESEL, (WPARAM)0, (LPARAM)g_szLineBuf);
		}
		else
		// print state
		{
			std::string stdstr;
			if (str != NULL && rk_string)
			{
				stdstr = str;
			}
			sprintf(sim_str, "Simulation %d.", simulation);
			sprintf(state_str, " ");
			sprintf(spin_str, " ");
			switch (state)
			{
			default:
				break;
			case INITIAL_SOLUTION:
				sprintf(state_str, "Initial solution %d.", use.Get_solution_ptr()->Get_n_user());
				break;
			case INITIAL_EXCHANGE:
				sprintf(state_str, "Initial exchange %d.", use.Get_exchange_ptr()->Get_n_user());
				break;
			case INITIAL_SURFACE:
				sprintf(state_str, "Initial surface %d.", use.Get_surface_ptr()->Get_n_user());
				break;
			case INVERSE:
				sprintf(state_str, "Inverse %d. Models = %d.", use.Get_inverse_ptr()->n_user, count);
				break;
			case REACTION:
				if (use.Get_kinetics_in() == TRUE)
				{
					sprintf(state_str, "Kinetic step %d.", reaction_step);
				}
				else
				{
					sprintf(state_str, "Reaction step %d.", reaction_step);
				}
				break;
			case ADVECTION:
				sprintf(state_str, "Advection, shift %d.", advection_step);
				break;
			}
			/**
			spinner++;
			if (spinner == 1)
			{
				spin_str[0] = '/';
			}
			else if (spinner == 2)
			{
				spin_str[0] = '-';
			}
			else
			{
				spin_str[0] = '\\';
				spinner = 0;
			}
			**/
			if (use.Get_kinetics_in() == TRUE)
			{
				sprintf(g_szLineBuf, "%-15s%-27s%38s", sim_str, state_str, stdstr.c_str());
				TRACE("g_status.hSim = \"%s\"\n", g_szLineBuf);
				SetWindowText(g_status.hSim, g_szLineBuf);
			}
			else
			{
				sprintf(g_szLineBuf, "%-15s%-27s%1s%37s", sim_str, state_str, spin_str, stdstr.c_str());
				TRACE("g_status.hSim = \"%s\"\n", g_szLineBuf);
				SetWindowText(g_status.hSim, g_szLineBuf);
			}
		}
		status_on = true;
		break;
	}
	return (OK);
}

void PhreeqcIWait(Phreeqc *phreeqc)
{
	if (::WaitForSingleObject(g_eventKill, 0) == WAIT_OBJECT_0)
	{
		phreeqc->error_msg("Execution canceled by user.", CONTINUE);
		::RaiseException(USER_CANCELED_RUN, 0, 0, NULL);
	}
}
