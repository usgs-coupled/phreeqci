// KPRatesPg1.h : header file
//
// $Id$
//////////////////////////////////////////////////////////////////////

//{{AFX_INCLUDES()
#include "mshflexgrid.h"
//}}AFX_INCLUDES
#include "EditGrid.h"
#include "ClipListBox.h"
#include "KeywordPageListItems.h"
#include "BasicDesc2.h"

#ifndef __KPRATESPG1_H__
#define __KPRATESPG1_H__

/////////////////////////////////////////////////////////////////////////////
// CKPRatesPg1 dialog

#include "KeywordPage.h"

#ifndef baseKPRatesPg1
#define baseKPRatesPg1 CKeywordPage
#endif

class CKPRatesPg1 : public baseKPRatesPg1
{
	DECLARE_DYNCREATE(CKPRatesPg1)

// inner class
protected:
	class XGridTabItem
	{
	public:
		CRate rate;
		long nRowIndex;
		int nTabIndex;

		XGridTabItem();
		virtual ~XGridTabItem(){}
	};

// Construction
public:
	CKPRatesPg1();
	~CKPRatesPg1();

	std::list<CRate> m_listRates;

// Dialog Data
	//{{AFX_DATA(CKPRatesPg1)
	enum { IDD = IDD_KEY_RATES_PG1 };
	CTabCtrl	m_tcNames;
	CEdit	m_eInputDesc;
	CEditGrid	m_egNames;
	CEditGrid	m_egBasic;
	//}}AFX_DATA

	CBasicDesc2 m_basicDesc;

protected:
	void DDX_RateList(CDataExchange *pDX);
	BOOL GetRate(long nRowIndex, CRate* pRate)const;
	CString GetRateName(long nRowIndex)const;
	BOOL DeleteRate(long nRowIndex);
	void InsertRate(long nRowIndex, CRate& rate);
	void DisplayTab(int nTabIndex);
	BOOL SetRateName(long nRowIndex, LPCTSTR lpszRateName);


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CKPRatesPg1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:


// Implementation
protected:
	std::map<CString, CString> m_mapFuncs;
	std::list<XGridTabItem> m_listItems;
	bool m_bRenumbering;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	// Generated message map functions
	//{{AFX_MSG(CKPRatesPg1)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeLbFuncs();
	afx_msg void OnSelchangeTcNames(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangingTcNames(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDownMshfgNames(short FAR* KeyCode, short Shift);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEnterCellMshfgNames();
	afx_msg void OnSetfocusLbFuncs();
	afx_msg void OnSetfocusEExplan();
	afx_msg void OnSetfocusLbArgs();
	afx_msg void OnEnterCellMshfgBasic();
	afx_msg void OnKeyDownMshfgBasic(short FAR* KeyCode, short Shift);
	afx_msg void OnBRenumber();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnSetfocusTreeArgs(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	// custom BN_SETFOCUS notifications
	afx_msg void OnSetfocusBRenumber();

	// custom grid handlers
	afx_msg LRESULT OnBeginCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndCellEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetfocusGrid(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

};



#endif // __KPRATESPG1_H__
