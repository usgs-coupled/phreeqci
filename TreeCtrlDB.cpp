// TreeCtrlDB.cpp : implementation file
//

#include "stdafx.h"
#include "phreeqci2.h"
#include "TreeCtrlDB.h"

#include "RichEditLineParser.h"
#include "RichLogicalLineParser.h"
#include "RichKeywordParser.h"


// CTreeCtrlDB

IMPLEMENT_DYNAMIC(CTreeCtrlDB, baseCTreeCtrlDB)

CTreeCtrlDB::CTreeCtrlDB()
{

}

CTreeCtrlDB::~CTreeCtrlDB()
{
}


BEGIN_MESSAGE_MAP(CTreeCtrlDB, baseCTreeCtrlDB)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CTreeCtrlDB::OnNMDblclk)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_DB_CLOSE, &CTreeCtrlDB::OnDbClose)
	ON_COMMAND(ID_DB_CLOSE_ALL, &CTreeCtrlDB::OnDbCloseAll)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CTreeCtrlDB::OnTvnSelchanged)
	ON_COMMAND(ID_DB_GOTO, &CTreeCtrlDB::OnDbGoto)
END_MESSAGE_MAP()



// CTreeCtrlDB message handlers

int CTreeCtrlDB::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (baseCTreeCtrlDB::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Create the Image List
	if (!m_ctrlImage.Create(IDB_INPUT_ITEMS, 16, 0, RGB(255,0,255)))
	{
		TRACE0("Failed to create image list\n");
		return -1;
	}
	// set overlay images
	VERIFY(m_ctrlImage.SetOverlayImage(UnSyncOverlayImage, UnSyncState));
	VERIFY(m_ctrlImage.SetOverlayImage(ErrorOverlayImage, ErrorState));

	// Attach image list to Tree
	SetImageList(&m_ctrlImage, TVSIL_NORMAL);

	this->InsertItem(_T("Database files"), CTreeCtrlKeyword::workImageDB, CTreeCtrlKeyword::workImageDB);

	return 0;
}

void CTreeCtrlDB::RemoveDatabaseKeyword(CRichEditDoc*)
{
	/* no-op */
}

void CTreeCtrlDB::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	UNUSED_ALWAYS(pNMHDR);

// COMMENT: {8/9/2012 5:46:49 PM}	CTreeCtrlNode node = this->GetSelectedItem();
// COMMENT: {8/9/2012 5:46:49 PM}	for (; node.GetLevel() > CTreeCtrlKeyword::FileLevel; node = node.GetParent())
// COMMENT: {8/9/2012 5:46:49 PM}	{
// COMMENT: {8/9/2012 5:46:49 PM}		if (!node) break;
// COMMENT: {8/9/2012 5:46:49 PM}	}

	CTreeCtrlNode node = this->GetSelectedItem();
	if (node.GetLevel() == CTreeCtrlKeyword::FileLevel && node.GetData())
	{
		::AfxMessageBox(((CRichEditDoc*)node.GetData())->GetPathName());
		*pResult = TRUE; // skip default handling
		return;
	}

	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CTreeCtrlDB::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: Add your message handler code here
	TRACE("CTreeCtrlDB::OnContextMenu\n");

	ASSERT(pWnd == this);
	UNUSED(pWnd);  // unused in release builds

	// must override OnRButtonDown to recieve this message
	CTreeCtrlNode node = GetSelectedItem();
	{
		if (point.x == -1 && point.y == -1)
		{
			node.EnsureVisible();
			//keystroke invocation
			CRect rect;
			node.GetRect(rect, TRUE);
			ClientToScreen(rect);
			point = rect.TopLeft();
			point.Offset(rect.Height(), rect.Height() / 2);
		}

		CMenu menu;
		VERIFY( menu.LoadMenu(IDR_POPUP_TREE_DB) );

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

void CTreeCtrlDB::OnRButtonDown(UINT nFlags, CPoint point)
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
		baseCTreeCtrlDB::OnRButtonDown(nFlags, point);
	}
}

void CTreeCtrlDB::OnDbCloseAll()
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

void CTreeCtrlDB::OnDbClose()
{
	CTreeCtrlNode selectedNode = GetSelectedItem();
	CRichEditDoc* pDoc = (CRichEditDoc*)selectedNode.GetData();
	ASSERT_KINDOF(CRichEditDoc, pDoc);
	pDoc->OnCloseDocument();	
}

void CTreeCtrlDB::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	UNUSED_ALWAYS(pNMTreeView);

	// Add your control notification handler code here
	CTreeCtrlNode item = this->GetSelectedItem();
	if (item.GetLevel() >= CTreeCtrlKeyword::FileLevel)
	{
		CTreeCtrlNode fileNode;
		for (fileNode = item; fileNode.GetLevel() > CTreeCtrlKeyword::FileLevel; )
		{
			fileNode = fileNode.GetParent();
		}
		if (CRichEditDoc* pDoc = (CRichEditDoc*) fileNode.GetData())
		{
			CRichEditView* pView = pDoc->GetView();
			if (CFrameWnd* frame = pView->GetParentFrame())
			{
				frame->ActivateFrame();
			}
		}
	}

	*pResult = 0;
}

void CTreeCtrlDB::OnDbGoto()
{
	CTreeCtrlNode node = GetSelectedItem();
	CRichEditView* pView = GetRichEditView(node);
	if (pView)
	{
		CWaitCursor wait;

		// set active document
		pView->GetParentFrame()->ActivateFrame();

		// select text
		CRichEditCtrl& rich = pView->GetRichEditCtrl();
		CHARRANGE cr = GetRange(node);
		rich.SetSel(cr.cpMin, cr.cpMin);

		// scroll into view
		rich.LineScroll(rich.LineFromChar(-1) - rich.GetFirstVisibleLine());
		pView->SetFocus();
	}
}

CRichEditView* CTreeCtrlDB::GetRichEditView(const CTreeCtrlNode &node)const
{
	CRichEditDoc* pDoc = GetRichEditDoc(node);
	if (pDoc == NULL)
	{
		return NULL;
	}
	return pDoc->GetView();
}

CRichEditDoc* CTreeCtrlDB::GetRichEditDoc(const CTreeCtrlNode &node)const
{
	CTreeCtrlNode fileNode = node;

	if (fileNode.GetLevel() == WorkLevel)
	{
		return NULL;
	}

	while (fileNode.GetLevel() > FileLevel)
	{
		fileNode = fileNode.GetParent();
	}

	ASSERT( fileNode.GetLevel() == FileLevel );

	CRichEditDoc* pDoc = (CRichEditDoc*)fileNode.GetData();
	ASSERT( pDoc );
	ASSERT( pDoc->IsKindOf(RUNTIME_CLASS(CRichEditDoc)) );

	return pDoc;
}

CHARRANGE CTreeCtrlDB::GetRange(CTreeCtrlNode node)const
{
	ASSERT(node);
	ASSERT(node.GetLevel() > WorkLevel);	// must not be root

	CHARRANGE cr = { 0, 0 };
	CTreeCtrlNode fileNode, sim, key, option;
	CString strLabel;

	fileNode = GetFileNode(node);
	ASSERT(fileNode);
	
	// special case for file node
	if (fileNode == node)
	{
		cr.cpMax = -1;
		return cr;
	}

	// get pointer to document
	CRichEditView* pView = GetRichEditView(fileNode);
	ASSERT(pView);
	ASSERT_KINDOF(CRichEditView, pView);

	CRichEditLineParser richEditLineParser(pView->m_hWnd);
	CRichLogicalLineParser richLogicalLineParser(richEditLineParser);
	CRichKeywordParser richKeywordParser(richLogicalLineParser);

	while (richKeywordParser.GetLine() == CKeywordParser::typeEmpty);

	for (sim = fileNode.GetChild(); sim != NULL; sim = sim.GetNextSibling())
	{
		if ( node == sim)
		{
			cr = richKeywordParser.cr;
		}
		for (key = sim.GetChild(); key != NULL; key = key.GetNextSibling())
		{
			strLabel = key.GetText();
			ASSERT( strLabel.CompareNoCase(richKeywordParser.tree_line) == 0 );

			if (node == key)
			{
				cr = richKeywordParser.cr;
			}
			else if (node == sim)
			{
				cr.cpMax = richKeywordParser.cr.cpMax;
			}

			while (richKeywordParser.GetLine() == CKeywordParser::typeEmpty);
			for (option = key.GetChild(); option != NULL; option = option.GetNextSibling())
			{
				strLabel = option.GetText();
				ASSERT( strLabel.CompareNoCase(richKeywordParser.tree_line) == 0 );

				if (node == option)
				{
					cr = richKeywordParser.cr;
				}
				else if (node == key || node == sim)
				{
					cr.cpMax = richKeywordParser.cr.cpMax;
				}
				while (richKeywordParser.GetLine() == CKeywordParser::typeEmpty);
			}

		}
	}
	return cr;
}

