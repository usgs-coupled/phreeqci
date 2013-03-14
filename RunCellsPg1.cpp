// RunCellsPg1.cpp : implementation file
//

#include "stdafx.h"
#include "phreeqci2.h"
#include "RunCellsPg1.h"

#include "DeletePg1.h"


// CRunCellsPg1 dialog

IMPLEMENT_DYNAMIC(CRunCellsPg1, baseRunCellsPg1)

CRunCellsPg1::CRunCellsPg1()
	: baseRunCellsPg1(CRunCellsPg1::IDD)
{
	this->m_tuTimeStep = TU_SECONDS;
	this->m_tuInitTime = TU_SECONDS;
}

CRunCellsPg1::~CRunCellsPg1()
{
}

void CRunCellsPg1::DoDataExchange(CDataExchange* pDX)
{
	baseRunCellsPg1::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO_STIME, this->m_cboInitTimeUnits);
	DDX_Control(pDX, IDC_COMBO_TIMEST, this->m_cboTimeStepUnits);
	DDX_Control(pDX, IDC_E_DESC_INPUT, this->m_eInputDesc);

	runner updatedRunner;

	if (pDX->m_bSaveAndValidate)
	{
		// start time
		double st;
		DDX_Text(pDX, IDC_EDIT_STIME, st);
		updatedRunner.Set_start_time(st);

		// time step
		double ts;
		DDX_Text(pDX, IDC_EDIT_TIMEST, ts);
		updatedRunner.Set_time_step(ts);
	}
	else
	{
		// start time
		double st = this->Runner.Get_start_time();
		if (st == NA) st = 0.0;
		DDX_Text(pDX, IDC_EDIT_STIME, st);

		// time step
		double ts = this->Runner.Get_time_step();
		if (ts == NA) ts = 0.0;
		DDX_Text(pDX, IDC_EDIT_TIMEST, ts);
	}

	if (pDX->m_bSaveAndValidate)
	{
		CString nums;
		DDX_Text(pDX, IDC_EDIT_CELLS, nums);

		if (!nums.IsEmpty())
		{
			CString str("-cells ");
			str += nums;

			std::string str_in(str);
			std::istringstream iss_in;
			iss_in.str(str_in);
			std::ostringstream oss;

			PHRQ_io phrq_io;
			phrq_io.Set_error_ostream(&oss);
			CParser parser(iss_in, &phrq_io);
			updatedRunner.Read(parser);

			if (phrq_io.Get_io_error_count())
			{
				CString str(oss.str().c_str());
				int n;
				if ((n = str.Find("\nERROR: ")) == 0)
				{
					str = str.Mid(8);					
				}
				::AfxMessageBox(str);
				pDX->Fail();
			}
		}
	}
	else
	{
		CString str = CDeletePg1::GetRanges(this->Runner.Get_cells().Get_numbers());
		DDX_Text(pDX, IDC_EDIT_CELLS, str);
	}

	// time units
	if (pDX->m_bSaveAndValidate)
	{
		int n = m_cboTimeStepUnits.GetCurSel();
		this->m_tuTimeStep = (TimeUnit)n;

		n = m_cboInitTimeUnits.GetCurSel();
		this->m_tuInitTime = (TimeUnit)n;
	}
	else
	{
		this->m_cboTimeStepUnits.SetCurSel((int)m_tuTimeStep);
		this->m_cboInitTimeUnits.SetCurSel((int)m_tuInitTime);
	}

	if (pDX->m_bSaveAndValidate)
	{
		this->Runner = updatedRunner;
	}
}


BEGIN_MESSAGE_MAP(CRunCellsPg1, baseRunCellsPg1)
	ON_EN_SETFOCUS(IDC_EDIT_CELLS, &CRunCellsPg1::OnEnSetfocusEditCells)
	ON_EN_SETFOCUS(IDC_EDIT_STIME, &CRunCellsPg1::OnEnSetfocusEditStime)
	ON_EN_SETFOCUS(IDC_EDIT_TIMEST, &CRunCellsPg1::OnEnSetfocusEditTimest)
	ON_CBN_SETFOCUS(IDC_COMBO_STIME, &CRunCellsPg1::OnCbnSetfocusComboStime)
	ON_CBN_SETFOCUS(IDC_COMBO_TIMEST, &CRunCellsPg1::OnCbnSetfocusComboTimest)
	ON_WM_HELPINFO()
END_MESSAGE_MAP()


// CRunCellsPg1 message handlers

BOOL CRunCellsPg1::OnInitDialog()
{
	baseRunCellsPg1::OnInitDialog();

	// set layout
	CreateRoot(VERTICAL, 5, 6)
		<< (pane(HORIZONTAL, ABSOLUTE_VERT, 0, 0, 0)
			<< item(IDC_STATIC_CELLS, NORESIZE)
			<< itemFixed(HORIZONTAL, 5)
			<< item(IDC_EDIT_CELLS, ABSOLUTE_VERT|ALIGN_VCENTER)
			<< itemFixed(HORIZONTAL, 5)
			)
		<< itemFixed(VERTICAL, 1)
		<< (pane(HORIZONTAL, ABSOLUTE_VERT, 0, 0, 0)
			<< item(IDC_STATIC_ST, NORESIZE|ALIGN_VCENTER)
			<< itemFixed(HORIZONTAL, 5)
			<< item(IDC_EDIT_STIME, ABSOLUTE_VERT|ALIGN_VCENTER)
			<< itemFixed(HORIZONTAL, 5)
			<< item(IDC_COMBO_STIME, NORESIZE)
			)
		<< itemFixed(VERTICAL, 1)
		<< (pane(HORIZONTAL, ABSOLUTE_VERT, 0, 0, 0)
			<< item(IDC_STATIC_TS, NORESIZE|ALIGN_VCENTER)
			<< itemFixed(HORIZONTAL, 5)
			<< item(IDC_EDIT_TIMEST, ABSOLUTE_VERT|ALIGN_VCENTER)
			<< itemFixed(HORIZONTAL, 5)
			<< item(IDC_COMBO_TIMEST, NORESIZE)
			)
		<< itemFixed(VERTICAL, 1)
		<< ( paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, GREEDY, nDefaultBorder, 10, 10)
			<< item(IDC_E_DESC_INPUT, GREEDY)
		);
	UpdateLayout();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRunCellsPg1::OnEnSetfocusEditCells()
{
	CString strRes;
	strRes.LoadString(IDS_STRING742);
	m_eInputDesc.SetWindowText(strRes);	
}

void CRunCellsPg1::OnEnSetfocusEditStime()
{
	CString strRes;
	strRes.LoadString(IDS_STRING743);
	m_eInputDesc.SetWindowText(strRes);	
}

void CRunCellsPg1::OnEnSetfocusEditTimest()
{
	CString strRes;
	strRes.LoadString(IDS_STRING745);
	m_eInputDesc.SetWindowText(strRes);	
}

void CRunCellsPg1::OnCbnSetfocusComboStime()
{
	CString strRes;
	strRes.LoadString(IDS_STRING744);
	m_eInputDesc.SetWindowText(strRes);	
}

void CRunCellsPg1::OnCbnSetfocusComboTimest()
{
	CString strRes;
	strRes.LoadString(IDS_STRING744);
	m_eInputDesc.SetWindowText(strRes);	
}

BOOL CRunCellsPg1::OnHelpInfo(HELPINFO* pHelpInfo)
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
	case IDC_STATIC_CELLS: case IDC_EDIT_CELLS:
		strRes.LoadString(IDS_STRING742);
		break;
	case IDC_STATIC_ST: case IDC_EDIT_STIME:
		strRes.LoadString(IDS_STRING743);
		break;
	case IDC_COMBO_STIME: case IDC_COMBO_TIMEST:
		strRes.LoadString(IDS_STRING744);
		break;
	case IDC_STATIC_TS: case IDC_EDIT_TIMEST:
		strRes.LoadString(IDS_STRING745);
		break;
	default:
		// No help topic is associated with this item. 
		strRes.LoadString(IDS_STRING441);
		// return baseCKPAdvectionPg1::OnHelpInfo(pHelpInfo);
	}
	myPopup.pszText = strRes;
	return ::HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}
