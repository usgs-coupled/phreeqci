#pragma once

#include "KeywordPage.h"
#include "afxwin.h"

// CKPPitzerGeneral dialog

#ifndef baseCKPPitzerGeneral
#define baseCKPPitzerGeneral CKeywordPage
#endif

class CKPPitzerGeneral : public baseCKPPitzerGeneral
{
	DECLARE_DYNCREATE(CKPPitzerGeneral)

public:
	CKPPitzerGeneral(UINT nIDTemplate = CKPPitzerGeneral::IDD, UINT nIDCaption = 0);
	virtual ~CKPPitzerGeneral();

// Dialog Data
	enum { IDD = IDD_KEY_PITZER_GEN };

protected:

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CEdit   m_eInputDesc;

	virtual BOOL OnInitDialog();

	CButton m_btnMacInnes;
	CButton m_btnEtheta;
	CButton m_btnRedox;

	bool m_bMacInnes;
	bool m_bUseEtheta;
	bool m_bRedox;
	afx_msg void OnBnSetfocusCheckMacinnes();
	afx_msg void OnBnSetfocusCheckUseEtheta();
	afx_msg void OnBnSetfocusCheckUseRedox();
};
