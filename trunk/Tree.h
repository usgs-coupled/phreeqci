// Tree.h :
//
// $Id$
//////////////////////////////////////////////////////////////////////

#ifndef __TREE_H__
#define __TREE_H__

#ifdef _AFX_NO_AFXCMN_SUPPORT
	#error Windows Common Control classes not supported in this library variant.
#endif

#ifndef __AFXWIN_H__
	#include <afxwin.h>
#endif

/////////////////////////////////////////////////////////////////////////////
// 

// forward declarations
class CTreeCtrlNode;
class CTreeCtrlEx;


/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlNode

class CTreeCtrlNode
{
	// Attributes
protected:
	HTREEITEM   m_hTreeItem;
	CTreeCtrlEx *m_pTree;

	// Implementation
protected:
	CTreeCtrlNode CTreeCtrlNode::_Insert(LPCTSTR strItem,int nImageIndex,HTREEITEM hAfter);

	// Operation
public:
	int GetLevel();
	CTreeCtrlNode GetLastChild();
	bool IsNodeDescendant(const CTreeCtrlNode& node)const;
	bool IsNodeAncestor(const CTreeCtrlNode& node)const;
	int GetIndex(const CTreeCtrlNode& node)const;
	int GetChildCount()const;
	int GetSiblingCount()const;
	CTreeCtrlNode CTreeCtrlNode::GetChildAt(int nIndex)const;


	CTreeCtrlNode();
	CTreeCtrlNode(HTREEITEM hTreeItem, CTreeCtrlEx* pTree);
	CTreeCtrlNode(const CTreeCtrlNode& posSrc);
	~CTreeCtrlNode();
	const CTreeCtrlNode& operator =(const CTreeCtrlNode& posSrc);
	operator HTREEITEM();

	CTreeCtrlNode InsertAfter(LPCTSTR strItem,HTREEITEM hAfter,int nImageIndex = -1);
	CTreeCtrlNode AddHead(LPCTSTR strItem,int nImageIndex = -1);
	CTreeCtrlNode AddTail(LPCTSTR strItem,int nImageIndex = -1);

	int GetImageID();

	BOOL GetRect(LPRECT lpRect, BOOL bTextOnly);
	CTreeCtrlNode GetNext(UINT nCode);
	CTreeCtrlNode GetChild();
	CTreeCtrlNode GetNextSibling();
	CTreeCtrlNode GetPrevSibling();
	CTreeCtrlNode GetParent();
	CTreeCtrlNode GetFirstVisible();
	CTreeCtrlNode GetNextVisible();
	CTreeCtrlNode GetPrevVisible();
	CTreeCtrlNode GetSelected();
	CTreeCtrlNode GetDropHilight();
	CTreeCtrlNode GetRoot();
	CString GetText();
	BOOL GetImage(int& nImage, int& nSelectedImage);
	UINT GetState(UINT nStateMask);
	DWORD GetData();
	BOOL Set(UINT nMask, LPCTSTR lpszItem, int nImage,
		int nSelectedImage, UINT nState, UINT nStateMask, LPARAM lParam);
	BOOL SetText(LPCTSTR lpszItem);
	BOOL SetImage(int nImage, int nSelectedImage);
	BOOL SetState(UINT nState, UINT nStateMask);
	BOOL SetData(DWORD dwData);
	BOOL HasChildren();

	// Operations
	BOOL Delete();

	BOOL Expand(UINT nCode = TVE_EXPAND);
	BOOL Select(UINT nCode);
	BOOL Select();
	BOOL SelectDropTarget();
	BOOL SelectSetFirstVisible();
	CEdit* EditLabel();
	CImageList* CreateDragImage();
	BOOL SortChildren();
	BOOL EnsureVisible();
};


/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlEx

class CTreeCtrlEx : public CTreeCtrl
{
	DECLARE_DYNAMIC(CTreeCtrlEx)
	// Attributes
protected:

	// Operation
public:
	CTreeCtrlEx();
	~CTreeCtrlEx();
	CImageList* SetImageList(CImageList* pImageList, int nImageListType = TVSIL_NORMAL);

	CTreeCtrlNode GetNextItem(HTREEITEM hItem, UINT nCode);
	CTreeCtrlNode GetChildItem(HTREEITEM hItem);
	CTreeCtrlNode GetNextSiblingItem(HTREEITEM hItem);
	CTreeCtrlNode GetPrevSiblingItem(HTREEITEM hItem);
	CTreeCtrlNode GetParentItem(HTREEITEM hItem);
	CTreeCtrlNode GetFirstVisibleItem();
	CTreeCtrlNode GetNextVisibleItem(HTREEITEM hItem);
	CTreeCtrlNode GetPrevVisibleItem(HTREEITEM hItem);
	CTreeCtrlNode GetSelectedItem();
	CTreeCtrlNode GetDropHilightItem();
	CTreeCtrlNode GetRootItem();
	CTreeCtrlNode InsertItem(LPTVINSERTSTRUCT lpInsertStruct);
	CTreeCtrlNode InsertItem(UINT nMask, LPCTSTR lpszItem, int nImage,
		int nSelectedImage, UINT nState, UINT nStateMask, LPARAM lParam,
		HTREEITEM hParent, HTREEITEM hInsertAfter);
	CTreeCtrlNode InsertItem(LPCTSTR lpszItem, HTREEITEM hParent = TVI_ROOT,
		HTREEITEM hInsertAfter = TVI_LAST);
	CTreeCtrlNode InsertItem(LPCTSTR lpszItem, int nImage, int nSelectedImage,
		HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);
	CTreeCtrlNode HitTest(CPoint pt, UINT* pFlags = NULL);
	CTreeCtrlNode HitTest(TVHITTESTINFO* pHitTestInfo);
};

/////////////////////////////////////////////////////////////////////////////

#include "Tree.inl"

#endif //__TREE_H__
