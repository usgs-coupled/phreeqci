// RichDocOut.cpp : implementation of the CRichDocOut class
//
// $Date: 2000/10/04 20:17:52 $
// $Revision: 1.1.1.29 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"

#include "RichDocOut.h"
#include "RichOutCntrItem.h"

#include "MainFrame.h"
#include "RichEditLineParserSafe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRichDocOut

IMPLEMENT_DYNCREATE(CRichDocOut, CRichEditDoc)

BEGIN_MESSAGE_MAP(CRichDocOut, CRichEditDoc)
	//{{AFX_MSG_MAP(CRichDocOut)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	//}}AFX_MSG_MAP
	// Enable default OLE container implementation
	ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_LINKS, CRichEditDoc::OnUpdateEditLinksMenu)
	ON_COMMAND(ID_OLE_EDIT_LINKS, CRichEditDoc::OnEditLinks)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OLE_VERB_FIRST, ID_OLE_VERB_LAST, CRichEditDoc::OnUpdateObjectVerbMenu)
	/*******************************************************
	ON_COMMAND(ID_FILE_SEND_MAIL, OnFileSendMail)
	ON_UPDATE_COMMAND_UI(ID_FILE_SEND_MAIL, OnUpdateFileSendMail)
	*******************************************************/
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRichDocOut construction/destruction

CRichDocOut::CRichDocOut()
{
	// TODO: add one-time construction code here
}

CRichDocOut::~CRichDocOut()
{
}

BOOL CRichDocOut::OnNewDocument()
{
	ASSERT( FALSE );	// should never be called
	if (!CRichEditDoc::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

CRichEditCntrItem* CRichDocOut::CreateClientItem(REOBJECT* preo) const
{
	// cast away constness of this
	return new CRichOutCntrItem(preo, (CRichDocOut*) this);
}



/////////////////////////////////////////////////////////////////////////////
// CRichDocOut serialization

void CRichDocOut::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		ASSERT( FALSE );
	}
	else
	{
		// TODO: add loading code here
	}

	// Calling the base class CRichEditDoc enables serialization
	//  of the container document's COleClientItem objects.
	// set CRichEditDoc::m_bRTF = FALSE if you are serializing as text
	CRichEditDoc::m_bRTF = FALSE;
	CRichEditDoc::Serialize(ar);

	// Note serialize is not called if CFile::GetLength() == 0
}

/////////////////////////////////////////////////////////////////////////////
// CRichDocOut diagnostics

#ifdef _DEBUG
void CRichDocOut::AssertValid() const
{
	CRichEditDoc::AssertValid();
}

void CRichDocOut::Dump(CDumpContext& dc) const
{
	CRichEditDoc::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRichDocOut commands

void CRichDocOut::OnCloseDocument() 
{
	// Add your specialized code here and/or call the base class

	CRichEditDoc::OnCloseDocument();
}

//////////////////////////////////////////////////////////////////////
// CFillTreeThreadProcParam Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void CRichDocOut::DeleteContents() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CRichEditDoc::DeleteContents();
}

void CRichDocOut::SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU) 
{
	// Add your specialized code here and/or call the base class

	// add it to the output file MRU list
	if (bAddToMRU)
		((CPhreeqciApp*)AfxGetApp())->AddToRecentOutFileList(lpszPathName);

	// call base class
	CRichEditDoc::SetPathName(lpszPathName, FALSE);
}

void CRichDocOut::OnChangedViewList() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateAllViews(NULL, CRichViewOut::viewAddedOrRemoved);
	
	CRichEditDoc::OnChangedViewList();
}

void CRichDocOut::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);
}

void CRichDocOut::OnUpdateFileSaveAs(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);
}
