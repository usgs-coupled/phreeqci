// MyRecentFileList.cpp: implementation of the CMyRecentFileList class.
//
// $Date: 2000/10/04 20:17:48 $
// $Revision: 1.1.1.25 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "MyRecentFileList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyRecentFileList::CMyRecentFileList(UINT nStart, LPCTSTR lpszSection,
	LPCTSTR lpszEntryFormat, int nSize, int nMaxDispLen)
	: CRecentFileList(nStart, lpszSection, lpszEntryFormat, nSize, nMaxDispLen)
{

}

CMyRecentFileList::~CMyRecentFileList()
{

}

void CMyRecentFileList::UpdateMenu(CCmdUI* pCmdUI)
{
	// This fixes bug which occurs if the recent file menu item is contained
	// within a sub menu.  Without this modification the files will be added
	// to the m_pMenu instead of the m_pSubMenu
	if (pCmdUI->m_pSubMenu)
	{
		// save original
		ASSERT( pCmdUI->m_pMenu );
		CMenu* pOldMenu = pCmdUI->m_pMenu;

		// reset to submenu
		pCmdUI->m_pMenu = pCmdUI->m_pSubMenu;

		// call base class
		CRecentFileList::UpdateMenu(pCmdUI);

		// restore original
		pCmdUI->m_pMenu = pOldMenu;
	}
	else
	{
		// call base class
		CRecentFileList::UpdateMenu(pCmdUI);
	}
}