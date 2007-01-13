// TreeCtrlNode.cpp :
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// Inline function declarations

#define _AFXCTL_INLINE inline


/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlNode


const CTreeCtrlNode& CTreeCtrlNode::operator =(const CTreeCtrlNode& posSrc)
{
	if(&posSrc != this){
		m_hTreeItem = posSrc.m_hTreeItem;
		m_pTree = posSrc.m_pTree;
	}
	return *this;
}


CTreeCtrlNode CTreeCtrlNode::_Insert(LPCTSTR strItem,int nImageIndex,HTREEITEM hAfter)
{
	TVINSERTSTRUCT ins;
	ins.hParent = m_hTreeItem;
	ins.hInsertAfter = hAfter;
	ins.item.mask = TVIF_TEXT;
	ins.item.pszText = (LPTSTR) strItem;
	if(nImageIndex != -1){
		ins.item.mask |= TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		ins.item.iImage = nImageIndex;
		ins.item.iSelectedImage = nImageIndex;
	}
	return CTreeCtrlNode(m_pTree->InsertItem(&ins), m_pTree);
}

int CTreeCtrlNode::GetImageID()
{
	TVITEM item;
	item.mask = TVIF_HANDLE | TVIF_IMAGE;
	item.hItem = m_hTreeItem;
	m_pTree->GetItem(&item);
	return item.iImage;
}

// Added SRC
int CTreeCtrlNode::GetLevel()
{
    ASSERT(::IsWindow(m_pTree->m_hWnd));
    CTreeCtrlNode parent;
    int retval = 0;
    for(parent = GetParent();
		parent != NULL;
		parent = parent.GetParent())
	{
            retval++;
    }
    return retval;
}

// Added SRC
CTreeCtrlNode CTreeCtrlNode::GetLastChild()
{
    ASSERT(::IsWindow(m_pTree->m_hWnd));
	CTreeCtrlNode child = GetChild();
	if (child != 0)
	{
		for(; child.GetNextSibling(); child = child.GetNextSibling())
		{
		}
	}
    return child;
}

// Added SRC
bool CTreeCtrlNode::IsNodeAncestor(const CTreeCtrlNode& node)const
{
	if (m_pTree == NULL || node.m_pTree == NULL) return false;
	if (m_hTreeItem == 0 || node.m_hTreeItem == 0) return false;
	if (m_pTree != node.m_pTree) return false;

	HTREEITEM hItem = m_hTreeItem;
	for (; hItem != 0; hItem = m_pTree->GetParentItem(hItem))
	{
		if (hItem == node.m_hTreeItem) return true;
	}
	return false;
}

// Added SRC
bool CTreeCtrlNode::IsNodeDescendant(const CTreeCtrlNode& node)const
{
	return (node.IsNodeAncestor(*this));
}

// Added SRC
int CTreeCtrlNode::GetIndex(const CTreeCtrlNode& child)const
{
	int nIndex = -1;	// not a child of this
	if (m_pTree == NULL || child.m_pTree == NULL) return nIndex;
	if (m_hTreeItem == 0 || child.m_hTreeItem == 0) return nIndex;
	if (m_pTree != child.m_pTree) return nIndex;

	HTREEITEM hItem = child.m_hTreeItem;

	if (m_hTreeItem != ((CTreeCtrl*)m_pTree)->GetParentItem(hItem))
	{
		return nIndex;
	}

	nIndex = 0;
	for (hItem = ((CTreeCtrl*)m_pTree)->GetPrevSiblingItem(hItem); hItem != 0;)
	{
		hItem = ((CTreeCtrl*)m_pTree)->GetPrevSiblingItem(hItem);
		nIndex++;
	}
	return nIndex;
}

// Added SRC
int CTreeCtrlNode::GetChildCount()const
{
	int nCount = 0;
	if (m_pTree == NULL) return nCount;
	if (m_hTreeItem == 0) return nCount;

	HTREEITEM hChildItem = ((CTreeCtrl*)m_pTree)->GetChildItem(m_hTreeItem);

	while (hChildItem != NULL)
	{
		nCount++;
		hChildItem = ((CTreeCtrl*)m_pTree)->GetNextItem(hChildItem, TVGN_NEXT);
	}
	return nCount;
}

// Added SRC
int CTreeCtrlNode::GetSiblingCount()const
{
	int nCount = 1;	// this node counts as one
	if (m_pTree == NULL) return nCount;
	if (m_hTreeItem == 0) return nCount;

	HTREEITEM hSiblingItem = ((CTreeCtrl*)m_pTree)->GetNextItem(m_hTreeItem, TVGN_NEXT);

	// count next siblings
	while (hSiblingItem != NULL)
	{
		nCount++;
		hSiblingItem = ((CTreeCtrl*)m_pTree)->GetNextItem(hSiblingItem, TVGN_NEXT);
	}

	hSiblingItem = ((CTreeCtrl*)m_pTree)->GetNextItem(m_hTreeItem, TVGN_PREVIOUS);

	// count previous siblings
	while (hSiblingItem != NULL)
	{
		nCount++;
		hSiblingItem = ((CTreeCtrl*)m_pTree)->GetNextItem(hSiblingItem, TVGN_PREVIOUS);
	}
	return nCount;
}
CTreeCtrlNode CTreeCtrlNode::GetChildAt(int nIndex)const
{
	ASSERT(nIndex >= 0);
	CTreeCtrlNode child;
	if (m_pTree == NULL) return child;
	if (m_hTreeItem == 0) return child;

	HTREEITEM hChildItem = ((CTreeCtrl*)m_pTree)->GetChildItem(m_hTreeItem);

	while (hChildItem != NULL && nIndex > 0)
	{
		hChildItem = ((CTreeCtrl*)m_pTree)->GetNextItem(hChildItem, TVGN_NEXT);
		nIndex--;
	}
	if (hChildItem)
	{
		child.m_hTreeItem = hChildItem;
		child.m_pTree = m_pTree;
	}
	return child;
}


/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlEx

// Rest handled by inlines
