// PhreeqcCallback.cpp: implementation of the CPhreeqcCallback class.
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "PhreeqcCallback.h"

#include "phreeqci_gui.h"

extern "C" {
#include "phreeqc/src/output.h"
}

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPhreeqcCallback::CPhreeqcCallback()
{
	m_guiCallback.dwCookie = NULL;
	m_guiCallback.pfnError = NULL;
	m_guiCallback.pfnLine = NULL;
	m_guiCallback.pfnWarn = NULL;
}

CPhreeqcCallback::CPhreeqcCallback(GUICALLBACK guiCB)
: m_guiCallback(guiCB)
{
}

CPhreeqcCallback::~CPhreeqcCallback()
{
	VERIFY( RemoveTempFiles() );
}

bool CPhreeqcCallback::OpenTempFiles()
{
	// sanity check
#if defined(NEEDS_FIXING)
	_ASSERTE( input      == NULL );
	_ASSERTE( output     == NULL ); // Note this will assert if there are more than one instances
	_ASSERTE( log_file   == NULL );
	_ASSERTE( punch_file == NULL );
	_ASSERTE( dump_file  == NULL );
	_ASSERTE( error_file == NULL );
#endif
	// get temp directory
	TCHAR szTempDirectory[MAX_PATH];
	GetTempPath(MAX_PATH, szTempDirectory);

	// open temp files
	TCHAR szFile[MAX_PATH];

	GetTempFileName(szTempDirectory, "err", 0, szFile);	
	VERIFY( m_errFile.Open(szFile, CFile::modeWrite | CFile::modeCreate) );

	GetTempFileName(szTempDirectory, "log", 0, szFile);	
	VERIFY( m_logFile.Open(szFile, CFile::modeWrite | CFile::modeCreate) );
	
	GetTempFileName(szTempDirectory, "out", 0, szFile);	
	VERIFY( m_outFile.Open(szFile, CFile::modeWrite | CFile::modeCreate) );

#if defined(NEEDS_FIXING)
	// set phreeqc vars
	error_file = m_errFile.m_pStream;
	log_file   = m_logFile.m_pStream;
	output     = m_outFile.m_pStream;
	input      = NULL;
#endif

	return true;
}

bool CPhreeqcCallback::RemoveTempFiles()
{
	// These files are closed in clean_up()
	m_logFile.Abort();
	m_errFile.Abort();
	m_outFile.Abort();

	CFileStatus fileStatus;
	m_errFile.GetStatus(fileStatus);

	// attempt to delete temp files
	try {
		CFile::Remove(m_errFile.GetFilePath());
		CFile::Remove(m_logFile.GetFilePath());
		CFile::Remove(m_outFile.GetFilePath());
	}
	catch (CFileException* e) {
		e->ReportError();
		_ASSERTE(FALSE);
		e->Delete();
		return false;
	}

#if defined(NEEDS_FIXING)
	// reset phreeqc vars
	input      = NULL;
	output     = NULL;
	log_file   = NULL;
	punch_file = NULL;
	dump_file  = NULL;
	error_file = NULL;
#endif

	return true;
}

bool CPhreeqcCallback::Go(LPFNPHREEQC lpfn)
{
	VERIFY( OpenTempFiles() );

	ASSERT(m_guiCallback.dwCookie != NULL);
	lpfn(m_guiCallback);	

	return true;
}

bool CPhreeqcCallback::Go(LPFNPHREEQC2 lpfn2, int nValue)
{
	VERIFY( OpenTempFiles() );

	ASSERT(m_guiCallback.dwCookie != NULL);
	lpfn2(m_guiCallback, nValue);	

	return true;
}
