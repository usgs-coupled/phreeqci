// CheckListCtrl.cpp : implementation file
//
// $Date: 2001/09/14 20:11:59 $
// $Revision: 1.1.1.26 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "CheckListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckListCtrl

CCheckListCtrl::CCheckListCtrl()
{
	m_pImageList = NULL;
}

CCheckListCtrl::~CCheckListCtrl()
{
	delete m_pImageList;
}


BEGIN_MESSAGE_MAP(CCheckListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CCheckListCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_CHAR()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckListCtrl message handlers

void CCheckListCtrl::PreSubclassWindow() 
{
	// must be list style
	ASSERT(GetStyle() & LVS_LIST);

	ASSERT(m_pImageList == NULL);
	m_pImageList = new CImageList();

	ASSERT_VALID(m_pImageList);

	m_pImageList->Create(IDB_CHECKS, 14, 0, RGB(255, 0, 255));

	SetImageList(m_pImageList, LVSIL_STATE);
	
	// Add your specialized code here and/or call the base class	
	CListCtrl::PreSubclassWindow();
}

void CCheckListCtrl::SetCheckBox(int nItem, int nCheckState)
{
	ASSERT(::IsWindow(m_hWnd));

	ASSERT(nCheckState == BST_UNCHECKED || nCheckState == BST_CHECKED
		|| nCheckState == BST_INDETERMINATE);

	SetItemState(nItem, INDEXTOSTATEIMAGEMASK(nCheckState + 1), LVIS_STATEIMAGEMASK);
}

int CCheckListCtrl::GetCheckBox(int nItem)const
{
	ASSERT(::IsWindow(m_hWnd));

	ASSERT(GetItemState(nItem, LVIS_STATEIMAGEMASK));

	int nReturn = BST_INDETERMINATE;

	switch (GetItemState(nItem, LVIS_STATEIMAGEMASK))
	{
	case INDEXTOSTATEIMAGEMASK(BST_UNCHECKED + 1):
		nReturn = BST_UNCHECKED;
		break;
	case INDEXTOSTATEIMAGEMASK(BST_CHECKED + 1):
		nReturn = BST_CHECKED;
		break;
	case INDEXTOSTATEIMAGEMASK(BST_INDETERMINATE + 1):
		nReturn = BST_INDETERMINATE;
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	return nReturn;
}

int CCheckListCtrl::InsertItem(int nItem, LPCTSTR lpszItem)
{
	ASSERT(::IsWindow(m_hWnd));
	LVITEM item;
	item.mask = LVIF_TEXT|LVIF_STATE;
	item.iItem = nItem;
	item.iSubItem = 0;
	item.pszText = (LPTSTR)lpszItem;
	item.state = INDEXTOSTATEIMAGEMASK(1);
	item.stateMask = LVIS_STATEIMAGEMASK;
	item.iImage = 0;
	item.lParam = 0;
	return CListCtrl::InsertItem(&item);
}

void CCheckListCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	ASSERT(::IsWindow(m_hWnd));
	SetFocus();

	LVHITTESTINFO hitInfo;
	hitInfo.pt = point;

	int nItem = HitTest(&hitInfo);
	if (hitInfo.flags & LVHT_ONITEMSTATEICON)
	{
		switch (GetItemState(nItem, LVIS_STATEIMAGEMASK))
		{
		case INDEXTOSTATEIMAGEMASK(BST_UNCHECKED + 1):
			SetCheckBox(nItem, BST_CHECKED);
			break;
		case INDEXTOSTATEIMAGEMASK(BST_CHECKED + 1):
			SetCheckBox(nItem, BST_UNCHECKED);
			break;
		case INDEXTOSTATEIMAGEMASK(BST_INDETERMINATE + 1):
			// do nothing
			break;
		default:
			ASSERT(FALSE);
		}
	}

// COMMENT: {4/3/2001 4:29:35 PM}	ASSERT(::IsWindow(m_hWnd));
// COMMENT: {4/3/2001 4:29:35 PM}	SetFocus();
// COMMENT: {4/3/2001 4:29:35 PM}
// COMMENT: {4/3/2001 4:29:35 PM}	LVHITTESTINFO hitInfo;
// COMMENT: {4/3/2001 4:29:35 PM}	hitInfo.pt = point;
// COMMENT: {4/3/2001 4:29:35 PM}
// COMMENT: {4/3/2001 4:29:35 PM}	int nItem = HitTest(&hitInfo);
// COMMENT: {4/3/2001 4:29:35 PM}	if (hitInfo.flags & LVHT_ONITEMSTATEICON)
// COMMENT: {4/3/2001 4:29:35 PM}	{
// COMMENT: {4/3/2001 4:29:35 PM}		switch (GetItemState(nItem, LVIS_STATEIMAGEMASK))
// COMMENT: {4/3/2001 4:29:35 PM}		{
// COMMENT: {4/3/2001 4:29:35 PM}		case INDEXTOSTATEIMAGEMASK(BST_UNCHECKED + 1):
// COMMENT: {4/3/2001 4:29:35 PM}			if (GetItem
// COMMENT: {4/3/2001 4:29:35 PM}			{
// COMMENT: {4/3/2001 4:29:35 PM}				POSITION pos = GetFirstSelectedItemPosition();
// COMMENT: {4/3/2001 4:29:35 PM}				while (pos)
// COMMENT: {4/3/2001 4:29:35 PM}				{
// COMMENT: {4/3/2001 4:29:35 PM}					int nItem = GetNextSelectedItem(pos);
// COMMENT: {4/3/2001 4:29:35 PM}					if (GetItemState(nItem, LVIS_STATEIMAGEMASK) != INDEXTOSTATEIMAGEMASK(BST_INDETERMINATE + 1))
// COMMENT: {4/3/2001 4:29:35 PM}					{
// COMMENT: {4/3/2001 4:29:35 PM}						SetCheckBox(nItem, BST_CHECKED);
// COMMENT: {4/3/2001 4:29:35 PM}					}
// COMMENT: {4/3/2001 4:29:35 PM}				}
// COMMENT: {4/3/2001 4:29:35 PM}			}
// COMMENT: {4/3/2001 4:29:35 PM}			break;
// COMMENT: {4/3/2001 4:29:35 PM}		case INDEXTOSTATEIMAGEMASK(BST_CHECKED + 1):
// COMMENT: {4/3/2001 4:29:35 PM}			{
// COMMENT: {4/3/2001 4:29:35 PM}				POSITION pos = GetFirstSelectedItemPosition();
// COMMENT: {4/3/2001 4:29:35 PM}				while (pos)
// COMMENT: {4/3/2001 4:29:35 PM}				{
// COMMENT: {4/3/2001 4:29:35 PM}					int nItem = GetNextSelectedItem(pos);
// COMMENT: {4/3/2001 4:29:35 PM}					if (GetItemState(nItem, LVIS_STATEIMAGEMASK) != INDEXTOSTATEIMAGEMASK(BST_INDETERMINATE + 1))
// COMMENT: {4/3/2001 4:29:35 PM}					{
// COMMENT: {4/3/2001 4:29:35 PM}						SetCheckBox(nItem, BST_UNCHECKED);
// COMMENT: {4/3/2001 4:29:35 PM}					}
// COMMENT: {4/3/2001 4:29:35 PM}				}
// COMMENT: {4/3/2001 4:29:35 PM}			}
// COMMENT: {4/3/2001 4:29:35 PM}			break;
// COMMENT: {4/3/2001 4:29:35 PM}		case INDEXTOSTATEIMAGEMASK(BST_INDETERMINATE + 1):
// COMMENT: {4/3/2001 4:29:35 PM}			// do nothing
// COMMENT: {4/3/2001 4:29:35 PM}			break;
// COMMENT: {4/3/2001 4:29:35 PM}		default:
// COMMENT: {4/3/2001 4:29:35 PM}			ASSERT(FALSE);
// COMMENT: {4/3/2001 4:29:35 PM}		}
// COMMENT: {4/3/2001 4:29:35 PM}	}


	// Add your message handler code here and/or call default	
	CListCtrl::OnLButtonDown(nFlags, point);
}

void CCheckListCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CWaitCursor* pWait = NULL;
	POSITION pos;
	switch (nChar) 
	{
	case VK_SPACE:
		{
			if (GetSelectedCount() > 100)
			{
				//pWait = new CWaitCursor();
			}
			pos = GetFirstSelectedItemPosition();
			while (pos)
			{
				int nItem = GetNextSelectedItem(pos);
				switch (GetItemState(nItem, LVIS_STATEIMAGEMASK))
				{
				case INDEXTOSTATEIMAGEMASK(BST_UNCHECKED + 1):
					SetCheckBox(nItem, BST_CHECKED);
					break;
				case INDEXTOSTATEIMAGEMASK(BST_CHECKED + 1):
					SetCheckBox(nItem, BST_UNCHECKED);
					break;
				case INDEXTOSTATEIMAGEMASK(BST_INDETERMINATE + 1):
					// do nothing
					break;
				default:
					ASSERT(FALSE);
				}
			}
			if (pWait) delete pWait;
		}
		break;
	case 0x01:	// Ctrl+A
		// select all
		int cItems = GetItemCount();
		for (int i = 0; i < cItems; ++i)
		{
			SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
		}
		break;
	}
	
	// Add your message handler code here and/or call default
	CListCtrl::OnChar(nChar, nRepCnt, nFlags);
}

void CCheckListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	OnLButtonDown(nFlags, point);

	// Add your message handler code here and/or call default	
	CListCtrl::OnLButtonDblClk(nFlags, point);
}

void CCheckListCtrl::SetEnabled(int nItem, BOOL bEnabled)
{
	ASSERT(::IsWindow(m_hWnd));

//	BOOL bChecked = GetItemState(nItem, LVIS_STATEIMAGEMASK) == INDEXTOSTATEIMAGEMASK(2);

	int prevState = GetItemState(nItem, LVIS_STATEIMAGEMASK);

	switch (prevState)
	{
	case INDEXTOSTATEIMAGEMASK(1):	// enabled unchecked
		bEnabled ? SetItemState(nItem, INDEXTOSTATEIMAGEMASK(1), LVIS_STATEIMAGEMASK) :
				   SetItemState(nItem, INDEXTOSTATEIMAGEMASK(3), LVIS_STATEIMAGEMASK);	

		break;
	case INDEXTOSTATEIMAGEMASK(2):	// enabled checked
		bEnabled ? SetItemState(nItem, INDEXTOSTATEIMAGEMASK(2), LVIS_STATEIMAGEMASK) :
				   SetItemState(nItem, INDEXTOSTATEIMAGEMASK(4), LVIS_STATEIMAGEMASK);	

		break;
	case INDEXTOSTATEIMAGEMASK(3):	// disabled unchecked
		bEnabled ? SetItemState(nItem, INDEXTOSTATEIMAGEMASK(1), LVIS_STATEIMAGEMASK) :
				   SetItemState(nItem, INDEXTOSTATEIMAGEMASK(3), LVIS_STATEIMAGEMASK);	

		break;
	case INDEXTOSTATEIMAGEMASK(4):	// disabled checked
		bEnabled ? SetItemState(nItem, INDEXTOSTATEIMAGEMASK(2), LVIS_STATEIMAGEMASK) :
				   SetItemState(nItem, INDEXTOSTATEIMAGEMASK(4), LVIS_STATEIMAGEMASK);	

		break;
	default:
		ASSERT(FALSE);
	}
}

int CCheckListCtrl::FindSubItem(int nSubItem, LPCTSTR psz) const
{
	int cItems = GetItemCount();
	CString strSubItem;

	for (int i = 0; i < cItems; ++i)
	{
		// TODO: may want to use a faster search method
		// if sorted
		strSubItem = GetItemText(i, nSubItem);
		if (strSubItem.Compare(psz) == 0)
			return i;
	}

	return -1;
}
