// GridListDoc.h: interface for the CGridListDoc class.
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "EditGrid.h"
#include "CheckListCtrl.h"

#if !defined(AFX_GRIDLISTDOC_H__1648D671_4F41_11D3_BB1A_02608C8F4400__INCLUDED_)
#define AFX_GRIDLISTDOC_H__1648D671_4F41_11D3_BB1A_02608C8F4400__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGridListItem
{
public:
	long m_nItem;
	long m_nRow;
	long m_nTab;
	CString m_strItem;

	CGridListItem();
	CGridListItem(long nItem,long nRow, CString strItem);
	CGridListItem(long nItem, long nRow, long nTab, CString strItem);

	// copy constructor
	CGridListItem(const CGridListItem& crGridListItem);
};


class CGridListDoc  
{
public:
	CGridListItem AddItem(LPCTSTR psz);
	CGridListItem AddSubItem(int nSubItem, LPCTSTR psz);
#ifdef SAVE
	long Push_Back(int nItem, CString strItem);
#endif
	void DeleteAllRows();
	unsigned int GetCount()const;
	//{{
	bool GetUseRowHeaders()const {return m_bUseRowHeaders;}
	void SetUseRowHeaders(bool bUseRowHeaders) {m_bUseRowHeaders = bUseRowHeaders;}
	//}}
	CGridListItem DeleteRow(long nRow);
	LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam, int nCheckListColumn = -1);
	void OnKeyDown(short FAR* KeyCode, short Shift);

	long GetNextRow()const;
	// LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	long OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	CGridListDoc();
	CGridListDoc(CEditGrid* pEditGrid, CCheckListCtrl* pCheckList, long nCol);
	CGridListDoc(CEditGrid* pEditGrid, CCheckListCtrl* pCheckList, CTabCtrl* pTabCtrl, long nCol);
	virtual ~CGridListDoc();

#ifdef _DEBUG
protected:
	virtual void AssertValid() const;
	// virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	BOOL m_bIgnoreCheckChange;
	CEditGrid* m_pEditGrid;
	CCheckListCtrl* m_pCheckList;
	CTabCtrl* m_pTabCtrl;
	long m_nCol;
	bool m_bUseRowHeaders;
private:
	std::list<CGridListItem> m_gridList;

};

#endif // !defined(AFX_GRIDLISTDOC_H__1648D671_4F41_11D3_BB1A_02608C8F4400__INCLUDED_)
