// BasicObj.cpp: implementation of the CBasicObj class.
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "BasicObj.h"

#include "DDX_DDV.h"
#include "EditGrid.h"
#include <signal.h>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "phreeqc3/src/PBasic.h"

// static inline functions
static inline linerec* FindLine(linerec* linebase, long n)
{
	linerec* l;
	l = linebase;
	while (l != NULL && l->num != n)
		l = l->next;
	return l;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBasicObj::CBasicObj()
: phreeqc()
, basic(&phreeqc)
{
	ASSERT(this->basic.Get_P_escapecode()   == 0);
	ASSERT(this->basic.Get_nIDErrPrompt()   == 0);
	ASSERT(this->basic.Get_nErrLineNumber() == 0);

#if defined MULTICHART
	ChartHandler& handler = phreeqc.Get_chart_handler();

	std::istringstream iss_in("1 ;-active true");
	CParser parser(iss_in, phreeqc.Get_phrq_io());
	handler.Read(&phreeqc, parser);
#endif

	this->basic.Set_phreeqci_gui(true);
	this->basic.Set_parse_all(true);

#if defined(SAVE_TEMP_FILE_EXAMPLE)
	// init files for phreeqc
	OpenTempFiles();
#endif
}

CBasicObj::~CBasicObj()
{
}

#if defined(SAVE_TEMP_FILE_EXAMPLE)

bool CBasicObj::OpenTempFiles()
{
	// sanity check
	_ASSERTE( input      == NULL );
	_ASSERTE( output     == NULL );
	_ASSERTE( log_file   == NULL );
	_ASSERTE( punch_file == NULL );
	_ASSERTE( dump_file  == NULL );
	_ASSERTE( error_file == NULL );
#ifdef _DEBUG
	// get temp directory
	TCHAR szTempDirectory[MAX_PATH];
	GetTempPath(MAX_PATH, szTempDirectory);

	// open temp files
	TCHAR szFile[MAX_PATH];

	GetTempFileName(szTempDirectory, _T("err"), 0, szFile);	
	VERIFY( m_errFile.Open(szFile, CFile::modeWrite | CFile::modeCreate) );

	GetTempFileName(szTempDirectory, _T("log"), 0, szFile);	
	VERIFY( m_logFile.Open(szFile, CFile::modeWrite | CFile::modeCreate) );

	GetTempFileName(szTempDirectory, _T("out"), 0, szFile);	
	VERIFY( m_outFile.Open(szFile, CFile::modeWrite | CFile::modeCreate) );
#else
	// Don't actually create files in release build
	VERIFY( m_errFile.Open(_T("NUL"), CFile::modeWrite | CFile::modeCreate) );
	VERIFY( m_logFile.Open(_T("NUL"), CFile::modeWrite | CFile::modeCreate) );
	VERIFY( m_outFile.Open(_T("NUL"), CFile::modeWrite | CFile::modeCreate) );
#endif	

	ASSERT(m_errFile.m_pStream != NULL);
	ASSERT(m_logFile.m_pStream != NULL);
	ASSERT(m_outFile.m_pStream != NULL);

	// set phreeqc vars
	error_file = m_errFile.m_pStream;
	log_file   = m_logFile.m_pStream;
	output     = m_outFile.m_pStream;
	input      = NULL;

	return true;
}

bool CBasicObj::RemoveTempFiles()
{
	CFile* arrfilesToRemove[] = { &m_errFile, &m_logFile, &m_outFile };

	for (int i = 0; i < sizeof(arrfilesToRemove) / sizeof(arrfilesToRemove[0]); ++i)
	{
		arrfilesToRemove[i]->Abort();

#ifdef _DEBUG
		// attempt to delete temp files
		try
		{
			CFile::Remove(arrfilesToRemove[i]->GetFilePath());
		}
		catch (CFileException* e)
		{
			e->ReportError();
			_ASSERTE(FALSE);
			e->Delete();
		}
#endif
	}

	// reset phreeqc vars
	input      = NULL;
	output     = NULL;
	log_file   = NULL;
	punch_file = NULL;
	dump_file  = NULL;
	error_file = NULL;

	return true;
}
#endif // SAVE_TEMP_FILE_EXAMPLE

UINT CBasicObj::ThreadProc(LPVOID pParam)
{
	ASSERT(pParam != NULL);

	struct XThreadParam* pThreadParam = (struct XThreadParam*)pParam;

	ASSERT(pThreadParam->basic         != NULL);
	ASSERT(pThreadParam->hInfiniteLoop != NULL);
	ASSERT(pThreadParam->szCommand     != NULL);
	ASSERT(pThreadParam->rate_ptr      != NULL);

	// init PBasic
	pThreadParam->basic->Set_hInfiniteLoop(pThreadParam->hInfiniteLoop);
	pThreadParam->basic->Set_parse_whole_program(true);

	int escapeCode = 0;
	try
	{
		escapeCode = pThreadParam->basic->basic_run(
			pThreadParam->szCommand,
			pThreadParam->rate_ptr->linebase,
			pThreadParam->rate_ptr->varbase,
			pThreadParam->rate_ptr->loopbase
			);
	}
	catch (...)
	{
		return BASIC_EXCEPTION;
	}

	if (pThreadParam->basic->Get_nIDErrPrompt() != 0)
	{
		ASSERT(escapeCode != 0);
		return BASIC_ERRORS;
	}
	return BASIC_OK;
}

//
// nCol should contain the line number and nCol + 1 should contain the basic command
//
void CBasicObj::DDX_BasicCommand(CDataExchange* pDX, int nIDC, long nRow, long nCol, basic_command& value)
{
	if (pDX->m_bSaveAndValidate)
	{
		DDX_GridText(pDX, nIDC, nRow, nCol + 1, value.strCommand);
		CString strCommand = value.strCommand;
		strCommand.TrimRight();

		if (strCommand.IsEmpty())
		{
			// empty command ok (ignore line number)
			value.strCommand = _T("");
		}
		else
		{
			// get line number
			DDX_GridText(pDX, nIDC, nRow, nCol, value.nLine);
			if (value.nLine <= 0)
			{
				DDX_GridFail(pDX, _T("Invalid line number."));
			}

			// re-retrieve command to reset grid row and col
			DDX_GridText(pDX, nIDC, nRow, nCol + 1, value.strCommand);
			value.strCommand.TrimRight();

			// format command
			strCommand.Format(_T("%ld %s"), value.nLine, (LPCTSTR)value.strCommand);
			struct rate command = {0};
			command.commands = strCommand.GetBuffer(strCommand.GetLength() + 4);

			// check command
			ASSERT(this->basic.Get_P_escapecode() == 0);

			// make enough space for inbuf
			int max_line_save = this->phreeqc.Get_max_line();
			this->phreeqc.Set_max_line(max(this->phreeqc.Get_max_line(), strCommand.GetLength() + 4));

			this->basic.Set_parse_whole_program(false);
			if (this->basic.basic_compile(command.commands, &command.linebase, &command.varbase, &command.loopbase) != 0)
			{
				// command contains errors 

				// restore
				this->phreeqc.Set_max_line(max_line_save);

				// clean up BASIC before throwing
				this->basic.Set_hInfiniteLoop(0);
				this->basic.Set_parse_whole_program(false);
				this->basic.basic_run("new; quit", command.linebase, command.varbase, command.loopbase);

				strCommand.ReleaseBuffer();
				ASSERT(this->basic.Get_nIDErrPrompt() != 0);

				DDX_GridFail(pDX, this->basic.Get_nIDErrPrompt(), IDR_MAINFRAME);
			}
			else
			{
				// restore
				this->phreeqc.Set_max_line(max_line_save);

				// command doesn't contain errors
				ASSERT(this->basic.Get_P_escapecode() == 0);
				ASSERT(this->basic.Get_nIDErrPrompt() == 0);
				strCommand.ReleaseBuffer();

				this->basic.Set_hInfiniteLoop(0);
				this->basic.Set_parse_whole_program(false);
				this->basic.basic_run("new; quit", command.linebase, command.varbase, command.loopbase);
			}
		}
	}
	else
	{
		// line number
		ASSERT(value.nLine > 0);
		DDX_GridText(pDX, nIDC, nRow, nCol, value.nLine);

		// command
		DDX_GridText(pDX, nIDC, nRow, nCol + 1, value.strCommand);
	}
}

// nCol should contain the line number and nCol + 1 should contain the basic command
//
void CBasicObj::DDV_BasicCommands(CDataExchange* pDX, int nIDC, std::list<basic_command>& r_listCommands, bool bRenumbering /* false */)
{
	if (pDX->m_bSaveAndValidate)
	{
		// verify state of globals
		ASSERT(this->basic.Get_P_escapecode()   == 0);
		ASSERT(this->basic.Get_nIDErrPrompt()   == 0);
		ASSERT(this->basic.Get_nErrLineNumber() == 0);

		// display hourglass
		CWaitCursor wait;

		// concatenate commands into one string separated by semicolons
		//
		CString str = _T(";");
		std::list<basic_command>::iterator commandIter = r_listCommands.begin();
		CString strLine;
		for (; commandIter != r_listCommands.end(); ++commandIter)
		{
			strLine.Format(_T("%ld %s;"),
				(*commandIter).nLine,
				(*commandIter).strCommand
				);
			str += strLine;
		}

		// set default exit code (assume basic_compile_1 failure)
		DWORD dwExitCode = BASIC_ERRORS;

		// compile commands and if no errors run
		//
		struct rate command = {0};
		command.commands = str.GetBuffer(str.GetLength() + 4);

		// make enough space for inbuf
		int max_line_save = this->phreeqc.Get_max_line();
		this->phreeqc.Set_max_line(max(this->phreeqc.Get_max_line(), str.GetLength() + 4));

		this->basic.Set_parse_whole_program(true);
		if (this->basic.basic_compile(command.commands, &command.linebase, &command.varbase, &command.loopbase) == 0)
		{
			// restore
			this->phreeqc.Set_max_line(max_line_save);

			// verify state of globals after successful compile
			ASSERT(this->basic.Get_P_escapecode()   == 0);
			ASSERT(this->basic.Get_nIDErrPrompt()   == 0);
			ASSERT(this->basic.Get_nErrLineNumber() == 0);

			//{{{{{{{{{{{{{{{{{{{{{{
			// iterate over each line
			struct linerec *l = (struct linerec *)command.linebase;
			while (l != NULL)
			{
				// iterate over each token
				struct tokenrec *buf = l->txt;
				while(buf != NULL)
				{
					if (buf->kind == PBasic::tokgoto || buf->kind == PBasic::tokgosub || buf->kind == PBasic::tokrestore)
					{
						while (buf && buf->next && buf->next->kind == PBasic::toknum)
						{
							buf = buf->next;
							long nLine = (long)floor(buf->UU.num + 0.5);
							if (FindLine((struct linerec *)command.linebase, nLine) == NULL)
							{
								// Example input to get here
								// 10 if a == b goto 800
								//
								SetErrorCell(pDX, nIDC, l->num);
								str.Format(_T("Undefined line %ld in line %ld\n"), nLine, l->num);

								// cleanup BASIC
								this->basic.Set_hInfiniteLoop(0);
								this->basic.Set_parse_whole_program(false);
								this->basic.basic_run("new; quit", command.linebase, command.varbase, command.loopbase);

								DDX_GridFail(pDX, str);
							}
							// skip possible commas
							if (buf) buf = buf->next;
						}
					}
					if (buf) buf = buf->next;
				}
				l = l->next;
			}
			//}}}}}}}}}}}}}}}}}}}}}}

			// create event to notify thread to stop and return
			CEvent eventInfiniteLoop;
			eventInfiniteLoop.ResetEvent();

			// init thread param
			struct XThreadParam basicCheck;
			basicCheck.basic         = &this->basic;
			basicCheck.szCommand     = "run";
			basicCheck.rate_ptr      = &command;
			basicCheck.hInfiniteLoop = eventInfiniteLoop.m_hObject;

			// create suspended thread in order to set m_bAutoDelete
			CWinThread* pThread = ::AfxBeginThread(ThreadProc, &basicCheck, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
			pThread->m_bAutoDelete = FALSE;

			// start thread
			pThread->ResumeThread();

			// wait 1.1 seconds for thread to finish
			DWORD dwWait = ::WaitForSingleObject(pThread->m_hThread, 1100);
			if (dwWait == WAIT_TIMEOUT)
			{
				// signal thread to return
				TRACE("Signaling thread\n");
				VERIFY(eventInfiniteLoop.SetEvent());

				// wait a maximum of 2 seconds for thread to finish
				for (int i = 0; i < 20; ++i)
				{
					ASSERT(pThread);
					dwWait = ::WaitForSingleObject(pThread->m_hThread, 100);
					if (dwWait != WAIT_TIMEOUT)
					{
						TRACE("Thread has returned\n");
						break;
					}
				}
			}

			// get exit status of thread (return value of BasicCheckProc())
			dwExitCode = BASIC_OK;
			VERIFY(::GetExitCodeThread(pThread->m_hThread, &dwExitCode));

			if (dwExitCode == STILL_ACTIVE)
			{
				ASSERT(dwWait == WAIT_TIMEOUT);
				::AfxMessageBox(_T("An unrecoverable error has occured.  The program will now exit"), MB_ICONSTOP);
				::TerminateThread(pThread->m_hThread, 1);
				
				this->basic.Set_hInfiniteLoop(0);
				this->basic.Set_parse_whole_program(false);
				this->basic.basic_run("new; quit", command.linebase, command.varbase, command.loopbase);

				delete pThread;
				AfxAbort(); //::PostQuitMessage(1);
				return;
			}

			// destroy thread
			delete pThread;
		}
		else
		{
			// restore
			this->phreeqc.Set_max_line(max_line_save);

			// unable to compile
			// verify error state
			ASSERT(this->basic.Get_P_escapecode() != 0);
			ASSERT(this->basic.Get_nIDErrPrompt() != 0);
		}

		// cleanup BASIC
		this->basic.Set_hInfiniteLoop(0);
		this->basic.Set_parse_whole_program(false);
		this->basic.basic_run("new; quit", command.linebase, command.varbase, command.loopbase);

		// check for errors
		if (dwExitCode == BASIC_ERRORS)
		{
			ASSERT(this->basic.Get_nIDErrPrompt() != 0);

			SetErrorCell(pDX, nIDC, this->basic.Get_nErrLineNumber());

			// save error prompt before resetting
			UINT nPrompt = this->basic.Get_nIDErrPrompt();

			// reset globals
			this->basic.Set_P_escapecode(0);
			this->basic.Set_nIDErrPrompt(0);
			this->basic.Set_nErrLineNumber(0);

			if (nPrompt == IDS_ERR_INFINITE_LOOP)
			{
				// force current cell to be visible
				CEditGrid* pGrid = (CEditGrid*)PrepareEditGridCtrl(pDX, nIDC);
				pGrid->GetCellTop();

				// let user see grid while msgbox is modal
				ASSERT(pGrid->GetHighLight() == flexHighlightWithFocus);
				pGrid->SetHighLight(flexHighlightAlways);
				pGrid->SetRedraw(TRUE);

				if (bRenumbering)
				{
					// use alternative when renumbering
					if (::AfxMessageBox(_T("Warning: Possible infinite loop detected.  Do you want to continue renumbering?"), MB_ICONEXCLAMATION | MB_YESNO) != IDYES)
					{
						// reset grid
						pGrid->SetHighLight(flexHighlightWithFocus);
						pDX->Fail(); // throws exception
					}
					// reset grid
					pGrid->SetHighLight(flexHighlightWithFocus);
				}
				else
				{
					if (::AfxMessageBox(IDS_ERR_INFINITE_LOOP, MB_ICONEXCLAMATION | MB_YESNO) == IDYES)
					{
						// reset grid
						pGrid->SetHighLight(flexHighlightWithFocus);
						pDX->Fail(); // throws exception
					}
					// reset grid
					pGrid->SetHighLight(flexHighlightWithFocus);
				}
			}
			else
			{
				DDX_GridFail(pDX, nPrompt, IDR_MAINFRAME);
			}
		}
		else if (dwExitCode == BASIC_EXCEPTION)
		{
			::AfxMessageBox("An unhandled error has occurred.  Please save your work and restart the program.");
		}
		else
		{
			// verify error state
			ASSERT(this->basic.Get_P_escapecode()   == 0);
			ASSERT(this->basic.Get_nErrLineNumber() == 0);
			ASSERT(this->basic.Get_nIDErrPrompt()   == 0);
		}
	}
}

BOOL CBasicObj::Renumber(std::list<basic_command>& r_listCommands)
{
	CString str = _T(";");
	std::list<basic_command>::iterator commandIter = r_listCommands.begin();
	CString strLine;
	for (; commandIter != r_listCommands.end(); ++commandIter)
	{
		strLine.Format(_T("%ld %s;"),
			(*commandIter).nLine,
			(*commandIter).strCommand
			);
		str += strLine;
	}

	// set default exit code (assume basic_compile_1 failure)
	DWORD dwExitCode = BASIC_ERRORS;

	// compile commands and if no errors run
	//
	struct rate command = {0};
	command.commands = str.GetBuffer(str.GetLength() + 4);

	// make enough space for inbuf
	int max_line_save = this->phreeqc.Get_max_line();
	this->phreeqc.Set_max_line(max(this->phreeqc.Get_max_line(), str.GetLength() + 4));

	this->basic.Set_parse_whole_program(true);
	if (this->basic.basic_compile(command.commands, &command.linebase, &command.varbase, &command.loopbase) == 0)
	{
		// restore
		this->phreeqc.Set_max_line(max_line_save);

		// verify state of globals after successful compile
		ASSERT(this->basic.Get_P_escapecode()   == 0);
		ASSERT(this->basic.Get_nIDErrPrompt()   == 0);
		ASSERT(this->basic.Get_nErrLineNumber() == 0);

		//{{ RUN THREAD
		// create event to notify thread to stop and return
		CEvent eventInfiniteLoop;
		eventInfiniteLoop.ResetEvent();

		// init thread param
		struct XThreadParam basicCheck;
		basicCheck.basic         = &this->basic;
		basicCheck.szCommand     = "renum";
		basicCheck.rate_ptr      = &command;
		basicCheck.hInfiniteLoop = eventInfiniteLoop.m_hObject;

		// create suspended thread in order to set m_bAutoDelete
		CWinThread* pThread =
			::AfxBeginThread(ThreadProc, &basicCheck, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
		pThread->m_bAutoDelete = FALSE;

		// start thread
		pThread->ResumeThread();

		// wait 1.1 seconds for thread to finish
		DWORD dwWait = ::WaitForSingleObject(pThread->m_hThread, 1100);
		if (dwWait == WAIT_TIMEOUT)
		{
			// signal thread to return
			TRACE("Signaling thread\n");
			VERIFY(eventInfiniteLoop.SetEvent());

			// wait a maximum of 2 seconds for thread to finish
			for (int i = 0; i < 20; ++i)
			{
				ASSERT(pThread);
				dwWait = ::WaitForSingleObject(pThread->m_hThread, 100);
				if (dwWait != WAIT_TIMEOUT)
				{
					TRACE("Thread has returned\n");
					break;
				}
			}
		}

		// get exit status of thread (return value of BasicCheckProc())
		dwExitCode = BASIC_OK;
		VERIFY(::GetExitCodeThread(pThread->m_hThread, &dwExitCode));

		if (dwExitCode == STILL_ACTIVE)
		{
			ASSERT(dwWait == WAIT_TIMEOUT);
			::AfxMessageBox(_T("An unrecoverable error has occured.  The program will now exit"), MB_ICONSTOP);
			::TerminateThread(pThread->m_hThread, 1);
			
			// clean up BASIC
			this->basic.Set_hInfiniteLoop(0);
			this->basic.Set_parse_whole_program(false);
			this->basic.basic_run("new; quit", command.linebase, command.varbase, command.loopbase);
			
			delete pThread;
			::PostQuitMessage(1);
			return FALSE;	// failure
		}

		// destroy thread
		delete pThread;
		//}} RUN THREAD

		if (dwExitCode == BASIC_OK)
		{
			r_listCommands.clear();
			basic_command basCommand;
			struct linerec *l = (struct linerec *)command.linebase;
			while (l != NULL)
			{
				basCommand.nLine = l->num;
				basCommand.strCommand = ListTokens(l->txt);
				r_listCommands.push_back(basCommand);
				l = l->next;
			}

			// clean up BASIC
			this->basic.Set_hInfiniteLoop(0);
			this->basic.Set_parse_whole_program(false);
			this->basic.basic_run("new; quit", command.linebase, command.varbase, command.loopbase);
			return TRUE; // success
		}
		else
		{
			ASSERT(FALSE);
			::AfxMessageBox("BASIC failed to compile", MB_ICONSTOP);
		}
	}
	else
	{
		// restore
		this->phreeqc.Set_max_line(max_line_save);

		ASSERT(FALSE);
		::AfxMessageBox("BASIC failed to compile", MB_ICONSTOP);
	}

	return FALSE; // failure
}

CString CBasicObj::ListTokens(void* pVoid)
{
	struct tokenrec *buf = (struct tokenrec *)pVoid;

	CString strReturn(_T(""));
	CString strFprintf(_T(""));

	bool ltr;
	// Char STR1[256];

	ltr = false;
	
	while (buf != NULL)
	{
		if ((buf->kind >= (long)PBasic::toknot && buf->kind <= (long)PBasic::tokrenum) || 
			buf->kind == (long)PBasic::toknum || buf->kind == (long)PBasic::tokvar ||
			buf->kind >= (long)PBasic::toktc)
		{
			if (ltr)
			{
				strReturn += _T(' '); // putc(' ', f);
			}
			ltr = (buf->kind != PBasic::toknot);
			
		}
		else
		{
			ltr = false;
		}
		switch (buf->kind)
		{
		case PBasic::tokvar:
			strReturn += buf->UU.vp->name; // fputs(buf->UU.vp->name, f);
			break;

		case PBasic::toknum:
			strReturn += buf->sz_num; // fputs(buf->sz_num, f);
			////fputs(numtostr(STR1, buf->UU.num), f);
			break;

		case PBasic::tokstr:
			if (::strchr(buf->UU.sp, '\"'))
			{
				ASSERT(::strchr(buf->UU.sp, '\'') == NULL);
				strFprintf.Format("\'%s\'", buf->UU.sp); // fprintf(f, "\"%s\"", buf->UU.sp);
			}
			else
			{
				strFprintf.Format("\"%s\"", buf->UU.sp); // fprintf(f, "\"%s\"", buf->UU.sp);
			}
			strReturn += strFprintf;
			break;

		case PBasic::toksnerr:
			strFprintf.Format("{%c}", buf->UU.snch); // fprintf(f, "{%c}", buf->UU.snch);
			strReturn += strFprintf;
			break;

		case PBasic::tokplus:
			strReturn += '+'; // putc('+', f);
			break;

		case PBasic::tokminus:
			strReturn += '-'; // putc('-', f);
			break;

		case PBasic::toktimes:
			strReturn += '*'; // putc('*', f);
			break;

		case PBasic::tokdiv:
			strReturn += '/'; // putc('/', f);
			break;

		case PBasic::tokup:
			strReturn += '^'; // putc('^', f);
			break;

		case PBasic::toklp:
			strReturn += '('; // putc('(', f);
			break;

		case PBasic::tokrp:
			strReturn += ')'; // putc(')', f);
			break;

		case PBasic::tokcomma:
			///strReturn += ','; // putc(',', f);
			strReturn += ", ";
			break;

		case PBasic::toksemi:
			strReturn += ';'; // putc(';', f);
			break;

		case PBasic::tokcolon:
			strReturn += " : "; // fprintf(f, " : ");
			break;

		case PBasic::tokeq:
			strReturn += " = "; // fprintf(f, " = ");
			break;

		case PBasic::toklt:
			strReturn += " < "; // fprintf(f, " < ");
			break;

		case PBasic::tokgt:
			strReturn += " > "; // fprintf(f, " > ");
			break;

		case PBasic::tokle:
			strReturn += " <= "; // fprintf(f, " <= ");
			break;

		case PBasic::tokge:
			strReturn += " >= "; // fprintf(f, " >= ");
			break;

		case PBasic::tokne:
			strReturn += " <> "; // fprintf(f, " <> ");
			break;

		case PBasic::tokand:
			strReturn += " AND "; // fprintf(f, " AND ");
			break;

		case PBasic::tokor:
			strReturn += " OR "; // fprintf(f, " OR ");
			break;

		case PBasic::tokxor:
			strReturn += " XOR "; // fprintf(f, " XOR ");
			break;

		case PBasic::tokmod:
			strReturn += " MOD "; // fprintf(f, " MOD ");
			break;

		case PBasic::toknot:
			strReturn += "NOT "; // fprintf(f, "NOT ");
			break;

		case PBasic::toksqr:
			strReturn += "SQR"; // fprintf(f, "SQR");
			break;

		case PBasic::toksqrt:
			strReturn += "SQRT"; // fprintf(f, "SQRT");
			break;

		case PBasic::toksin:
			strReturn += "SIN"; // fprintf(f, "SIN");
			break;

		case PBasic::tokcos:
			strReturn += "COS"; // fprintf(f, "COS");
			break;

		case PBasic::toktan:
			strReturn += "TAN"; // fprintf(f, "TAN");
			break;

		case PBasic::tokarctan:
			strReturn += "ARCTAN"; // fprintf(f, "ARCTAN");
			break;

		case PBasic::toklog:
			strReturn += "LOG"; // fprintf(f, "LOG");
			break;

		case PBasic::tokexp:
			strReturn += "EXP"; // fprintf(f, "EXP");
			break;

		case PBasic::tokabs:
			strReturn += "ABS"; // fprintf(f, "ABS");
			break;

		case PBasic::toksgn:
			strReturn += "SGN"; // fprintf(f, "SGN");
			break;

		case PBasic::tokstr_:
			strReturn += "STR$"; // fprintf(f, "STR$");
			break;

		case PBasic::tokval:
			strReturn += "VAL"; // fprintf(f, "VAL");
			break;

		case PBasic::tokchr_:
			strReturn += "CHR$"; // fprintf(f, "CHR$");
			break;

		case PBasic::tokasc:
			strReturn += "ASC"; // fprintf(f, "ASC");
			break;

		case PBasic::toklen:
			strReturn += "LEN"; // fprintf(f, "LEN");
			break;

		case PBasic::tokmid_:
			strReturn += "MID$"; // fprintf(f, "MID$");
			break;

		case PBasic::tokpeek:
			strReturn += "PEEK"; // fprintf(f, "PEEK");
			break;

		case PBasic::tokrem:
			strFprintf.Format("REM%s", buf->UU.sp); // fprintf(f, "REM%s", buf->UU.sp);
			strReturn += strFprintf;
			break;

		case PBasic::toklet:
			strReturn += "LET"; // fprintf(f, "LET");
			break;

		case PBasic::tokprint:
			strReturn += "PRINT "; // fprintf(f, "PRINT");
			break;

		case PBasic::tokinput:
			strReturn += "INPUT"; // fprintf(f, "INPUT");
			break;

		case PBasic::tokgoto:
			strReturn += "GOTO"; // fprintf(f, "GOTO");
			break;

		case PBasic::tokif:
			strReturn += "IF"; // fprintf(f, "IF");
			break;

		case PBasic::tokend:
			strReturn += "END"; // fprintf(f, "END");
			break;

		case PBasic::tokstop:
			strReturn += "STOP"; // fprintf(f, "STOP");
			break;

		case PBasic::tokfor:
			strReturn += "FOR"; // fprintf(f, "FOR");
			break;

		case PBasic::toknext:
			strReturn += "NEXT"; // fprintf(f, "NEXT");
			break;

		case PBasic::tokwhile:
			strReturn += "WHILE"; // fprintf(f, "WHILE");
			break;

		case PBasic::tokwend:
			strReturn += "WEND"; // fprintf(f, "WEND");
			break;

		case PBasic::tokgosub:
			strReturn += "GOSUB"; // fprintf(f, "GOSUB");
			break;

		case PBasic::tokreturn:
			strReturn += "RETURN"; // fprintf(f, "RETURN");
			break;

		case PBasic::tokread:
			strReturn += "READ"; // fprintf(f, "READ");
			break;

		case PBasic::tokdata:
			strReturn += "DATA"; // fprintf(f, "DATA");
			break;

		case PBasic::tokrestore:
			strReturn += "RESTORE"; // fprintf(f, "RESTORE");
			break;

		case PBasic::tokgotoxy:
			strReturn += "GOTOXY"; // fprintf(f, "GOTOXY");
			break;

		case PBasic::tokon:
			strReturn += "ON"; // fprintf(f, "ON");
			break;

		case PBasic::tokdim:
			strReturn += "DIM"; // fprintf(f, "DIM");
			break;

		case PBasic::tokpoke:
			strReturn += "POKE"; // fprintf(f, "POKE");
			break;

		case PBasic::toklist:
			strReturn += "LIST"; // fprintf(f, "LIST");
			break;

		case PBasic::tokrun:
			strReturn += "RUN"; // fprintf(f, "RUN");
			break;

		case PBasic::toknew:
			strReturn += "NEW"; // fprintf(f, "NEW");
			break;

		case PBasic::tokload:
			strReturn += "LOAD"; // fprintf(f, "LOAD");
			break;

		case PBasic::tokmerge:
			strReturn += "MERGE"; // fprintf(f, "MERGE");
			break;

		case PBasic::toksave:
			strReturn += "SAVE"; // fprintf(f, "SAVE");
			break;

		case PBasic::tokbye:
			strReturn += "BYE"; // fprintf(f, "BYE");
			break;

		case PBasic::tokdel:
			strReturn += "DEL"; // fprintf(f, "DEL");
			break;

		case PBasic::tokrenum:
			strReturn += "RENUM"; // fprintf(f, "RENUM");
			break;

		case PBasic::tokthen:
			strReturn += " THEN "; // fprintf(f, " THEN ");
			break;

		case PBasic::tokelse:
			strReturn += " ELSE "; // fprintf(f, " ELSE ");
			break;

		case PBasic::tokto:
			strReturn += " TO "; // fprintf(f, " TO ");
			break;

		case PBasic::tokstep:
			strReturn += " STEP "; // fprintf(f, " STEP ");
			break;

		case PBasic::toktc:
			strReturn += "TC"; // fprintf(f, "TC");
			break;

		case PBasic::tokm0:
			strReturn += "M0"; // fprintf(f, "M0");
			break;

		case PBasic::tokm:
			strReturn += "M"; // fprintf(f, "M");
			break;

		case PBasic::tokparm:
			strReturn += "PARM"; // fprintf(f, "PARM");
			break;

		case PBasic::tokact:
			strReturn += "ACT"; // fprintf(f, "ACT");
			break;

		case PBasic::tokmol:
			strReturn += "MOL"; // fprintf(f, "MOL");
			break;

		case PBasic::tokla:
			strReturn += "LA"; // fprintf(f, "LA");
			break;

		case PBasic::toklm:
			strReturn += "LM"; // fprintf(f, "LM");
			break;

		case PBasic::toksr:
			strReturn += "SR"; // fprintf(f, "SR");
			break;

		case PBasic::toksi:
			strReturn += "SI"; // fprintf(f, "SI");
			break;

		case PBasic::toktot:
			strReturn += "TOT"; // fprintf(f, "TOT");
			break;

		case PBasic::toktk:
			strReturn += "TK"; // fprintf(f, "TK");
			break;

		case PBasic::toktime:
			strReturn += "TIME"; // fprintf(f, "TIME");
			break;

		case PBasic::toklog10:
			strReturn += "LOG10"; // fprintf(f, "LOG10");
			break;

		case PBasic::toksim_time:
			strReturn += "SIM_TIME"; // fprintf(f, "SIM_TIME");
			break;

		case PBasic::tokequi:
			strReturn += "EQUI"; // fprintf(f, "EQUI");
			break;

		case PBasic::tokgas:
			strReturn += "GAS"; // fprintf(f, "GAS");
			break;

		case PBasic::tokpunch:
			strReturn += "PUNCH "; // fprintf(f, "PUNCH");
			break;

		case PBasic::tokkin:
			strReturn += "KIN"; // fprintf(f, "KIN");
			break;

		case PBasic::toks_s:
			strReturn += "S_S"; // fprintf(f, "S_S");
			break;

		case PBasic::tokmu:
			strReturn += "MU"; // fprintf(f, "MU");
			break;

		case PBasic::tokalk:
			strReturn += "ALK"; // fprintf(f, "ALK");
			break;

		case PBasic::tokrxn:
			strReturn += "RXN"; // fprintf(f, "RXN");
			break;

		case PBasic::tokdist:
			strReturn += "DIST"; // fprintf(f, "DIST");
			break;

		case PBasic::tokmisc1:
			strReturn += "MISC1"; // fprintf(f, "MISC1");
			break;

		case PBasic::tokmisc2:
			strReturn += "MISC2"; // fprintf(f, "MISC2");
			break;

		case PBasic::tokedl:
			strReturn += "EDL"; // fprintf(f, "EDL");
			break;

		case PBasic::tokstep_no:
			strReturn += "STEP_NO"; // fprintf(f, "STEP_NO");
			break;

		case PBasic::toksim_no:
			strReturn += "SIM_NO"; // fprintf(f, "SIM_NO");
			break;

		case PBasic::toktotal_time:
			strReturn += "TOTAL_TIME"; // fprintf(f, "TOTAL_TIME");
			break;

		case PBasic::tokput:
			strReturn += "PUT"; // fprintf(f, "PUT");
			break;

		case PBasic::tokget:
			strReturn += "GET"; // fprintf(f, "GET");
			break;

		case PBasic::tokcharge_balance:
			strReturn += "CHARGE_BALANCE"; // fprintf(f, "CHARGE_BALANCE");
			break;

		case PBasic::tokpercent_error:
			strReturn += "PERCENT_ERROR"; // fprintf(f, "PERCENT_ERROR");
			break;

#if defined PHREEQ98 || defined MULTICHART

		case PBasic::tokgraph_x:
			strReturn += "GRAPH_X"; // fprintf(f, "GRAPH_X");
			break;

		case PBasic::tokgraph_y:
			strReturn += "GRAPH_Y"; // fprintf(f, "GRAPH_Y");
			break;

		case PBasic::tokgraph_sy:
			strReturn += "GRAPH_SY"; // fprintf(f, "GRAPH_SY");
			break;
#endif

		case PBasic::tokcell_no:
			strReturn += "CELL_NO"; // fprintf(f, "CELL_NO");
			break;

		case PBasic::tokexists:
			strReturn += "EXISTS"; // fprintf(f, "EXISTS");
			break;

		case PBasic::toksurf:
			strReturn += "SURF"; // output_msg (OUTPUT_BASIC, "SURF");
			break;

		case PBasic::toklk_species:
			strReturn += "LK_SPECIES"; // output_msg (OUTPUT_BASIC, "LK_SPECIES");
			break;

		case PBasic::toklk_named:
			strReturn += "LK_NAMED"; // output_msg (OUTPUT_BASIC, "LK_NAMED");
			break;

		case PBasic::toklk_phase:
			strReturn += "LK_PHASE"; // output_msg (OUTPUT_BASIC, "LK_PHASE");
			break;

		case PBasic::toksum_species:
			strReturn += "SUM_SPECIES"; // output_msg (OUTPUT_BASIC, "SUM_SPECIES");
			break;

		case PBasic::toksum_gas:
			strReturn += "SUM_GAS"; // output_msg (OUTPUT_BASIC, "SUM_GAS");
			break;

		case PBasic::toksum_s_s:
			strReturn += "SUM_s_s"; // output_msg (OUTPUT_BASIC, "SUM_s_s");
			break;

		case PBasic::tokcalc_value:
			strReturn += "CALC_VALUE"; // output_msg (OUTPUT_BASIC, "CALC_VALUE");
			break;

		case PBasic::tokdescription:
			strReturn += "DESCRIPTION"; // output_msg(OUTPUT_BASIC, "DESCRIPTION");
			break;

		case PBasic::toksys:
			strReturn += "SYS"; // output_msg(OUTPUT_BASIC, "SYS");
			break;

		case PBasic::tokinstr:
			strReturn += "INSTR"; // output_msg(OUTPUT_BASIC, "INSTR");
			break;

		case PBasic::tokltrim:
			strReturn += "LTRIM"; // output_msg(OUTPUT_BASIC, "LTRIM");
			break;

		case PBasic::tokrtrim:
			strReturn += "RTRIM"; // output_msg(OUTPUT_BASIC, "RTRIM");
			break;

		case PBasic::toktrim:
			strReturn += "TRIM"; // output_msg(OUTPUT_BASIC, "TRIM");
			break;

		case PBasic::tokpad:
			strReturn += "PAD"; // output_msg(OUTPUT_BASIC, "PAD");
			break;

		case PBasic::tokchange_por:
			strReturn += "CHANGE_POR"; // output_msg (OUTPUT_BASIC, "CHANGE_POR");
			break;

		case PBasic::tokget_por:
			strReturn += "GET_POR"; // output_msg (OUTPUT_BASIC, "GET_POR");
			break;

		case PBasic::tokosmotic:
			strReturn += "OSMOTIC"; // output_msg (OUTPUT_BASIC, "OSMOTIC");
			break;

		case PBasic::tokchange_surf:
			strReturn += "CHANGE_SURF"; // output_msg (OUTPUT_BASIC, "CHANGE_SURF");
			break;

		case PBasic::tokporevolume:
			strReturn += "POREVOLUME"; // output_msg (OUTPUT_BASIC, "POREVOLUME");
			break;

		case PBasic::toksc:
			strReturn += "SC";    // output_msg (OUTPUT_BASIC, "SC");
			break;

		case PBasic::tokgamma:
			strReturn += "GAMMA"; // output_msg(OUTPUT_BASIC, "GAMMA");
			break;

		case PBasic::toklg:
			strReturn += "LG";    // output_msg(OUTPUT_BASIC, "LG");
			break;

		case PBasic::tokrho:
			strReturn += "RHO";   // output_msg(OUTPUT_BASIC, "RHO");
			break;

		case PBasic::tokcell_volume:
			strReturn += "CELL_VOLUME";   // output_msg(OUTPUT_BASIC, "CELL_VOLUME");
			break;

		case PBasic::tokcell_pore_volume:
			strReturn += "CELL_PORE_VOLUME";   // output_msg(OUTPUT_BASIC, "CELL_PORE_VOLUME");
			break;

		case PBasic::tokcell_porosity:
			strReturn += "CELL_POROSITY";   // output_msg(OUTPUT_BASIC, "CELL_POROSITY");
			break;

		case PBasic::tokcell_saturation:
			strReturn += "CELL_SATURATION";   // output_msg(OUTPUT_BASIC, "CELL_SATURATION");
			break;

#if defined MULTICHART
		case PBasic::tokplot_xy:
			strReturn += "PLOT_XY"; // output_msg(OUTPUT_BASIC, "PLOT_XY");
			break;
#endif

		case PBasic::toktotmole:
			strReturn += "TOTMOLE"; // output_msg(OUTPUT_BASIC, "TOTMOLE");
			break;

		case PBasic::tokiso:
			strReturn += "ISO";             // output_msg(OUTPUT_BASIC, "ISO");
			break;

		case PBasic::tokiso_unit:
			strReturn += "ISO_UNIT";        // output_msg(OUTPUT_BASIC, "ISO_UNIT");
			break;

		case PBasic::toktotmol:
		case PBasic::toktotmoles:
			strReturn += "TOTMOLE"; // output_msg(OUTPUT_BASIC, "TOTMOLE");
			break;

		case PBasic::tokeol_:
			strReturn += "EOL$"; // output_msg(OUTPUT_BASIC, "EOL$");
			break;

		case PBasic::tokceil:
			strReturn += "CEIL"; // output_msg(OUTPUT_BASIC, "CEIL");
			break;

		case PBasic::tokfloor:
			strReturn += "FLOOR"; // output_msg(OUTPUT_BASIC, "FLOOR");
			break;

		case PBasic::tokphase_formula:
		case PBasic::tokphase_formula_:
			strReturn += "PHASE_FORMULA$";  // output_msg("PHASE_FORMULA$");
			break;

		case PBasic::toklist_s_s:
			strReturn += "LIST_S_S";        // output_msg("LIST_S_S");
			break;

		case PBasic::tokpr_p:
			strReturn += "PR_P";            // output_msg("PR_P");
			break;

		case PBasic::tokpr_phi:
			strReturn += "PR_PHI";          // output_msg("PR_PHI");
			break;

 		case PBasic::tokgas_p:
 			strReturn += "GAS_P";           // output_msg("GAS_P");
 			break;

 		case PBasic::tokgas_vm:
 			strReturn += "GAS_VM";          // output_msg("GAS_VM");
 			break;

  		case PBasic::tokpressure:
  			strReturn += "PRESSURE";        // output_msg("PRESSURE");
  			break;

		case PBasic::tokerase:
			strReturn += "ERASE";           // output_msg("ERASE");
			break;

		case PBasic::tokeps_r:
			strReturn += "EPS_R";           // output_msg("EPS_R"); // dielectric constant
			break;

 		case PBasic::tokvm:
 			strReturn += "VM";              // output_msg("VM"); // mole volume of aqueous solute
 			break;

 		case PBasic::tokdh_a:
 			strReturn += "DH_A";            // output_msg("DH_A"); // Debye-Hueckel A
 			break;

 		case PBasic::tokdh_b:
 			strReturn += "DH_B";            // output_msg("DH_B"); // Debye-Hueckel B
 			break;

 		case PBasic::tokdh_av:
 			strReturn += "DH_Av";           // output_msg("DH_Av"); // Debye-Hueckel Av
 			break;

 		case PBasic::tokqbrn:
 			strReturn += "QBrn";            // output_msg("QBrn"); // Q_Born, d(eps_r)/d(P)/(eps_r^2)
 			break;

 		case PBasic::tokkappa:
 			strReturn += "KAPPA";           // output_msg("KAPPA"); // compressibility of pure water, d(rho)/d(P) / rho
 			break;

		case PBasic::tokgfw:
 			strReturn += "GFW";             // output_msg("GFW"); // gram formula weight of a formula
 			break;

		case PBasic::toksoln_vol:
 			strReturn += "SOLN_VOL";        // output_msg("SOLN_VOL"); // volume of solution
 			break;

		case PBasic::tokequi_delta:
			strReturn += "EQUI_DELTA";      // output_msg("EQUI_DELTA");
			break;

		case PBasic::tokkin_delta:
			strReturn += "KIN_DELTA";       // output_msg("KIN_DELTA");
			break;

		case PBasic::tokkin_time:
			strReturn += "KIN_TIME";        // output_msg("KIN_TIME");
			break;

		case PBasic::tokstr_f_:
			strReturn += "STR_F$";          // output_msg("STR_F$");
			break;

		case PBasic::tokstr_e_:
			strReturn += "STR_E$";          // output_msg("STR_E$");
			break;

		case PBasic::tokspecies_formula:
		case PBasic::tokspecies_formula_:
			strReturn += "SPECIES_FORMULA$";    // output_msg("SPECIES_FORMULA$");
			break;			

		case PBasic::tokeq_frac:
			strReturn += "EQ_FRAC";             // output_msg("EQ_FRAC");
			break;

		case PBasic::tokdiff_c:
			strReturn += "DIFF_C";              // output_msg("DIFF_C");
			break;

		case PBasic::tokedl_species:
			strReturn += "EDL_SPECIES";         // output_msg("EDL_SPECIES");
			break;

		case PBasic::tokkinetics_formula:
		case PBasic::tokkinetics_formula_:
			strReturn += "KINETICS_FORMULA$";   // output_msg("KINETICS_FORMULA$");
			break;

		case PBasic::tokrho_0:
			strReturn += "RHO_0";               // output_msg("RHO_0");
			break;

 		case PBasic::tokphase_vm:
 			strReturn += "PHASE_VM";            // output_msg("PHASE_VM"); // mole volume of a phase 
 			break;

 		case PBasic::tokaphi:
 			strReturn += "APHI";                // output_msg("APHI"); // mole volume of a phase 
 			break;

		case PBasic::toktitle:
			strReturn += "TITLE";               // output_msg("TITLE");
			break;

		//{{NEW BASIC HERE}}

		default:
			ASSERT(FALSE);
			break;
	}
    buf = buf->next;
  }
  strReturn.Replace("PUNCH  ", "PUNCH ");
  strReturn.Replace("PRINT  ", "PRINT ");
  return strReturn;
}

void CBasicObj::DDV_ContainsSave(CDataExchange* pDX, int nIDC, std::list<basic_command>& r_listCommands)
{
	UNUSED(nIDC);

	CString str = _T(";");
	std::list<basic_command>::iterator commandIter = r_listCommands.begin();
	CString strLine;
	for (; commandIter != r_listCommands.end(); ++commandIter)
	{
		strLine.Format(_T("%ld %s;"),
			(*commandIter).nLine,
			(*commandIter).strCommand
			);
		str += strLine;
	}


	// compile commands
	struct rate command = {0};
	command.commands = str.GetBuffer(str.GetLength() + 4);
	this->basic.Set_parse_whole_program(true);
	if (this->basic.basic_compile(command.commands, &command.linebase, &command.varbase, &command.loopbase) == 0)
	{
		// verify state of globals after successful compile
		ASSERT(this->basic.Get_P_escapecode()   == 0);
		ASSERT(this->basic.Get_nIDErrPrompt()   == 0);
		ASSERT(this->basic.Get_nErrLineNumber() == 0);

		bool bSaveFound = false;

		// iterate over each line
		struct linerec *l = (struct linerec *)command.linebase;
		while (l != NULL && !bSaveFound)
		{
			// iterate over each token
			struct tokenrec *buf = l->txt;
			while(buf != NULL)
			{
				if (buf->kind == PBasic::toksave)
				{
					bSaveFound = true;
					break;
				}
				buf = buf->next;
			}
			l = l->next;
		}

		// clean up BASIC
		this->basic.Set_hInfiniteLoop(0);
		this->basic.Set_parse_whole_program(false);
		this->basic.basic_run("new; quit", command.linebase, command.varbase, command.loopbase);

		if (!bSaveFound)
		{
			DDX_GridFail(pDX, _T("SAVE statement not found.  Each rate must contain \"SAVE expression\" where the "
				"value of expression is the moles of reaction that occur during time subinterval TIME."));
		}
	}
	else
	{
		ASSERT(FALSE);
		::AfxMessageBox("BASIC failed to compile", MB_ICONSTOP);
		// clean up BASIC
		this->basic.Set_hInfiniteLoop(0);
		this->basic.Set_parse_whole_program(false);
		this->basic.basic_run("new; quit", command.linebase, command.varbase, command.loopbase);
	}
}

void CBasicObj::SetErrorCell(CDataExchange* pDX, int nIDC, long nLine)
{
	// place selection on first line 
	CEditGrid* pGrid = (CEditGrid*)PrepareEditGridCtrl(pDX, nIDC);
	ASSERT_VALID(pGrid);
	pGrid->SetRow(0);
	pGrid->SetCol(1);

	if (nLine != 0)
	{
		// find row error occured on (assume line numbers are in column 0)
		CString strNum;
		strNum.Format(_T("%ld"), nLine);
		for (long nRow = pGrid->GetFixedRows(); nRow < pGrid->GetRows(); ++nRow)
		{
			if (strNum.Compare(pGrid->GetTextMatrix(nRow, 0)) == 0)
			{
				pGrid->SetRow(nRow);
				break;
			}
		}
	}
}
