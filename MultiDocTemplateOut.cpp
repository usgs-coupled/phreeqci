// MultiDocTemplateOut.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "MultiDocTemplateOut.h"

BOOL AFXAPI AfxComparePath(LPCTSTR lpszPath1, LPCTSTR lpszPath2);
#include "RichDocOut.h"
#include "Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMultiDocTemplateOut

IMPLEMENT_DYNAMIC(CMultiDocTemplateOut, baseMultiDocTemplateOut)

CMultiDocTemplateOut::CMultiDocTemplateOut(UINT nIDResource, CRuntimeClass* pDocClass,
	CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass)
		: baseMultiDocTemplateOut(nIDResource, pDocClass, pFrameClass, pViewClass)
{
}

CMultiDocTemplateOut::~CMultiDocTemplateOut()
{
}


BEGIN_MESSAGE_MAP(CMultiDocTemplateOut, baseMultiDocTemplateOut)
	//{{AFX_MSG_MAP(CMultiDocTemplateOut)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CDocTemplate::Confidence CMultiDocTemplateOut::MatchDocType(LPCTSTR lpszPathName,
	CDocument*& rpDocMatch)
{
	ASSERT(lpszPathName != NULL);
	rpDocMatch = NULL;

	// go through all documents
	POSITION pos = GetFirstDocPosition();
	while (pos != NULL)
	{
		CDocument* pDoc = GetNextDoc(pos);
#ifndef USE_ORIGINAL
		ASSERT( pDoc->IsKindOf(RUNTIME_CLASS(CRichDocOut)) );
#endif
		if (AfxComparePath(pDoc->GetPathName(), lpszPathName))
		{
			// already open
#ifndef USE_ORIGINAL
			pDoc->OnCloseDocument();
			return yesAttemptNative; // extension matches, looks like ours
#endif
			rpDocMatch = pDoc;
			return yesAlreadyOpen;
		}
	}

	// see if it matches our default suffix
	CString strFilterExt;
	if (GetDocString(strFilterExt, CDocTemplate::filterExt) &&
	  !strFilterExt.IsEmpty())
	{
		// see if extension matches
		ASSERT(strFilterExt[0] == '.');
		LPCTSTR lpszDot = _tcsrchr(lpszPathName, '.');
		if (lpszDot != NULL && lstrcmpi(lpszDot, strFilterExt) == 0)
			return yesAttemptNative; // extension matches, looks like ours
	}
#ifndef USE_ORIGINAL
	// check for binary file
	if (CUtil::IsFileBinary(lpszPathName))
	{
		return noAttempt;
	}
	// see if file contains "Reading data base"
	if (CUtil::IsFileOutput(lpszPathName))
	{
		ASSERT( rpDocMatch == NULL );
		return yesAttemptNative;
	}
#endif

	// otherwise we will guess it may work
	return yesAttemptForeign;
}

/////////////////////////////////////////////////////////////////////////////
// CMultiDocTemplateOut message handlers


