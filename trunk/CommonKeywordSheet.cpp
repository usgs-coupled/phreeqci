// CommonKeywordSheet.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "CommonKeywordSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCommonKeywordSheet

IMPLEMENT_DYNAMIC(CCommonKeywordSheet, baseCommonKeywordSheet)

CCommonKeywordSheet::CCommonKeywordSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:baseCommonKeywordSheet(nIDCaption, pParentWnd, iSelectPage)
{
	m_n_user = 1;
	m_n_user_end = N_EMPTY;
	m_strDesc = _T("");
}

CCommonKeywordSheet::CCommonKeywordSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:baseCommonKeywordSheet(pszCaption, pParentWnd, iSelectPage)
{
	m_n_user = 1;
	m_n_user_end = N_EMPTY;
	m_strDesc = _T("");
}

CCommonKeywordSheet::~CCommonKeywordSheet()
{
}


BEGIN_MESSAGE_MAP(CCommonKeywordSheet, baseCommonKeywordSheet)
	//{{AFX_MSG_MAP(CCommonKeywordSheet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommonKeywordSheet message handlers


CString CCommonKeywordSheet::GetLineZero(enum CKeyword::type nType)
{
	CString strLineZero;

	CString strKeyword = CKeyword::GetString(nType);
	ASSERT(CKeyword::IsKeyword(strKeyword));

	if (m_n_user_end == N_EMPTY)
	{
		ASSERT(m_n_user != N_EMPTY);
		strLineZero.Format(_T("%s %d %s"),
			(LPCTSTR)strKeyword,
			m_n_user, 
			(LPCTSTR)m_strDesc
			);
	}
	else
	{
		ASSERT(m_n_user <= m_n_user_end);
		strLineZero.Format(_T("%s %d-%d %s"),
			(LPCTSTR)strKeyword,
			m_n_user, 
			m_n_user_end,
			(LPCTSTR)m_strDesc
			);
	}

	strLineZero.TrimRight();

	return strLineZero;
}
