// BasicObj.h: interface for the CBasicObj class.
//
// $Id$
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASICOBJ_H__77728E92_61C8_492B_A71E_82997A02C7F2__INCLUDED_)
#define AFX_BASICOBJ_H__77728E92_61C8_492B_A71E_82997A02C7F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KeywordPageListItems.h"

#include "PhreeqcI.h"
#include "phreeqc3/src/PBasic.h"

class CBasicObj  
{
public:
	CBasicObj();
	virtual ~CBasicObj();

	static UINT ThreadProc(LPVOID pParam);

	struct XThreadParam
	{
		PBasic* basic;
		char* szCommand;
		struct rate* rate_ptr;
		HANDLE hInfiniteLoop;
	};

	enum
	{
		BASIC_OK = 0,
		BASIC_ERRORS = 1,
		BASIC_EXCEPTION = 2
	};

public:
	void DDX_BasicCommand(CDataExchange* pDX, int nIDC, long nRow, long nCol, basic_command& value);
	void DDV_BasicCommands(CDataExchange* pDX, int nIDC, std::list<basic_command>& r_listCommands, bool bRenumbering = false);
	void DDV_ContainsSave(CDataExchange* pDX, int nIDC, std::list<basic_command>& r_listCommands);
	BOOL Renumber(std::list<basic_command>& r_listCommands);


protected:
	CStdioFile m_errFile;
	CStdioFile m_logFile;
	CStdioFile m_outFile;

	PhreeqcI phreeqc;
	PBasic basic;

#if defined(SAVE_TEMP_FILE_EXAMPLE)
	bool RemoveTempFiles();
	bool OpenTempFiles();
#endif
	void SetErrorCell(CDataExchange* pDX, int nIDC, long nLine);

	CString ListTokens(void* pVoid);
};

#endif // !defined(AFX_BASICOBJ_H__77728E92_61C8_492B_A71E_82997A02C7F2__INCLUDED_)
