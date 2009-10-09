// KPPrintPg1.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "KPPrintPg1.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CKPPrintPg1, baseCKPPrintPg1)


/////////////////////////////////////////////////////////////////////////////
// CKPPrintPg1 property page

CKPPrintPg1::CKPPrintPg1() : baseCKPPrintPg1(CKPPrintPg1::IDD)
{
	//{{AFX_DATA_INIT(CKPPrintPg1)
	//}}AFX_DATA_INIT
	m_nLimit = -1;
	for (int i = 0; i < sizeof(m_arrValue) / sizeof(m_arrValue[0]); ++i)
	{
		m_arrValue[i] = AS_IS;
	}
}

CKPPrintPg1::~CKPPrintPg1()
{
}

void CKPPrintPg1::DoDataExchange(CDataExchange* pDX)
{
	baseCKPPrintPg1::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKPPrintPg1)
	DDX_Control(pDX, IDC_E_DESC_INPUT, m_eInputDesc);
	DDX_Control(pDX, IDC_SPIN_LIMIT, m_spinLimit);
	//}}AFX_DATA_MAP

	if (m_bFirstSetActive)
	{
		UDACCEL uda[7];
		uda[0].nSec = 0;
		uda[0].nInc = 1;
		uda[1].nSec = 2;
		uda[1].nInc = 2;
		uda[2].nSec = 4;
		uda[2].nInc = 10;
		uda[3].nSec = 8;
		uda[3].nInc = 100;
		uda[4].nSec = 16;
		uda[4].nInc = 1000;
		uda[5].nSec = 32;
		uda[5].nInc = 10000;
		uda[6].nSec = 64;
		uda[6].nInc = 100000;

		// init spinners
		m_spinLimit.SetRange32(1, INT_MAX - 1);
		m_spinLimit.SetAccel(7, uda);
	}

	if (pDX->m_bSaveAndValidate)
	{
		CButton* pBtn = (CButton*)GetDlgItem(IDC_R_LIMIT);
		if (pBtn->GetCheck() == BST_CHECKED)
		{
			DDX_Text(pDX, IDC_E_LIMIT, m_nLimit);
			DDV_MinMaxInt(pDX, m_nLimit, 1, INT_MAX);
		}
		else
		{
			pBtn = (CButton*)GetDlgItem(IDC_R_ALL);
			if (pBtn->GetCheck() == BST_CHECKED)
			{
				m_nLimit = -1;
			}
			else
			{
				m_nLimit = 0;
			}
		}

	}
	else
	{
		if (m_nLimit > 0)
		{
			DDX_Text(pDX, IDC_E_LIMIT, m_nLimit);
			CButton* pBtn = (CButton*)GetDlgItem(IDC_R_LIMIT);
			pBtn->SetCheck(BST_CHECKED);
			OnClickedRadio(IDC_R_LIMIT);
		}
		else
		{
			int nDefLimit = 10;
			DDX_Text(pDX, IDC_E_LIMIT, nDefLimit);
			if (m_nLimit == 0)
			{
				CButton* pBtn = (CButton*)GetDlgItem(IDC_R_NONE);
				pBtn->SetCheck(BST_CHECKED);
				OnClickedRadio(IDC_R_NONE);
			}
			else
			{
				CButton* pBtn = (CButton*)GetDlgItem(IDC_R_ALL);
				pBtn->SetCheck(BST_CHECKED);
				OnClickedRadio(IDC_R_ALL);
			}
		}
	}


	for (int i = 0; i < sizeof(m_arrValue) / sizeof(m_arrValue[0]); ++i)
	{
		ASSERT(m_arrValue[i] == AS_IS || m_arrValue[i] == AS_TRUE || m_arrValue[i] == AS_FALSE);
		int nValue = static_cast<int>(m_arrValue[i]);
		DDX_CBIndex(pDX, IDC_CBO_00 + i, nValue);
		if (pDX->m_bSaveAndValidate)
		{
			m_arrValue[i] = static_cast<ValueType>(nValue);
			ASSERT(m_arrValue[i] == AS_IS || m_arrValue[i] == AS_TRUE || m_arrValue[i] == AS_FALSE);
		}
	}
}


BEGIN_MESSAGE_MAP(CKPPrintPg1, baseCKPPrintPg1)
	//{{AFX_MSG_MAP(CKPPrintPg1)
	ON_BN_CLICKED(IDC_B_AS_IS, OnBAsIs)
	ON_BN_CLICKED(IDC_B_TRUE, OnBTrue)
	ON_BN_CLICKED(IDC_B_FALSE, OnBFalse)
	ON_WM_HELPINFO()
	ON_EN_SETFOCUS(IDC_E_LIMIT, OnSetfocusELimit)
	//}}AFX_MSG_MAP
	ON_CONTROL_RANGE(CBN_SETFOCUS, IDC_CBO_00, IDC_CBO_16, OnSetfocusCbo)
	ON_CONTROL_RANGE(BN_SETFOCUS, IDC_B_AS_IS, IDC_B_FALSE, OnSetfocusBtn)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_R_ALL, IDC_R_LIMIT, OnClickedRadio)
	ON_CONTROL_RANGE(BN_SETFOCUS, IDC_R_ALL, IDC_R_LIMIT, OnSetfocusRadioBtn)
END_MESSAGE_MAP()

BOOL CKPPrintPg1::OnInitDialog() 
{
	baseCKPPrintPg1::OnInitDialog();
	
	// Add extra initialization here
	// set layout
	CreateRoot(VERTICAL, 5, 6) 
		<< (paneCtrl(IDC_S_PRINT_OPTS, HORIZONTAL, ABSOLUTE_VERT, 0, 3, 16, 0)
			<< (pane(VERTICAL, GREEDY, 0, 0, 0)
				<< (pane(HORIZONTAL, GREEDY, 0, 0, 0)
					<< item(IDC_STATIC_00, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_CBO_00, NORESIZE | ALIGN_CENTER)
					<< itemGrowing(HORIZONTAL)
					<< item(IDC_STATIC_09, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_CBO_09, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					)
				<< itemFixed(VERTICAL, 7)
				<< (pane(HORIZONTAL, GREEDY, 0, 0, 0)
					<< item(IDC_STATIC_01, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_CBO_01, NORESIZE | ALIGN_CENTER)
					<< itemGrowing(HORIZONTAL)
					<< item(IDC_STATIC_10A, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_CBO_10, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					)
				<< itemFixed(VERTICAL, 7)
				<< (pane(HORIZONTAL, GREEDY, 0, 0, 0)
					<< item(IDC_STATIC_02, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_CBO_02, NORESIZE | ALIGN_CENTER)
					<< itemGrowing(HORIZONTAL)
					<< item(IDC_STATIC_11, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_CBO_11, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					)
				<< itemFixed(VERTICAL, 7)
				<< (pane(HORIZONTAL, GREEDY, 0, 0, 0)
					<< item(IDC_STATIC_03, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_CBO_03, NORESIZE | ALIGN_CENTER)
					<< itemGrowing(HORIZONTAL)
					<< item(IDC_STATIC_12, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_CBO_12, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					)
				<< itemFixed(VERTICAL, 7)
				<< (pane(HORIZONTAL, GREEDY, 0, 0, 0)
					<< item(IDC_STATIC_04, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_CBO_04, NORESIZE | ALIGN_CENTER)
					<< itemGrowing(HORIZONTAL)
					<< item(IDC_STATIC_13, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_CBO_13, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					)
				<< itemFixed(VERTICAL, 7)
				<< (pane(HORIZONTAL, GREEDY, 0, 0, 0)
					<< item(IDC_STATIC_05, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_CBO_05, NORESIZE | ALIGN_CENTER)
					<< itemGrowing(HORIZONTAL)
					<< item(IDC_STATIC_14, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_CBO_14, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					)
				<< itemFixed(VERTICAL, 7)
				<< (pane(HORIZONTAL, GREEDY, 0, 0, 0)
					<< item(IDC_STATIC_06, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_CBO_06, NORESIZE | ALIGN_CENTER)
					<< itemGrowing(HORIZONTAL)
					<< item(IDC_STATIC_15, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_CBO_15, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					)
				<< itemFixed(VERTICAL, 7)
				<< (pane(HORIZONTAL, GREEDY, 0, 0, 0)
					<< item(IDC_STATIC_07, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_CBO_07, NORESIZE | ALIGN_CENTER)
					<< itemGrowing(HORIZONTAL)
					<< item(IDC_STATIC_16, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_CBO_16, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					)
				<< itemFixed(VERTICAL, 7)
				<< (pane(HORIZONTAL, GREEDY, 0, 0, 0)
					<< item(IDC_STATIC_08, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_CBO_08, NORESIZE | ALIGN_CENTER)
					<< itemGrowing(HORIZONTAL)
					<< item(IDC_B_AS_IS, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_B_TRUE, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_B_FALSE, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					)
				<< itemFixed(VERTICAL, 7)
				)
			)
		<< (paneCtrl(IDC_S_WARNINGS, VERTICAL, ABSOLUTE_VERT, nDefaultBorder, 10, 10)
			<< (pane(HORIZONTAL, GREEDY, 0, 0, 0)
				<< (pane(HORIZONTAL, GREEDY, 0, 0, 0)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_R_ALL, ABSOLUTE_VERT | ALIGN_CENTER)
					)
				<< (pane(HORIZONTAL, GREEDY, 0, 0, 0)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_R_LIMIT, NORESIZE | ALIGN_CENTER)
					<< item(IDC_E_LIMIT, NORESIZE | ALIGN_CENTER)
					<< item(IDC_SPIN_LIMIT, NORESIZE | ALIGN_CENTER)
					)
				)
			<< (pane(HORIZONTAL, GREEDY, 0, 0, 0)
				<< itemFixed(HORIZONTAL, 10)
				<< item(IDC_R_NONE, ABSOLUTE_VERT | ALIGN_CENTER)
				)
			)
		<< (paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, GREEDY, nDefaultBorder, 10, 10)
			<< item(IDC_E_DESC_INPUT, GREEDY)
			)
	;
	UpdateLayout();

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKPPrintPg1::OnBAsIs() 
{
	for (int i = 0; i < sizeof(m_arrValue) / sizeof(m_arrValue[0]); ++i)
	{
		m_arrValue[i] = AS_IS;
	}
	UpdateData(FALSE);
}

void CKPPrintPg1::OnBTrue() 
{
	for (int i = 0; i < sizeof(m_arrValue) / sizeof(m_arrValue[0]); ++i)
	{
		m_arrValue[i] = AS_TRUE;
	}
	UpdateData(FALSE);
}

void CKPPrintPg1::OnBFalse() 
{
	for (int i = 0; i < sizeof(m_arrValue) / sizeof(m_arrValue[0]); ++i)
	{
		m_arrValue[i] = AS_FALSE;
	}
	UpdateData(FALSE);
}

void CKPPrintPg1::OnSetfocusCbo(UINT nID) 
{
	UINT nResID = nID - IDC_CBO_00 + IDS_PRINT_00;

	CString strRes;

	VERIFY(strRes.LoadString(nResID));

	m_eInputDesc.SetWindowText(strRes);	
}

void CKPPrintPg1::OnSetfocusBtn(UINT nID) 
{
	UINT nResID = nID - IDC_B_AS_IS + IDS_PRINT_B_0;

	CString strRes;

	VERIFY(strRes.LoadString(nResID));

	m_eInputDesc.SetWindowText(strRes);	
}

void CKPPrintPg1::OnSetfocusRadioBtn(UINT nID) 
{
	UINT nResID = nID - IDC_R_ALL + IDS_STRING535;

	CString strRes;

	VERIFY(strRes.LoadString(nResID));

	m_eInputDesc.SetWindowText(strRes);	
}

void CKPPrintPg1::OnClickedRadio(UINT nID)
{
	ASSERT(IDC_R_ALL <= nID && nID <= IDC_R_LIMIT);
	ASSERT(IDC_R_ALL < IDC_R_NONE && IDC_R_NONE < IDC_R_LIMIT);

	CWnd* pWnd = GetDlgItem(IDC_E_LIMIT);
	pWnd->EnableWindow(nID == IDC_R_LIMIT);

	pWnd = GetDlgItem(IDC_SPIN_LIMIT);
	pWnd->EnableWindow(nID == IDC_R_LIMIT);
}


BOOL CKPPrintPg1::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// Declare the popup structure and initialize it.
	HH_POPUP        myPopup;
	memset(&myPopup, 0, sizeof(HH_POPUP));
	
	// Fill in the popup structure
	myPopup.cbStruct         = sizeof(HH_POPUP);
	myPopup.rcMargins.top    = 5;
	myPopup.rcMargins.bottom = 5;
	myPopup.rcMargins.left   = 5;
	myPopup.rcMargins.right  = 5;
	myPopup.clrForeground    = (COLORREF)-1;  // use default
	myPopup.clrBackground    = (COLORREF)-1;  // use default
	myPopup.idString         = 0;             // set the ID string to 0

	// Set placement
	if (IsContextHelp())
	{
		myPopup.pt.x = pHelpInfo->MousePos.x;
		myPopup.pt.y = pHelpInfo->MousePos.y;
	}
	else
	{
		if (pHelpInfo->hItemHandle)
		{
			// Center pt of window w/ offset
			CRect rc;
			::GetWindowRect((HWND)pHelpInfo->hItemHandle, &rc);
			myPopup.pt = rc.CenterPoint();
			myPopup.pt.y += 10;
		}
	}

	// hack since IDC_S_XX are non-contiguous
	switch (pHelpInfo->iCtrlId)
	{
	case IDC_STATIC_00:
		pHelpInfo->iCtrlId = IDC_CBO_00;
		break;
	case IDC_STATIC_01:
		pHelpInfo->iCtrlId = IDC_CBO_01;
		break;
	case IDC_STATIC_02:
		pHelpInfo->iCtrlId = IDC_CBO_02;
		break;
	case IDC_STATIC_03:
		pHelpInfo->iCtrlId = IDC_CBO_03;
		break;
	case IDC_STATIC_04:
		pHelpInfo->iCtrlId = IDC_CBO_04;
		break;
	case IDC_STATIC_05:
		pHelpInfo->iCtrlId = IDC_CBO_05;
		break;
	case IDC_STATIC_06:
		pHelpInfo->iCtrlId = IDC_CBO_06;
		break;
	case IDC_STATIC_07:
		pHelpInfo->iCtrlId = IDC_CBO_07;
		break;
	case IDC_STATIC_08:
		pHelpInfo->iCtrlId = IDC_CBO_08;
		break;
	case IDC_STATIC_09:
		pHelpInfo->iCtrlId = IDC_CBO_09;
		break;
	case IDC_STATIC_10A:
		pHelpInfo->iCtrlId = IDC_CBO_10;
		break;
	case IDC_STATIC_11:
		pHelpInfo->iCtrlId = IDC_CBO_11;
		break;
	case IDC_STATIC_12:
		pHelpInfo->iCtrlId = IDC_CBO_12;
		break;
	case IDC_STATIC_13:
		pHelpInfo->iCtrlId = IDC_CBO_13;
		break;
	case IDC_STATIC_14:
		pHelpInfo->iCtrlId = IDC_CBO_14;
		break;
	case IDC_STATIC_15:
		pHelpInfo->iCtrlId = IDC_CBO_15;
		break;
	case IDC_STATIC_16:
		pHelpInfo->iCtrlId = IDC_CBO_16;
		break;
	default:
		// do nothing
		break;
	}

	CString strRes;

	if (IDC_B_AS_IS <= pHelpInfo->iCtrlId && pHelpInfo->iCtrlId <= IDC_B_FALSE)
	{
		VERIFY(strRes.LoadString(pHelpInfo->iCtrlId - IDC_B_AS_IS + IDS_PRINT_B_0));
	}
	else if (IDC_CBO_00 <= pHelpInfo->iCtrlId && pHelpInfo->iCtrlId <= IDC_CBO_16)
	{
		VERIFY(strRes.LoadString(pHelpInfo->iCtrlId - IDC_CBO_00 + IDS_PRINT_00));
	}
	else
	{
		switch (pHelpInfo->iCtrlId)
		{
		case IDC_R_ALL:
			VERIFY(strRes.LoadString(IDS_STRING535));
			break;
		case IDC_R_NONE:
			VERIFY(strRes.LoadString(IDS_STRING536));
			break;
		case IDC_R_LIMIT:
			VERIFY(strRes.LoadString(IDS_STRING537));
			break;
		case IDC_E_LIMIT: case IDC_SPIN_LIMIT:
			VERIFY(strRes.LoadString(IDS_STRING538));
			break;
		default:
			// No help topic is associated with this item. 
			strRes.LoadString(IDS_STRING441);
			break;
		}
	}
	myPopup.pszText = strRes;
	return ::HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}


void CKPPrintPg1::OnSetfocusELimit() 
{
	CString strRes;
	VERIFY(strRes.LoadString(IDS_STRING538));
	m_eInputDesc.SetWindowText(strRes);	
}
