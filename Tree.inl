// Tree.inl
//
// $Id$
//////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlNode TREE_INLINE functions

#define TREE_INLINE inline

TREE_INLINE CTreeCtrlNode::CTreeCtrlNode() : m_hTreeItem(NULL),   m_pTree(NULL)
	{ }
TREE_INLINE CTreeCtrlNode::CTreeCtrlNode(HTREEITEM hTreeItem,CTreeCtrlEx* pTree) :    m_hTreeItem(hTreeItem), m_pTree(pTree)
	{ }
TREE_INLINE CTreeCtrlNode::CTreeCtrlNode(const CTreeCtrlNode& posSrc)
	{ *this = posSrc; }
TREE_INLINE CTreeCtrlNode::~CTreeCtrlNode()
	{ }
TREE_INLINE CTreeCtrlNode::operator HTREEITEM()
	{ return m_hTreeItem; }
TREE_INLINE CTreeCtrlNode CTreeCtrlNode::InsertAfter(LPCTSTR strItem,HTREEITEM hAfter,int nImageIndex)
	{ return _Insert(strItem,nImageIndex,hAfter); }
TREE_INLINE CTreeCtrlNode CTreeCtrlNode::AddHead(LPCTSTR strItem,int nImageIndex)
	{ return _Insert(strItem,nImageIndex,TVI_FIRST); }
TREE_INLINE CTreeCtrlNode CTreeCtrlNode::AddTail(LPCTSTR strItem,int nImageIndex)
	{ return _Insert(strItem,nImageIndex,TVI_LAST); }
TREE_INLINE BOOL CTreeCtrlNode::GetRect(LPRECT lpRect, BOOL bTextOnly)
	{ return m_pTree->GetItemRect(m_hTreeItem,lpRect,bTextOnly); }
TREE_INLINE CTreeCtrlNode CTreeCtrlNode::GetNext(UINT nCode)
	{ return m_pTree->GetNextItem(m_hTreeItem,nCode); }
TREE_INLINE CTreeCtrlNode CTreeCtrlNode::GetChild()
	{ return m_pTree->GetChildItem(m_hTreeItem); }
TREE_INLINE CTreeCtrlNode CTreeCtrlNode::GetNextSibling()
	{ return m_pTree->GetNextSiblingItem(m_hTreeItem); }
TREE_INLINE CTreeCtrlNode CTreeCtrlNode::GetPrevSibling()
	{ return m_pTree->GetPrevSiblingItem(m_hTreeItem); }
TREE_INLINE CTreeCtrlNode CTreeCtrlNode::GetParent()
	{ return m_pTree->GetParentItem(m_hTreeItem); }
TREE_INLINE CTreeCtrlNode CTreeCtrlNode::GetFirstVisible()
	{ return m_pTree->GetFirstVisibleItem(); }
TREE_INLINE CTreeCtrlNode CTreeCtrlNode::GetNextVisible()
	{ return m_pTree->GetNextVisibleItem(m_hTreeItem); }
TREE_INLINE CTreeCtrlNode CTreeCtrlNode::GetPrevVisible()
	{ return m_pTree->GetPrevVisibleItem(m_hTreeItem); }
TREE_INLINE CTreeCtrlNode CTreeCtrlNode::GetSelected()
	{ return m_pTree->GetSelectedItem(); }
TREE_INLINE CTreeCtrlNode CTreeCtrlNode::GetDropHilight()
	{ return m_pTree->GetDropHilightItem(); }
TREE_INLINE CTreeCtrlNode CTreeCtrlNode::GetRoot()
	{ return m_pTree->GetRootItem(); }
TREE_INLINE CString CTreeCtrlNode::GetText()
	{ return m_pTree->GetItemText(m_hTreeItem); }
TREE_INLINE BOOL CTreeCtrlNode::GetImage(int& nImage, int& nSelectedImage)
	{ return m_pTree->GetItemImage(m_hTreeItem,nImage,nSelectedImage); }
TREE_INLINE UINT CTreeCtrlNode::GetState(UINT nStateMask)
	{ return m_pTree->GetItemState(m_hTreeItem,nStateMask); }
TREE_INLINE DWORD CTreeCtrlNode::GetData()
	{ return m_pTree->GetItemData(m_hTreeItem); }
//TREE_INLINE BOOL SetItem(UINT nMask, LPCTSTR lpszItem, int nImage,
//  int nSelectedImage, UINT nState,nStateMask,lParam);
TREE_INLINE BOOL CTreeCtrlNode::SetText(LPCTSTR lpszItem)
	{ return m_pTree->SetItemText(m_hTreeItem,lpszItem); }
TREE_INLINE BOOL CTreeCtrlNode::SetImage(int nImage, int nSelectedImage)
	{ return m_pTree->SetItemImage(m_hTreeItem,nImage,nSelectedImage); }
TREE_INLINE BOOL CTreeCtrlNode::SetState(UINT nState, UINT nStateMask)
	{ return m_pTree->SetItemState(m_hTreeItem,nState,nStateMask); }
TREE_INLINE BOOL CTreeCtrlNode::SetData(DWORD dwData)
	{ return m_pTree->SetItemData(m_hTreeItem,dwData); }
TREE_INLINE BOOL CTreeCtrlNode::HasChildren()
	{ return m_pTree->ItemHasChildren(m_hTreeItem); }
// Operations
TREE_INLINE BOOL CTreeCtrlNode::Delete()
	{ return m_pTree->DeleteItem(m_hTreeItem); }
TREE_INLINE BOOL CTreeCtrlNode::Expand(UINT nCode)
	{ return m_pTree->Expand(m_hTreeItem,nCode); }
TREE_INLINE BOOL CTreeCtrlNode::Select(UINT nCode)
	{ return m_pTree->Select(m_hTreeItem,nCode); }
TREE_INLINE BOOL CTreeCtrlNode::Select()
	{ return m_pTree->SelectItem(m_hTreeItem); }
TREE_INLINE BOOL CTreeCtrlNode::SelectDropTarget()
	{ return m_pTree->SelectDropTarget(m_hTreeItem); }
TREE_INLINE BOOL CTreeCtrlNode::SelectSetFirstVisible()
	{ return m_pTree->SelectSetFirstVisible(m_hTreeItem); }
TREE_INLINE CEdit* CTreeCtrlNode::EditLabel()
	{ return m_pTree->EditLabel(m_hTreeItem); }
TREE_INLINE CImageList* CTreeCtrlNode::CreateDragImage()
	{ return m_pTree->CreateDragImage(m_hTreeItem); }
TREE_INLINE BOOL CTreeCtrlNode::SortChildren()
	{ return m_pTree->SortChildren(m_hTreeItem); }
TREE_INLINE BOOL CTreeCtrlNode::EnsureVisible()
	{ return m_pTree->EnsureVisible(m_hTreeItem); }

/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlEx TREE_INLINE functions

TREE_INLINE CTreeCtrlEx::CTreeCtrlEx() : CTreeCtrl()
	{ }
TREE_INLINE CTreeCtrlEx::~CTreeCtrlEx()
	{ }
TREE_INLINE CImageList* CTreeCtrlEx::SetImageList(CImageList* pImageList, int nImageListType)
	{ return CTreeCtrl::SetImageList(pImageList,nImageListType); }
TREE_INLINE CTreeCtrlNode CTreeCtrlEx::GetNextItem(HTREEITEM hItem, UINT nCode)
	{ return CTreeCtrlNode(CTreeCtrl::GetNextItem(hItem,nCode),this); }
TREE_INLINE CTreeCtrlNode CTreeCtrlEx::GetChildItem(HTREEITEM hItem)
	{ return CTreeCtrlNode(CTreeCtrl::GetChildItem(hItem),this); }
TREE_INLINE CTreeCtrlNode CTreeCtrlEx::GetNextSiblingItem(HTREEITEM hItem)
	{ return CTreeCtrlNode(CTreeCtrl::GetNextSiblingItem(hItem),this); }
TREE_INLINE CTreeCtrlNode CTreeCtrlEx::GetPrevSiblingItem(HTREEITEM hItem)
	{ return CTreeCtrlNode(CTreeCtrl::GetPrevSiblingItem(hItem),this); }
TREE_INLINE CTreeCtrlNode CTreeCtrlEx::GetParentItem(HTREEITEM hItem)
	{ return CTreeCtrlNode(CTreeCtrl::GetParentItem(hItem),this); }
TREE_INLINE CTreeCtrlNode CTreeCtrlEx::GetFirstVisibleItem()
	{ return CTreeCtrlNode(CTreeCtrl::GetFirstVisibleItem(),this); }
TREE_INLINE CTreeCtrlNode CTreeCtrlEx::GetNextVisibleItem(HTREEITEM hItem)
	{ return CTreeCtrlNode(CTreeCtrl::GetNextVisibleItem(hItem),this); }
TREE_INLINE CTreeCtrlNode CTreeCtrlEx::GetPrevVisibleItem(HTREEITEM hItem)
	{ return CTreeCtrlNode(CTreeCtrl::GetPrevVisibleItem(hItem),this); }
TREE_INLINE CTreeCtrlNode CTreeCtrlEx::GetSelectedItem()
	{ return CTreeCtrlNode(CTreeCtrl::GetSelectedItem(),this); }
TREE_INLINE CTreeCtrlNode CTreeCtrlEx::GetDropHilightItem()
	{ return CTreeCtrlNode(CTreeCtrl::GetDropHilightItem(),this); }
TREE_INLINE CTreeCtrlNode CTreeCtrlEx::GetRootItem()
	{ return CTreeCtrlNode(CTreeCtrl::GetRootItem(),this); }
TREE_INLINE CTreeCtrlNode CTreeCtrlEx::InsertItem(LPTVINSERTSTRUCT lpInsertStruct)
	{ return CTreeCtrlNode(CTreeCtrl::InsertItem(lpInsertStruct),this); }
TREE_INLINE CTreeCtrlNode CTreeCtrlEx::InsertItem(UINT nMask, LPCTSTR lpszItem, int nImage,
	int nSelectedImage, UINT nState, UINT nStateMask, LPARAM lParam,
	HTREEITEM hParent, HTREEITEM hInsertAfter)
	{ return CTreeCtrlNode(CTreeCtrl::InsertItem(nMask, lpszItem, nImage,
		nSelectedImage, nState, nStateMask, lParam, hParent, hInsertAfter),this); }
TREE_INLINE CTreeCtrlNode CTreeCtrlEx::InsertItem(LPCTSTR lpszItem, HTREEITEM hParent,HTREEITEM hInsertAfter)
	{ return CTreeCtrlNode(CTreeCtrl::InsertItem(lpszItem, hParent, hInsertAfter),this); }
TREE_INLINE CTreeCtrlNode CTreeCtrlEx::InsertItem(LPCTSTR lpszItem, int nImage, int nSelectedImage,
		HTREEITEM hParent, HTREEITEM hInsertAfter)
	{ return CTreeCtrlNode(CTreeCtrl::InsertItem(lpszItem, nImage, nSelectedImage,
		hParent, hInsertAfter),this); }
TREE_INLINE CTreeCtrlNode CTreeCtrlEx::HitTest(CPoint pt, UINT* pFlags)
	{ return CTreeCtrlNode(CTreeCtrl::HitTest(pt,pFlags),this); }
TREE_INLINE CTreeCtrlNode CTreeCtrlEx::HitTest(TVHITTESTINFO* pHitTestInfo)
	{ return CTreeCtrlNode(CTreeCtrl::HitTest(pHitTestInfo),this); }
