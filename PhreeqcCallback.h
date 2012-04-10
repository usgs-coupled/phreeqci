// PhreeqcCallback.h: interface for the CPhreeqcCallback class.
//
// $Id$
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PHREEQCCALLBACK_H__6891B774_CF9C_11D3_BB5D_02608C8F4400__INCLUDED_)
#define AFX_PHREEQCCALLBACK_H__6891B774_CF9C_11D3_BB5D_02608C8F4400__INCLUDED_

#include "phreeqci_gui.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// COMMENT: {2/23/2012 5:02:54 PM}#include "phreeqci_gui.h"

class CPhreeqcCallback : public CObject  
{
public:
	bool Go(LPFNPHREEQC lpfn);
	bool Go(LPFNPHREEQC2 lpfn2, int nValue);
	CPhreeqcCallback();
	CPhreeqcCallback(GUICALLBACK guiCB);
	virtual ~CPhreeqcCallback();

protected:
	CStdioFile m_errFile;
	CStdioFile m_logFile;
	CStdioFile m_outFile;

	bool RemoveTempFiles();
	bool OpenTempFiles();
	GUICALLBACK m_guiCallback;
};

#endif // !defined(AFX_PHREEQCCALLBACK_H__6891B774_CF9C_11D3_BB5D_02608C8F4400__INCLUDED_)
