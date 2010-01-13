// KPPitzerB0.cpp : implementation file
//

#include "stdafx.h"
#include "phreeqci2.h"
#include "KPPitzerGeneral.h"
#include "Util.h"

// CKPPitzerGeneral dialog

// IMPLEMENT_DYNAMIC(CKPPitzerGeneral, CPropertyPage)
IMPLEMENT_DYNCREATE(CKPPitzerGeneral, baseCKPPitzerGeneral)

CKPPitzerGeneral::CKPPitzerGeneral(UINT nIDTemplate, UINT nIDCaption) : baseCKPPitzerGeneral(nIDTemplate, nIDCaption)
, m_bMacInnes(true)
, m_bUseEtheta(true)
, m_bRedox(false)
{
}

CKPPitzerGeneral::~CKPPitzerGeneral()
{
}

void CKPPitzerGeneral::DoDataExchange(CDataExchange* pDX)
{
	baseCKPPitzerGeneral::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_DESCRIPTION, m_eInputDesc);
	DDX_Control(pDX, IDC_CHECK_MACINNES, m_btnMacInnes);
	DDX_Control(pDX, IDC_CHECK_USE_ETHETA, m_btnEtheta);
	DDX_Control(pDX, IDC_CHECK_USE_REDOX, m_btnRedox);

	if (pDX->m_bSaveAndValidate)
	{
		if (this->m_btnMacInnes.GetCheck() == BST_CHECKED)
		{
			this->m_bMacInnes = true;
		}
		else
		{
			this->m_bMacInnes = false;
		}

		if (this->m_btnEtheta.GetCheck() == BST_CHECKED)
		{
			this->m_bUseEtheta = true;
		}
		else
		{
			this->m_bUseEtheta = false;
		}

		if (this->m_btnRedox.GetCheck() == BST_CHECKED)
		{
			this->m_bRedox = true;
		}
		else
		{
			this->m_bRedox = false;
		}
	}
	else
	{
		if (this->m_bMacInnes)
		{
			this->m_btnMacInnes.SetCheck(BST_CHECKED);
		}
		else
		{
			this->m_btnMacInnes.SetCheck(BST_UNCHECKED);
		}

		if (this->m_bUseEtheta)
		{
			this->m_btnEtheta.SetCheck(BST_CHECKED);
		}
		else
		{
			this->m_btnEtheta.SetCheck(BST_UNCHECKED);
		}

		if (this->m_bRedox)
		{
			this->m_btnRedox.SetCheck(BST_CHECKED);
		}
		else
		{
			this->m_btnRedox.SetCheck(BST_UNCHECKED);
		}
	}
}

BEGIN_MESSAGE_MAP(CKPPitzerGeneral, baseCKPPitzerGeneral)
	ON_BN_SETFOCUS(IDC_CHECK_MACINNES, &CKPPitzerGeneral::OnBnSetfocusCheckMacinnes)
	ON_BN_SETFOCUS(IDC_CHECK_USE_ETHETA, &CKPPitzerGeneral::OnBnSetfocusCheckUseEtheta)
	ON_BN_SETFOCUS(IDC_CHECK_USE_REDOX, &CKPPitzerGeneral::OnBnSetfocusCheckUseRedox)
END_MESSAGE_MAP()

// CKPPitzerGeneral message handlers
BOOL CKPPitzerGeneral::OnInitDialog()
{
	baseCKPPitzerGeneral::OnInitDialog();

	// set layout
	CreateRoot(VERTICAL, 5, 6)

		<< (paneCtrl(IDC_STATIC_MAIN, HORIZONTAL, ABSOLUTE_VERT, nDefaultBorder, 10, 10)
			<< (pane(VERTICAL, GREEDY)
				<< item(IDC_CHECK_MACINNES, ABSOLUTE_VERT | ALIGN_CENTER)
				<< itemFixed(VERTICAL, 10)
				<< item(IDC_CHECK_USE_ETHETA, ABSOLUTE_VERT | ALIGN_CENTER)
				<< itemFixed(VERTICAL, 10)
				<< item(IDC_CHECK_USE_REDOX, ABSOLUTE_VERT | ALIGN_CENTER)
				)
			)
		<< (paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, ABSOLUTE_VERT, nDefaultBorder, 10, 10)
			<< item(IDC_DESCRIPTION, GREEDY)
			)
	;
	UpdateLayout();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CKPPitzerGeneral::OnBnSetfocusCheckMacinnes()
{
	// in order for a check box to recieve this notification
	// it must have the notify style set
	CString strRes;
	strRes.LoadString(IDS_STRING699);
	m_eInputDesc.SetWindowText(strRes);
}

void CKPPitzerGeneral::OnBnSetfocusCheckUseEtheta()
{
	// in order for a check box to recieve this notification
	// it must have the notify style set
	CString strRes;
	strRes.LoadString(IDS_STRING700);
	m_eInputDesc.SetWindowText(strRes);
}

void CKPPitzerGeneral::OnBnSetfocusCheckUseRedox()
{
	// in order for a check box to recieve this notification
	// it must have the notify style set
	CString strRes;
	strRes.LoadString(IDS_STRING701);
	m_eInputDesc.SetWindowText(strRes);
}
