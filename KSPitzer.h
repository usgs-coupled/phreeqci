// KSKSPitzer.h : header file
//
// $Id$
//////////////////////////////////////////////////////////////////////
// This class defines custom modal property sheet 
// CKSPitzer.
 
#ifndef __KSPITZER_H__
#define __KSPITZER_H__

#include "KPPitzerGeneral.h"
#include "KPPitzerCA.h"
#include "KPPitzerXXY.h"
#include "KPPitzerXX.h"
#include "KPPitzerXN.h"
#include "KPPitzerCAN.h"
#include "KPPitzerNNV.h"
#include "KPPitzerNXX.h"
#include "KPPitzerAlphas.h"

/////////////////////////////////////////////////////////////////////////////
// CKSSolutionSpecies
#include "OleCtlKeywordSheet.h"

#ifndef baseCKSPitzer
#define baseCKSPitzer COleCtlKeywordSheet
#endif

class CKSPitzer : public baseCKSPitzer
{
	DECLARE_DYNAMIC(CKSPitzer)

// Construction
public:
	CKSPitzer(CWnd* pWndParent = NULL);

// Attributes
public:
	CKPPitzerGeneral m_PageGen;
	CKPPitzerCA      m_PageB0;
	CKPPitzerCA      m_PageB1;
	CKPPitzerCA      m_PageB2;
	CKPPitzerCA      m_PageC0;
	CKPPitzerXXY     m_PagePsi;
	CKPPitzerXX      m_PageTheta;      // IDD_KEY_PITZER_XX
	CKPPitzerXN      m_PageLamda;      // IDD_KEY_PITZER_XN
	CKPPitzerCAN     m_PageZeta;       // IDD_KEY_PITZER_CAN
	CKPPitzerNNV     m_PageMu;         // IDD_KEY_PITZER_NNV
	CKPPitzerNXX     m_PageEta;        // IDD_KEY_PITZER_NXX
	CKPPitzerAlphas  m_PageAlphas;     // IDD_KEY_PITZER_ALPHAS

// Operations
public:
	virtual CString GetString();
	virtual void Edit(CString& rStr);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKSSolutionSpecies)
	public:
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CKSPitzer();

// Generated message map functions
protected:
	//{{AFX_MSG(CKSPitzer)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __KSPITZER_H__
