#pragma once

#include "RunCellsPg1.h"

// CRunCells

#include "OleCtlKeywordSheet.h"

#ifndef baseRunCells
#define baseRunCells COleCtlKeywordSheet
#endif


class CRunCells : public baseRunCells
{
	DECLARE_DYNAMIC(CRunCells)

public:
	CRunCells(CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CRunCells();

	virtual CString GetString();
	virtual CString GetLine(StorageBinListItem& item);

	virtual void Edit(CString& rStr);

	CRunCellsPg1 m_Page1;

protected:
	DECLARE_MESSAGE_MAP()
};


