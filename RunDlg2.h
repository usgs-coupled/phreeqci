#pragma once
#include "RunDlg.h"

class CRunDlg2 : public CRunDlg, public PHRQ_io 
{
public:
	CRunDlg2(void);
	~CRunDlg2(void);

public:
	virtual int getc(void);
	virtual void warning_msg(const char *err_str);
	virtual void error_msg(const char * str, bool stop=false);
	virtual void screen_msg(const char *err_str);

public:
	static UINT RunThreadProc(LPVOID pParam);

protected:
	// Generated message map functions
	//{{AFX_MSG(CRunDlg2)
	afx_msg void OnStart();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
