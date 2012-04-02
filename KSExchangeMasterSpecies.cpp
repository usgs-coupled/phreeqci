// KSExchangeMasterSpecies.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "KSExchangeMasterSpecies.h"

#include "KeywordLoader2.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKSExchangeMasterSpecies

IMPLEMENT_DYNAMIC(CKSExchangeMasterSpecies, baseCKSExchangeMasterSpecies)

CKSExchangeMasterSpecies::CKSExchangeMasterSpecies(CWnd* pWndParent)
	 : baseCKSExchangeMasterSpecies(IDS_PROPSHT_CAPTION31, pWndParent)
{
	m_bNeedDatabase = false;
	// Add all of the property pages here.  Note that
	// the order that they appear in here will be
	// the order they appear in on screen.  By default,
	// the first page of the set is the active one.
	// One way to make a different property page the 
	// active one is to call SetActivePage().

	AddPage(&m_Page1);
}

CKSExchangeMasterSpecies::~CKSExchangeMasterSpecies()
{
}


BEGIN_MESSAGE_MAP(CKSExchangeMasterSpecies, baseCKSExchangeMasterSpecies)
	//{{AFX_MSG_MAP(CKSExchangeMasterSpecies)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CKSExchangeMasterSpecies message handlers

CString CKSExchangeMasterSpecies::GetString()
{
	/*
	Line 0: EXCHANGE_MASTER_SPECIES
	Line 1a:     X     X-
	Line 1b:     Xa    Xa-
	*/

	// Line 0
	CString strLines = _T("EXCHANGE_MASTER_SPECIES");
	CString strFormat;

	std::list<CMaster>::const_iterator cIter = m_Page1.m_listMaster.begin();
	for (; cIter != m_Page1.m_listMaster.end(); ++cIter)
	{
		// Line 1: exchange name, exchange master species 
		strFormat.Format(_T("%s%4c%-13s %-13s"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			(LPCTSTR)cIter->m_strName,
			(LPCTSTR)cIter->m_strMasterSpecies
			);
		strLines += strFormat;
	}

	return strLines + s_strNewLine;
}

void CKSExchangeMasterSpecies::Edit(CString& rStr)
{
	PhreeqcI p(rStr);
	p.GetData(this);
}
