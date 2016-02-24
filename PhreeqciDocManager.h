#pragma once

// CPhreeqciDocManager command target

class CPhreeqciDocManager : public CDocManager
{
	DECLARE_DYNAMIC(CPhreeqciDocManager)
public:
	CPhreeqciDocManager();
	virtual ~CPhreeqciDocManager();

	// helper for standard commdlg dialogs
	virtual BOOL DoPromptFileName(CString& fileName, UINT nIDSTitle,
			DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* pTemplate);

};


