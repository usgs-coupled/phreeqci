// TreeCtrlOut.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "TreeCtrlOut.h"

#include "NodeObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlOut

CTreeCtrlOut::CTreeCtrlOut()
{
}

CTreeCtrlOut::~CTreeCtrlOut()
{
}


BEGIN_MESSAGE_MAP(CTreeCtrlOut, baseCTreeCtrlOut)
	//{{AFX_MSG_MAP(CTreeCtrlOut)
	ON_WM_CREATE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_POPUP_CLOSE, OnPopupClose)
	ON_COMMAND(ID_SUBFILE_GOTO, OnSubfileGoto)
	ON_COMMAND(ID_FILE_CLOSE_ALL_OUTPUT, OnFileCloseAllOutput)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlOut message handlers

int CTreeCtrlOut::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (baseCTreeCtrlOut::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Add your specialized creation code here

	// Create the Image List
	if (!m_ctrlImage.Create(IDB_OUTPUT_ITEMS, 16, 0, RGB(255,0,255)))
	{
		TRACE0("Failed to create image list\n");
		return -1;
	}

	/// Attach image list to Tree
	SetImageList(&m_ctrlImage, TVSIL_NORMAL);

	InsertItem(_T("Output files"), workNode, workNode);
	
	return 0;
}

void CTreeCtrlOut::OnLButtonDblClk(UINT nFlags, CPoint point) 
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
			pDoc->UpdateAllViews(NULL, NODEOBJECT_JUMPTOOUT, &nodeObject);
		}
		else
		{
			baseCTreeCtrlOut::OnLButtonDblClk(nFlags, point);
		}		
	}
	else
	{
		baseCTreeCtrlOut::OnLButtonDblClk(nFlags, point);
	}
}

void CTreeCtrlOut::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	ASSERT(pWnd == this);
	UNUSED(pWnd);  // unused in release builds

	// must override OnRButtonDown to recieve this message
	CTreeCtrlNode node = GetSelectedItem();
	{
		if (point.x == -1 && point.y == -1){
			node.EnsureVisible();
			//keystroke invocation
			CRect rect;
			node.GetRect(rect, TRUE);
			ClientToScreen(rect);
			point = rect.TopLeft();
			point.Offset(rect.Height(), rect.Height() / 2);
		}

		CMenu menu;
		VERIFY( menu.LoadMenu(IDR_POPUP_TREE_OUT) );

		CMenu* pPopup;

		switch (node.GetLevel())
		{
		case 0 :
			pPopup = menu.GetSubMenu(2);
			ASSERT( pPopup != NULL );
			break;
		case 1 :	// file level
			pPopup = menu.GetSubMenu(0);
			ASSERT( pPopup != NULL );
			break;
		default :
			pPopup = menu.GetSubMenu(1);
			ASSERT( pPopup != NULL );
			break;
		}

		ASSERT( pPopup != NULL );
		CWnd* pWndPopupOwner = this;

		pWndPopupOwner = AfxGetMainWnd();
		ASSERT(pWndPopupOwner);

		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
			pWndPopupOwner);
	}	
}

void CTreeCtrlOut::OnRButtonDown(UINT nFlags, CPoint point) 
{
	UNUSED(nFlags);
	CTreeCtrlNode hitNode = HitTest(point);
	if (hitNode != NULL)
	{
		hitNode.Select();
		SetFocus();
	}
	else
	{
		// The next line will cause OnContextMenu to NOT be called
		baseCTreeCtrlOut::OnRButtonDown(nFlags, point);
	}
}

void CTreeCtrlOut::OnPopupClose() 
{
	CTreeCtrlNode selectedNode = GetSelectedItem();
	CRichEditDoc* pDoc = (CRichEditDoc*)selectedNode.GetData();
	ASSERT_KINDOF(CRichEditDoc, pDoc);
	pDoc->OnCloseDocument();	
}

void CTreeCtrlOut::OnSubfileGoto() 
{
	CTreeCtrlNode selectedNode = GetSelectedItem();
	CTreeCtrlNode fileNode;

	for (fileNode = selectedNode; fileNode.GetLevel() > 1; )
	{
		fileNode = fileNode.GetParent();
	}

	CDocument* pDoc = (CDocument*) fileNode.GetData();

	if (pDoc)
	{
		ASSERT_KINDOF(CDocument, pDoc);
		CNodeObject nodeObject(selectedNode);
		pDoc->UpdateAllViews(NULL, NODEOBJECT_JUMPTOOUT, &nodeObject);
	}
}

void CTreeCtrlOut::OnFileCloseAllOutput() 
{
	// make list of files to close
	std::list<CRichEditDoc*> listOutputFiles;
	CTreeCtrlNode fileNode = GetSelectedItem().GetChild();
	ASSERT(fileNode);
	
	while (fileNode)
	{
		CRichEditDoc* pDoc = (CRichEditDoc*)fileNode.GetData();
		ASSERT_KINDOF(CRichEditDoc, pDoc);

		listOutputFiles.push_back(pDoc);
		fileNode = fileNode.GetNextSibling();
	}

	std::list<CRichEditDoc*>::iterator iterFile = listOutputFiles.begin();
	for (; iterFile != listOutputFiles.end(); ++iterFile)
	{
		(*iterFile)->OnCloseDocument();
	}
}
