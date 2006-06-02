#pragma once

// CDropDownButton

#include "ListBoxDrop.h"

class CDropDownButton : public CButton
{
	DECLARE_DYNAMIC(CDropDownButton)

public:
	CDropDownButton();
	virtual ~CDropDownButton();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
protected:
	 CListBoxDrop* m_pListBoxDropDown; // The List
	 CRect         m_rcListBox;        // Client Coords
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
protected:
	virtual void PreSubclassWindow();
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnEndInPlaceEdit(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};


