// RichDocIn.cpp : implementation of the CRichDocIn class
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
extern CPhreeqciApp theApp;

#include "RichDocIn.h"
#include "RichInCntrItem.h"

#include "RunDlg.h"
#include "RichDocInSheet.h"

#include "MainFrame.h"
#include "Util.h"

#include "UpdateObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ID_KEY_FIRST ID_KEY_END
#define ID_KEY_LAST  ID_KEY_SOLUTION_SPREAD


/////////////////////////////////////////////////////////////////////////////
// CRichDocIn

IMPLEMENT_DYNCREATE(CRichDocIn, CRichEditDoc)

BEGIN_MESSAGE_MAP(CRichDocIn, CRichEditDoc)
	//{{AFX_MSG_MAP(CRichDocIn)
	ON_COMMAND(ID_RUN_INPUT, OnRunInput)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_NEW, OnUpdateWindowNew)
	ON_COMMAND(ID_VIEW_PROPERTIES, OnViewProperties)
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
	//}}AFX_MSG_MAP
	// Enable default OLE container implementation
	ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_LINKS, CRichEditDoc::OnUpdateEditLinksMenu)
	ON_COMMAND(ID_OLE_EDIT_LINKS, CRichEditDoc::OnEditLinks)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OLE_VERB_FIRST, ID_OLE_VERB_LAST, CRichEditDoc::OnUpdateObjectVerbMenu)
// COMMENT: {8/29/2001 6:22:28 PM}	/*******************************************************
	ON_COMMAND(ID_FILE_SEND_MAIL, OnFileSendMail)
	ON_UPDATE_COMMAND_UI(ID_FILE_SEND_MAIL, OnUpdateFileSendMail)
// COMMENT: {8/29/2001 6:22:30 PM}	*******************************************************/


END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRichDocIn construction/destruction

CRichDocIn::CRichDocIn()
: database(m_database)
{
	m_props.m_pDoc = this;
}

CRichDocIn::~CRichDocIn()
{
}

BOOL CRichDocIn::OnNewDocument()
{
	if (!CRichEditDoc::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	// New files do not contain an error node
	m_props.SetDefaults(NULL);

	return TRUE;
}

CRichEditCntrItem* CRichDocIn::CreateClientItem(REOBJECT* preo) const
{
	// cast away constness of this
	return new CRichInCntrItem(preo, (CRichDocIn*) this);
}



/////////////////////////////////////////////////////////////////////////////
// CRichDocIn serialization

void CRichDocIn::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		CString strDBLine;
		strDBLine.Format(_T("DATABASE %s\r\n"), m_props.m_strDBPathName);
		ar.GetFile()->Write(strDBLine, strDBLine.GetLength());
	}
	else
	{
		// Note: DATABASE line is removed by CTreeCtrlIn::ParseTree
		GetView()->SetRedraw(FALSE);
	}

	// Calling the base class CRichEditDoc enables serialization
	//  of the container document's COleClientItem objects.
	// set CRichEditDoc::m_bRTF = FALSE if you are serializing as text
	CRichEditDoc::m_bRTF = FALSE;
	CRichEditDoc::Serialize(ar);

	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		/*
		CCheckErrors check(GetView(), m_errNode);
		check.Go();
		*/
		GetView()->SetRedraw(TRUE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CRichDocIn diagnostics

#ifdef _DEBUG
void CRichDocIn::AssertValid() const
{
	CRichEditDoc::AssertValid();
}

void CRichDocIn::Dump(CDumpContext& dc) const
{
	CRichEditDoc::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRichDocIn commands

void CRichDocIn::OnRunInput()
{
	if (GetPathName().IsEmpty()) {
		if (AfxMessageBox(IDS_MUST_SAVE_133, MB_OKCANCEL) == IDCANCEL)
		{
			return;
		}
		//{{
		else
		{
			if (!DoFileSave())
			{
				return;
			}
		}
		//}}
	}
// COMMENT: {12/19/2000 2:16:12 PM}	if (!DoFileSave())
// COMMENT: {12/19/2000 2:16:12 PM}	{
// COMMENT: {12/19/2000 2:16:12 PM}		return;
// COMMENT: {12/19/2000 2:16:12 PM}	}

	ASSERT( !GetPathName().IsEmpty() );

	if (m_props.m_strOutPathName.IsEmpty())
	{
		// Create default output file name
		m_props.m_strOutPathName = CUtil::SetFileExtension(GetPathName(), _T("pqo"));
	}

	CRunDlg dlg;

	// dlg.m_strInput = GetPathName();
	dlg.m_props = m_props;
	dlg.m_bCloseOnFinish = ((CPhreeqciApp*)AfxGetApp())->m_settings.m_bCloseOnFinish;

	if (dlg.DoModal() == IDOK)
	{
		m_props = dlg.m_props;
		AfxGetApp()->OpenDocumentFile(m_props.m_strOutPathName);
		CUpdateObject obj;
		obj.m_str = dlg.m_strError;
		UpdateAllViews(NULL, RICHDOCIN_LOADERRORFILE, &obj);
		TRACE("After Update\n");
	}
}


void CRichDocIn::SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU)
{
	// add it to the input file MRU list
	if (bAddToMRU)
	{
		((CPhreeqciApp*)AfxGetApp())->AddToRecentInFileList(lpszPathName);
	}

	// call base class
	CRichEditDoc::SetPathName(lpszPathName, FALSE);

	//if (bAddToMRU)
	{
		UpdateAllViews(NULL, RICHDOCIN_SET_PATH_NAME, NULL);
		m_props.m_strOutPathName = CUtil::SetFileExtension(lpszPathName, _T("pqo"));
	}
}

void CRichDocIn::DeleteContents()
{
	// Add your specialized code here and/or call the base class
	CRichEditDoc::DeleteContents();
}

BOOL CRichDocIn::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CRichEditDoc::OnOpenDocument(lpszPathName))
		return FALSE;

	// Add your specialized creation code here

	// Note: Serialize is called by the base OnOpenDocument


	m_props.SetDefaults(lpszPathName);


	return TRUE;
}

void CRichDocIn::XProperties::SetDefaults(LPCTSTR lpszPathName)
{
	// Create default output file name
	if (lpszPathName)
		m_strOutPathName = CUtil::SetFileExtension(lpszPathName, _T("pqo"));

	// get default database
	m_strDBPathName = ((CPhreeqciApp*)AfxGetApp())->m_settings.m_strDefDBPathName;
}

void CRichDocIn::OnChangedViewList()
{
	// TODO: Add your specialized code here and/or call the base class

	CRichEditDoc::OnChangedViewList();
}

void CRichDocIn::OnUpdateWindowNew(CCmdUI* pCmdUI)
{
	// Add your command update UI handler code here

	// Do not allow a new window for input files
	pCmdUI->Enable(FALSE);
}

//DEL void CRichDocIn::OnUpdateKey(CCmdUI* pCmdUI)
//DEL {
//DEL 	// TODO: Add your command update UI handler code here
//DEL 	ASSERT( pCmdUI->m_nID >= ID_KEY_FIRST );
//DEL 	ASSERT( pCmdUI->m_nID <= ID_KEY_LAST );
//DEL 
//DEL 	switch (pCmdUI->m_nID)
//DEL 	{
//DEL 	case ID_KEY_TITLE :
//DEL 		pCmdUI->Enable(TRUE);
//DEL 		break;
//DEL 	default:
//DEL 		pCmdUI->Enable(FALSE);
//DEL 		break;
//DEL 	}
//DEL }
//DEL void CRichDocIn::OnKey(UINT nID)
//DEL {
//DEL 	CPropertySheet* pPropertySheet = NULL;
//DEL 	
//DEL 	switch (nID)
//DEL 	{
//DEL 	case ID_KEY_TITLE :
//DEL 		pPropertySheet = new CKSTitle();
//DEL 		break;
//DEL 	}
//DEL 
//DEL 	if (pPropertySheet == NULL)
//DEL 		return;
//DEL 	
//DEL 	if (pPropertySheet->DoModal() == IDOK)
//DEL 	{
//DEL 	}
//DEL 	delete pPropertySheet;
//DEL // COMMENT: {5/3/2000 6:13:20 PM}	This requires DECLARE_DYNCREATE and IMPLEMENT_DYNCREATE
//DEL // COMMENT: {5/3/2000 6:13:20 PM}	CRuntimeClass* pRuntimeClass = NULL;
//DEL // COMMENT: {5/3/2000 6:13:20 PM}
//DEL // COMMENT: {5/3/2000 6:13:20 PM}	switch (nID)
//DEL // COMMENT: {5/3/2000 6:13:20 PM}	{
//DEL // COMMENT: {5/3/2000 6:13:20 PM}	case ID_KEY_TITLE :
//DEL // COMMENT: {5/3/2000 6:13:20 PM}		pRuntimeClass = RUNTIME_CLASS(CKSTitle);
//DEL // COMMENT: {5/3/2000 6:13:20 PM}		break;
//DEL // COMMENT: {5/3/2000 6:13:20 PM}	}
//DEL // COMMENT: {5/3/2000 6:13:20 PM}
//DEL // COMMENT: {5/3/2000 6:13:20 PM}	if (pRuntimeClass == NULL)
//DEL // COMMENT: {5/3/2000 6:13:20 PM}	{
//DEL // COMMENT: {5/3/2000 6:13:20 PM}		return;
//DEL // COMMENT: {5/3/2000 6:13:20 PM}	}
//DEL // COMMENT: {5/3/2000 6:13:20 PM}
//DEL // COMMENT: {5/3/2000 6:13:20 PM}	CPropertySheet* pPropertySheet = (CPropertySheet*)pRuntimeClass->CreateObject();
//DEL // COMMENT: {5/3/2000 6:13:20 PM}	ASSERT( pPropertySheet );
//DEL // COMMENT: {5/3/2000 6:13:20 PM}	ASSERT( pPropertySheet->IsKindOf(RUNTIME_CLASS(CPropertySheet)) );
//DEL // COMMENT: {5/3/2000 6:13:20 PM}
//DEL // COMMENT: {5/3/2000 6:13:20 PM}	if (pPropertySheet->DoModal() == IDOK)
//DEL // COMMENT: {5/3/2000 6:13:20 PM}	{
//DEL // COMMENT: {5/3/2000 6:13:20 PM}
//DEL // COMMENT: {5/3/2000 6:13:20 PM}	}
//DEL // COMMENT: {5/3/2000 6:13:20 PM}	delete pPropertySheet;
//DEL }

void CRichDocIn::OnViewProperties() 
{
	CRichDocInSheet propSheet;
	propSheet.SetTitle(GetTitle(), PSH_PROPTITLE);
	propSheet.m_Page1.m_strPathName = GetPathName();
	propSheet.m_Page1.m_strTitle = GetTitle();
	propSheet.m_Page1.m_pRichView = GetView();
	propSheet.m_Page1.m_props = m_props;

	if (propSheet.DoModal() == IDOK)
	{
		// if user changes database mark the file as dirty
		if (m_props.m_strDBPathName.CompareNoCase(propSheet.m_Page1.m_props.m_strDBPathName) != 0)
		{
			if (!GetPathName().IsEmpty())
			{
				// only set if the file has been saved at least once
				SetModifiedFlag(TRUE);
			}
		}

		// set properties
		m_props = propSheet.m_Page1.m_props;
	}
}

void CRichDocIn::UpdateDatabase(int nSimulation)
{
	m_database.Load(&GetView()->GetRichEditCtrl(), nSimulation);
	m_database.Merge(theApp.GetMergeDatabase(m_props.m_strDBPathName));
}

void CRichDocIn::OnFileClose()
{
	// TODO: Add your command handler code here
	CRichEditDoc::OnFileClose();
}


BOOL CRichDocIn::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CRichEditDoc::OnSaveDocument(lpszPathName);
}

void CRichDocIn::UpdateFrameCounts()
{
	CRichEditDoc::UpdateFrameCounts();
	//UpdateAllViews(NULL, RICHDOCIN_UPDATE_FRAME_COUNTS, NULL);
}

