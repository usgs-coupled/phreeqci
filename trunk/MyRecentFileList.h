// MyRecentFileList.h: interface for the CMyRecentFileList class.
//
// $Id$
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYRECENTFILELIST_H__28DB3A90_D48A_457B_A79E_5EF08FFDCB1B__INCLUDED_)
#define AFX_MYRECENTFILELIST_H__28DB3A90_D48A_457B_A79E_5EF08FFDCB1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxadv.h>

class CMyRecentFileList : public CRecentFileList  
{
public:
	CMyRecentFileList(UINT nStart, LPCTSTR lpszSection,
		LPCTSTR lpszEntryFormat, int nSize,
		int nMaxDispLen = AFX_ABBREV_FILENAME_LEN);

	virtual ~CMyRecentFileList();

// Operations
	virtual void UpdateMenu(CCmdUI* pCmdUI);

};

#endif // !defined(AFX_MYRECENTFILELIST_H__28DB3A90_D48A_457B_A79E_5EF08FFDCB1B__INCLUDED_)
