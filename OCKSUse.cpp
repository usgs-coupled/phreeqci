// OCKSUse.cpp : implementation file
//
// $Date: 2001/09/14 20:12:03 $
// $Revision: 1.1.1.27 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "OCKSUse.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COCKSUse

IMPLEMENT_DYNAMIC(COCKSUse, baseOCKSUse)

COCKSUse::COCKSUse(CWnd* pWndParent, CTreeCtrlNode simNode)
	 : baseOCKSUse(IDS_PROPSHT_CAPTION2, pWndParent), m_Page1(simNode)
{
	// Add all of the property pages here.  Note that
	// the order that they appear in here will be
	// the order they appear in on screen.  By default,
	// the first page of the set is the active one.
	// One way to make a different property page the 
	// active one is to call SetActivePage().

	AddPage(&m_Page1);

	ASSERT(simNode && simNode.GetLevel() == CTreeCtrlIn::SimLevel);

	CString strKey;
	CTreeCtrlNode keyNode = simNode.GetChild();

	// make list of USE keyword nodes
	// and parse for m_Page1
	ASSERT(m_listUseNodes.empty());
	for ( ; keyNode != 0 ; keyNode = keyNode.GetNextSibling())
	{
		if (keyNode.GetImageID() == CTreeCtrlIn::useImage)
		{
			m_listUseNodes.push_back(keyNode);
			strKey = keyNode.GetText();
			ParseString(strKey);
		}
	}
}

COCKSUse::~COCKSUse()
{
}


BEGIN_MESSAGE_MAP(COCKSUse, baseOCKSUse)
	//{{AFX_MSG_MAP(COCKSUse)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COCKSUse message handlers

CString COCKSUse::GetString()
{
	/*
	Line 0a: USE equilibrium_phases none
	Line 0b: USE exchange 2
	Line 0c: USE gas_phase 3
	Line 0d: USE kinetics 1
	Line 0e: USE mix 1
	Line 0f: USE reaction 2
	Line 0g: USE reaction_temperature 1
	Line 0h: USE solid_solution 6
	Line 0i: USE solution 1
	Line 0j: USE surface 1
	*/
	CString strLines;
	CString strLine;
	for (int i = CKPUsePg1::FIRST_ROW; i < CKPUsePg1::LAST_ROW; ++i)
	{
		int nNum = m_Page1.m_arrNums[i];
		if (nNum != CKPUsePg1::N_EMPTY)
		{
			if (nNum == CKPUsePg1::N_NONE)
			{
				strLine.Format(_T("USE %s none%s"),
					(LPCTSTR)CKPUsePg1::s_arrStrKeys[i],
					(LPCTSTR)s_strNewLine
					);
			}
			else
			{
				strLine.Format(_T("USE %s %d%s"),
					(LPCTSTR)CKPUsePg1::s_arrStrKeys[i],
					nNum,
					(LPCTSTR)s_strNewLine
					);
			}
			strLines += strLine;
		}
	}
	return strLines;
}

void COCKSUse::Edit(CString& rStr)
{
	UNUSED_ALWAYS(rStr);
	// handled in ctor
}

void COCKSUse::Edit(const CTreeCtrlNode& rKeyNode)
{
	ASSERT(m_editNode == 0);
	m_editNode = rKeyNode;
}

void COCKSUse::ParseString(const CString& str)
{
	CString strKey(str);
	CString strCut;
	int n_user;

	// remove use and whitespace
	strCut = strKey.SpanExcluding(_T(" \t#"));
	strKey = strKey.Mid(strCut.GetLength());
	strKey.TrimLeft();

	// get keyword after USE (ie solution)
	strCut = strKey.SpanExcluding(_T(" \t#"));

	// remove keyword (ie solution)
	strKey = strKey.Mid(strCut.GetLength());

	// remove whitespace
	strKey.TrimLeft();
	if (strKey.IsEmpty())
	{
		n_user = 1;
	}
	else if (strKey[0] == _T('n') || strKey[0] == _T('N'))
	{
		n_user = CKPUsePg1::N_NONE;
	}
	else
	{
		ASSERT( ::_istdigit(strKey[0]) );
		VERIFY( ::_stscanf(strKey, _T("%d"), &n_user) );
	}

	// find keyword type and set n_user
	CKeyword::type nType = CKeyword::GetKeywordType(strCut);
	for (int i = CKPUsePg1::FIRST_ROW; i < CKPUsePg1::LAST_ROW; ++i)
	{
		if (nType == CKPUsePg1::s_arrKeyTypes[i])
		{
			m_Page1.m_arrNums[i] = n_user;
			break;
		}
	}
}

int COCKSUse::DoModal() 
{
	int nReturn = baseOCKSUse::DoModal();

	// Note: nodes will be removed by CTreeCtrlIn
	if (nReturn == IDOK && m_editNode != 0)
	{
		// find edit node and remove from list
		// so that CTreeCtrlIn doesn't attempt to remove it twice
		std::list<CTreeCtrlNode>::iterator nodeIter = m_listUseNodes.begin();
		for (; nodeIter != m_listUseNodes.end(); ++nodeIter)
		{
			if (m_editNode == (*nodeIter))
			{
				m_listUseNodes.erase(nodeIter);
				break;
			}
		}
	}
	return nReturn;
}
