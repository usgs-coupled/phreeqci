// Util.cpp: implementation of the CUtil class.
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "Util.h"

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>

#include "MainFrame.h"
#include "DefinedRanges.h"
#include "Database.h"
#include "CheckListCtrl.h"
#include "Tree.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CComboBox* CUtil::PrepareCombo(HWND hWndCombo)
{
	CComboBox* pCombo = NULL;
	if (hWndCombo)
	{
		pCombo = (CComboBox*)CWnd::FromHandle(hWndCombo);
		ASSERT_KINDOF(CComboBox, pCombo);	// must be combobox
		pCombo->ResetContent();
	}
	return pCombo;
}

CDC* CUtil::PrepareDC(CWnd* pWnd)
{
	CDC* pDC = (pWnd) ? pWnd->GetDC() : NULL;
	return pDC;
}


bool CUtil::IsFileBinary(LPCTSTR lpszPathName)
{
	CFile file;
	ASSERT(lpszPathName != NULL);

	// file may not exist
	if (!file.Open(lpszPathName, CFile::modeRead | CFile::shareDenyWrite))
	{
		return false;
	}

	CFileStatus stat;
	VERIFY(file.GetStatus(stat));

	if (stat.m_size == 0) // file is empty
	{
		return FALSE;
	}

	const UINT nSize = 3200;

	BYTE buf[nSize] = {0};

	UINT nChar = file.Read(buf, nSize);
	nChar--;

	UINT i = 0;
	while (i++ < nChar)
	{
		if (buf[i] < 9 || (buf[i] > 13 && buf[i] < 32) || buf[i] >= 127)
		{
			file.Close();
			return TRUE;
		}
	}

	file.Close();
	return FALSE;
}

bool CUtil::IsFileOutput(LPCTSTR lpszPathName)
{
	ASSERT( lpszPathName );
	ASSERT( _tcslen(lpszPathName) );

	bool bReturn = false;

	try
	{
		// may throw( CFileException );
		CStdioFile file(lpszPathName, CFile::modeRead | CFile::shareDenyWrite);

		CFileStatus stat;
		VERIFY( file.GetStatus(stat) );

		if (stat.m_size == 0) // file is empty
		{
			// may throw( CFileException );
			file.Close();
			return false;
		}

		CString strLine;
		// look for "Reading data base." within the first 20 lines
		for (int i = 0; i < 20 ; ++i)
		{
			// may throw( CFileException );
			if (!file.ReadString(strLine))
			{
				// may throw( CFileException );
				file.Close();
				break;
			}
			// no simulations have been read when dup_print is called therefore
			// pr.headings == TRUE for all output files
			// if (strLine.Find(_T("Reading data base."), 0) != -1)
			if (_tcsncmp(strLine, _T("Reading data base."), 18) == 0)
			{
				// may throw( CFileException );
				file.Close();
				bReturn = true;
				break;
			}
		}
	}
	catch (CFileException* e)
	{
#ifdef _DEBUG
		e->ReportError();
#endif
		e->Delete();		
	}

	return bReturn;
}

CString CUtil::GetTitleWithExt(LPCTSTR lpszPathName)
{
	TCHAR szDrive[_MAX_DRIVE];
	TCHAR szDir[_MAX_DIR];
	TCHAR szFname[_MAX_FNAME];
	TCHAR szExt[_MAX_EXT];
	TCHAR szOutput[_MAX_PATH];

	_tsplitpath(lpszPathName, szDrive, szDir, szFname, szExt);
	_tmakepath(szOutput, NULL, NULL, szFname, szExt);

	return szOutput;
}

bool CUtil::IsFileWriteable(LPCTSTR szPath)
{
	int fh;

	// check if file exists
	if ( _taccess( szPath, 00 ) == 0 ) {
		// file exists
		fh = _topen( szPath, _O_WRONLY );
		if( fh == -1 ) {
			return false;
		} else {
			_close( fh );
			return true;
		}
	} else {
		// new file
		fh = _topen( szPath, _O_CREAT | _O_TEMPORARY, _S_IWRITE );
		if( fh == -1 ) {
			return false;
		} else {
			_close( fh );
			return true;
		}
	}
}

CString CUtil::SetFileExtension(LPCTSTR lpszPathName, LPCTSTR lpszExt)
{
	// Create default output file name
	TCHAR szDrive[_MAX_DRIVE];
	TCHAR szDir[_MAX_DIR];
	TCHAR szFname[_MAX_FNAME];
	TCHAR szExt[_MAX_EXT];
	TCHAR szOutput[_MAX_PATH];

	ASSERT( lpszPathName );
	ASSERT( _tcslen(lpszPathName) );

	_tsplitpath(lpszPathName, szDrive, szDir, szFname, szExt);
	_tmakepath(szOutput, szDrive, szDir, szFname, lpszExt);

	return szOutput;
}

CStatusBar& CUtil::GetStatusBar()
{
	return GetMainFrame().m_wndStatusBar;
}

CMainFrame& CUtil::GetMainFrame()
{
	CMainFrame* pMainFrame = (CMainFrame*)::AfxGetMainWnd();
	ASSERT_KINDOF(CMainFrame, pMainFrame);
	return (*pMainFrame);
}

//
// Fills rNumSet with the numbers contained within rRangeSet
//
CString CUtil::CreateRange(std::set<CDBRange>& rNumSet, const std::set<CRange>& rRangeSet)
{
	// find largest number
	int nMax = -1;
	std::set<CRange>::const_iterator constIter = rRangeSet.begin();
	for (; constIter != rRangeSet.end(); ++constIter)
	{
		nMax = max(nMax, (*constIter).nMax);
	}

	// default format
	CString strNumFormat(_T("%d"));

	// nothing to do
	if (nMax < 0) 
		return strNumFormat;

	if (nMax != 0)
	{
		// determine width of largest number
		int width = (int)log10((double)nMax) + 1;
		strNumFormat.Format(_T("%%%dd"), width);
	}

	CString str;
	for (constIter = rRangeSet.begin(); constIter != rRangeSet.end(); ++constIter)
	{
		std::set<CDBRange>::iterator p = rNumSet.begin();
		for (int nStart = (*constIter).nMin; nStart <= (*constIter).nMax; ++nStart)
		{
			str.Format(strNumFormat, nStart);
			p = rNumSet.insert(p, str);
		}
	}
	return strNumFormat;
}

//
// Clears the ComboBox and adds each CDBRange.m_strName to the
// ComboBox list
//
int CUtil::InsertRange(HWND hWndCombo, std::set<CDBRange>& rNumSet)
{
	// 08/06/2001 -- Modified to handle hWndCombo = NULL
	CComboBox* pCombo = PrepareCombo(hWndCombo);
	CDC* pDC = PrepareDC(pCombo);
	CSize size(0, 0);
	long nWidest = 0;

	// use InitStorage if list is greater than 90
	std::set<CDBRange>::size_type nSize = rNumSet.size();
	if (nSize > 90)
	{
		if (pCombo) pCombo->InitStorage((int)nSize, 5 * sizeof(TCHAR));
	}
	std::set<CDBRange>::iterator objectIter = rNumSet.begin();
	for (; objectIter != rNumSet.end(); ++objectIter)
	{
		if (pCombo)
		{
			pCombo->AddString((*objectIter).m_strName);

			size = pDC->GetTextExtent((*objectIter).m_strName);
			if (nWidest < size.cx )
			{
				nWidest = size.cx;
			}
		}
		else
		{
			return 1; // at least one item
		}
	}
	if (pCombo)
	{
		pCombo->SetDroppedWidth(nWidest);
		return pCombo->GetCount();
	}
	return 0;
}

int CUtil::InsertRedox(HWND hWndCombo, const CDatabase& rDatabase)
{
	// 08/06/2001 -- Modified to handle hWndCombo = NULL
	CComboBox* pCombo = PrepareCombo(hWndCombo);
	CDC* pDC = PrepareDC(pCombo);
	CSize size(0, 0);
	long nWidest = 0;

	// use InitStorage if list is greater than 90
	std::set<CDBRedox>::size_type nSize = rDatabase.m_redoxSet.size();
	if (nSize > 90)
	{
		if (pCombo) pCombo->InitStorage((int)nSize, 5 * sizeof(TCHAR));
	}
	std::set<CDBRedox>::const_iterator redoxIter = rDatabase.m_redoxSet.begin();
	for (; redoxIter != rDatabase.m_redoxSet.end(); ++redoxIter)
	{
		if (pCombo)
		{
			pCombo->AddString((*redoxIter).m_strName);

			size = pDC->GetTextExtent((*redoxIter).m_strName);
			if (nWidest < size.cx )
			{
				nWidest = size.cx;
			}
		}
		else
		{
			return 1; // at least one item
		}
	}
	if (pCombo)
	{
		pCombo->SetDroppedWidth(nWidest);
		return pCombo->GetCount();
	}
	return 0;
}

int CUtil::InsertPhases(HWND hWndCombo, const CDatabase& rDatabase)
{
	// 08/06/2001 -- Modified to handle hWndCombo = NULL
	CComboBox* pCombo = PrepareCombo(hWndCombo);
	CDC* pDC = PrepareDC(pCombo);
	CSize size(0, 0);
	long nWidest = 0;

	// use InitStorage if list is greater than 90
	std::set<CDBPhase>::size_type nSize = rDatabase.m_phaseSet.size();
	if (nSize > 90)
	{
		if (pCombo) pCombo->InitStorage((int)nSize, 5 * sizeof(TCHAR));
	}
	std::set<CDBPhase>::const_iterator phaseIter = rDatabase.m_phaseSet.begin();
	for (; phaseIter != rDatabase.m_phaseSet.end(); ++phaseIter)
	{
		if (pCombo)
		{
			pCombo->AddString((*phaseIter).m_strName);

			size = pDC->GetTextExtent((*phaseIter).m_strName);
			if (nWidest < size.cx )
			{
				nWidest = size.cx;
			}
		}
		else
		{
			return 1; // at least one item
		}
	}
	if (pCombo)
	{
		pCombo->SetDroppedWidth(nWidest);
		return pCombo->GetCount();
	}
	return 0;
}

int CUtil::InsertPhasesExchangeSpecies(HWND hWndCombo, const CDatabase& rDatabase)
{
	// 08/06/2001 -- Modified to handle hWndCombo = NULL
	CComboBox* pCombo = PrepareCombo(hWndCombo);
	CDC* pDC = PrepareDC(pCombo);
	CSize size(0, 0);
	long nWidest = 0;

	// use InitStorage if list is greater than 90
	std::set<CDBPhase>::size_type nSize = rDatabase.m_phaseSet.size();
	if (nSize > 90)
	{
		if (pCombo) pCombo->InitStorage((int)nSize, 5 * sizeof(TCHAR));
	}

	// create set to store names
	std::set<CString> sortedNames;
	std::set<CString>::iterator iterPos = sortedNames.begin();

	std::set<CDBPhase>::const_iterator phaseIter = rDatabase.m_phaseSet.begin();
	for (; phaseIter != rDatabase.m_phaseSet.end(); ++phaseIter)
	{
		iterPos = sortedNames.insert(iterPos, (*phaseIter).m_strName);
	}

	std::set<CDBSpecies>::const_iterator specIter = rDatabase.m_speciesExSet.begin();
	for (; specIter != rDatabase.m_speciesExSet.end(); ++specIter)
	{
		iterPos = sortedNames.insert(iterPos, (*specIter).m_strName);
	}

	// add all names
	std::set<CString>::const_iterator strIter = sortedNames.begin();
	for (; strIter != sortedNames.end(); ++strIter)
	{
		if (pCombo)
		{
			pCombo->AddString((*strIter));

			size = pDC->GetTextExtent((*strIter));
			if (nWidest < size.cx )
			{
				nWidest = size.cx;
			}
		}
		else
		{
			return 1;
		}
	}

	if (pCombo)
	{
		pCombo->SetDroppedWidth(nWidest);
		return pCombo->GetCount();
	}
	return 0;
}

void CUtil::InsertPhasesExchangeSpecies(CCheckListCtrl* pCLC, const CDatabase& rDatabase)
{
	ASSERT_KINDOF(CListCtrl, pCLC);	// must be CListCtrl
	ASSERT(::IsWindow(pCLC->m_hWnd));
	
	VERIFY( pCLC->InsertColumn(0, _T("Phase name")) == 0 );
	VERIFY( pCLC->InsertColumn(1, _T("Formula")) == 1 );

	if (!pCLC) return;

	// create set to store names
	std::set<CString> sortedNames;
	std::set<CString>::iterator iterPos = sortedNames.begin();

	std::set<CDBPhase>::const_iterator phaseIter = rDatabase.m_phaseSet.begin();
	for (; phaseIter != rDatabase.m_phaseSet.end(); ++phaseIter)
	{
		iterPos = sortedNames.insert(iterPos, (*phaseIter).m_strName);
	}

	std::set<CDBSpecies>::const_iterator specIter = rDatabase.m_speciesExSet.begin();
	for (; specIter != rDatabase.m_speciesExSet.end(); ++specIter)
	{
		iterPos = sortedNames.insert(iterPos, (*specIter).m_strName);
	}

	// add all names
	std::set<CString>::const_iterator strIter = sortedNames.begin();
	for (int nItem = 0; strIter != sortedNames.end(); ++strIter)
	{
		if ((nItem = pCLC->InsertItem(nItem + 1, (*strIter))) != -1)
		{
			// add additional cols here
		}
	}
}

void CUtil::InsertSolidSolutions(CCheckListCtrl* pCLC, const CDatabase& rDatabase)
{
	ASSERT_KINDOF(CListCtrl, pCLC);	// must be CListCtrl
	ASSERT(::IsWindow(pCLC->m_hWnd));
	if (!pCLC) return;

	// add columns
	VERIFY( pCLC->InsertColumn(0, _T("Component name")) == 0 );

	int nItem;

	std::set<CDBSS>::const_iterator s_sIter = rDatabase.m_solidSolutionSet.begin();
	for (; s_sIter != rDatabase.m_solidSolutionSet.end(); ++s_sIter)
	{
		if ((nItem = pCLC->InsertItem(0, (*s_sIter).m_strName)) != -1)
		{
			// add additional cols here
		}
	}
}

int CUtil::InsertSolidSolutions(HWND hWndCombo, const CDatabase& rDatabase)
{
	// 08/06/2001 -- Modified to handle hWndCombo = NULL
	CComboBox* pCombo = PrepareCombo(hWndCombo);
	CDC* pDC = PrepareDC(pCombo);
	CSize size(0, 0);
	long nWidest = 0;

	// use InitStorage if list is greater than 90
	std::set<CDBSS>::size_type nSize = rDatabase.m_solidSolutionSet.size();
	if (nSize > 90)
	{
		if (pCombo) pCombo->InitStorage((int)nSize, 15 * sizeof(TCHAR));
	}
	std::set<CDBSS>::const_iterator s_sIter = rDatabase.m_solidSolutionSet.begin();
	for (; s_sIter != rDatabase.m_solidSolutionSet.end(); ++s_sIter)
	{
		if (pCombo)
		{
			pCombo->AddString((*s_sIter).m_strName);

			size = pDC->GetTextExtent((*s_sIter).m_strName);
			if (nWidest < size.cx )
			{
				nWidest = size.cx;
			}
		}
		else
		{
			return 1; // at least one item
		}
	}
	if (pCombo)
	{
		pCombo->SetDroppedWidth(nWidest);
		return pCombo->GetCount();
	}
	return 0;
}

void CUtil::InsertSolidSolutions(CTreeCtrl* pTree, const CDatabase& rDatabase, HTREEITEM hParent /*=TVI_ROOT*/)
{
	ASSERT(::IsWindow(pTree->m_hWnd));
	if (!pTree) return;

	// Note must remove prev items before calling

	std::set<CDBSS>::const_iterator s_sIter = rDatabase.m_solidSolutionSet.begin();
	for (; s_sIter != rDatabase.m_solidSolutionSet.end(); ++s_sIter)
	{
		pTree->InsertItem((*s_sIter).m_strName, hParent);
	}
}

void CUtil::InsertSolidSolutions(CListBox* pLB, const CDatabase& rDatabase)
{
	ASSERT(::IsWindow(pLB->m_hWnd));
	if (!pLB) return;

	// remove prev items
	pLB->ResetContent();

	// use InitStorage if list is greater than 90
	std::set<CDBSS>::size_type nSize = rDatabase.m_solidSolutionSet.size();
	if (nSize > 90)
	{
		pLB->InitStorage((int)nSize, 15 * sizeof(TCHAR));
	}
	std::set<CDBSS>::const_iterator s_sIter = rDatabase.m_solidSolutionSet.begin();
	for (; s_sIter != rDatabase.m_solidSolutionSet.end(); ++s_sIter)
	{
		pLB->AddString((*s_sIter).m_strName);
	}
}

void CUtil::InsertSolidSolutionComps(CTreeCtrl* pTree, const CDatabase& rDatabase, HTREEITEM hParent /*=TVI_ROOT*/)
{
	ASSERT(::IsWindow(pTree->m_hWnd));
	if (!pTree) return;

	// Note must remove prev items before calling

	std::set<CString> setStr;

	std::set<CDBSS>::const_iterator s_sIter = rDatabase.m_solidSolutionSet.begin();
	for (; s_sIter != rDatabase.m_solidSolutionSet.end(); ++s_sIter)
	{
		std::list<CDBSSComp>::const_iterator compIter = (*s_sIter).m_listSSComp.begin();
		for (; compIter != (*s_sIter).m_listSSComp.end(); ++compIter)
		{
			setStr.insert(compIter->m_strName);
		}
	}

	std::set<CString>::const_iterator strIter = setStr.begin();
	for (; strIter != setStr.end(); ++strIter)
	{
		pTree->InsertItem(*strIter, hParent);
	}
}

void CUtil::InsertSolidSolutionComps(CListBox* pLB, const CDatabase& rDatabase)
{
	ASSERT(::IsWindow(pLB->m_hWnd));
	if (!pLB) return;

	// remove prev items
	pLB->ResetContent();

	// use InitStorage if list is greater than 90
	std::set<CDBSS>::size_type nSize = rDatabase.m_solidSolutionSet.size();
	if (nSize > 90)
	{
		pLB->InitStorage((int)nSize, 15 * sizeof(TCHAR));
	}

	std::set<CString> setStr;

	std::set<CDBSS>::const_iterator s_sIter = rDatabase.m_solidSolutionSet.begin();
	for (; s_sIter != rDatabase.m_solidSolutionSet.end(); ++s_sIter)
	{
		std::list<CDBSSComp>::const_iterator compIter = (*s_sIter).m_listSSComp.begin();
		for (; compIter != (*s_sIter).m_listSSComp.end(); ++compIter)
		{
			setStr.insert(compIter->m_strName);
		}
	}

	std::set<CString>::const_iterator strIter = setStr.begin();
	for (; strIter != setStr.end(); ++strIter)
	{
		pLB->AddString(*strIter);
	}
}

void CUtil::InsertPhases(CListBox* pLB, const CDatabase& rDatabase)
{
	ASSERT(::IsWindow(pLB->m_hWnd));
	if (!pLB) return;

	// remove prev items
	pLB->ResetContent();

	// use InitStorage if list is greater than 90
	std::set<CDBPhase>::size_type nSize = rDatabase.m_phaseSet.size();
	if (nSize > 90)
	{
		pLB->InitStorage((int)nSize, 5 * sizeof(TCHAR));
	}
	std::set<CDBPhase>::const_iterator phaseIter = rDatabase.m_phaseSet.begin();
	for (; phaseIter != rDatabase.m_phaseSet.end(); ++phaseIter)
	{
		pLB->AddString((*phaseIter).m_strName);
	}
}

void CUtil::InsertPhases(CTreeCtrl* pTree, const CDatabase& rDatabase, HTREEITEM hParent /*=TVI_ROOT*/)
{
	ASSERT(::IsWindow(pTree->m_hWnd));
	if (!pTree) return;

	// Note must remove prev items before calling

	std::set<CDBPhase>::const_iterator phaseIter = rDatabase.m_phaseSet.begin();
	for (; phaseIter != rDatabase.m_phaseSet.end(); ++phaseIter)
	{
		pTree->InsertItem((*phaseIter).m_strName, hParent);
	}
}

void CUtil::InsertGases(CTreeCtrl* pTree, const CDatabase& rDatabase, HTREEITEM hParent /*=TVI_ROOT*/)
{
	ASSERT(::IsWindow(pTree->m_hWnd));
	if (!pTree) return;

	// Note must remove prev items before calling

	std::set<CDBPhase>::const_iterator phaseIter = rDatabase.m_phaseSet.begin();
	for (; phaseIter != rDatabase.m_phaseSet.end(); ++phaseIter)
	{
		if ( (*phaseIter).m_strName.Find(_T("(g)")) != -1)
		{
			pTree->InsertItem((*phaseIter).m_strName, hParent);
		}
	}
}

void CUtil::InsertGases(CListBox* pLB, const CDatabase& rDatabase)
{
	ASSERT(::IsWindow(pLB->m_hWnd));
	if (!pLB) return;

	// remove prev items
	pLB->ResetContent();

	// use InitStorage if list is greater than 90
	std::set<CDBPhase>::size_type nSize = rDatabase.m_phaseSet.size();
	if (nSize > 90)
	{
		pLB->InitStorage((int)nSize, 5 * sizeof(TCHAR));
	}
	std::set<CDBPhase>::const_iterator phaseIter = rDatabase.m_phaseSet.begin();
	for (; phaseIter != rDatabase.m_phaseSet.end(); ++phaseIter)
	{
		if ( (*phaseIter).m_strName.Find(_T("(g)")) != -1)
		{
			pLB->AddString((*phaseIter).m_strName);
		}
	}
}

int CUtil::InsertGases(HWND hWndCombo, const CDatabase& rDatabase)
{
	// 08/06/2001 -- Modified to handle hWndCombo = NULL
	CComboBox* pCombo = PrepareCombo(hWndCombo);
	CDC* pDC = PrepareDC(pCombo);
	CSize size(0, 0);
	long nWidest = 0;

	// use InitStorage if list is greater than 90
	std::set<CDBPhase>::size_type nSize = rDatabase.m_phaseSet.size();
	if (nSize > 90)
	{
		if (pCombo) pCombo->InitStorage((int)nSize, 5 * sizeof(TCHAR));
	}
	std::set<CDBPhase>::const_iterator phaseIter = rDatabase.m_phaseSet.begin();
	for (; phaseIter != rDatabase.m_phaseSet.end(); ++phaseIter)
	{
		if ( (*phaseIter).m_strName.Find(_T("(g)")) != -1)
		{
			if (pCombo)
			{
				pCombo->AddString((*phaseIter).m_strName);

				size = pDC->GetTextExtent((*phaseIter).m_strName);
				if (nWidest < size.cx )
				{
					nWidest = size.cx;
				}
			}
			else
			{
				return 1; // at least one item
			}
		}
	}
	if (pCombo)
	{
		pCombo->SetDroppedWidth(nWidest);
		return pCombo->GetCount();
	}
	return 0;
}

int CUtil::InsertExchangeSpecies(HWND hWndCombo, const CDatabase& rDatabase)
{
	// 08/06/2001 -- Modified to handle hWndCombo = NULL
	CComboBox* pCombo = PrepareCombo(hWndCombo);
	CDC* pDC = PrepareDC(pCombo);
	CSize size(0, 0);
	long nWidest = 0;

	// use InitStorage if list is greater than 90
	std::set<CDBSpecies>::size_type nSize = rDatabase.m_speciesExSet.size();
	if (nSize > 90)
	{
		if (pCombo) pCombo->InitStorage((int)nSize, 5 * sizeof(TCHAR));
	}

	std::set<CDBSpecies>::const_iterator specIter = rDatabase.m_speciesExSet.begin();
	for (; specIter != rDatabase.m_speciesExSet.end(); ++specIter)
	{
		if (pCombo)
		{
			pCombo->AddString((*specIter).m_strName);

			size = pDC->GetTextExtent((*specIter).m_strName);
			if (nWidest < size.cx )
			{
				nWidest = size.cx;
			}
		}
		else
		{
			return 1; // at least one item
		}
	}
	if (pCombo)
	{
		pCombo->SetDroppedWidth(nWidest);
		return pCombo->GetCount();
	}
	return 0;
}

int CUtil::InsertExchangeSites(HWND hWndCombo, const CDatabase& rDatabase)
{
	// 08/06/2001 -- Modified to handle hWndCombo = NULL
	CComboBox* pCombo = PrepareCombo(hWndCombo);
	CDC* pDC = PrepareDC(pCombo);
	CSize size(0, 0);
	long nWidest = 0;

	// use InitStorage if list is greater than 90
	std::set<CDBElement>::size_type nSize = rDatabase.m_elementSet.size();
	if (nSize > 90)
	{
		if (pCombo) pCombo->InitStorage((int)nSize, 5 * sizeof(TCHAR));
	}

	std::set<CDBElement>::const_iterator elemIter = rDatabase.m_elementSet.begin();
	for (; elemIter != rDatabase.m_elementSet.end(); ++elemIter)
	{
		if ((*elemIter).m_type == CDBElement::typeEX)
		{
			if (pCombo)
			{
				pCombo->AddString((*elemIter).m_strName);

				size = pDC->GetTextExtent((*elemIter).m_strName);
				if (nWidest < size.cx )
				{
					nWidest = size.cx;
				}
			}
			else
			{
				return 1; // at least one item
			}
		}
	}
	if (pCombo)
	{
		pCombo->SetDroppedWidth(nWidest);
		return pCombo->GetCount();
	}
	return 0;
}

int CUtil::InsertConcUnits(HWND hWndCombo, bool bAlk /* = false*/)
{
	// 08/06/2001 -- Modified to handle hWndCombo = NULL
	CComboBox* pCombo = PrepareCombo(hWndCombo);
	CDC* pDC = PrepareDC(pCombo);

	// get units from string resource
	CString strRes;
	VERIFY(strRes.LoadString(IDS_CONC_UNITS_190));

	CSize size(0, 0);
	long nWidest = 0;

	LPTSTR lpsz, lpszUnits;
	lpsz = strRes.LockBuffer();
	lpszUnits = _tcstok(lpsz, _T(";"));
	while (lpszUnits)
	{
		if (pCombo)
		{
			if (bAlk)
			{
				CString str(lpszUnits);
				str.Replace(_T("mol"), _T("eq"));
				pCombo->AddString(str);
				size = pDC->GetTextExtent(str);
			}
			else
			{
				pCombo->AddString(lpszUnits);
				size = pDC->GetTextExtent(lpszUnits);
			}
			if (nWidest < size.cx )
			{
				nWidest = size.cx;
			}
		}
		else
		{
			return 1; // at least one item
		}
		lpszUnits = _tcstok(NULL, _T(";"));
	}
	strRes.UnlockBuffer();
	if (pCombo)
	{
		pCombo->SetDroppedWidth(nWidest);
		return pCombo->GetCount();
	}
	return 0;
}

int CUtil::InsertIsotopes(HWND hWndCombo)
{
	// 08/06/2001 -- Modified to handle hWndCombo = NULL
	CComboBox* pCombo = PrepareCombo(hWndCombo);
	CDC* pDC = PrepareDC(pCombo);
	CSize size(0, 0);
	long nWidest = 0;

	for (int l = 0; l < count_iso_defaults; ++l)
	{
		if (pCombo)
		{
			pCombo->AddString(iso_defaults[l].name);
			size = pDC->GetTextExtent(iso_defaults[l].name);
			if (nWidest < size.cx )
			{
				nWidest = size.cx;
			}
		}
		else
		{
			return 1; // at least one item
		}
	}

	if (pCombo)
	{
		pCombo->SetDroppedWidth(nWidest);
		return pCombo->GetCount();
	}
	return 0;
}

void CUtil::InsertIsotopes(CCheckListCtrl* pCLC)
{
	ASSERT_KINDOF(CListCtrl, pCLC);	// must be CListCtrl
	ASSERT(::IsWindow(pCLC->m_hWnd));
	
	VERIFY( pCLC->InsertColumn(0, _T("Isotope")) == 0 );

// COMMENT: {6/5/2001 6:15:27 PM}	// get isotopes from string resource
// COMMENT: {6/5/2001 6:15:27 PM}	CString strRes;
// COMMENT: {6/5/2001 6:15:27 PM}	VERIFY(strRes.LoadString(IDS_ISOTOPES_191));
// COMMENT: {6/5/2001 6:15:27 PM}
// COMMENT: {6/5/2001 6:15:27 PM}	LPTSTR lpsz, lpszIsotope;
// COMMENT: {6/5/2001 6:15:27 PM}	lpsz = strRes.LockBuffer();
// COMMENT: {6/5/2001 6:15:27 PM}	lpszIsotope = _tcstok(lpsz, _T(";"));
// COMMENT: {6/5/2001 6:15:27 PM}	int nItem = 0;
// COMMENT: {6/5/2001 6:15:27 PM}	while (lpszIsotope)
// COMMENT: {6/5/2001 6:15:27 PM}	{
// COMMENT: {6/5/2001 6:15:27 PM}		VERIFY((nItem = pCLC->InsertItem(nItem + 1, lpszIsotope)) != -1);
// COMMENT: {6/5/2001 6:15:27 PM}		lpszIsotope = _tcstok(NULL, _T(";"));
// COMMENT: {6/5/2001 6:15:27 PM}	}

	int nItem = 0;
	for (int l = 0; l < count_iso_defaults; ++l)
	{
		VERIFY((nItem = pCLC->InsertItem(nItem + 1, iso_defaults[l].name)) != -1);
	}
}

int CUtil::InsertRates(HWND hWndCombo, const CDatabase& rDatabase)
{
	// 08/06/2001 -- Modified to handle hWndCombo = NULL
	CComboBox* pCombo = PrepareCombo(hWndCombo);
	CDC* pDC = PrepareDC(pCombo);
	CSize size(0, 0);
	long nWidest = 0;

	// use InitStorage if list is greater than 90
	std::set<CDBRate>::size_type nSize = rDatabase.m_rateSet.size();
	if (nSize > 90)
	{
		if (pCombo) pCombo->InitStorage((int)nSize, 5 * sizeof(TCHAR));
	}

	std::set<CDBRate>::const_iterator rateIter = rDatabase.m_rateSet.begin();
	for (; rateIter != rDatabase.m_rateSet.end(); ++rateIter)
	{
		if (pCombo)
		{
			pCombo->AddString((*rateIter).m_strName);

			size = pDC->GetTextExtent((*rateIter).m_strName);
			if (nWidest < size.cx )
			{
				nWidest = size.cx;
			}
		}
		else
		{
			return 1; // at least one item
		}
	}
	if (pCombo)
	{
		pCombo->SetDroppedWidth(nWidest);
		return pCombo->GetCount();
	}
	return 0;
}

void CUtil::InsertRates(CListBox* pLB, const CDatabase& rDatabase)
{
	ASSERT(::IsWindow(pLB->m_hWnd));
	if (!pLB) return;

	// remove prev items
	pLB->ResetContent();

	// use InitStorage if list is greater than 90
	std::set<CDBRate>::size_type nSize = rDatabase.m_rateSet.size();
	if (nSize > 90)
	{
		pLB->InitStorage((int)nSize, 10 * sizeof(TCHAR));
	}
	std::set<CDBRate>::const_iterator rateIter = rDatabase.m_rateSet.begin();
	for (; rateIter != rDatabase.m_rateSet.end(); ++rateIter)
	{
		pLB->AddString((*rateIter).m_strName);
	}
}

void CUtil::InsertRates(CTreeCtrl* pTree, const CDatabase& rDatabase, HTREEITEM hParent /*=TVI_ROOT*/)
{
	ASSERT(::IsWindow(pTree->m_hWnd));
	if (!pTree) return;

	// Note must remove prev items before calling

	std::set<CDBRate>::const_iterator rateIter = rDatabase.m_rateSet.begin();
	for (; rateIter != rDatabase.m_rateSet.end(); ++rateIter)
	{
		pTree->InsertItem((*rateIter).m_strName, hParent);
	}
}

void CUtil::InsertGases(CCheckListCtrl* pCLC, const CDatabase& rDatabase)
{
	ASSERT_KINDOF(CListCtrl, pCLC);	// must be CListCtrl
	ASSERT(::IsWindow(pCLC->m_hWnd));

	VERIFY( pCLC->InsertColumn(0, _T("Gas")) == 0 );
	
	std::set<CDBPhase>::const_iterator phaseIter = rDatabase.m_phaseSet.begin();

	int nItem = 0;
	for ( ; phaseIter != rDatabase.m_phaseSet.end(); ++phaseIter )
	{
		if ( (*phaseIter).m_strName.Find(_T("(g)")) != -1)
		{
			if ((nItem = pCLC->InsertItem(nItem + 1, (*phaseIter).m_strName)) != -1)
			{
				// add secondary cols here
			}
		}
	}
}

void CUtil::InsertAqElements(CTreeCtrl* pTree, const CDatabase& rDatabase, HTREEITEM hParent /*=TVI_ROOT*/)
{
	ASSERT(::IsWindow(pTree->m_hWnd));
	if (!pTree) return;

	// Note must remove prev items before calling

	std::set<CDBElement>::const_iterator elemIter = rDatabase.m_elementSet.begin();
	for (; elemIter != rDatabase.m_elementSet.end(); ++elemIter)
	{
		if ((*elemIter).m_type == CDBElement::typeAQ)
		{
			if ( ((*elemIter).m_strName != _T("E")) && ((*elemIter).m_strName != _T("e")     ) &&
				 ((*elemIter).m_strName != _T("H")) && ((*elemIter).m_strName != _T("H(1)")  ) &&
				 ((*elemIter).m_strName != _T("O")) && ((*elemIter).m_strName != _T("O(-2)") )
				 )
			{
				pTree->InsertItem((*elemIter).m_strName, hParent);
			}
		}
	}
}

void CUtil::InsertAqElements(CListBox* pLB, const CDatabase& rDatabase)
{
	ASSERT(::IsWindow(pLB->m_hWnd));
	if (!pLB) return;

	// remove prev items
	pLB->ResetContent();

	// use InitStorage if list is greater than 90
	std::set<CDBElement>::size_type nSize = rDatabase.m_elementSet.size();
	if (nSize > 90)
	{
		pLB->InitStorage((int)nSize, 5 * sizeof(TCHAR));
	}

	std::set<CDBElement>::const_iterator elemIter = rDatabase.m_elementSet.begin();
	for (; elemIter != rDatabase.m_elementSet.end(); ++elemIter)
	{
		if ((*elemIter).m_type == CDBElement::typeAQ)
		{
			if ( ((*elemIter).m_strName != _T("E")) && ((*elemIter).m_strName != _T("e")     ) &&
				 ((*elemIter).m_strName != _T("H")) && ((*elemIter).m_strName != _T("H(1)")  ) &&
				 ((*elemIter).m_strName != _T("O")) && ((*elemIter).m_strName != _T("O(-2)") )
				 )
			{
				pLB->AddString((*elemIter).m_strName);
			}
		}
	}
}

void CUtil::InsertTotals(CCheckListCtrl* pCLC, const CDatabase& rDatabase)
{
	ASSERT_KINDOF(CListCtrl, pCLC);	// must be CListCtrl
	ASSERT(::IsWindow(pCLC->m_hWnd));

	// add columns
	VERIFY( pCLC->InsertColumn(0, _T("Element")        ) == 0 );
	//VERIFY( pCLC->InsertColumn(1, _T("Element name")   ) == 1 );
	//VERIFY( pCLC->InsertColumn(2, _T("Master species") ) == 2 );
	//VERIFY( pCLC->InsertColumn(3, _T("GFW")            ) == 3 );

	CString strFormat;
	int nItem;

	std::set<CDBElement>::const_iterator elemIter = rDatabase.m_elementSet.begin();
	for (; elemIter != rDatabase.m_elementSet.end(); ++elemIter)
	{
		if ((*elemIter).m_type == CDBElement::typeAQ ||
			(*elemIter).m_type == CDBElement::typeEX ||
			(*elemIter).m_type == CDBElement::typeSURF
			)
		{
			if ( ((*elemIter).m_strName != _T("E")) && ((*elemIter).m_strName != _T("e")     ) &&
				 ((*elemIter).m_strName != _T("H")) && ((*elemIter).m_strName != _T("H(1)")  ) &&
				 ((*elemIter).m_strName != _T("O")) && ((*elemIter).m_strName != _T("O(-2)") )
				 )
			{
				if ((nItem = pCLC->InsertItem(0, (*elemIter).m_strName)) != -1)
				{
					// format GFW
					// strFormat.Format(_T("%g"), (*elemIter).m_dGFW);

					// add column items
					//pCLC->SetItemText(nItem, 1, (*elemIter).m_strName);
					//pCLC->SetItemText(nItem, 2, (*elemIter).m_strMaster);
					//pCLC->SetItemText(nItem, 3, strFormat);
				}
			}

		}
	}

	pCLC->SetColumnWidth(0, 65);
	pCLC->ModifyStyle(LVS_SORTASCENDING, 0);
}

int CUtil::InsertTotals(HWND hWndCombo, const CDatabase& rDatabase)
{
	// 08/06/2001 -- Modified to handle hWndCombo = NULL
	CComboBox* pCombo = PrepareCombo(hWndCombo);
	CDC* pDC = PrepareDC(pCombo);
	CSize size(0, 0);
	long nWidest = 0;

	CString strFormat;

	std::set<CDBElement>::const_iterator elemIter = rDatabase.m_elementSet.begin();
	for (; elemIter != rDatabase.m_elementSet.end(); ++elemIter)
	{
		if ((*elemIter).m_type == CDBElement::typeAQ ||
			(*elemIter).m_type == CDBElement::typeEX ||
			(*elemIter).m_type == CDBElement::typeSURF
			)
		{
			if ( ((*elemIter).m_strName != _T("E")) && ((*elemIter).m_strName != _T("e")     ) &&
				 ((*elemIter).m_strName != _T("H")) && ((*elemIter).m_strName != _T("H(1)")  ) &&
				 ((*elemIter).m_strName != _T("O")) && ((*elemIter).m_strName != _T("O(-2)") )
				 )
			{
				if (pCombo)
				{
					pCombo->AddString((*elemIter).m_strName);
					size = pDC->GetTextExtent((*elemIter).m_strName);
					if (nWidest < size.cx )
					{
						nWidest = size.cx;
					}
				}
				else
				{
					return 1; // at least one item
				}
			}

		}
	}
	if (pCombo)
	{
		pCombo->SetDroppedWidth(nWidest);
		return pCombo->GetCount();
	}
	return 0; // no items
}

//
// Insert Aq elements into combobox
//
int CUtil::InsertAqElements(HWND hWndCombo, const CDatabase& rDatabase)
{
	// 08/06/2001 -- Modified to handle hWndCombo = NULL
	CComboBox* pCombo = PrepareCombo(hWndCombo);
	CDC* pDC = PrepareDC(pCombo);
	CSize size(0, 0);
	long nWidest = 0;

	// use InitStorage if list is greater than 90
	std::set<CDBElement>::size_type nSize = rDatabase.m_elementSet.size();
	if (nSize > 90)
	{
		if (pCombo) pCombo->InitStorage((int)nSize, 5 * sizeof(TCHAR));
	}

	std::set<CDBElement>::const_iterator elemIter = rDatabase.m_elementSet.begin();
	for (; elemIter != rDatabase.m_elementSet.end(); ++elemIter)
	{
		if ((*elemIter).m_type == CDBElement::typeAQ)
		{
			if ( ((*elemIter).m_strName != _T("E")) && ((*elemIter).m_strName != _T("e")     ) &&
				 ((*elemIter).m_strName != _T("H")) && ((*elemIter).m_strName != _T("H(1)")  ) &&
				 ((*elemIter).m_strName != _T("O")) && ((*elemIter).m_strName != _T("O(-2)") )
				 )
			{
				if (pCombo)
				{
					pCombo->AddString((*elemIter).m_strName);
					size = pDC->GetTextExtent((*elemIter).m_strName);
					if (nWidest < size.cx )
					{
						nWidest = size.cx;
					}
				}
				else
				{
					return 1; // at least one item
				}
			}
		}
	}

	if (pCombo)
	{
		pCombo->SetDroppedWidth(nWidest);
		return pCombo->GetCount();
	}
	return 0;
}

//
// Insert Aq elements into CCheckListCtrl
//
void CUtil::InsertAqElements(CCheckListCtrl* pCLC, const CDatabase& rDatabase)
{
	ASSERT_KINDOF(CListCtrl, pCLC);	// must be CListCtrl
	ASSERT(::IsWindow(pCLC->m_hWnd));

	// add columns
	VERIFY( pCLC->InsertColumn(0, _T("Element")        ) == 0 );
	VERIFY( pCLC->InsertColumn(1, _T("Element name")   ) == 1 );
	VERIFY( pCLC->InsertColumn(2, _T("Master species") ) == 2 );
	VERIFY( pCLC->InsertColumn(3, _T("GFW")            ) == 3 );

	CString strFormat;
	int nItem;

	std::set<CDBElement>::const_iterator elemIter = rDatabase.m_elementSet.begin();
	for (; elemIter != rDatabase.m_elementSet.end(); ++elemIter)
	{
		if ((*elemIter).m_type == CDBElement::typeAQ)
		{
			if ( ((*elemIter).m_strName != _T("E")) && ((*elemIter).m_strName != _T("e")     ) &&
				 ((*elemIter).m_strName != _T("H")) && ((*elemIter).m_strName != _T("H(1)")  ) &&
				 ((*elemIter).m_strName != _T("O")) && ((*elemIter).m_strName != _T("O(-2)") )
				 )
			{
				if ((nItem = pCLC->InsertItem(0, (*elemIter).m_strName)) != -1)
				{
					// format GFW
					strFormat.Format(_T("%g"), (*elemIter).m_dGFW);

					// add additional cols here
					pCLC->SetItemText(nItem, 1, (*elemIter).m_strName);
					pCLC->SetItemText(nItem, 2, (*elemIter).m_strMaster);
					pCLC->SetItemText(nItem, 3, strFormat);
				}
			}

		}
	}

	pCLC->SetColumnWidth(0, 65);
	pCLC->ModifyStyle(LVS_SORTASCENDING, 0);

	// Rearrange oxidation states
	int cItems = pCLC->GetItemCount();
	CString strItem;
	for (int j = 0; j < cItems; ++j)
	{
		strItem = pCLC->GetItemText(j, 0);
		if (strItem.Find(_T("(")) != -1)
		{
			pCLC->SetItemText(j, 0, pCLC->GetItemText(j, 2));
		}
	}
}

void CUtil::InsertAqSpecies(CListBox* pLB, const CDatabase& rDatabase)
{
	ASSERT(::IsWindow(pLB->m_hWnd));
	if (!pLB || rDatabase.m_speciesSet.size() == 0) return;

	// remove prev items
	pLB->ResetContent();

	// use InitStorage if list is greater than 90
	std::set<CDBSpecies>::size_type nSize = rDatabase.m_speciesSet.size();
	if (nSize > 90)
	{
		pLB->InitStorage((int)nSize, 5 * sizeof(TCHAR));
	}

	std::set<CDBSpecies>::const_iterator specIter = rDatabase.m_speciesSet.begin();
	for (; specIter != rDatabase.m_speciesSet.end(); ++specIter)
	{
		if ((*specIter).m_strName.Compare(_T("e-")) == 0) continue;

		pLB->AddString((*specIter).m_strName);
	}
}

void CUtil::InsertAqSpecies(CTreeCtrl* pTree, const CDatabase& rDatabase, HTREEITEM hParent /*=TVI_ROOT*/)
{
	ASSERT(::IsWindow(pTree->m_hWnd));
	if (!pTree || rDatabase.m_speciesSet.size() == 0) return;

	// Note must remove prev items before calling

	std::set<CDBSpecies>::const_iterator specIter = rDatabase.m_speciesSet.begin();
	for (; specIter != rDatabase.m_speciesSet.end(); ++specIter)
	{
		if ((*specIter).m_strName.Compare(_T("e-")) == 0) continue;
		pTree->InsertItem((*specIter).m_strName, hParent);
	}
}


int CUtil::InsertAqSpecies(HWND hWndCombo, const CDatabase& rDatabase)
{
	// 08/06/2001 -- Modified to handle hWndCombo = NULL
	CComboBox* pCombo = PrepareCombo(hWndCombo);
	CDC* pDC = PrepareDC(pCombo);
	CSize size(0, 0);
	long nWidest = 0;

	// use InitStorage if list is greater than 90
	std::set<CDBSpecies>::size_type nSize = rDatabase.m_speciesSet.size();
	if (nSize > 90)
	{
		if (pCombo) pCombo->InitStorage((int)nSize, 5 * sizeof(TCHAR));
	}

	std::set<CDBSpecies>::const_iterator specIter = rDatabase.m_speciesSet.begin();
	for (; specIter != rDatabase.m_speciesSet.end(); ++specIter)
	{
		if ((*specIter).m_strName.Compare(_T("e-")) == 0) continue;

		if (pCombo)
		{
			pCombo->AddString((*specIter).m_strName);
			size = pDC->GetTextExtent((*specIter).m_strName);
			if (nWidest < size.cx )
			{
				nWidest = size.cx;
			}
		}
		else
		{
			return 1; // at least one item
		}
	}
	if (pCombo)
	{
		pCombo->SetDroppedWidth(nWidest);
		return pCombo->GetCount();
	}
	return 0;
}

int CUtil::InsertCations(HWND hWndCombo, const CDatabase& rDatabase)
{
	CComboBox* pCombo = PrepareCombo(hWndCombo);
	CDC* pDC = PrepareDC(pCombo);
	CSize size(0, 0);
	long nWidest = 0;

	// use InitStorage if list is greater than 90
	std::set<CDBSpecies>::size_type nSize = rDatabase.m_speciesAqCationSet.size();
	if (nSize > 90)
	{
		if (pCombo) pCombo->InitStorage((int)nSize, 5 * sizeof(TCHAR));
	}

	std::set<CDBSpecies>::const_iterator specIter = rDatabase.m_speciesAqCationSet.begin();
	for (; specIter != rDatabase.m_speciesAqCationSet.end(); ++specIter)
	{
		ASSERT((*specIter).m_strName.Compare(_T("e-")) != 0);

		if (pCombo)
		{
			ASSERT((*specIter).m_z > 0);
			pCombo->AddString((*specIter).m_strName);
			size = pDC->GetTextExtent((*specIter).m_strName);
			if (nWidest < size.cx )
			{
				nWidest = size.cx;
			}
		}
		else
		{
			return 1; // at least one item
		}
	}
	if (pCombo)
	{
		pCombo->AddString(_T("H+"));
		pCombo->SetDroppedWidth(nWidest);
		return pCombo->GetCount();
	}
	return 0;
}

int CUtil::InsertAnions(HWND hWndCombo, const CDatabase& rDatabase)
{
	CComboBox* pCombo = PrepareCombo(hWndCombo);
	CDC* pDC = PrepareDC(pCombo);
	CSize size(0, 0);
	long nWidest = 0;

	// use InitStorage if list is greater than 90
	std::set<CDBSpecies>::size_type nSize = rDatabase.m_speciesAqAnionSet.size();
	if (nSize > 90)
	{
		if (pCombo) pCombo->InitStorage((int)nSize, 5 * sizeof(TCHAR));
	}

	std::set<CDBSpecies>::const_iterator specIter = rDatabase.m_speciesAqAnionSet.begin();
	for (; specIter != rDatabase.m_speciesAqAnionSet.end(); ++specIter)
	{
		ASSERT((*specIter).m_strName.Compare(_T("e-")) != 0);

		if (pCombo)
		{
			ASSERT((*specIter).m_z < 0);
			pCombo->AddString((*specIter).m_strName);
			size = pDC->GetTextExtent((*specIter).m_strName);
			if (nWidest < size.cx )
			{
				nWidest = size.cx;
			}
		}
		else
		{
			return 1; // at least one item
		}
	}
	if (pCombo)
	{
		pCombo->SetDroppedWidth(nWidest);
		return pCombo->GetCount();
	}
	return 0;
}

int CUtil::InsertNeutral(HWND hWndCombo, const CDatabase& rDatabase)
{
	CComboBox* pCombo = PrepareCombo(hWndCombo);
	CDC* pDC = PrepareDC(pCombo);
	CSize size(0, 0);
	long nWidest = 0;

	// use InitStorage if list is greater than 90
	std::set<CDBSpecies>::size_type nSize = rDatabase.m_speciesAqNeutralSet.size();
	if (nSize > 90)
	{
		if (pCombo) pCombo->InitStorage((int)nSize, 5 * sizeof(TCHAR));
	}

	std::set<CDBSpecies>::const_iterator specIter = rDatabase.m_speciesAqNeutralSet.begin();
	for (; specIter != rDatabase.m_speciesAqNeutralSet.end(); ++specIter)
	{
		ASSERT((*specIter).m_strName.Compare(_T("e-")) != 0);

		if (pCombo)
		{
			ASSERT((*specIter).m_z == 0);
			pCombo->AddString((*specIter).m_strName);
			size = pDC->GetTextExtent((*specIter).m_strName);
			if (nWidest < size.cx )
			{
				nWidest = size.cx;
			}
		}
		else
		{
			return 1; // at least one item
		}
	}
	if (pCombo)
	{
		pCombo->SetDroppedWidth(nWidest);
		return pCombo->GetCount();
	}
	return 0;
}

int CUtil::InsertCationsAnions(HWND hWndCombo, const CDatabase& rDatabase)
{
	CComboBox* pCombo = PrepareCombo(hWndCombo);
	CDC* pDC = PrepareDC(pCombo);
	CSize size(0, 0);
	long nWidest = 0;

	ASSERT(pCombo);
	ASSERT(pCombo == 0 || (pCombo->GetStyle() & CBS_SORT));

	// use InitStorage if list is greater than 90
	std::set<CDBSpecies>::size_type nSize = rDatabase.m_speciesAqCationSet.size() + rDatabase.m_speciesAqAnionSet.size();
	if (nSize > 90)
	{
		if (pCombo) pCombo->InitStorage((int)nSize, 5 * sizeof(TCHAR));
	}

	std::set<CDBSpecies>::const_iterator specIter = rDatabase.m_speciesAqCationSet.begin();
	for (; specIter != rDatabase.m_speciesAqCationSet.end(); ++specIter)
	{
		ASSERT((*specIter).m_strName.Compare(_T("e-")) != 0);

		if (pCombo)
		{
			ASSERT((*specIter).m_z > 0);
			pCombo->AddString((*specIter).m_strName);
			size = pDC->GetTextExtent((*specIter).m_strName);
			if (nWidest < size.cx )
			{
				nWidest = size.cx;
			}
		}
		else
		{
			return 1; // at least one item
		}
	}
	specIter = rDatabase.m_speciesAqAnionSet.begin();
	for (; specIter != rDatabase.m_speciesAqAnionSet.end(); ++specIter)
	{
		ASSERT((*specIter).m_strName.Compare(_T("e-")) != 0);

		if (pCombo)
		{
			ASSERT((*specIter).m_z < 0);
			pCombo->AddString((*specIter).m_strName);
			size = pDC->GetTextExtent((*specIter).m_strName);
			if (nWidest < size.cx )
			{
				nWidest = size.cx;
			}
		}
		else
		{
			return 1; // at least one item
		}
	}
	if (pCombo)
	{
		pCombo->AddString(_T("H+"));
		pCombo->SetDroppedWidth(nWidest);
		return pCombo->GetCount();
	}
	return 0;
}

void CUtil::InsertAqSpecies(CCheckListCtrl* pCLC, const CDatabase& rDatabase)
{
	ASSERT_KINDOF(CListCtrl, pCLC);	// must be CListCtrl
	ASSERT(::IsWindow(pCLC->m_hWnd));

	if (!pCLC || rDatabase.m_speciesSet.size() == 0) return;

	std::set<CDBSpecies>::const_iterator specIter = rDatabase.m_speciesSet.begin();
	for (int nItem = 0; specIter != rDatabase.m_speciesSet.end(); ++specIter)
	{
		if ((*specIter).m_strName.Compare("e-") == 0) continue;

		if ((nItem = pCLC->InsertItem(nItem + 1, (*specIter).m_strName)) != -1)
		{
			// add additional cols here
		}
	}
}

//
// Insert rates into CCheckListCtrl
//
void CUtil::InsertRates(CCheckListCtrl* pCLC, const CDatabase& rDatabase)
{
	ASSERT_KINDOF(CListCtrl, pCLC);	// must be CListCtrl
	ASSERT(::IsWindow(pCLC->m_hWnd));

	if (!pCLC || rDatabase.m_rateSet.size() == 0) return;
	
	VERIFY( pCLC->InsertColumn(0, _T("Rate name")) == 0 );

	std::set<CDBRate>::const_iterator rateIter = rDatabase.m_rateSet.begin();
	for (int nItem = 0; rateIter != rDatabase.m_rateSet.end(); ++rateIter)
	{
		if ((nItem = pCLC->InsertItem(nItem + 1, (*rateIter).m_strName)) != -1)
		{
			// add additional cols here
		}
	}
}

void CUtil::InsertPhases(CCheckListCtrl* pCLC, const CDatabase& rDatabase)
{
	ASSERT_KINDOF(CListCtrl, pCLC);	// must be CListCtrl
	ASSERT(::IsWindow(pCLC->m_hWnd));

	if (!pCLC || rDatabase.m_phaseSet.size() == 0) return;
	
	VERIFY( pCLC->InsertColumn(0, _T("Phase name")) == 0 );
	VERIFY( pCLC->InsertColumn(1, _T("Formula")) == 1 );

	std::set<CDBPhase>::const_iterator phaseIter = rDatabase.m_phaseSet.begin();
	for (int nItem = 0; phaseIter != rDatabase.m_phaseSet.end(); ++phaseIter)
	{
		if ((nItem = pCLC->InsertItem(nItem + 1, (*phaseIter).m_strName)) != -1)
		{
			// add additional cols here
			pCLC->SetItemText(nItem, 1, (*phaseIter).m_strFormula);
		}
	}
}

//
// Insert range into given CCheckListCtrl
//
void CUtil::InsertRange(CCheckListCtrl* pCLC, std::set<CDBRange>& rNumSet)
{
	ASSERT_KINDOF(CListCtrl, pCLC);	// must be CListCtrl
	ASSERT(::IsWindow(pCLC->m_hWnd));

	if (!pCLC || rNumSet.size() == 0) return;
	
	VERIFY(pCLC->InsertColumn(0, _T("Solutions")) == 0);
	VERIFY(pCLC->SetColumnWidth(0, 60));

	int nItem = 0;
	std::set<CDBRange>::iterator objectIter = rNumSet.begin();
	for (; objectIter != rNumSet.end(); ++objectIter)
	{
		if ((nItem = pCLC->InsertItem(nItem + 1, (*objectIter).m_strName)) != -1)
		{
			// add additional cols here
		}
	}
}

bool CUtil::FileExists(LPCTSTR lpszPathName)
{
#ifdef SAVE_EXPAND_ENVIR
	TCHAR infoBuf[32767];
	DWORD bufCharCount = 32767;
	bufCharCount = ExpandEnvironmentStrings(lpszPathName, infoBuf, 32767); 
#endif
	if (GetFileAttributes(lpszPathName) == -1)
	{
#ifdef _DEBUG
		DWORD dw = GetLastError();
		ASSERT(
			dw == ERROR_FILE_NOT_FOUND ||
			dw == ERROR_PATH_NOT_FOUND ||
			dw == ERROR_INVALID_DRIVE ||
			dw == ERROR_BAD_NETPATH ||
			dw == ERROR_BAD_NET_NAME
			);
#endif
		return false;
	}
	else
	{
		// Note: May want to distinguish between file and directory
		return true;
	}
}

bool CUtil::IsValidName(const CString& strFormula)
{
	if (!strFormula.IsEmpty())
	{
		return ( ::_istupper(strFormula[0]) || strFormula[0] == _T('[') );
	}
	return false;
}

bool CUtil::IsValidExchangerName(const CString& strFormula)
{
	return CUtil::IsValidName(strFormula);
}

bool CUtil::IsValidSurfaceName(const CString& strFormula)
{
	return CUtil::IsValidName(strFormula);
}

bool CUtil::IsValidElementName(const CString& strFormula)
{
	return CUtil::IsValidName(strFormula);
}

bool CUtil::IsValidMasterSpeciesName(const CString& strFormula)
{
	if (strFormula.Compare("e-") == 0)
	{
		return true;
	}
	return CUtil::IsValidName(strFormula);
}

CString CUtil::s_strLastError;

CString CUtil::GetLastErrorString()
{
	return s_strLastError;
}

/* ---------------------------------------------------------------------- */
static int get_elt (char **t_ptr, char *element, int *i)
/* ---------------------------------------------------------------------- */
/*
 *      Function reads an element name out of the equation string.
 *      An element name is composed of a capital letter followed by any number
 *      of lower case characters.
 *
 *      Arguments:
 *         **t_ptr   input, points to position in the equation to begin
 *                   output, points to next character of equation after
 *                      element name.
 *         *element  input pointer to place to return element character string
 */
{
	char c;

	c=*(*t_ptr)++;
	if (c == '\0') {
		////sprintf(error_string, "Empty string in get_elt.  Expected an element name.");
		////error_msg(error_string, CONTINUE);
		return(ERROR);
	}
/*
 *   Load name into char array element
 */
	element[0]=c;
	*i=1;
	if (c == '[') {
		while ((c=(**t_ptr)) != ']') {
			element[*i]=c;
			(*i)++;
			(*t_ptr)++;
			if ((c=(**t_ptr)) == ']') {
				element[*i]=c;
				(*i)++;
				(*t_ptr)++;
				break;
			} else if (**t_ptr == '\0') {
				////error_msg("No ending bracket (]) for element name", CONTINUE);
				////input_error++;
				break;
			}
		}
	} else {
	while (islower((int) (c=(**t_ptr))) || c == '_') {
		element[*i]=c;
		(*i)++;
		(*t_ptr)++;
	}
	}
	element[*i]='\0';
	return(OK);
}


// modified from utilities.c : parse_couple
bool CUtil::IsValidRedoxCouple(const CString& strCouple)
{
	CString strCopy = strCouple;
/*
 *   Parse couple puts redox couples in standard form
 *   "+" is removed and couples are rewritten in sort
 *    order.
 */
	int e1, e2, p1, p2;
	char *ptr;
	char elt1[MAX_LENGTH], elt2[MAX_LENGTH], paren1[MAX_LENGTH], paren2[MAX_LENGTH];

	if (strCopy.CompareNoCase(_T("pe")) == 0)
	{
		return true;
	}

	strCopy.Replace("+", "");
	char token[MAX_LENGTH];
	strcpy(token, strCopy);
	
	ptr=token;

	get_elt( &ptr, elt1, &e1);
	if (*ptr != '(') {
		s_strLastError.Format("Element name must be followed by "
			"parentheses in redox couple, %s.", token);
		return false;
	}
	paren_count=1;
	paren1[0]='(';
	p1=1;
	while ( ptr != '\0') {
		ptr++;
		if (*ptr == '/' || *ptr == '\0') {
			s_strLastError.Format("End of line or  ""/"" encountered before end of parentheses, %s.", token);
			return false;
		}
		paren1[p1++]=*ptr;
		if (*ptr == '(') paren_count++;
		if (*ptr == ')') paren_count--;
		if (paren_count == 0) break;
	}
	paren1[p1]='\0';
	ptr++;
	if (*ptr != '/') {
		s_strLastError.Format(" ""/"" must follow parentheses "
			"ending first half of redox couple, %s.", token);
		return false;
	}
	ptr++;
	get_elt( &ptr, elt2, &e2);
	if (strcmp(elt1,elt2) != 0) {
		s_strLastError.Format("Redox couple must be two redox states "
			"of the same element, %s.", token);
		return false;
	}
	if (*ptr != '(') {
		s_strLastError.Format("Element name must be followed by "
			"parentheses in redox couple, %s.", token);
		return false;
	}
	paren2[0]='(';
	paren_count=1;
	p2=1;
	while ( *ptr != '\0') {
		ptr++;
		if (*ptr == '/' || *ptr == '\0') {
			s_strLastError.Format("End of line or ""/"" encountered"
				" before end of parentheses, %s.", token);
			return false;
		}

		paren2[p2++]=*ptr;
		if (*ptr == '(') paren_count++;
		if (*ptr == ')') paren_count--;
		if (paren_count == 0) break;
	}
	paren2[p2]='\0';
	if (strcmp(paren1,paren2) == 0) {
		s_strLastError.Format("Both parts of redox couple are the same, %s.",token);
		return false;
	}
	return true;
}

void CUtil::InsertNamedExp(CTreeCtrl* pTree, const CDatabase& rDatabase, HTREEITEM hParent /*=TVI_ROOT*/)
{
	ASSERT(::IsWindow(pTree->m_hWnd));
	if (!pTree || rDatabase.m_speciesSet.size() == 0) return;

	// Note must remove prev items before calling

	std::set<CDBNamedExp>::const_iterator namedIter = rDatabase.m_namedExpSet.begin();
	for (; namedIter != rDatabase.m_namedExpSet.end(); ++namedIter)
	{
		pTree->InsertItem((*namedIter).m_strName, hParent);
	}
}

void CUtil::InsertCalcVal(CTreeCtrl* pTree, const CDatabase& rDatabase, HTREEITEM hParent /*=TVI_ROOT*/)
{
	ASSERT(::IsWindow(pTree->m_hWnd));
	if (!pTree || rDatabase.m_speciesSet.size() == 0) return;

	// Note must remove prev items before calling

	std::set<CDBCalcVal>::const_iterator calcValIter = rDatabase.m_calcValSet.begin();
	for (; calcValIter != rDatabase.m_calcValSet.end(); ++calcValIter)
	{
		pTree->InsertItem((*calcValIter).m_strName, hParent);
	}
}

