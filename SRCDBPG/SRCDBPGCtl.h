#if !defined(AFX_SRCDBPGCTL_H__34623F2F_56FE_4C3B_B80F_74FB7C9BEA4F__INCLUDED_)
#define AFX_SRCDBPGCTL_H__34623F2F_56FE_4C3B_B80F_74FB7C9BEA4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// SRCDBPGCtl.h : Declaration of the CSRCDBPagerCtrl ActiveX Control class.
// $Id$
//////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CSRCDBPagerCtrl : See SRCDBPGCtl.cpp for implementation.

class CSRCDBPagerCtrl : public COleControl
{
	DECLARE_DYNCREATE(CSRCDBPagerCtrl)

// Constructor
public:
	CSRCDBPagerCtrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSRCDBPagerCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	virtual BOOL OnSetExtent(LPSIZEL lpSizeL);
	//}}AFX_VIRTUAL

// Implementation
protected:
	short m_nCaptionWidth;
	CRect m_rcFirst;
	CRect m_rcPrev;
	CRect m_rcNext;
	CRect m_rcLast;
	CRect m_rcNew;
	enum BUTTON_TYPE {
		NONE           = 0,
		GOTO_FIRST_BTN = 1,
		GOTO_PREV_BTN  = 2,
		GOTO_NEXT_BTN  = 3,
		GOTO_LAST_BTN  = 4,
		GOTO_NEW_BTN   = 5,
	} CAPTURED_BTN, DOWN_BTN;

	enum CSRCDBPagerCtrl::BUTTON_TYPE HitTest(const CPoint& point);

	void DrawButton(CDC* pDC, CPoint ptPos, BOOL bDown);
	void DrawEndMark(CDC *pDC, CPoint ptPos, CSize sizeOffset, BOOL bDown);
	void DrawLeftArrow(CDC *pDC, CPoint ptPos, CSize sizeOffset, BOOL bDown);
	void DrawRightArrow(CDC *pDC, CPoint ptPos, CSize sizeOffset, BOOL bDown);
	void DrawStar(CDC *pDC, CPoint ptPos, CSize sizeOffset, BOOL bDown);

	void DrawDisabledEndMark(CDC *pDC, CPoint ptPos, CSize sizeOffset, BOOL bDown);
	void DrawDisabledLeftArrow(CDC *pDC, CPoint ptPos, CSize sizeOffset, BOOL bDown);
	void DrawDisabledRightArrow(CDC *pDC, CPoint ptPos, CSize sizeOffset, BOOL bDown);
	void DrawDisabledStar(CDC *pDC, CPoint ptPos, CSize sizeOffset, BOOL bDown);


	~CSRCDBPagerCtrl();

	DECLARE_OLECREATE_EX(CSRCDBPagerCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CSRCDBPagerCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CSRCDBPagerCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CSRCDBPagerCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CSRCDBPagerCtrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnCancelMode();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CSRCDBPagerCtrl)
	BOOL m_bGotoFirstEnabled;
	afx_msg void OnGotoFirstEnabledChanged();
	BOOL m_bGotoLastEnabled;
	afx_msg void OnGotoLastEnabledChanged();
	BOOL m_bGotoPrevEnabled;
	afx_msg void OnGotoPrevEnabledChanged();
	BOOL m_bGotoNextEnabled;
	afx_msg void OnGotoNextEnabledChanged();
	BOOL m_bGotoNewEnabled;
	afx_msg void OnGotoNewEnabledChanged();
	afx_msg short GetCaptionWidth();
	afx_msg void SetCaptionWidth(short nNewValue);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	//{{AFX_EVENT(CSRCDBPagerCtrl)
	void FireGotoFirstClicked()
		{FireEvent(eventidGotoFirstClicked,EVENT_PARAM(VTS_NONE));}
	void FireGotoPrevClicked()
		{FireEvent(eventidGotoPrevClicked,EVENT_PARAM(VTS_NONE));}
	void FireGotoNextClicked()
		{FireEvent(eventidGotoNextClicked,EVENT_PARAM(VTS_NONE));}
	void FireGotoLastClicked()
		{FireEvent(eventidGotoLastClicked,EVENT_PARAM(VTS_NONE));}
	void FireGotoNewClicked()
		{FireEvent(eventidGotoNewClicked,EVENT_PARAM(VTS_NONE));}
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CSRCDBPagerCtrl)
	dispidGotoFirstEnabled = 1L,
	dispidGotoLastEnabled = 2L,
	dispidGotoPrevEnabled = 3L,
	dispidGotoNextEnabled = 4L,
	dispidGotoNewEnabled = 5L,
	dispidCaptionWidth = 6L,
	eventidGotoFirstClicked = 1L,
	eventidGotoPrevClicked = 2L,
	eventidGotoNextClicked = 3L,
	eventidGotoLastClicked = 4L,
	eventidGotoNewClicked = 5L,
	//}}AFX_DISP_ID
	};
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SRCDBPGCTL_H__34623F2F_56FE_4C3B_B80F_74FB7C9BEA4F__INCLUDED)
