// RunDlg.h : header file
//
// $Date: 2004/11/03 06:56:12 $
// $Revision: 1.3 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNDLG_H__A66165D8_E0FB_4ECB_B39E_9AC93C708989__INCLUDED_)
#define AFX_RUNDLG_H__A66165D8_E0FB_4ECB_B39E_9AC93C708989__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RunDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRunDlg dialog
#include "Layout/ETSLayout.h"
#include "RichDocIn.h"

#define WM_THREADFINISHED               WM_USER + 5
#define WM_THREADCANCELED               WM_USER + 6
#define WM_THREADERRORS                 WM_USER + 7
#define WM_THREADHARDWARE_EXCEPTION     WM_USER + 8

#ifndef baseRunDlg
#define baseRunDlg ETSLayoutDialog
#endif

class CRunDlg : public baseRunDlg
{
// Construction
public:
	DECLARE_LAYOUT();
	CRichDocIn::XProperties m_props;
	CString m_strError;
	bool m_bErrors;
	CString m_strInput;
	static UINT RunThreadProc(LPVOID pParam);
	static int WriteCallBack(const int action, const int type, const char *err_str, const int stop, void *cookie, const char *format, va_list args);
	CRunDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRunDlg();	// standard destructor

// Dialog Data
	//{{AFX_DATA(CRunDlg)
	enum { IDD = IDD_RUN_DLG };
	CButton	m_btnSetAsDefault;
	CComboBox	m_cboDatabase;
	BOOL	m_bCloseOnFinish;
	BOOL	m_bSetAsDefault;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRunDlg)
	public:
	virtual int DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL m_bFirstSetActive;
	static CString GetErrorFile();
	void RemoveErrorFile();
	void ExpandDialog();
	bool SaveInitialSize(int IDD);
	CSize GetMinSize(int IDD);
	void CreateLayout();

	std::map<int, CSize> m_mapSizes;

	// Generated message map functions
	//{{AFX_MSG(CRunDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnStart();
	virtual void OnCancel();
	afx_msg void OnBrowseOutput();
	afx_msg void OnBrowseDatabase();
	afx_msg void OnBrowseInput();
	afx_msg void OnSelchangeCboDatabase();
	//}}AFX_MSG
	afx_msg LRESULT OnThreadFinished(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnThreadCanceled(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnThreadErrors(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnThreadHardware(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	void UpdateSetAsDefault();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RUNDLG_H__A66165D8_E0FB_4ECB_B39E_9AC93C708989__INCLUDED_)
