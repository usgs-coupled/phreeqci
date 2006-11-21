// MultiDocTemplateIn.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "MultiDocTemplateIn.h"

BOOL AFXAPI AfxComparePath(LPCTSTR lpszPath1, LPCTSTR lpszPath2);
#include "RichDocIn.h"
#include "Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMultiDocTemplateIn

IMPLEMENT_DYNAMIC(CMultiDocTemplateIn, baseMultiDocTemplateIn)

CMultiDocTemplateIn::CMultiDocTemplateIn(UINT nIDResource, CRuntimeClass* pDocClass,
	CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass)
		: baseMultiDocTemplateIn(nIDResource, pDocClass, pFrameClass, pViewClass)
{
}

CMultiDocTemplateIn::~CMultiDocTemplateIn()
{
}


BEGIN_MESSAGE_MAP(CMultiDocTemplateIn, baseMultiDocTemplateIn)
	//{{AFX_MSG_MAP(CMultiDocTemplateIn)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CDocTemplate::Confidence CMultiDocTemplateIn::MatchDocType(LPCTSTR lpszPathName,
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
		ASSERT( pDoc->IsKindOf(RUNTIME_CLASS(CRichDocIn)) );
#endif
		if (AfxComparePath(pDoc->GetPathName(), lpszPathName))
		{
			// already open
#ifdef USE_ORIGINAL
			rpDocMatch = pDoc;
			return yesAlreadyOpen;
#else
			// Based on CDocument::SaveModified() and Microsoft Excel 2000
			if (pDoc->IsModified())
			{
				CString strMsg;
				AfxFormatString1(strMsg, IDS_STRING134, CUtil::GetTitleWithExt(lpszPathName));
				switch (AfxMessageBox(strMsg, MB_YESNO))
				{
				case IDYES :
					pDoc->OnCloseDocument();
					return yesAttemptNative;
					break;
				case IDNO :
					rpDocMatch = pDoc;
					return yesAlreadyOpen;
					break;
				}
			}
			else
			{
				pDoc->OnCloseDocument();
				return yesAttemptNative;
			}
#endif
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
#endif

	// otherwise we will guess it may work
	return yesAttemptForeign;
}

/////////////////////////////////////////////////////////////////////////////
// CMultiDocTemplateIn message handlers

