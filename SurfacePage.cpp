// SurfacePage.cpp : implementation file
//

#include "stdafx.h"
#include "phreeqci2.h"
#include "SurfacePage.h"

#include "SurfaceSheet.h"

// CSurfacePage dialog

IMPLEMENT_DYNAMIC(CSurfacePage, baseCSurfacePage)

CSurfacePage::CSurfacePage()
: baseCSurfacePage(CSurfacePage::IDD)
, GridListDocGen(&GridSurfTypes, &ListCtrlSurface, 1)
, GridListDocEqu(&GridSurfTypes, &ListCtrlSurface, 1)
, GridListDocKin(&GridSurfTypes, &ListCtrlSurface, 1)
, n_solution(CSurfacePage::N_NONE)
, DT(CSurfacePage::DT_THICKNESS)
, thickness(1e-8)
, debye_lengths(1.0)
, DDL_limit(0.8)
, DDL_viscosity(1.0)
, only_counter_ions(FALSE)
, type(DDL)
, dl_type(NO_DL)
, transport(FALSE)
{
	// Note: cannot use this->GetSheet() in here
// COMMENT: {12/29/2009 2:57:29 PM}	this->SolutionNumberFormat = CUtil::CreateRange(this->SetSolutions, this->GetSheet()->DefinedRanges[CKeyword::K_SOLUTION]);
}

CSurfacePage::~CSurfacePage()
{
}

void CSurfacePage::DoDataExchange(CDataExchange* pDX)
{
	baseCSurfacePage::DoDataExchange(pDX);

	// controls
	DDX_Control(pDX, IDC_TAB_SURF_TYPE,     this->TabCtrlSurfType);
	DDX_Control(pDX, IDC_CL_SURFACE,        this->ListCtrlSurface);
	DDX_Control(pDX, IDC_MSHFG_SURFTYPE,    this->GridSurfTypes);
	DDX_Control(pDX, IDC_MSHFG_SURFACES,    this->GridSurfaces);
	DDX_Control(pDX, IDC_CHECK_EQUILIBRATE, this->CheckBoxEquilibrate);
	DDX_Control(pDX, IDC_CBO_SOLUTIONS,     this->ComboSolutions);
	DDX_Control(pDX, IDC_LV_DEFINED,        this->ListCtrlDefined);

// COMMENT: {12/30/2009 4:50:16 PM}	if (!pDX->m_bSaveAndValidate)
// COMMENT: {12/30/2009 4:50:16 PM}	{
// COMMENT: {12/30/2009 4:50:16 PM}		ASSERT(GridListDoc.GetCount() == 0);
// COMMENT: {12/30/2009 4:50:16 PM}	}

	if (m_bFirstSetActive)
	{
		this->InitComboSolutions();
		if (this->TabCtrlSurfType.GetItemCount() == 0)
		{
			this->TabCtrlSurfType.InsertItem(0, _T("&General"));
			this->TabCtrlSurfType.InsertItem(1, _T("&Equilibrium phases (Advanced)"));
			this->TabCtrlSurfType.InsertItem(2, _T("&Kinetic reactants (Advanced)"));
		}
		if (this->ListCtrlDefined.GetHeaderCtrl()->GetItemCount() == 0)
		{
			this->ListCtrlDefined.InsertColumn(0, _T("Surface"));
			this->ListCtrlDefined.InsertColumn(1, _T("Type"));
// COMMENT: {1/7/2010 8:23:39 PM}			this->ListCtrlDefined.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
// COMMENT: {1/7/2010 8:23:39 PM}			this->ListCtrlDefined.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
		}

		this->InitCLC();
		this->InitEGSurfTypes();
		this->InitEGSurfaces();
		this->UpdateListCtrlDefined();
		if (this->ListCtrlDefined.GetItemCount())
		{
			this->ListCtrlDefined.SetColumnWidth(0, LVSCW_AUTOSIZE);
		}
		else
		{
			this->ListCtrlDefined.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
		}
		this->ListCtrlDefined.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
	}

	if (pDX->m_bSaveAndValidate)
	{
		this->ValidateComboSolutions();
		this->ValidateSCM(pDX);
		this->ValidateEGSurfaces(pDX);
		this->ValidateEGSurfTypes(pDX);
	}
	else
	{
		// If any other pages might modify
		// any of the variables set through these exchanges
		// they should be handled in this spot
		// otherwise place in OnInitDialog
		this->ExchangeComboSolutions();
		this->ExchangeSCM(pDX);
		this->ExchangeEG_CLC(pDX);
	}
}


BEGIN_MESSAGE_MAP(CSurfacePage, baseCSurfacePage)
	// custom grid notifications
	ON_MESSAGE(EGN_SETFOCUS, &CSurfacePage::OnSetfocusEG)
	ON_MESSAGE(EGN_BEGINCELLEDIT, &CSurfacePage::OnBeginCellEdit)
	ON_MESSAGE(EGN_ENDCELLEDIT, &CSurfacePage::OnEndCellEdit)

	// check boxes
	ON_BN_CLICKED(IDC_CHECK_EQUILIBRATE, &CSurfacePage::OnBnClickedCheckEquilibrate)
	ON_BN_CLICKED(IDC_CHECK_RETARD2, &CSurfacePage::OnBnClickedCheckRetard2)

	// radio buttons (SCM)
	ON_BN_CLICKED(IDC_RADIO_NO_EDL2, &CSurfacePage::OnBnClickedRadioNoEdl2)
	ON_BN_CLICKED(IDC_RADIO_DDL2, &CSurfacePage::OnBnClickedRadioDdl2)
	ON_BN_CLICKED(IDC_RADIO_CD_MUSIC2, &CSurfacePage::OnBnClickedRadioCdMusic2)

	// radio buttons (DLO)
	ON_BN_CLICKED(IDC_RADIO_DL_TYPE_N, &CSurfacePage::OnBnClickedRadioDlTypeN)
	ON_BN_CLICKED(IDC_RADIO_DL_TYPE_D, &CSurfacePage::OnBnClickedRadioDlTypeD)
	ON_BN_CLICKED(IDC_RADIO_DL_TYPE_B, &CSurfacePage::OnBnClickedRadioDlTypeB)

	// radio buttons (thickness or debye lengths)
	ON_BN_CLICKED(IDC_RADIO_THICK, &CSurfacePage::OnBnClickedRadioThick)
	ON_BN_CLICKED(IDC_RADIO_DEBYE, &CSurfacePage::OnBnClickedRadioDebye)

	// solutions combobox (IDC_CBO_SOLUTIONS)
	ON_CBN_SETFOCUS(IDC_CBO_SOLUTIONS, &CSurfacePage::OnCbnSetfocusCboSolutions)

	// SysTabControl32 messages
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB_SURF_TYPE, &CSurfacePage::OnTcnSelchangingTabSurfType)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_SURF_TYPE, &CSurfacePage::OnTcnSelchangeTabSurfType)

	// SysListView32 messages
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CL_SURFACE, &CSurfacePage::OnLvnItemchangedClSurface)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_CHECK_SITE_DENSITY, &CSurfacePage::OnBnClickedCheckSiteDensity)
	ON_BN_SETFOCUS(IDC_CHECK_EQUILIBRATE, &CSurfacePage::OnBnSetfocusCheckEquilibrate)
	ON_BN_SETFOCUS(IDC_CHECK_DONNAN_CIO, &CSurfacePage::OnBnSetfocusCheckDonnanCio)
	ON_BN_SETFOCUS(IDC_CHECK_BORK_CIO, &CSurfacePage::OnBnSetfocusCheckBorkCio)
	ON_NOTIFY(NM_SETFOCUS, IDC_CL_SURFACE, &CSurfacePage::OnNMSetfocusClSurface)
	ON_EN_SETFOCUS(IDC_EDIT_DONNAN_THICKNESS, &CSurfacePage::OnEnSetfocusEditDonnanThickness)
	ON_BN_CLICKED(IDC_CHECK_MOBILE, &CSurfacePage::OnBnClickedCheckMobile)
	ON_BN_SETFOCUS(IDC_RADIO_NO_EDL2, &CSurfacePage::OnBnSetfocusRadioNoEdl2)
	ON_BN_SETFOCUS(IDC_RADIO_DDL2, &CSurfacePage::OnBnSetfocusRadioDdl2)
	ON_BN_SETFOCUS(IDC_RADIO_CD_MUSIC2, &CSurfacePage::OnBnSetfocusRadioCdMusic2)
	ON_BN_SETFOCUS(IDC_RADIO_DL_TYPE_D, &CSurfacePage::OnBnSetfocusRadioDlTypeD)
	ON_BN_SETFOCUS(IDC_RADIO_THICK, &CSurfacePage::OnBnSetfocusRadioThick)
	ON_BN_SETFOCUS(IDC_RADIO_DEBYE, &CSurfacePage::OnBnSetfocusRadioDebye)
	ON_EN_SETFOCUS(IDC_EDIT_DEBYE_LENGTH, &CSurfacePage::OnEnSetfocusEditDebyeLength)
	ON_EN_SETFOCUS(IDC_EDIT_DEB_LEN_LIMIT, &CSurfacePage::OnEnSetfocusEditDebLenLimit)
	ON_EN_SETFOCUS(IDC_EDIT_VISCOSITY, &CSurfacePage::OnEnSetfocusEditViscosity)
	ON_BN_SETFOCUS(IDC_CHECK_RETARD2, &CSurfacePage::OnBnSetfocusCheckRetard2)
	ON_WM_HELPINFO()
	ON_BN_SETFOCUS(IDC_CHECK_SITE_DENSITY, &CSurfacePage::OnBnSetfocusCheckSiteDensity)
	ON_BN_SETFOCUS(IDC_CHECK_MOBILE, &CSurfacePage::OnBnSetfocusCheckMobile)
	ON_EN_SETFOCUS(IDC_EDIT_BORK_THICKNESS, &CSurfacePage::OnEnSetfocusEditBorkThickness)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CSurfacePage, CCommonKeywordPage)
	ON_EVENT(CSurfacePage, IDC_MSHFG_SURFTYPE, 71, CSurfacePage::EnterCellMshfgSurftype, VTS_NONE)
	ON_EVENT(CSurfacePage, IDC_MSHFG_SURFACES, 71, CSurfacePage::EnterCellMshfgSurfaces, VTS_NONE)
	ON_EVENT(CSurfacePage, IDC_MSHFG_NUM_DESC, 71, CSurfacePage::EnterCellMshfgNumDesc, VTS_NONE)
	ON_EVENT(CSurfacePage, IDC_MSHFG_SURFTYPE, DISPID_KEYDOWN, CSurfacePage::KeyDownMshfgSurftype, VTS_PI2 VTS_I2)
	ON_EVENT(CSurfacePage, IDC_MSHFG_SURFACES, DISPID_KEYDOWN, CSurfacePage::KeyDownMshfgSurfaces, VTS_PI2 VTS_I2)
END_EVENTSINK_MAP()

// CSurfacePage message handlers

#ifdef _DEBUG
CSurfaceSheet* CSurfacePage::GetSheet() // non-debug version is inline
{
	CSurfaceSheet* pSheet = (CSurfaceSheet*)GetParent();
	ASSERT_KINDOF(CSurfaceSheet, pSheet);
	return pSheet;
}
#endif

void CSurfacePage::OnBnClickedCheckEquilibrate()
{
	int nState = this->CheckBoxEquilibrate.GetCheck();

	if (this->TabCtrlSurfType.GetCurSel() == TI_GENERAL)
	{
		if (this->GridListDocGen.GetCount() > 0)
		{
			if (::AfxMessageBox(IDS_SURFACE_302, MB_YESNO) == IDNO)
			{
				nState = (nState == BST_CHECKED) ? BST_UNCHECKED : BST_CHECKED;
				this->CheckBoxEquilibrate.SetCheck(nState);
				return;
			}
			else
			{
				this->GridListDocGen.DeleteAllRows();
				this->GridSurfaces.ClearWorkSpace();
				for (long row = this->GridSurfaces.GetFixedRows(); row < this->GridSurfaces.GetRows(); ++row)
				{
					this->GridSurfaces.SetTextMatrix(row, 0, _T(""));
				}
			}
		}
	}
	else
	{
		if (this->ListSurfComp[TI_GENERAL].size() > 0)
		{
			if (::AfxMessageBox(IDS_SURFACE_301, MB_YESNO) == IDNO)
			{
				nState = (nState == BST_CHECKED) ? BST_UNCHECKED : BST_CHECKED;
				this->CheckBoxEquilibrate.SetCheck(nState);
				return;
			}
			else
			{
				this->ListSurfComp[TI_GENERAL].clear();
			}
		}
	}

	int nItem;

	switch (nState)
	{
	case BST_CHECKED :
		{
			this->ComboSolutions.EnableWindow(TRUE);
			if (this->TabCtrlSurfType.GetCurSel() == TI_GENERAL)
			{
				this->ListCtrlSurface.DeleteAllItems();
				std::set<CDBElement>::const_iterator elemIter = GetDatabase().m_elementSet.begin();
				for ( ; elemIter != GetDatabase().m_elementSet.end(); ++elemIter)
				{
					if ( (*elemIter).m_type == CDBElement::typeSURF )
					{
						if ((nItem = this->ListCtrlSurface.InsertItem(0, (*elemIter).m_strName)) != -1)
						{
							// add to columns here
						}
					}
				}
			}
		}
		break;

	case BST_UNCHECKED :
		{
			this->ComboSolutions.EnableWindow(FALSE);
			if (this->TabCtrlSurfType.GetCurSel() == TI_GENERAL)
			{
				this->ListCtrlSurface.DeleteAllItems();
				std::set<CDBElement>::const_iterator elemIter = GetDatabase().m_elementSet.begin();
				for ( ; elemIter != GetDatabase().m_elementSet.end(); ++elemIter)
				{
					if ( (*elemIter).m_type == CDBElement::typeSURF )
					{
						if ((nItem = this->ListCtrlSurface.InsertItem(0, (*elemIter).m_strMaster)) != -1)
						{
							// add to columns here
						}
					}
				}
			}
		}
		break;

	default :
		ASSERT( FALSE );
		break;
	}	
}

BOOL CSurfacePage::InitComboSolutions()
{
#ifdef _DEBUG
	CString str;
	this->ComboSolutions.GetWindowText(str);
	ASSERT(str.IsEmpty());
	ASSERT(this->ComboSolutions.GetCount() == 0);
#endif

	CUtil::InsertRange(this->ComboSolutions.m_hWnd, this->GetSheet()->SetSolutions);
	return TRUE;
}

void CSurfacePage::ExchangeComboSolutions()
{
	if (this->n_solution != CSurfacePage::N_NONE)
	{
		this->ComboSolutions.EnableWindow(TRUE);
		ASSERT( this->n_solution >= 0 );

		if (this->GetSheet()->SolutionNumberFormat.IsEmpty())
		{
			this->GetSheet()->SolutionNumberFormat = _T("%d");
		}

		CString strSolNum;
		strSolNum.Format(this->GetSheet()->SolutionNumberFormat, this->n_solution);

		int nFind = this->ComboSolutions.SelectString(-1, strSolNum);
		if (nFind == -1)
		{
			this->ComboSolutions.SetWindowText(strSolNum);
		}
		this->CheckBoxEquilibrate.SetCheck(BST_CHECKED);
	}
	else 
	{
		this->ComboSolutions.EnableWindow(FALSE);
		this->CheckBoxEquilibrate.SetCheck(BST_UNCHECKED);
	}

	this->OnBnClickedCheckEquilibrate();
}

void CSurfacePage::ValidateComboSolutions()
{
	int nState = this->IsDlgButtonChecked(IDC_CHECK_EQUILIBRATE);

	if (nState == BST_UNCHECKED)
	{
		this->n_solution = CSurfacePage::N_NONE;
		return;
	}

	CString strText;
	this->ComboSolutions.GetWindowText(strText);
	strText.TrimLeft();
	char* pEnd;

	int nCurSel = this->ComboSolutions.GetCurSel();
	if (nCurSel != -1)
	{
		this->n_solution = strtol(strText, &pEnd, 10);
	}
	else
	{
		if (strText.IsEmpty())
		{
			this->n_solution = CSurfacePage::N_NONE;
			CString str;
			str = _T("Equilibration solution must be a positive integer.");
			this->OnCbnSetfocusCboSolutions();
			this->ComboSolutions.SetFocus();
			MessageBox(str, "Missing solution number", MB_OK);
			AfxThrowUserException();
		}
		else
		{
			this->n_solution = strtol(strText, &pEnd, 10);
			if (*pEnd != 0
				||
				this->n_solution < 0 
				||
				(this->n_solution == 0 && *pEnd != 0))
			{
				CString str;
				str = _T("Equilibration solution must be a positive integer.");
				this->OnCbnSetfocusCboSolutions();
				this->ComboSolutions.SetFocus();
				MessageBox(str, "Invalid solution number", MB_OK);
				AfxThrowUserException();
			}
		}
	}	

}

void CSurfacePage::ExchangeSCM(CDataExchange* pDX)
{
	// thickness
	//
	static int thicknessIds[] = 
	{
		IDC_EDIT_DONNAN_THICKNESS,
		IDC_EDIT_BORK_THICKNESS
	};
	for (int i = 0; i < sizeof(thicknessIds) / sizeof(thicknessIds[0]); ++i)
	{
		::DDX_Text(pDX, thicknessIds[i], this->thickness);
	}

	// debye_lengths & DDL_limit
	//
	::DDX_Text(pDX, IDC_EDIT_DEBYE_LENGTH,  this->debye_lengths);
	::DDX_Text(pDX, IDC_EDIT_DEB_LEN_LIMIT, this->DDL_limit);

	switch (this->DT)
	{
	case CSurfacePage::DT_THICKNESS:
		this->CheckRadioButton(IDC_RADIO_THICK, IDC_RADIO_DEBYE, IDC_RADIO_THICK);
		break;
	case CSurfacePage::DT_DEBYE_LENGTHS:
		this->CheckRadioButton(IDC_RADIO_THICK, IDC_RADIO_DEBYE, IDC_RADIO_DEBYE);
		break;
	default:
		ASSERT(FALSE);
		this->CheckRadioButton(IDC_RADIO_THICK, IDC_RADIO_DEBYE, IDC_RADIO_THICK);
		break;
	}

	// viscosity
	//
	::DDX_Text(pDX, IDC_EDIT_VISCOSITY, this->DDL_viscosity);
	if (this->DDL_viscosity != 1.)
	{
		this->CheckDlgButton(IDC_CHECK_RETARD2, BST_CHECKED);
	}

	// counter ions only
	//
	static int onlyCounterIonsIds[] = 
	{
		IDC_CHECK_DONNAN_CIO,
		IDC_CHECK_BORK_CIO,
	};
	for (int i = 0; i < sizeof(onlyCounterIonsIds) / sizeof(onlyCounterIonsIds[0]); ++i)
	{
		this->CheckDlgButton(onlyCounterIonsIds[i], this->only_counter_ions ? BST_CHECKED : BST_UNCHECKED);
	}

	// type
	//
	if (this->type == NO_EDL)
	{
		this->CheckRadioButton(IDC_RADIO_NO_EDL2, IDC_RADIO_CD_MUSIC2, IDC_RADIO_NO_EDL2);
		this->CheckRadioButton(IDC_RADIO_DL_TYPE_N, IDC_RADIO_DL_TYPE_B, IDC_RADIO_DL_TYPE_N);
	}
	else if (this->type == DDL)
	{
		this->CheckRadioButton(IDC_RADIO_NO_EDL2, IDC_RADIO_CD_MUSIC2, IDC_RADIO_DDL2);

		switch (this->dl_type)
		{
		case NO_DL:
			this->CheckRadioButton(IDC_RADIO_DL_TYPE_N, IDC_RADIO_DL_TYPE_B, IDC_RADIO_DL_TYPE_N);
			break;

		case BORKOVEK_DL:
			this->CheckRadioButton(IDC_RADIO_DL_TYPE_N, IDC_RADIO_DL_TYPE_B, IDC_RADIO_DL_TYPE_B);
			break;

		case DONNAN_DL:
			this->CheckRadioButton(IDC_RADIO_DL_TYPE_N, IDC_RADIO_DL_TYPE_B, IDC_RADIO_DL_TYPE_D);
			break;

		default:
			ASSERT(FALSE);
			break;
		}
	}
	else if (this->type == CD_MUSIC)
	{
		this->CheckRadioButton(IDC_RADIO_NO_EDL2, IDC_RADIO_CD_MUSIC2, IDC_RADIO_CD_MUSIC2);
		switch(this->dl_type)
		{
		case NO_DL:
			this->CheckRadioButton(IDC_RADIO_DL_TYPE_N, IDC_RADIO_DL_TYPE_B, IDC_RADIO_DL_TYPE_N);
			break;

		case BORKOVEK_DL:
			ASSERT(FALSE);
			break;

		case DONNAN_DL:
			this->CheckRadioButton(IDC_RADIO_DL_TYPE_N, IDC_RADIO_DL_TYPE_B, IDC_RADIO_DL_TYPE_D);
			break;

		default:
			ASSERT(FALSE);
			break;
		}
	}
	this->UpdateRadioSCM();
}

void CSurfacePage::ValidateSCM(CDataExchange* pDX)
{
	// init
	//
	int thicknessID = 0;
	int onlyCounterIonsID = 0;
	int viscosityID = 0;

	this->dl_type = NO_DL;
	this->only_counter_ions = FALSE;
	this->DDL_viscosity = 1.0;

	//{{
	// type
	//
	switch(this->GetCheckedRadioButton(IDC_RADIO_NO_EDL2, IDC_RADIO_CD_MUSIC2))
	{
	case IDC_RADIO_NO_EDL2:
		this->type = NO_EDL;
		break;
	case IDC_RADIO_DDL2:
		this->type = DDL;
		break;
	case IDC_RADIO_CD_MUSIC2:
		this->type = CD_MUSIC;
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	// dl_type
	//
	if (this->type != NO_EDL)
	{
		switch(this->GetCheckedRadioButton(IDC_RADIO_DL_TYPE_N, IDC_RADIO_DL_TYPE_B))
		{
		case IDC_RADIO_DL_TYPE_N:
			this->dl_type = NO_DL;
			break;
		case IDC_RADIO_DL_TYPE_D:
			this->dl_type = DONNAN_DL;
			break;
		case IDC_RADIO_DL_TYPE_B:
			this->dl_type = BORKOVEK_DL;
			ASSERT(this->type != CD_MUSIC);
			break;
		default:
			ASSERT(FALSE);
			break;
		}
	}
	//}}


// COMMENT: {12/29/2009 5:05:06 PM}	// type
// COMMENT: {12/29/2009 5:05:06 PM}	//
// COMMENT: {12/29/2009 5:05:06 PM}	switch(this->GetCheckedRadioButton(IDC_RADIO_NO_EDL2, IDC_RADIO_CD_MUSIC2))
// COMMENT: {12/29/2009 5:05:06 PM}	{
// COMMENT: {12/29/2009 5:05:06 PM}	case IDC_RADIO_NO_EDL2:
// COMMENT: {12/29/2009 5:05:06 PM}		this->type = NO_EDL;
// COMMENT: {12/29/2009 5:05:06 PM}		break;
// COMMENT: {12/29/2009 5:05:06 PM}
// COMMENT: {12/29/2009 5:05:06 PM}	case IDC_RADIO_DDL2:
// COMMENT: {12/29/2009 5:05:06 PM}		this->type = DDL;
// COMMENT: {12/29/2009 5:05:06 PM}		switch(this->GetCheckedRadioButton(IDC_RADIO_DL_TYPE_N, IDC_RADIO_DL_TYPE_B))
// COMMENT: {12/29/2009 5:05:06 PM}		{
// COMMENT: {12/29/2009 5:05:06 PM}		case IDC_RADIO_DL_TYPE_N:
// COMMENT: {12/29/2009 5:05:06 PM}			this->dl_type = NO_DL;
// COMMENT: {12/29/2009 5:05:06 PM}			break;
// COMMENT: {12/29/2009 5:05:06 PM}		case IDC_RADIO_DL_TYPE_D:
// COMMENT: {12/29/2009 5:05:06 PM}			this->dl_type = DONNAN_DL;
// COMMENT: {12/29/2009 5:05:06 PM}			viscosityID = IDC_EDIT_VISCOSITY;
// COMMENT: {12/29/2009 5:05:06 PM}			onlyCounterIonsID = IDC_CHECK_DONNAN_CIO;
// COMMENT: {12/29/2009 5:05:06 PM}			break;
// COMMENT: {12/29/2009 5:05:06 PM}		case IDC_RADIO_DL_TYPE_B:
// COMMENT: {12/29/2009 5:05:06 PM}			this->dl_type = BORKOVEK_DL;
// COMMENT: {12/29/2009 5:05:06 PM}			thicknessID = IDC_EDIT_DONNAN_THICKNESS;
// COMMENT: {12/29/2009 5:05:06 PM}			break;
// COMMENT: {12/29/2009 5:05:06 PM}		default:
// COMMENT: {12/29/2009 5:05:06 PM}			ASSERT(FALSE);
// COMMENT: {12/29/2009 5:05:06 PM}			break;
// COMMENT: {12/29/2009 5:05:06 PM}		}
// COMMENT: {12/29/2009 5:05:06 PM}		break;
// COMMENT: {12/29/2009 5:05:06 PM}
// COMMENT: {12/29/2009 5:05:06 PM}	case IDC_RADIO_CD_MUSIC2:
// COMMENT: {12/29/2009 5:05:06 PM}		this->type = CD_MUSIC;
// COMMENT: {12/29/2009 5:05:06 PM}		switch(this->GetCheckedRadioButton(IDC_RADIO_DL_TYPE_N, IDC_RADIO_DL_TYPE_B))
// COMMENT: {12/29/2009 5:05:06 PM}		{
// COMMENT: {12/29/2009 5:05:06 PM}		case IDC_RADIO_DL_TYPE_N:
// COMMENT: {12/29/2009 5:05:06 PM}			this->dl_type = NO_DL;
// COMMENT: {12/29/2009 5:05:06 PM}			break;
// COMMENT: {12/29/2009 5:05:06 PM}		case IDC_RADIO_DL_TYPE_D:
// COMMENT: {12/29/2009 5:05:06 PM}			this->dl_type = DONNAN_DL;
// COMMENT: {12/29/2009 5:05:06 PM}			viscosityID = IDC_EDIT_VISCOSITY;
// COMMENT: {12/29/2009 5:05:06 PM}			onlyCounterIonsID = IDC_CHECK_DONNAN_CIO;
// COMMENT: {12/29/2009 5:05:06 PM}			break;
// COMMENT: {12/29/2009 5:05:06 PM}		case IDC_RADIO_DL_TYPE_B:
// COMMENT: {12/29/2009 5:05:06 PM}			ASSERT(FALSE); // No Borkovec for CD_MUSIC
// COMMENT: {12/29/2009 5:05:06 PM}			this->dl_type = NO_DL;
// COMMENT: {12/29/2009 5:05:06 PM}			break;
// COMMENT: {12/29/2009 5:05:06 PM}		default:
// COMMENT: {12/29/2009 5:05:06 PM}			ASSERT(FALSE);
// COMMENT: {12/29/2009 5:05:06 PM}			this->dl_type = NO_DL;
// COMMENT: {12/29/2009 5:05:06 PM}			break;
// COMMENT: {12/29/2009 5:05:06 PM}		}
// COMMENT: {12/29/2009 5:05:06 PM}		break;
// COMMENT: {12/29/2009 5:05:06 PM}
// COMMENT: {12/29/2009 5:05:06 PM}	default:
// COMMENT: {12/29/2009 5:05:06 PM}		ASSERT(FALSE);
// COMMENT: {12/29/2009 5:05:06 PM}		break;
// COMMENT: {12/29/2009 5:05:06 PM}	}

	//{{
	// dl_type
	//
	if (this->dl_type == DONNAN_DL)
	{
		switch(this->GetCheckedRadioButton(IDC_RADIO_THICK, IDC_RADIO_DEBYE))
		{
		case IDC_RADIO_THICK:
			this->DT = DT_THICKNESS;
			thicknessID = IDC_EDIT_DONNAN_THICKNESS;
			break;
		case IDC_RADIO_DEBYE:
			this->DT = DT_DEBYE_LENGTHS;
			::DDX_Text(pDX, IDC_EDIT_DEBYE_LENGTH, this->debye_lengths);
			::DDX_Text(pDX, IDC_EDIT_DEB_LEN_LIMIT, this->DDL_limit);
			break;
		default:
			ASSERT(FALSE);
			break;
		}
		onlyCounterIonsID = IDC_CHECK_DONNAN_CIO;
		viscosityID       = IDC_EDIT_VISCOSITY;
	}
	else if (this->dl_type == BORKOVEK_DL)
	{
		thicknessID       = IDC_EDIT_BORK_THICKNESS;
		onlyCounterIonsID = IDC_CHECK_BORK_CIO;
	}
	//}}

	// thickness
	//
	if (thicknessID)
	{
		CString str;
		::DDX_Text(pDX, thicknessID, str);
		if (str.IsEmpty())
		{
			this->MessageBox("Expected thickness of diffuse layer.", "Invalid thickness", MB_OK);
			pDX->Fail();
		}
		::DDX_Text(pDX, thicknessID, this->thickness);
	}

	// counter only
	//
	if (onlyCounterIonsID)
	{
		if (CButton* pCounterOnly = (CButton*)this->GetDlgItem(onlyCounterIonsID))
		{
			if (pCounterOnly->GetCheck() == BST_CHECKED)
			{
				this->only_counter_ions = TRUE;
			}
		}
	}

	// viscosity (Retard diffusion in diffuse layer)
	//
	bool bRetard = (this->IsDlgButtonChecked(IDC_CHECK_RETARD2) == BST_CHECKED);
	if (bRetard && viscosityID)
	{
		ASSERT(this->dl_type == DONNAN_DL);
		CString str;
		::DDX_Text(pDX, viscosityID, str);
		if (str.IsEmpty())
		{
			this->MessageBox("Expected viscosity of diffuse layer.", "Invalid viscosity", MB_OK);
			pDX->Fail();
		}
		::DDX_Text(pDX, viscosityID, this->DDL_viscosity);
	}

}

void CSurfacePage::UpdateRadioSCM()
{
	// Update Surface complexation model
	//

	BOOL bElectrostatics = TRUE;
	BOOL bCD_MUSIC = FALSE;

	switch (this->GetCheckedRadioButton(IDC_RADIO_NO_EDL2, IDC_RADIO_CD_MUSIC2))
	{
	case IDC_RADIO_NO_EDL2:
		bElectrostatics = FALSE;
		break;
	case IDC_RADIO_DDL2:
		break;
	case IDC_RADIO_CD_MUSIC2:
		bCD_MUSIC = TRUE;
		break;
	default:
		break;
	}

	// No explicit diffuse layer
	if (CWnd* pWnd = this->GetDlgItem(IDC_RADIO_DL_TYPE_N))
	{
		pWnd->EnableWindow(bElectrostatics);
	}

	// Donnan diffuse layer
	if (CWnd* pWnd = this->GetDlgItem(IDC_RADIO_DL_TYPE_D))
	{
		pWnd->EnableWindow(bElectrostatics);
	}

	// Borkovec diffuse layer
	if (CWnd* pWnd = this->GetDlgItem(IDC_RADIO_DL_TYPE_B))
	{
		pWnd->EnableWindow(bElectrostatics && !bCD_MUSIC);
	}
	if (bCD_MUSIC)
	{
		if (this->GetCheckedRadioButton(IDC_RADIO_DL_TYPE_N, IDC_RADIO_DL_TYPE_B) == IDC_RADIO_DL_TYPE_B)
		{
			this->CheckRadioButton(IDC_RADIO_DL_TYPE_N, IDC_RADIO_DL_TYPE_B, IDC_RADIO_DL_TYPE_D);
		}
	}

	this->UpdateRadioDLO();
}

void CSurfacePage::UpdateRadioDLO()
{
	// Update Diffuse layer options
	//

	BOOL bEnableNEDL = FALSE;
	BOOL bEnableDonn = FALSE;
	BOOL bEnableBork = FALSE;

	if (this->GetCheckedRadioButton(IDC_RADIO_NO_EDL2, IDC_RADIO_CD_MUSIC2) != IDC_RADIO_NO_EDL2)
	{
		switch (this->GetCheckedRadioButton(IDC_RADIO_DL_TYPE_N, IDC_RADIO_DL_TYPE_B))
		{
		case IDC_RADIO_DL_TYPE_N:
			bEnableNEDL = TRUE;
			break;
		case IDC_RADIO_DL_TYPE_D:
			bEnableDonn = TRUE;
			break;
		case IDC_RADIO_DL_TYPE_B:
			bEnableBork = TRUE;
			break;
		default:
			ASSERT(FALSE);
			break;
		}
	}

	// Donnan diffuse layer
	if (CWnd* pWnd = this->GetDlgItem(IDC_RADIO_THICK))
	{
		pWnd->EnableWindow(bEnableDonn);
	}
	if (CWnd* pWnd = this->GetDlgItem(IDC_RADIO_DEBYE))
	{
		pWnd->EnableWindow(bEnableDonn);
	}
	if (CWnd* pWnd = this->GetDlgItem(IDC_CHECK_DONNAN_CIO))
	{
		pWnd->EnableWindow(bEnableDonn);
	}
	if (CWnd* pWnd = this->GetDlgItem(IDC_CHECK_RETARD2))
	{
		pWnd->EnableWindow(bEnableDonn);
	}

	// Borkovec diffuse layer
	if (CWnd* pWnd = this->GetDlgItem(IDC_ST_BORK_THICKNESS))
	{
		pWnd->EnableWindow(bEnableBork);
	}
	if (CWnd* pWnd = this->GetDlgItem(IDC_EDIT_BORK_THICKNESS))
	{
		pWnd->EnableWindow(bEnableBork);
	}
	if (CWnd* pWnd = this->GetDlgItem(IDC_CHECK_BORK_CIO))
	{
		pWnd->EnableWindow(bEnableBork);
	}

	this->UpdateRadioDDL(bEnableDonn);
}

void CSurfacePage::UpdateRadioDDL(BOOL bEnabled)
{
	BOOL bEnableThickness = FALSE;
	switch (this->GetCheckedRadioButton(IDC_RADIO_THICK, IDC_RADIO_DEBYE))
	{
	case IDC_RADIO_THICK:
		bEnableThickness = TRUE;
		break;
	case IDC_RADIO_DEBYE:
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	// Thickness
	if (CWnd* pWnd = this->GetDlgItem(IDC_EDIT_DONNAN_THICKNESS))
	{
		pWnd->EnableWindow(bEnabled && bEnableThickness);
	}

	// Debye lengths
	if (CWnd* pWnd = this->GetDlgItem(IDC_EDIT_DEBYE_LENGTH))
	{
		pWnd->EnableWindow(bEnabled && !bEnableThickness);
	}
	if (CWnd* pWnd = this->GetDlgItem(IDC_ST_LIMIT_DDL))
	{
		pWnd->EnableWindow(bEnabled && !bEnableThickness);
	}
	if (CWnd* pWnd = this->GetDlgItem(IDC_EDIT_DEB_LEN_LIMIT))
	{
		pWnd->EnableWindow(bEnabled && !bEnableThickness);
	}

	// Counter ions only
	if (CWnd* pWnd = this->GetDlgItem(IDC_CHECK_DONNAN_CIO))
	{
		pWnd->EnableWindow(bEnabled);
	}

	// Retard diffusion
	if (CWnd* pWnd = this->GetDlgItem(IDC_CHECK_RETARD2))
	{
		pWnd->EnableWindow(bEnabled);
	}
	this->UpdateRetard(bEnabled);
}

void CSurfacePage::UpdateRetard(BOOL bEnabled)
{
	BOOL bEnableViscosity = (this->IsDlgButtonChecked(IDC_CHECK_RETARD2) == BST_CHECKED);
	if (CWnd* pWnd = this->GetDlgItem(IDC_ST_VISCOSITY))
	{
		pWnd->EnableWindow(bEnabled && bEnableViscosity);
	}
	if (CWnd* pWnd = this->GetDlgItem(IDC_EDIT_VISCOSITY))
	{
		pWnd->EnableWindow(bEnabled && bEnableViscosity);
	}
}

void CSurfacePage::OnBnClickedRadioNoEdl2()
{
	this->UpdateRadioSCM();
	this->GridSurfaces.SetColWidth(3, 0, 0);
	this->GridSurfaces.SetColWidth(4, 0, 0);
}

void CSurfacePage::OnBnClickedRadioDdl2()
{
	this->UpdateRadioSCM();
	this->GridSurfaces.SetColWidth(3, 0, 0);
	this->GridSurfaces.SetColWidth(4, 0, 0);
}

void CSurfacePage::OnBnClickedRadioCdMusic2()
{
	this->UpdateRadioSCM();
	this->GridSurfaces.SetColWidth(3, 0, 1500);
	this->GridSurfaces.SetColWidth(4, 0, 1500);
}

void CSurfacePage::OnBnClickedRadioDlTypeN()
{
	this->UpdateRadioDLO();
}

void CSurfacePage::OnBnClickedRadioDlTypeD()
{
	this->UpdateRadioDLO();
}

void CSurfacePage::OnBnClickedRadioDlTypeB()
{
	this->UpdateRadioDLO();
}

void CSurfacePage::OnBnClickedCheckRetard2()
{
	ASSERT(this->GetDlgItem(IDC_CHECK_RETARD2)->IsWindowEnabled());
	this->UpdateRetard(TRUE);
}

void CSurfacePage::OnBnClickedRadioThick()
{
	ASSERT(this->GetDlgItem(IDC_RADIO_THICK)->IsWindowEnabled());
	this->UpdateRadioDDL(TRUE);
}

void CSurfacePage::OnBnClickedRadioDebye()
{
	ASSERT(this->GetDlgItem(IDC_RADIO_DEBYE)->IsWindowEnabled());
	this->UpdateRadioDDL(TRUE);
}

void CSurfacePage::OnCbnSetfocusCboSolutions()
{
	CString strRes;
	strRes.LoadString(IDS_SURFACE_303);
	m_eInputDesc.SetWindowText(strRes);
}

#define DELETE_EXCEPTION(e) do { if(e) { e->Delete(); } } while (0)

void CSurfacePage::OnTcnSelchangingTabSurfType(NMHDR *pNMHDR, LRESULT *pResult)
{
	pNMHDR = pNMHDR;
	TRACE("%s\n", __FUNCTION__);

	// modified from CWnd::UpdateData()
	//
	CDataExchange dx(this, TRUE);

	// prevent control notifications from being dispatched during UpdateData
	_AFX_THREAD_STATE* pThreadState = AfxGetThreadState();
	HWND hWndOldLockout = pThreadState->m_hLockoutNotifyWindow;
	ASSERT(hWndOldLockout != m_hWnd);   // must not recurse
	pThreadState->m_hLockoutNotifyWindow = m_hWnd;

	BOOL bOK = FALSE;       // assume failure
	TRY
	{
		//DoDataExchange(&dx);
		this->ValidateEGSurfaces(&dx);
		this->ValidateEGSurfTypes(&dx);
		bOK = TRUE;         // it worked
	}
	CATCH(CUserException, e)
	{
		// validation failed - user already alerted, fall through
		ASSERT(!bOK);											
		// Note: DELETE_EXCEPTION_(e) not required
	}
	AND_CATCH_ALL(e)
	{
		// validation failed due to OOM or other resource failure
		e->ReportError(MB_ICONEXCLAMATION, AFX_IDP_INTERNAL_FAILURE);
		ASSERT(!bOK);
		DELETE_EXCEPTION(e);
	}
	END_CATCH_ALL

	pThreadState->m_hLockoutNotifyWindow = hWndOldLockout;

	if (bOK)
	{
		// clear GridListDoc
		switch (this->TabCtrlSurfType.GetCurSel())
		{
		case TI_GENERAL:
			this->GridListDocGen.DeleteAllRows();
			break;
		case TI_EQUILIBRIUM_PHASES:
			this->GridListDocEqu.DeleteAllRows();
			break;
		case TI_KINETIC_REACTANTS:
			this->GridListDocKin.DeleteAllRows();
			break;
		default:
			ASSERT(FALSE);
			break;
		}
		*pResult = FALSE; // allow tab change
	}
	else
	{
		*pResult = TRUE;  // don't allow tab change
	}
}

void CSurfacePage::OnTcnSelchangeTabSurfType(NMHDR *pNMHDR, LRESULT *pResult)
{
	TRACE("%s\n", __FUNCTION__);

	UNREFERENCED_PARAMETER(pNMHDR);

	// enable/disable Use site density & Mobile surfaces
	int n = TabCtrl_GetCurSel(this->GetDlgItem(IDC_TAB_SURF_TYPE)->GetSafeHwnd());
	if (CWnd *pWnd = this->GetDlgItem(IDC_CHECK_SITE_DENSITY))
	{
		pWnd->EnableWindow(n == TI_GENERAL);
	}
	if (CWnd *pWnd = this->GetDlgItem(IDC_CHECK_MOBILE))
	{
		pWnd->EnableWindow(n == TI_GENERAL);
	}
	*pResult = 0;

	// initialize
	this->InitCLC();
	this->InitEGSurfTypes();
	this->InitEGSurfaces();
	this->ListCtrlDefined.DeleteAllItems();
	this->UpdateListCtrlDefined();

	// modified from CWnd::UpdateData()
	//
	CDataExchange dx(this, FALSE);

	// prevent control notifications from being dispatched during UpdateData
	_AFX_THREAD_STATE* pThreadState = AfxGetThreadState();
	HWND hWndOldLockout = pThreadState->m_hLockoutNotifyWindow;
	ASSERT(hWndOldLockout != m_hWnd);   // must not recurse
	pThreadState->m_hLockoutNotifyWindow = m_hWnd;

	BOOL bOK = FALSE;       // assume failure
	TRY
	{
		//DoDataExchange(&dx);
		this->ExchangeEG_CLC(&dx);
		bOK = TRUE;         // it worked
	}
	CATCH(CUserException, e)
	{
		// validation failed - user already alerted, fall through
		ASSERT(!bOK);											
		// Note: DELETE_EXCEPTION_(e) not required
	}
	AND_CATCH_ALL(e)
	{
		// validation failed due to OOM or other resource failure
		e->ReportError(MB_ICONEXCLAMATION, AFX_IDP_INTERNAL_FAILURE);
		ASSERT(!bOK);
		DELETE_EXCEPTION(e);
	}
	END_CATCH_ALL

	pThreadState->m_hLockoutNotifyWindow = hWndOldLockout;
}

#undef DELETE_EXCEPTION

LRESULT CSurfacePage::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
{
	TabIndex ti = (TabIndex)TabCtrl_GetCurSel(this->GetDlgItem(IDC_TAB_SURF_TYPE)->GetSafeHwnd());
	switch (ti)
	{
	case TI_GENERAL:
		return this->OnEndCellEditGen(wParam, lParam);
		break;
	case TI_EQUILIBRIUM_PHASES:
		return this->OnEndCellEditEqu(wParam, lParam);
		break;
	case TI_KINETIC_REACTANTS:
		return this->OnEndCellEditKin(wParam, lParam);
		break;
	default:
		ASSERT(FALSE);
		break;
	}
	BOOL bMakeChange = FALSE;
	return bMakeChange;
}

LRESULT CSurfacePage::OnEndCellEditGen(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	
	if (nID == (UINT)this->GridSurfTypes.GetDlgCtrlID())
	{
		BOOL bMakeChange = TRUE;

		// ignore if edit is cancelled
		if ( pInfo->item.pszText == NULL ) return bMakeChange;

		if (pInfo->item.iCol == 1)
		{
			// fill in defaults
			CString str = pInfo->item.pszText;
			str.TrimLeft();
			if ( str.GetLength() > 0 )
			{
				CString strSave = this->GridSurfTypes.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol);
				this->GridSurfTypes.SetTextMatrix(pInfo->item.iRow, pInfo->item.iCol, pInfo->item.pszText);
				SetEGDefaults(pInfo->item.iRow);
				RemoveSurface(strSave);
				this->GridSurfTypes.SetTextMatrix(pInfo->item.iRow, pInfo->item.iCol, strSave);
			}
			else if (str.IsEmpty())
			{
				CString strSave = this->GridSurfTypes.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol);
				this->GridSurfTypes.SetTextMatrix(pInfo->item.iRow, pInfo->item.iCol, pInfo->item.pszText);
				RemoveSurface(strSave);
				this->GridSurfTypes.SetTextMatrix(pInfo->item.iRow, pInfo->item.iCol, strSave);
			}
		}
		return this->GridListDocGen.OnEndCellEdit(wParam, lParam);
	}
	else if (nID == (UINT)this->GridSurfaces.GetDlgCtrlID())
	{
		BOOL bMakeChange = TRUE;
		if (this->GridSurfaces.GetTextMatrix(pInfo->item.iRow, 0).IsEmpty())
		{
			bMakeChange = FALSE;
		}
		return bMakeChange;
	}
	else if (nID == (UINT)this->m_egNumDesc.GetDlgCtrlID())
	{
		BOOL bMakeChange = TRUE;
		return bMakeChange;
	}
	else
	{
		ASSERT(FALSE);
		BOOL bMakeChange = FALSE;
		return bMakeChange;
	}
}

LRESULT CSurfacePage::OnEndCellEditEqu(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	
	if (nID == (UINT)this->GridSurfTypes.GetDlgCtrlID())
	{
		BOOL bMakeChange = TRUE;

		// ignore if edit is cancelled
		if ( pInfo->item.pszText == NULL ) return bMakeChange;

		if (pInfo->item.iCol == 1)
		{
			// fill in defaults
			CString str = pInfo->item.pszText;
			str.TrimLeft();
			if ( str.GetLength() > 0 )
			{
				CString strSave = this->GridSurfTypes.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol);
				this->GridSurfTypes.SetTextMatrix(pInfo->item.iRow, pInfo->item.iCol, pInfo->item.pszText);
				SetEGDefaults(pInfo->item.iRow);
				RemoveSurface(strSave);
				this->GridSurfTypes.SetTextMatrix(pInfo->item.iRow, pInfo->item.iCol, strSave);
			}
			//{{
			else if (str.IsEmpty())
			{
				CString strSave = this->GridSurfTypes.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol);
				this->GridSurfTypes.SetTextMatrix(pInfo->item.iRow, pInfo->item.iCol, pInfo->item.pszText);
				RemoveSurface(strSave);
				this->GridSurfTypes.SetTextMatrix(pInfo->item.iRow, pInfo->item.iCol, strSave);
			}
			//}}
		}
		return this->GridListDocEqu.OnEndCellEdit(wParam, lParam);
	}
	else if (nID == (UINT)this->GridSurfaces.GetDlgCtrlID())
	{
		BOOL bMakeChange = TRUE;
		if (this->GridSurfaces.GetTextMatrix(pInfo->item.iRow, 0).IsEmpty())
		{
			bMakeChange = FALSE;
		}
		return bMakeChange;
	}
	else if (nID == (UINT)this->m_egNumDesc.GetDlgCtrlID())
	{
		BOOL bMakeChange = TRUE;
		return bMakeChange;
	}
	else
	{
		ASSERT(FALSE);
		BOOL bMakeChange = FALSE;
		return bMakeChange;
	}
}

LRESULT CSurfacePage::OnEndCellEditKin(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	
	if (nID == (UINT)this->GridSurfTypes.GetDlgCtrlID())
	{
		BOOL bMakeChange = TRUE;

		// ignore if edit is cancelled
		if (pInfo->item.pszText == NULL) return bMakeChange;

		if (pInfo->item.iCol == 1)
		{
			// fill in defaults
			CString str = pInfo->item.pszText;
			str.TrimLeft();
			if ( str.GetLength() > 0 )
			{
				CString strSave = this->GridSurfTypes.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol);
				this->GridSurfTypes.SetTextMatrix(pInfo->item.iRow, pInfo->item.iCol, pInfo->item.pszText);
				SetEGDefaults(pInfo->item.iRow);
				RemoveSurface(strSave);
				this->GridSurfTypes.SetTextMatrix(pInfo->item.iRow, pInfo->item.iCol, strSave);
			}
			else if (str.IsEmpty())
			{
				CString strSave = this->GridSurfTypes.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol);
				this->GridSurfTypes.SetTextMatrix(pInfo->item.iRow, pInfo->item.iCol, pInfo->item.pszText);
				RemoveSurface(strSave);
				this->GridSurfTypes.SetTextMatrix(pInfo->item.iRow, pInfo->item.iCol, strSave);
			}
		}
		return this->GridListDocKin.OnEndCellEdit(wParam, lParam);
	}
	else if (nID == (UINT)this->GridSurfaces.GetDlgCtrlID())
	{
		BOOL bMakeChange = TRUE;
		if (this->GridSurfaces.GetTextMatrix(pInfo->item.iRow, 0).IsEmpty())
		{
			bMakeChange = FALSE;
		}
		return bMakeChange;
	}
	else if (nID == (UINT)this->m_egNumDesc.GetDlgCtrlID())
	{
		BOOL bMakeChange = TRUE;
		return bMakeChange;
	}
	else
	{
		ASSERT(FALSE);
		BOOL bMakeChange = FALSE;
		return bMakeChange;
	}
}

long CSurfacePage::AddSurface(const CString& strSurface)
{
	CString type[3] = { _T("General"), _T("Equilibrium"), _T("Kinetic" )};

	//{{
	LVFINDINFO findinfo;
	findinfo.flags = LVFI_STRING;
	findinfo.psz = strSurface;
	int n = this->ListCtrlDefined.FindItem(&findinfo, -1);
	if (n == -1)
	{
		int i = this->ListCtrlDefined.GetItemCount();
		VERIFY(this->ListCtrlDefined.InsertItem(i, strSurface) != -1);
		VERIFY(this->ListCtrlDefined.SetItemText(i, 1, type[this->TabCtrlSurfType.GetCurSel()]));
	}
	//}}

	long row;
	CString str;
	CString strSurf = strSurface.SpanExcluding(_T("_"));
	for (row = 1; row < this->GridSurfaces.GetRows() - 1; ++row)
	{
		str = this->GridSurfaces.GetTextMatrix(row, 0);
		if (str.Compare(strSurf) == 0)
		{
			// already in list
			break;
		}
		else if (str.IsEmpty())
		{
			this->GridSurfaces.SetTextMatrix(row, 0, strSurf);
			this->GridSurfaces.SetRow(row);
			this->GridSurfaces.SetCol(0);
			this->GridSurfaces.SetCellAlignment(flexAlignCenterCenter);
			this->GridSurfaces.SetCol(1);
			break;
		}
	}
	return row;
}

void CSurfacePage::RemoveSurface(const CString& strSurface)
{
	long row;
	CString str, strSurf2;
	CString strSurf = strSurface.SpanExcluding(_T("_"));
	BOOL bDoNothing = FALSE;
	for (row = 1; row < this->GridSurfTypes.GetRows() - 1; ++row)
	{
		str = this->GridSurfTypes.GetTextMatrix(row, 1);
		strSurf2 = str.SpanExcluding(_T("_"));
		if (strSurf2.Compare(strSurf) == 0)
		{
			// surface still in list
			bDoNothing = TRUE;
			break;
		}
	}
	if (bDoNothing == FALSE)
	{
		for (row = 1; row < this->GridSurfaces.GetRows() - 1; ++row)
		{
			str = this->GridSurfaces.GetTextMatrix(row, 0);
			if (str.Compare(strSurf) == 0)
			{
				// delete row doesn't remove header column
				this->GridSurfaces.SetTextMatrix(row, 0, _T(""));
				this->GridSurfaces.DeleteRowAndHeader(row);
				break;
			}
			else if (str.IsEmpty())
			{
				ASSERT(FALSE);
				break;
			}
		}
	}

	//{{
	if (!strSurface.IsEmpty())
	{
		LVFINDINFO findinfo;
		findinfo.flags = LVFI_STRING;
		findinfo.psz = strSurface;
		int n = this->ListCtrlDefined.FindItem(&findinfo, -1);
		if (n != -1)
		{
			this->ListCtrlDefined.DeleteItem(n);
		}
	}
	//}}
}

long CSurfacePage::GetSurface(const char *szFormula)
{
	long row;
	CString strSurf = CString(szFormula).SpanExcluding(_T("_"));
	CString str;
	for (row = 1; row < this->GridSurfaces.GetRows() - 1; ++row)
	{
		str = this->GridSurfaces.GetTextMatrix(row, 0);
		if (str.Compare(strSurf) == 0)
		{
			return row;
			break;
		}
		if (str.IsEmpty())
		{
			break;
		}
	}
	return -1;
}

BOOL CSurfacePage::InitCLC()
{
	TabIndex ti = (TabIndex)this->TabCtrlSurfType.GetCurSel();
	switch (ti)
	{
	case TI_GENERAL:
		return this->InitCLCGen();
		break;
	case TI_EQUILIBRIUM_PHASES:
		return this->InitCLCEqu();
		break;
	case TI_KINETIC_REACTANTS:
		return this->InitCLCKin();
		break;
	default:
		ASSERT(FALSE);
		break;
	}
	return FALSE;
}

BOOL CSurfacePage::InitCLCGen()
{
	int nItem;

	this->ListCtrlSurface.DeleteAllItems();
	if (this->n_solution == N_NONE && this->CheckBoxEquilibrate.GetCheck() == BST_UNCHECKED)
	{
		ASSERT(this->ListCtrlSurface.GetItemCount() == 0);
		std::set<CDBElement>::const_iterator elemIter = this->GetDatabase().m_elementSet.begin();
		for ( ; elemIter != this->GetDatabase().m_elementSet.end(); ++elemIter)
		{
			if ( (*elemIter).m_type == CDBElement::typeSURF )
			{
				if ((nItem = this->ListCtrlSurface.InsertItem(0, (*elemIter).m_strMaster)) != -1)
				{
					// add to columns here
				}
			}
		}
	}
	else 
	{
		ASSERT(this->ListCtrlSurface.GetItemCount() == 0);
		std::set<CDBElement>::const_iterator elemIter = this->GetDatabase().m_elementSet.begin();
		for ( ; elemIter != this->GetDatabase().m_elementSet.end(); ++elemIter)
		{
			if ( (*elemIter).m_type == CDBElement::typeSURF )
			{
				if ((nItem = this->ListCtrlSurface.InsertItem(0, (*elemIter).m_strName)) != -1)
				{
					// add to columns here
				}
			}
		}
	}

	this->ListCtrlSurface.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);

	return TRUE;
}

BOOL CSurfacePage::InitCLCEqu()
{
	int nItem;

	this->ListCtrlSurface.DeleteAllItems();
	std::set<CDBElement>::const_iterator elemIter = this->GetDatabase().m_elementSet.begin();
	for ( ; elemIter != this->GetDatabase().m_elementSet.end(); ++elemIter)
	{
		if ( (*elemIter).m_type == CDBElement::typeSURF )
		{
			if ((nItem = this->ListCtrlSurface.InsertItem(0, (*elemIter).m_strMaster)) != -1)
			{
				// add to columns here
			}
		}
	}

	this->ListCtrlSurface.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);

	return TRUE;
}

BOOL CSurfacePage::InitCLCKin()
{
	int nItem;

	this->ListCtrlSurface.DeleteAllItems();
	std::set<CDBElement>::const_iterator elemIter = this->GetDatabase().m_elementSet.begin();
	for ( ; elemIter != this->GetDatabase().m_elementSet.end(); ++elemIter)
	{
		if ( (*elemIter).m_type == CDBElement::typeSURF )
		{
			if ((nItem = this->ListCtrlSurface.InsertItem(0, (*elemIter).m_strMaster)) != -1)
			{
				// add to columns here
			}
		}
	}

	this->ListCtrlSurface.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);

	return TRUE;
}

void CSurfacePage::ExchangeEG_CLC(CDataExchange* pDX)
{
	TabIndex ti = (TabIndex)TabCtrl_GetCurSel(this->GetDlgItem(IDC_TAB_SURF_TYPE)->GetSafeHwnd());
	switch (ti)
	{
	case TI_GENERAL:
		this->ExchangeEG_CLCGen(pDX);
		break;
	case TI_EQUILIBRIUM_PHASES:
		this->ExchangeEG_CLCEqu(pDX);
		break;
	case TI_KINETIC_REACTANTS:
		this->ExchangeEG_CLCKin(pDX);
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}

void CSurfacePage::ExchangeEG_CLCGen(CDataExchange* pDX)
{
	pDX = pDX;
	// TI_GENERAL

	CString format;

	ASSERT( this->ListCtrlSurface.GetSafeHwnd() != NULL );
	ASSERT( this->GridSurfTypes.GetSafeHwnd() != NULL );
	ASSERT( this->GridSurfaces.GetSafeHwnd() != NULL );

	// verify state of gridlistdoc
	ASSERT( this->GridListDocGen.GetCount() == 0 );

	if (CButton* pButton = (CButton*)this->GetDlgItem(IDC_CHECK_SITE_DENSITY))
	{
		if (this->sites_units == SITES_DENSITY)
		{
			pButton->SetCheck(BST_CHECKED);
		}
		else
		{
			pButton->SetCheck(BST_UNCHECKED);
		}
	}

	if (CButton *pBtn = (CButton*)this->GetDlgItem(IDC_CHECK_MOBILE))
	{
		if (this->transport)
		{
			pBtn->SetCheck(BST_CHECKED);
		}
		else
		{
			pBtn->SetCheck(BST_UNCHECKED);
		}
	}

	long row = 1;
	std::list<CSurfComp>::const_iterator const_iter = this->ListSurfComp[CSurfacePage::TI_GENERAL].begin();
	for (; const_iter != this->ListSurfComp[CSurfacePage::TI_GENERAL].end(); ++const_iter, ++row)
	{
		// exchange formula
		row = this->GridListDocGen.AddItem((*const_iter).m_strFormula).m_nRow;		

		// amount
		format.Format("%g", (*const_iter).m_dMoles);
		this->GridSurfTypes.SetTextMatrix(row, 2, format);

		// surfaces
		long row2 = this->AddSurface((*const_iter).m_strFormula);
		if (row2 != -1)
		{
			format.Format("%g", (*const_iter).m_dSpecific_area);
			this->GridSurfaces.SetTextMatrix(row2, 1, format);
			format.Format("%g", (*const_iter).m_dGrams);
			this->GridSurfaces.SetTextMatrix(row2, 2, format);

// COMMENT: {1/7/2010 3:25:28 PM}			if (this->dl_type == CD_MUSIC)
// COMMENT: {1/7/2010 3:25:28 PM}			{
// COMMENT: {1/7/2010 3:25:28 PM}				format.Format("%g", (*const_iter).m_dCapacitance0);
// COMMENT: {1/7/2010 3:25:28 PM}				this->GridSurfaces.SetTextMatrix(row2, 3, format);
// COMMENT: {1/7/2010 3:25:28 PM}
// COMMENT: {1/7/2010 3:25:28 PM}				format.Format("%g", (*const_iter).m_dCapacitance1);
// COMMENT: {1/7/2010 3:25:28 PM}				this->GridSurfaces.SetTextMatrix(row2, 4, format);
// COMMENT: {1/7/2010 3:25:28 PM}			}

			format.Format("%g", (*const_iter).m_dCapacitance0);
			this->GridSurfaces.SetTextMatrix(row2, 3, format);

			format.Format("%g", (*const_iter).m_dCapacitance1);
			this->GridSurfaces.SetTextMatrix(row2, 4, format);

			if ((*const_iter).m_dDw > 0.)
			{
				format.Format("%g", (*const_iter).m_dDw);
				this->GridSurfaces.SetTextMatrix(row2, 5, format);
			}
			else
			{
				this->GridSurfaces.SetTextMatrix(row2, 5, _T(""));
			}
		}
	}

	// verify state of gridlistdoc
	ASSERT( this->ListSurfComp[CSurfacePage::TI_GENERAL].size() == this->GridListDocGen.GetCount() );
}

void CSurfacePage::ExchangeEG_CLCEqu(CDataExchange* pDX)
{
	// TI_EQUILIBRIUM_PHASES

	long nRow = 1;
	CString format;

	ASSERT( this->ListCtrlSurface.GetSafeHwnd() != NULL );
	ASSERT( this->GridSurfTypes.GetSafeHwnd() != NULL );
	ASSERT( this->GridSurfaces.GetSafeHwnd() != NULL );
	ASSERT( this->GridListDocEqu.GetCount() == 0 );

	std::list<CSurfComp>::iterator iter = this->ListSurfComp[CSurfacePage::TI_EQUILIBRIUM_PHASES].begin();
	for (; iter != this->ListSurfComp[CSurfacePage::TI_EQUILIBRIUM_PHASES].end(); ++iter, ++nRow)
	{
		if (!(*iter).m_strPhase_name.IsEmpty())
		{
			nRow = this->GridListDocEqu.AddItem((*iter).m_strFormula).m_nRow;
		
			// name
			ASSERT((*iter).m_strRate_name.GetLength() == 0);
			DDX_GridText(pDX, this->GridSurfTypes.GetDlgCtrlID(), nRow, 2, (*iter).m_strPhase_name);

			// sites_per_mole
			DDX_GridText(pDX, this->GridSurfTypes.GetDlgCtrlID(), nRow, 3, (*iter).m_dPhase_proportion);

			// surfaces
			long nRow2 = this->AddSurface((*iter).m_strFormula);
			if (nRow2 != -1)
			{
				DDX_GridText(pDX, this->GridSurfaces.GetDlgCtrlID(), nRow2, 1, (*iter).m_dSpecific_area);
			}
		}
	}

	// verify state of gridlistdoc
	ASSERT( this->ListSurfComp[CSurfacePage::TI_EQUILIBRIUM_PHASES].size() == this->GridListDocEqu.GetCount() );
}

void CSurfacePage::ExchangeEG_CLCKin(CDataExchange* pDX)
{
	// TI_KINETIC_REACTANTS

	CString format;

	ASSERT( this->ListCtrlSurface.GetSafeHwnd() != NULL );
	ASSERT( this->GridSurfTypes.GetSafeHwnd() != NULL );
	ASSERT( this->GridSurfaces.GetSafeHwnd() != NULL );
	ASSERT( this->GridListDocKin.GetCount() == 0 );

	std::list<CSurfComp>::iterator iter = this->ListSurfComp[CSurfacePage::TI_KINETIC_REACTANTS].begin();
	for (long nRow = 1; iter != this->ListSurfComp[CSurfacePage::TI_KINETIC_REACTANTS].end(); ++iter, ++nRow)
	{
		if (!(*iter).m_strRate_name.IsEmpty())
		{
			nRow = this->GridListDocKin.AddItem((*iter).m_strFormula).m_nRow;

			// name
			ASSERT((*iter).m_strPhase_name.IsEmpty());
			DDX_GridText(pDX, this->GridSurfTypes.GetDlgCtrlID(), nRow, 2, (*iter).m_strRate_name);

			// sites_per_mole
			DDX_GridText(pDX, this->GridSurfTypes.GetDlgCtrlID(), nRow, 3, (*iter).m_dPhase_proportion);

			// surfaces
			long nRow2 = this->AddSurface((*iter).m_strFormula);
			if (nRow2 != -1)
			{
				DDX_GridText(pDX, this->GridSurfaces.GetDlgCtrlID(), nRow2, 1, (*iter).m_dSpecific_area);
			}
		}
	}	

	// verify state of gridlistdoc
	ASSERT( this->ListSurfComp[CSurfacePage::TI_KINETIC_REACTANTS].size() == this->GridListDocKin.GetCount() );
}

BOOL CSurfacePage::InitEGSurfTypes()
{
	TabIndex ti = (TabIndex)TabCtrl_GetCurSel(this->GetDlgItem(IDC_TAB_SURF_TYPE)->GetSafeHwnd());
	switch (ti)
	{
	case TI_GENERAL:
		return this->InitEGSurfTypesGen();
		break;
	case TI_EQUILIBRIUM_PHASES:
		return this->InitEGSurfTypesEqu();
		break;
	case TI_KINETIC_REACTANTS:
		return this->InitEGSurfTypesKin();
		break;
	default:
		ASSERT(FALSE);
		break;
	}
	return FALSE;
}

BOOL CSurfacePage::InitEGSurfTypesGen()
{
	const long ROWS = 100;

	// set rows, cols
	this->GridSurfTypes.SetRows(ROWS);
	this->GridSurfTypes.SetCols(0, 3);

	// set column titles
	this->GridSurfTypes.SetTextMatrix(0, 1, _T("Site type"));
	if (this->sites_units == SITES_DENSITY)
	{
		this->GridSurfTypes.SetTextMatrix(0, 2, _T("D(sites/nm^2)"));
	}
	else
	{
		this->GridSurfTypes.SetTextMatrix(0, 2, _T("Sites (moles)"));
	}

	// set bold titles
	this->GridSurfTypes.SetRow(0), this->GridSurfTypes.SetCol(1);
	this->GridSurfTypes.SetCellFontBold(TRUE);
	this->GridSurfTypes.SetRow(0), this->GridSurfTypes.SetCol(2);
	this->GridSurfTypes.SetCellFontBold(TRUE);

	// center align the column headers
	this->GridSurfTypes.SetFillStyle(flexFillRepeat);
	this->GridSurfTypes.SetRow(0);
	this->GridSurfTypes.SetCol(1);
	this->GridSurfTypes.SetColSel(this->GridSurfTypes.GetCols(0) - 1);
	this->GridSurfTypes.SetCellAlignment(flexAlignCenterCenter);
	this->GridSurfTypes.SetFillStyle(flexFillSingle);

	// set column widths
	this->GridSurfTypes.SetColWidth(0, 0, 500);
	this->GridSurfTypes.SetColWidth(1, 0, 900);
	this->GridSurfTypes.SetColWidth(2, 0, 1300);

	// set row titles
	this->GridSurfTypes.AddRowHeaders();

	// clear cells
	this->GridSurfTypes.ClearWorkSpace();

// COMMENT: {1/6/2010 6:36:00 PM}	// clear cells
// COMMENT: {1/6/2010 6:36:00 PM}	for (long row = 1; row < this->GridSurfTypes.GetRows(); ++row)
// COMMENT: {1/6/2010 6:36:00 PM}	{
// COMMENT: {1/6/2010 6:36:00 PM}		for (long col = 1; col < this->GridSurfTypes.GetCols(0); ++col)
// COMMENT: {1/6/2010 6:36:00 PM}		{
// COMMENT: {1/6/2010 6:36:00 PM}			this->GridSurfTypes.SetTextMatrix(row, col, _T(""));
// COMMENT: {1/6/2010 6:36:00 PM}		}
// COMMENT: {1/6/2010 6:36:00 PM}	}

	// scroll to top of grid
	this->GridSurfTypes.SetTopRow(1);
	this->GridSurfTypes.SetLeftCol(1);

	// set initial cell
	this->GridSurfTypes.SetRow(1);
	this->GridSurfTypes.SetCol(1);

	return TRUE;
}

BOOL CSurfacePage::InitEGSurfTypesEqu()
{
	const long ROWS = 100;

	// set rows, cols
	this->GridSurfTypes.SetRows(ROWS);
	this->GridSurfTypes.SetCols(0, 4);

	// set column titles
	this->GridSurfTypes.SetTextMatrix(0, 1, _T("Site type"));
	this->GridSurfTypes.SetTextMatrix(0, 2, _T("Equil. phase"));
	this->GridSurfTypes.SetTextMatrix(0, 3, _T("Sites/mol"));

	// set bold titles
	this->GridSurfTypes.SetRow(0), this->GridSurfTypes.SetCol(1);
	this->GridSurfTypes.SetCellFontBold(TRUE);
	this->GridSurfTypes.SetRow(0), this->GridSurfTypes.SetCol(2);
	this->GridSurfTypes.SetCellFontBold(TRUE);
	this->GridSurfTypes.SetRow(0), this->GridSurfTypes.SetCol(3);
	this->GridSurfTypes.SetCellFontBold(TRUE);

	// center align the column headers
	this->GridSurfTypes.SetFillStyle(flexFillRepeat);
	this->GridSurfTypes.SetRow(0);
	this->GridSurfTypes.SetCol(1);
	this->GridSurfTypes.SetColSel(this->GridSurfTypes.GetCols(0) - 1);
	this->GridSurfTypes.SetCellAlignment(flexAlignCenterCenter);
	this->GridSurfTypes.SetFillStyle(flexFillSingle);

	// set column widths
	this->GridSurfTypes.SetColWidth(0, 0, 1050);
	this->GridSurfTypes.SetColWidth(1, 0, 1200);
	this->GridSurfTypes.SetColWidth(2, 0, 1100);

	// set row titles
	this->GridSurfTypes.AddRowHeaders();

	// clear cells
	this->GridSurfTypes.ClearWorkSpace();

// COMMENT: {1/6/2010 6:36:00 PM}	// clear cells
// COMMENT: {1/6/2010 6:36:00 PM}	for (long row = 1; row < this->GridSurfTypes.GetRows(); ++row)
// COMMENT: {1/6/2010 6:36:00 PM}	{
// COMMENT: {1/6/2010 6:36:00 PM}		for (long col = 1; col < this->GridSurfTypes.GetCols(0); ++col)
// COMMENT: {1/6/2010 6:36:00 PM}		{
// COMMENT: {1/6/2010 6:36:00 PM}			this->GridSurfTypes.SetTextMatrix(row, col, _T(""));
// COMMENT: {1/6/2010 6:36:00 PM}		}
// COMMENT: {1/6/2010 6:36:00 PM}	}

	// scroll to top of grid
	this->GridSurfTypes.SetTopRow(1);
	this->GridSurfTypes.SetLeftCol(1);

	// set initial cell
	this->GridSurfTypes.SetRow(1);
	this->GridSurfTypes.SetCol(1);

	return TRUE;
}

BOOL CSurfacePage::InitEGSurfTypesKin()
{
	const long ROWS = 100;

	// set rows, cols
	this->GridSurfTypes.SetRows(ROWS);
	this->GridSurfTypes.SetCols(0, 4);

	// set column titles
	this->GridSurfTypes.SetTextMatrix(0, 1, _T("Site type"));
	this->GridSurfTypes.SetTextMatrix(0, 2, _T("Kin. reactant"));
	this->GridSurfTypes.SetTextMatrix(0, 3, _T("Sites/mol"));

	// set bold titles
	this->GridSurfTypes.SetRow(0), this->GridSurfTypes.SetCol(1);
	this->GridSurfTypes.SetCellFontBold(TRUE);
	this->GridSurfTypes.SetRow(0), this->GridSurfTypes.SetCol(2);
	this->GridSurfTypes.SetCellFontBold(TRUE);
	this->GridSurfTypes.SetRow(0), this->GridSurfTypes.SetCol(3);
	this->GridSurfTypes.SetCellFontBold(TRUE);

	// center align the column headers
	this->GridSurfTypes.SetFillStyle(flexFillRepeat);
	this->GridSurfTypes.SetRow(0);
	this->GridSurfTypes.SetCol(1);
	this->GridSurfTypes.SetColSel(this->GridSurfTypes.GetCols(0) - 1);
	this->GridSurfTypes.SetCellAlignment(flexAlignCenterCenter);
	this->GridSurfTypes.SetFillStyle(flexFillSingle);

	// set column widths
	this->GridSurfTypes.SetColWidth(0, 0, 1050);
	this->GridSurfTypes.SetColWidth(1, 0, 1300);
	this->GridSurfTypes.SetColWidth(2, 0, 1000);

	// set row titles
	this->GridSurfTypes.AddRowHeaders();

	// clear cells
	this->GridSurfTypes.ClearWorkSpace();

// COMMENT: {1/6/2010 6:36:00 PM}	// clear cells
// COMMENT: {1/6/2010 6:36:00 PM}	for (long row = 1; row < this->GridSurfTypes.GetRows(); ++row)
// COMMENT: {1/6/2010 6:36:00 PM}	{
// COMMENT: {1/6/2010 6:36:00 PM}		for (long col = 1; col < this->GridSurfTypes.GetCols(0); ++col)
// COMMENT: {1/6/2010 6:36:00 PM}		{
// COMMENT: {1/6/2010 6:36:00 PM}			this->GridSurfTypes.SetTextMatrix(row, col, _T(""));
// COMMENT: {1/6/2010 6:36:00 PM}		}
// COMMENT: {1/6/2010 6:36:00 PM}	}

	// scroll to top of grid
	this->GridSurfTypes.SetTopRow(1);
	this->GridSurfTypes.SetLeftCol(1);

	// set initial cell
	this->GridSurfTypes.SetRow(1);
	this->GridSurfTypes.SetCol(1);

	return TRUE;
}

BOOL CSurfacePage::InitEGSurfaces()
{
	TabIndex ti = (TabIndex)TabCtrl_GetCurSel(this->GetDlgItem(IDC_TAB_SURF_TYPE)->GetSafeHwnd());
	switch (ti)
	{
	case TI_GENERAL:
		return this->InitEGSurfacesGen();
		break;
	case TI_EQUILIBRIUM_PHASES:
		return this->InitEGSurfacesEqu();
		break;
	case TI_KINETIC_REACTANTS:
		return this->InitEGSurfacesKin();
		break;
	default:
		ASSERT(FALSE);
		break;
	}
	return FALSE;
}

BOOL CSurfacePage::InitEGSurfacesGen()
{
	const long ROWS = 100;

// COMMENT: {1/6/2010 6:36:47 PM}	if ( m_bFirstSetActive == FALSE )
// COMMENT: {1/6/2010 6:36:47 PM}	{
// COMMENT: {1/6/2010 6:36:47 PM}		this->GridSurfaces.ClearWorkSpace();
// COMMENT: {1/6/2010 6:36:47 PM}		for ( long row = this->GridSurfaces.GetFixedRows(); row < this->GridSurfaces.GetRows(); ++row)
// COMMENT: {1/6/2010 6:36:47 PM}		{
// COMMENT: {1/6/2010 6:36:47 PM}			this->GridSurfaces.SetTextMatrix(row, 0, _T(""));
// COMMENT: {1/6/2010 6:36:47 PM}		}
// COMMENT: {1/6/2010 6:36:47 PM}		return TRUE;
// COMMENT: {1/6/2010 6:36:47 PM}	}

	// set rows, cols
	this->GridSurfaces.SetRows(ROWS);
	this->GridSurfaces.SetCols(0, 6);

	// set column titles
	this->GridSurfaces.SetTextMatrix(0, 0, _T("Surface"));
	this->GridSurfaces.SetTextMatrix(0, 1, _T("Area (m^2/g)"));
	this->GridSurfaces.SetTextMatrix(0, 2, _T("Mass (g)"));
	this->GridSurfaces.SetTextMatrix(0, 3, _T("Cap. 0-1 plane"));
	this->GridSurfaces.SetTextMatrix(0, 4, _T("Cap. 1-2 plane"));
	this->GridSurfaces.SetTextMatrix(0, 5, _T("Dw"));

	// set bold titles
	this->GridSurfaces.SetRow(0), this->GridSurfaces.SetCol(0);
	this->GridSurfaces.SetCellFontBold(TRUE);
	this->GridSurfaces.SetRow(0), this->GridSurfaces.SetCol(1);
	this->GridSurfaces.SetCellFontBold(TRUE);
	this->GridSurfaces.SetRow(0), this->GridSurfaces.SetCol(2);
	this->GridSurfaces.SetCellFontBold(TRUE);
	this->GridSurfaces.SetRow(0), this->GridSurfaces.SetCol(3);
	this->GridSurfaces.SetCellFontBold(TRUE);
	this->GridSurfaces.SetRow(0), this->GridSurfaces.SetCol(4);
	this->GridSurfaces.SetCellFontBold(TRUE);
	this->GridSurfaces.SetRow(0), this->GridSurfaces.SetCol(5);
	this->GridSurfaces.SetCellFontBold(TRUE);

	// center align the column headers
	this->GridSurfaces.SetFillStyle(flexFillRepeat);
	this->GridSurfaces.SetRow(0);
	this->GridSurfaces.SetCol(0);
	this->GridSurfaces.SetColSel(this->GridSurfaces.GetCols(0) - 1);
	this->GridSurfaces.SetCellAlignment(flexAlignCenterCenter);
	this->GridSurfaces.SetFillStyle(flexFillSingle);

	// set column widths
	this->GridSurfaces.SetColWidth(0, 0, 800);
	this->GridSurfaces.SetColWidth(1, 0, 1300);
	this->GridSurfaces.SetColWidth(2, 0, 920);
	if (this->type == CD_MUSIC)
	{
		this->GridSurfaces.SetColWidth(3, 0, 1500);
		this->GridSurfaces.SetColWidth(4, 0, 1500);
	}
	else
	{
		this->GridSurfaces.SetColWidth(3, 0, 0);
		this->GridSurfaces.SetColWidth(4, 0, 0);
	}

	if (this->transport)
	{
		this->GridSurfaces.SetColWidth(5, 0, 800);
	}
	else
	{
		this->GridSurfaces.SetColWidth(5, 0, 0);
	}

	// clear cells
	for (int row = 1; row < this->GridSurfaces.GetRows(); ++row)
	{
		this->GridSurfaces.SetTextMatrix(row, 0, _T(""));
	}
	this->GridSurfaces.ClearWorkSpace();

	// scroll to top of grid
	this->GridSurfaces.SetTopRow(1); this->GridSurfaces.SetLeftCol(1);

	// set initial cell
	this->GridSurfaces.SetRow(1); this->GridSurfaces.SetCol(1);

	return TRUE;
}

BOOL CSurfacePage::InitEGSurfacesEqu()
{
	const long ROWS = 100;

// COMMENT: {1/6/2010 6:36:47 PM}	if ( m_bFirstSetActive == FALSE )
// COMMENT: {1/6/2010 6:36:47 PM}	{
// COMMENT: {1/6/2010 6:36:47 PM}		this->GridSurfaces.ClearWorkSpace();
// COMMENT: {1/6/2010 6:36:47 PM}		for ( long row = this->GridSurfaces.GetFixedRows(); row < this->GridSurfaces.GetRows(); ++row)
// COMMENT: {1/6/2010 6:36:47 PM}		{
// COMMENT: {1/6/2010 6:36:47 PM}			this->GridSurfaces.SetTextMatrix(row, 0, _T(""));
// COMMENT: {1/6/2010 6:36:47 PM}		}
// COMMENT: {1/6/2010 6:36:47 PM}		return TRUE;
// COMMENT: {1/6/2010 6:36:47 PM}	}

	// set rows, cols
	this->GridSurfaces.SetRows(ROWS);
	this->GridSurfaces.SetCols(0, 4);

	// set column titles
	this->GridSurfaces.SetTextMatrix(0, 0, _T("Surface"));
	this->GridSurfaces.SetTextMatrix(0, 1, _T("Area (m^2/g)"));
	this->GridSurfaces.SetTextMatrix(0, 2, _T("Cap. 0-1 plane"));
	this->GridSurfaces.SetTextMatrix(0, 3, _T("Cap. 1-2 plane"));

	// set bold titles
	this->GridSurfaces.SetRow(0), this->GridSurfaces.SetCol(0);
	this->GridSurfaces.SetCellFontBold(TRUE);
	this->GridSurfaces.SetRow(0), this->GridSurfaces.SetCol(1);
	this->GridSurfaces.SetCellFontBold(TRUE);
	this->GridSurfaces.SetRow(0), this->GridSurfaces.SetCol(2);
	this->GridSurfaces.SetCellFontBold(TRUE);
	this->GridSurfaces.SetRow(0), this->GridSurfaces.SetCol(3);
	this->GridSurfaces.SetCellFontBold(TRUE);

	// center align the column headers
	this->GridSurfaces.SetFillStyle(flexFillRepeat);
	this->GridSurfaces.SetRow(0);
	this->GridSurfaces.SetCol(1);
	this->GridSurfaces.SetColSel(this->GridSurfaces.GetCols(0) - 1);
	this->GridSurfaces.SetCellAlignment(flexAlignCenterCenter);
	this->GridSurfaces.SetFillStyle(flexFillSingle);

	// set column widths
	this->GridSurfaces.SetColWidth(0, 0, 800);
	this->GridSurfaces.SetColWidth(1, 0, 1500);
	if (this->type == CD_MUSIC)
	{
		this->GridSurfaces.SetColWidth(2, 0, 1500);
		this->GridSurfaces.SetColWidth(3, 0, 1500);
	}
	else
	{
		this->GridSurfaces.SetColWidth(2, 0, 0);
		this->GridSurfaces.SetColWidth(3, 0, 0);
	}

	// clear cells
	for (int row = 1; row < this->GridSurfaces.GetRows(); ++row)
	{
		this->GridSurfaces.SetTextMatrix(row, 0, _T(""));
	}
	this->GridSurfaces.ClearWorkSpace();

	// scroll to top of grid
	this->GridSurfaces.SetTopRow(1); this->GridSurfaces.SetLeftCol(1);

	// set initial cell
	this->GridSurfaces.SetRow(1); this->GridSurfaces.SetCol(1);

	return TRUE;
}

BOOL CSurfacePage::InitEGSurfacesKin()
{
	const long ROWS = 100;

// COMMENT: {1/6/2010 6:36:47 PM}	if ( m_bFirstSetActive == FALSE )
// COMMENT: {1/6/2010 6:36:47 PM}	{
// COMMENT: {1/6/2010 6:36:47 PM}		this->GridSurfaces.ClearWorkSpace();
// COMMENT: {1/6/2010 6:36:47 PM}		for ( long row = this->GridSurfaces.GetFixedRows(); row < this->GridSurfaces.GetRows(); ++row)
// COMMENT: {1/6/2010 6:36:47 PM}		{
// COMMENT: {1/6/2010 6:36:47 PM}			this->GridSurfaces.SetTextMatrix(row, 0, _T(""));
// COMMENT: {1/6/2010 6:36:47 PM}		}
// COMMENT: {1/6/2010 6:36:47 PM}		return TRUE;
// COMMENT: {1/6/2010 6:36:47 PM}	}

	// set rows, cols
	this->GridSurfaces.SetRows(ROWS);
	this->GridSurfaces.SetCols(0, 4);

	// set column titles
	this->GridSurfaces.SetTextMatrix(0, 0, _T("Surface"));
	this->GridSurfaces.SetTextMatrix(0, 1, _T("Area (m^2/g)"));
	this->GridSurfaces.SetTextMatrix(0, 2, _T("Cap. 0-1 plane"));
	this->GridSurfaces.SetTextMatrix(0, 3, _T("Cap. 1-2 plane"));

	// set bold titles
	this->GridSurfaces.SetRow(0), this->GridSurfaces.SetCol(0);
	this->GridSurfaces.SetCellFontBold(TRUE);
	this->GridSurfaces.SetRow(0), this->GridSurfaces.SetCol(1);
	this->GridSurfaces.SetCellFontBold(TRUE);
	this->GridSurfaces.SetRow(0), this->GridSurfaces.SetCol(2);
	this->GridSurfaces.SetCellFontBold(TRUE);
	this->GridSurfaces.SetRow(0), this->GridSurfaces.SetCol(3);
	this->GridSurfaces.SetCellFontBold(TRUE);

	// center align the column headers
	this->GridSurfaces.SetFillStyle(flexFillRepeat);
	this->GridSurfaces.SetRow(0);
	this->GridSurfaces.SetCol(1);
	this->GridSurfaces.SetColSel(this->GridSurfaces.GetCols(0) - 1);
	this->GridSurfaces.SetCellAlignment(flexAlignCenterCenter);
	this->GridSurfaces.SetFillStyle(flexFillSingle);

	// set column widths
	this->GridSurfaces.SetColWidth(0, 0, 800);
	this->GridSurfaces.SetColWidth(1, 0, 1500);
	if (this->type == CD_MUSIC)
	{
		this->GridSurfaces.SetColWidth(2, 0, 1500);
		this->GridSurfaces.SetColWidth(3, 0, 1500);
	}
	else
	{
		this->GridSurfaces.SetColWidth(2, 0, 0);
		this->GridSurfaces.SetColWidth(3, 0, 0);
	}

	// clear cells
	for (int row = 1; row < this->GridSurfaces.GetRows(); ++row)
	{
		this->GridSurfaces.SetTextMatrix(row, 0, _T(""));
	}
	this->GridSurfaces.ClearWorkSpace();

	// scroll to top of grid
	this->GridSurfaces.SetTopRow(1); this->GridSurfaces.SetLeftCol(1);

	// set initial cell
	this->GridSurfaces.SetRow(1); this->GridSurfaces.SetCol(1);

	return TRUE;
}

void CSurfacePage::EnterCellMshfgSurftype()
{
	CString strRes;
	switch (TabCtrl_GetCurSel(this->GetDlgItem(IDC_TAB_SURF_TYPE)->GetSafeHwnd()))
	{
	case TI_GENERAL:
		switch (this->GridSurfTypes.GetCol())
		{
		case 1:
			strRes.LoadString(IDS_SURFACE_305);
			break;
		case 2:
			if (this->IsDlgButtonChecked(IDC_CHECK_SITE_DENSITY) == BST_CHECKED)
			{
				strRes.LoadString(IDS_SURFACE_626);
			}
			else
			{
				strRes.LoadString(IDS_SURFACE_306);
			}
			break;
		}
		break;

	case TI_EQUILIBRIUM_PHASES:
		switch (this->GridSurfTypes.GetCol())
		{
		case 1:
			strRes.LoadString(IDS_SURFACE_309);
			break;
		case 2:
			strRes.LoadString(IDS_SURFACE_310);
			break;
		case 3:
			strRes.LoadString(IDS_SURFACE_311);
			break;
		case 4:
			strRes.LoadString(IDS_SURFACE_312);
			break;
		}
		break;

	case TI_KINETIC_REACTANTS:
		switch (this->GridSurfTypes.GetCol())
		{
		case 1:
			strRes.LoadString(IDS_SURFACE_309);
			break;

		case 2:
			strRes.LoadString(IDS_SURFACE_310);
			break;

		case 3:
			strRes.LoadString(IDS_SURFACE_311);
			break;

		case 4:
			strRes.LoadString(IDS_SURFACE_314);
			break;
		}
		break;
	}
	m_eInputDesc.SetWindowText(strRes);	
}

void CSurfacePage::EnterCellMshfgSurfaces()
{
	CString strRes;
	switch (TabCtrl_GetCurSel(this->GetDlgItem(IDC_TAB_SURF_TYPE)->GetSafeHwnd()))
	{
	case TI_GENERAL:
		switch (this->GridSurfaces.GetCol())
		{
		case 1:
			strRes.LoadString(IDS_SURFACE_307);
			break;
		case 2:
			strRes.LoadString(IDS_SURFACE_308);
			break;
		case 3:
			strRes.LoadString(IDS_SURFACE_627);
			break;
		case 4:
			strRes.LoadString(IDS_SURFACE_628);
			break;
		}
		break;

	case TI_EQUILIBRIUM_PHASES:
		switch (this->GridSurfaces.GetCol())
		{
		case 1:
			strRes.LoadString(IDS_SURFACE_313);
			break;
		case 2:
			strRes.LoadString(IDS_SURFACE_627);
			break;
		case 3:
			strRes.LoadString(IDS_SURFACE_628);
			break;
		}
		break;

	case TI_KINETIC_REACTANTS:
		switch (this->GridSurfaces.GetCol())
		{
		case 1:
			strRes.LoadString(IDS_SURFACE_313);
			break;
		case 2:
			strRes.LoadString(IDS_SURFACE_627);
			break;
		case 3:
			strRes.LoadString(IDS_SURFACE_628);
			break;
		}
		break;
	}
	m_eInputDesc.SetWindowText(strRes);	
}

LRESULT CSurfacePage::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
{
	TabIndex ti = (TabIndex)TabCtrl_GetCurSel(this->GetDlgItem(IDC_TAB_SURF_TYPE)->GetSafeHwnd());
	switch (ti)
	{
	case TI_GENERAL:
		return this->OnBeginCellEditGen(wParam, lParam);
		break;
	case TI_EQUILIBRIUM_PHASES:
		return this->OnBeginCellEditEqu(wParam, lParam);
		break;
	case TI_KINETIC_REACTANTS:
		return this->OnBeginCellEditKin(wParam, lParam);
		break;
	default:
		ASSERT(FALSE);
		break;
	}
	return FALSE;
}

LRESULT CSurfacePage::OnBeginCellEditGen(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bDenyEdit = FALSE;	// allow edit	

	if ( nID == IDC_MSHFG_SURFTYPE )
	{
		switch (pInfo->item.iCol)
		{
		case 1 :
			if (pInfo->item.hWndCombo)
			{
				CComboBox* pCombo = (CComboBox*)CWnd::FromHandle(pInfo->item.hWndCombo);
				ASSERT_KINDOF(CComboBox, pCombo);	// must be combobox
				ASSERT(::IsWindow(pInfo->item.hWndCombo));
				if (pCombo)
				{
					CDC* pDC = pCombo->GetDC();
					CSize size(0, 0);
					long nWidest = 0;

					int cItems = this->ListCtrlSurface.GetItemCount();
					if (cItems > 0)
					{
						pCombo->ResetContent();
					}
					else
					{
						return bDenyEdit;
					}

					CString str;
					for (int i = 0; i < cItems; ++i)
					{
						str = this->ListCtrlSurface.GetItemText(i, 0);
						pCombo->AddString(str);

						size = pDC->GetTextExtent(str);
						if (nWidest < size.cx )
						{
							nWidest = size.cx;
						}

					}
					pCombo->SetDroppedWidth(nWidest);
					pInfo->item.bUseCombo = (pCombo->GetCount() > 0);
				}
			}
			else
			{
				pInfo->item.bUseCombo = (this->ListCtrlSurface.GetItemCount() > 0);
			}
			break;

		default :
			break;

		}
	}
	return bDenyEdit;
}

LRESULT CSurfacePage::OnBeginCellEditEqu(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bDenyEdit = FALSE;	// allow edit	

	if ( nID == IDC_MSHFG_SURFTYPE )
	{
		switch ( pInfo->item.iCol )
		{
		case 1 :
			if (pInfo->item.hWndCombo)
			{
				CComboBox* pCombo = (CComboBox*)CWnd::FromHandle(pInfo->item.hWndCombo);
				ASSERT_KINDOF(CComboBox, pCombo);	// must be combobox
				ASSERT(::IsWindow(pInfo->item.hWndCombo));
				if (pCombo)
				{
					CDC* pDC = pCombo->GetDC();
					CSize size(0, 0);
					long nWidest = 0;

					int cItems = this->ListCtrlSurface.GetItemCount();
					if (cItems > 0)
					{
						pCombo->ResetContent();
					}
					else
					{
						return bDenyEdit;
					}

					CString str;
					for (int i = 0; i < cItems; ++i)
					{
						str = this->ListCtrlSurface.GetItemText(i, 0);
						pCombo->AddString(str);

						size = pDC->GetTextExtent(str);
						if (nWidest < size.cx )
						{
							nWidest = size.cx;
						}

					}
					pCombo->SetDroppedWidth(nWidest);
					pInfo->item.bUseCombo = (pCombo->GetCount() > 0);
				}
			}
			else
			{
				pInfo->item.bUseCombo = (this->ListCtrlSurface.GetItemCount() > 0);
			}
			break;

		case 2 :
			pInfo->item.bUseCombo = (CUtil::InsertPhases(pInfo->item.hWndCombo, GetDatabase()) > 0);
			break;

		default :
			break;

		}
	}
	return bDenyEdit;
}

LRESULT CSurfacePage::OnBeginCellEditKin(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bDenyEdit = FALSE;	// allow edit	

	if ( nID == IDC_MSHFG_SURFTYPE )
	{
		switch ( pInfo->item.iCol )
		{
		case 1 :
			if (pInfo->item.hWndCombo)
			{
				CComboBox* pCombo = (CComboBox*)CWnd::FromHandle(pInfo->item.hWndCombo);
				ASSERT_KINDOF(CComboBox, pCombo);	// must be combobox
				ASSERT(::IsWindow(pInfo->item.hWndCombo));
				if (pCombo)
				{
					CDC* pDC = pCombo->GetDC();
					CSize size(0, 0);
					long nWidest = 0;

					int cItems = this->ListCtrlSurface.GetItemCount();
					if (cItems > 0)
					{
						pCombo->ResetContent();
					}
					else
					{
						return bDenyEdit;
					}

					CString str;
					for (int i = 0; i < cItems; ++i)
					{
						str = this->ListCtrlSurface.GetItemText(i, 0);
						pCombo->AddString(str);

						size = pDC->GetTextExtent(str);
						if (nWidest < size.cx )
						{
							nWidest = size.cx;
						}
					}
					pCombo->SetDroppedWidth(nWidest);
					pInfo->item.bUseCombo = (pCombo->GetCount() > 0);
				}
			}
			else
			{
				pInfo->item.bUseCombo = (this->ListCtrlSurface.GetItemCount() > 0);
			}
			break;

		case 2 :
			// set to rate list
			pInfo->item.bUseCombo = (CUtil::InsertRates(pInfo->item.hWndCombo, GetDatabase()) > 0);
			break;

		default :
			break;

		}
	}
	return bDenyEdit;
}

LRESULT CSurfacePage::OnSetfocusEG(WPARAM wParam, LPARAM lParam)
{
	lParam = lParam;
	UINT nID = wParam;

	switch (nID)
	{
	case IDC_MSHFG_SURFTYPE :
		this->EnterCellMshfgSurftype();
		break;
	case IDC_MSHFG_SURFACES :
		this->EnterCellMshfgSurfaces();
		break;
	default :
		this->EnterCellMshfgNumDesc();
		break;
	}
	return 0;
}

void CSurfacePage::EnterCellMshfgNumDesc()
{
	CString strRes;
	switch (this->m_egNumDesc.GetRow())
	{
	case 0 :
		AfxFormatString1(strRes, IDS_N_USER_140, _T("surface") ); 
		break;
	case 1 :
		AfxFormatString1(strRes, IDS_N_USER_END_141, _T("surface") ); 
		break;
	case 2 :
		AfxFormatString1(strRes, IDS_DESC_STR_142, _T("surface") ); 
		break;
	default :
		ASSERT(FALSE);
		break;
	}
	this->m_eInputDesc.SetWindowText(strRes);
}

void CSurfacePage::OnLvnItemchangedClSurface(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	pNMLV = pNMLV;
	// verify state of gridlistdoc
// COMMENT: {12/30/2009 3:58:54 PM}	ASSERT( this->GridListDoc.GetCount() == 0 );

	TabIndex ti = (TabIndex)TabCtrl_GetCurSel(this->GetDlgItem(IDC_TAB_SURF_TYPE)->GetSafeHwnd());
	switch (ti)
	{
	case TI_GENERAL:
		this->OnLvnItemchangedClSurfaceGen(pNMHDR, pResult);
		break;
	case TI_EQUILIBRIUM_PHASES:
		this->OnLvnItemchangedClSurfaceEqu(pNMHDR, pResult);
		break;
	case TI_KINETIC_REACTANTS:
		this->OnLvnItemchangedClSurfaceKin(pNMHDR, pResult);
		break;
	default:
		ASSERT(FALSE);
		break;
	}
	*pResult = 0;
}

void CSurfacePage::OnLvnItemchangedClSurfaceGen(NMHDR *pNMHDR, LRESULT *pResult)
{
	// do default
	long row = this->GridListDocGen.OnItemChanged(pNMHDR, pResult);
	SetEGDefaults(row);

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if ((pNMListView->uNewState ^ pNMListView->uOldState) & LVIS_STATEIMAGEMASK)
	{		
		if (this->ListCtrlSurface.GetCheckBox(pNMListView->iItem) == BST_UNCHECKED)
		{
			CString str = this->ListCtrlSurface.GetItemText(pNMListView->iItem, 0);
			this->RemoveSurface(str);
		}
	}
}

void CSurfacePage::OnLvnItemchangedClSurfaceEqu(NMHDR *pNMHDR, LRESULT *pResult)
{
	// do default
	long row = this->GridListDocEqu.OnItemChanged(pNMHDR, pResult);
	SetEGDefaults(row);

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if ((pNMListView->uNewState ^ pNMListView->uOldState) & LVIS_STATEIMAGEMASK)
	{		
		if (this->ListCtrlSurface.GetCheckBox(pNMListView->iItem) == BST_UNCHECKED)
		{
			CString str = this->ListCtrlSurface.GetItemText(pNMListView->iItem, 0);
			RemoveSurface(str);
		}
	}
}

void CSurfacePage::OnLvnItemchangedClSurfaceKin(NMHDR *pNMHDR, LRESULT *pResult)
{
	// do default
	long row = this->GridListDocKin.OnItemChanged(pNMHDR, pResult);
	SetEGDefaults(row);

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if ((pNMListView->uNewState ^ pNMListView->uOldState) & LVIS_STATEIMAGEMASK)
	{		
		if (this->ListCtrlSurface.GetCheckBox(pNMListView->iItem) == BST_UNCHECKED)
		{
			CString str = this->ListCtrlSurface.GetItemText(pNMListView->iItem, 0);
			RemoveSurface(str);
		}
	}
}

void CSurfacePage::SetEGDefaults(long nRow)
{
	TabIndex ti = (TabIndex)TabCtrl_GetCurSel(this->GetDlgItem(IDC_TAB_SURF_TYPE)->GetSafeHwnd());
	switch (ti)
	{
	case TI_GENERAL:
		this->SetEGDefaultsGen(nRow);
		break;
	case TI_EQUILIBRIUM_PHASES:
		this->SetEGDefaultsEqu(nRow);
		break;
	case TI_KINETIC_REACTANTS:
		this->SetEGDefaultsKin(nRow);
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}


void CSurfacePage::SetEGDefaultsGen(long nRow)
{
	if (nRow == -1 ) return;

	ASSERT( nRow >= this->GridSurfTypes.GetFixedRows() );

	if (this->GridSurfTypes.GetTextMatrix(nRow, 2).IsEmpty())
	{
		this->GridSurfTypes.SetTextMatrix(nRow, 2, _T("0.0"));
	}

	long nRow2 = this->AddSurface(this->GridSurfTypes.GetTextMatrix(nRow, 1));

	if (this->GridSurfaces.GetTextMatrix(nRow2, 1).IsEmpty())
	{
		this->GridSurfaces.SetTextMatrix(nRow2, 1, _T("600.0"));
	}
	if (this->GridSurfaces.GetTextMatrix(nRow2, 2).IsEmpty())
	{
		this->GridSurfaces.SetTextMatrix(nRow2, 2, _T("0.0"));
	}
	if (this->GridSurfaces.GetTextMatrix(nRow2, 3).IsEmpty())
	{
		this->GridSurfaces.SetTextMatrix(nRow2, 3, _T("1.0"));
	}
	if (this->GridSurfaces.GetTextMatrix(nRow2, 4).IsEmpty())
	{
		this->GridSurfaces.SetTextMatrix(nRow2, 4, _T("5.0"));
	}

}

void CSurfacePage::SetEGDefaultsEqu(long nRow)
{
	if (nRow == -1 ) return;

	ASSERT( nRow >= this->GridSurfTypes.GetFixedRows() );

	if (this->GridSurfTypes.GetTextMatrix(nRow, 3).IsEmpty())
	{
		this->GridSurfTypes.SetTextMatrix(nRow, 3, _T("0.0"));
	}

	long nRow2 = this->AddSurface(this->GridSurfTypes.GetTextMatrix(nRow, 1));

	if (this->GridSurfaces.GetTextMatrix(nRow2, 1).IsEmpty())
	{
		this->GridSurfaces.SetTextMatrix(nRow2, 1, _T("0.0"));
	}
	if (this->GridSurfaces.GetTextMatrix(nRow2, 2).IsEmpty())
	{
		this->GridSurfaces.SetTextMatrix(nRow2, 2, _T("1.0"));
	}
	if (this->GridSurfaces.GetTextMatrix(nRow2, 3).IsEmpty())
	{
		this->GridSurfaces.SetTextMatrix(nRow2, 3, _T("5.0"));
	}
}

void CSurfacePage::SetEGDefaultsKin(long nRow)
{
	if (nRow == -1 ) return;

	ASSERT( nRow >= this->GridSurfTypes.GetFixedRows() );

	if (this->GridSurfTypes.GetTextMatrix(nRow, 3).IsEmpty())
	{
		this->GridSurfTypes.SetTextMatrix(nRow, 3, _T("0.0"));
	}

	long nRow2 = this->AddSurface(this->GridSurfTypes.GetTextMatrix(nRow, 1));

	if (this->GridSurfaces.GetTextMatrix(nRow2, 1).IsEmpty())
	{
		this->GridSurfaces.SetTextMatrix(nRow2, 1, _T("0.0"));
	}
	if (this->GridSurfaces.GetTextMatrix(nRow2, 2).IsEmpty())
	{
		this->GridSurfaces.SetTextMatrix(nRow2, 2, _T("1.0"));
	}
	if (this->GridSurfaces.GetTextMatrix(nRow2, 3).IsEmpty())
	{
		this->GridSurfaces.SetTextMatrix(nRow2, 3, _T("5.0"));
	}
}

void CSurfacePage::ValidateEGSurfaces(CDataExchange* pDX)
{
	TabIndex ti = (TabIndex)TabCtrl_GetCurSel(this->GetDlgItem(IDC_TAB_SURF_TYPE)->GetSafeHwnd());
	switch (ti)
	{
	case TI_GENERAL:
		this->ValidateEGSurfacesGen(pDX);
		break;
	case TI_EQUILIBRIUM_PHASES:
		this->ValidateEGSurfacesEqu(pDX);
		break;
	case TI_KINETIC_REACTANTS:
		this->ValidateEGSurfacesKin(pDX);
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}

void CSurfacePage::ValidateEGSurfacesGen(CDataExchange* pDX)
{
	if (CButton* pButton = (CButton*)this->GetDlgItem(IDC_CHECK_SITE_DENSITY))
	{
		if (pButton->GetCheck() == BST_CHECKED)
		{
			this->sites_units = SITES_DENSITY;
		}
		else
		{
			this->sites_units = SITES_ABSOLUTE;
		}
	}

	if (CButton *pBtn = (CButton*)this->GetDlgItem(IDC_CHECK_MOBILE))
	{
		if (pBtn->GetCheck() == BST_CHECKED)
		{
			this->transport = TRUE;
		}
		else
		{
			this->transport = FALSE;
		}
	}

	CString strDummy;
	for (long nRow = 1; nRow < this->GridSurfaces.GetRows(); ++nRow)
	{
		// Line 2: surface binding-site name, sites, specific_area_per_gram, mass
		strDummy = this->GridSurfaces.GetTextMatrix(nRow, 0);
		if (!strDummy.IsEmpty())
		{			
			// Read surface area (m^2/g)
			DDX_GridText(pDX, this->GridSurfaces.GetDlgCtrlID(), nRow, 1, strDummy);
			if (strDummy.IsEmpty())
			{
				DDX_GridFail(pDX, _T("Expected specific area in m^2/g."), _T("Invalid specific area"));
			}
			double dArea;
			DDX_GridText(pDX, this->GridSurfaces.GetDlgCtrlID(), nRow, 1, dArea);
			if (dArea <= 0.0)
			{
				DDX_GridFail(pDX, _T("Specific area must be greater than 0."), _T("Invalid specific area"));
			}

			// Read grams of solid (g)
			double dGrams;
			DDX_GridText(pDX, this->GridSurfaces.GetDlgCtrlID(), nRow, 2, strDummy);
			if (strDummy.IsEmpty())
			{
				DDX_GridFail(pDX, _T("Expected mass of solid in grams."), _T("Invalid mass"));
			}
			DDX_GridText(pDX, this->GridSurfaces.GetDlgCtrlID(), nRow, 2, dGrams);

			if (dGrams <= 0.0)
			{
				DDX_GridFail(pDX, _T("Mass of solid must be greater than 0."), _T("Invalid mass"));
			}

			if (this->type == CD_MUSIC)
			{
				// this assumes capacitance is req'd for -cd_music
				double dCap;

				DDX_GridText(pDX, this->GridSurfaces.GetDlgCtrlID(), nRow, 3, strDummy);
				if (strDummy.IsEmpty())
				{
					DDX_GridFail(pDX, _T("Expected capacitance for the 0-1 planes."), _T("Invalid capacitance"));
				}
				DDX_GridText(pDX, this->GridSurfaces.GetDlgCtrlID(), nRow, 3, dCap);

				DDX_GridText(pDX, this->GridSurfaces.GetDlgCtrlID(), nRow, 4, strDummy);
				if (strDummy.IsEmpty())
				{
					DDX_GridFail(pDX, _T("Expected capacitance for the 1-2 planes."), _T("Invalid capacitance"));
				}
				DDX_GridText(pDX, this->GridSurfaces.GetDlgCtrlID(), nRow, 4, dCap);
			}

			if (this->transport)
			{
				double dDw;

				DDX_GridText(pDX, this->GridSurfaces.GetDlgCtrlID(), nRow, 5, strDummy);
				if (strDummy.IsEmpty())
				{
					DDX_GridFail(pDX, _T("Expected surface diffusion coefficient (m2/s)."), _T("Invalid Dw"));
				}
				DDX_GridText(pDX, this->GridSurfaces.GetDlgCtrlID(), nRow, 5, dDw);
			}
		}
	}
}

void CSurfacePage::ValidateEGSurfacesEqu(CDataExchange* pDX)
{
	CString strDummy;

	for (long row = 1; row < this->GridSurfaces.GetRows(); ++row)
	{
		strDummy = this->GridSurfaces.GetTextMatrix(row, 0);
		// Note: grid trims white space on entry
		if ( !strDummy.IsEmpty() )
		{
			// Read surface area (m^2/g)
			double dArea;
			DDX_GridText(pDX, this->GridSurfaces.GetDlgCtrlID(), row, 1, strDummy);
			if (strDummy.IsEmpty())
			{
				DDX_GridFail(pDX, "Expected specific area in m^2/g.", "Invalid specific area");
			}
			DDX_GridText(pDX, this->GridSurfaces.GetDlgCtrlID(), row, 1, dArea);
			if (dArea <= 0.0)
			{
				DDX_GridFail(pDX, "Specific area must be greater than 0.", "Invalid specific area");
			}

			if (this->type == CD_MUSIC)
			{
				// this assumes capacitance is req'd for -cd_music
				double dCap;

				DDX_GridText(pDX, this->GridSurfaces.GetDlgCtrlID(), row, 2, strDummy);
				if (strDummy.IsEmpty())
				{
					DDX_GridFail(pDX, _T("Expected capacitance for the 0-1 planes."), _T("Invalid capacitance"));
				}
				DDX_GridText(pDX, this->GridSurfaces.GetDlgCtrlID(), row, 2, dCap);

				DDX_GridText(pDX, this->GridSurfaces.GetDlgCtrlID(), row, 3, strDummy);
				if (strDummy.IsEmpty())
				{
					DDX_GridFail(pDX, _T("Expected capacitance for the 1-2 planes."), _T("Invalid capacitance"));
				}
				DDX_GridText(pDX, this->GridSurfaces.GetDlgCtrlID(), row, 3, dCap);
			}
		}
	}
}

void CSurfacePage::ValidateEGSurfacesKin(CDataExchange* pDX)
{
	CString strDummy;
	for (long nRow = 1; nRow < this->GridSurfaces.GetRows(); ++nRow)
	{
		strDummy = this->GridSurfaces.GetTextMatrix(nRow, 0);
		// Note: grid trims white space on entry
		if ( !strDummy.IsEmpty() )
		{		
			// Read surface area (m^2/g)
			double dArea;
			DDX_GridText(pDX, this->GridSurfaces.GetDlgCtrlID(), nRow, 1, strDummy);
			if (strDummy.IsEmpty())
			{
				DDX_GridFail(pDX, _T("Expected specific area in m^2/g."), _T("Invalid specific area"));
			}
			DDX_GridText(pDX, this->GridSurfaces.GetDlgCtrlID(), nRow, 1, dArea);
			if (dArea <= 0.0)
			{
				DDX_GridFail(pDX, _T("Specific area must be greater than 0."), _T("Invalid specific area"));
			}

			if (this->type == CD_MUSIC)
			{
				// this assumes capacitance is req'd for -cd_music
				double dCap;

				DDX_GridText(pDX, this->GridSurfaces.GetDlgCtrlID(), nRow, 2, strDummy);
				if (strDummy.IsEmpty())
				{
					DDX_GridFail(pDX, _T("Expected capacitance for the 0-1 planes."), _T("Invalid capacitance"));
				}
				DDX_GridText(pDX, this->GridSurfaces.GetDlgCtrlID(), nRow, 2, dCap);

				DDX_GridText(pDX, this->GridSurfaces.GetDlgCtrlID(), nRow, 3, strDummy);
				if (strDummy.IsEmpty())
				{
					DDX_GridFail(pDX, _T("Expected capacitance for the 1-2 planes."), _T("Invalid capacitance"));
				}
				DDX_GridText(pDX, this->GridSurfaces.GetDlgCtrlID(), nRow, 3, dCap);
			}
		}
	}
}

void CSurfacePage::ValidateEGSurfTypes(CDataExchange* pDX)
{
	TabIndex ti = (TabIndex)TabCtrl_GetCurSel(this->GetDlgItem(IDC_TAB_SURF_TYPE)->GetSafeHwnd());
	switch (ti)
	{
	case TI_GENERAL:
		this->ValidateEGSurfTypesGen(pDX);
		break;
	case TI_EQUILIBRIUM_PHASES:
		this->ValidateEGSurfTypesEqu(pDX);
		break;
	case TI_KINETIC_REACTANTS:
		this->ValidateEGSurfTypesKin(pDX);
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}

void CSurfacePage::ValidateEGSurfTypesGen(CDataExchange* pDX)
{
	std::list<CSurfComp> listSurfComp;

	CString strDummy;
	for (long row = 1; row < this->GridSurfTypes.GetRows(); ++row)
	{
		// Line 2: surface binding-site name, sites, specific_area_per_gram, mass

		DDX_GridText(pDX, this->GridSurfTypes.GetDlgCtrlID(), row, 1, strDummy);

		// Note: grid trims white space on entry
		if ( !strDummy.IsEmpty() )
		{
			CSurfComp loSurfComp;

			// surface binding-site name
			if (!CUtil::IsValidSurfaceName(strDummy))
			{
				DDX_GridFail(pDX, _T("Expected surface name to begin with a capital letter."), _T("Invalid surface name"));
			}
			loSurfComp.m_strFormula = strDummy;

			// sites 
			// Read surface concentration
			DDX_GridText(pDX, this->GridSurfTypes.GetDlgCtrlID(), row, 2, strDummy);
			if (strDummy.IsEmpty())
			{
				DDX_GridFail(pDX, _T("Expected number of surface sites in moles.."), _T("Invalid surface sites"));
			}
			DDX_GridText(pDX, this->GridSurfTypes.GetDlgCtrlID(), row, 2, loSurfComp.m_dMoles);


			// Read surface area (m^2/g)
			long nRow2 = GetSurface(loSurfComp.m_strFormula);
			ASSERT( nRow2 != -1 );
			// validated in ValidateEGSurfaces()
			DDX_GridText(pDX, this->GridSurfaces.GetDlgCtrlID(), nRow2, 1, loSurfComp.m_dSpecific_area);


			// Read grams of solid (g)
			// validated in ValidateEGSurfaces()
			DDX_GridText(pDX, this->GridSurfaces.GetDlgCtrlID(), nRow2, 2, loSurfComp.m_dGrams);

			if (this->type == CD_MUSIC)
			{
				// this assumes capacitance is req'd for -cd_music
				// validated in ValidateEGSurfaces()
				DDX_GridText(pDX, this->GridSurfaces.GetDlgCtrlID(), nRow2, 3, loSurfComp.m_dCapacitance0);
				DDX_GridText(pDX, this->GridSurfaces.GetDlgCtrlID(), nRow2, 4, loSurfComp.m_dCapacitance1);
			}

			// Read surface diffusion coefficient (m2/s)
			// validated in ValidateEGSurfaces()
			if (this->transport)
			{
				DDX_GridText(pDX, this->GridSurfaces.GetDlgCtrlID(), nRow2, 5, strDummy);
				loSurfComp.m_dDw = 0.;
				if (!strDummy.IsEmpty())
				{
					DDX_GridText(pDX, this->GridSurfaces.GetDlgCtrlID(), nRow2, 5, loSurfComp.m_dDw);
				}
			}

			listSurfComp.push_back(loSurfComp);
		}
	}
	this->ListSurfComp[CSurfacePage::TI_GENERAL].assign(listSurfComp.begin(), listSurfComp.end());
}

void CSurfacePage::ValidateEGSurfTypesEqu(CDataExchange* pDX)
{
	CString strDummy;

	std::list<CSurfComp> listSurfComp;

	ASSERT( this->GridSurfTypes.GetSafeHwnd() != NULL );
	
	for (long nRow = this->GridSurfTypes.GetFixedRows(); nRow < this->GridSurfTypes.GetRows(); ++nRow)
	{
		// Line 2: exchange formula, name, [(equilibrium_phase or kinetic_reactant)], exchange_per_mole
		DDX_GridText(pDX, this->GridSurfTypes.GetDlgCtrlID(), nRow, 1, strDummy);

		if ( !strDummy.IsEmpty() )
		{
			CSurfComp loSurfComp;

			// surface binding-site name
			if (!CUtil::IsValidSurfaceName(strDummy))
			{
				DDX_GridFail(pDX, _T("Expected surface name to begin with a capital letter."), _T("Invalid surface name"));
			}
			loSurfComp.m_strFormula = strDummy;

			// mineral name
			DDX_GridText(pDX, this->GridSurfTypes.GetDlgCtrlID(), nRow, 2, loSurfComp.m_strPhase_name);
			if (loSurfComp.m_strPhase_name.IsEmpty())
			{
				DDX_GridFail(pDX, _T("Expected mineral name."), _T("Invalid mineral name"));
			}

			// read proportion
			// str = this->GridSurfTypes.GetTextMatrix(row, 3);
			DDX_GridText(pDX, this->GridSurfTypes.GetDlgCtrlID(), nRow, 3, strDummy);
			if (strDummy.IsEmpty())
			{
				DDX_GridFail(pDX, _T("Expected a coefficient to relate surface to mineral."), _T("Missing ratio"));
			}
			DDX_GridText(pDX, this->GridSurfTypes.GetDlgCtrlID(), nRow, 3, loSurfComp.m_dPhase_proportion);

			// Read surface area (m^2/g)
			long nRow2 = GetSurface(loSurfComp.m_strFormula);
			// validated in ValidateEGSurfaces()
			ASSERT( nRow2 != -1 );
			DDX_GridText(pDX, this->GridSurfaces.GetDlgCtrlID(), nRow2, 1, loSurfComp.m_dSpecific_area);

			if (this->type == CD_MUSIC)
			{
				// this assumes capacitance is req'd for -cd_music
				// validated in ValidateEGSurfaces()
				DDX_GridText(pDX, this->GridSurfaces.GetDlgCtrlID(), nRow2, 2, loSurfComp.m_dCapacitance0);
				DDX_GridText(pDX, this->GridSurfaces.GetDlgCtrlID(), nRow2, 3, loSurfComp.m_dCapacitance1);
			}

			// all ok add to list
			listSurfComp.push_back(loSurfComp);			
		}
	}

	this->ListSurfComp[CSurfacePage::TI_EQUILIBRIUM_PHASES].assign(listSurfComp.begin(), listSurfComp.end());
}

void CSurfacePage::ValidateEGSurfTypesKin(CDataExchange* pDX)
{
	CString strDummy;

	std::list<CSurfComp> listSurfComp;

	ASSERT( this->GridSurfTypes.GetSafeHwnd() != NULL );
	
	for (long nRow = this->GridSurfTypes.GetFixedRows(); nRow < this->GridSurfTypes.GetRows(); ++nRow)
	{
		// Line 2: exchange formula, name, [(equilibrium_phase or kinetic_reactant)], exchange_per_mole
		DDX_GridText(pDX, this->GridSurfTypes.GetDlgCtrlID(), nRow, 1, strDummy);
		if (!strDummy.IsEmpty())
		{
			CSurfComp loSurfComp;

			// surface binding-site name
			if (!CUtil::IsValidSurfaceName(strDummy))
			{
				DDX_GridFail(pDX, _T("Expected surface name to begin with a capital letter."), _T("Invalid surface name"));
			}
			loSurfComp.m_strFormula = strDummy;

			// rate name
			DDX_GridText(pDX, this->GridSurfTypes.GetDlgCtrlID(), nRow, 2, loSurfComp.m_strRate_name);
			if (loSurfComp.m_strRate_name.IsEmpty())
			{
				DDX_GridFail(pDX, _T("Expected kinetic reaction name."), _T("Invalid kinetic reaction name"));
			}

			// read proportion
			DDX_GridText(pDX, this->GridSurfTypes.GetDlgCtrlID(), nRow, 3, strDummy);
			if (strDummy.IsEmpty())
			{
				DDX_GridFail(pDX, _T("Expected a coefficient to relate surface to kinetic reaction."), _T("Missing ratio"));
			}
			DDX_GridText(pDX, this->GridSurfTypes.GetDlgCtrlID(), nRow, 3, loSurfComp.m_dPhase_proportion);

			// Read surface area (m^2/g)
			long nRow2 = GetSurface(loSurfComp.m_strFormula);
			// validated in ValidateEGSurfaces()
			ASSERT( nRow2 != -1 );
			DDX_GridText(pDX, this->GridSurfaces.GetDlgCtrlID(), nRow2, 1, loSurfComp.m_dSpecific_area);

			if (this->type == CD_MUSIC)
			{
				// this assumes capacitance is req'd for -cd_music
				// validated in ValidateEGSurfaces()
				DDX_GridText(pDX, this->GridSurfaces.GetDlgCtrlID(), nRow2, 2, loSurfComp.m_dCapacitance0);
				DDX_GridText(pDX, this->GridSurfaces.GetDlgCtrlID(), nRow2, 3, loSurfComp.m_dCapacitance1);
			}

			// all ok add to list
			listSurfComp.push_back(loSurfComp);
		}
	}
	this->ListSurfComp[CSurfacePage::TI_KINETIC_REACTANTS].assign(listSurfComp.begin(), listSurfComp.end());
}

BOOL CSurfacePage::OnInitDialog()
{
	CCommonKeywordPage::OnInitDialog();

	// set layout
	CreateRoot(VERTICAL, 5, 6) 
		<< (pane(HORIZONTAL, ABSOLUTE_VERT)
			<< item(IDC_MSHFG_NUM_DESC, ABSOLUTE_VERT)
			)
		<< (pane(HORIZONTAL, GREEDY)
			<< item(IDC_EQUILIBRATE, NORESIZE)
			<< item(IDC_CB_SOLUTIONS, NORESIZE)
			)
		<< itemFixed(VERTICAL, 15)
		<< (paneCtrl(IDC_GB_SCM, VERTICAL, GREEDY, nDefaultBorder, 10, 10, 10)
			<< (pane(HORIZONTAL, ABSOLUTE_VERT)
				<< item(IDC_RADIO_NO_EDL2, ABSOLUTE_HORZ)
				<< item(IDC_RADIO_DDL2, ABSOLUTE_HORZ)
				<< item(IDC_RADIO_CD_MUSIC2, ABSOLUTE_HORZ)
				)
			<< (paneCtrl(IDC_GB_DLO, HORIZONTAL, GREEDY, nDefaultBorder, 10, 10, 10)
				<< (paneCtrl(IDC_GB_DLO_NONE, VERTICAL, ABSOLUTE_VERT, nDefaultBorder, 10, 10, 10)
					<< item(IDC_BUTTON5, ABSOLUTE_VERT)
					)
				<< (paneCtrl(IDC_GB_DLO_DONNAN, VERTICAL, ABSOLUTE_VERT, nDefaultBorder, 10, 10, 10)
					<< (pane(VERTICAL, ABSOLUTE_VERT)
						<< (pane(HORIZONTAL, ABSOLUTE_HORZ)
							<< item(IDC_RADIO_THICK, NORESIZE | ALIGN_VCENTER)
							<< item(IDC_EDIT_DONNAN_THICKNESS, GREEDY | ALIGN_VCENTER)
							)
						<< (pane(HORIZONTAL, ABSOLUTE_HORZ)
							<< item(IDC_RADIO_DEBYE, NORESIZE | ALIGN_VCENTER)
							<< item(IDC_EDIT_DEBYE_LENGTH, GREEDY | ALIGN_VCENTER)
							<< item(IDC_ST_LIMIT_DDL, NORESIZE | ALIGN_VCENTER)
							<< item(IDC_EDIT_DEB_LEN_LIMIT, GREEDY | ALIGN_VCENTER)
							)
						<< item(IDC_CHECK_DONNAN_CIO, NORESIZE)
						<< (pane(HORIZONTAL, ABSOLUTE_HORZ)
							<< item(IDC_CHECK_RETARD2, NORESIZE | ALIGN_VCENTER)
							<< item(IDC_ST_VISCOSITY, NORESIZE | ALIGN_VCENTER)
							<< item(IDC_EDIT_VISCOSITY, NORESIZE | ALIGN_VCENTER)
							)
						)
					)
				<< (paneCtrl(IDC_GB_DLO_BORK, VERTICAL, ABSOLUTE_VERT, nDefaultBorder, 10, 10, 10) // , 10, 30, 10)
					<< (pane(HORIZONTAL, ABSOLUTE_HORZ)
						<< item(IDC_ST_BORK_THICKNESS, NORESIZE | ALIGN_VCENTER)
						<< item(IDC_EDIT_BORK_THICKNESS, ABSOLUTE_VERT | ALIGN_VCENTER)
						)
					<< item(IDC_CHECK_BORK_CIO, NORESIZE)
					)
				)
			)
		<< (pane(HORIZONTAL, GREEDY)
			<< (paneCtrl(IDC_TAB_SURF_TYPE, HORIZONTAL, GREEDY, nDefaultBorder, 10, 19, 10)
				<< (pane(VERTICAL, GREEDY)
				//<< (pane(VERTICAL, RELATIVE_HORZ, 20)
					//<< itemSpaceLike(VERTICAL, IDC_CHECK_SITE_DENSITY)
					<< item(IDC_CL_SURFACE, ABSOLUTE_HORZ)
					)
				<< (pane(VERTICAL, GREEDY)
				//<< (pane(VERTICAL, RELATIVE_HORZ, 40)
					<< item(IDC_CHECK_SITE_DENSITY, NORESIZE | ALIGN_VCENTER)
					<< item(IDC_MSHFG_SURFTYPE, RELATIVE_HORZ, 30)
					)
				<< (pane(VERTICAL, GREEDY)
				//<< (pane(VERTICAL, RELATIVE_HORZ, 40)
					<< item(IDC_CHECK_MOBILE, NORESIZE | ALIGN_VCENTER)
					<< item(IDC_MSHFG_SURFACES, RELATIVE_HORZ, 70)
					)
				)
			<< (pane(VERTICAL, GREEDY)
				<< item(IDC_ST_DEFINED, NORESIZE)
				<< item(IDC_LV_DEFINED, ABSOLUTE_HORZ)
				)
			)
		<< (paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, ABSOLUTE_VERT, nDefaultBorder, 10, 10)
			<< item(IDC_E_DESC_INPUT, ABSOLUTE_VERT)
			)
	;

	UpdateLayout();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSurfacePage::OnSize(UINT nType, int cx, int cy)
{
	CCommonKeywordPage::OnSize(nType, cx, cy);

	const int x_offset = 7;
	const int y_offset = 0;

	// No explicit diffuse layer
	if (CWnd *pWnd = this->GetDlgItem(IDC_RADIO_DL_TYPE_N))
	{
		CRect rc;
		this->GetDlgItem(IDC_GB_DLO_NONE)->GetWindowRect(&rc);
		this->ScreenToClient(&rc);

		CRect rcWnd;
		pWnd->GetWindowRect(&rcWnd);
		this->ScreenToClient(&rcWnd);

		int h = rcWnd.Height();
		int w = rcWnd.Width();

		rcWnd.top    = rc.top;
		rcWnd.bottom = rc.top + h;
		rcWnd.left   = rc.left;
		rcWnd.right  = rc.left + w;

		rcWnd.OffsetRect(x_offset, y_offset);

		pWnd->MoveWindow(&rcWnd);
	}

	// Donnan diffuse layer
	if (CWnd *pWnd = this->GetDlgItem(IDC_RADIO_DL_TYPE_D))
	{
		CRect rc;
		this->GetDlgItem(IDC_GB_DLO_DONNAN)->GetWindowRect(&rc);
		this->ScreenToClient(&rc);

		CRect rcWnd;
		pWnd->GetWindowRect(&rcWnd);
		this->ScreenToClient(&rcWnd);

		int h = rcWnd.Height();
		int w = rcWnd.Width();

		rcWnd.top    = rc.top;
		rcWnd.bottom = rc.top + h;
		rcWnd.left   = rc.left;
		rcWnd.right  = rc.left + w;

		rcWnd.OffsetRect(x_offset, y_offset);

		pWnd->MoveWindow(&rcWnd);
	}

	// Borkovec diffuse layer
	if (CWnd *pWnd = this->GetDlgItem(IDC_RADIO_DL_TYPE_B))
	{
		CRect rc;
		this->GetDlgItem(IDC_GB_DLO_BORK)->GetWindowRect(&rc);
		this->ScreenToClient(&rc);

		CRect rcWnd;
		pWnd->GetWindowRect(&rcWnd);
		this->ScreenToClient(&rcWnd);

		int h = rcWnd.Height();
		int w = rcWnd.Width();

		rcWnd.top    = rc.top;
		rcWnd.bottom = rc.top + h;
		rcWnd.left   = rc.left;
		rcWnd.right  = rc.left + w;

		rcWnd.OffsetRect(x_offset, y_offset);

		pWnd->MoveWindow(&rcWnd);
	}
}

void CSurfacePage::OnBnClickedCheckSiteDensity()
{
	if (CButton* pButton = (CButton*)this->GetDlgItem(IDC_CHECK_SITE_DENSITY))
	{
		if (pButton->GetCheck() == BST_CHECKED)
		{
			this->GridSurfTypes.SetTextMatrix(0, 2, _T("D(sites/nm^2)"));
		}
		else
		{
			this->GridSurfTypes.SetTextMatrix(0, 2, _T("Sites (moles)"));
		}
	}
}

void CSurfacePage::OnBnSetfocusCheckEquilibrate()
{
	// in order for a check box to recieve this notification
	// it must have the notify style set
	CString strRes;
	strRes.LoadString(IDS_SURFACE_304);
	m_eInputDesc.SetWindowText(strRes);
}

void CSurfacePage::OnBnSetfocusCheckDonnanCio()
{
	// in order to recieve this notification
	// it must have the notify style set
	CString strRes;
	strRes.LoadString(IDS_STRING689);
	m_eInputDesc.SetWindowText(strRes);
}

void CSurfacePage::OnBnSetfocusCheckBorkCio()
{
	// in order to recieve this notification
	// it must have the notify style set
	CString strRes;
	strRes.LoadString(IDS_STRING689);
	m_eInputDesc.SetWindowText(strRes);
}

void CSurfacePage::OnNMSetfocusClSurface(NMHDR *pNMHDR, LRESULT *pResult)
{
	UNREFERENCED_PARAMETER( pNMHDR );

	CString strRes;
	strRes.LoadString(IDS_STRING481);
	m_eInputDesc.SetWindowText(strRes);
	
	// set default selection
	if (this->ListCtrlSurface.GetFirstSelectedItemPosition() == NULL)
	{
		this->ListCtrlSurface.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}
	
	*pResult = 0;
}

void CSurfacePage::OnEnSetfocusEditDonnanThickness()
{
	// Add your control notification handler code here
	CString strRes;
	strRes.LoadString(IDS_STRING485);
	m_eInputDesc.SetWindowText(strRes);	
}

void CSurfacePage::OnBnClickedCheckMobile()
{
	if (CButton *pBtn = (CButton*)this->GetDlgItem(IDC_CHECK_MOBILE))
	{
		if (pBtn->GetCheck() == BST_CHECKED)
		{
			this->GridSurfaces.SetColWidth(5, 0, 800);
		}
		else
		{
			this->GridSurfaces.SetColWidth(5, 0, 0);
		}
	}
}

void CSurfacePage::KeyDownMshfgSurftype(short* KeyCode, short Shift)
{
	UNREFERENCED_PARAMETER(Shift);

	int iStartRow;
	int cSelectedRows;

	CGridListDoc *pDoc = 0;

	switch (this->TabCtrlSurfType.GetCurSel())
	{
	case TI_GENERAL:
		pDoc = &this->GridListDocGen;
		break;
	case TI_EQUILIBRIUM_PHASES:
		pDoc = &this->GridListDocEqu;
		break;
	case TI_KINETIC_REACTANTS:
		pDoc = &this->GridListDocKin;
		break;
	}
	ASSERT(pDoc);
	if (!pDoc) return;

	switch (*KeyCode)
	{
	case VK_DELETE :
		// check if col 1 is included
		if (min(this->GridSurfTypes.GetCol(), this->GridSurfTypes.GetColSel()) == 1)
		{
			// determine how many rows to delete
			cSelectedRows = abs(this->GridSurfTypes.GetRow() - this->GridSurfTypes.GetRowSel()) + 1;

			// determine first row to delete
			iStartRow = min(this->GridSurfTypes.GetRow(), this->GridSurfTypes.GetRowSel());

			// delete each row
			for (int i = 0; i < cSelectedRows; ++i)
			{
				CString strRemove = this->GridSurfTypes.GetTextMatrix(iStartRow, 1);
				pDoc->DeleteRow(iStartRow);
				RemoveSurface(strRemove);
			}

		}
		else
		{
			// just fill with empty strings
			this->GridSurfTypes.SetFillStyle(flexFillRepeat);
			this->GridSurfTypes.SetText(_T(""));
			this->GridSurfTypes.SetFillStyle(flexFillSingle);
		}
		break;
	}	
}

void CSurfacePage::KeyDownMshfgSurfaces(short* KeyCode, short Shift)
{
	UNREFERENCED_PARAMETER(Shift);

	switch (*KeyCode)
	{
	case VK_DELETE :
		// just fill with empty strings
		this->GridSurfaces.SetFillStyle(flexFillRepeat);
		this->GridSurfaces.SetText(_T(""));
		this->GridSurfaces.SetFillStyle(flexFillSingle);
		break;
	}	
}

void CSurfacePage::UpdateListCtrlDefined(void)
{
	CString type[3] = { _T("General"), _T("Equilibrium"), _T("Kinetic" )};

	int i = 0;
	std::list<CSurfComp>::const_iterator const_iter;

	const_iter = this->ListSurfComp[CSurfacePage::TI_GENERAL].begin();
	for (; const_iter != this->ListSurfComp[CSurfacePage::TI_GENERAL].end(); ++const_iter, ++i)
	{		
		this->ListCtrlDefined.InsertItem(i, (*const_iter).m_strFormula);
		this->ListCtrlDefined.SetItemText(i, 1, type[CSurfacePage::TI_GENERAL]);
	}

	const_iter = this->ListSurfComp[CSurfacePage::TI_EQUILIBRIUM_PHASES].begin();
	for (; const_iter != this->ListSurfComp[CSurfacePage::TI_EQUILIBRIUM_PHASES].end(); ++const_iter, ++i)
	{		
		this->ListCtrlDefined.InsertItem(i, (*const_iter).m_strFormula);
		this->ListCtrlDefined.SetItemText(i, 1, type[CSurfacePage::TI_EQUILIBRIUM_PHASES]);
	}

	const_iter = this->ListSurfComp[CSurfacePage::TI_KINETIC_REACTANTS].begin();
	for (; const_iter != this->ListSurfComp[CSurfacePage::TI_KINETIC_REACTANTS].end(); ++const_iter, ++i)
	{		
		this->ListCtrlDefined.InsertItem(i, (*const_iter).m_strFormula);
		this->ListCtrlDefined.SetItemText(i, 1, type[CSurfacePage::TI_KINETIC_REACTANTS]);
	}
}

void CSurfacePage::OnBnSetfocusRadioNoEdl2()
{
	// in order for a check box to recieve this notification
	// it must have the notify style set
	CString strRes;
	strRes.LoadString(IDS_STRING482);
	m_eInputDesc.SetWindowText(strRes);
}

void CSurfacePage::OnBnSetfocusRadioDdl2()
{
}

void CSurfacePage::OnBnSetfocusRadioCdMusic2()
{
	// in order for a check box to recieve this notification
	// it must have the notify style set
	CString strRes;
	strRes.LoadString(IDS_STRING683);
	m_eInputDesc.SetWindowText(strRes);
}

void CSurfacePage::OnBnSetfocusRadioDlTypeD()
{
	// in order for a check box to recieve this notification
	// it must have the notify style set
	CString strRes;
	strRes.LoadString(IDS_STRING684);
	m_eInputDesc.SetWindowText(strRes);
}

void CSurfacePage::OnBnSetfocusRadioThick()
{
	// in order for a check box to recieve this notification
	// it must have the notify style set
	CString strRes;
	strRes.LoadString(IDS_STRING686);
	m_eInputDesc.SetWindowText(strRes);
}

void CSurfacePage::OnBnSetfocusRadioDebye()
{
	// in order for a check box to recieve this notification
	// it must have the notify style set
	CString strRes;
	strRes.LoadString(IDS_STRING685);
	m_eInputDesc.SetWindowText(strRes);
}

void CSurfacePage::OnEnSetfocusEditDebyeLength()
{
	CString strRes;
	strRes.LoadString(IDS_STRING687);
	m_eInputDesc.SetWindowText(strRes);
}

void CSurfacePage::OnEnSetfocusEditDebLenLimit()
{
	CString strRes;
	strRes.LoadString(IDS_STRING688);
	m_eInputDesc.SetWindowText(strRes);
}

void CSurfacePage::OnEnSetfocusEditViscosity()
{
	CString strRes;
	strRes.LoadString(IDS_STRING690);
	m_eInputDesc.SetWindowText(strRes);
}

void CSurfacePage::OnBnSetfocusCheckRetard2()
{
	CString strRes;
	strRes.LoadString(IDS_STRING691);
	m_eInputDesc.SetWindowText(strRes);
}

BOOL CSurfacePage::OnHelpInfo(HELPINFO* pHelpInfo)
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
	case IDC_CHECK_EQUILIBRATE:
		strRes.LoadString(IDS_SURFACE_304);
		break;
	case IDC_CBO_SOLUTIONS:
		strRes.LoadString(IDS_SURFACE_303);
		break;
	case IDC_RADIO_NO_EDL2:
		strRes.LoadString(IDS_STRING482);
		break;
	case IDC_RADIO_DDL2:
		//strRes.LoadString(XXXX);
		break;
	case IDC_RADIO_CD_MUSIC2:
		strRes.LoadString(IDS_STRING683);
		break;
	case IDC_RADIO_DL_TYPE_N:
		//strRes.LoadString(XXXX);
		break;
	case IDC_RADIO_DL_TYPE_D:
		strRes.LoadString(IDS_STRING684);
		break;
	case IDC_RADIO_DL_TYPE_B:
		//strRes.LoadString(XXXX);
		break;
	case IDC_RADIO_THICK:
		strRes.LoadString(IDS_STRING686);
		break;
	case IDC_EDIT_DONNAN_THICKNESS:
		strRes.LoadString(IDS_STRING485);
		break;
	case IDC_RADIO_DEBYE:
		strRes.LoadString(IDS_STRING685);
		break;
	case IDC_EDIT_DEBYE_LENGTH:
		strRes.LoadString(IDS_STRING687);
		break;
	case IDC_ST_LIMIT_DDL: case IDC_EDIT_DEB_LEN_LIMIT:
		strRes.LoadString(IDS_STRING688);
		break;
	case IDC_CHECK_DONNAN_CIO:
		strRes.LoadString(IDS_STRING689);
		break;
	case IDC_CHECK_RETARD2:
		strRes.LoadString(IDS_STRING691);
		break;
	case IDC_ST_VISCOSITY: case IDC_EDIT_VISCOSITY:
		strRes.LoadString(IDS_STRING690);
		break;
	case IDC_ST_BORK_THICKNESS: case IDC_EDIT_BORK_THICKNESS:
		strRes.LoadString(IDS_STRING694);
		break;
	case IDC_CHECK_BORK_CIO:
		strRes.LoadString(IDS_STRING689);
		break;
	case IDC_CHECK_SITE_DENSITY:
		strRes.LoadString(IDS_STRING692);
		break;
	case IDC_CHECK_MOBILE:
		strRes.LoadString(IDS_STRING693);
		break;

	case IDC_MSHFG_SURFTYPE:
		if (!IsContextHelp())
		{
			if (!(this->GridSurfTypes.GetRowIsVisible(this->GridSurfTypes.GetRow()) && this->GridSurfTypes.GetColIsVisible(this->GridSurfTypes.GetCol())))
			{
				::MessageBeep((UINT)-1);
				return TRUE;
			}

			// modify placement
			CDC* pDC = this->GridSurfTypes.GetDC();
			int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
			int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

			long nLeft = ::MulDiv(this->GridSurfTypes.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
			long nTop  = ::MulDiv(this->GridSurfTypes.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

			CRect rc;
			this->GridSurfTypes.GetWindowRect(rc);

			myPopup.pt.x = rc.left + nLeft;
			myPopup.pt.y = rc.top + nTop + 10;
		}
		switch (IsContextHelp() ? this->GridSurfTypes.GetMouseCol() : this->GridSurfTypes.GetCol())
		{
		case 1:
			strRes.LoadString(IDS_SURFACE_305);
			break;
		case 2:
			if (this->IsDlgButtonChecked(IDC_CHECK_SITE_DENSITY) == BST_CHECKED)
			{
				strRes.LoadString(IDS_SURFACE_626);
			}
			else
			{
				strRes.LoadString(IDS_SURFACE_306);
			}
			break;
		default:
			// No help topic is associated with this item. 
			strRes.LoadString(IDS_STRING441);
			break;
		}
		break;
	case IDC_MSHFG_SURFACES:
		if (!IsContextHelp())
		{
			if (!(this->GridSurfaces.GetRowIsVisible(this->GridSurfaces.GetRow()) && this->GridSurfaces.GetColIsVisible(this->GridSurfaces.GetCol())))
			{
				::MessageBeep((UINT)-1);
				return TRUE;
			}

			// modify placement
			CDC* pDC = this->GridSurfaces.GetDC();
			int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
			int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

			long nLeft = ::MulDiv(this->GridSurfaces.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
			long nTop  = ::MulDiv(this->GridSurfaces.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

			CRect rc;
			this->GridSurfaces.GetWindowRect(rc);

			myPopup.pt.x = rc.left + nLeft;
			myPopup.pt.y = rc.top + nTop + 10;
		}
		switch (IsContextHelp() ? this->GridSurfaces.GetMouseCol() : this->GridSurfaces.GetCol())
		{
		case 1:
			strRes.LoadString(IDS_SURFACE_307);
			break;
		case 2:
			strRes.LoadString(IDS_SURFACE_308);
			break;
		default:
			// No help topic is associated with this item. 
			strRes.LoadString(IDS_STRING441);
			break;
		}
		break;
	default:
		// No help topic is associated with this item. 
		strRes.LoadString(IDS_STRING441);
		break;
	}
	myPopup.pszText = strRes;
	return ::HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}

void CSurfacePage::OnBnSetfocusCheckSiteDensity()
{
	// in order for a check box to recieve this notification
	// it must have the notify style set
	CString strRes;
	strRes.LoadString(IDS_STRING692);
	m_eInputDesc.SetWindowText(strRes);
}

void CSurfacePage::OnBnSetfocusCheckMobile()
{
	// in order for a check box to recieve this notification
	// it must have the notify style set
	CString strRes;
	strRes.LoadString(IDS_STRING693);
	m_eInputDesc.SetWindowText(strRes);
}

void CSurfacePage::OnEnSetfocusEditBorkThickness()
{
	CString strRes;
	strRes.LoadString(IDS_STRING694);
	m_eInputDesc.SetWindowText(strRes);
}
