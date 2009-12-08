// KPTransportPg6.h : header file
// KPSelectedOutputPg6.h : header file
//
// $Id$
//////////////////////////////////////////////////////////////////////

//{{AFX_INCLUDES()
#include "mshflexgrid.h"
//}}AFX_INCLUDES

#ifndef __KPTRANSPORTPG6_H__
#define __KPTRANSPORTPG6_H__

#include "KeywordPage.h"
#include "EditGrid.h"
#include "CheckListCtrl.h"
#include "DefinedRanges.h"
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CKPTransportPg6 dialog

#ifndef baseCKPTransportPg6
#define baseCKPTransportPg6 CKeywordPage
#endif

class CKPTransportPg6 : public baseCKPTransportPg6
{
	DECLARE_DYNCREATE(CKPTransportPg6)

// Construction
public:
	CKPTransportPg6();
	~CKPTransportPg6();

// Dialog Data
	enum { IDD = IDD_KEY_TRANSPORT_PG6 };
	CEdit	m_eInputDesc;
	CButton m_ctrlUseMCD;
	CButton m_ctrlUseID;


// Overrides
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnBnSetfocusCheckMultiD();
	afx_msg void OnBnClickedCheckMultiD();
	afx_msg void OnBnSetfocusCheckInterlayerD();
	afx_msg void OnBnClickedCheckInterlayerD();

	void UpdateMCD();
	void UpdateID();
	DECLARE_MESSAGE_MAP()

public:
	bool m_bUseMCD;
	bool m_bUseID;

	double m_default_Dw;
	double m_multi_Dpor;
	double m_multi_Dpor_lim;
	double m_multi_Dn;

	double m_interlayer_Dpor;
	double m_interlayer_Dpor_lim;
	double m_interlayer_tortf;
	afx_msg void OnEnSetfocusEditDiffCoef();
	afx_msg void OnEnSetfocusEditPorosity();
	afx_msg void OnEnSetfocusEditPorosityLimit();
	afx_msg void OnEnSetfocusEditPorosityExp();
	afx_msg void OnEnSetfocusEditPorosityId();
	afx_msg void OnEnSetfocusEditPorosityMin();
	afx_msg void OnEnSetfocusEditTortuosity();
};
#endif // __KPTRANSPORTPG6_H__
