// TreeCtrlErr.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "TreeCtrlErr.h"

#include "NodeObject.h"

#include "RichDocIn.h"
#include "RichViewIn.h"
#include "ErrorChecker3.h"
#include "RichEditLineParser.h"
#include "RichLogicalLineParser.h"
#include "StdioFileLineParser.h"

#include "WorkspaceBar.h"
#include "Util.h"
#include "SaveCurrentDirectory.h"

#include "UpdateObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlErr

CTreeCtrlErr::CTreeCtrlErr()
{
}

CTreeCtrlErr::~CTreeCtrlErr()
{
}


BEGIN_MESSAGE_MAP(CTreeCtrlErr, baseCTreeCtrlErr)
	//{{AFX_MSG_MAP(CTreeCtrlErr)
	ON_WM_CREATE()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlErr message handlers

int CTreeCtrlErr::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (baseCTreeCtrlErr::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Add your specialized creation code here
	// Create the Image List
	if (!m_ctrlImage.Create(IDB_ERROR_ITEMS, 16, 0, RGB(255,0,255)))
	{
		TRACE0("Failed to create image list\n");
		return -1;
	}

	/// Attach image list to Tree
	SetImageList(&m_ctrlImage, TVSIL_NORMAL);

	// Create base node
	m_baseNode = InsertItem(_T("Errors/Warnings"), workImage, workImage);
	
	return 0;
}

void CTreeCtrlErr::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// Add your message handler code here and/or call default
	TVHITTESTINFO tvHitTestInfo;
	tvHitTestInfo.pt = point;

	CTreeCtrlNode item = HitTest(&tvHitTestInfo);
	CNodeObject nodeObject = HitTest(&tvHitTestInfo);
	CTreeCtrlNode fileNode;

	const int FILE_LEVEL = 1;

	if (item && (tvHitTestInfo.flags & TVHT_ONITEM) && item.GetLevel() >= FILE_LEVEL)
	{
		for (fileNode = item; fileNode.GetLevel() > FILE_LEVEL; )
		{
			fileNode = fileNode.GetParent();
		}

		CDocument* pDoc = (CDocument*) fileNode.GetData();

		if ( pDoc )
		{
			ASSERT( pDoc->IsKindOf(RUNTIME_CLASS(CDocument)) );
			CNodeObject nodeObject(item);
			pDoc->UpdateAllViews(NULL, NODEOBJECT_JUMPTOERROR, &nodeObject);
		}
		else
		{
			baseCTreeCtrlErr::OnLButtonDblClk(nFlags, point);
		}		
	}
	else
	{
		baseCTreeCtrlErr::OnLButtonDblClk(nFlags, point);
	}
}

CTreeCtrlNode CTreeCtrlErr::ParseTree(CRichEditDoc *pDoc)
{
	// sanity checks
	ASSERT(pDoc != NULL);
	ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(CRichDocIn)));
	ASSERT(pDoc->GetView() != NULL);
	ASSERT(pDoc->GetView()->IsKindOf(RUNTIME_CLASS(CRichViewIn)));
	ASSERT(::IsWindow(pDoc->GetView()->m_hWnd));

	// create and set file node
	CTreeCtrlNode fileNode =  InsertItem(pDoc->GetTitle(), fileImage, fileImage, m_baseNode);
	fileNode.SetData((DWORD)pDoc);
	ASSERT((CRichEditDoc*)fileNode.GetData() == pDoc);

	fileNode.AddTail(_T("Checking..."), lineImage);

	CErrorChecker3 errorChecker3(pDoc->GetView()->m_hWnd, fileNode);

	return fileNode;
}

CTreeCtrlNode CTreeCtrlErr::GetFileNode(CRichEditDoc *pDoc)
{
	ASSERT_KINDOF(CRichEditDoc, pDoc);
	CTreeCtrlNode fileNode;
	bool bFound = false;
	for (fileNode = m_baseNode.GetChild(); fileNode != NULL; fileNode = fileNode.GetNextSibling())
	{
		if (pDoc == (CRichEditDoc*)fileNode.GetData())
		{
			bFound = true;
			break;
		}
	}
	if (!bFound)
	{
		fileNode = InsertItem(pDoc->GetTitle(), fileImage, fileImage, m_baseNode);
		fileNode.SetData((DWORD)pDoc);
		ASSERT((CRichEditDoc*)fileNode.GetData() == pDoc);
	}
	return fileNode;
}

int CTreeCtrlErr::CheckErrors(CRichEditDoc *pDoc)
{
	// set hourglass
	CWaitCursor wait;

	// set status bar
	CString strStatus;
	strStatus.Format(_T("Checking %s for errors"), pDoc->GetTitle());
	CUtil::GetStatusBar().SetWindowText(strStatus);

	// sanity checks
	ASSERT(pDoc != NULL);
	ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(CRichDocIn)));
	ASSERT(pDoc->GetView() != NULL);
	ASSERT(pDoc->GetView()->IsKindOf(RUNTIME_CLASS(CRichViewIn)));
	ASSERT(::IsWindow(pDoc->GetView()->m_hWnd));


	// create and set file node
	CTreeCtrlNode fileNode = GetFileNode(pDoc);
	if (fileNode.HasChildren())
	{
		CTreeCtrlNode child = fileNode.GetChild();
		for (;child != NULL; child = fileNode.GetChild())
		{
			VERIFY(child.Delete());
		}
		ASSERT(!fileNode.HasChildren());
	}
	ASSERT((CRichEditDoc*)fileNode.GetData() == pDoc);

	fileNode.AddTail(_T("Checking..."), lineImage);

	CSaveCurrentDirectory save;
	TRACE("%s\n", pDoc->GetPathName());
	if (pDoc->GetPathName().GetLength())
	{
		TCHAR szDrive[_MAX_DRIVE];
		TCHAR szDir[_MAX_DIR];
		TCHAR szFName[_MAX_FNAME];
		TCHAR szExt[_MAX_EXT];
		VERIFY(::_tsplitpath_s(pDoc->GetPathName(), szDrive, _MAX_DRIVE, szDir, _MAX_DIR, szFName, _MAX_FNAME, szExt, _MAX_EXT) == 0);

		TCHAR szNewDir[_MAX_PATH];
		VERIFY(::_tmakepath_s(szNewDir, _MAX_PATH, szDrive, szDir, NULL, NULL) == 0);

		VERIFY(save.SetCurrentDirectory(szNewDir));
	}

	int error_count = 0;
	try
	{
		// let error checker do the work
		CErrorChecker3 errorChecker3(pDoc->GetView()->m_hWnd, fileNode);

		fileNode.Select();
		fileNode.Expand();
		fileNode.EnsureVisible();

		error_count = errorChecker3.GetErrorCount();
	}
	catch (...)
	{
		CString strResource;
		strResource.LoadString(IDS_EXCEPTION_ACCESS_VIOLATION);
		::MessageBox(NULL, strResource, _T("Unhandled Exception"), MB_OK|MB_ICONERROR);
	}

	// reset status bar
	::AfxGetMainWnd()->PostMessage(WM_SETMESSAGESTRING, AFX_IDS_IDLEMESSAGE);

	return error_count;
}

CWorkspaceBar& CTreeCtrlErr::GetWorkspaceBar()
{
	CWorkspaceBar* pBar = (CWorkspaceBar*)GetParent()->GetParent();
	ASSERT_KINDOF(CWorkspaceBar, pBar);
	return (*pBar);
}

void CTreeCtrlErr::UpdateAllViews(CDocument *pDoc, LPARAM lHint, CObject *pHint)
{
	ASSERT_KINDOF(CRichDocIn, pDoc);

	switch (lHint)
	{
	case RICHVIEWIN_EN_CHANGE :
		/// currently handled by CTreeCtrlIn
		TRACE("CTreeCtrlErr RICHVIEWIN_EN_CHANGE\n");
		break;

	case RICHDOCIN_LOADERRORFILE :
		ASSERT_KINDOF(CUpdateObject, pHint);
		TRACE("CTreeCtrlErr RICHDOCIN_LOADERRORFILE\n");
		LoadErrorFile(pDoc, ((CUpdateObject*)pHint)->m_str);
		break;

	case RICHDOCIN_SET_PATH_NAME:
		CTreeCtrlNode fileNode = GetFileNode((CRichDocIn*)pDoc);
		ASSERT(fileNode);
		fileNode.SetText(pDoc->GetTitle());
		break;
	}
}

void CTreeCtrlErr::LoadErrorFile(CDocument *pDoc, LPCTSTR szFile)
{
	// set hourglass
	CWaitCursor wait;

	// set status bar
	CString strStatus;
	strStatus.Format(_T("Loading..."));
	CUtil::GetStatusBar().SetWindowText(strStatus);

	// sanity checks
	ASSERT(pDoc != NULL);
	ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(CRichDocIn)));

	// get file node
	CTreeCtrlNode fileNode = GetFileNode((CRichDocIn*)pDoc);
	if (fileNode.HasChildren())
	{
		CTreeCtrlNode child = fileNode.GetChild();
		for (; child != NULL; child = fileNode.GetChild())
		{
			VERIFY(child.Delete());
		}
		ASSERT(!fileNode.HasChildren());
	}
	ASSERT((CRichEditDoc*)fileNode.GetData() == pDoc);

	fileNode.AddTail(_T("Running..."), lineImage);

	// load file
	CStdioFileLineParser lineParser(szFile);
	CString strLine;
	while (lineParser.GetNextLine(strLine))
	{
		strLine.TrimRight();
		if (!strLine.IsEmpty()) {
			strLine.Replace(_T('\t'), _T('»'));
			fileNode.AddTail(strLine, lineImage);
		}
	}

	fileNode.Select();
	fileNode.Expand();
	fileNode.EnsureVisible();

	// reset status bar
	::AfxGetMainWnd()->PostMessage(WM_SETMESSAGESTRING, AFX_IDS_IDLEMESSAGE);
}
