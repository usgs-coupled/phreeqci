// UserGraphPg1.cpp : implementation file
//

#include "stdafx.h"
#include "phreeqci2.h"
#include "UserGraphPg1.h"

#include "DDX_DDV.h"
#include "DelayUpdate.h"
#include "BasicObj.h"
#include "UserGraph.h"
#include "phreeqc3/src/Utils.h"


// CUserGraphPg1 dialog

//IMPLEMENT_DYNAMIC(CUserGraphPg1, CPropertyPage)
IMPLEMENT_DYNCREATE(CUserGraphPg1, baseUserGraphPg1)

CUserGraphPg1::CUserGraphPg1() : baseUserGraphPg1(CUserGraphPg1::IDD)
, auto_min_x(true)
, auto_max_x(true)
, auto_maj_x(true)
, auto_minor_x(true)
, auto_min_y(true)
, auto_max_y(true)
, auto_maj_y(true)
, auto_minor_y(true)
, auto_min_y2(true)
, auto_max_y2(true)
, auto_maj_y2(true)
, auto_minor_y2(true)
, m_chart_type(CT_CONC_VS_X)
{
}

CUserGraphPg1::~CUserGraphPg1()
{
}

void CUserGraphPg1::DoDataExchange(CDataExchange* pDX)
{
	baseUserGraphPg1::DoDataExchange(pDX);

	if (m_bFirstSetActive)
	{
		//
		// Init NumDesc (USER_GRAPH doesn't support a range)
		//
		this->m_egNumDesc.SetColWidth(0, 0, 1100);
		this->m_egNumDesc.SetTextMatrix(0, 0, _T("Number"));
		this->m_egNumDesc.SetRowHeight(1, 0);
	}

	DDX_Text(pDX, IDC_E_HEAD, this->m_strHeadings);
	if (pDX->m_bSaveAndValidate)
	{
		this->m_strHeadings.Trim();
	}

	DDX_Text(pDX, IDC_EDIT_XAT,  this->axis_title_x);
	DDX_Text(pDX, IDC_EDIT_YAT,  this->axis_title_y);
	DDX_Text(pDX, IDC_EDIT_Y2AT, this->axis_title_y2);
	if (pDX->m_bSaveAndValidate)
	{
		this->axis_title_x.Trim();
		this->axis_title_y.Trim();
		this->axis_title_y2.Trim();
	}

	// -chart_title
	DDX_Text(pDX, IDC_EDIT_CT, this->chart_title);
	if (pDX->m_bSaveAndValidate)
	{
		this->chart_title.Trim();
	}

	// -plot_concentration_vs
	if (pDX->m_bSaveAndValidate)
	{
		if (this->GetCheckedRadioButton(IDC_RADIO_CONC_X, IDC_RADIO_CONC_T) == IDC_RADIO_CONC_T)
		{
			this->m_chart_type = CT_CONC_VS_T;
		}
		else
		{
			this->m_chart_type = CT_CONC_VS_X;
		}
	}
	else
	{
		if (this->m_chart_type == CT_CONC_VS_T)
		{
			this->CheckRadioButton(IDC_RADIO_CONC_X, IDC_RADIO_CONC_T, IDC_RADIO_CONC_T);
		}
		else
		{
			this->CheckRadioButton(IDC_RADIO_CONC_X, IDC_RADIO_CONC_T, IDC_RADIO_CONC_X);
		}
	}

	// min x
	if (pDX->m_bSaveAndValidate)
	{
		if (this->IsDlgButtonChecked(IDC_CHK_AUTO_MIN_X) == BST_CHECKED)
		{
			this->auto_min_x = true;
		}
		else
		{
			this->auto_min_x = false;
			DDX_Text(pDX, IDC_EDIT_MIN_X, min_x);
		}
	}
	else
	{
		if (this->auto_min_x)
		{
			this->CheckDlgButton(IDC_CHK_AUTO_MIN_X, BST_CHECKED);
		}
		else
		{
			this->CheckDlgButton(IDC_CHK_AUTO_MIN_X, BST_UNCHECKED);
			DDX_Text(pDX, IDC_EDIT_MIN_X, min_x);
		}
	}

	// max x
	if (pDX->m_bSaveAndValidate)
	{
		if (this->IsDlgButtonChecked(IDC_CHK_AUTO_MAX_X) == BST_CHECKED)
		{
			this->auto_max_x = true;
		}
		else
		{
			this->auto_max_x = false;
			DDX_Text(pDX, IDC_EDIT_MAX_X, max_x);
		}
	}
	else
	{
		if (this->auto_max_x)
		{
			this->CheckDlgButton(IDC_CHK_AUTO_MAX_X, BST_CHECKED);
		}
		else
		{
			this->CheckDlgButton(IDC_CHK_AUTO_MAX_X, BST_UNCHECKED);
			DDX_Text(pDX, IDC_EDIT_MAX_X, max_x);
		}
	}

	// major x
	if (pDX->m_bSaveAndValidate)
	{
		if (this->IsDlgButtonChecked(IDC_CHK_AUTO_MAJ_X) == BST_CHECKED)
		{
			this->auto_maj_x = true;
		}
		else
		{
			this->auto_maj_x = false;
			DDX_Text(pDX, IDC_EDIT_MAJ_X, maj_x);
		}
	}
	else
	{
		if (this->auto_maj_x)
		{
			this->CheckDlgButton(IDC_CHK_AUTO_MAJ_X, BST_CHECKED);
		}
		else
		{
			this->CheckDlgButton(IDC_CHK_AUTO_MAJ_X, BST_UNCHECKED);
			DDX_Text(pDX, IDC_EDIT_MAJ_X, maj_x);
		}
	}

	// minor x
	if (pDX->m_bSaveAndValidate)
	{
		if (this->IsDlgButtonChecked(IDC_CHK_AUTO_MINOR_X) == BST_CHECKED)
		{
			this->auto_minor_x = true;
		}
		else
		{
			this->auto_minor_x = false;
			DDX_Text(pDX, IDC_EDIT_MINOR_X, minor_x);
		}
	}
	else
	{
		if (this->auto_maj_x)
		{
			this->CheckDlgButton(IDC_CHK_AUTO_MINOR_X, BST_CHECKED);
		}
		else
		{
			this->CheckDlgButton(IDC_CHK_AUTO_MINOR_X, BST_UNCHECKED);
			DDX_Text(pDX, IDC_EDIT_MINOR_X, maj_x);
		}
	}

	// min y
	if (pDX->m_bSaveAndValidate)
	{
		if (this->IsDlgButtonChecked(IDC_CHK_AUTO_MIN_Y) == BST_CHECKED)
		{
			this->auto_min_y = true;
		}
		else
		{
			this->auto_min_y = false;
			DDX_Text(pDX, IDC_EDIT_MIN_Y, min_y);
		}
	}
	else
	{
		if (this->auto_min_y)
		{
			this->CheckDlgButton(IDC_CHK_AUTO_MIN_Y, BST_CHECKED);
		}
		else
		{
			this->CheckDlgButton(IDC_CHK_AUTO_MIN_Y, BST_UNCHECKED);
			DDX_Text(pDX, IDC_EDIT_MIN_Y, min_y);
		}
	}

	// max y
	if (pDX->m_bSaveAndValidate)
	{
		if (this->IsDlgButtonChecked(IDC_CHK_AUTO_MAX_Y) == BST_CHECKED)
		{
			this->auto_max_y = true;
		}
		else
		{
			this->auto_max_y = false;
			DDX_Text(pDX, IDC_EDIT_MAX_Y, max_y);
		}
	}
	else
	{
		if (this->auto_max_y)
		{
			this->CheckDlgButton(IDC_CHK_AUTO_MAX_Y, BST_CHECKED);
		}
		else
		{
			this->CheckDlgButton(IDC_CHK_AUTO_MAX_Y, BST_UNCHECKED);
			DDX_Text(pDX, IDC_EDIT_MAX_Y, max_y);
		}
	}

	// major y
	if (pDX->m_bSaveAndValidate)
	{
		if (this->IsDlgButtonChecked(IDC_CHK_AUTO_MAJ_Y) == BST_CHECKED)
		{
			this->auto_maj_y = true;
		}
		else
		{
			this->auto_maj_y = false;
			DDX_Text(pDX, IDC_EDIT_MAJ_Y, maj_y);
		}
	}
	else
	{
		if (this->auto_maj_y)
		{
			this->CheckDlgButton(IDC_CHK_AUTO_MAJ_Y, BST_CHECKED);
		}
		else
		{
			this->CheckDlgButton(IDC_CHK_AUTO_MAJ_Y, BST_UNCHECKED);
			DDX_Text(pDX, IDC_EDIT_MAJ_Y, maj_y);
		}
	}

	// minor y
	if (pDX->m_bSaveAndValidate)
	{
		if (this->IsDlgButtonChecked(IDC_CHK_AUTO_MINOR_Y) == BST_CHECKED)
		{
			this->auto_minor_y = true;
		}
		else
		{
			this->auto_minor_y = false;
			DDX_Text(pDX, IDC_EDIT_MINOR_Y, minor_y);
		}
	}
	else
	{
		if (this->auto_maj_y)
		{
			this->CheckDlgButton(IDC_CHK_AUTO_MINOR_Y, BST_CHECKED);
		}
		else
		{
			this->CheckDlgButton(IDC_CHK_AUTO_MINOR_Y, BST_UNCHECKED);
			DDX_Text(pDX, IDC_EDIT_MINOR_Y, maj_y);
		}
	}

	// min y2
	if (pDX->m_bSaveAndValidate)
	{
		if (this->IsDlgButtonChecked(IDC_CHK_AUTO_MIN_Y2) == BST_CHECKED)
		{
			this->auto_min_y2 = true;
		}
		else
		{
			this->auto_min_y2 = false;
			DDX_Text(pDX, IDC_EDIT_MIN_Y2, min_y2);
		}
	}
	else
	{
		if (this->auto_min_y2)
		{
			this->CheckDlgButton(IDC_CHK_AUTO_MIN_Y2, BST_CHECKED);
		}
		else
		{
			this->CheckDlgButton(IDC_CHK_AUTO_MIN_Y2, BST_UNCHECKED);
			DDX_Text(pDX, IDC_EDIT_MIN_Y2, min_y2);
		}
	}

	// max y2
	if (pDX->m_bSaveAndValidate)
	{
		if (this->IsDlgButtonChecked(IDC_CHK_AUTO_MAX_Y2) == BST_CHECKED)
		{
			this->auto_max_y2 = true;
		}
		else
		{
			this->auto_max_y2 = false;
			DDX_Text(pDX, IDC_EDIT_MAX_Y2, max_y2);
		}
	}
	else
	{
		if (this->auto_max_y2)
		{
			this->CheckDlgButton(IDC_CHK_AUTO_MAX_Y2, BST_CHECKED);
		}
		else
		{
			this->CheckDlgButton(IDC_CHK_AUTO_MAX_Y2, BST_UNCHECKED);
			DDX_Text(pDX, IDC_EDIT_MAX_Y2, max_y2);
		}
	}

	// major y2
	if (pDX->m_bSaveAndValidate)
	{
		if (this->IsDlgButtonChecked(IDC_CHK_AUTO_MAJ_Y2) == BST_CHECKED)
		{
			this->auto_maj_y2 = true;
		}
		else
		{
			this->auto_maj_y2 = false;
			DDX_Text(pDX, IDC_EDIT_MAJ_Y2, maj_y2);
		}
	}
	else
	{
		if (this->auto_maj_y2)
		{
			this->CheckDlgButton(IDC_CHK_AUTO_MAJ_Y2, BST_CHECKED);
		}
		else
		{
			this->CheckDlgButton(IDC_CHK_AUTO_MAJ_Y2, BST_UNCHECKED);
			DDX_Text(pDX, IDC_EDIT_MAJ_Y2, maj_y2);
		}
	}

	// minor y2
	if (pDX->m_bSaveAndValidate)
	{
		if (this->IsDlgButtonChecked(IDC_CHK_AUTO_MINOR_Y2) == BST_CHECKED)
		{
			this->auto_minor_y2 = true;
		}
		else
		{
			this->auto_minor_y2 = false;
			DDX_Text(pDX, IDC_EDIT_MINOR_Y2, minor_y2);
		}
	}
	else
	{
		if (this->auto_maj_y2)
		{
			this->CheckDlgButton(IDC_CHK_AUTO_MINOR_Y2, BST_CHECKED);
		}
		else
		{
			this->CheckDlgButton(IDC_CHK_AUTO_MINOR_Y2, BST_UNCHECKED);
			DDX_Text(pDX, IDC_EDIT_MINOR_Y2, maj_y2);
		}
	}

	// initial solutions
	if (pDX->m_bSaveAndValidate)
	{
		if (this->IsDlgButtonChecked(IDC_CHECK_INIT_SOLNS) == BST_CHECKED)
		{
			this->m_initial_solutions = true;
		}
		else
		{
			this->m_initial_solutions = false;
		}
	}
	else
	{
		if (this->m_initial_solutions)
		{
			this->CheckDlgButton(IDC_CHECK_INIT_SOLNS, BST_CHECKED);
		}
		else
		{
			this->CheckDlgButton(IDC_CHECK_INIT_SOLNS, BST_UNCHECKED);
		}
	}

	// connect simulations
	if (pDX->m_bSaveAndValidate)
	{
		if (this->IsDlgButtonChecked(IDC_CHECK_CONNECT_SIMS) == BST_CHECKED)
		{
			this->m_connect_simulations = true;
		}
		else
		{
			this->m_connect_simulations = false;
		}
	}
	else
	{
		if (this->m_connect_simulations)
		{
			this->CheckDlgButton(IDC_CHECK_CONNECT_SIMS, BST_CHECKED);
		}
		else
		{
			this->CheckDlgButton(IDC_CHECK_CONNECT_SIMS, BST_UNCHECKED);
		}
	}
	
	// detach
	if (pDX->m_bSaveAndValidate)
	{
		if (this->IsDlgButtonChecked(IDC_CHECK_DETACH) == BST_CHECKED)
		{
			this->m_detach = true;
		}
		else
		{
			this->m_detach = false;
		}
	}
	else
	{
		if (this->m_detach)
		{
			this->CheckDlgButton(IDC_CHECK_DETACH, BST_CHECKED);
		}
		else
		{
			this->CheckDlgButton(IDC_CHECK_DETACH, BST_UNCHECKED);
		}
		if (this->m_bFirstSetActive)
		{
			// update tabs
			this->OnBnClickedCheckDetach();
		}
	}

	// active
	if (pDX->m_bSaveAndValidate)
	{
		if (this->IsDlgButtonChecked(IDC_CHECK_ACTIVE) == BST_CHECKED)
		{
			this->m_active = true;
		}
		else
		{
			this->m_active = false;
		}
	}
	else
	{
		if (this->m_active)
		{
			this->CheckDlgButton(IDC_CHECK_ACTIVE, BST_CHECKED);
		}
		else
		{
			this->CheckDlgButton(IDC_CHECK_ACTIVE, BST_UNCHECKED);
		}
	}
}


BEGIN_MESSAGE_MAP(CUserGraphPg1, baseUserGraphPg1)
	//{{AFX_MSG_MAP(CKPUserPunchPg1)
	ON_WM_SIZE()
	ON_EN_SETFOCUS(IDC_E_HEAD, OnSetfocusEHead)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP

	// custom edit grid notifications
	ON_MESSAGE(EGN_BEGINCELLEDIT, OnBeginCellEdit)
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
	ON_MESSAGE(EGN_SETFOCUS, OnSetfocusGrid)
	ON_BN_CLICKED(IDC_CHECK_DETACH, &CUserGraphPg1::OnBnClickedCheckDetach)

	ON_BN_CLICKED(IDC_CHK_AUTO_MIN_X, &CUserGraphPg1::OnBnClickedChkAutoMinX)
	ON_BN_CLICKED(IDC_CHK_AUTO_MAX_X, &CUserGraphPg1::OnBnClickedChkAutoMaxX)
	ON_BN_CLICKED(IDC_CHK_AUTO_MAJ_X, &CUserGraphPg1::OnBnClickedChkAutoMajX)
	ON_BN_CLICKED(IDC_CHK_AUTO_MINOR_X, &CUserGraphPg1::OnBnClickedChkAutoMinorX)

	ON_BN_CLICKED(IDC_CHK_AUTO_MIN_Y, &CUserGraphPg1::OnBnClickedChkAutoMinY)
	ON_BN_CLICKED(IDC_CHK_AUTO_MAX_Y, &CUserGraphPg1::OnBnClickedChkAutoMaxY)
	ON_BN_CLICKED(IDC_CHK_AUTO_MAJ_Y, &CUserGraphPg1::OnBnClickedChkAutoMajY)
	ON_BN_CLICKED(IDC_CHK_AUTO_MINOR_Y, &CUserGraphPg1::OnBnClickedChkAutoMinorY)

	ON_BN_CLICKED(IDC_CHK_AUTO_MIN_Y2, &CUserGraphPg1::OnBnClickedChkAutoMinY2)
	ON_BN_CLICKED(IDC_CHK_AUTO_MAX_Y2, &CUserGraphPg1::OnBnClickedChkAutoMaxY2)
	ON_BN_CLICKED(IDC_CHK_AUTO_MAJ_Y2, &CUserGraphPg1::OnBnClickedChkAutoMajY2)
	ON_BN_CLICKED(IDC_CHK_AUTO_MINOR_Y2, &CUserGraphPg1::OnBnClickedChkAutoMinorY2)

	ON_BN_SETFOCUS(IDC_CHECK_DETACH, &CUserGraphPg1::OnBnSetfocusCheckDetach)
	ON_EN_SETFOCUS(IDC_EDIT_CT, &CUserGraphPg1::OnEnSetfocusEditCt)

	ON_BN_SETFOCUS(IDC_RADIO_CONC_X, &CUserGraphPg1::OnBnSetfocusRadioConc)
	ON_BN_SETFOCUS(IDC_RADIO_CONC_T, &CUserGraphPg1::OnBnSetfocusRadioConc)

	ON_BN_SETFOCUS(IDC_CHK_AUTO_MIN_X, &CUserGraphPg1::OnBnSetfocusChkAutoMinX)
	ON_BN_SETFOCUS(IDC_CHK_AUTO_MIN_Y, &CUserGraphPg1::OnBnSetfocusChkAutoMinY)
	ON_BN_SETFOCUS(IDC_CHK_AUTO_MIN_Y2, &CUserGraphPg1::OnBnSetfocusChkAutoMinY2)

	ON_BN_SETFOCUS(IDC_CHK_AUTO_MAX_X, &CUserGraphPg1::OnBnSetfocusChkAutoMaxX)
	ON_BN_SETFOCUS(IDC_CHK_AUTO_MAX_Y, &CUserGraphPg1::OnBnSetfocusChkAutoMaxY)
	ON_BN_SETFOCUS(IDC_CHK_AUTO_MAX_Y2, &CUserGraphPg1::OnBnSetfocusChkAutoMaxY2)

	ON_BN_SETFOCUS(IDC_CHK_AUTO_MAJ_X, &CUserGraphPg1::OnBnSetfocusChkAutoMajX)
	ON_BN_SETFOCUS(IDC_CHK_AUTO_MAJ_Y, &CUserGraphPg1::OnBnSetfocusChkAutoMajY)
	ON_BN_SETFOCUS(IDC_CHK_AUTO_MAJ_Y2, &CUserGraphPg1::OnBnSetfocusChkAutoMajY2)

	ON_BN_SETFOCUS(IDC_CHK_AUTO_MINOR_X, &CUserGraphPg1::OnBnSetfocusChkAutoMinorX)
	ON_BN_SETFOCUS(IDC_CHK_AUTO_MINOR_Y, &CUserGraphPg1::OnBnSetfocusChkAutoMinorY)
	ON_BN_SETFOCUS(IDC_CHK_AUTO_MINOR_Y2, &CUserGraphPg1::OnBnSetfocusChkAutoMinorY2)

	ON_EN_SETFOCUS(IDC_EDIT_MIN_X, &CUserGraphPg1::OnEnSetfocusEditMinX)
	ON_BN_SETFOCUS(IDC_EDIT_MIN_Y, &CUserGraphPg1::OnEnSetfocusEditMinY)
	ON_BN_SETFOCUS(IDC_EDIT_MIN_Y2, &CUserGraphPg1::OnEnSetfocusEditMinY2)

	ON_EN_SETFOCUS(IDC_EDIT_MAX_X, &CUserGraphPg1::OnEnSetfocusEditMaxX)
	ON_EN_SETFOCUS(IDC_EDIT_MAX_Y, &CUserGraphPg1::OnEnSetfocusEditMaxY)
	ON_EN_SETFOCUS(IDC_EDIT_MAX_Y2, &CUserGraphPg1::OnEnSetfocusEditMaxY2)

	ON_EN_SETFOCUS(IDC_EDIT_MAJ_X, &CUserGraphPg1::OnEnSetfocusEditMajX)
	ON_EN_SETFOCUS(IDC_EDIT_MAJ_Y, &CUserGraphPg1::OnEnSetfocusEditMajY)
	ON_EN_SETFOCUS(IDC_EDIT_MAJ_Y2, &CUserGraphPg1::OnEnSetfocusEditMajY2)

	ON_EN_SETFOCUS(IDC_EDIT_MINOR_X, &CUserGraphPg1::OnEnSetfocusEditMinorX)
	ON_EN_SETFOCUS(IDC_EDIT_MINOR_Y, &CUserGraphPg1::OnEnSetfocusEditMinorY)
	ON_EN_SETFOCUS(IDC_EDIT_MINOR_Y2, &CUserGraphPg1::OnEnSetfocusEditMinorY2)

	ON_BN_SETFOCUS(IDC_CHK_LOG_X, &CUserGraphPg1::OnBnSetfocusChkLogX)
	ON_BN_SETFOCUS(IDC_CHECK_INIT_SOLNS, &CUserGraphPg1::OnBnSetfocusCheckInitSolns)
	ON_BN_SETFOCUS(IDC_CHECK_CONNECT_SIMS, &CUserGraphPg1::OnBnSetfocusCheckConnectSims)
	ON_BN_SETFOCUS(IDC_CHECK_ACTIVE, &CUserGraphPg1::OnBnSetfocusCheckActive)

	ON_EN_SETFOCUS(IDC_EDIT_XAT, &CUserGraphPg1::OnEnSetfocusEditXat)
	ON_EN_SETFOCUS(IDC_EDIT_YAT, &CUserGraphPg1::OnEnSetfocusEditYat)
	ON_EN_SETFOCUS(IDC_EDIT_Y2AT, &CUserGraphPg1::OnEnSetfocusEditY2at)
END_MESSAGE_MAP()

// CUserGraphPg1 message handlers

BOOL CUserGraphPg1::OnInitDialog()
{
	baseUserGraphPg1::OnInitDialog();

	CreateRoot(VERTICAL, 5, 6)

		<< (pane(HORIZONTAL, ABSOLUTE_VERT)
			<< item(IDC_MSHFG_NUM_DESC, ABSOLUTE_VERT)
			)

		<< (pane(HORIZONTAL, GREEDY)
			<< item(IDC_CHECK_DETACH, ABSOLUTE_VERT | ALIGN_BOTTOM)
			)

		<< (paneCtrl(IDC_ST_GB_GENERAL, VERTICAL, GREEDY, nDefaultBorder, 10, 10)

			// chart title
			<< (pane(HORIZONTAL, GREEDY)
				<< item(IDC_STATIC_CT, NORESIZE | ALIGN_VCENTER)
				<< item(IDC_EDIT_CT, ABSOLUTE_VERT | ALIGN_VCENTER)
				)

			// legend titles
			<< (pane(HORIZONTAL, GREEDY)
				<< item(IDC_ST_HEAD, NORESIZE | ALIGN_VCENTER)
				<< item(IDC_E_HEAD, ABSOLUTE_VERT | ALIGN_VCENTER)
				)

			<< (pane(HORIZONTAL, GREEDY)
				// initial solutions
				<< item(IDC_CHECK_INIT_SOLNS, ABSOLUTE_VERT)
				// connect simulations
				<< item(IDC_CHECK_CONNECT_SIMS, ABSOLUTE_VERT)
				// active
				<< item(IDC_CHECK_ACTIVE, ABSOLUTE_VERT)
				)

			// plot_concentration_vs
			<< (pane(HORIZONTAL, GREEDY)
				<< item(IDC_STATIC_PLOT_CONC, NORESIZE | ALIGN_VCENTER)
				<< item(IDC_RADIO_CONC_X, NORESIZE | ALIGN_VCENTER)
				<< item(IDC_RADIO_CONC_T, NORESIZE | ALIGN_VCENTER)

				<< itemFixed(HORIZONTAL, 3)

				)

			)

		<< (paneCtrl(IDC_ST_GB_XAXIS, VERTICAL, GREEDY, nDefaultBorder, 10, 10)
			// x axis title
			<< (pane(HORIZONTAL, GREEDY)
				<< item(IDC_STATIC_XAT, NORESIZE | ALIGN_VCENTER)
				<< item(IDC_EDIT_XAT, ABSOLUTE_VERT | ALIGN_VCENTER)
				)

			<< (pane(HORIZONTAL, GREEDY)
				// x min
				<< item(IDC_ST_MIN_X, NORESIZE | ALIGN_VCENTER)
				<< item(IDC_EDIT_MIN_X, ABSOLUTE_VERT | ALIGN_VCENTER)
				<< item(IDC_CHK_AUTO_MIN_X, NORESIZE | ALIGN_VCENTER)
				<< itemFixed(HORIZONTAL, 3)
				// x max
				<< item(IDC_ST_MAX_X, NORESIZE | ALIGN_VCENTER)
				<< item(IDC_EDIT_MAX_X, ABSOLUTE_VERT | ALIGN_VCENTER)
				<< item(IDC_CHK_AUTO_MAX_X, NORESIZE | ALIGN_VCENTER)
				<< itemFixed(HORIZONTAL, 3)
				// x major
				<< item(IDC_ST_MAJ_X, NORESIZE | ALIGN_VCENTER)
				<< item(IDC_EDIT_MAJ_X, ABSOLUTE_VERT | ALIGN_VCENTER)
				<< item(IDC_CHK_AUTO_MAJ_X, NORESIZE | ALIGN_VCENTER)
				)

			<< (pane(HORIZONTAL, GREEDY)
				// x minor
				<< item(IDC_ST_MINOR_X, NORESIZE | ALIGN_VCENTER)
				<< item(IDC_EDIT_MINOR_X, ABSOLUTE_VERT | ALIGN_VCENTER)
				<< item(IDC_CHK_AUTO_MINOR_X, NORESIZE | ALIGN_VCENTER)
				<< itemFixed(HORIZONTAL, 3)
				<< itemSpaceLike(HORIZONTAL, IDC_ST_MAX_X)
				// x log
				<< item(IDC_CHK_LOG_X, ABSOLUTE_VERT | ALIGN_VCENTER)
				<< itemGrowing(HORIZONTAL)
				<< itemSpaceLike(HORIZONTAL, IDC_CHK_AUTO_MAX_X)
				<< itemFixed(HORIZONTAL, 3)
				<< itemSpaceLike(HORIZONTAL, IDC_ST_MAJ_X)
				<< itemSpaceLike(HORIZONTAL, IDC_CHK_AUTO_MAJ_X)
				)
			)

		<< (paneCtrl(IDC_ST_GB_YAXIS, VERTICAL, GREEDY, nDefaultBorder, 10, 10)
			// y axis title
			<< (pane(HORIZONTAL, GREEDY)
				<< item(IDC_STATIC_YAT, NORESIZE | ALIGN_VCENTER)
				<< item(IDC_EDIT_YAT, ABSOLUTE_VERT)
				)

			<< (pane(HORIZONTAL, GREEDY)
				// y min
				<< item(IDC_ST_MIN_Y, NORESIZE | ALIGN_VCENTER)
				<< item(IDC_EDIT_MIN_Y, ABSOLUTE_VERT)
				<< item(IDC_CHK_AUTO_MIN_Y, NORESIZE | ALIGN_VCENTER)
				<< itemFixed(HORIZONTAL, 3)
				// y max
				<< item(IDC_ST_MAX_Y, NORESIZE | ALIGN_VCENTER)
				<< item(IDC_EDIT_MAX_Y, ABSOLUTE_VERT)
				<< item(IDC_CHK_AUTO_MAX_Y, NORESIZE | ALIGN_VCENTER)
				<< itemFixed(HORIZONTAL, 3)
				// y major
				<< item(IDC_ST_MAJ_Y, NORESIZE | ALIGN_VCENTER)
				<< item(IDC_EDIT_MAJ_Y, ABSOLUTE_VERT)
				<< item(IDC_CHK_AUTO_MAJ_Y, NORESIZE | ALIGN_VCENTER)
				)

			<< (pane(HORIZONTAL, GREEDY)
				// y minor
				<< item(IDC_ST_MINOR_Y, NORESIZE | ALIGN_VCENTER)
				<< item(IDC_EDIT_MINOR_Y, ABSOLUTE_VERT | ALIGN_VCENTER)
				<< item(IDC_CHK_AUTO_MINOR_Y, NORESIZE | ALIGN_VCENTER)
				<< itemFixed(HORIZONTAL, 3)
				<< itemSpaceLike(HORIZONTAL, IDC_ST_MAX_X)
				// y log
				<< item(IDC_CHK_LOG_Y, ABSOLUTE_VERT | ALIGN_VCENTER)
				<< itemGrowing(HORIZONTAL)
				<< itemSpaceLike(HORIZONTAL, IDC_CHK_AUTO_MAX_X)
				<< itemFixed(HORIZONTAL, 3)
				<< itemSpaceLike(HORIZONTAL, IDC_ST_MAJ_X)
				<< itemSpaceLike(HORIZONTAL, IDC_CHK_AUTO_MAJ_X)
				)
			)

		<< (paneCtrl(IDC_ST_GB_Y2AXIS, VERTICAL, GREEDY, nDefaultBorder, 10, 10)
			// y axis title
			<< (pane(HORIZONTAL, GREEDY)
				<< item(IDC_STATIC_Y2AT, NORESIZE | ALIGN_VCENTER)
				<< item(IDC_EDIT_Y2AT, ABSOLUTE_VERT)
				)

			<< (pane(HORIZONTAL, GREEDY)
				// y2 min
				<< item(IDC_ST_MIN_Y2, NORESIZE | ALIGN_VCENTER)
				<< item(IDC_EDIT_MIN_Y2, ABSOLUTE_VERT)
				<< item(IDC_CHK_AUTO_MIN_Y2, NORESIZE | ALIGN_VCENTER)
				<< itemFixed(HORIZONTAL, 3)
				// y2 max
				<< item(IDC_ST_MAX_Y2, NORESIZE | ALIGN_VCENTER)
				<< item(IDC_EDIT_MAX_Y2, ABSOLUTE_VERT)
				<< item(IDC_CHK_AUTO_MAX_Y2, NORESIZE | ALIGN_VCENTER)
				<< itemFixed(HORIZONTAL, 3)
				// y2 major
				<< item(IDC_ST_MAJ_Y2, NORESIZE | ALIGN_VCENTER)
				<< item(IDC_EDIT_MAJ_Y2, ABSOLUTE_VERT)
				<< item(IDC_CHK_AUTO_MAJ_Y2, NORESIZE | ALIGN_VCENTER)
				)

			<< (pane(HORIZONTAL, GREEDY)
				// y2 minor
				<< item(IDC_ST_MINOR_Y2, NORESIZE | ALIGN_VCENTER)
				<< item(IDC_EDIT_MINOR_Y2, ABSOLUTE_VERT | ALIGN_VCENTER)
				<< item(IDC_CHK_AUTO_MINOR_Y2, NORESIZE | ALIGN_VCENTER)
				<< itemFixed(HORIZONTAL, 3)
				<< itemSpaceLike(HORIZONTAL, IDC_ST_MAX_X)
				// y2 log
				<< item(IDC_CHK_LOG_Y2, ABSOLUTE_VERT | ALIGN_VCENTER)
				<< itemGrowing(HORIZONTAL)
				<< itemSpaceLike(HORIZONTAL, IDC_CHK_AUTO_MAX_X)
				<< itemFixed(HORIZONTAL, 3)
				<< itemSpaceLike(HORIZONTAL, IDC_ST_MAJ_X)
				<< itemSpaceLike(HORIZONTAL, IDC_CHK_AUTO_MAJ_X)
				)
			)

		<< (paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, GREEDY, nDefaultBorder, 10, 10)
			<< item(IDC_E_DESC_INPUT, GREEDY)
			);
	UpdateLayout();

	// allow indention
	m_egBasic.SetTrimOnEntry(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUserGraphPg1::OnSize(UINT nType, int cx, int cy)
{
	baseUserGraphPg1::OnSize(nType, cx, cy);
}

BEGIN_EVENTSINK_MAP(CUserGraphPg1, baseUserGraphPg1)
    //{{AFX_EVENTSINK_MAP(CUserGraphPg1)
	ON_EVENT(CUserGraphPg1, IDC_MSHFG_NUM_DESC, 71 /* EnterCell */, OnEnterCellMshfgNumDesc, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


LRESULT CUserGraphPg1::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bDenyEdit = FALSE;	// allow edit

	return bDenyEdit;
}

LRESULT CUserGraphPg1::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bMakeChange = TRUE;

	// ignore if edit is cancelled
	if ( pInfo->item.pszText == NULL ) return bMakeChange;

	switch ( nID )
	{
	case IDC_MSHFG_NUM_DESC :
		// nothing to do
		break;

	default:
		ASSERT(FALSE);
		break;
	}

	return bMakeChange;
}

void CUserGraphPg1::OnEnterCellMshfgNumDesc() 
{
	CString strRes;
	switch (m_egNumDesc.GetRow())
	{
	case 0 :
		// Positive number to designate this %1 and its composition. Default is 1.
		AfxFormatString1(strRes, IDS_STRING711, _T("user-graph") ); 
		break;
	case 1 :
		ASSERT(FALSE);
		break;
	case 2 :
		// Optional comment that describes the %1. The description will appear
		// in the title of the chart window.
		AfxFormatString1(strRes, IDS_STRING712, _T("user-graph chart") ); 
		break;
	default :
		ASSERT(FALSE);
		break;
	}

	m_eInputDesc.SetWindowText(strRes);
}


LRESULT CUserGraphPg1::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
{
	UNUSED_ALWAYS(lParam);
	int nID = wParam;
	switch (nID)
	{
	case IDC_MSHFG_NUM_DESC :
		this->OnEnterCellMshfgNumDesc();
		break;
	default :
		ASSERT(FALSE);
		break;
	}
	return 0;
}

BOOL CUserGraphPg1::OnHelpInfo(HELPINFO* pHelpInfo) 
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

	CString strRes;

	switch (pHelpInfo->iCtrlId)
	{
	case IDC_MSHFG_NUM_DESC:
		if (!IsContextHelp())
		{
			if (!(m_egNumDesc.GetRowIsVisible(m_egNumDesc.GetRow()) && m_egNumDesc.GetColIsVisible(m_egNumDesc.GetCol())))
			{
				::MessageBeep((UINT)-1);
				return TRUE;
			}

			// modify placement
			CDC* pDC = m_egNumDesc.GetDC();
			int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
			int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

			long nLeft = ::MulDiv(m_egNumDesc.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
			long nTop  = ::MulDiv(m_egNumDesc.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

			CRect rc;
			m_egNumDesc.GetWindowRect(rc);

			myPopup.pt.x = rc.left + nLeft;
			myPopup.pt.y = rc.top + nTop + 10;
		}
		switch (IsContextHelp() ? m_egNumDesc.GetMouseRow() : m_egNumDesc.GetRow())
		{
		case 0 :
			AfxFormatString1(strRes, IDS_STRING711, _T("user-graph") ); 
			break;
		case 1 :
			ASSERT(FALSE);
			break;
		case 2 :
			AfxFormatString1(strRes, IDS_STRING712, _T("user-graph chart") ); 
			break;
		default :
			// No help topic is associated with this item. 
			strRes.LoadString(IDS_STRING441);
			break;
		}	
		break;
	case IDC_CHECK_DETACH:
		strRes.LoadString(IDS_STRING713);
		break;
	case IDC_STATIC_CT: case IDC_EDIT_CT:
		strRes.LoadString(IDS_STRING714);
		break;
	case IDC_ST_HEAD: case IDC_E_HEAD:
		strRes.LoadString(IDS_STRING715);
		break;
	case IDC_CHECK_INIT_SOLNS:
		strRes.LoadString(IDS_STRING726);
		break;		
	case IDC_CHECK_CONNECT_SIMS:
		strRes.LoadString(IDS_STRING727);
		break;		
	case IDC_CHECK_ACTIVE:
		strRes.LoadString(IDS_STRING728);
		break;
	case IDC_STATIC_PLOT_CONC: case IDC_RADIO_CONC_X: case IDC_RADIO_CONC_T:
		strRes.LoadString(IDS_STRING716);
		break;
	case IDC_STATIC_XAT: case IDC_EDIT_XAT:
		AfxFormatString1(strRes, IDS_STRING729, _T("x") );
		break;
	case IDC_STATIC_YAT: case IDC_EDIT_YAT:
		AfxFormatString1(strRes, IDS_STRING729, _T("y") );
		break;
	case IDC_STATIC_Y2AT: case IDC_EDIT_Y2AT:
		AfxFormatString1(strRes, IDS_STRING729, _T("secondary y") );
		break;
	case IDC_ST_MIN_X: case IDC_EDIT_MIN_X:
		AfxFormatString1(strRes, IDS_STRING721, _T("x") ); 
		break;
	case IDC_ST_MIN_Y: case IDC_EDIT_MIN_Y:
		AfxFormatString1(strRes, IDS_STRING721, _T("y") ); 
		break;
	case IDC_ST_MIN_Y2: case IDC_EDIT_MIN_Y2:
		AfxFormatString1(strRes, IDS_STRING721, _T("secondary y") ); 
		break;
	case IDC_ST_MAX_X: case IDC_EDIT_MAX_X:
		AfxFormatString1(strRes, IDS_STRING722, _T("x") ); 
		break;
	case IDC_ST_MAX_Y: case IDC_EDIT_MAX_Y:
		AfxFormatString1(strRes, IDS_STRING722, _T("y") ); 
		break;
	case IDC_ST_MAX_Y2: case IDC_EDIT_MAX_Y2:
		AfxFormatString1(strRes, IDS_STRING722, _T("secondary y") ); 
		break;
	case IDC_ST_MAJ_X: case IDC_EDIT_MAJ_X:
		AfxFormatString1(strRes, IDS_STRING723, _T("x") ); 
		break;
	case IDC_ST_MAJ_Y: case IDC_EDIT_MAJ_Y:
		AfxFormatString1(strRes, IDS_STRING723, _T("y") ); 
		break;
	case IDC_ST_MAJ_Y2: case IDC_EDIT_MAJ_Y2:
		AfxFormatString1(strRes, IDS_STRING723, _T("secondary y") ); 
		break;
	case IDC_ST_MINOR_X: case IDC_EDIT_MINOR_X:
		AfxFormatString1(strRes, IDS_STRING724, _T("x") ); 
		break;
	case IDC_ST_MINOR_Y: case IDC_EDIT_MINOR_Y:
		AfxFormatString1(strRes, IDS_STRING724, _T("y") ); 
		break;
	case IDC_ST_MINOR_Y2: case IDC_EDIT_MINOR_Y2:
		AfxFormatString1(strRes, IDS_STRING724, _T("secondary y") ); 
		break;
	case IDC_CHK_AUTO_MIN_X:
		AfxFormatString1(strRes, IDS_STRING717, _T("x") ); 
		break;
	case IDC_CHK_AUTO_MIN_Y:
		AfxFormatString1(strRes, IDS_STRING717, _T("y") ); 
		break;
	case IDC_CHK_AUTO_MIN_Y2:
		AfxFormatString1(strRes, IDS_STRING717, _T("secondary y") ); 
		break;
	case IDC_CHK_AUTO_MAX_X:
		AfxFormatString1(strRes, IDS_STRING718, _T("x") ); 
		break;
	case IDC_CHK_AUTO_MAX_Y:
		AfxFormatString1(strRes, IDS_STRING718, _T("y") ); 
		break;
	case IDC_CHK_AUTO_MAX_Y2:
		AfxFormatString1(strRes, IDS_STRING718, _T("secondary y") ); 
		break;
	case IDC_CHK_AUTO_MAJ_X:
		AfxFormatString1(strRes, IDS_STRING719, _T("x") ); 
		break;
	case IDC_CHK_AUTO_MAJ_Y:
		AfxFormatString1(strRes, IDS_STRING719, _T("y") ); 
		break;
	case IDC_CHK_AUTO_MAJ_Y2:
		AfxFormatString1(strRes, IDS_STRING719, _T("secondary y") ); 
		break;
	case IDC_CHK_AUTO_MINOR_X:
		AfxFormatString1(strRes, IDS_STRING720, _T("x") ); 
		break;
	case IDC_CHK_AUTO_MINOR_Y:
		AfxFormatString1(strRes, IDS_STRING720, _T("y") ); 
		break;
	case IDC_CHK_AUTO_MINOR_Y2:
		AfxFormatString1(strRes, IDS_STRING720, _T("secondary y") ); 
		break;
	case IDC_CHK_LOG_X:
		AfxFormatString1(strRes, IDS_STRING725, _T("x") ); 
		break;
	case IDC_CHK_LOG_Y:
		AfxFormatString1(strRes, IDS_STRING725, _T("y") ); 
		break;
	case IDC_CHK_LOG_Y2:
		AfxFormatString1(strRes, IDS_STRING725, _T("secondary y") ); 
		break;

	default:
		// No help topic is associated with this item. 
		strRes.LoadString(IDS_STRING441);
		break;
	}

	myPopup.pszText = strRes;
	return ::HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}

void CUserGraphPg1::OnBnClickedCheckDetach()
{
	int Ids[] = 
	{
		// general
		IDC_ST_GB_GENERAL,
		IDC_STATIC_CT,
		IDC_EDIT_CT,
		IDC_ST_HEAD,
		IDC_E_HEAD,
		IDC_CHECK_INIT_SOLNS,
		IDC_CHECK_CONNECT_SIMS,
		IDC_CHECK_ACTIVE,
		IDC_STATIC_PLOT_CONC,
		IDC_RADIO_CONC_X,
		IDC_RADIO_CONC_T,
		IDC_STATIC_PLOT_CVS,
		IDC_EDIT_CVS_FILE,
		IDC_BUTTON_CVS,
		// x-axis
		IDC_ST_GB_XAXIS,
		IDC_STATIC_XAT,
		IDC_EDIT_XAT,
		IDC_ST_MIN_X,
		IDC_EDIT_MIN_X,
		IDC_CHK_AUTO_MIN_X,
		IDC_ST_MAX_X,
		IDC_EDIT_MAX_X,
		IDC_CHK_AUTO_MAX_X,
		IDC_ST_MAJ_X,
		IDC_EDIT_MAJ_X,
		IDC_CHK_AUTO_MAJ_X,
		IDC_ST_MINOR_X,
		IDC_EDIT_MINOR_X,
		IDC_CHK_AUTO_MINOR_X,
		IDC_CHK_LOG_X,
		// y-axis
		IDC_ST_GB_YAXIS,
		IDC_STATIC_YAT,
		IDC_EDIT_YAT,
		IDC_ST_MIN_Y,
		IDC_EDIT_MIN_Y,
		IDC_CHK_AUTO_MIN_Y,
		IDC_ST_MAX_Y,
		IDC_EDIT_MAX_Y,
		IDC_CHK_AUTO_MAX_Y,
		IDC_ST_MAJ_Y,
		IDC_EDIT_MAJ_Y,
		IDC_CHK_AUTO_MAJ_Y,
		IDC_ST_MINOR_Y,
		IDC_EDIT_MINOR_Y,
		IDC_CHK_AUTO_MINOR_Y,
		IDC_CHK_LOG_Y,
		// y2-axis
		IDC_ST_GB_Y2AXIS,
		IDC_STATIC_Y2AT,
		IDC_EDIT_Y2AT,
		IDC_ST_MIN_Y2,
		IDC_EDIT_MIN_Y2,
		IDC_CHK_AUTO_MIN_Y2,
		IDC_ST_MAX_Y2,
		IDC_EDIT_MAX_Y2,
		IDC_CHK_AUTO_MAX_Y2,
		IDC_ST_MAJ_Y2,
		IDC_EDIT_MAJ_Y2,
		IDC_CHK_AUTO_MAJ_Y2,
		IDC_ST_MINOR_Y2,
		IDC_EDIT_MINOR_Y2,
		IDC_CHK_AUTO_MINOR_Y2,
		IDC_CHK_LOG_Y2,
	};

	BOOL bEnable = (this->IsDlgButtonChecked(IDC_CHECK_DETACH) != BST_CHECKED);

	for (int i = 0; i < sizeof(Ids) / sizeof(Ids[0]); ++i)
	{
		if (CWnd *pWnd = this->GetDlgItem(Ids[i]))
		{
			BOOL b = bEnable;
			switch (Ids[i])
			{
			// x-axis
			case IDC_EDIT_MIN_X:
				b &= (this->IsDlgButtonChecked(IDC_CHK_AUTO_MIN_X) != BST_CHECKED);
				break;
			case IDC_EDIT_MAX_X:
				b &= (this->IsDlgButtonChecked(IDC_CHK_AUTO_MAX_X) != BST_CHECKED);
				break;
			case IDC_EDIT_MAJ_X:
				b &= (this->IsDlgButtonChecked(IDC_CHK_AUTO_MAJ_X) != BST_CHECKED);
				break;
			case IDC_EDIT_MINOR_X:
				b &= (this->IsDlgButtonChecked(IDC_CHK_AUTO_MINOR_X) != BST_CHECKED);
				break;
			// y-axis
			case IDC_EDIT_MIN_Y:
				b &= (this->IsDlgButtonChecked(IDC_CHK_AUTO_MIN_Y) != BST_CHECKED);
				break;
			case IDC_EDIT_MAX_Y:
				b &= (this->IsDlgButtonChecked(IDC_CHK_AUTO_MAX_Y) != BST_CHECKED);
				break;
			case IDC_EDIT_MAJ_Y:
				b &= (this->IsDlgButtonChecked(IDC_CHK_AUTO_MAJ_Y) != BST_CHECKED);
				break;
			case IDC_EDIT_MINOR_Y:
				b &= (this->IsDlgButtonChecked(IDC_CHK_AUTO_MINOR_Y) != BST_CHECKED);
				break;
			// y2-axis
			case IDC_EDIT_MIN_Y2:
				b &= (this->IsDlgButtonChecked(IDC_CHK_AUTO_MIN_Y2) != BST_CHECKED);
				break;
			case IDC_EDIT_MAX_Y2:
				b &= (this->IsDlgButtonChecked(IDC_CHK_AUTO_MAX_Y2) != BST_CHECKED);
				break;
			case IDC_EDIT_MAJ_Y2:
				b &= (this->IsDlgButtonChecked(IDC_CHK_AUTO_MAJ_Y2) != BST_CHECKED);
				break;
			case IDC_EDIT_MINOR_Y2:
				b &= (this->IsDlgButtonChecked(IDC_CHK_AUTO_MINOR_Y2) != BST_CHECKED);
				break;
			}
			pWnd->EnableWindow(b);
		}
	}

	if (!bEnable)
	{
		if (CUserGraph *pSheet = (CUserGraph*)this->GetParent())
		{
			ASSERT(pSheet->IsKindOf(RUNTIME_CLASS(CUserGraph)) == TRUE);
			ASSERT(pSheet->GetPageCount() == 2 || pSheet->GetPageCount() == 1);
			if (pSheet->GetPageCount() == 2)
			{
				pSheet->RemovePage(pSheet->m_pPage2);
			}
		}
	}
	else
	{
		if (CUserGraph *pSheet = (CUserGraph*)this->GetParent())
		{
			ASSERT(pSheet->IsKindOf(RUNTIME_CLASS(CUserGraph)) == TRUE);
			ASSERT(pSheet->GetPageCount() == 2 || pSheet->GetPageCount() == 1);
			if (pSheet->GetPageCount() == 1)
			{
				CUserGraphPg2 *pg2 = pSheet->m_pPage2;
				pSheet->m_pPage2 = new CUserGraphPg2;
				pSheet->m_pPage2->m_listCommands.assign(pg2->m_listCommands.begin(), pg2->m_listCommands.end());
				pSheet->AddPage(pSheet->m_pPage2);
				delete pg2;
			}
		}
	}

}

void CUserGraphPg1::OnBnClickedChkAutoMinX()
{
	int Ids[] = 
	{
		IDC_EDIT_MIN_X
	};

	BOOL bEnable = (this->IsDlgButtonChecked(IDC_CHK_AUTO_MIN_X) != BST_CHECKED);

	for (int i = 0; i < sizeof(Ids) / sizeof(Ids[0]); ++i)
	{
		if (CWnd *pWnd = this->GetDlgItem(Ids[i]))
		{
			pWnd->EnableWindow(bEnable);
		}
	}
}

void CUserGraphPg1::OnBnClickedChkAutoMaxX()
{
	int Ids[] = 
	{
		IDC_EDIT_MAX_X
	};

	BOOL bEnable = (this->IsDlgButtonChecked(IDC_CHK_AUTO_MAX_X) != BST_CHECKED);

	for (int i = 0; i < sizeof(Ids) / sizeof(Ids[0]); ++i)
	{
		if (CWnd *pWnd = this->GetDlgItem(Ids[i]))
		{
			pWnd->EnableWindow(bEnable);
		}
	}
}

void CUserGraphPg1::OnBnClickedChkAutoMajX()
{
	int Ids[] = 
	{
		IDC_EDIT_MAJ_X
	};

	BOOL bEnable = (this->IsDlgButtonChecked(IDC_CHK_AUTO_MAJ_X) != BST_CHECKED);

	for (int i = 0; i < sizeof(Ids) / sizeof(Ids[0]); ++i)
	{
		if (CWnd *pWnd = this->GetDlgItem(Ids[i]))
		{
			pWnd->EnableWindow(bEnable);
		}
	}
}

void CUserGraphPg1::OnBnClickedChkAutoMinorX()
{
	int Ids[] = 
	{
		IDC_EDIT_MINOR_X
	};

	BOOL bEnable = (this->IsDlgButtonChecked(IDC_CHK_AUTO_MINOR_X) != BST_CHECKED);

	for (int i = 0; i < sizeof(Ids) / sizeof(Ids[0]); ++i)
	{
		if (CWnd *pWnd = this->GetDlgItem(Ids[i]))
		{
			pWnd->EnableWindow(bEnable);
		}
	}
}

void CUserGraphPg1::OnBnClickedChkAutoMinY()
{
	int Ids[] = 
	{
		IDC_EDIT_MIN_Y
	};

	BOOL bEnable = (this->IsDlgButtonChecked(IDC_CHK_AUTO_MIN_Y) != BST_CHECKED);

	for (int i = 0; i < sizeof(Ids) / sizeof(Ids[0]); ++i)
	{
		if (CWnd *pWnd = this->GetDlgItem(Ids[i]))
		{
			pWnd->EnableWindow(bEnable);
		}
	}
}

void CUserGraphPg1::OnBnClickedChkAutoMaxY()
{
	int Ids[] = 
	{
		IDC_EDIT_MAX_Y
	};

	BOOL bEnable = (this->IsDlgButtonChecked(IDC_CHK_AUTO_MAX_Y) != BST_CHECKED);

	for (int i = 0; i < sizeof(Ids) / sizeof(Ids[0]); ++i)
	{
		if (CWnd *pWnd = this->GetDlgItem(Ids[i]))
		{
			pWnd->EnableWindow(bEnable);
		}
	}
}

void CUserGraphPg1::OnBnClickedChkAutoMajY()
{
	int Ids[] = 
	{
		IDC_EDIT_MAJ_Y
	};

	BOOL bEnable = (this->IsDlgButtonChecked(IDC_CHK_AUTO_MAJ_Y) != BST_CHECKED);

	for (int i = 0; i < sizeof(Ids) / sizeof(Ids[0]); ++i)
	{
		if (CWnd *pWnd = this->GetDlgItem(Ids[i]))
		{
			pWnd->EnableWindow(bEnable);
		}
	}
}

void CUserGraphPg1::OnBnClickedChkAutoMinorY()
{
	int Ids[] = 
	{
		IDC_EDIT_MINOR_Y
	};

	BOOL bEnable = (this->IsDlgButtonChecked(IDC_CHK_AUTO_MINOR_Y) != BST_CHECKED);

	for (int i = 0; i < sizeof(Ids) / sizeof(Ids[0]); ++i)
	{
		if (CWnd *pWnd = this->GetDlgItem(Ids[i]))
		{
			pWnd->EnableWindow(bEnable);
		}
	}
}

void CUserGraphPg1::OnBnClickedChkAutoMinY2()
{
	int Ids[] = 
	{
		IDC_EDIT_MIN_Y2
	};

	BOOL bEnable = (this->IsDlgButtonChecked(IDC_CHK_AUTO_MIN_Y2) != BST_CHECKED);

	for (int i = 0; i < sizeof(Ids) / sizeof(Ids[0]); ++i)
	{
		if (CWnd *pWnd = this->GetDlgItem(Ids[i]))
		{
			pWnd->EnableWindow(bEnable);
		}
	}
}

void CUserGraphPg1::OnBnClickedChkAutoMaxY2()
{
	int Ids[] = 
	{
		IDC_EDIT_MAX_Y2
	};

	BOOL bEnable = (this->IsDlgButtonChecked(IDC_CHK_AUTO_MAX_Y2) != BST_CHECKED);

	for (int i = 0; i < sizeof(Ids) / sizeof(Ids[0]); ++i)
	{
		if (CWnd *pWnd = this->GetDlgItem(Ids[i]))
		{
			pWnd->EnableWindow(bEnable);
		}
	}
}

void CUserGraphPg1::OnBnClickedChkAutoMajY2()
{
	int Ids[] = 
	{
		IDC_EDIT_MAJ_Y2
	};

	BOOL bEnable = (this->IsDlgButtonChecked(IDC_CHK_AUTO_MAJ_Y2) != BST_CHECKED);

	for (int i = 0; i < sizeof(Ids) / sizeof(Ids[0]); ++i)
	{
		if (CWnd *pWnd = this->GetDlgItem(Ids[i]))
		{
			pWnd->EnableWindow(bEnable);
		}
	}
}

void CUserGraphPg1::OnBnClickedChkAutoMinorY2()
{
	int Ids[] = 
	{
		IDC_EDIT_MINOR_Y2
	};

	BOOL bEnable = (this->IsDlgButtonChecked(IDC_CHK_AUTO_MINOR_Y2) != BST_CHECKED);

	for (int i = 0; i < sizeof(Ids) / sizeof(Ids[0]); ++i)
	{
		if (CWnd *pWnd = this->GetDlgItem(Ids[i]))
		{
			pWnd->EnableWindow(bEnable);
		}
	}
}

void CUserGraphPg1::OnBnSetfocusCheckDetach()
{
	CString strRes;
	// Indicates that no more data will be added to the chart and that the identifying number
	// is available for a new USER_GRAPH definition. The chart window remains visible and all
	// mouse functions for the chart remain functional.
	strRes.LoadString(IDS_STRING713);
	m_eInputDesc.SetWindowText(strRes);
}

void CUserGraphPg1::OnEnSetfocusEditCt()
{
	CString strRes;
	// Identifier provides a title that is printed at the top of the chart.
	strRes.LoadString(IDS_STRING714);
	m_eInputDesc.SetWindowText(strRes);
}

void CUserGraphPg1::OnSetfocusEHead() 
{
	CString strRes;
	// List of labels, one for each of the curves. The labels can be changed in subsequent
	// simulations for proper identification of the parameters graphed (without need of 
	// repeating the Basic statements that define the data to be plotted).
	strRes.LoadString(IDS_STRING715);
	m_eInputDesc.SetWindowText(strRes);
}

void CUserGraphPg1::OnBnSetfocusRadioConc()
{
	CString strRes;
	// Identifier selects whether to plot distance or time on the X axis in advection or
	// transport simulations.
	strRes.LoadString(IDS_STRING716);
	m_eInputDesc.SetWindowText(strRes);
}

void CUserGraphPg1::OnBnSetfocusChkAutoMinX()
{
	CString strRes;
	// Select this to automatically set the minimum value for the %1 axis.
	AfxFormatString1(strRes, IDS_STRING717, _T("x") ); 
	m_eInputDesc.SetWindowText(strRes);
}

void CUserGraphPg1::OnBnSetfocusChkAutoMinY()
{
	CString strRes;
	// Select this to automatically set the minimum value for the %1 axis.
	AfxFormatString1(strRes, IDS_STRING717, _T("y") ); 
	m_eInputDesc.SetWindowText(strRes);
}

void CUserGraphPg1::OnBnSetfocusChkAutoMinY2()
{
	CString strRes;
	// Select this to automatically set the minimum value for the %1 axis.
	AfxFormatString1(strRes, IDS_STRING717, _T("secondary y") ); 
	m_eInputDesc.SetWindowText(strRes);
}

void CUserGraphPg1::OnBnSetfocusChkAutoMaxX()
{
	CString strRes;
	// Select this to automatically set the maximum value for the %1 axis.
	AfxFormatString1(strRes, IDS_STRING718, _T("x") ); 
	m_eInputDesc.SetWindowText(strRes);
}

void CUserGraphPg1::OnBnSetfocusChkAutoMaxY()
{
	CString strRes;
	// Select this to automatically set the maximum value for the %1 axis.
	AfxFormatString1(strRes, IDS_STRING718, _T("y") ); 
	m_eInputDesc.SetWindowText(strRes);
}

void CUserGraphPg1::OnBnSetfocusChkAutoMaxY2()
{
	CString strRes;
	// Select this to automatically set the maximum value for the %1 axis.
	AfxFormatString1(strRes, IDS_STRING718, _T("secondary y") ); 
	m_eInputDesc.SetWindowText(strRes);
}

void CUserGraphPg1::OnBnSetfocusChkAutoMajX()
{
	CString strRes;
	// Select this to automatically set the major tic marks for the %1 axis.
	AfxFormatString1(strRes, IDS_STRING719, _T("x") ); 
	m_eInputDesc.SetWindowText(strRes);
}

void CUserGraphPg1::OnBnSetfocusChkAutoMajY()
{
	CString strRes;
	// Select this to automatically set the major tic marks for the %1 axis.
	AfxFormatString1(strRes, IDS_STRING719, _T("y") ); 
	m_eInputDesc.SetWindowText(strRes);
}

void CUserGraphPg1::OnBnSetfocusChkAutoMajY2()
{
	CString strRes;
	// Select this to automatically set the major tic marks for the %1 axis.
	AfxFormatString1(strRes, IDS_STRING719, _T("secondary y") ); 
	m_eInputDesc.SetWindowText(strRes);
}

void CUserGraphPg1::OnBnSetfocusChkAutoMinorX()
{
	CString strRes;
	// Select this to automatically set the minor tic marks for the %1 axis.
	AfxFormatString1(strRes, IDS_STRING720, _T("x") ); 
	m_eInputDesc.SetWindowText(strRes);
}

void CUserGraphPg1::OnBnSetfocusChkAutoMinorY()
{
	CString strRes;
	// Select this to automatically set the minor tic marks for the %1 axis.
	AfxFormatString1(strRes, IDS_STRING720, _T("y") ); 
	m_eInputDesc.SetWindowText(strRes);
}

void CUserGraphPg1::OnBnSetfocusChkAutoMinorY2()
{
	CString strRes;
	// Select this to automatically set the minor tic marks for the %1 axis.
	AfxFormatString1(strRes, IDS_STRING720, _T("secondary y") ); 
	m_eInputDesc.SetWindowText(strRes);
}

void CUserGraphPg1::OnEnSetfocusEditMinX()
{
	CString strRes;
	// Use this to manually set the minimum value for the %1 axis.
	AfxFormatString1(strRes, IDS_STRING721, _T("x") ); 
	m_eInputDesc.SetWindowText(strRes);
}

void CUserGraphPg1::OnEnSetfocusEditMinY()
{
	CString strRes;
	// Use this to manually set the minimum value for the %1 axis.
	AfxFormatString1(strRes, IDS_STRING721, _T("y") ); 
	m_eInputDesc.SetWindowText(strRes);
}

void CUserGraphPg1::OnEnSetfocusEditMinY2()
{
	CString strRes;
	// Use this to manually set the minimum value for the %1 axis.
	AfxFormatString1(strRes, IDS_STRING721, _T("secondary y") ); 
	m_eInputDesc.SetWindowText(strRes);
}

void CUserGraphPg1::OnEnSetfocusEditMaxX()
{
	CString strRes;
	// Use this to manually set the maximum value for the %1 axis.
	AfxFormatString1(strRes, IDS_STRING722, _T("x") ); 
	m_eInputDesc.SetWindowText(strRes);
}

void CUserGraphPg1::OnEnSetfocusEditMaxY()
{
	CString strRes;
	// Use this to manually set the maximum value for the %1 axis.
	AfxFormatString1(strRes, IDS_STRING722, _T("y") ); 
	m_eInputDesc.SetWindowText(strRes);
}

void CUserGraphPg1::OnEnSetfocusEditMaxY2()
{
	CString strRes;
	// Use this to manually set the maximum value for the %1 axis.
	AfxFormatString1(strRes, IDS_STRING722, _T("secondary y") ); 
	m_eInputDesc.SetWindowText(strRes);
}

void CUserGraphPg1::OnEnSetfocusEditMajX()
{
	CString strRes;
	// Use this to manually set the major tic marks for the %1 axis.
	AfxFormatString1(strRes, IDS_STRING723, _T("x") ); 
	m_eInputDesc.SetWindowText(strRes);
}

void CUserGraphPg1::OnEnSetfocusEditMajY()
{
	CString strRes;
	// Use this to manually set the major tic marks for the %1 axis.
	AfxFormatString1(strRes, IDS_STRING723, _T("y") ); 
	m_eInputDesc.SetWindowText(strRes);
}

void CUserGraphPg1::OnEnSetfocusEditMajY2()
{
	CString strRes;
	// Use this to manually set the major tic marks for the %1 axis.
	AfxFormatString1(strRes, IDS_STRING723, _T("secondary y") ); 
	m_eInputDesc.SetWindowText(strRes);
}

void CUserGraphPg1::OnEnSetfocusEditMinorX()
{
	CString strRes;
	// Use this to manually set the minor tic marks for the %1 axis.
	AfxFormatString1(strRes, IDS_STRING724, _T("x") ); 
	m_eInputDesc.SetWindowText(strRes);
}

void CUserGraphPg1::OnEnSetfocusEditMinorY()
{
	CString strRes;
	// Use this to manually set the minor tic marks for the %1 axis.
	AfxFormatString1(strRes, IDS_STRING724, _T("y") ); 
	m_eInputDesc.SetWindowText(strRes);
}

void CUserGraphPg1::OnEnSetfocusEditMinorY2()
{
	CString strRes;
	// Use this to manually set the minor tic marks for the %1 axis.
	AfxFormatString1(strRes, IDS_STRING724, _T("secondary y") ); 
	m_eInputDesc.SetWindowText(strRes);
}

void CUserGraphPg1::OnBnSetfocusChkLogX()
{
	CString strRes;
	// Select this to logarithmically scale the %1 axis.
	AfxFormatString1(strRes, IDS_STRING725, _T("x") ); 
	m_eInputDesc.SetWindowText(strRes);
}

void CUserGraphPg1::OnBnSetfocusChkLogY()
{
	CString strRes;
	// Select this to logarithmically scale the %1 axis.
	AfxFormatString1(strRes, IDS_STRING725, _T("y") ); 
	m_eInputDesc.SetWindowText(strRes);
}

void CUserGraphPg1::OnBnSetfocusChkLogY2()
{
	CString strRes;
	// Select this to logarithmically scale the %1 axis.
	AfxFormatString1(strRes, IDS_STRING725, _T("secondary y") ); 
	m_eInputDesc.SetWindowText(strRes);
}

void CUserGraphPg1::OnBnSetfocusCheckInitSolns()
{
	CString strRes;
	// Selects to plot results from initial solution, initial exchange,
	// initial surface, and initial gas-phase calculations. Default is false.
	strRes.LoadString(IDS_STRING726);
	m_eInputDesc.SetWindowText(strRes);
}

void CUserGraphPg1::OnBnSetfocusCheckConnectSims()
{
	CString strRes;
	// Selects to retain curve properties (colors, symbols, line widths)
	// in subsequent simulations, or in subsequent shifts for transport
	// and advection simulations. Default value is true.
	strRes.LoadString(IDS_STRING727);
	m_eInputDesc.SetWindowText(strRes);
}

void CUserGraphPg1::OnBnSetfocusCheckActive()
{
	CString strRes;
	// Allows plotting of data to the chart to be suspended or resumed. Default is true.
	strRes.LoadString(IDS_STRING728);
	m_eInputDesc.SetWindowText(strRes);
}

void CUserGraphPg1::OnEnSetfocusEditXat()
{
	CString strRes;
	// Label printed below the chart along the %1 axis.
	AfxFormatString1(strRes, IDS_STRING729, _T("x") ); 
	m_eInputDesc.SetWindowText(strRes);
}

void CUserGraphPg1::OnEnSetfocusEditYat()
{
	CString strRes;
	// Label printed below the chart along the %1 axis.
	AfxFormatString1(strRes, IDS_STRING729, _T("y") ); 
	m_eInputDesc.SetWindowText(strRes);
}

void CUserGraphPg1::OnEnSetfocusEditY2at()
{
	CString strRes;
	// Label printed below the chart along the %1 axis.
	AfxFormatString1(strRes, IDS_STRING729, _T("secondary y") ); 
	m_eInputDesc.SetWindowText(strRes);
}
