// WorkspaceBar.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "WorkspaceBar.h"

#include "RichDocIn.h"
#include "RichDocOut.h"
#include "RichDocDB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar

IMPLEMENT_DYNAMIC(CWorkspaceBar, baseWorkspaceBar);

const int DEFAULT_DIFF_TEXT_Y = 128;

CWorkspaceBar::CWorkspaceBar()
// COMMENT: {8/16/2012 6:52:33 PM}: m_PosBarY(130)
: m_PosBarY(495)
, m_cyRich(128)
, m_bResizing(false)
{
}

CWorkspaceBar::~CWorkspaceBar()
{
}


BEGIN_MESSAGE_MAP(CWorkspaceBar, baseWorkspaceBar)
	//{{AFX_MSG_MAP(CWorkspaceBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_NCLBUTTONUP()
	ON_WM_MOUSEACTIVATE()
	//}}AFX_MSG_MAP

	// Tab control notifications
	ON_NOTIFY(TCN_SELCHANGE, IDC_TC_WORKSPACE, OnSelChange)

	// Tree control notifications
	ON_NOTIFY(TVN_DELETEITEM, IDC_TV_WS_OUT, OnDeleteItemOut)
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar message handlers

int CWorkspaceBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (baseWorkspaceBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Add your specialized creation code here

	// Create tab control
	if (!m_wndTab.Create(WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN		// standard window styles
		| TCS_FOCUSNEVER | TCS_BOTTOM,									// TabCtrl styles
		CRect(0,0,0,0), this, IDC_TC_WORKSPACE))
	{
		TRACE0("Failed to create tab control\n");
		return -1;
	}

	// Create Input files tree
	if (!m_wndTreeIn.CWnd::Create(WC_TREEVIEW, _T("Input files"),
		WS_CHILD | /*WS_VISIBLE |*/ WS_BORDER | WS_CLIPSIBLINGS						// standard window styles
		| TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS,		// TreeView styles
		CRect(0,0,0,0), &m_wndTab, IDC_TV_WS_IN))
	{
		TRACE0("Failed to create input tree control\n");
		return -1;
	}

	// Create Output files tree
	if (!m_wndTreeOut.CWnd::Create(WC_TREEVIEW, _T("Output files"),
		WS_CHILD | /*WS_VISIBLE |*/ WS_BORDER | WS_CLIPSIBLINGS						// standard window styles
		| TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS,		// TreeView styles
		CRect(0,0,0,0), &m_wndTab, IDC_TV_WS_OUT))
	{
		TRACE0("Failed to create output tree control\n");
		return -1;
	}

	// Create DB files tree
	if (!m_wndTreeDB.CWnd::Create(WC_TREEVIEW, _T("Database files"),
		WS_CHILD | /*WS_VISIBLE |*/ WS_BORDER | WS_CLIPSIBLINGS						// standard window styles
		| TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS,		// TreeView styles
		CRect(0,0,0,0), &m_wndTab, IDC_TV_WS_DB))
	{
		TRACE0("Failed to create database tree control\n");
		return -1;
	}

	// Create pfw desc
	if (!m_wndRichPfw.Create(WS_CHILD | WS_BORDER | WS_CLIPSIBLINGS | WS_VSCROLL    // standard window styles
		| ES_READONLY | ES_MULTILINE | ES_AUTOVSCROLL,                              // edit styles
		CRect(0,0,0,0), &m_wndTab, IDC_RE_WS_PFW))
	{
		TRACE0("Failed to create pfw richedit control\n");
		return -1;
	}

	// Create pfw tree
	if (!m_wndTreePfw.CWnd::Create(WC_TREEVIEW, _T("pfw"),
		WS_CHILD | /*WS_VISIBLE |*/ WS_BORDER | WS_CLIPSIBLINGS                     // standard window styles
		| TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS,      // TreeView styles
		CRect(0,0,0,0), &m_wndTab, IDC_TV_WS_PFW))
	{
		TRACE0("Failed to create pfw tree control\n");
		return -1;
	}
	m_wndTreePfw.SetDescWindow(&m_wndRichPfw);

	// Create Errors tree
	if (!m_wndTreeErr.CWnd::Create(WC_TREEVIEW, _T("Errors"),
		WS_CHILD | /*WS_VISIBLE |*/ WS_BORDER | WS_CLIPSIBLINGS                     // standard window styles
		| TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS,      // TreeView styles
		CRect(0,0,0,0), &m_wndTab, IDC_TV_WS_ERR))
	{
		TRACE0("Failed to create error tree control\n");
		return -1;
	}

	// Create the Image List
	if (!m_ctrlImage.Create(IDB_TAB_ITEMS, 16, 0, RGB(255,0,255)))
	{
		TRACE0("Failed to create image list\n");
		return -1;
	}

	m_wndTab.SetImageList(&m_ctrlImage);

	// Get Default GUI font
	// older versions of Windows* (NT 3.51 for instance)
	// fail with DEFAULT_GUI_FONT
	if (!m_font.CreateStockObject(DEFAULT_GUI_FONT))
	{
		if (!m_font.CreatePointFont(80, _T("MS Sans Serif")))
		{
			TRACE0("Failed to create default font\n");
			return -1;
		}
	}

	// Get Default GUI font
	m_wndTab.SetFont(&m_font);
	m_wndTreeIn.SetFont(&m_font);
	m_wndTreeOut.SetFont(&m_font);
	m_wndTreeErr.SetFont(&m_font);
	m_wndTreePfw.SetFont(&m_font);
	m_wndRichPfw.SetFont(&m_font);

	return 0;
}

void CWorkspaceBar::OnSize(UINT nType, int cx, int cy)
{
	baseWorkspaceBar::OnSize(nType, cx, cy);

	// Add your message handler code here

	CRect rc;
	GetClientRect(rc);

	// This along with the WS_CLIPCHILDREN style avoids flashing within the tree
	m_wndTab.MoveWindow(rc, FALSE);
	m_wndTab.InvalidateRect(NULL, TRUE);

	SetRect(&rc, 0, 0, cx, cy);
	m_wndTab.AdjustRect(FALSE, &rc);
	rc.bottom -= 2;

	if ( m_wndTreeIn.GetSafeHwnd() )
	{
		m_wndTreeIn.MoveWindow(&rc);
		if (IsFloating())
		{
			m_wndTreeIn.RedrawWindow();
		}
	}

	if ( m_wndTreeOut.GetSafeHwnd() )
	{
		m_wndTreeOut.MoveWindow(&rc);
	}

	if ( m_wndTreeDB.GetSafeHwnd() )
	{
		m_wndTreeDB.MoveWindow(&rc);
	}

	if ( m_wndTreeErr.GetSafeHwnd() )
	{
		m_wndTreeErr.MoveWindow(&rc);
	}

	if ( m_wndTreePfw.GetSafeHwnd() )
	{
// COMMENT: {8/16/2012 6:53:22 PM}		TRACE("AdjustRect %d, %d, %d, %d\n", rc.left, rc.top, rc.right, rc.bottom);
// COMMENT: {8/16/2012 6:53:22 PM}		//{{
// COMMENT: {8/16/2012 6:53:22 PM}		CRect rcText(rc);
// COMMENT: {8/16/2012 6:53:22 PM}		rc.bottom -= 2;
// COMMENT: {8/16/2012 6:53:22 PM}		rc.bottom -= m_PosBarY;
// COMMENT: {8/16/2012 6:53:22 PM}		//}}
// COMMENT: {8/16/2012 6:53:22 PM}		TRACE("m_wndTreePfw %d, %d, %d, %d\n", rc.left, rc.top, rc.right, rc.bottom);
// COMMENT: {8/16/2012 6:53:22 PM}		m_wndTreePfw.MoveWindow(&rc);
// COMMENT: {8/16/2012 6:53:22 PM}		
// COMMENT: {8/16/2012 6:53:22 PM}		//{{
// COMMENT: {8/16/2012 6:53:22 PM}		//rcText.top += rc.bottom + 2;
// COMMENT: {8/16/2012 6:53:22 PM}		rcText.top += rc.bottom;
// COMMENT: {8/16/2012 6:53:22 PM}		TRACE("m_wndRich Pfw %d, %d, %d, %d\n", rcText.left, rcText.top, rcText.right, rcText.bottom);
// COMMENT: {8/16/2012 6:53:22 PM}		m_wndRichPfw.MoveWindow(&rcText);
// COMMENT: {8/16/2012 6:53:22 PM}		//}}

// COMMENT: {8/16/2012 4:58:00 PM}		//{{
// COMMENT: {8/16/2012 4:58:00 PM}		SetRect(&rc, 0, 0, cx, cy);
// COMMENT: {8/16/2012 4:58:00 PM}		m_wndTab.AdjustRect(FALSE, &rc);
// COMMENT: {8/16/2012 4:58:00 PM}		CRect rcText(rc);
// COMMENT: {8/16/2012 4:58:00 PM}		TRACE("AdjustRect %d, %d, %d, %d\n", rc.left, rc.top, rc.right, rc.bottom);
// COMMENT: {8/16/2012 4:58:00 PM}
// COMMENT: {8/16/2012 4:58:00 PM}		rc.bottom = this->m_PosBarY - 2;
// COMMENT: {8/16/2012 4:58:00 PM}		TRACE("m_wndTreePfw %d, %d, %d, %d\n", rc.left, rc.top, rc.right, rc.bottom);
// COMMENT: {8/16/2012 4:58:00 PM}		m_wndTreePfw.MoveWindow(&rc);
// COMMENT: {8/16/2012 4:58:00 PM}
// COMMENT: {8/16/2012 4:58:00 PM}		rcText.top += rc.bottom;
// COMMENT: {8/16/2012 4:58:00 PM}		rcText.bottom -= 2;
// COMMENT: {8/16/2012 4:58:00 PM}		TRACE("m_wndRich Pfw %d, %d, %d, %d\n", rcText.left, rcText.top, rcText.right, rcText.bottom);
// COMMENT: {8/16/2012 4:58:00 PM}		m_wndRichPfw.MoveWindow(&rcText);
// COMMENT: {8/16/2012 4:58:00 PM}		//}}

		//{{
		SetRect(&rc, 0, 0, cx, cy);
		m_wndTab.AdjustRect(FALSE, &rc);

// COMMENT: {8/16/2012 9:03:14 PM}		//{{ HACK
// COMMENT: {8/16/2012 9:03:14 PM}		this->m_PosBarY = rc.bottom - 2 - DEFAULT_DIFF_TEXT_Y;
// COMMENT: {8/16/2012 9:03:14 PM}		//}} HACK

		CRect rcText(rc);
		TRACE("AdjustRect %d, %d, %d, %d\n", rc.left, rc.top, rc.right, rc.bottom);

// COMMENT: {8/16/2012 10:18:00 PM}		rc.bottom = this->m_PosBarY - 2;
		/*
		// this->m_PosBarY = rc.bottom - 2 - this->m_cyRich;
		// this->m_cyRich  = rc.bottom - 2 - this->m_PosBarY
		this->m_PosBarY = rc.bottom - 2 - this->m_cyRich;
		*/
		rc.bottom = rc.bottom - 2 - this->m_cyRich - 2;
		TRACE("m_wndTreePfw %d, %d, %d, %d\n", rc.left, rc.top, rc.right, rc.bottom);
		m_wndTreePfw.MoveWindow(&rc);

		rcText.top += rc.bottom;
		rcText.bottom -= 2;
		TRACE("m_wndRich Pfw %d, %d, %d, %d\n", rcText.left, rcText.top, rcText.right, rcText.bottom);
		m_wndRichPfw.MoveWindow(&rcText);
		//}}

		// set hot spot rect
		m_rcHotSpot.left   = rc.left;
		m_rcHotSpot.top    = rc.bottom;
		m_rcHotSpot.right  = rcText.right;
		m_rcHotSpot.bottom = rcText.top;
	}
}

void CWorkspaceBar::OnSelChange(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	ASSERT(m_wndTab.GetItemCount());	// shouldn't be called if there are no tabs present
	// determine which tab was selected
	TCITEM item;
	item.mask = TCIF_IMAGE;
	m_wndTab.GetItem(m_wndTab.GetCurSel(), &item);

	ASSERT( item.iImage != -1 );	// image must be set

	switch (item.iImage)
	{
	case InputImage :
		// display input tree
		ASSERT(m_wndTreeIn.GetRootItem().HasChildren());
		ASSERT(!m_bResizing);
		m_wndTreeIn.ShowWindow(SW_SHOW);
		m_wndTreeIn.SetFocus();

		// hide other tabs
		m_wndTreeOut.ShowWindow(SW_HIDE);
		m_wndTreeDB.ShowWindow(SW_HIDE);
		m_wndTreeErr.ShowWindow(SW_HIDE);
		m_wndTreePfw.ShowWindow(SW_HIDE);
		m_wndRichPfw.ShowWindow(SW_HIDE);

		if (pResult)
		{
			CTreeCtrlNode item = m_wndTreeIn.GetSelectedItem();
			if (item.GetLevel() >= CTreeCtrlKeyword::FileLevel)
			{
				CTreeCtrlNode fileNode;
				for (fileNode = item; fileNode.GetLevel() > CTreeCtrlKeyword::FileLevel; )
				{
					fileNode = fileNode.GetParent();
				}
				if (CRichDocIn* pDoc = (CRichDocIn*) fileNode.GetData())
				{
					CRichEditView* pView = pDoc->GetView();
					if (CFrameWnd* frame = pView->GetParentFrame())
					{
						frame->ActivateFrame();
					}
				}
			}
		}
		break;

	case OutputImage :
		// display output tree
		ASSERT(m_wndTreeOut.GetRootItem().HasChildren());
		ASSERT(!m_bResizing);
		m_wndTreeOut.ShowWindow(SW_SHOW);
		m_wndTreeOut.SetFocus();

		// hide other tabs
		m_wndTreeIn.ShowWindow(SW_HIDE);
		m_wndTreeDB.ShowWindow(SW_HIDE);
		m_wndTreeErr.ShowWindow(SW_HIDE);
		m_wndTreePfw.ShowWindow(SW_HIDE);
		m_wndRichPfw.ShowWindow(SW_HIDE);

		if (pResult)
		{
			CTreeCtrlNode item = m_wndTreeOut.GetSelectedItem();
			if (item.GetLevel() >= CTreeCtrlKeyword::FileLevel)
			{
				CTreeCtrlNode fileNode;
				for (fileNode = item; fileNode.GetLevel() > CTreeCtrlKeyword::FileLevel; )
				{
					fileNode = fileNode.GetParent();
				}
				if (CRichDocOut* pDoc = (CRichDocOut*) fileNode.GetData())
				{
					CRichEditView* pView = pDoc->GetView();
					if (CFrameWnd* frame = pView->GetParentFrame())
					{
						frame->ActivateFrame();
					}
				}
			}
		}
		break;

	case DatabaseImage :
		// display database tree
		ASSERT(m_wndTreeDB.GetRootItem().HasChildren());
		ASSERT(!m_bResizing);
		m_wndTreeDB.ShowWindow(SW_SHOW);
		m_wndTreeDB.SetFocus();

		// hide other tabs
		m_wndTreeIn.ShowWindow(SW_HIDE);
		m_wndTreeOut.ShowWindow(SW_HIDE);
		m_wndTreeErr.ShowWindow(SW_HIDE);
		m_wndTreePfw.ShowWindow(SW_HIDE);
		m_wndRichPfw.ShowWindow(SW_HIDE);

		if (pResult)
		{
			CTreeCtrlNode item = m_wndTreeDB.GetSelectedItem();
			if (item.GetLevel() >= CTreeCtrlKeyword::FileLevel)
			{
				CTreeCtrlNode fileNode;
				for (fileNode = item; fileNode.GetLevel() > CTreeCtrlKeyword::FileLevel; )
				{
					fileNode = fileNode.GetParent();
				}
				if (CRichDocDB* pDoc = (CRichDocDB*) fileNode.GetData())
				{
					CRichEditView* pView = pDoc->GetView();
					if (CFrameWnd* frame = pView->GetParentFrame())
					{
						frame->ActivateFrame();
					}
				}
			}
		}
		break;

	case ErrorImage :
		// display output tree
		ASSERT(m_wndTreeErr.GetRootItem().HasChildren());
		ASSERT(!m_bResizing);
		m_wndTreeErr.ShowWindow(SW_SHOW);
		m_wndTreeErr.SetFocus();

		// hide other tabs
		m_wndTreeIn.ShowWindow(SW_HIDE);
		m_wndTreeOut.ShowWindow(SW_HIDE);
		m_wndTreeDB.ShowWindow(SW_HIDE);
		m_wndTreePfw.ShowWindow(SW_HIDE);
		m_wndRichPfw.ShowWindow(SW_HIDE);
		break;

	case PfwImage :
		// display output tree
		ASSERT(m_wndTreePfw.GetRootItem().HasChildren());
		ASSERT(!m_bResizing);
		m_wndTreePfw.ShowWindow(SW_SHOW);
		m_wndRichPfw.ShowWindow(SW_SHOW);
		m_wndTreePfw.SetFocus();

		// hide other tabs
		m_wndTreeIn.ShowWindow(SW_HIDE);
		m_wndTreeOut.ShowWindow(SW_HIDE);
		m_wndTreeErr.ShowWindow(SW_HIDE);
		m_wndTreeDB.ShowWindow(SW_HIDE);
		break;
	}
	// refresh tab control
	m_wndTab.RedrawWindow();
}

CTreeCtrlNode CWorkspaceBar::AddOutputFileNode(LPCTSTR lpszLabel)
{
	if ( !m_wndTreeOut.GetRootItem().HasChildren() )
	{
		ASSERT( m_wndTreeOut.IsWindowVisible() == FALSE );
		ASSERT( GetTabIndex(OutputImage) == -1);
		// add output tab
		m_wndTab.InsertItem(TCIF_TEXT | TCIF_IMAGE | TCIF_PARAM, this->GetInsertIndex(CWorkspaceBar::OutputImage),
			_T("Output"), CWorkspaceBar::OutputImage, IDC_TV_WS_OUT);

		// resize tree
		CRect rc;
		GetClientRect(rc);

		int cx = rc.Width();
		int cy = rc.Height();
		SetRect(&rc, 0, 0, cx, cy);
		m_wndTab.AdjustRect(FALSE, &rc);
		rc.bottom -= 2;
		m_wndTreeOut.MoveWindow(&rc);
	}

	CTreeCtrlNode node =  m_wndTreeOut.InsertItem(lpszLabel, CTreeCtrlOut::fileNode,
		CTreeCtrlOut::fileNode, m_wndTreeOut.GetRootItem());

	m_wndTab.SetCurSel(GetTabIndex(OutputImage));
	OnSelChange(NULL, NULL);
	ASSERT( m_wndTreeOut.IsWindowVisible() == TRUE );

	return node;
}

void CWorkspaceBar::RemoveInputFileNode(CTreeCtrlNode node)
{
	ASSERT( node );
	VERIFY( node.Delete() );
	if ( !m_wndTreeIn.GetRootItem().HasChildren() )
	{
		// remove tab
		int nCurSel = m_wndTab.GetCurSel();
		int nItem = GetTabIndex(InputImage);

		ASSERT( nItem != -1 );
		m_wndTab.DeleteItem(nItem);
		m_wndTreeIn.ShowWindow(SW_HIDE);

		if (nItem == nCurSel)
		{
			if (m_wndTab.GetItemCount())
			{
				m_wndTab.SetCurSel(0);
				OnSelChange(NULL, NULL);
			}
		}
	}
}

void CWorkspaceBar::RemoveErrorFileNode(CTreeCtrlNode node)
{
	ASSERT( node );
	VERIFY( node.Delete() );
	if (!m_wndTreeErr.GetRootItem().HasChildren())
	{
		// remove tab
		int nCurSel = m_wndTab.GetCurSel();
		int nItem = GetTabIndex(ErrorImage);

		ASSERT( nItem != -1 );
		m_wndTab.DeleteItem(nItem);
		m_wndTreeErr.ShowWindow(SW_HIDE);

		if (nItem == nCurSel)
		{
			if (m_wndTab.GetItemCount())
			{
				m_wndTab.SetCurSel(0);
				OnSelChange(NULL, NULL);
			}
		}
	}
}

int CWorkspaceBar::GetTabIndex(enum CWorkspaceBar::TabImage imageIndex)
{
	// find tab by image index
	TCITEM item;
	item.mask = TCIF_IMAGE;
	int nItems = m_wndTab.GetItemCount();
	for (int i = 0; i < nItems; ++i)
	{
		m_wndTab.GetItem(i, &item);
		if (item.iImage == imageIndex)
		{
			return i;
		}
	}
	return -1;	// no item found
}

int CWorkspaceBar::GetInsertIndex(enum CWorkspaceBar::TabImage tabIndex)
{
	ASSERT(CWorkspaceBar::GetTabIndex(tabIndex) == -1);
	for (int i = tabIndex - 1; i >= 0; --i)
	{
		int n = CWorkspaceBar::GetTabIndex((enum CWorkspaceBar::TabImage)i);
		if (n != -1)
		{
			return n + 1;
		}
	}
	return 0;
}

void CWorkspaceBar::OnDeleteItemOut(NMHDR* pNMHDR, LRESULT* /*pResult*/)
{
	NMTREEVIEW* pNMTREEVIEW = (NMTREEVIEW*)pNMHDR;

	// ignore if not direct child (file node)
	if (m_wndTreeOut.GetRootItem() != m_wndTreeOut.GetParentItem(pNMTREEVIEW->itemOld.hItem))
		return;

	CTreeCtrlNode deleteNode(pNMTREEVIEW->itemOld.hItem, &m_wndTreeOut);
	ASSERT( deleteNode.GetParent() == m_wndTreeOut.GetRootItem() );

	// note item has not been deleted yet
	if (deleteNode.GetNextSibling() == NULL && deleteNode.GetPrevSibling() == NULL)
	{
		// remove tab
		int nCurSel = m_wndTab.GetCurSel();
		int nItem = GetTabIndex(OutputImage);

		ASSERT( nItem != -1 );
		m_wndTab.DeleteItem(nItem);
		m_wndTreeOut.ShowWindow(SW_HIDE);

		if (nItem == nCurSel)
		{
			if (m_wndTab.GetItemCount())
			{
				m_wndTab.SetCurSel(0);
				OnSelChange(NULL, NULL);
			}
		}
	}

}

BOOL CWorkspaceBar::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	int nItem = m_wndTab.GetCurSel();

	if (nItem != -1 && IsVisible())
	{
		TCITEM tcitem;
		tcitem.mask = TCIF_IMAGE;

		m_wndTab.GetItem(nItem, &tcitem);


		switch (tcitem.iImage)
		{
		case CWorkspaceBar::InputImage :
			if (m_wndTreeIn.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			{
				return TRUE;
			}
			break;
		case CWorkspaceBar::OutputImage :
			if (m_wndTreeOut.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			{
				return TRUE;
			}
			break;
		case CWorkspaceBar::DatabaseImage :
			if (m_wndTreeDB.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			{
				return TRUE;
			}
		case CWorkspaceBar::ErrorImage :
			if (m_wndTreeErr.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			{
				return TRUE;
			}
		case CWorkspaceBar::PfwImage :
			if (m_wndTreePfw.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			{
				return TRUE;
			}
			break;
		}
	}

	return baseWorkspaceBar::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CWorkspaceBar::AddInputDoc(CRichEditDoc *pDoc)
{
	// Initialize error tree
	if ( !m_wndTreeErr.GetRootItem().HasChildren() )
	{
		ASSERT(!m_wndTreeIn.IsWindowVisible());

		// add errors tab
		m_wndTab.InsertItem(TCIF_TEXT | TCIF_IMAGE | TCIF_PARAM, this->GetInsertIndex(CWorkspaceBar::ErrorImage),
			_T("Errors"), CWorkspaceBar::ErrorImage, IDC_TV_WS_ERR);

		// resize tree
		CRect rc;
		GetClientRect(rc);

		int cx = rc.Width();
		int cy = rc.Height();
		SetRect(&rc, 0, 0, cx, cy);
		m_wndTab.AdjustRect(FALSE, &rc);
		rc.bottom -= 2;
		m_wndTreeErr.MoveWindow(&rc);
	}

	// Initialize input tree
	if ( !m_wndTreeIn.GetRootItem().HasChildren() )
	{
		ASSERT(!m_wndTreeIn.IsWindowVisible());

		// add Pfw tab
		m_wndTab.InsertItem(TCIF_TEXT | TCIF_IMAGE | TCIF_PARAM, this->GetInsertIndex(CWorkspaceBar::PfwImage),
			_T("Pfw"), CWorkspaceBar::PfwImage, IDC_TV_WS_PFW);

		// add input tab
		m_wndTab.InsertItem(TCIF_TEXT | TCIF_IMAGE | TCIF_PARAM, this->GetInsertIndex(CWorkspaceBar::InputImage),
			_T("Input"), CWorkspaceBar::InputImage, IDC_TV_WS_IN);

		// resize tree
		CRect rc;
		GetClientRect(&rc);
		
		////int cx = rc.Width();
		////int cy = rc.Height();
		////SetRect(&rc, 0, 0, cx, cy);
		ASSERT(rc.left == 0 && rc.top == 0);
		m_wndTab.AdjustRect(FALSE, &rc);
		rc.bottom -= 2;
		m_wndTreeIn.MoveWindow(&rc);

// COMMENT: {8/16/2012 8:59:49 PM}		CRect rcText(rc);
// COMMENT: {8/16/2012 8:59:49 PM}		rc.bottom -= m_PosBarY;
// COMMENT: {8/16/2012 8:59:49 PM}		m_wndTreePfw.MoveWindow(&rc);
// COMMENT: {8/16/2012 8:59:49 PM}
// COMMENT: {8/16/2012 8:59:49 PM}		rcText.top += rc.bottom;
// COMMENT: {8/16/2012 8:59:49 PM}// COMMENT: {8/15/2012 9:23:55 PM}		ASSERT(rcText.Height() == 180);
// COMMENT: {8/16/2012 8:59:49 PM}		m_wndRichPfw.MoveWindow(&rcText);

		//{{
		this->GetClientRect(&rc);
		this->m_wndTab.AdjustRect(FALSE, &rc);

		// this->m_PosBarY = rc.bottom - 2 - this->m_cyRich;
		// this->m_cyRich  = rc.bottom - 2 - this->m_PosBarY
		this->m_PosBarY = rc.bottom - 2 - this->m_cyRich;

		CRect rcText(rc);
		TRACE("AdjustRect %d, %d, %d, %d\n", rc.left, rc.top, rc.right, rc.bottom);

		rc.bottom = this->m_PosBarY - 2;
		TRACE("m_wndTreePfw %d, %d, %d, %d\n", rc.left, rc.top, rc.right, rc.bottom);
		m_wndTreePfw.MoveWindow(&rc);

		rcText.top += rc.bottom;
		rcText.bottom -= 2;
		TRACE("m_wndRich Pfw %d, %d, %d, %d\n", rcText.left, rcText.top, rcText.right, rcText.bottom);
		m_wndRichPfw.MoveWindow(&rcText);
		//}}


		// set hot spot rect
		m_rcHotSpot.left   = rc.left;
		m_rcHotSpot.top    = rc.bottom;
		m_rcHotSpot.right  = rcText.right;
		m_rcHotSpot.bottom = rcText.top;
	}

	int nErrors = SyncInputTrees(pDoc);

	if (nErrors > 0)
	{
		// if errors select error tab
		m_wndTab.SetCurSel(GetTabIndex(CWorkspaceBar::ErrorImage));
		OnSelChange(NULL, NULL);
		::AfxMessageBox(_T("Input file contains errors"), MB_OK);
	}
	else
	{
		// if no errors select input tab
		m_wndTab.SetCurSel(GetTabIndex(CWorkspaceBar::InputImage));
		OnSelChange(NULL, NULL);
	}
}

void CWorkspaceBar::AddDatabaseDoc(CRichEditDoc *pDoc)
{
	// Initialize input tree
	if ( !m_wndTreeDB.GetRootItem().HasChildren() )
	{
		ASSERT(!m_wndTreeDB.IsWindowVisible());
		ASSERT(this->GetTabIndex(CWorkspaceBar::DatabaseImage) == -1);

		// add Database tab
		m_wndTab.InsertItem(TCIF_TEXT | TCIF_IMAGE | TCIF_PARAM, this->GetInsertIndex(CWorkspaceBar::DatabaseImage),
			_T("Database"), CWorkspaceBar::DatabaseImage, IDC_TV_WS_DB);

		// resize tree
		CRect rc;
		GetClientRect(rc);

		int cx = rc.Width();
		int cy = rc.Height();
		SetRect(&rc, 0, 0, cx, cy);
		m_wndTab.AdjustRect(FALSE, &rc);
		rc.bottom -= 2;
		m_wndTreeDB.MoveWindow(&rc);
	}

	CTreeCtrlNode node = this->m_wndTreeDB.ParseTree(pDoc, 0);
	node.Select();
}

void CWorkspaceBar::RemoveInputDoc(CRichEditDoc *pDoc)
{
	CTreeCtrlNode errorNode = m_wndTreeErr.GetFileNode(pDoc);

	ASSERT( errorNode );
	VERIFY( errorNode.Delete() );

	// hide tab if no more files
	if (!m_wndTreeErr.GetRootItem().HasChildren())
	{
		// remove tab
		int nCurSel = m_wndTab.GetCurSel();
		int nItem = GetTabIndex(CWorkspaceBar::ErrorImage);

		ASSERT( nItem != -1 );
		if (nItem != -1)
		{
			m_wndTab.DeleteItem(nItem);
		}
		m_wndTreeErr.ShowWindow(SW_HIDE);

		if (nItem == nCurSel)
		{
			if (m_wndTab.GetItemCount())
			{
				m_wndTab.SetCurSel(0);
				OnSelChange(NULL, NULL);
			}
		}
	}

	CTreeCtrlNode inputNode = m_wndTreeIn.GetFileNode(pDoc);

	ASSERT( inputNode );
	VERIFY( inputNode.Delete() );

	// hide tab if no more files
	if (!m_wndTreeIn.GetRootItem().HasChildren())
	{
		// remove tab
		int nCurSel = m_wndTab.GetCurSel();
		int nItem = GetTabIndex(CWorkspaceBar::InputImage);

		ASSERT( nItem != -1 );
		if (nItem != -1)
		{
			m_wndTab.DeleteItem(nItem);
		}
		m_wndTreeIn.ShowWindow(SW_HIDE);

		if (nItem == nCurSel)
		{
			if (m_wndTab.GetItemCount())
			{
				m_wndTab.SetCurSel(0);
				OnSelChange(NULL, NULL);
			}
		}

		nItem = GetTabIndex(CWorkspaceBar::PfwImage);

		ASSERT( nItem != -1 );
		if (nItem != -1)
		{
			m_wndTab.DeleteItem(nItem);
		}
		m_wndTreePfw.ShowWindow(SW_HIDE);

		if (nItem == nCurSel)
		{
			if (m_wndTab.GetItemCount())
			{
				m_wndTab.SetCurSel(0);
				OnSelChange(NULL, NULL);
			}
		}
	}
}

void CWorkspaceBar::RemoveDatabaseDoc(CRichEditDoc *pDoc)
{
	CTreeCtrlNode dbNode = this->m_wndTreeDB.GetFileNode(pDoc);

	ASSERT( dbNode );
	VERIFY( dbNode.Delete() );

	// hide tab if no more files
	if (!this->m_wndTreeDB.GetRootItem().HasChildren())
	{
		// remove tab
		int nCurSel = this->m_wndTab.GetCurSel();
		int nItem = this->GetTabIndex(CWorkspaceBar::DatabaseImage);

		ASSERT( nItem != -1 );
		if (nItem != -1)
		{
			m_wndTab.DeleteItem(nItem);
		}
		this->m_wndTreeDB.ShowWindow(SW_HIDE);

		if (nItem == nCurSel)
		{
			if (m_wndTab.GetItemCount())
			{
				m_wndTab.SetCurSel(0);
				OnSelChange(NULL, NULL);
			}
		}
	}
}

void CWorkspaceBar::UpdateAllInViews(CDocument *pDoc, LPARAM lHint, CObject *pHint)
{
	m_wndTreeErr.UpdateAllViews(pDoc, lHint, pHint);
	m_wndTreeIn.UpdateAllViews(pDoc, lHint, pHint);
}

int CWorkspaceBar::SyncInputTrees(CRichEditDoc *pDoc)
{
	int nErrors = m_wndTreeErr.CheckErrors(pDoc);
	m_wndTreeIn.ParseTree(pDoc, nErrors);
	return nErrors;
}

void CWorkspaceBar::OnNcLButtonUp(UINT nHitTest, CPoint point) 
{
	if (nHitTest == HTCLOSE)
	{
		// close through CMainFrame
		SendMessage(WM_COMMAND, ID_VIEW_WORKSPACE, 0);
		baseCSizingControlBar::OnNcLButtonUp(nHitTest, point);
		return;
	}
	baseWorkspaceBar::OnNcLButtonUp(nHitTest, point);
}

int CWorkspaceBar::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) 
{
	// Add your message handler code here and/or call default
	if (!::IsChild(this->m_hWnd, ::GetFocus()))
	{
		if (m_wndTab.GetItemCount())
		{
			OnSelChange(NULL, NULL);
		}
	}	
	return baseWorkspaceBar::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

CTreeCtrlIn& CWorkspaceBar::GetTreeCtrlIn()
{
	return m_wndTreeIn;
}

enum CWorkspaceBar::TabIndex CWorkspaceBar::GetCurSelTab()const
{
	return static_cast<enum CWorkspaceBar::TabIndex>(m_wndTab.GetCurSel());
}

void CWorkspaceBar::SelectDatabaseTreeItem(CRichEditDoc* pDoc) 
{
	if (pDoc)
	{
		ASSERT_KINDOF(CRichDocDB, pDoc);
		CTreeCtrlNode node = this->m_wndTreeDB.GetFileNode(pDoc);
		if (node)
		{
			node.Select();
		}
	}
}

void CWorkspaceBar::OnMouseMove(UINT nFlags, CPoint point)
{
	int nItem = m_wndTab.GetCurSel();

#ifdef _DEBUG
	afxDump << " OnMouseMove " << point << "\n";
#endif

	if (this->m_bResizing)
	{
		HCURSOR cursor = ::LoadCursor(NULL, IDC_SIZENS);
		if (cursor)
		{
			::SetCursor(cursor);
		}
		////{{
		CRect rc;
		this->GetClientRect(&rc);
		this->m_wndTab.AdjustRect(FALSE, &rc);

		// this->m_PosBarY = rc.bottom - 2 - this->m_cyRich;
		// this->m_cyRich  = rc.bottom - 2 - this->m_PosBarY
		// this->m_PosBarY = rc.bottom - 2 - this->m_cyRich;
		this->m_cyRich = rc.bottom - 2 - point.y;

		CRect rcText(rc);
		TRACE("AdjustRect %d, %d, %d, %d\n", rc.left, rc.top, rc.right, rc.bottom);

		rc.bottom = rc.bottom - 2 - this->m_cyRich - 2;
		TRACE("m_wndTreePfw %d, %d, %d, %d\n", rc.left, rc.top, rc.right, rc.bottom);
		m_wndTreePfw.MoveWindow(&rc);

		rcText.top += rc.bottom;
		rcText.bottom -= 2;
		TRACE("m_wndRich Pfw %d, %d, %d, %d\n", rcText.left, rcText.top, rcText.right, rcText.bottom);
		m_wndRichPfw.MoveWindow(&rcText);

		// set hot spot rect
		m_rcHotSpot.left   = rc.left;
		m_rcHotSpot.top    = rc.bottom;
		m_rcHotSpot.right  = rcText.right;
		m_rcHotSpot.bottom = rcText.top;
		////}}
	}

	if (nItem != -1 && this->IsVisible())
	{
		TCITEM tcitem;
		tcitem.mask = TCIF_IMAGE;

		m_wndTab.GetItem(nItem, &tcitem);

		switch (tcitem.iImage)
		{
		case CWorkspaceBar::PfwImage :
			if (m_rcHotSpot.PtInRect(point))
			{
				TRACE("m_rcHotSpot.PtInRect\n");
#ifdef _DEBUG
				afxDump << " m_rcHotSpot " << m_rcHotSpot << " point " << point << "\n";
#endif
				HCURSOR cursor = ::LoadCursor(NULL, IDC_SIZENS);
				if (cursor)
				{
					::SetCursor(cursor);
				}
			}
			break;
		}
	}
	CSizingControlBar::OnMouseMove(nFlags, point);
}

BOOL CWorkspaceBar::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// Add your message handler code here and/or call default

// COMMENT: {8/15/2012 7:15:43 PM}	if (true)
// COMMENT: {8/15/2012 7:15:43 PM}	{
// COMMENT: {8/15/2012 7:15:43 PM}		HCURSOR cursor = ::LoadCursor(NULL, IDC_SIZENS);
// COMMENT: {8/15/2012 7:15:43 PM}		if (cursor)
// COMMENT: {8/15/2012 7:15:43 PM}		{
// COMMENT: {8/15/2012 7:15:43 PM}			::SetCursor(cursor);
// COMMENT: {8/15/2012 7:15:43 PM}			return TRUE;
// COMMENT: {8/15/2012 7:15:43 PM}		}
// COMMENT: {8/15/2012 7:15:43 PM}	}

	return CSizingControlBar::OnSetCursor(pWnd, nHitTest, message);
}

void CWorkspaceBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (m_rcHotSpot.PtInRect(point))
	{
		TRACE("m_rcHotSpot.PtInRect OnLButtonDown\n");
		HCURSOR cursor = ::LoadCursor(NULL, IDC_SIZENS);
		if (cursor)
		{
			::SetCapture(this->GetSafeHwnd());
			::SetCursor(cursor);
			ASSERT(!this->m_bResizing);
			this->m_bResizing = true;
			return;
		}
	}

	CSizingControlBar::OnLButtonDown(nFlags, point);
}

void CWorkspaceBar::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	
// COMMENT: {8/16/2012 10:35:01 PM}	////{{
// COMMENT: {8/16/2012 10:35:01 PM}	CRect rc;
// COMMENT: {8/16/2012 10:35:01 PM}	this->GetClientRect(&rc);
// COMMENT: {8/16/2012 10:35:01 PM}	this->m_wndTab.AdjustRect(FALSE, &rc);
// COMMENT: {8/16/2012 10:35:01 PM}
// COMMENT: {8/16/2012 10:35:01 PM}	// this->m_PosBarY = rc.bottom - 2 - this->m_cyRich;
// COMMENT: {8/16/2012 10:35:01 PM}	// this->m_cyRich  = rc.bottom - 2 - this->m_PosBarY
// COMMENT: {8/16/2012 10:35:01 PM}	// this->m_PosBarY = rc.bottom - 2 - this->m_cyRich;
// COMMENT: {8/16/2012 10:35:01 PM}	this->m_cyRich = rc.bottom - 2 - point.y;
// COMMENT: {8/16/2012 10:35:01 PM}
// COMMENT: {8/16/2012 10:35:01 PM}	CRect rcText(rc);
// COMMENT: {8/16/2012 10:35:01 PM}	TRACE("AdjustRect %d, %d, %d, %d\n", rc.left, rc.top, rc.right, rc.bottom);
// COMMENT: {8/16/2012 10:35:01 PM}
// COMMENT: {8/16/2012 10:35:01 PM}	rc.bottom = rc.bottom - 2 - this->m_cyRich - 2;
// COMMENT: {8/16/2012 10:35:01 PM}	TRACE("m_wndTreePfw %d, %d, %d, %d\n", rc.left, rc.top, rc.right, rc.bottom);
// COMMENT: {8/16/2012 10:35:01 PM}	m_wndTreePfw.MoveWindow(&rc);
// COMMENT: {8/16/2012 10:35:01 PM}
// COMMENT: {8/16/2012 10:35:01 PM}	rcText.top += rc.bottom;
// COMMENT: {8/16/2012 10:35:01 PM}	rcText.bottom -= 2;
// COMMENT: {8/16/2012 10:35:01 PM}	TRACE("m_wndRich Pfw %d, %d, %d, %d\n", rcText.left, rcText.top, rcText.right, rcText.bottom);
// COMMENT: {8/16/2012 10:35:01 PM}	m_wndRichPfw.MoveWindow(&rcText);
// COMMENT: {8/16/2012 10:35:01 PM}
// COMMENT: {8/16/2012 10:35:01 PM}	// set hot spot rect
// COMMENT: {8/16/2012 10:35:01 PM}	m_rcHotSpot.left   = rc.left;
// COMMENT: {8/16/2012 10:35:01 PM}	m_rcHotSpot.top    = rc.bottom;
// COMMENT: {8/16/2012 10:35:01 PM}	m_rcHotSpot.right  = rcText.right;
// COMMENT: {8/16/2012 10:35:01 PM}	m_rcHotSpot.bottom = rcText.top;
// COMMENT: {8/16/2012 10:35:01 PM}	////}}
// COMMENT: {8/16/2012 10:35:01 PM}
	if (this->m_bResizing)
	{
		this->m_bResizing = false;
		::ReleaseCapture();

		////{{
		CRect rc;
		this->GetClientRect(&rc);
		this->m_wndTab.AdjustRect(FALSE, &rc);

		// this->m_PosBarY = rc.bottom - 2 - this->m_cyRich;
		// this->m_cyRich  = rc.bottom - 2 - this->m_PosBarY
		// this->m_PosBarY = rc.bottom - 2 - this->m_cyRich;
		this->m_cyRich = rc.bottom - 2 - point.y;

		CRect rcText(rc);
		TRACE("AdjustRect %d, %d, %d, %d\n", rc.left, rc.top, rc.right, rc.bottom);

		rc.bottom = rc.bottom - 2 - this->m_cyRich - 2;
		TRACE("m_wndTreePfw %d, %d, %d, %d\n", rc.left, rc.top, rc.right, rc.bottom);
		m_wndTreePfw.MoveWindow(&rc);

		rcText.top += rc.bottom;
		rcText.bottom -= 2;
		TRACE("m_wndRich Pfw %d, %d, %d, %d\n", rcText.left, rcText.top, rcText.right, rcText.bottom);
		m_wndRichPfw.MoveWindow(&rcText);
		////{{
		m_wndRichPfw.Invalidate(FALSE);
		////}}

		// set hot spot rect
		m_rcHotSpot.left   = rc.left;
		m_rcHotSpot.top    = rc.bottom;
		m_rcHotSpot.right  = rcText.right;
		m_rcHotSpot.bottom = rcText.top;
		////}}
	}
	if (m_rcHotSpot.PtInRect(point))
	{
		TRACE("m_rcHotSpot.PtInRect OnLButtonUp\n");
		HCURSOR cursor = ::LoadCursor(NULL, IDC_SIZENS);
		if (cursor)
		{
			return;
		}
	}

	CSizingControlBar::OnLButtonUp(nFlags, point);
}
