// phreeqci2.h : main header file for the PHREEQCI2 application
//
// $Id$
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PHREEQCI2_H__ED3B981C_5444_4F1C_82E4_79A6F6CCC3E8__INCLUDED_)
#define AFX_PHREEQCI2_H__ED3B981C_5444_4F1C_82E4_79A6F6CCC3E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "Database.h"	// Added by ClassView

class CRichDocIn;
class CRichDocDB;

/////////////////////////////////////////////////////////////////////////////
// CPhreeqciApp:
// See phreeqci2.cpp for the implementation of this class
//

class CPhreeqciApp : public CWinApp
{
public:
// inner class
	class CSettings
	{
	public:
		CSettings() : m_bCloseOnFinish(FALSE), m_bWarnOverwriteInput(TRUE),  m_bWarnOverwriteOutput(TRUE) {};
		CString m_strDefDBPathName;
		CString m_strExePath;
		CString m_strHelpDirectory;
		CString m_strInitialDir;
		BOOL m_bCloseOnFinish;
		BOOL m_bWarnOverwriteInput;
		BOOL m_bWarnOverwriteOutput;
		virtual ~CSettings(){};
	};
	CSettings m_settings;
public:
	CPhreeqciApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPhreeqciApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
	//}}AFX_VIRTUAL

	void SaveSettings();

// Implementation
protected:
	BOOL LoadMoreProfileSettings(UINT nMaxMRU);
	// void SaveSettings();

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void PreLoadDatabase(LPCTSTR lpszPathName);
	const CDatabase& GetMergeDatabase(LPCTSTR lpszPathName);
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName, const CString& rFileTypeId);
	void AddToRecentInFileList(LPCTSTR lpszPathName);
	void AddToRecentOutFileList(LPCTSTR lpszPathName);
	void AddToRecentDBFileList(LPCTSTR lpszPathName);
	//{{AFX_MSG(CPhreeqciApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	afx_msg void OnUpdateRecentInFileMenu(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRecentOutFileMenu(CCmdUI* pCmdUI);
	afx_msg void OnOptionsSetdefaultdatabase();
	//}}AFX_MSG
	// MRU
	afx_msg BOOL OnOpenRecentInFile(UINT nID);
	afx_msg BOOL OnOpenRecentOutFile(UINT nID);

	CMultiDocTemplate* GetDBDocTemplate(void) { return DBDocTemplate; };	
	CRichDocDB* OpenAssocDB(CRichDocIn *pDoc);
	void CloseAssocDB(CRichDocIn *pDoc);
	void RemoveDB(CRichDocDB* db);

	DECLARE_MESSAGE_MAP()
protected:
	const CDatabase& PreLoadDatabase_(LPCTSTR lpszPathName);
	std::map<CString, CDatabase> m_databaseMap;

	std::map<CString, CRichDocDB*> MapPathToDB;
	std::map<CString, std::set<CRichDocIn*> > MapPathToDocs;
	
	CRecentFileList* m_pRecentInFileList;
	CRecentFileList* m_pRecentOutFileList;

	CMultiDocTemplate *DBDocTemplate;

	friend class CRichDocInPage1;
	friend class CRunDlg;
	friend class CCustomizePage1;

	CRecentFileList* m_pRecentDBFileList;
	BOOL OpenNewDocument(const CString& strTarget);
	void UnRegisterShellNew(CMultiDocTemplate* pTemplate);
private:
	void ShowTipOfTheDay(void);
	void ShowTipOfTheDayFix(void);
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PHREEQCI2_H__ED3B981C_5444_4F1C_82E4_79A6F6CCC3E8__INCLUDED_)
