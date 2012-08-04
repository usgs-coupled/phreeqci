// RichDocIn.cpp : implementation of the CRichDocIn class
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
extern CPhreeqciApp theApp;

#include "RichDocIn.h"
#include "RichInCntrItem.h"

#include "RunDlg2.h"
#include "RichDocInSheet.h"

#include "MainFrame.h"
#include "Util.h"

#include "UpdateObject.h"

#include "RichDocDB.h"
#include "RichViewDB.h"
#include "ChildFrame.h"


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
	if (CWinApp *pApp = ::AfxGetApp())
	{
		((CPhreeqciApp*)pApp)->CloseAssocDB(this);
	}
}

BOOL CRichDocIn::OnNewDocument()
{
	if (!CRichEditDoc::OnNewDocument())
		return FALSE;

	// add reinitialization code here
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
		// add storing code here
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
	if (GetPathName().IsEmpty())
	{
		if (AfxMessageBox(IDS_MUST_SAVE_133, MB_OKCANCEL) == IDCANCEL)
		{
			return;
		}
		else
		{
			if (!DoFileSave())
			{
				return;
			}
		}
	}

	ASSERT( !GetPathName().IsEmpty() );

	if (m_props.m_strOutPathName.IsEmpty())
	{
		// Create default output file name
		m_props.m_strOutPathName = CUtil::SetFileExtension(GetPathName(), _T("pqo"));
	}

	CRunDlg2 dlg;
	dlg.m_props          = m_props;
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
	// Add your specialized code here and/or call the base class

	CRichEditDoc::OnChangedViewList();
}

void CRichDocIn::OnUpdateWindowNew(CCmdUI* pCmdUI)
{
	// Add your command update UI handler code here

	// Do not allow a new window for input files
	pCmdUI->Enable(FALSE);
}

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
	// Add your command handler code here
	CRichEditDoc::OnFileClose();
}


BOOL CRichDocIn::OnSaveDocument(LPCTSTR lpszPathName)
{
	// Add your specialized code here and/or call the base class
	
	return CRichEditDoc::OnSaveDocument(lpszPathName);
}

void CRichDocIn::UpdateFrameCounts()
{
	CRichEditDoc::UpdateFrameCounts();
}

