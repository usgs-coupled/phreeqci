#pragma once


// CListBoxDrop
#include "GridCtrl.h"

//////////////////////////////////////////////////
//LBDM_SETCAPTURE
//    wParam = 0;
//    lParam = 0;
//////////////////////////////////////////////////
#define LBDM_SETCAPTURE        0x01AE

//////////////////////////////////////////////////
//LBDM_ENDEDITLABELNOW
//    wParam = (WPARAM) (BOOL) fCancel;
//    lParam = 0;
//////////////////////////////////////////////////
#define LBDM_ENDEDITLABELNOW   TVM_ENDEDITLABELNOW

//////////////////////////////////////////////////
//LBDM_EDITLABEL
//    wParam = 0;
//    lParam = (LPARAM)(GV_DISPINFO*)pgvDispInfo;
//////////////////////////////////////////////////
#define LBDM_EDITLABEL         LVM_EDITLABEL


class CListBoxDrop : public CListBox
{
	DECLARE_DYNAMIC(CListBoxDrop)

public:
	CListBoxDrop();
	virtual ~CListBoxDrop();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnCancelMode();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnLbnKillfocus();
	afx_msg void OnLbnSetfocus();
	afx_msg LRESULT OnSetCapture(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndEditLabelNow(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEditLabel(WPARAM wParam, LPARAM lParam);

	void EndEdit(BOOL bCancel);
	void Display(const CRect& rc);

protected:
	int     m_nCol;
	int     m_nRow;
 	UINT    m_nLastChar; 
};


