// OCKSCopy.cpp : implementation file
//

#include "stdafx.h"
#include "phreeqci2.h"
#include "OCKSCopy.h"

extern "C"
{
	int read_number_description (char *ptr, int *n_user, int *n_user_end, char **description);
	void PHRQ_free( void *ptr );
}

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COCKSCopy

IMPLEMENT_DYNAMIC(COCKSCopy, baseOCKSCopy)

COCKSCopy::COCKSCopy(CWnd* pWndParent, CTreeCtrlNode simNode)
	 : baseOCKSCopy(IDS_PROPSHT_CAPTION_121, pWndParent), m_Page1(simNode)
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

	// make list of COPY keyword nodes
	// and parse for m_Page1
	ASSERT(m_listCopyNodes.empty());
	for ( ; keyNode != 0 ; keyNode = keyNode.GetNextSibling())
	{
		if (keyNode.GetImageID() == CTreeCtrlIn::copyImage)
		{
			m_listCopyNodes.push_back(keyNode);
			strKey = keyNode.GetText();
			ParseString(strKey);
		}
	}
}


// COMMENT: {9/20/2004 7:04:35 PM}COCKSCopy::COCKSCopy(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
// COMMENT: {9/20/2004 7:04:35 PM}	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
// COMMENT: {9/20/2004 7:04:35 PM}{
// COMMENT: {9/20/2004 7:04:35 PM}}
// COMMENT: {9/20/2004 7:04:35 PM}
// COMMENT: {9/20/2004 7:04:35 PM}COCKSCopy::COCKSCopy(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
// COMMENT: {9/20/2004 7:04:35 PM}	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
// COMMENT: {9/20/2004 7:04:35 PM}{
// COMMENT: {9/20/2004 7:04:35 PM}}

COCKSCopy::~COCKSCopy()
{
}


BEGIN_MESSAGE_MAP(COCKSCopy, baseOCKSCopy)
	//{{AFX_MSG_MAP(COCKSCopy)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COCKSCopy message handlers

CString COCKSCopy::GetString()
{
	// COPY keyword index index_start[-index_end]
	//
	CString strLines;
	CString strLine;
	CString strKey;
	std::list< std::vector<int> >::iterator iter = this->m_Page1.m_listCopies.begin();
	for (; iter != this->m_Page1.m_listCopies.end(); ++iter)
	{
		strKey = CKeyword::GetString((CKeyword::type)(*iter)[0]);
		strKey.MakeLower();

		if ((*iter)[2] == (*iter)[3])
		{
			strLine.Format(_T("COPY %s %d %d%s"),
				(LPCTSTR)strKey,
				(*iter)[1],
				(*iter)[2],
				(LPCTSTR)s_strNewLine
				);
		}
		else
		{
			strLine.Format(_T("COPY %s %d %d-%d%s"),
				(LPCTSTR)strKey,
				(*iter)[1],
				(*iter)[2],
				(*iter)[3],
				(LPCTSTR)s_strNewLine
				);
		}
		strLines += strLine;
	}
	return strLines;
}

void COCKSCopy::Edit(CString& rStr)
{
	UNUSED_ALWAYS(rStr);
	// handled in ctor
}

void COCKSCopy::Edit(const CTreeCtrlNode& rKeyNode)
{
	ASSERT(m_editNode == 0);
	m_editNode = rKeyNode;
}

void COCKSCopy::ParseString(const CString& str)
{
	//
	// COPY keyword index index_start[-index_end]
	//

	CString strKey(str);

	// remove COPY and whitespace
	CString strCut = strKey.SpanExcluding(" \t#");
	strKey = strKey.Mid(strCut.GetLength());
	strKey.TrimLeft();
	ASSERT(CKeyword::GetKeywordType(strCut) == CKeyword::K_COPY);

	// remove keyword and whitespace
	strCut = strKey.SpanExcluding(" \t#");
	strKey = strKey.Mid(strCut.GetLength());
	strKey.TrimLeft();

	// determine keyword type
	CKeyword::type nType = CKeyword::GetKeywordType(strCut);
	switch (nType)				
	{
	case CKeyword::K_SOLUTION:
	case CKeyword::K_MIX:
	case CKeyword::K_KINETICS:
	case CKeyword::K_REACTION:
	case CKeyword::K_REACTION_TEMPERATURE:
	case CKeyword::K_EQUILIBRIUM_PHASES:
	case CKeyword::K_EXCHANGE:
	case CKeyword::K_SURFACE:
	case CKeyword::K_GAS_PHASE:
	case CKeyword::K_SOLID_SOLUTIONS:
		break;
	default:
		ASSERT(FALSE);
	}

	// remove index and whitespace
	int index;
	strCut = strKey.SpanExcluding(" \t#");
	strKey = strKey.Mid(strCut.GetLength());
	strKey.TrimLeft();
	ASSERT( ::_istdigit(strCut[0]) );
	VERIFY( ::_stscanf(strCut, _T("%d"), &index) );


	// call phreeqc method
	int index_start;
	int index_end;
	char* description;
	strCut = _T("solution ");
	strCut += strKey;
	::read_number_description(strCut.GetBuffer(strCut.GetLength()),
		&index_start, &index_end, &description);
	::PHRQ_free(description);
	strCut.ReleaseBuffer();

	std::vector<int> intvec;
	intvec.resize(4);
	intvec[0] = nType;
	intvec[1] = index;
	intvec[2] = index_start;
	intvec[3] = index_end;

	this->m_Page1.m_listCopies.push_back(intvec);
}
