// AllVirtualPropertyPage.h : header file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALLVIRTUALPROPERTYPAGE_H__CF42B123_983B_407F_8A28_24423F6619B0__INCLUDED_)
#define AFX_ALLVIRTUALPROPERTYPAGE_H__CF42B123_983B_407F_8A28_24423F6619B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CAllVirtualPropertyPage dialog

class CAllVirtualPropertyPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CAllVirtualPropertyPage)

// Construction
public:
	CAllVirtualPropertyPage();
	~CAllVirtualPropertyPage();

// Dialog Data
	//{{AFX_DATA(CAllVirtualPropertyPage)
	enum { IDD = 0 };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CAllVirtualPropertyPage)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL DestroyWindow();
	virtual int DoModal();
	virtual CScrollBar* GetScrollBarCtrl(int nBar) const;
	virtual BOOL OnAmbientProperty(COleControlSite* pSite, DISPID dispid, VARIANT* pvar);
	virtual BOOL OnApply();
	virtual void OnCancel();
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual void OnFinalRelease();
	virtual BOOL OnKillActive();
	virtual void OnOK();
	virtual BOOL OnQueryCancel();
	virtual void OnReset();
	virtual BOOL OnSetActive();
	virtual void OnSetFont(CFont* pFont);
	virtual LRESULT OnWizardBack();
	virtual BOOL OnWizardFinish();
	virtual LRESULT OnWizardNext();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void Serialize(CArchive& ar);
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual void PostNcDestroy();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CAllVirtualPropertyPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALLVIRTUALPROPERTYPAGE_H__CF42B123_983B_407F_8A28_24423F6619B0__INCLUDED_)
