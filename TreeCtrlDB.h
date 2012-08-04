#pragma once


// CTreeCtrlDB
#include "Tree.h"
#include "TreeCtrlKeyword.h"


#ifndef baseCTreeCtrlDB
#define baseCTreeCtrlDB CTreeCtrlKeyword
#endif

class CTreeCtrlDB : public baseCTreeCtrlDB
{
	DECLARE_DYNAMIC(CTreeCtrlDB)

public:
	CTreeCtrlDB();
	virtual ~CTreeCtrlDB();

	CTreeCtrlNode GetFileNode(CRichEditDoc *pDoc);

protected:
	virtual void RemoveDatabaseKeyword(CRichEditDoc *pDoc);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


