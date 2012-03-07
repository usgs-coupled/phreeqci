// GridListDoc.cpp: implementation of the CGridListDoc class.
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "GridListDoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGridListItem::CGridListItem ()
: m_nItem(-1), m_nRow(-1), m_nTab(-1), m_strItem(_T(""))
{

}

CGridListItem::CGridListItem(long nItem, long nRow, CString strItem)
: m_nItem(nItem), m_nRow(nRow), m_nTab(-1), m_strItem(strItem)
{
	//{{{3/8/2001 1:38:08 PM}
	ASSERT(!strItem.IsEmpty());
	//}}{3/8/2001 1:38:08 PM}
}

CGridListItem::CGridListItem(long nItem, long nRow, long nTab, CString strItem)
: m_nItem(nItem), m_nRow(nRow), m_nTab(nTab), m_strItem(strItem)
{
	//{{{3/8/2001 1:38:08 PM}
	ASSERT(!strItem.IsEmpty());
	//}}{3/8/2001 1:38:08 PM}
}

// copy constructor
CGridListItem::CGridListItem(const CGridListItem& crGridListItem)
{
	m_nItem   = crGridListItem.m_nItem;
	m_nRow    = crGridListItem.m_nRow;
	m_nTab    = crGridListItem.m_nTab;
	//{{{3/8/2001 1:38:08 PM}
	///ASSERT(!crGridListItem.m_strItem.IsEmpty());
	//}}{3/8/2001 1:38:08 PM}
	m_strItem = crGridListItem.m_strItem;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/**
  12/04/2000
  Added support for tab control
 */
CGridListDoc::CGridListDoc()
: m_pEditGrid(NULL), m_pCheckList(NULL), m_pTabCtrl(NULL), m_nCol(-1), m_bIgnoreCheckChange(FALSE), m_bUseRowHeaders(true)
{

}

/**
  12/04/2000
  Added support for tab control
 */
CGridListDoc::CGridListDoc(CEditGrid* pEditGrid, CCheckListCtrl* pCheckList, long nCol)
: m_pEditGrid(pEditGrid), m_pCheckList(pCheckList), m_pTabCtrl(NULL), m_nCol(nCol), m_bIgnoreCheckChange(FALSE), m_bUseRowHeaders(true)
{

}

/**
  12/04/2000
  Added support for tab control
 */
CGridListDoc::CGridListDoc(CEditGrid* pEditGrid, CCheckListCtrl* pCheckList, CTabCtrl* pTabCtrl, long nCol)
: m_pEditGrid(pEditGrid), m_pCheckList(pCheckList), m_pTabCtrl(pTabCtrl), m_nCol(nCol), m_bIgnoreCheckChange(FALSE), m_bUseRowHeaders(true)
{

}

CGridListDoc::~CGridListDoc()
{

}

/**
  12/06/2000
  Added support for tab control 
*/
long CGridListDoc::OnItemChanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLISTVIEW* pNMListView = (NMLISTVIEW*)pNMHDR;
	ASSERT(pNMListView->hdr.hwndFrom == m_pCheckList->m_hWnd);

	long row = -1; // signifies no change

	if ((pNMListView->uNewState ^ pNMListView->uOldState) & LVIS_STATEIMAGEMASK)
	{
		if (m_bIgnoreCheckChange)
			return row;

		CGridListItem glItem;

		int nItem = pNMListView->iItem;

		CString str = m_pCheckList->GetItemText(nItem, 0);

		//{{2/3/00 5:01:52 PM
		str.TrimLeft();
		str.TrimRight();
		//}}2/3/00 5:01:52 PM

		ASSERT( !str.IsEmpty() );

		std::list<CGridListItem>::iterator iter = m_gridList.begin();

		switch (m_pCheckList->GetCheckBox(nItem))
		{
		case BST_CHECKED :
			{
				ASSERT ( m_pCheckList->GetItemData(nItem) == 0 );
				m_pCheckList->SetItemData(nItem, (DWORD) m_pCheckList->GetItemData(nItem) + 1);
				glItem.m_nItem   = nItem;
				glItem.m_nRow    = GetNextRow();
				glItem.m_strItem = str;

				//{{COMMON
				// insert into list
				//
				// Find first item after new CGridListItem
				//
				// Note: m_gridList is kept sorted with respect to m_nRow and consequently
				//       to m_nTab
				//
				std::list<CGridListItem>::iterator itemIter       = m_gridList.begin();
				std::list<CGridListItem>::iterator firstItemAfter = m_gridList.end();
				for (; itemIter != firstItemAfter; ++itemIter)
				{
					if (itemIter->m_nRow > glItem.m_nRow)
					{
						firstItemAfter = itemIter;
						break;
					}
				}

				if (m_pTabCtrl)
				{
					if (firstItemAfter == m_gridList.end())
					{
						//
						// this will be the last CGridListItem
						//

						// add tab 
						ASSERT(glItem.m_nTab == -1);
						glItem.m_nTab = m_pTabCtrl->InsertItem(m_pTabCtrl->GetItemCount(), glItem.m_strItem);
						ASSERT(glItem.m_nTab != -1);

						// should be last tab
						ASSERT(glItem.m_nTab + 1 == m_pTabCtrl->GetItemCount());
					}
					else
					{
						//
						// this WILL NOT be the first CGridListItem in the list
						//

						// must be at least one tab
						ASSERT(m_pTabCtrl->GetItemCount() > 0);

						// insert tab 
						ASSERT(glItem.m_nTab == -1);
						ASSERT(firstItemAfter->m_nTab >= 0);
						glItem.m_nTab = m_pTabCtrl->InsertItem(firstItemAfter->m_nTab, glItem.m_strItem);
						ASSERT(glItem.m_nTab != -1);

						// shouldn't be first tab
						ASSERT(glItem.m_nTab > 0);
					}
				}

				m_pEditGrid->CMSHFlexGrid::SetTextMatrix(glItem.m_nRow, m_nCol, glItem.m_strItem);
				std::list<CGridListItem>::iterator theItem = m_gridList.insert(firstItemAfter, glItem);

				if (m_pTabCtrl)
				{
					// update tabs after this one (itemIter)
					itemIter = theItem;
					for (itemIter++; itemIter != m_gridList.end(); ++itemIter)
					{
						ASSERT(itemIter->m_nTab != -1);
						itemIter->m_nTab++;
					}
				}
				//}}COMMON

				//{{OLD
// COMMENT: {12/6/2000 5:14:21 PM}				m_gridList.push_back(glItem);			
// COMMENT: {12/6/2000 5:14:21 PM}				m_pEditGrid->CMSHFlexGrid::SetTextMatrix(glItem.m_nRow, m_nCol, str);
				//}}OLD

				// make sure item is visible;
				m_pEditGrid->SetRow(glItem.m_nRow);
				m_pEditGrid->GetCellTop();

				row = glItem.m_nRow;
			}

			break;

		case BST_UNCHECKED :

			do
			{
				BOOL bFound = FALSE;
				iter = m_gridList.begin();
				for (; iter != m_gridList.end(); ++iter)
				{
					if (iter->m_nItem == nItem)
					{
						bFound = TRUE;
						break;
					}
				}
				ASSERT(bFound || pNMListView->uOldState == INDEXTOSTATEIMAGEMASK(BST_INDETERMINATE + 1));
				
				// if not in list ignore
				if (!bFound) 
					break;

				DeleteRow(iter->m_nRow);
			} while (m_pCheckList->GetItemData(nItem) != 0);

			break;

		case BST_INDETERMINATE :

			break;
		}

	}
	*pResult = 0;	

	return row;

}

long CGridListDoc::GetNextRow()const
{
	long row;
	CString destination;
	
	// find first empty row
	for (row =  m_pEditGrid->GetFixedRows(); row < m_pEditGrid->GetRows(); ++row)
	{
		destination = m_pEditGrid->GetTextMatrix(row, m_nCol);
		if (destination.IsEmpty())
			break;
	}
	
	//{{ NEW 1/12/00 7:06:37 PM
	ASSERT( row != m_pEditGrid->GetRows() );

	// always keep one empty row
	if (m_gridList.size() == 
		(std::list<CGridListItem>::size_type) (m_pEditGrid->GetRows() - m_pEditGrid->GetFixedRows() - 1))
	{
		CString fmt;
		fmt.Format(_T("%d"), m_pEditGrid->GetRows() - m_pEditGrid->GetFixedRows() + 1);
		m_pEditGrid->SetRows(m_pEditGrid->GetRows() + 1);
		if (m_bUseRowHeaders)
		{
			m_pEditGrid->SetTextMatrix(m_pEditGrid->GetRows() - 1, 0, fmt);
		}
	}
	//{{ NEW 1/12/00 7:06:37 PM

	return row;
}

void CGridListDoc::OnKeyDown(short FAR* KeyCode, short Shift)
{
	UNUSED(Shift);

	switch (*KeyCode)
	{
	case VK_DELETE :
		CWaitCursor wait;	// display hourglass cursor
		m_pEditGrid->SetRedraw(FALSE);
		long nCol = m_pEditGrid->GetCol();
		long nColSel = m_pEditGrid->GetColSel();

		// check if associated col is included (m_nCol)
		if ((min(nCol, nColSel) <= m_nCol) && (m_nCol <= max(nCol, nColSel)))
		{
			long nRow = m_pEditGrid->GetRow();
			long nRowSel = m_pEditGrid->GetRowSel();

			// determine how many rows to delete
			long cSelectedRows = abs(nRow - nRowSel) + 1;

			// determine first row to delete
			long iStartRow = min(nRow, nRowSel);

			// delete each row
			for (long i = 0; i < cSelectedRows; ++i)
			{
				DeleteRow(iStartRow);
			}
		}
		else 
		{
#ifdef _DEBUG
			// check for invalid grid cells
			long nRow = m_pEditGrid->GetRow();
			long nRowSel = m_pEditGrid->GetRowSel();
			for (long i = min(nRow, nRowSel); i <= max(nRow, nRowSel); ++i)
			{
				for (long j = min(nCol, nColSel); j <= max(nCol, nColSel); ++j)
				{
					ASSERT(m_pEditGrid->GetCellEnabled(i, j)); // attempt to erase a disabled cell
				}
			}
			// reset selection
			m_pEditGrid->SetRow(nRow);
			m_pEditGrid->SetCol(nCol);
			m_pEditGrid->SetRowSel(nRowSel);
			m_pEditGrid->SetColSel(nColSel);
#endif
			// just fill with empty strings
			m_pEditGrid->SetFillStyle(flexFillRepeat);
			m_pEditGrid->SetText(_T(""));
			m_pEditGrid->SetFillStyle(flexFillSingle);
		}
		m_pEditGrid->SetRedraw(TRUE);
		break;
	}
}

/**
  12/05/2000
  Added support for tab control 
*/
LRESULT CGridListDoc::OnEndCellEdit(WPARAM wParam, LPARAM lParam, int nCheckListColumn /* = -1 */)
{
#ifdef _DEBUG
	AssertValid();
#endif

	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	int nID = wParam;
	ASSERT( nID == m_pEditGrid->GetDlgCtrlID() );

	BOOL bMakeChange = TRUE;

	// ignore if edit is cancelled
	if (pInfo->item.pszText == NULL)
		return bMakeChange;


	LVFINDINFO info;

	if (pInfo->item.iCol == m_nCol)
	{
		CString strOld = m_pEditGrid->GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol);
		CString strNew = pInfo->item.pszText;

		if (strOld.Compare(_T("")) != 0)
		{
			//
			// Grid cell contains text
			//
			BOOL bFound = FALSE;
			std::list<CGridListItem>::iterator iter = m_gridList.begin();
			for (; iter != m_gridList.end(); ++iter)
			{
				CString temp = iter->m_strItem;
				// remove old check mark
				if (iter->m_nRow == pInfo->item.iRow)
				{
					CString temp = iter->m_strItem;
					bFound = TRUE;
					if (iter->m_nItem != -1)
					{
						m_pCheckList->SetItemData(iter->m_nItem, (DWORD) m_pCheckList->GetItemData(iter->m_nItem) - 1);
						if (m_pCheckList->GetItemData(iter->m_nItem ) == 0)
						{
							m_bIgnoreCheckChange = TRUE;
							m_pCheckList->SetCheckBox(iter->m_nItem, BST_UNCHECKED);
							m_bIgnoreCheckChange = FALSE;
						}
					}
					break;
				}
			}
			ASSERT( bFound == TRUE );
			if (strNew.Compare(_T("")) == 0)
			{
				// if empty string treat as a deletion
				if (m_pTabCtrl)
				{
					ASSERT(iter->m_nTab != -1);

					TRACE("Deleting TabIndex = %d\n", iter->m_nTab);
					VERIFY(m_pTabCtrl->DeleteItem(iter->m_nTab));
					std::list<CGridListItem>::iterator itemIter = m_gridList.begin();
					for (; itemIter != m_gridList.end(); ++itemIter)
					{
						if (itemIter->m_nTab > iter->m_nTab)
						{
							itemIter->m_nTab--;
						}
					}
				}
				// remove from list
				m_gridList.erase(iter);
			}
			else
			{
				// change text of existing tab
				if (m_pTabCtrl)
				{
					ASSERT(iter->m_nTab != -1);

					TCITEM item;
					item.mask = TCIF_TEXT;
					item.pszText = (LPTSTR)(LPCTSTR)strNew;
					VERIFY(m_pTabCtrl->SetItem(iter->m_nTab, &item));
				}


				iter->m_strItem = strNew;
				m_pEditGrid->CMSHFlexGrid::SetTextMatrix(pInfo->item.iRow, pInfo->item.iCol, iter->m_strItem);

				info.flags = LVFI_STRING;
				info.psz = strNew.GetBuffer(strNew.GetLength());
				iter->m_nItem = m_pCheckList->FindItem(&info, -1);

				if (iter->m_nItem != -1)
				{
					// make case sensitive
					CString strCase = m_pCheckList->GetItemText(iter->m_nItem, 0);
					if (strCase.Compare(info.psz) != 0)
					{
						iter->m_nItem = -1;
					}
					else
					{
						m_pCheckList->SetItemData(iter->m_nItem, (DWORD) m_pCheckList->GetItemData(iter->m_nItem) + 1);
						m_bIgnoreCheckChange = TRUE;
						m_pCheckList->SetCheckBox(iter->m_nItem, BST_CHECKED);
						m_bIgnoreCheckChange = FALSE;
					}
				}
				else if (nCheckListColumn != -1)
				{
					ASSERT( 0 <= nCheckListColumn );
					ASSERT( ::IsWindow(m_pCheckList->m_hWnd) );
					ASSERT( nCheckListColumn < m_pCheckList->GetHeaderCtrl()->GetItemCount() );
					iter->m_nItem = m_pCheckList->FindSubItem(nCheckListColumn, strNew);
					if (iter->m_nItem != -1)
					{
						m_pCheckList->SetItemData(iter->m_nItem, (DWORD) m_pCheckList->GetItemData(iter->m_nItem) + 1);
						m_bIgnoreCheckChange = TRUE;
						m_pCheckList->SetCheckBox(iter->m_nItem, BST_CHECKED);
						m_bIgnoreCheckChange = FALSE;
					}
				}
			}

		}
		else
		{
			//
			// Grid cell contains no text
			//
			//{{ May 17, 2001
			if (strNew.IsEmpty())
			{
				return bMakeChange;
			}
			//}} May 17, 2001

			info.flags = LVFI_STRING;
			info.psz   = pInfo->item.pszText;
			
			CGridListItem glItem;
			glItem.m_nItem   = m_pCheckList->FindItem(&info, -1);
			glItem.m_strItem = pInfo->item.pszText;
			glItem.m_nRow    = pInfo->item.iRow;

			//
			// Find first item after new CGridListItem
			// See AddItem
			//
			ASSERT(glItem.m_nRow >= 0);
			std::list<CGridListItem>::iterator itemIter       = m_gridList.begin();
			std::list<CGridListItem>::iterator firstItemAfter = m_gridList.end();
			for (; itemIter != firstItemAfter; ++itemIter)
			{
				if (itemIter->m_nRow > glItem.m_nRow)
				{
					firstItemAfter = itemIter;
					break;
				}
			}
			if (m_pTabCtrl)
			{
				if (firstItemAfter == m_gridList.end())
				{
					//
					// this will be the last CGridListItem
					//

					// add tab 
					ASSERT(glItem.m_nTab == -1);
					glItem.m_nTab = m_pTabCtrl->InsertItem(m_pTabCtrl->GetItemCount(), glItem.m_strItem);
					ASSERT(glItem.m_nTab != -1);

					// should be last tab
					ASSERT(glItem.m_nTab + 1 == m_pTabCtrl->GetItemCount());
				}
				else
				{
					//
					// this WILL NOT be the first CGridListItem in the list
					//

					// must be at least one tab
					ASSERT(m_pTabCtrl->GetItemCount() > 0);

					// insert tab 
					ASSERT(glItem.m_nTab == -1);
					ASSERT(firstItemAfter->m_nTab >= 0);
					glItem.m_nTab = m_pTabCtrl->InsertItem(firstItemAfter->m_nTab, glItem.m_strItem);
					ASSERT(glItem.m_nTab != -1);

// COMMENT: {5/17/2001 2:16:03 PM}					// shouldn't be first tab
// COMMENT: {5/17/2001 2:16:03 PM}					ASSERT(glItem.m_nTab > 0);
				}

				// glItem.m_nTab = m_pTabCtrl->InsertItem(glItem.m_nRow, info.psz);
				ASSERT(glItem.m_nTab != -1);
			}

			if (glItem.m_nItem != -1)
			{
				// make case sensitive
				CString strCase = m_pCheckList->GetItemText(glItem.m_nItem, 0);
				if (strCase.Compare(info.psz) != 0)
				{
					glItem.m_nItem = -1;
				}
				else
				{
					m_bIgnoreCheckChange = TRUE;
					m_pCheckList->SetCheckBox(glItem.m_nItem, BST_CHECKED);
					m_pCheckList->SetItemData(glItem.m_nItem, (DWORD) m_pCheckList->GetItemData(glItem.m_nItem) + 1);
					m_bIgnoreCheckChange = FALSE;
				}
			}
			else if (nCheckListColumn != -1)
			{
				ASSERT( 0 <= nCheckListColumn );
				ASSERT( nCheckListColumn < m_pCheckList->GetHeaderCtrl()->GetItemCount() );
				// Note: FindSubItem is case sensitive
				glItem.m_nItem = m_pCheckList->FindSubItem(nCheckListColumn, strNew);
				if (glItem.m_nItem != -1)
				{
					m_pCheckList->SetItemData(glItem.m_nItem, (DWORD) m_pCheckList->GetItemData(glItem.m_nItem) + 1);
					m_bIgnoreCheckChange = TRUE;
					m_pCheckList->SetCheckBox(glItem.m_nItem, BST_CHECKED);
					m_bIgnoreCheckChange = FALSE;
				}
			}

			// always keep one empty row
			if (m_gridList.size() == 
				(std::list<CGridListItem>::size_type) (m_pEditGrid->GetRows() - m_pEditGrid->GetFixedRows() - 1))
			{
				CString fmt;
				fmt.Format(_T("%d"), m_pEditGrid->GetRows() - m_pEditGrid->GetFixedRows() + 1);
				m_pEditGrid->SetRows(m_pEditGrid->GetRows() + 1);
				m_pEditGrid->SetTextMatrix(m_pEditGrid->GetRows() - 1, 0, fmt);
			}

			//{{{3/8/2001 1:38:08 PM}
			if (glItem.m_strItem.IsEmpty())
			{
				return bMakeChange;
			}
			//}}{3/8/2001 1:38:08 PM}
			// insert into list
			std::list<CGridListItem>::iterator theItem = m_gridList.insert(firstItemAfter, glItem);

			if (m_pTabCtrl)
			{
				// update tabs after this one (itemIter)
				itemIter = theItem;
				for (itemIter++; itemIter != m_gridList.end(); ++itemIter)
				{
					ASSERT(itemIter->m_nTab != -1);
					itemIter->m_nTab++;
				}
			}
		}
	}

#ifdef _DEBUG
	AssertValid();
#endif
	return bMakeChange;
}

/**
  12/06/2000
  Added support for tab control 
*/
/*void*/
CGridListItem CGridListDoc::DeleteRow(long nRow)
{
#ifdef _DEBUG
	AssertValid();
#endif

	CGridListItem theItem;
	BOOL bFound = FALSE;

	std::list<CGridListItem>::iterator iter = m_gridList.begin();
	for (; iter != m_gridList.end(); ++iter)
	{
#ifdef _DEBUG
		CGridListItem glItem(*iter);
#endif
		if (iter->m_nRow == nRow)
		{
			bFound = TRUE;
			if (iter->m_nItem != -1)
			{
				// decrement checkmark item data (number of rows associatied with a checkmark)
				ASSERT(m_pCheckList->GetItemData(iter->m_nItem) > 0);
				m_pCheckList->SetItemData(iter->m_nItem, (DWORD) m_pCheckList->GetItemData(iter->m_nItem) - 1);
				if (m_pCheckList->GetItemData(iter->m_nItem ) == 0)
				{
					// remove check mark
					m_bIgnoreCheckChange = TRUE;
					m_pCheckList->SetCheckBox(iter->m_nItem, BST_UNCHECKED);
					m_bIgnoreCheckChange = FALSE;
				}
			}
			break;
		}
	}

	m_pEditGrid->DeleteRow(nRow);

	if (bFound)
	{
		// remove tab
		if (m_pTabCtrl)
		{
			m_pTabCtrl->DeleteItem(iter->m_nTab);
		}

		// copy the deleted CGridListItem
		theItem = *iter;

		// remove CGridListItem from list
		iter = m_gridList.erase(iter);

		// update remaining CGridListItems
		for (; iter != m_gridList.end(); ++iter)
		{
			ASSERT(iter->m_nRow > nRow);
			iter->m_nRow--;
			iter->m_nTab--;
		}
	}
	else
	{
		//{{{3/12/2001 4:54:12 PM}
		iter = m_gridList.begin();
		for (; iter != m_gridList.end(); ++iter)
		{
			if (iter->m_nRow > nRow)
			{
				iter->m_nRow--;
				iter->m_nTab--;
			}
		}
		//}}{3/12/2001 4:54:12 PM}
	}
#ifdef _DEBUG
	AssertValid();
#endif
	return theItem;
}

unsigned int CGridListDoc::GetCount()const
{
	if (m_pTabCtrl)
	{
		ASSERT((UINT)m_pTabCtrl->GetItemCount() == m_gridList.size());
	}
	return m_gridList.size();
}

/**
  12/06/2000
  Added support for tab control 
*/
void CGridListDoc::DeleteAllRows()
{
	std::list<CGridListItem>::iterator iter = m_gridList.begin();
	for (; iter != m_gridList.end(); iter = m_gridList.begin())
	{
		DeleteRow((*iter).m_nRow);
	}
	ASSERT(m_gridList.empty());
	m_gridList.clear();

#ifdef _DEBUG
	// verify state of m_pCheckList items
	int nCount = m_pCheckList->GetItemCount();
	for (int nItem = 0; nItem < nCount; ++nItem)
	{
		DWORD dw = m_pCheckList->GetItemData(nItem);
		ASSERT(dw == 0);
	}

	// verify state of m_pEditGrid cells
	long cRows = m_pEditGrid->GetRows();
	CString strCell;
	for (long nRow = m_pEditGrid->GetFixedRows(); nRow < cRows; ++nRow)
	{
		strCell = m_pEditGrid->GetTextMatrix(nRow, m_nCol);
		ASSERT(strCell.IsEmpty());
	}
	// verify state of m_pTabCtrl
	if (m_pTabCtrl)
	{
		ASSERT(m_pTabCtrl->GetItemCount() == 0);
	}
#endif
}

#ifdef SAVE
long CGridListDoc::Push_Back(int nItem, CString strItem)
{
	ASSERT( nItem != -1 );

	CGridListItem glItem;
	
	glItem.m_nItem   = nItem;
	glItem.m_nRow    = GetNextRow();
	glItem.m_strItem = strItem;
	m_gridList.push_back(glItem);

	ASSERT( m_nCol >= m_pEditGrid->GetFixedCols() );
	ASSERT( !strItem.IsEmpty() );
	m_pEditGrid->CMSHFlexGrid::SetTextMatrix(glItem.m_nRow, m_nCol, strItem);

	m_pCheckList->SetItemData(nItem, (DWORD) m_pCheckList->GetItemData(nItem) + 1);

	return glItem.m_nRow;
}
#endif

/**
Sets checkmark in checklist control if it exists; Sets first empty
grid cell to psz;  If m_pTabCtrl is non-null adds tab

  This function may be called from either OnInitDialog or DoDataExchange

  Note: This function won't add the specified item if the checkbox is
  BST_INDETERMINATE

  12/04/2000
  Added support for tab control
  
  04/14/2009
  Fixed bug that occurred when Co (Cobalt) was entered and CO (Carbon Monoxide)
  was selected -- Made FindItem() case-dependent

*/
CGridListItem CGridListDoc::AddItem(LPCTSTR psz)
{
#ifdef _DEBUG
	AssertValid();
#endif
	ASSERT( m_pCheckList->GetSafeHwnd() != NULL );
	ASSERT( m_pEditGrid->GetSafeHwnd() != NULL );
	ASSERT( m_pTabCtrl == NULL || m_pTabCtrl->GetSafeHwnd() != NULL );

	//
	// If psz is in CCheckListCtrl set check mark
	//
	LVFINDINFO info;
	info.flags = LVFI_STRING;
	info.psz = psz;

	int nStart = -1;
	int nItem = -1;
	while (true)
	{
		nItem = m_pCheckList->FindItem(&info, nStart);
		if (nItem == -1) break;
		
		// verify match -- must match case (ie CO vs Co)
		CString s = m_pCheckList->GetItemText(nItem, 0);
		if (s.Compare(info.psz) == 0)
		{
			if (m_pCheckList->GetCheckBox(nItem) != BST_INDETERMINATE)
			{
				m_bIgnoreCheckChange = TRUE;
				m_pCheckList->SetCheckBox(nItem, BST_CHECKED);
				m_bIgnoreCheckChange = FALSE;
				m_pCheckList->SetItemData(nItem, (DWORD) m_pCheckList->GetItemData(nItem) + 1);
				break;
			}
			else
			{
				// invalid won't be added to list, grid or tab
				ASSERT(FALSE);
				return CGridListItem();
			}
		}
		else
		{
			ASSERT(s.CompareNoCase(info.psz) == 0);
			nStart = nItem + 1;
		}
	}

// COMMENT: {4/14/2009 2:09:01 PM}	int nItem = m_pCheckList->FindItem(&info, -1);
// COMMENT: {4/14/2009 2:09:01 PM}
// COMMENT: {4/14/2009 2:09:01 PM}	if ( nItem != -1 )
// COMMENT: {4/14/2009 2:09:01 PM}	{
// COMMENT: {4/14/2009 2:09:01 PM}		if (m_pCheckList->GetCheckBox(nItem) != BST_INDETERMINATE)
// COMMENT: {4/14/2009 2:09:01 PM}		{
// COMMENT: {4/14/2009 2:09:01 PM}			m_bIgnoreCheckChange = TRUE;
// COMMENT: {4/14/2009 2:09:01 PM}			m_pCheckList->SetCheckBox(nItem, BST_CHECKED);
// COMMENT: {4/14/2009 2:09:01 PM}			m_bIgnoreCheckChange = FALSE;
// COMMENT: {4/14/2009 2:09:01 PM}			m_pCheckList->SetItemData(nItem, (DWORD) m_pCheckList->GetItemData(nItem) + 1);
// COMMENT: {4/14/2009 2:09:01 PM}		}
// COMMENT: {4/14/2009 2:09:01 PM}		else
// COMMENT: {4/14/2009 2:09:01 PM}		{
// COMMENT: {4/14/2009 2:09:01 PM}			// invalid won't be added to list, grid or tab
// COMMENT: {4/14/2009 2:09:01 PM}			ASSERT(FALSE);
// COMMENT: {4/14/2009 2:09:01 PM}			return CGridListItem();
// COMMENT: {4/14/2009 2:09:01 PM}		}
// COMMENT: {4/14/2009 2:09:01 PM}	}

	//{{{3/8/2001 1:38:08 PM}
	ASSERT(psz && strlen(psz));
	//}}{3/8/2001 1:38:08 PM}

	CGridListItem glItem(nItem, GetNextRow(), psz);

	//
	// Set grid cell text
	//
#ifdef _DEBUG
	{
		CString strPrev = m_pEditGrid->CMSHFlexGrid::GetTextMatrix(glItem.m_nRow, m_nCol);
		ASSERT(strPrev.IsEmpty());
	}
#endif
	m_pEditGrid->CMSHFlexGrid::SetTextMatrix(glItem.m_nRow, m_nCol, glItem.m_strItem);

	//
	// Find first item after new CGridListItem
	//
	// Note: m_gridList is kept sorted with respect to m_nRow and consequently
	//       to m_nTab
	//
	std::list<CGridListItem>::iterator itemIter       = m_gridList.begin();
	std::list<CGridListItem>::iterator firstItemAfter = m_gridList.end();
	for (; itemIter != firstItemAfter; ++itemIter)
	{
		if (itemIter->m_nRow > glItem.m_nRow)
		{
			firstItemAfter = itemIter;
			break;
		}
	}

	if (m_pTabCtrl)
	{
		if (firstItemAfter == m_gridList.end())
		{
			//
			// this will be the last CGridListItem
			//

			// add tab 
			ASSERT(glItem.m_nTab == -1);
			glItem.m_nTab = m_pTabCtrl->InsertItem(m_pTabCtrl->GetItemCount(), glItem.m_strItem);
			ASSERT(glItem.m_nTab != -1);

			// should be last tab
			ASSERT(glItem.m_nTab + 1 == m_pTabCtrl->GetItemCount());
		}
		else
		{
			//
			// this WILL NOT be the first CGridListItem in the list
			//

			// must be at least one tab
			ASSERT(m_pTabCtrl->GetItemCount() > 0);

			// insert tab 
			ASSERT(glItem.m_nTab == -1);
			ASSERT(firstItemAfter->m_nTab >= 0);
			glItem.m_nTab = m_pTabCtrl->InsertItem(firstItemAfter->m_nTab, glItem.m_strItem);
			ASSERT(glItem.m_nTab != -1);

			// shouldn't be first tab
			ASSERT(glItem.m_nTab > 0);
		}
	}

	// insert into list
	std::list<CGridListItem>::iterator theItem = m_gridList.insert(firstItemAfter, glItem);

	if (m_pTabCtrl)
	{
		// update tabs after this one (itemIter)
		itemIter = theItem;
		for (itemIter++; itemIter != m_gridList.end(); ++itemIter)
		{
			ASSERT(itemIter->m_nTab != -1);
			itemIter->m_nTab++;
		}
	}

#ifdef _DEBUG
	AssertValid();
	ASSERT(theItem != m_gridList.end());
#endif
	return *theItem;
}

/**
This function may be called from either OnInitDialog or DoDataExchange

  Note: This function won't add the specified item if the checkbox is
  BST_INDETERMINATE

  12/06/2000
  Added support for tab control 
*/
CGridListItem CGridListDoc::AddSubItem(int nSubItem, LPCTSTR psz)
{
	ASSERT( m_pCheckList->GetSafeHwnd() != NULL );
	ASSERT( m_pEditGrid->GetSafeHwnd() != NULL );

	// If psz is in list set check mark
	int nItem = m_pCheckList->FindSubItem(nSubItem, psz);

	if ( nItem != -1 )
	{
		if (m_pCheckList->GetCheckBox(nItem) != BST_INDETERMINATE)
		{
			m_bIgnoreCheckChange = TRUE;
			m_pCheckList->SetCheckBox(nItem, BST_CHECKED);
			m_bIgnoreCheckChange = FALSE;
			m_pCheckList->SetItemData(nItem, (DWORD) m_pCheckList->GetItemData(nItem) + 1);
		}
		else
		{
			// invalid won't be added to list, grid or tab
			return CGridListItem();
		}
	}

	//{{{3/8/2001 1:38:08 PM}
	ASSERT(psz && strlen(psz));
	//}}{3/8/2001 1:38:08 PM}

	CGridListItem glItem(nItem, GetNextRow(), psz);

	//{{COMMON
	// insert into list
	//
	// Find first item after new CGridListItem
	//
	// Note: m_gridList is kept sorted with respect to m_nRow and consequently
	//       to m_nTab
	//
	std::list<CGridListItem>::iterator itemIter       = m_gridList.begin();
	std::list<CGridListItem>::iterator firstItemAfter = m_gridList.end();
	for (; itemIter != firstItemAfter; ++itemIter)
	{
		if (itemIter->m_nRow > glItem.m_nRow)
		{
			firstItemAfter = itemIter;
			break;
		}
	}

	if (m_pTabCtrl)
	{
		if (firstItemAfter == m_gridList.end())
		{
			//
			// this will be the last CGridListItem
			//

			// add tab 
			ASSERT(glItem.m_nTab == -1);
			glItem.m_nTab = m_pTabCtrl->InsertItem(m_pTabCtrl->GetItemCount(), glItem.m_strItem);
			ASSERT(glItem.m_nTab != -1);

			// should be last tab
			ASSERT(glItem.m_nTab + 1 == m_pTabCtrl->GetItemCount());
		}
		else
		{
			//
			// this WILL NOT be the first CGridListItem in the list
			//

			// must be at least one tab
			ASSERT(m_pTabCtrl->GetItemCount() > 0);

			// insert tab 
			ASSERT(glItem.m_nTab == -1);
			ASSERT(firstItemAfter->m_nTab >= 0);
			glItem.m_nTab = m_pTabCtrl->InsertItem(firstItemAfter->m_nTab, glItem.m_strItem);
			ASSERT(glItem.m_nTab != -1);

			// shouldn't be first tab
			ASSERT(glItem.m_nTab > 0);
		}
	}

	m_pEditGrid->CMSHFlexGrid::SetTextMatrix(glItem.m_nRow, m_nCol, glItem.m_strItem);
	std::list<CGridListItem>::iterator theItem = m_gridList.insert(firstItemAfter, glItem);

	if (m_pTabCtrl)
	{
		// update tabs after this one (itemIter)
		itemIter = theItem;
		for (itemIter++; itemIter != m_gridList.end(); ++itemIter)
		{
			ASSERT(itemIter->m_nTab != -1);
			itemIter->m_nTab++;
		}
	}
	//}}COMMON


	//{{OLD
	// add to grid
// COMMENT: {12/6/2000 5:00:36 PM}	m_pEditGrid->CMSHFlexGrid::SetTextMatrix(glItem.m_nRow, m_nCol, glItem.m_strItem);
// COMMENT: {12/6/2000 5:00:36 PM}
// COMMENT: {12/6/2000 5:00:36 PM}	m_gridList.push_back(glItem);
	//}}OLD

	return glItem;
}

#ifdef _DEBUG
void CGridListDoc::AssertValid() const
{
	if (m_pTabCtrl)
	{
		ASSERT((UINT)m_pTabCtrl->GetItemCount() == GetCount());
	}

	// verify state of list
	long nRowPrev = -1;
	long nTabPrev = -1;
	std::list<CGridListItem>::const_iterator itemIter = m_gridList.begin();
	TRACE("AssertValid Begin\n");
	TRACE("m_gridList contains %d items\n", m_gridList.size());
	for (; itemIter != m_gridList.end(); ++itemIter)
	{
		//{{ {3/8/2001 1:32:48 PM}
		ASSERT(!itemIter->m_strItem.IsEmpty());
		//}} {3/8/2001 1:32:48 PM}
		// Note: the list should be in the same order as tabs and the grid
		ASSERT(itemIter->m_nRow > nRowPrev);

		if (m_pTabCtrl)
		{
			ASSERT(itemIter->m_nTab != -1);
			ASSERT(itemIter->m_nTab > nTabPrev);

			// check tab
			TCITEM item;
			TCHAR buffer[80];
			item.mask = TCIF_TEXT;
			item.cchTextMax = 80;
			item.pszText = buffer;
			ASSERT(m_pTabCtrl->GetItem(itemIter->m_nTab, &item));
			ASSERT(item.pszText);
		}

		TRACE("%s nTab = %d, nRow = %d\n", itemIter->m_strItem, itemIter->m_nTab, itemIter->m_nRow);

		// can't check grid since GetTextMatrix is not const
		// ASSERT(itemIter->rate.m_strName == m_egNames.GetTextMatrix(itemIter->nRowIndex, 0));

		nRowPrev = itemIter->m_nRow;
		nTabPrev = itemIter->m_nTab;
	}
	TRACE("AssertValid End\n");
}
#endif