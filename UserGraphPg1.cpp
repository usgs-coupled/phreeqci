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
// COMMENT: {5/1/2012 8:06:34 PM}, m_basicDesc(GetDatabase(), IDC_LB_FUNCS, IDC_E_EXPLAN, IDC_TREE_ARGS)
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

// COMMENT: {5/1/2012 6:28:31 PM}	DDX_Control(pDX, IDC_MSHFG_BASIC, m_egBasic);
	DDX_Text(pDX, IDC_E_HEAD, m_strHead);

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

// COMMENT: {5/1/2012 6:28:12 PM}
// COMMENT: {5/1/2012 6:28:12 PM}	m_basicDesc.DoDataExchange(pDX);
// COMMENT: {5/1/2012 6:28:12 PM}	DDX_ListCommands(pDX);
}


BEGIN_MESSAGE_MAP(CUserGraphPg1, baseUserGraphPg1)
	//{{AFX_MSG_MAP(CKPUserPunchPg1)
// COMMENT: {5/1/2012 8:11:57 PM}	ON_LBN_SELCHANGE(IDC_LB_FUNCS, OnSelchangeLbFuncs)
// COMMENT: {5/1/2012 8:11:57 PM}	ON_LBN_SETFOCUS(IDC_LB_FUNCS, OnSetfocusLbFuncs)
// COMMENT: {5/1/2012 8:11:57 PM}	ON_LBN_SETFOCUS(IDC_LB_ARGS, OnSetfocusLbArgs)
// COMMENT: {5/1/2012 8:11:57 PM}	ON_EN_SETFOCUS(IDC_E_EXPLAN, OnSetfocusEExplan)
	ON_WM_SIZE()
// COMMENT: {5/1/2012 8:17:21 PM}	ON_BN_CLICKED(IDC_B_RENUMBER, OnBRenumber)
	ON_EN_SETFOCUS(IDC_E_HEAD, OnSetfocusEHead)
	ON_WM_HELPINFO()
// COMMENT: {5/1/2012 8:19:14 PM}	ON_NOTIFY(NM_SETFOCUS, IDC_TREE_ARGS, OnSetfocusTreeArgs)
	//}}AFX_MSG_MAP

	// custom edit grid notifications
	ON_MESSAGE(EGN_BEGINCELLEDIT, OnBeginCellEdit)
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
	ON_MESSAGE(EGN_SETFOCUS, OnSetfocusGrid)
	ON_BN_CLICKED(IDC_CHECK_DETACH, &CUserGraphPg1::OnBnClickedCheckDetach)
	ON_BN_CLICKED(IDC_CHK_AUTO_MIN_X, &CUserGraphPg1::OnBnClickedChkAutoMinX)
	ON_BN_CLICKED(IDC_CHK_AUTO_MAX_X, &CUserGraphPg1::OnBnClickedChkAutoMaxX)
	ON_BN_CLICKED(IDC_CHK_AUTO_MAJ_X, &CUserGraphPg1::OnBnClickedChkAutoMajX)
	ON_BN_CLICKED(IDC_CHK_AUTO_MIN_Y, &CUserGraphPg1::OnBnClickedChkAutoMinY)
	ON_BN_CLICKED(IDC_CHK_AUTO_MAX_Y, &CUserGraphPg1::OnBnClickedChkAutoMaxY)
	ON_BN_CLICKED(IDC_CHK_AUTO_MAJ_Y, &CUserGraphPg1::OnBnClickedChkAutoMajY)
	ON_BN_CLICKED(IDC_CHK_AUTO_MIN_Y2, &CUserGraphPg1::OnBnClickedChkAutoMinY2)
	ON_BN_CLICKED(IDC_CHK_AUTO_MAX_Y2, &CUserGraphPg1::OnBnClickedChkAutoMaxY2)
	ON_BN_CLICKED(IDC_CHK_AUTO_MAJ_Y2, &CUserGraphPg1::OnBnClickedChkAutoMajY2)
END_MESSAGE_MAP()

// CUserGraphPg1 message handlers

BOOL CUserGraphPg1::OnInitDialog()
{
	baseUserGraphPg1::OnInitDialog();

	CreateRoot(VERTICAL, 5, 6)
		//{{
		<< (pane(HORIZONTAL, ABSOLUTE_VERT)
			<< item(IDC_MSHFG_NUM_DESC, ABSOLUTE_VERT)
			)
		//}}

		//{{
// COMMENT: {5/3/2012 10:17:16 PM}		<< itemFixed(VERTICAL, 4) // itemSpaceLike(HORIZONTAL, IDC_CHECK_DETACH)
		<< (pane(HORIZONTAL, GREEDY)
			///<< itemFixed(HORIZONTAL, 30) // itemSpaceLike(HORIZONTAL, IDC_CHECK_DETACH)
			<< item(IDC_CHECK_DETACH, ABSOLUTE_VERT | ALIGN_BOTTOM)
			)
// COMMENT: {5/3/2012 10:17:09 PM}		<< itemFixed(VERTICAL, 1) // itemSpaceLike(HORIZONTAL, IDC_CHECK_DETACH)
		//}}

// COMMENT: {5/2/2012 9:14:30 PM}		<< item(IDC_ST_HEAD, ABSOLUTE_VERT | ALIGN_BOTTOM)
// COMMENT: {5/2/2012 9:14:30 PM}		<< item(IDC_E_HEAD, ABSOLUTE_VERT)
		//{{
		//}}
		///<< itemFixed(VERTICAL, 40) // itemSpaceLike(HORIZONTAL, IDC_CHECK_DETACH)

		
// COMMENT: {5/3/2012 10:14:17 PM}		<< (paneCtrl(IDC_STATIC_PROPS, VERTICAL, GREEDY, nDefaultBorder, 10, 10)

			<< (paneCtrl(IDC_ST_GB_XAXIS, VERTICAL, GREEDY, nDefaultBorder, 10, 10)
				// x axis title
				<< (pane(HORIZONTAL, GREEDY)
					<< item(IDC_STATIC_XAT, NORESIZE | ALIGN_VCENTER)
					<< item(IDC_EDIT_XAT, ABSOLUTE_VERT | ALIGN_VCENTER)
					)
// COMMENT: {5/3/2012 10:29:42 PM}				<< itemFixed(VERTICAL, 3)
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
// COMMENT: {5/3/2012 10:29:35 PM}				<< itemFixed(VERTICAL, 3)
				<< (pane(HORIZONTAL, GREEDY)
					// x log
					<< item(IDC_CHK_LOG_X, ABSOLUTE_VERT | ALIGN_VCENTER)
					)
				)

// COMMENT: {5/3/2012 10:18:15 PM}			<< itemFixed(VERTICAL, 5)

			<< (paneCtrl(IDC_ST_GB_YAXIS, VERTICAL, GREEDY, nDefaultBorder, 10, 10)
				// y axis title
				<< (pane(HORIZONTAL, GREEDY)
					<< item(IDC_STATIC_YAT, NORESIZE | ALIGN_VCENTER)
					<< item(IDC_EDIT_YAT, ABSOLUTE_VERT)
					)
// COMMENT: {5/3/2012 10:33:58 PM}				<< itemFixed(VERTICAL, 3)
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
// COMMENT: {5/3/2012 10:34:02 PM}				<< itemFixed(VERTICAL, 3)
				<< (pane(HORIZONTAL, GREEDY)
					// y log
					<< item(IDC_CHK_LOG_Y, ABSOLUTE_VERT)
					)
				)

// COMMENT: {5/3/2012 10:18:20 PM}			<< itemFixed(VERTICAL, 5)

			<< (paneCtrl(IDC_ST_GB_Y2AXIS, VERTICAL, GREEDY, nDefaultBorder, 10, 10)
				// y axis title
				<< (pane(HORIZONTAL, GREEDY)
					<< item(IDC_STATIC_Y2AT, NORESIZE | ALIGN_VCENTER)
					<< item(IDC_EDIT_Y2AT, ABSOLUTE_VERT)
					)
// COMMENT: {5/3/2012 10:34:07 PM}				<< itemFixed(VERTICAL, 3)
				<< (pane(HORIZONTAL, GREEDY)
					// y min
					<< item(IDC_ST_MIN_Y2, NORESIZE | ALIGN_VCENTER)
					<< item(IDC_EDIT_MIN_Y2, ABSOLUTE_VERT)
					<< item(IDC_CHK_AUTO_MIN_Y2, NORESIZE | ALIGN_VCENTER)
					<< itemFixed(HORIZONTAL, 3)
					// y max
					<< item(IDC_ST_MAX_Y2, NORESIZE | ALIGN_VCENTER)
					<< item(IDC_EDIT_MAX_Y2, ABSOLUTE_VERT)
					<< item(IDC_CHK_AUTO_MAX_Y2, NORESIZE | ALIGN_VCENTER)
					<< itemFixed(HORIZONTAL, 3)
					// y major
					<< item(IDC_ST_MAJ_Y2, NORESIZE | ALIGN_VCENTER)
					<< item(IDC_EDIT_MAJ_Y2, ABSOLUTE_VERT)
					<< item(IDC_CHK_AUTO_MAJ_Y2, NORESIZE | ALIGN_VCENTER)
					)
// COMMENT: {5/3/2012 10:34:10 PM}				<< itemFixed(VERTICAL, 3)
				<< (pane(HORIZONTAL, GREEDY)
					// y log
					<< item(IDC_CHK_LOG_Y2, ABSOLUTE_VERT)
					)
				)

// COMMENT: {5/3/2012 10:18:59 PM}			<< itemFixed(VERTICAL, 5)


// COMMENT: {5/3/2012 4:09:27 PM}			// Legend titles
// COMMENT: {5/3/2012 4:09:27 PM}			<< item(IDC_ST_HEAD, ABSOLUTE_VERT | ALIGN_BOTTOM)
// COMMENT: {5/3/2012 4:09:27 PM}			<< item(IDC_E_HEAD, ABSOLUTE_VERT)
// COMMENT: {5/3/2012 4:09:27 PM}
// COMMENT: {5/3/2012 4:09:27 PM}
// COMMENT: {5/3/2012 4:09:27 PM}			// Y axis title
// COMMENT: {5/3/2012 4:09:27 PM}			<< item(IDC_STATIC_YAT, ABSOLUTE_VERT | ALIGN_BOTTOM)
// COMMENT: {5/3/2012 4:09:27 PM}			<< item(IDC_EDIT_YAT, ABSOLUTE_VERT)
// COMMENT: {5/3/2012 4:09:27 PM}
// COMMENT: {5/3/2012 4:09:27 PM}			// Y2 axis title
// COMMENT: {5/3/2012 4:09:27 PM}			<< item(IDC_STATIC_Y2AT, ABSOLUTE_VERT | ALIGN_BOTTOM)
// COMMENT: {5/3/2012 4:09:27 PM}			<< item(IDC_EDIT_Y2AT, ABSOLUTE_VERT)
// COMMENT: {5/3/2012 4:09:27 PM}
// COMMENT: {5/3/2012 4:09:27 PM}			// Chart title
// COMMENT: {5/3/2012 4:09:27 PM}			<< item(IDC_STATIC_CT, ABSOLUTE_VERT | ALIGN_BOTTOM)
// COMMENT: {5/3/2012 4:09:27 PM}			<< item(IDC_EDIT_CT, ABSOLUTE_VERT)
// COMMENT: {5/3/2012 4:09:27 PM}
// COMMENT: {5/3/2012 4:09:27 PM}			<< item(IDC_CHECK_INIT_SOLNS, ABSOLUTE_VERT | ALIGN_BOTTOM)
// COMMENT: {5/3/2012 4:09:27 PM}			<< item(IDC_CHECK_CONNECT_SIMS, ABSOLUTE_VERT | ALIGN_BOTTOM)
// COMMENT: {5/3/2012 4:09:27 PM}			<< item(IDC_CHECK_ACTIVE, ABSOLUTE_VERT | ALIGN_BOTTOM)

// COMMENT: {5/3/2012 10:14:26 PM}			)

		/***
		<< (pane(HORIZONTAL, GREEDY)
			<< item(IDC_ST_BASIC, ABSOLUTE_VERT | ALIGN_BOTTOM)
			<< item(IDC_B_RENUMBER, NORESIZE)
			)
			<< item(IDC_MSHFG_BASIC, GREEDY)
			<< (pane(HORIZONTAL, GREEDY)
				<< (pane(VERTICAL, ABSOLUTE_VERT | ALIGN_BOTTOM)
					<< item(IDC_ST_FUNCS, ABSOLUTE_VERT | ALIGN_BOTTOM)
					<< item(IDC_LB_FUNCS, ABSOLUTE_VERT | ALIGN_BOTTOM)
					)
				<< (pane(VERTICAL, ABSOLUTE_VERT | ALIGN_BOTTOM)
					<< item(IDC_ST_EXPLAN, ABSOLUTE_VERT | ALIGN_BOTTOM)
					<< item(IDC_E_EXPLAN, ABSOLUTE_VERT | ALIGN_BOTTOM)
					)
				<< (pane(VERTICAL, ABSOLUTE_VERT | ALIGN_BOTTOM)
					<< item(IDC_ST_ARGS, ABSOLUTE_VERT | ALIGN_BOTTOM)
					<< item(IDC_TREE_ARGS, ABSOLUTE_VERT | ALIGN_BOTTOM)
					)
				)
		***/
		<< (paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, GREEDY, nDefaultBorder, 10, 10)
			<< item(IDC_E_DESC_INPUT, GREEDY)
			);
	UpdateLayout();

	// allow indention
	m_egBasic.SetTrimOnEntry(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// COMMENT: {5/1/2012 8:12:22 PM}void CUserGraphPg1::OnSelchangeLbFuncs()
// COMMENT: {5/1/2012 8:12:22 PM}{
// COMMENT: {5/1/2012 8:12:22 PM}	m_basicDesc.OnSelchangeLbFuncs();
// COMMENT: {5/1/2012 8:12:22 PM}}
// COMMENT: {5/1/2012 8:12:22 PM}
// COMMENT: {5/1/2012 8:12:22 PM}void CUserGraphPg1::OnSetfocusLbFuncs()
// COMMENT: {5/1/2012 8:12:22 PM}{
// COMMENT: {5/1/2012 8:12:22 PM}	CString strRes;
// COMMENT: {5/1/2012 8:12:22 PM}	strRes.LoadString(IDS_RATE_258);
// COMMENT: {5/1/2012 8:12:22 PM}	m_eInputDesc.SetWindowText(strRes);
// COMMENT: {5/1/2012 8:12:22 PM}}
// COMMENT: {5/1/2012 8:12:22 PM}
// COMMENT: {5/1/2012 8:12:22 PM}void CUserGraphPg1::OnSetfocusLbArgs()
// COMMENT: {5/1/2012 8:12:22 PM}{
// COMMENT: {5/1/2012 8:12:22 PM}	CString strRes;
// COMMENT: {5/1/2012 8:12:22 PM}	strRes.LoadString(IDS_RATE_260);
// COMMENT: {5/1/2012 8:12:22 PM}	m_eInputDesc.SetWindowText(strRes);
// COMMENT: {5/1/2012 8:12:22 PM}}
// COMMENT: {5/1/2012 8:12:22 PM}
// COMMENT: {5/1/2012 8:12:22 PM}void CUserGraphPg1::OnSetfocusEExplan()
// COMMENT: {5/1/2012 8:12:22 PM}{
// COMMENT: {5/1/2012 8:12:22 PM}	CString strRes;
// COMMENT: {5/1/2012 8:12:22 PM}	strRes.LoadString(IDS_RATE_259);
// COMMENT: {5/1/2012 8:12:22 PM}	m_eInputDesc.SetWindowText(strRes);
// COMMENT: {5/1/2012 8:12:22 PM}}

void CUserGraphPg1::OnSetfocusBRenumber() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING512);
	m_eInputDesc.SetWindowText(strRes);
}

void CUserGraphPg1::OnSize(UINT nType, int cx, int cy)
{
	baseUserGraphPg1::OnSize(nType, cx, cy);

// COMMENT: {5/1/2012 8:13:28 PM}	// resize the columns within the grid
// COMMENT: {5/1/2012 8:13:28 PM}	if (m_egBasic.GetSafeHwnd())
// COMMENT: {5/1/2012 8:13:28 PM}	{
// COMMENT: {5/1/2012 8:13:28 PM}		long nCol0 = m_egBasic.GetColWidth(0, 0);
// COMMENT: {5/1/2012 8:13:28 PM}
// COMMENT: {5/1/2012 8:13:28 PM}		CRect rect;
// COMMENT: {5/1/2012 8:13:28 PM}		CDC* pDC = GetDC();
// COMMENT: {5/1/2012 8:13:28 PM}		int nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
// COMMENT: {5/1/2012 8:13:28 PM}		m_egBasic.GetClientRect(&rect);
// COMMENT: {5/1/2012 8:13:28 PM}		m_egBasic.SetColWidth(1, 0, MulDiv(rect.right, TWIPS_PER_INCH, nLogX) - nCol0/* - 50*/);
// COMMENT: {5/1/2012 8:13:28 PM}	}
}

// COMMENT: {5/1/2012 8:15:49 PM}void CUserGraphPg1::OnBRenumber()
// COMMENT: {5/1/2012 8:15:49 PM}{
// COMMENT: {5/1/2012 8:15:49 PM}	m_bRenumbering = true;
// COMMENT: {5/1/2012 8:15:49 PM}
// COMMENT: {5/1/2012 8:15:49 PM}	if (UpdateData(TRUE))
// COMMENT: {5/1/2012 8:15:49 PM}	{
// COMMENT: {5/1/2012 8:15:49 PM}		CBasicObj basicCheck;
// COMMENT: {5/1/2012 8:15:49 PM}		if (basicCheck.Renumber(m_listCommands))
// COMMENT: {5/1/2012 8:15:49 PM}		{
// COMMENT: {5/1/2012 8:15:49 PM}			// update grid
// COMMENT: {5/1/2012 8:15:49 PM}			DisplayCommands();
// COMMENT: {5/1/2012 8:15:49 PM}		}
// COMMENT: {5/1/2012 8:15:49 PM}	}
// COMMENT: {5/1/2012 8:15:49 PM}	else
// COMMENT: {5/1/2012 8:15:49 PM}	{
// COMMENT: {5/1/2012 8:15:49 PM}		// do nothing BASIC contains errors
// COMMENT: {5/1/2012 8:15:49 PM}	}
// COMMENT: {5/1/2012 8:15:49 PM}	m_bRenumbering = false;
// COMMENT: {5/1/2012 8:15:49 PM}}

// COMMENT: {5/1/2012 8:16:09 PM}void CUserGraphPg1::DDX_ListCommands(CDataExchange *pDX)
// COMMENT: {5/1/2012 8:16:09 PM}{
// COMMENT: {5/1/2012 8:16:09 PM}	// validate  state
// COMMENT: {5/1/2012 8:16:09 PM}	ASSERT_VALID(this);
// COMMENT: {5/1/2012 8:16:09 PM}
// COMMENT: {5/1/2012 8:16:09 PM}	// avoid flashing grid
// COMMENT: {5/1/2012 8:16:09 PM}	CDelayUpdate update(this, IDC_MSHFG_BASIC);
// COMMENT: {5/1/2012 8:16:09 PM}
// COMMENT: {5/1/2012 8:16:09 PM}	if (pDX->m_bSaveAndValidate)
// COMMENT: {5/1/2012 8:16:09 PM}	{
// COMMENT: {5/1/2012 8:16:09 PM}		// create object to check basic
// COMMENT: {5/1/2012 8:16:09 PM}		CBasicObj basicCheck;
// COMMENT: {5/1/2012 8:16:09 PM}
// COMMENT: {5/1/2012 8:16:09 PM}		std::list<basic_command> updatedList;
// COMMENT: {5/1/2012 8:16:09 PM}		std::list<basic_command> cleanedList;
// COMMENT: {5/1/2012 8:16:09 PM}
// COMMENT: {5/1/2012 8:16:09 PM}		// save view
// COMMENT: {5/1/2012 8:16:09 PM}		long nRowSave = m_egBasic.GetRow();
// COMMENT: {5/1/2012 8:16:09 PM}		long nTopRowSave = m_egBasic.GetTopRow();
// COMMENT: {5/1/2012 8:16:09 PM}
// COMMENT: {5/1/2012 8:16:09 PM}		for (long nRow = m_egBasic.GetFixedRows(); nRow < m_egBasic.GetRows(); ++nRow)
// COMMENT: {5/1/2012 8:16:09 PM}		{
// COMMENT: {5/1/2012 8:16:09 PM}			basic_command command;
// COMMENT: {5/1/2012 8:16:09 PM}			basicCheck.DDX_BasicCommand(pDX, IDC_MSHFG_BASIC, nRow, 0, command);
// COMMENT: {5/1/2012 8:16:09 PM}
// COMMENT: {5/1/2012 8:16:09 PM}			if (command.strCommand.IsEmpty())
// COMMENT: {5/1/2012 8:16:09 PM}			{
// COMMENT: {5/1/2012 8:16:09 PM}				// don't add empty commands to list
// COMMENT: {5/1/2012 8:16:09 PM}				continue;
// COMMENT: {5/1/2012 8:16:09 PM}			}
// COMMENT: {5/1/2012 8:16:09 PM}			updatedList.push_back(command);
// COMMENT: {5/1/2012 8:16:09 PM}
// COMMENT: {5/1/2012 8:16:09 PM}			// cleanup cmd
// COMMENT: {5/1/2012 8:16:09 PM}			std::string cmd(command.strCommand);
// COMMENT: {5/1/2012 8:16:09 PM}			std::string cmd_lower = cmd;
// COMMENT: {5/1/2012 8:16:09 PM}			Utilities::str_tolower(cmd_lower);
// COMMENT: {5/1/2012 8:16:09 PM}			if (cmd_lower.find("plot_xy") != std::string::npos)
// COMMENT: {5/1/2012 8:16:09 PM}			{
// COMMENT: {5/1/2012 8:16:09 PM}				ChartObject::ExtractCurveInfo(cmd); // truncates cmd
// COMMENT: {5/1/2012 8:16:09 PM}			}
// COMMENT: {5/1/2012 8:16:09 PM}			basic_command cleaned(command);
// COMMENT: {5/1/2012 8:16:09 PM}			cleaned.strCommand = cmd.c_str();
// COMMENT: {5/1/2012 8:16:09 PM}			cleanedList.push_back(cleaned);
// COMMENT: {5/1/2012 8:16:09 PM}		}
// COMMENT: {5/1/2012 8:16:09 PM}
// COMMENT: {5/1/2012 8:16:09 PM}		// check by running
// COMMENT: {5/1/2012 8:16:09 PM}		basicCheck.DDV_BasicCommands(pDX, IDC_MSHFG_BASIC, cleanedList, m_bRenumbering);
// COMMENT: {5/1/2012 8:16:09 PM}
// COMMENT: {5/1/2012 8:16:09 PM}		// restore view
// COMMENT: {5/1/2012 8:16:09 PM}		m_egBasic.SetRow(nRowSave);
// COMMENT: {5/1/2012 8:16:09 PM}		m_egBasic.SetTopRow(nTopRowSave);
// COMMENT: {5/1/2012 8:16:09 PM}
// COMMENT: {5/1/2012 8:16:09 PM}		m_listCommands.assign(updatedList.begin(), updatedList.end());
// COMMENT: {5/1/2012 8:16:09 PM}	}
// COMMENT: {5/1/2012 8:16:09 PM}	else
// COMMENT: {5/1/2012 8:16:09 PM}	{
// COMMENT: {5/1/2012 8:16:09 PM}		if (m_bFirstSetActive)
// COMMENT: {5/1/2012 8:16:09 PM}		{
// COMMENT: {5/1/2012 8:16:09 PM}			// layout basic grid
// COMMENT: {5/1/2012 8:16:09 PM}			CRect rect;
// COMMENT: {5/1/2012 8:16:09 PM}			CDC* pDC = GetDC();
// COMMENT: {5/1/2012 8:16:09 PM}			int nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
// COMMENT: {5/1/2012 8:16:09 PM}			m_egBasic.GetClientRect(&rect);
// COMMENT: {5/1/2012 8:16:09 PM}			m_egBasic.SetColWidth(1, 0, MulDiv(rect.right, TWIPS_PER_INCH, nLogX) - 425 - 3);
// COMMENT: {5/1/2012 8:16:09 PM}
// COMMENT: {5/1/2012 8:16:09 PM}			// set grid
// COMMENT: {5/1/2012 8:16:09 PM}			DisplayCommands();
// COMMENT: {5/1/2012 8:16:09 PM}		}
// COMMENT: {5/1/2012 8:16:09 PM}	}
// COMMENT: {5/1/2012 8:16:09 PM}
// COMMENT: {5/1/2012 8:16:09 PM}	// validate  state
// COMMENT: {5/1/2012 8:16:09 PM}	ASSERT_VALID(this);
// COMMENT: {5/1/2012 8:16:09 PM}}

// COMMENT: {5/1/2012 8:16:34 PM}void CUserGraphPg1::DisplayCommands()
// COMMENT: {5/1/2012 8:16:34 PM}{
// COMMENT: {5/1/2012 8:16:34 PM}	ASSERT_VALID(this);
// COMMENT: {5/1/2012 8:16:34 PM}
// COMMENT: {5/1/2012 8:16:34 PM}	// set line number column width
// COMMENT: {5/1/2012 8:16:34 PM}	m_egBasic.SetColWidth(0, 0, 625);
// COMMENT: {5/1/2012 8:16:34 PM}
// COMMENT: {5/1/2012 8:16:34 PM}	// init m_egBasic rows
// COMMENT: {5/1/2012 8:16:34 PM}	long size = /*foundItem->rate.*/m_listCommands.size();
// COMMENT: {5/1/2012 8:16:34 PM}	if (m_egBasic.GetRows() < size + 5)
// COMMENT: {5/1/2012 8:16:34 PM}	{
// COMMENT: {5/1/2012 8:16:34 PM}		m_egBasic.SetRows(size + 5);
// COMMENT: {5/1/2012 8:16:34 PM}	}
// COMMENT: {5/1/2012 8:16:34 PM}
// COMMENT: {5/1/2012 8:16:34 PM}	CString str;
// COMMENT: {5/1/2012 8:16:34 PM}	long nLastLineNumber = 0;
// COMMENT: {5/1/2012 8:16:34 PM}	long nRow = 0;
// COMMENT: {5/1/2012 8:16:34 PM}
// COMMENT: {5/1/2012 8:16:34 PM}	std::list<basic_command>::iterator cmdIter = /*foundItem->rate.*/m_listCommands.begin();
// COMMENT: {5/1/2012 8:16:34 PM}	for (; cmdIter != /*foundItem->rate.*/m_listCommands.end(); ++cmdIter, ++nRow)
// COMMENT: {5/1/2012 8:16:34 PM}	{
// COMMENT: {5/1/2012 8:16:34 PM}		nLastLineNumber = cmdIter->nLine;
// COMMENT: {5/1/2012 8:16:34 PM}		str.Format(_T("%d"), cmdIter->nLine);
// COMMENT: {5/1/2012 8:16:34 PM}		m_egBasic.SetTextMatrix(nRow, 0, str);
// COMMENT: {5/1/2012 8:16:34 PM}		m_egBasic.SetTextMatrix(nRow, 1, cmdIter->strCommand);
// COMMENT: {5/1/2012 8:16:34 PM}	}
// COMMENT: {5/1/2012 8:16:34 PM}
// COMMENT: {5/1/2012 8:16:34 PM}	// determine next line number
// COMMENT: {5/1/2012 8:16:34 PM}	long nLineNumber = ((nLastLineNumber / 10) + 1) * 10;
// COMMENT: {5/1/2012 8:16:34 PM}	for (; nRow < m_egBasic.GetRows(); nRow++, nLineNumber += 10)
// COMMENT: {5/1/2012 8:16:34 PM}	{
// COMMENT: {5/1/2012 8:16:34 PM}		str.Format(_T("%d"), nLineNumber);
// COMMENT: {5/1/2012 8:16:34 PM}		m_egBasic.SetTextMatrix(nRow, 0, str);
// COMMENT: {5/1/2012 8:16:34 PM}		m_egBasic.SetTextMatrix(nRow, 1, _T(""));
// COMMENT: {5/1/2012 8:16:34 PM}	}
// COMMENT: {5/1/2012 8:16:34 PM}
// COMMENT: {5/1/2012 8:16:34 PM}	// reset grid
// COMMENT: {5/1/2012 8:16:34 PM}	m_egBasic.SetTopRow(0);
// COMMENT: {5/1/2012 8:16:34 PM}	m_egBasic.SetRow(0);
// COMMENT: {5/1/2012 8:16:34 PM}	m_egBasic.SetCol(1);
// COMMENT: {5/1/2012 8:16:34 PM}
// COMMENT: {5/1/2012 8:16:34 PM}	ASSERT_VALID(this);
// COMMENT: {5/1/2012 8:16:34 PM}}

BEGIN_EVENTSINK_MAP(CUserGraphPg1, baseUserGraphPg1)
    //{{AFX_EVENTSINK_MAP(CUserGraphPg1)
	ON_EVENT(CUserGraphPg1, IDC_MSHFG_BASIC, 71 /* EnterCell */, OnEnterCellMshfgBasic, VTS_NONE)
	ON_EVENT(CUserGraphPg1, IDC_MSHFG_BASIC, -602 /* KeyDown */, OnKeyDownMshfgBasic, VTS_PI2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CUserGraphPg1::OnEnterCellMshfgBasic()
{
	CString strRes;
	switch (m_egBasic.GetCol())
	{
	case 0 :
		strRes.LoadString(IDS_RATE_261);
		break;
	case 1 :
		AfxFormatString1(strRes, IDS_USER_PRINT_297, _T("PUNCH"));
		break;
	}
	m_eInputDesc.SetWindowText(strRes);
}

void CUserGraphPg1::OnKeyDownMshfgBasic(short FAR* KeyCode, short Shift)
{
	UNUSED(Shift);

	switch (*KeyCode)
	{
	case VK_DELETE:
		{
			CWaitCursor wait;	// display hourglass cursor
			m_egBasic.SetRedraw(FALSE);
			long nCol    = m_egBasic.GetCol();
			long nColSel = m_egBasic.GetColSel();

			// check if entire row is selected
			if ((min(nCol, nColSel) == 0) && (1 == max(nCol, nColSel)))
			{
				long nRow    = m_egBasic.GetRow();
				long nRowSel = m_egBasic.GetRowSel();

				// determine how many rows to delete
				long cSelectedRows = abs(nRow - nRowSel) + 1;

				// determine first row to delete
				long iStartRow = min(nRow, nRowSel);

				// delete each row
				for (long i = 0; i < cSelectedRows; ++i)
				{
					m_egBasic.DeleteRow(iStartRow);
				}
			}
			else
			{
				// just fill with empty strings
				m_egBasic.SetFillStyle(flexFillRepeat);
				m_egBasic.SetText(_T(""));
				m_egBasic.SetFillStyle(flexFillSingle);
			}
			m_egBasic.SetRedraw(TRUE);
		}
		break;
	}
}

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
		break;

	default:
		ASSERT(FALSE);
		break;
	}

	return bMakeChange;
}

LRESULT CUserGraphPg1::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
{
	UNUSED_ALWAYS(lParam);
	int nID = wParam;
	switch (nID)
	{
	case IDC_MSHFG_BASIC :
		OnEnterCellMshfgBasic();
		break;
	}
	return 0;
}

void CUserGraphPg1::OnSetfocusEHead() 
{
	CString strRes;
	strRes.LoadString(IDS_USER_PUNCH_299);
	m_eInputDesc.SetWindowText(strRes);
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
	case IDC_E_HEAD: case IDC_ST_HEAD:
		strRes.LoadString(IDS_USER_PUNCH_299);
		break;
	case IDC_B_RENUMBER:
		strRes.LoadString(IDS_STRING512);
		break;
	case IDC_LB_FUNCS: case IDC_ST_FUNCS:
		strRes.LoadString(IDS_RATE_258);
		break;
	case IDC_TREE_ARGS: case IDC_ST_ARGS:
		strRes.LoadString(IDS_RATE_260);
		break;
	case IDC_MSHFG_BASIC:
		if (!IsContextHelp())
		{
			if (!(m_egBasic.GetRowIsVisible(m_egBasic.GetRow()) && m_egBasic.GetColIsVisible(m_egBasic.GetCol())))
			{
				::MessageBeep((UINT)-1);
				return TRUE;
			}

			// modify placement
			CDC* pDC = m_egBasic.GetDC();
			int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
			int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

			long nLeft = ::MulDiv(m_egBasic.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
			long nTop  = ::MulDiv(m_egBasic.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

			CRect rc;
			m_egBasic.GetWindowRect(rc);

			myPopup.pt.x = rc.left + nLeft;
			myPopup.pt.y = rc.top + nTop + 10;
		}
		switch (IsContextHelp() ? m_egBasic.GetMouseCol() : m_egBasic.GetCol())
		{
		case 0 :
			strRes.LoadString(IDS_RATE_261);
			break;
		case 1 :
			AfxFormatString1(strRes, IDS_USER_PRINT_297, _T("PRINT"));
			break;
		}
		break;
	case IDC_E_EXPLAN: case IDC_ST_EXPLAN:
		strRes.LoadString(IDS_RATE_259);
		break;
	default:
		// No help topic is associated with this item. 
		strRes.LoadString(IDS_STRING441);
		break;
	}

	myPopup.pszText = strRes;
	return ::HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}

// COMMENT: {5/1/2012 8:19:25 PM}void CUserGraphPg1::OnSetfocusTreeArgs(NMHDR* pNMHDR, LRESULT* pResult) 
// COMMENT: {5/1/2012 8:19:25 PM}{
// COMMENT: {5/1/2012 8:19:25 PM}	UNUSED(pNMHDR);
// COMMENT: {5/1/2012 8:19:25 PM}	CString strRes;
// COMMENT: {5/1/2012 8:19:25 PM}	strRes.LoadString(IDS_RATE_260);
// COMMENT: {5/1/2012 8:19:25 PM}	m_eInputDesc.SetWindowText(strRes);
// COMMENT: {5/1/2012 8:19:25 PM}	
// COMMENT: {5/1/2012 8:19:25 PM}	*pResult = 0;
// COMMENT: {5/1/2012 8:19:25 PM}}

/////////////[[[[[[[[[[[[[[[[[[[
// COMMENT: {4/30/2012 3:34:45 PM}LRESULT CUserGraphPg1::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
// COMMENT: {4/30/2012 3:34:45 PM}{
// COMMENT: {4/30/2012 3:34:45 PM}	UINT nID = wParam;
// COMMENT: {4/30/2012 3:34:45 PM}	UNUSED_ALWAYS(lParam);
// COMMENT: {4/30/2012 3:34:45 PM}
// COMMENT: {4/30/2012 3:34:45 PM}	switch (nID)
// COMMENT: {4/30/2012 3:34:45 PM}	{
// COMMENT: {4/30/2012 3:34:45 PM}	case IDC_MSHFG_NUM_DESC :
// COMMENT: {4/30/2012 3:34:45 PM}		OnEnterCellMshfgNumDesc();
// COMMENT: {4/30/2012 3:34:45 PM}		break;
// COMMENT: {4/30/2012 3:34:45 PM}	case IDC_MSHFG_BASIC :
// COMMENT: {4/30/2012 3:34:45 PM}		OnEnterCellMshfgMix();
// COMMENT: {4/30/2012 3:34:45 PM}		break;
// COMMENT: {4/30/2012 3:34:45 PM}	default :
// COMMENT: {4/30/2012 3:34:45 PM}		ASSERT(FALSE);
// COMMENT: {4/30/2012 3:34:45 PM}		break;
// COMMENT: {4/30/2012 3:34:45 PM}	}
// COMMENT: {4/30/2012 3:34:45 PM}	return 0;
// COMMENT: {4/30/2012 3:34:45 PM}}

void CUserGraphPg1::OnBnClickedCheckDetach()
{
	int Ids[] = 
	{
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
