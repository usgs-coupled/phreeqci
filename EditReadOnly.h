#pragma once


// CEditReadOnly

class CEditReadOnly : public CEdit
{
	DECLARE_DYNAMIC(CEditReadOnly)

public:
	CEditReadOnly();
	virtual ~CEditReadOnly();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
};


