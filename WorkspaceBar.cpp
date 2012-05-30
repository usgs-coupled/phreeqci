// WorkspaceBar.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "WorkspaceBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar

IMPLEMENT_DYNAMIC(CWorkspaceBar, baseWorkspaceBar);

CWorkspaceBar::CWorkspaceBar()
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

	// Create Errors tree
	if (!m_wndTreeErr.CWnd::Create(WC_TREEVIEW, _T("Errors"),
		WS_CHILD | /*WS_VISIBLE |*/ WS_BORDER | WS_CLIPSIBLINGS						// standard window styles
		| TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS,		// TreeView styles
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

	if ( m_wndTreeErr.GetSafeHwnd() )
	{
		m_wndTreeErr.MoveWindow(&rc);
	}
}

void CWorkspaceBar::OnSelChange(NMHDR* /*pNMHDR*/, LRESULT* /*pResult*/)
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
		m_wndTreeIn.ShowWindow(SW_SHOW);
		m_wndTreeIn.SetFocus();

		// hide other tabs
		m_wndTreeOut.ShowWindow(SW_HIDE);
		m_wndTreeErr.ShowWindow(SW_HIDE);
		break;

	case OutputImage :
		// display output tree
		ASSERT(m_wndTreeOut.GetRootItem().HasChildren());
		m_wndTreeOut.ShowWindow(SW_SHOW);
		m_wndTreeOut.SetFocus();

		// hide other tabs
		m_wndTreeIn.ShowWindow(SW_HIDE);
		m_wndTreeErr.ShowWindow(SW_HIDE);
		break;

	case ErrorImage :
		// display output tree
		ASSERT(m_wndTreeErr.GetRootItem().HasChildren());
		m_wndTreeErr.ShowWindow(SW_SHOW);
		m_wndTreeErr.SetFocus();

		// hide other tabs
		m_wndTreeIn.ShowWindow(SW_HIDE);
		m_wndTreeOut.ShowWindow(SW_HIDE);
		break;
	}
	// refresh tab control
	m_wndTab.RedrawWindow();
}

CTreeCtrlNode CWorkspaceBar::AddInputFileNode(LPCTSTR lpszLabel)
{
	if ( !m_wndTreeIn.GetRootItem().HasChildren() )
	{
		// add input tab
		m_wndTab.InsertItem(TCIF_TEXT | TCIF_IMAGE | TCIF_PARAM, InputTab,
			_T("Input"), InputImage, IDC_TV_WS_IN);

		// resize tree
		CRect rc;
		GetClientRect(rc);

		int cx = rc.Width();
		int cy = rc.Height();
		SetRect(&rc, 0, 0, cx, cy);
		m_wndTab.AdjustRect(FALSE, &rc);
		rc.bottom -= 2;
		m_wndTreeIn.MoveWindow(&rc);
	}

	CTreeCtrlNode node =  m_wndTreeIn.InsertItem(lpszLabel, CTreeCtrlIn::fileImage,
		CTreeCtrlIn::fileImage, m_wndTreeIn.GetRootItem());

	m_wndTab.SetCurSel(GetTabIndex(InputImage));
	OnSelChange(NULL, NULL);

	return node;

}

CTreeCtrlNode CWorkspaceBar::AddOutputFileNode(LPCTSTR lpszLabel)
{
	if ( !m_wndTreeOut.GetRootItem().HasChildren() )
	{
		ASSERT( m_wndTreeOut.IsWindowVisible() == FALSE );
		ASSERT( GetTabIndex(OutputImage) == -1);
		// add output tab
		m_wndTab.InsertItem(TCIF_TEXT | TCIF_IMAGE | TCIF_PARAM, OutputTab,
			_T("Output"), OutputImage, IDC_TV_WS_OUT);

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

CTreeCtrlNode CWorkspaceBar::AddErrorFileNode(LPCTSTR lpszLabel)
{
	if ( !m_wndTreeErr.GetRootItem().HasChildren() )
	{
		// add errors tab
		m_wndTab.InsertItem(TCIF_TEXT | TCIF_IMAGE | TCIF_PARAM, ErrorTab,
			_T("Errors/Warnings"), ErrorImage, IDC_TV_WS_ERR);

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

	CTreeCtrlNode node =  m_wndTreeErr.InsertItem(lpszLabel, CTreeCtrlErr::fileImage,
		CTreeCtrlErr::fileImage, m_wndTreeErr.GetRootItem());

	////m_wndTab.SetCurSel(GetTabIndex(ErrorImage));
	OnSelChange(NULL, NULL);

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
		case InputImage :
			if (m_wndTreeIn.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			{
				return TRUE;
			}
			break;
		case OutputImage :
			if (m_wndTreeOut.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			{
				return TRUE;
			}
			break;
		case ErrorImage :
			if (m_wndTreeErr.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
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
		m_wndTab.InsertItem(TCIF_TEXT | TCIF_IMAGE | TCIF_PARAM, ErrorTab,
			_T("Errors/Warnings"), ErrorImage, IDC_TV_WS_ERR);

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
	if ( !m_wndTreeIn.m_baseNode.HasChildren() )
	{
		ASSERT(!m_wndTreeIn.IsWindowVisible());

		// add input tab
		m_wndTab.InsertItem(TCIF_TEXT | TCIF_IMAGE | TCIF_PARAM, InputTab,
			_T("Input"), InputImage, IDC_TV_WS_IN);

		// resize tree
		CRect rc;
		GetClientRect(rc);

		int cx = rc.Width();
		int cy = rc.Height();
		SetRect(&rc, 0, 0, cx, cy);
		m_wndTab.AdjustRect(FALSE, &rc);
		rc.bottom -= 2;
		m_wndTreeIn.MoveWindow(&rc);
	}

	int nErrors = SyncInputTrees(pDoc);

	if (nErrors > 0)
	{
		// if errors select error tab
		m_wndTab.SetCurSel(GetTabIndex(ErrorImage));
		OnSelChange(NULL, NULL);
		::AfxMessageBox(_T("Input file contains errors"), MB_OK);
	}
	else
	{
		// if no errors select input tab
		m_wndTab.SetCurSel(GetTabIndex(InputImage));
		OnSelChange(NULL, NULL);
	}
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

	CTreeCtrlNode inputNode = m_wndTreeIn.GetFileNode(pDoc);

	ASSERT( inputNode );
	VERIFY( inputNode.Delete() );

	// hide tab if no more files
	if (!m_wndTreeIn.GetRootItem().HasChildren())
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
