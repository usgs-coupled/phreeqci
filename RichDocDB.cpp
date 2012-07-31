// RichDocDB.cpp : implementation file
//

#include "stdafx.h"
#include "phreeqci2.h"
#include "RichDocDB.h"


// CRichDocDB

IMPLEMENT_DYNCREATE(CRichDocDB, CRichEditDoc)

CRichDocDB::CRichDocDB()
{
}

BOOL CRichDocDB::OnNewDocument()
{
	if (!CRichEditDoc::OnNewDocument())
		return FALSE;
	return TRUE;
}

CRichDocDB::~CRichDocDB()
{
}


BEGIN_MESSAGE_MAP(CRichDocDB, CRichEditDoc)
END_MESSAGE_MAP()


// CRichDocDB diagnostics

#ifdef _DEBUG
void CRichDocDB::AssertValid() const
{
	CRichEditDoc::AssertValid();
}

#ifndef _WIN32_WCE
void CRichDocDB::Dump(CDumpContext& dc) const
{
	CRichEditDoc::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CRichDocDB serialization

void CRichDocDB::Serialize(CArchive& ar)
{
	//{{
	// Calling the base class CRichEditDoc enables serialization
	//  of the container document's COleClientItem objects.
	// set CRichEditDoc::m_bRTF = FALSE if you are serializing as text
	CRichEditDoc::m_bRTF = FALSE;
	CRichEditDoc::Serialize(ar);
	//}}

	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}
#endif

CRichEditCntrItem* CRichDocDB::CreateClientItem(REOBJECT* preo) const
{
	// cast away constness of this
	return new CRichEditCntrItem(preo, (CRichDocDB*) this);
}


// CRichDocDB commands
