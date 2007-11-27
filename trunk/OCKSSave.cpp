// OCKSSave.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "OCKSSave.h"

extern "C"
{
	int read_number_description (char *ptr, int *n_user, int *n_user_end, char **description);
	void PHRQ_free( void *ptr );
}

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COCKSSave

IMPLEMENT_DYNAMIC(COCKSSave, baseOCKSSave)

COCKSSave::COCKSSave(CWnd* pWndParent, CTreeCtrlNode simNode)
	 : baseOCKSSave(IDS_PROPSHT_CAPTION5, pWndParent)
{
	// Add all of the property pages here.  Note that
	// the order that they appear in here will be
	// the order they appear in on screen.  By default,
	// the first page of the set is the active one.
	// One way to make a different property page the 
	// active one is to call SetActivePage().

	AddPage(&m_Page1);

	ASSERT(simNode && simNode.GetLevel() == CTreeCtrlIn::SimLevel);

	// make list of SAVE keyword nodes
	// and parse for m_Page1
	ASSERT(m_listSaveNodes.empty());
	CString strKey;
	CTreeCtrlNode keyNode = simNode.GetChild();
	for ( ; keyNode != 0 ; keyNode = keyNode.GetNextSibling())
	{
		if (keyNode.GetImageID() == CTreeCtrlIn::saveImage)
		{
			m_listSaveNodes.push_back(keyNode);
			strKey = keyNode.GetText();
			ParseString(strKey);
		}
	}

}

COCKSSave::~COCKSSave()
{
}


BEGIN_MESSAGE_MAP(COCKSSave, baseOCKSSave)
	//{{AFX_MSG_MAP(COCKSSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COCKSSave message handlers
CString COCKSSave::GetString()
{
	/*
	Line 0a: SAVE equilibrium_phases 2
	Line 0b: SAVE exchange 2
	Line 0c: SAVE gas_phase 2
	Line 0d: SAVE solid_solution 1
	Line 0e: SAVE solution 2
	Line 0f: SAVE surface 1
	*/
	CString strLines;
	CString strLine;
	CString strKey;
	for (int i = CKPSavePg1::FIRST_ROW; i < CKPSavePg1::LAST_ROW; ++i)
	{
		CRange range = m_Page1.m_arrRange[i];
		if (range.nMin != CKPSavePg1::N_EMPTY)
		{
			strKey = CKeyword::GetString(CKPSavePg1::s_arrKeyTypes[i]);
			strKey.MakeLower();
			if (range.nMax == CKPSavePg1::N_EMPTY)
			{
				strLine.Format(_T("SAVE %s %d%s"),
					(LPCTSTR)strKey,
					range.nMin,
					(LPCTSTR)s_strNewLine
					);
			}
			else
			{
				strLine.Format(_T("SAVE %s %d-%d%s"),
					(LPCTSTR)strKey,
					range.nMin,
					range.nMax,
					(LPCTSTR)s_strNewLine
					);
			}
			strLines += strLine;
		}
	}
	return strLines;
}

void COCKSSave::Edit(CString& rStr)
{
	UNUSED_ALWAYS(rStr);
	// handled in ctor
}

void COCKSSave::Edit(const CTreeCtrlNode& rKeyNode)
{
	ASSERT(m_editNode == 0);
	m_editNode = rKeyNode;
	ASSERT(m_editNode);
}

void COCKSSave::ParseString(CString strKey)
{
	// remove SAVE and whitespace
	CString strCut = strKey.SpanExcluding(" \t#");
	strKey = strKey.Mid(strCut.GetLength());
	strKey.TrimLeft();

	// call phreeqc method
	int n_user, n_user_end;
	char* description;
	read_number_description(strKey.GetBuffer(strKey.GetLength()),
		&n_user, &n_user_end, &description);
	PHRQ_free(description);
	strKey.ReleaseBuffer();

	CRange range(n_user, n_user_end);

	// get keyword after SAVE (ie solution)
	strCut = strKey.SpanExcluding(" \t#");

	// find keyword type and set range
	CKeyword::type nType = CKeyword::GetKeywordType(strCut);
	for (int i = CKPSavePg1::FIRST_ROW; i < CKPSavePg1::LAST_ROW; ++i)
	{
		if (nType == CKPSavePg1::s_arrKeyTypes[i])
		{
			m_Page1.m_arrRange[i] = range;
			break;
		}
	}

}

int COCKSSave::DoModal() 
{
	int nReturn = baseOCKSSave::DoModal();

	// Note: nodes will be removed by CTreeCtrlIn
	if (nReturn == IDOK && m_editNode != 0)
	{
		// find edit node and remove from list
		// so that CTreeCtrlIn doesn't attempt to remove it twice
		std::list<CTreeCtrlNode>::iterator nodeIter = m_listSaveNodes.begin();
		for (; nodeIter != m_listSaveNodes.end(); ++nodeIter)
		{
			if (m_editNode == (*nodeIter))
			{
				m_listSaveNodes.erase(nodeIter);
				break;
			}
		}
	}
	return nReturn;
}
