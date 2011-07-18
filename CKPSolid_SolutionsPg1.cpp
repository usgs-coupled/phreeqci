// CKPSolid_SolutionsPg1.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "CKPSolid_SolutionsPg1.h"

#include "DelayUpdate.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CCKPSolid_SolutionsPg1, baseCKPSolid_SolutionsPg1)
IMPLEMENT_DYNCREATE(CCKPSolid_SolutionsPg2, baseCKPSolid_SolutionsPg2)


/////////////////////////////////////////////////////////////////////////////
// CCKPSolid_SolutionsPg1 property page

CCKPSolid_SolutionsPg1::CCKPSolid_SolutionsPg1() : baseCKPSolid_SolutionsPg1(CCKPSolid_SolutionsPg1::IDD)
{
	//{{AFX_DATA_INIT(CCKPSolid_SolutionsPg1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCKPSolid_SolutionsPg1::~CCKPSolid_SolutionsPg1()
{
}

void CCKPSolid_SolutionsPg1::DoDataExchange(CDataExchange* pDX)
{
	baseCKPSolid_SolutionsPg1::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCKPSolid_SolutionsPg1)
	DDX_Control(pDX, IDC_MSHFG_SOL_SOLS, m_egSolSols);
	//}}AFX_DATA_MAP
	DDX_S_S_List(pDX, IDC_MSHFG_SOL_SOLS);
}

void CCKPSolid_SolutionsPg1::DDX_S_S_List(CDataExchange* pDX, int nIDC)
{
	ASSERT(nIDC == IDC_MSHFG_SOL_SOLS);

	// reduce grid flicker
	CDelayUpdate update(this, nIDC);

	if (m_bFirstSetActive)
	{
		// Init edit grid
		InitGrid();
	}

	if (pDX->m_bSaveAndValidate)
	{
		PrepareEditGridCtrl(pDX, nIDC);

		std::list<CS_S> listS_S;
		for (long nRow = m_egSolSols.GetFixedRows(); nRow < m_egSolSols.GetRows(); nRow = GetLastCompRow(nRow) + 1)
		{
			ASSERT(m_egSolSols.GetCellEnabled(nRow, 0));

			CString strName;
			DDX_GridText(pDX, nIDC, nRow, 0, strName);

			if (strName.IsEmpty())
			{
				continue;
			}

			//
			// solid-solution name
			//
			CS_S S_S;
			S_S.m_strName = strName;

			// get components
			long nLastComp = GetLastCompRow(nRow);
			for (long nCompRow = nRow + 1; nCompRow <= nLastComp; ++nCompRow)
			{
				CS_S_Comp S_S_Comp;

				//
				// phase name
				//
				DDX_GridText(pDX, nIDC, nCompRow, 1, S_S_Comp.m_strName);
				if (S_S_Comp.m_strName.IsEmpty()) continue;

				//
				// moles
				//
				CString strTemp;
				DDX_GridText(pDX, nIDC, nCompRow, 2, strTemp);
				if (strTemp.IsEmpty())
				{
					DDX_GridFail(pDX, _T("You must define the number of moles for this component."),
						_T("No moles defined"));
				}
				double dMoles;
				DDX_GridText(pDX, nIDC, nCompRow, 2, dMoles);
				S_S_Comp.m_ldMoles = dMoles;

				S_S.m_listComp.push_back(S_S_Comp);
			}

			if (S_S.m_listComp.size() < 2)
			{
				DDX_GridText(pDX, nIDC, nRow, 0, S_S.m_strName);
				DDX_GridFail(pDX, _T("You must define at least two components for each solid-solution."),
					_T("Not enough components"));
			}
			// OK
			listS_S.push_back(S_S);
		}
		// All OK
		m_listS_S.assign(listS_S.begin(), listS_S.end());
	}
	else
	{
		// for now only exchange on first activation
		if (!m_bFirstSetActive) return;

		std::list<CS_S>::iterator iter = m_listS_S.begin();
		for (long nRow = 1; iter != m_listS_S.end(); ++iter, nRow = GetLastCompRow(nRow) + 1)
		{
			// solid-solution name
			DDX_GridText(pDX, nIDC, nRow, 0, (*iter).m_strName);
			m_egSolSols.SetRow(nRow); // needed for OnBtnAddComp

			// add rows if nec
			int nNewRows = (*iter).m_listComp.size() - 2;
			for (int i = 0; i < nNewRows; ++i)
			{
				OnBtnAddComp();
			}

			std::list<CS_S_Comp>::iterator iterComp = (*iter).m_listComp.begin();
			for (long nCompRow = nRow + 1; iterComp != (*iter).m_listComp.end(); ++iterComp, ++nCompRow)
			{
				// phase name
				DDX_GridText(pDX, nIDC, nCompRow, 1, (*iterComp).m_strName);
				// moles
				DDX_GridText(pDX, nIDC, nCompRow, 2, (*iterComp).m_ldMoles);
			}
		}
		// reset starting grid position
		m_egSolSols.SetRow(1); m_egSolSols.SetCol(0);
	}
}

void CCKPSolid_SolutionsPg1::InitGrid()
{
	// set rows
	m_egSolSols.SetRows(max(m_egSolSols.GetRows(), (long)(m_listS_S.size() + 1) * 3 + 1));

	// set titles
	m_egSolSols.SetTextMatrix(0, 0, _T("Solid-solution name"));
	m_egSolSols.SetTextMatrix(0, 1, _T("Phase name"));
	m_egSolSols.SetTextMatrix(0, 2, _T("Moles"));

	// bold and centered
	for (long nCol = 0; nCol < m_egSolSols.GetCols(0); ++nCol)
	{
		m_egSolSols.SetRow(0); m_egSolSols.SetCol(nCol);
		m_egSolSols.SetCellFontBold(TRUE);
		m_egSolSols.SetCellAlignment(flexAlignCenterCenter);
	}

	// set col widths
	const long col0 = 1849;
	const long col1 = 1207;
	const long col2 = 902;
	m_egSolSols.SetColWidth(0, 0, col0);
	m_egSolSols.SetColWidth(1, 0, col1);
	m_egSolSols.SetColWidth(2, 0, col2);

	switch ((m_egSolSols.GetRows() - 1) % 3)
	{
	case 1 :
		m_egSolSols.SetRows(m_egSolSols.GetRows() + 2);
		break;
	case 2 :
		m_egSolSols.SetRows(m_egSolSols.GetRows() + 1);
		break;
	}
	ASSERT( (m_egSolSols.GetRows() - 1) % 3 == 0 );

	for (long nRow = 1; nRow < m_egSolSols.GetRows(); ++nRow)
	{
		switch (nRow % 3)
		{
		case 0 :
			m_egSolSols.SetTextMatrix(nRow, 0, _T("comp2"));
			m_egSolSols.SetHeaderCell(nRow, 0);
			m_egSolSols.SetRow(nRow); m_egSolSols.SetCol(0);
			m_egSolSols.SetCellAlignment(flexAlignRightCenter);
			break;
		case 1 :
			m_egSolSols.DisableCell(nRow, 1);
			m_egSolSols.DisableCell(nRow, 2);
			break;
		case 2 :
			m_egSolSols.SetTextMatrix(nRow, 0, _T("comp1"));
			m_egSolSols.SetHeaderCell(nRow, 0);
			m_egSolSols.SetRow(nRow); m_egSolSols.SetCol(0);
			m_egSolSols.SetCellAlignment(flexAlignRightCenter);
			break;
		}
	}

	// set initial position
	m_egSolSols.SetRow(1); m_egSolSols.SetCol(0);
}

BEGIN_MESSAGE_MAP(CCKPSolid_SolutionsPg1, baseCKPSolid_SolutionsPg1)
	//{{AFX_MSG_MAP(CCKPSolid_SolutionsPg1)
	ON_BN_CLICKED(IDC_BTN_ADD_COMP, OnBtnAddComp)
	ON_BN_CLICKED(IDC_BTN_REM_COMP, OnBtnRemComp)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
	// custom set focus notifications
	ON_BN_SETFOCUS(IDC_BTN_ADD_COMP, OnSetfocusBtnAddComp)
	ON_BN_SETFOCUS(IDC_BTN_REM_COMP, OnSetfocusBtnRemComp)
	// custom edit grid notifications
	ON_MESSAGE(EGN_BEGINCELLEDIT, OnBeginCellEdit)
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
	ON_MESSAGE(EGN_SETFOCUS, OnSetfocusEG)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CCKPSolid_SolutionsPg2, baseCKPSolid_SolutionsPg2)
	//{{AFX_MSG_MAP(CCKPSolid_SolutionsPg2)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
	ON_MESSAGE(EGN_SETFOCUS, OnSetfocusEG)
	ON_MESSAGE(EGN_BEGINCELLEDIT, OnBeginCellEdit)
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
END_MESSAGE_MAP()

void CCKPSolid_SolutionsPg1::OnSetfocusBtnAddComp()
{
	// in order for a button to recieve this notification
	// it must have the notify style set
	CString strRes;
	strRes.LoadString(IDS_STRING479);
	m_eInputDesc.SetWindowText(strRes);
}

void CCKPSolid_SolutionsPg1::OnSetfocusBtnRemComp()
{
	// in order for a button to recieve this notification
	// it must have the notify style set
	CString strRes;
	strRes.LoadString(IDS_STRING480);
	m_eInputDesc.SetWindowText(strRes);
}

/////////////////////////////////////////////////////////////////////////////
// CCKPSolid_SolutionsPg2 property page

CCKPSolid_SolutionsPg2::CCKPSolid_SolutionsPg2() : baseCKPSolid_SolutionsPg2(CCKPSolid_SolutionsPg2::IDD)
{
	//{{AFX_DATA_INIT(CCKPSolid_SolutionsPg2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	// Fill options list
	m_mapIC2String[CS_S::IC_GUGG_NONDIMENSIONAL]       =  _T("-Gugg_nondim");
	m_mapIC2String[CS_S::IC_GUGG_KJ]                   =  _T("-Gugg_kj");
	m_mapIC2String[CS_S::IC_ACTIVITY_COEFFICIENTS]     =  _T("-activity_coefficients");
	m_mapIC2String[CS_S::IC_DISTRIBUTION_COEFFICIENTS] =  _T("-distribution_coefficients");
	m_mapIC2String[CS_S::IC_MISCIBILITY_GAP]           =  _T("-miscibility_gap");
	m_mapIC2String[CS_S::IC_SPINODAL_GAP]              =  _T("-spinodal_gap");
	m_mapIC2String[CS_S::IC_CRITICAL_POINT]            =  _T("-critical_point");
	m_mapIC2String[CS_S::IC_ALYOTROPIC_POINT]          =  _T("-alyotropic_point");
	m_mapIC2String[CS_S::IC_THOMPSON]                  =  _T("-Thompson");
	m_mapIC2String[CS_S::IC_MARGULES]                  =  _T("-Margules");

	std::map<enum CS_S::InputCase, CString>::const_iterator iter = 
		m_mapIC2String.begin();
	for (; iter != m_mapIC2String.end(); ++iter)
	{
		m_mapString2IC[(*iter).second] = (*iter).first;
	}
}

CCKPSolid_SolutionsPg2::~CCKPSolid_SolutionsPg2()
{
}

void CCKPSolid_SolutionsPg2::DoDataExchange(CDataExchange* pDX)
{
	baseCKPSolid_SolutionsPg2::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCKPSolid_SolutionsPg2)
	DDX_Control(pDX, IDC_MSHFG_SOL_SOLS, m_egSolSols);
	//}}AFX_DATA_MAP
	DDX_S_S_List(pDX, IDC_MSHFG_SOL_SOLS);
}

void CCKPSolid_SolutionsPg2::DDX_S_S_List(CDataExchange* pDX, int nIDC)
{
	ASSERT(nIDC == IDC_MSHFG_SOL_SOLS);
	ASSERT(nIDC == m_egSolSols.GetDlgCtrlID());

	// reduce grid flicker
	CDelayUpdate update(this, nIDC);

	if (m_bFirstSetActive)
	{
		// Init edit grid
		InitGrid();
	}

	if (pDX->m_bSaveAndValidate)
	{
		// VALIDATE
		//// ValidateEGSolSols();
		PrepareEditGridCtrl(pDX, nIDC);

		std::list<CS_S> listS_S;
		for (long nRow = m_egSolSols.GetFixedRows(); nRow < m_egSolSols.GetRows(); nRow = GetLastCompRow(nRow) + 1)
		{
			ASSERT(m_egSolSols.GetCellEnabled(nRow, 0));

			CString strDummy;
			DDX_GridText(pDX, nIDC, nRow, 0, strDummy);

			if (strDummy.IsEmpty())
			{
				continue;
			}

			//
			// solid-solution name
			//
			CS_S S_S;
			S_S.m_strName = strDummy;

			//
			// excess free energy option
			//
			DDX_GridText(pDX, nIDC, nRow, 3, strDummy);
			if (strDummy.IsEmpty())
			{
				DDX_GridFail(pDX, _T("You must define the excess free energy option for this solid-solution."),
					_T("No free energy option"));
			}

			std::map<CString, enum CS_S::InputCase>::const_iterator item = m_mapString2IC.find(strDummy);
			if (item == m_mapString2IC.end())
			{
				DDX_GridFail(pDX, _T("Please select an excess free energy option from the list."),
					_T("Unknown free energy option"));
			}
			S_S.m_nInputCase = (*item).second;

			//
			// temperature in Kelvin
			//
			DDX_GridText(pDX, nIDC, nRow, 5, strDummy);
			if ( strDummy.CompareNoCase(_T("deg C")) == 0 )
			{
				S_S.m_dTk = 273.15;

			}
			else if ( strDummy.CompareNoCase(_T("deg K")) == 0 )
			{
				S_S.m_dTk = 0;
			}
			else
			{
				DDX_GridFail(pDX, _T("Please select the appropriate temperature units from the list."),
					_T("Unknown temperature units"));
			}

			DDX_GridText(pDX, nIDC, nRow, 4, strDummy);
			if ( strDummy.IsEmpty() )
			{
				DDX_GridFail(pDX, _T("You must define the temperature for this solid-solution."),
					_T("No temperature defined"));
			}

			double dTemp;
			DDX_GridText(pDX, nIDC, nRow, 4, dTemp);
			S_S.m_dTk += dTemp;
			if ( S_S.m_dTk < 0.0 )
			{
				DDX_GridFail(pDX, _T("Temperature must be greater than zero."),
					_T("Invalid temperature"));
			}

			//
			// param 1-4
			//
			int nParams = 2;
			switch (S_S.m_nInputCase)
			{
			case CS_S::IC_GUGG_NONDIMENSIONAL :
			case CS_S::IC_MISCIBILITY_GAP :
			case CS_S::IC_SPINODAL_GAP :
			case CS_S::IC_CRITICAL_POINT :
			case CS_S::IC_ALYOTROPIC_POINT :
			case CS_S::IC_GUGG_KJ :
			case CS_S::IC_THOMPSON :
			case CS_S::IC_MARGULES :
				ASSERT( m_egSolSols.GetCellEnabled(nRow, 8) == FALSE );
				ASSERT( m_egSolSols.GetCellEnabled(nRow, 9) == FALSE );
				nParams = 2;
				break;

			case CS_S::IC_ACTIVITY_COEFFICIENTS :
			case CS_S::IC_DISTRIBUTION_COEFFICIENTS :
				ASSERT( m_egSolSols.GetCellEnabled(nRow, 8) == TRUE );
				ASSERT( m_egSolSols.GetCellEnabled(nRow, 9) == TRUE );
				nParams = 4;
				break;
			}
			for (int i = 0; i < nParams; ++i)
			{
				DDX_GridText(pDX, nIDC, nRow, i + 6, strDummy);
				if ( strDummy.IsEmpty() )
				{
					DDX_GridFail(pDX, _T("You must define this parameter for this solid-solution."),
						_T("Missing parameter"));

				}
				DDX_GridText(pDX, nIDC, nRow, i + 6, S_S.m_arrldP[i]);
			}

			// get components
			long nLastComp = GetLastCompRow(nRow);
			for (long nCompRow = nRow + 1; nCompRow <= nLastComp; ++nCompRow)
			{
				CS_S_Comp S_S_Comp;

				//
				// phase name
				//
				DDX_GridText(pDX, nIDC, nCompRow, 1, S_S_Comp.m_strName);
				if (S_S_Comp.m_strName.IsEmpty()) continue;

				//
				// moles
				//
				CString strTemp;
				DDX_GridText(pDX, nIDC, nCompRow, 2, strTemp);
				if (strTemp.IsEmpty())
				{
					DDX_GridFail(pDX, _T("You must define the number of moles for this component."),
						_T("No moles defined"));
				}
				double dMoles;
				DDX_GridText(pDX, nIDC, nCompRow, 2, dMoles);
				S_S_Comp.m_ldMoles = dMoles;

				S_S.m_listComp.push_back(S_S_Comp);
			}

			if (S_S.m_listComp.size() < 2)
			{
				DDX_GridText(pDX, nIDC, nRow, 0, S_S.m_strName);
				DDX_GridFail(pDX, _T("You must define two components for each solid-solution."),
					_T("Not enough components"));
			}
			// OK
			listS_S.push_back(S_S);
		}
		// All OK
		m_listS_S.assign(listS_S.begin(), listS_S.end());
	}
	else
	{
		// for now only exchange on first activation
		if (!m_bFirstSetActive) return;

		std::list<CS_S>::iterator iter = m_listS_S.begin();
		for (long nRow = 1; iter != m_listS_S.end(); ++iter, nRow = GetLastCompRow(nRow) + 1)
		{
			//
			// solid-solution name
			//
			DDX_GridText(pDX, nIDC, nRow, 0, (*iter).m_strName);

			//
			// excess free energy option
			//
			std::map<enum CS_S::InputCase, CString>::iterator item = m_mapIC2String.find((*iter).m_nInputCase);
			ASSERT( item != m_mapIC2String.end() );
			DDX_GridText(pDX, nIDC, nRow, 3, (*item).second);

			long i;
			switch ((*iter).m_nInputCase)
			{
			case CS_S::IC_GUGG_NONDIMENSIONAL :
			case CS_S::IC_MISCIBILITY_GAP :
			case CS_S::IC_SPINODAL_GAP :
			case CS_S::IC_CRITICAL_POINT :
			case CS_S::IC_ALYOTROPIC_POINT :
			case CS_S::IC_GUGG_KJ :
			case CS_S::IC_THOMPSON :
			case CS_S::IC_MARGULES :
				ASSERT( m_egSolSols.GetCellEnabled(nRow, 8) == FALSE );
				ASSERT( m_egSolSols.GetCellEnabled(nRow, 9) == FALSE );
				//
				// param 1-2
				//
				for (i = 0; i < 2; ++i)
				{
					DDX_GridText(pDX, nIDC, nRow, i + 6, (*iter).m_arrldP[i]);
				}
				break;

			case CS_S::IC_ACTIVITY_COEFFICIENTS :
			case CS_S::IC_DISTRIBUTION_COEFFICIENTS :
				m_egSolSols.EnableCell(nRow, 8);
				m_egSolSols.EnableCell(nRow, 9);
				//
				// param 1-4
				//
				for (i = 0; i < 4; ++i)
				{
					DDX_GridText(pDX, nIDC, nRow, i + 6, (*iter).m_arrldP[i]);
				}
				break;
			default :
				ASSERT( FALSE );	// invalid input case
				break;
			}

			//
			// temperature in Kelvin
			//
			DDX_GridText(pDX, nIDC, nRow, 4, (*iter).m_dTk);
			CString strDummy(_T("deg K"));
			DDX_GridText(pDX, nIDC, nRow, 5, strDummy);

			// set components
			std::list<CS_S_Comp>::iterator iterComp = (*iter).m_listComp.begin();
			for (long nCompRow = nRow + 1; iterComp != (*iter).m_listComp.end(); ++iterComp, ++nCompRow)
			{
				//
				// phase name
				//
				DDX_GridText(pDX, nIDC, nCompRow, 1, (*iterComp).m_strName);
				//
				// moles
				//
				DDX_GridText(pDX, nIDC, nCompRow, 2, (*iterComp).m_ldMoles);
			}
		}
	}
}

BOOL CCKPSolid_SolutionsPg2::OnInitDialog()
{
	baseCKPSolid_SolutionsPg2::OnInitDialog();

	CreateRoot(VERTICAL, 5, 6) 
		<< (pane(HORIZONTAL, ABSOLUTE_VERT)
			<< item(IDC_MSHFG_NUM_DESC, ABSOLUTE_VERT)
			)

		<< item(IDC_MSHFG_SOL_SOLS, GREEDY)

		<< (paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, GREEDY, nDefaultBorder, 10, 10)
			<< item(IDC_E_DESC_INPUT, ABSOLUTE_VERT)
			)
	;
	UpdateLayout();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CCKPSolid_SolutionsPg2, baseCKPSolid_SolutionsPg2)
    //{{AFX_EVENTSINK_MAP(CCKPSolid_SolutionsPg2)
	ON_EVENT(CCKPSolid_SolutionsPg2, IDC_MSHFG_SOL_SOLS, -602 /* KeyDown */, OnKeyDownMshfgSolSols, VTS_PI2 VTS_I2)
	ON_EVENT(CCKPSolid_SolutionsPg2, IDC_MSHFG_NUM_DESC, 71 /* EnterCell */, OnEnterCellMshfgNumDesc, VTS_NONE)
	ON_EVENT(CCKPSolid_SolutionsPg2, IDC_MSHFG_SOL_SOLS, 71 /* EnterCell */, OnEnterCellMshfgSolSols, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

LRESULT CCKPSolid_SolutionsPg1::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bMakeChange = TRUE;

	// ignore if edit is cancelled
	if ( pInfo->item.pszText == NULL ) return bMakeChange;

	switch ( nID )
	{
	case IDC_MSHFG_SOL_SOLS :
		if (pInfo->item.iCol == 0 && (GetLastCompRow() + 1 == m_egSolSols.GetRows()))
		{
			// add more rows if nec
			m_egSolSols.SetRows(m_egSolSols.GetRows() + 3);
			for (long nRow = m_egSolSols.GetRows() - 3; nRow < m_egSolSols.GetRows(); ++nRow)
			{
				switch (nRow % 3)
				{
				case 0 :
					m_egSolSols.SetTextMatrix(nRow, 0, _T("comp2"));
					m_egSolSols.SetHeaderCell(nRow, 0);
					m_egSolSols.SetRow(nRow); m_egSolSols.SetCol(0);
					m_egSolSols.SetCellAlignment(flexAlignRightCenter);
					break;
				case 1 :
					m_egSolSols.DisableCell(nRow, 1);
					m_egSolSols.DisableCell(nRow, 2);
					break;
				case 2 :
					m_egSolSols.SetTextMatrix(nRow, 0, _T("comp1"));
					m_egSolSols.SetHeaderCell(nRow, 0);
					m_egSolSols.SetRow(nRow); m_egSolSols.SetCol(0);
					m_egSolSols.SetCellAlignment(flexAlignRightCenter);
					break;
				}
			}
			// reset position
			m_egSolSols.SetRow(pInfo->item.iRow); m_egSolSols.SetCol(0);
		}
		break;
	}

	return bMakeChange;
}

LRESULT CCKPSolid_SolutionsPg1::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bDenyEdit = FALSE;	// allow edit

	if ( nID == IDC_MSHFG_SOL_SOLS )
	{
		switch ( pInfo->item.iCol )
		{
		case 1 :
			// set to phase list
			pInfo->item.bUseCombo = (CUtil::InsertPhases(pInfo->item.hWndCombo, GetDatabase()) > 0);
			break;

		default :
			break;

		}
	}

	return bDenyEdit;
}

void CCKPSolid_SolutionsPg1::OnBtnAddComp()
{
	long nSaveRow = m_egSolSols.GetRow();
	long nSaveCol = m_egSolSols.GetCol();
	long nRow = GetLastCompRow(nSaveRow);


	CString strComp = m_egSolSols.GetTextMatrix(nRow, 0);

	ASSERT(strComp.GetLength() > 4);
	CString strVal = strComp.Mid(4);

	ASSERT(::_istdigit(strVal[0]));
	int nVal = _ttoi(strVal);

	m_egSolSols.SetRows(m_egSolSols.GetRows() + 1);

	strComp.Format(_T("comp%d"), nVal + 1);
	m_egSolSols.SetTextMatrix(m_egSolSols.GetRows() - 1, 0, strComp);

	m_egSolSols.SetHeaderCell(m_egSolSols.GetRows() - 1, 0);
	m_egSolSols.SetRow(m_egSolSols.GetRows() - 1); m_egSolSols.SetCol(0);
	m_egSolSols.SetCellAlignment(flexAlignRightCenter);
	m_egSolSols.SetRowPosition(m_egSolSols.GetRows() - 1, nRow + 1);
	m_egSolSols.SetRow(nSaveRow); m_egSolSols.SetCol(nSaveCol);
}

long CCKPSolid_SolutionsPg1::GetLastCompRow(long nRow /* -1 */)
{
	CDelayUpdate delay(&m_egSolSols);

	if (nRow == -1)
	{
		nRow = m_egSolSols.GetRow();
	}
	if (m_egSolSols.GetCellEnabled(nRow, 0))
	{
		nRow += 2;
	}
	CString strComp = m_egSolSols.GetTextMatrix(nRow, 0);
	ASSERT( strComp.Left(4).Compare(_T("comp")) == 0 );
	while (m_egSolSols.GetRows() > nRow + 1)
	{
		TRACE("In while nRow = %d\n", nRow);
		ASSERT( m_egSolSols.GetRows() > nRow + 1 );
		if (m_egSolSols.GetCellEnabled(nRow + 1, 0))
		{
			break;
		}
		nRow++;
	};

	strComp = m_egSolSols.GetTextMatrix(nRow, 0);
	ASSERT( strComp.Left(4).Compare(_T("comp")) == 0 );
	return nRow;
}

long CCKPSolid_SolutionsPg2::GetLastCompRow(long nRow /* -1 */)
{
	// avoid flashing grid
	CDelayUpdate delay(&m_egSolSols);

	if (nRow == -1)
	{
		nRow = m_egSolSols.GetRow();
	}
	if (m_egSolSols.GetCellEnabled(nRow, 0))
	{
		nRow += 2;
	}
	CString strComp = m_egSolSols.GetTextMatrix(nRow, 0);
	ASSERT( strComp.Left(4).Compare(_T("comp")) == 0 );
	while (m_egSolSols.GetRows() > nRow + 1)
	{
		ASSERT( m_egSolSols.GetRows() > nRow + 1 );
		if (m_egSolSols.GetCellEnabled(nRow + 1, 0))
		{
			break;
		}
		nRow++;
	};

	strComp = m_egSolSols.GetTextMatrix(nRow, 0);
	ASSERT( strComp.Left(4).Compare(_T("comp")) == 0 );
	return nRow;
}

BEGIN_EVENTSINK_MAP(CCKPSolid_SolutionsPg1, baseCKPSolid_SolutionsPg1)
    //{{AFX_EVENTSINK_MAP(CCKPSolid_SolutionsPg1)
	ON_EVENT(CCKPSolid_SolutionsPg1, IDC_MSHFG_SOL_SOLS, -602 /* KeyDown */, OnKeyDownMshfgSolSols, VTS_PI2 VTS_I2)
	ON_EVENT(CCKPSolid_SolutionsPg1, IDC_MSHFG_SOL_SOLS, 71 /* EnterCell */, OnEnterCellMshfgSolSols, VTS_NONE)
	ON_EVENT(CCKPSolid_SolutionsPg1, IDC_MSHFG_NUM_DESC, 71 /* EnterCell */, OnEnterCellMshfgNumDesc, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CCKPSolid_SolutionsPg1::OnKeyDownMshfgSolSols(short FAR* KeyCode, short Shift)
{
	UNUSED(Shift);
	CString strComp, strLastComp, strSolSol, strFormat;
	long nRow, nLastRow, nDeleteRow;
	long nCol;
	switch (*KeyCode)
	{
	case VK_DELETE :
		nCol = m_egSolSols.GetCol();
		nRow = m_egSolSols.GetRow();
		if (nCol == 0)
		{
			if (m_egSolSols.GetCellEnabled(nRow, nCol) == TRUE) // delete solid
			{
				strSolSol = m_egSolSols.GetTextMatrix(nRow, 0);
				if (strSolSol.IsEmpty())
				{
					MessageBeep(0);
					break;
				}
				strFormat.Format(_T("Are you sure you want to delete %s?"), strSolSol);
				if (MessageBox(strFormat, _T("Confirm deletion"), MB_YESNO) == IDYES)
				{
					nLastRow = GetLastCompRow();
					m_egSolSols.SetRedraw(FALSE);
					int nCount = nLastRow - nRow + 1;
					for (int n = 0; n < nCount; ++n)
					{
						m_egSolSols.EnableCell(nRow, 0);
						m_egSolSols.EnableCell(nRow, 1);
						m_egSolSols.EnableCell(nRow, 2);
						m_egSolSols.DeleteRow(nRow);
					}
					// delete extra rows
					m_egSolSols.SetRows(m_egSolSols.GetRows() - nCount + 3);

					// add headers
					m_egSolSols.DisableCell(m_egSolSols.GetRows() - 3, 1); // phase name
					m_egSolSols.DisableCell(m_egSolSols.GetRows() - 3, 2); // moles

					m_egSolSols.SetTextMatrix(m_egSolSols.GetRows() - 2, 0, _T("comp1"));
					m_egSolSols.DisableCell(m_egSolSols.GetRows() - 2, 0);
					m_egSolSols.SetRow(m_egSolSols.GetRows() - 2); m_egSolSols.SetCol(0);
					m_egSolSols.SetCellAlignment(flexAlignRightCenter);

					m_egSolSols.SetTextMatrix(m_egSolSols.GetRows() - 1, 0, _T("comp2"));
					m_egSolSols.DisableCell(m_egSolSols.GetRows() - 1, 0);
					m_egSolSols.SetRow(m_egSolSols.GetRows() - 1); m_egSolSols.SetCol(0);
					m_egSolSols.SetCellAlignment(flexAlignRightCenter);

					// reset selection
					m_egSolSols.SetRow(nRow); m_egSolSols.SetCol(0);
					m_egSolSols.SetRowSel(nRow); m_egSolSols.SetColSel(0);
					m_egSolSols.SetRedraw(TRUE);
				}
			}
			else // delete component
			{
				nDeleteRow = m_egSolSols.GetRow();
				nLastRow = GetLastCompRow(nDeleteRow);
				strLastComp = m_egSolSols.GetTextMatrix(nLastRow, 0);
				if (strLastComp.Compare(_T("comp2")) == 0)
				{
					MessageBox(_T("Each solid solution requires at least two components."), _T("Error"));
					break;
				}
				// confirm deletion
				strComp = m_egSolSols.GetTextMatrix(nDeleteRow, 0);
				strFormat.Format(_T("Are you sure you want to delete %s?"), strComp);
				if (MessageBox(strFormat, _T("Confirm deletion"), MB_YESNO) == IDYES)
				{
					if (nDeleteRow == nLastRow)
					{
						// delete row
						m_egSolSols.DeleteRow(nDeleteRow);
						m_egSolSols.SetRows(m_egSolSols.GetRows() - 1);

						// reset selection
						m_egSolSols.SetRow(nDeleteRow); m_egSolSols.SetCol(0);
						m_egSolSols.SetRowSel(nDeleteRow); m_egSolSols.SetColSel(0);

					}
					else
					{
						// set clipping region
						m_egSolSols.SetRow(nDeleteRow + 1); m_egSolSols.SetCol(1);
						m_egSolSols.SetRowSel(nLastRow); m_egSolSols.SetColSel(2);
						CString strClip = m_egSolSols.GetClip();

						// set new clipping region
						m_egSolSols.SetRow(nDeleteRow);      m_egSolSols.SetCol(1);
						m_egSolSols.SetRowSel(nLastRow - 1); m_egSolSols.SetColSel(2);
						m_egSolSols.SetClip(strClip);

						// delete row
						m_egSolSols.DeleteRow(nLastRow);
						m_egSolSols.SetRows(m_egSolSols.GetRows() - 1);

						// reset selection
						m_egSolSols.SetRow(nDeleteRow); m_egSolSols.SetCol(0);
						m_egSolSols.SetRowSel(nDeleteRow); m_egSolSols.SetColSel(0);
					}
				}
			}
		}
		else
		{
			long nFirstRow = min(m_egSolSols.GetRow(), m_egSolSols.GetRowSel());
			long nLastRow = max(m_egSolSols.GetRow(), m_egSolSols.GetRowSel());
			for (long nRow = nFirstRow; nRow <= nLastRow; ++nRow)
			{
				long nFirstCol = min(m_egSolSols.GetCol(), m_egSolSols.GetColSel());
				long nLastCol = max(m_egSolSols.GetCol(), m_egSolSols.GetColSel());
				for (long nCol = nFirstCol; nCol <= nLastCol; ++nCol)
				{
					if (m_egSolSols.GetCellEnabled(nRow, nCol))
					{
						m_egSolSols.SetTextMatrix(nRow, nCol, _T(""));
					}
				}

			}
		}
		break;
	}
}

void CCKPSolid_SolutionsPg2::OnKeyDownMshfgSolSols(short FAR* KeyCode, short Shift)
{
	UNUSED(Shift);
	CString strComp, strLastComp, strSolSol, strFormat;
	long nRow, nLastRow /*, nDeleteRow */;
	long nCol;
	switch (*KeyCode)
	{
	case VK_DELETE :
		nCol = m_egSolSols.GetCol();
		nRow = m_egSolSols.GetRow();
		if (m_egSolSols.GetCellEnabled(nRow, nCol) == FALSE)
		{
			MessageBeep(0);
			break;
		}
		if (nCol == 0)
		{
			if (m_egSolSols.GetCellEnabled(nRow, nCol) == TRUE) // delete solid
			{
				strSolSol = m_egSolSols.GetTextMatrix(nRow, 0);
				if (strSolSol.IsEmpty())
				{
					MessageBeep(0);
					break;
				}
				strFormat.Format(_T("Are you sure you want to delete %s?"), strSolSol);
				if (MessageBox(strFormat, _T("Confirm deletion"), MB_YESNO) == IDYES)
				{
					nLastRow = GetLastCompRow(nRow);
					m_egSolSols.SetRedraw(FALSE);
					int nCount = nLastRow - nRow + 1;
					for (int n = 0; n < nCount; ++n)
					{
						m_egSolSols.EnableCell(nRow, 0);
						m_egSolSols.EnableCell(nRow, 1);
						m_egSolSols.EnableCell(nRow, 2);
						m_egSolSols.DeleteRow(nRow);
					}
					// delete extra rows
					m_egSolSols.SetRows(m_egSolSols.GetRows() - nCount + 3);

					// add headers
					m_egSolSols.DisableCell(m_egSolSols.GetRows() - 3, 1); // phase name
					m_egSolSols.DisableCell(m_egSolSols.GetRows() - 3, 2); // moles

					//{{
					m_egSolSols.SetTextMatrix(m_egSolSols.GetRows() - 3, 3, _T("-Gugg_nondim"));
					m_egSolSols.SetTextMatrix(m_egSolSols.GetRows() - 3, 5, _T("deg C"));
					//}}

					m_egSolSols.SetTextMatrix(m_egSolSols.GetRows() - 2, 0, _T("comp1"));
					m_egSolSols.DisableCell(m_egSolSols.GetRows() - 2, 0);
					m_egSolSols.SetRow(m_egSolSols.GetRows() - 2); m_egSolSols.SetCol(0);
					m_egSolSols.SetCellAlignment(flexAlignRightCenter);

					m_egSolSols.SetTextMatrix(m_egSolSols.GetRows() - 1, 0, _T("comp2"));
					m_egSolSols.DisableCell(m_egSolSols.GetRows() - 1, 0);
					m_egSolSols.SetRow(m_egSolSols.GetRows() - 1); m_egSolSols.SetCol(0);
					m_egSolSols.SetCellAlignment(flexAlignRightCenter);

					// reset selection
					m_egSolSols.SetRow(nRow); m_egSolSols.SetCol(0);
					m_egSolSols.SetRowSel(nRow); m_egSolSols.SetColSel(0);
					m_egSolSols.SetRedraw(TRUE);
				}
			}
		}
		else
		{
			m_egSolSols.SetTextMatrix(nRow, nCol, _T(""));
		}
		break;
	}
}

void CCKPSolid_SolutionsPg1::OnEnterCellMshfgSolSols()
{
	CString strRes;
	if ( m_egSolSols.GetCellEnabled(m_egSolSols.GetRow(), m_egSolSols.GetCol()) == TRUE )
	{
		switch (m_egSolSols.GetCol())
		{
		case 0 :
			strRes.LoadString(IDS_S_S_COL_0_207);
			break;

		case 1 :
			strRes.LoadString(IDS_S_S_COL_1_208);
			break;

		case 2 :
			strRes.LoadString(IDS_S_S_COL_2_209);
			break;

		default :
			ASSERT(FALSE);
			break;
		}
	}
	m_eInputDesc.SetWindowText(strRes);
}

void CCKPSolid_SolutionsPg2::OnEnterCellMshfgSolSols()
{
	CString strLoad;
	CString strExcessFreeEnergy;
	if ( m_egSolSols.GetCellEnabled(m_egSolSols.GetRow(), m_egSolSols.GetCol()) == TRUE )
	{
		switch (m_egSolSols.GetCol())
		{
		case 0 :
			strLoad.LoadString(IDS_S_S_COL_0_207);
			break;

		case 1 :
			if (m_egSolSols.GetRow() % 2 == 0)
			{
				AfxFormatString1(strLoad, IDS_S_S_COL_1A_210, _T("1"));
			}
			else
			{
				AfxFormatString1(strLoad, IDS_S_S_COL_1A_210, _T("2"));
			}
			break;

		case 2 :
			strLoad.LoadString(IDS_S_S_COL_2_209);
			break;

		case 3 :
			strLoad.LoadString(IDS_S_S_COL_3_211);
			break;

		case 4 :
			strLoad.LoadString(IDS_S_S_COL_4_212);
			break;

		case 5 :
			strLoad.LoadString(IDS_S_S_COL_5_213);
			break;

		case 6 :
		case 7 :
		case 8 :
		case 9 :
			strExcessFreeEnergy =(LPCTSTR)m_egSolSols.GetTextMatrix(m_egSolSols.GetRow(), 3);

			std::map<CString, enum CS_S::InputCase>::const_iterator item = m_mapString2IC.find(strExcessFreeEnergy);

			if ( item != m_mapString2IC.end() )
			{
				switch ((*item).second)
				{
				case CS_S::IC_GUGG_NONDIMENSIONAL :
					switch (m_egSolSols.GetCol())
					{
					case 6 :
						strLoad.LoadString(IDS_GUGG_NONDIM1_214);
						break;
					case 7 :
						strLoad.LoadString(IDS_GUGG_NONDIM2_215);
						break;
					}
					break;
				case CS_S::IC_MISCIBILITY_GAP :
					switch (m_egSolSols.GetCol())
					{
					case 6 :
						strLoad.LoadString(IDS_MISCIBILITY_GAP1_216);
						break;
					case 7 :
						strLoad.LoadString(IDS_MISCIBILITY_GAP2_217);
						break;
					}
					break;
				case CS_S::IC_SPINODAL_GAP :
					switch (m_egSolSols.GetCol())
					{
					case 6 :
						strLoad.LoadString(IDS_SPINODAL_GAP1_218);
						break;
					case 7 :
						strLoad.LoadString(IDS_SPINODAL_GAP2_219);
						break;
					}
					break;
				case CS_S::IC_CRITICAL_POINT :
					switch (m_egSolSols.GetCol())
					{
					case 6 :
						strLoad.LoadString(IDS_CRITICAL_POINT1_220);
						break;
					case 7 :
						strLoad.LoadString(IDS_CRITICAL_POINT2_221);
						break;
					}
					break;
				case CS_S::IC_ALYOTROPIC_POINT :
					switch (m_egSolSols.GetCol())
					{
					case 6 :
						strLoad.LoadString(IDS_ALYOTROPIC_POINT1_222);
						break;
					case 7 :
						strLoad.LoadString(IDS_ALYOTROPIC_POINT2_223);
						break;
					}
					break;
				case CS_S::IC_GUGG_KJ :
					switch (m_egSolSols.GetCol())
					{
					case 6 :
						strLoad.LoadString(IDS_GUGG_KJ1_224);
						break;
					case 7 :
						strLoad.LoadString(IDS_GUGG_KJ2_225);
						break;
					}
					break;
				case CS_S::IC_THOMPSON :
					switch (m_egSolSols.GetCol())
					{
					case 6 :
						strLoad.LoadString(IDS_THOMPSON1_226);
						break;
					case 7 :
						strLoad.LoadString(IDS_THOMPSON2_227);
						break;
					}
					break;
				case CS_S::IC_MARGULES :
					switch (m_egSolSols.GetCol())
					{
					case 6 :
						strLoad.LoadString(IDS_MARGULES1_228);
						break;
					case 7 :
						strLoad.LoadString(IDS_MARGULES2_229);
						break;
					}
					break;
				case CS_S::IC_ACTIVITY_COEFFICIENTS :
					switch (m_egSolSols.GetCol())
					{
					case 6 :
						strLoad.LoadString(IDS_ACT_COEF1_230);
						break;
					case 7 :
						strLoad.LoadString(IDS_ACT_COEF2_231);
						break;
					case 8 :
						strLoad.LoadString(IDS_ACT_COEF3_232);
						break;
					case 9 :
						strLoad.LoadString(IDS_ACT_COEF4_233);
						break;
					}
					break;
				case CS_S::IC_DISTRIBUTION_COEFFICIENTS :
					switch (m_egSolSols.GetCol())
					{
					case 6 :
						strLoad.LoadString(IDS_DIST_COEF1_234);
						break;
					case 7 :
						strLoad.LoadString(IDS_DIST_COEF2_235);
						break;
					case 8 :
						strLoad.LoadString(IDS_DIST_COEF3_236);
						break;
					case 9 :
						strLoad.LoadString(IDS_DIST_COEF4_237);
						break;
					}
					break;
				}
			}
		}
	}
	m_eInputDesc.SetWindowText(strLoad);
}

void CCKPSolid_SolutionsPg2::InitGrid()
{
	// set rows
	m_egSolSols.SetRows(max(m_egSolSols.GetRows(), (long)(m_listS_S.size() + 1) * 3 + 1));

	// set titles
	m_egSolSols.SetTextMatrix(0, 0, _T("Solid-solution name"));
	m_egSolSols.SetTextMatrix(0, 1, _T("Phase name"));
	m_egSolSols.SetTextMatrix(0, 2, _T("Moles"));
	//{{
	m_egSolSols.SetTextMatrix(0, 3, _T("Excess free energy option"));
	m_egSolSols.SetTextMatrix(0, 4, _T("Temp"));
	m_egSolSols.SetTextMatrix(0, 5, _T("Temp units"));
	m_egSolSols.SetTextMatrix(0, 6, _T("param1"));
	m_egSolSols.SetTextMatrix(0, 7, _T("param2"));
	m_egSolSols.SetTextMatrix(0, 8, _T("param3"));
	m_egSolSols.SetTextMatrix(0, 9, _T("param4"));
	//}}

	// bold and centered
	for (long nCol = 0; nCol < m_egSolSols.GetCols(0); ++nCol)
	{
		m_egSolSols.SetRow(0); m_egSolSols.SetCol(nCol);
		m_egSolSols.SetCellFontBold(TRUE);
		m_egSolSols.SetCellAlignment(flexAlignCenterCenter);
	}

	// set col widths
	const long col0 = 1283;
	const long col1 = 1436;
	const long col2 = 825;
	const long col3 = 1803;
	const long col4 = 733;
	const long col5 = 794;
	const long col6_9 = 900;

	m_egSolSols.SetColWidth(0, 0, col0);
	m_egSolSols.SetColWidth(1, 0, col1);
	m_egSolSols.SetColWidth(2, 0, col2);
	m_egSolSols.SetColWidth(3, 0, col3);
	m_egSolSols.SetColWidth(4, 0, col4);
	m_egSolSols.SetColWidth(5, 0, col5);
	m_egSolSols.SetColWidth(6, 0, col6_9);
	m_egSolSols.SetColWidth(7, 0, col6_9);
	m_egSolSols.SetColWidth(8, 0, col6_9);
	m_egSolSols.SetColWidth(9, 0, col6_9);

	// set row heights
	m_egSolSols.SetRowHeight(0, (long) 2.5 * m_egSolSols.GetRowHeight(0));

	// set alignment
	m_egSolSols.SetColAlignment(3, flexAlignCenterCenter);
	m_egSolSols.SetColAlignment(5, flexAlignCenterCenter);

	switch ((m_egSolSols.GetRows() - 1) % 3)
	{
	case 1 :
		m_egSolSols.SetRows(m_egSolSols.GetRows() + 2);
		break;
	case 2 :
		m_egSolSols.SetRows(m_egSolSols.GetRows() + 1);
		break;
	}
	ASSERT( (m_egSolSols.GetRows() - 1) % 3 == 0 );

	for (long nRow = 1; nRow < m_egSolSols.GetRows(); ++nRow)
	{
		switch (nRow % 3)
		{
		case 0 :
			m_egSolSols.SetTextMatrix(nRow, 0, _T("comp2"));
			m_egSolSols.SetHeaderCell(nRow, 0);
			m_egSolSols.SetRow(nRow); m_egSolSols.SetCol(0);
			m_egSolSols.SetCellAlignment(flexAlignRightCenter);
			m_egSolSols.DisableCell(nRow, 3);
			m_egSolSols.DisableCell(nRow, 4);
			m_egSolSols.DisableCell(nRow, 5);
			m_egSolSols.DisableCell(nRow, 6);
			m_egSolSols.DisableCell(nRow, 7);
			m_egSolSols.DisableCell(nRow, 8);
			m_egSolSols.DisableCell(nRow, 9);
			break;
		case 1 :
			m_egSolSols.DisableCell(nRow, 1);
			m_egSolSols.DisableCell(nRow, 2);

			m_egSolSols.SetTextMatrix(nRow, 3, _T("-Gugg_nondim"));
			m_egSolSols.SetTextMatrix(nRow, 4, _T("25"));
			m_egSolSols.SetTextMatrix(nRow, 5, _T("deg C"));

			m_egSolSols.DisableCell(nRow, 8);
			m_egSolSols.DisableCell(nRow, 9);
			break;
		case 2 :
			m_egSolSols.SetTextMatrix(nRow, 0, _T("comp1"));
			m_egSolSols.SetHeaderCell(nRow, 0);
			m_egSolSols.SetRow(nRow); m_egSolSols.SetCol(0);
			m_egSolSols.SetCellAlignment(flexAlignRightCenter);
			m_egSolSols.DisableCell(nRow, 3);
			m_egSolSols.DisableCell(nRow, 4);
			m_egSolSols.DisableCell(nRow, 5);
			m_egSolSols.DisableCell(nRow, 6);
			m_egSolSols.DisableCell(nRow, 7);
			m_egSolSols.DisableCell(nRow, 8);
			m_egSolSols.DisableCell(nRow, 9);
			break;
		}
	}

	// set initial position
	m_egSolSols.SetRow(1); m_egSolSols.SetCol(0);
}

LRESULT CCKPSolid_SolutionsPg2::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bDenyEdit = FALSE;	// allow edit

	CComboBox* pCombo = NULL;

	if ( nID == IDC_MSHFG_SOL_SOLS )
	{
		switch ( pInfo->item.iCol )
		{
		case 1 :
			// set to phase list
			pInfo->item.bUseCombo = (CUtil::InsertPhases(pInfo->item.hWndCombo, GetDatabase()) > 0);
			break;

		case 3 :
			if (pInfo->item.hWndCombo)
			{
				pCombo = (CComboBox*)CWnd::FromHandle(pInfo->item.hWndCombo);
				ASSERT_KINDOF(CComboBox, pCombo);	// must be combobox
			}
			if (pCombo)
			{
				pCombo->ResetContent();
				std::map<enum CS_S::InputCase, CString>::const_iterator iter = 
					m_mapIC2String.begin();
				for (; iter != m_mapIC2String.end(); ++iter)
				{
					pCombo->AddString((*iter).second);
				}
				pInfo->item.bUseCombo = (pCombo->GetCount() > 0);
			}
			else
			{
				pInfo->item.bUseCombo = (m_mapIC2String.size() > 0);
			}
			break;

		case 5 :
			if (pInfo->item.hWndCombo)
			{
				pCombo = (CComboBox*)CWnd::FromHandle(pInfo->item.hWndCombo);
				ASSERT_KINDOF(CComboBox, pCombo);	// must be combobox
			}
			if (pCombo)
			{
				pCombo->ResetContent();
				pCombo->AddString(_T("deg C"));
				pCombo->AddString(_T("deg K"));
				pInfo->item.bUseCombo = (pCombo->GetCount() > 0);
			}
			else
			{
				pInfo->item.bUseCombo = TRUE;
			}
			break;

		default :
			break;

		}
	}

	return bDenyEdit;
}
LRESULT CCKPSolid_SolutionsPg2::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bMakeChange = TRUE;

	// ignore if edit is cancelled
	if ( pInfo->item.pszText == NULL ) return bMakeChange;

	CString str;

	switch ( nID )
	{
	case IDC_MSHFG_SOL_SOLS :
		switch (m_egSolSols.GetCol())
		{
		case 3 : // free energy option
			str = pInfo->item.pszText;
			if (str.CompareNoCase(_T("-Gugg_nondim"))       == 0 ||
				str.CompareNoCase(_T("-Gugg_kj"))           == 0 ||
				str.CompareNoCase(_T("-miscibility_gap"))   == 0 ||
				str.CompareNoCase(_T("-spinodal_gap"))      == 0 ||
				str.CompareNoCase(_T("-critical_point"))    == 0 ||
				str.CompareNoCase(_T("-alyotropic_point"))  == 0 ||
				str.CompareNoCase(_T("-Thompson"))          == 0 ||
				str.CompareNoCase(_T("-Margules"))          == 0 )
			{
				m_egSolSols.DisableCell(pInfo->item.iRow, 8);
				m_egSolSols.SetTextMatrix(pInfo->item.iRow, 8, _T(""));
				m_egSolSols.DisableCell(pInfo->item.iRow, 9);
				m_egSolSols.SetTextMatrix(pInfo->item.iRow, 9, _T(""));
			}
			else
			{
				m_egSolSols.EnableCell(pInfo->item.iRow, 8);
				m_egSolSols.EnableCell(pInfo->item.iRow, 9);
			}

			break;
		}
		break;
	}

	return bMakeChange;
}

void CCKPSolid_SolutionsPg1::OnEnterCellMshfgNumDesc()
{
	// May make this CSolidSolutionsCommon
	CString str;
	switch (m_egNumDesc.GetRow())
	{
	case 0 :
		AfxFormatString1(str, IDS_N_USER_140, _T("solid-solution assemblage") );
		break;
	case 1 :
		AfxFormatString1(str, IDS_N_USER_END_141, _T("solid-solution assemblage") );
		break;
	case 2 :
		AfxFormatString1(str, IDS_DESC_STR_142, _T("solid-solution assemblage") );
		break;
	}
	m_eInputDesc.SetWindowText(str);
}

void CCKPSolid_SolutionsPg2::OnEnterCellMshfgNumDesc()
{
	// May make this CSolidSolutionsCommon
	CString str;
	switch (m_egNumDesc.GetRow())
	{
	case 0 :
		AfxFormatString1(str, IDS_N_USER_140, _T("solid-solution assemblage") );
		break;
	case 1 :
		AfxFormatString1(str, IDS_N_USER_END_141, _T("solid-solution assemblage") );
		break;
	case 2 :
		AfxFormatString1(str, IDS_DESC_STR_142, _T("solid-solution assemblage") );
		break;
	}
	m_eInputDesc.SetWindowText(str);
}

LRESULT CCKPSolid_SolutionsPg1::OnSetfocusEG(WPARAM wParam, LPARAM lParam)
{
	// May make this CSolidSolutionsCommon
	UINT nID = wParam;
	UNUSED_ALWAYS(lParam);

	switch (nID)
	{
	case IDC_MSHFG_NUM_DESC :
		OnEnterCellMshfgNumDesc();
		break;
	case IDC_MSHFG_SOL_SOLS :
		OnEnterCellMshfgSolSols();
		break;
	default :
		break;
	}

	return 0;
}

LRESULT CCKPSolid_SolutionsPg2::OnSetfocusEG(WPARAM wParam, LPARAM lParam)
{
	// May make this CSolidSolutionsCommon
	UINT nID = wParam;
	UNUSED_ALWAYS(lParam);

	switch (nID)
	{
	case IDC_MSHFG_NUM_DESC :
		OnEnterCellMshfgNumDesc();
		break;
	case IDC_MSHFG_SOL_SOLS :
		OnEnterCellMshfgSolSols();
		break;
	default :
		break;
	}
	return 0;
}

void CCKPSolid_SolutionsPg1::OnBtnRemComp() 
{
	m_egSolSols.SendMessage(WM_KEYDOWN, (WPARAM)VK_DELETE, (LPARAM)0);
}

BOOL CCKPSolid_SolutionsPg1::OnInitDialog() 
{
	baseCKPSolid_SolutionsPg1::OnInitDialog();
	
	// Add extra initialization here
	// set layout
	CreateRoot(VERTICAL, 5, 6) 
		<< (pane(HORIZONTAL, ABSOLUTE_VERT)
			<< item(IDC_MSHFG_NUM_DESC, ABSOLUTE_VERT)
			)

		<< (pane(HORIZONTAL, GREEDY)
			<< item(IDC_MSHFG_SOL_SOLS, GREEDY)
			<< (pane(VERTICAL, NORESIZE)
				<< item(IDC_BTN_ADD_COMP, NORESIZE)
				<< item(IDC_BTN_REM_COMP, NORESIZE)
				)
			)

		<< (paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, GREEDY, nDefaultBorder, 10, 10)
			<< item(IDC_E_DESC_INPUT, ABSOLUTE_VERT)
			)
	;
	UpdateLayout();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCKPSolid_SolutionsPg1::OnHelpInfo(HELPINFO* pHelpInfo) 
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
	case IDC_BTN_ADD_COMP:
		strRes.LoadString(IDS_STRING479);
		break;
	case IDC_BTN_REM_COMP:
		strRes.LoadString(IDS_STRING480);
		break;
	case IDC_MSHFG_SOL_SOLS:
		if ( m_egSolSols.GetCellEnabled(IsContextHelp() ? m_egSolSols.GetMouseRow() : m_egSolSols.GetRow(), IsContextHelp() ? m_egSolSols.GetMouseCol() : m_egSolSols.GetCol()) == TRUE )
		{
			if (!IsContextHelp())
			{
				if (!(m_egSolSols.GetRowIsVisible(m_egSolSols.GetRow()) && m_egSolSols.GetColIsVisible(m_egSolSols.GetCol())))
				{
					::MessageBeep((UINT)-1);
					return TRUE;
				}

				// modify placement
				CDC* pDC = m_egSolSols.GetDC();
				int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
				int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

				long nLeft = ::MulDiv(m_egSolSols.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
				long nTop  = ::MulDiv(m_egSolSols.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

				CRect rc;
				m_egSolSols.GetWindowRect(rc);

				myPopup.pt.x = rc.left + nLeft;
				myPopup.pt.y = rc.top + nTop + 10;
			}
			switch (IsContextHelp() ? m_egSolSols.GetMouseCol() : m_egSolSols.GetCol())
			{
			case 0 :
				strRes.LoadString(IDS_S_S_COL_0_207);
				break;
			case 1 :
				strRes.LoadString(IDS_S_S_COL_1_208);
				break;
			case 2 :
				strRes.LoadString(IDS_S_S_COL_2_209);
				break;
			default :
				// No help topic is associated with this item. 
				strRes.LoadString(IDS_STRING441);
				break;
			}
		}
		else
		{
			// No help topic is associated with this item. 
			strRes.LoadString(IDS_STRING441);
		}
		break;
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
			AfxFormatString1(strRes, IDS_N_USER_140, _T("solid-solution assemblage") );
			break;
		case 1 :
			AfxFormatString1(strRes, IDS_N_USER_END_141, _T("solid-solution assemblage") );
			break;
		case 2 :
			AfxFormatString1(strRes, IDS_DESC_STR_142, _T("solid-solution assemblage") );
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

BOOL CCKPSolid_SolutionsPg2::OnHelpInfo(HELPINFO* pHelpInfo) 
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
	case IDC_MSHFG_SOL_SOLS:
		{
			CString strLoad;
			CString strExcessFreeEnergy;
			if ( m_egSolSols.GetCellEnabled(IsContextHelp() ? m_egSolSols.GetMouseRow() : m_egSolSols.GetRow(), IsContextHelp() ? m_egSolSols.GetMouseCol() : m_egSolSols.GetCol()) == TRUE )
			{
				if (!IsContextHelp())
				{
					if (!(m_egSolSols.GetRowIsVisible(m_egSolSols.GetRow()) && m_egSolSols.GetColIsVisible(m_egSolSols.GetCol())))
					{
						::MessageBeep((UINT)-1);
						return TRUE;
					}

					// modify placement
					CDC* pDC = m_egSolSols.GetDC();
					int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
					int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

					long nLeft = ::MulDiv(m_egSolSols.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
					long nTop  = ::MulDiv(m_egSolSols.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

					CRect rc;
					m_egSolSols.GetWindowRect(rc);

					myPopup.pt.x = rc.left + nLeft;
					myPopup.pt.y = rc.top + nTop + 10;
				}
				switch (IsContextHelp() ? m_egSolSols.GetMouseCol() : m_egSolSols.GetCol())
				{
				case 0 :
					strLoad.LoadString(IDS_S_S_COL_0_207);
					break;

				case 1 :
					if (m_egSolSols.GetRow() % 2 == 0)
					{
						AfxFormatString1(strLoad, IDS_S_S_COL_1A_210, _T("1"));
					}
					else
					{
						AfxFormatString1(strLoad, IDS_S_S_COL_1A_210, _T("2"));
					}
					break;

				case 2 :
					strLoad.LoadString(IDS_S_S_COL_2_209);
					break;

				case 3 :
					strLoad.LoadString(IDS_S_S_COL_3_211);
					break;

				case 4 :
					strLoad.LoadString(IDS_S_S_COL_4_212);
					break;

				case 5 :
					strLoad.LoadString(IDS_S_S_COL_5_213);
					break;

				case 6 :
				case 7 :
				case 8 :
				case 9 :
					strExcessFreeEnergy =(LPCTSTR)m_egSolSols.GetTextMatrix(IsContextHelp() ? m_egSolSols.GetMouseRow() : m_egSolSols.GetRow(), 3);

					std::map<CString, enum CS_S::InputCase>::const_iterator item = m_mapString2IC.find(strExcessFreeEnergy);

					if ( item != m_mapString2IC.end() )
					{
						switch ((*item).second)
						{
						case CS_S::IC_GUGG_NONDIMENSIONAL :
							if (!IsContextHelp())
							{
								if (!(m_egSolSols.GetRowIsVisible(m_egSolSols.GetRow()) && m_egSolSols.GetColIsVisible(m_egSolSols.GetCol())))
								{
									::MessageBeep((UINT)-1);
									return TRUE;
								}

								// modify placement
								CDC* pDC = m_egSolSols.GetDC();
								int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
								int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

								long nLeft = ::MulDiv(m_egSolSols.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
								long nTop  = ::MulDiv(m_egSolSols.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

								CRect rc;
								m_egSolSols.GetWindowRect(rc);

								myPopup.pt.x = rc.left + nLeft;
								myPopup.pt.y = rc.top + nTop + 10;
							}
							switch (IsContextHelp() ? m_egSolSols.GetMouseCol() : m_egSolSols.GetCol())
							{
							case 6 :
								strLoad.LoadString(IDS_GUGG_NONDIM1_214);
								break;
							case 7 :
								strLoad.LoadString(IDS_GUGG_NONDIM2_215);
								break;
							}
							break;
						case CS_S::IC_MISCIBILITY_GAP :
							if (!IsContextHelp())
							{
								if (!(m_egSolSols.GetRowIsVisible(m_egSolSols.GetRow()) && m_egSolSols.GetColIsVisible(m_egSolSols.GetCol())))
								{
									::MessageBeep((UINT)-1);
									return TRUE;
								}

								// modify placement
								CDC* pDC = m_egSolSols.GetDC();
								int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
								int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

								long nLeft = ::MulDiv(m_egSolSols.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
								long nTop  = ::MulDiv(m_egSolSols.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

								CRect rc;
								m_egSolSols.GetWindowRect(rc);

								myPopup.pt.x = rc.left + nLeft;
								myPopup.pt.y = rc.top + nTop + 10;
							}
							switch (IsContextHelp() ? m_egSolSols.GetMouseCol() : m_egSolSols.GetCol())
							{
							case 6 :
								strLoad.LoadString(IDS_MISCIBILITY_GAP1_216);
								break;
							case 7 :
								strLoad.LoadString(IDS_MISCIBILITY_GAP2_217);
								break;
							}
							break;
						case CS_S::IC_SPINODAL_GAP :
							if (!IsContextHelp())
							{
								if (!(m_egSolSols.GetRowIsVisible(m_egSolSols.GetRow()) && m_egSolSols.GetColIsVisible(m_egSolSols.GetCol())))
								{
									::MessageBeep((UINT)-1);
									return TRUE;
								}

								// modify placement
								CDC* pDC = m_egSolSols.GetDC();
								int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
								int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

								long nLeft = ::MulDiv(m_egSolSols.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
								long nTop  = ::MulDiv(m_egSolSols.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

								CRect rc;
								m_egSolSols.GetWindowRect(rc);

								myPopup.pt.x = rc.left + nLeft;
								myPopup.pt.y = rc.top + nTop + 10;
							}
							switch (IsContextHelp() ? m_egSolSols.GetMouseCol() : m_egSolSols.GetCol())
							{
							case 6 :
								strLoad.LoadString(IDS_SPINODAL_GAP1_218);
								break;
							case 7 :
								strLoad.LoadString(IDS_SPINODAL_GAP2_219);
								break;
							}
							break;
						case CS_S::IC_CRITICAL_POINT :
							if (!IsContextHelp())
							{
								if (!(m_egSolSols.GetRowIsVisible(m_egSolSols.GetRow()) && m_egSolSols.GetColIsVisible(m_egSolSols.GetCol())))
								{
									::MessageBeep((UINT)-1);
									return TRUE;
								}

								// modify placement
								CDC* pDC = m_egSolSols.GetDC();
								int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
								int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

								long nLeft = ::MulDiv(m_egSolSols.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
								long nTop  = ::MulDiv(m_egSolSols.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

								CRect rc;
								m_egSolSols.GetWindowRect(rc);

								myPopup.pt.x = rc.left + nLeft;
								myPopup.pt.y = rc.top + nTop + 10;
							}
							switch (IsContextHelp() ? m_egSolSols.GetMouseCol() : m_egSolSols.GetCol())
							{
							case 6 :
								strLoad.LoadString(IDS_CRITICAL_POINT1_220);
								break;
							case 7 :
								strLoad.LoadString(IDS_CRITICAL_POINT2_221);
								break;
							}
							break;
						case CS_S::IC_ALYOTROPIC_POINT :
							if (!IsContextHelp())
							{
								if (!(m_egSolSols.GetRowIsVisible(m_egSolSols.GetRow()) && m_egSolSols.GetColIsVisible(m_egSolSols.GetCol())))
								{
									::MessageBeep((UINT)-1);
									return TRUE;
								}

								// modify placement
								CDC* pDC = m_egSolSols.GetDC();
								int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
								int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

								long nLeft = ::MulDiv(m_egSolSols.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
								long nTop  = ::MulDiv(m_egSolSols.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

								CRect rc;
								m_egSolSols.GetWindowRect(rc);

								myPopup.pt.x = rc.left + nLeft;
								myPopup.pt.y = rc.top + nTop + 10;
							}
							switch (IsContextHelp() ? m_egSolSols.GetMouseCol() : m_egSolSols.GetCol())
							{
							case 6 :
								strLoad.LoadString(IDS_ALYOTROPIC_POINT1_222);
								break;
							case 7 :
								strLoad.LoadString(IDS_ALYOTROPIC_POINT2_223);
								break;
							}
							break;
						case CS_S::IC_GUGG_KJ :
							if (!IsContextHelp())
							{
								if (!(m_egSolSols.GetRowIsVisible(m_egSolSols.GetRow()) && m_egSolSols.GetColIsVisible(m_egSolSols.GetCol())))
								{
									::MessageBeep((UINT)-1);
									return TRUE;
								}

								// modify placement
								CDC* pDC = m_egSolSols.GetDC();
								int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
								int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

								long nLeft = ::MulDiv(m_egSolSols.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
								long nTop  = ::MulDiv(m_egSolSols.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

								CRect rc;
								m_egSolSols.GetWindowRect(rc);

								myPopup.pt.x = rc.left + nLeft;
								myPopup.pt.y = rc.top + nTop + 10;
							}
							switch (IsContextHelp() ? m_egSolSols.GetMouseCol() : m_egSolSols.GetCol())
							{
							case 6 :
								strLoad.LoadString(IDS_GUGG_KJ1_224);
								break;
							case 7 :
								strLoad.LoadString(IDS_GUGG_KJ2_225);
								break;
							}
							break;
						case CS_S::IC_THOMPSON :
							if (!IsContextHelp())
							{
								if (!(m_egSolSols.GetRowIsVisible(m_egSolSols.GetRow()) && m_egSolSols.GetColIsVisible(m_egSolSols.GetCol())))
								{
									::MessageBeep((UINT)-1);
									return TRUE;
								}

								// modify placement
								CDC* pDC = m_egSolSols.GetDC();
								int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
								int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

								long nLeft = ::MulDiv(m_egSolSols.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
								long nTop  = ::MulDiv(m_egSolSols.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

								CRect rc;
								m_egSolSols.GetWindowRect(rc);

								myPopup.pt.x = rc.left + nLeft;
								myPopup.pt.y = rc.top + nTop + 10;
							}
							switch (IsContextHelp() ? m_egSolSols.GetMouseCol() : m_egSolSols.GetCol())
							{
							case 6 :
								strLoad.LoadString(IDS_THOMPSON1_226);
								break;
							case 7 :
								strLoad.LoadString(IDS_THOMPSON2_227);
								break;
							}
							break;
						case CS_S::IC_MARGULES :
							if (!IsContextHelp())
							{
								if (!(m_egSolSols.GetRowIsVisible(m_egSolSols.GetRow()) && m_egSolSols.GetColIsVisible(m_egSolSols.GetCol())))
								{
									::MessageBeep((UINT)-1);
									return TRUE;
								}

								// modify placement
								CDC* pDC = m_egSolSols.GetDC();
								int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
								int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

								long nLeft = ::MulDiv(m_egSolSols.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
								long nTop  = ::MulDiv(m_egSolSols.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

								CRect rc;
								m_egSolSols.GetWindowRect(rc);

								myPopup.pt.x = rc.left + nLeft;
								myPopup.pt.y = rc.top + nTop + 10;
							}
							switch (IsContextHelp() ? m_egSolSols.GetMouseCol() : m_egSolSols.GetCol())
							{
							case 6 :
								strLoad.LoadString(IDS_MARGULES1_228);
								break;
							case 7 :
								strLoad.LoadString(IDS_MARGULES2_229);
								break;
							}
							break;
						case CS_S::IC_ACTIVITY_COEFFICIENTS :
							if (!IsContextHelp())
							{
								if (!(m_egSolSols.GetRowIsVisible(m_egSolSols.GetRow()) && m_egSolSols.GetColIsVisible(m_egSolSols.GetCol())))
								{
									::MessageBeep((UINT)-1);
									return TRUE;
								}

								// modify placement
								CDC* pDC = m_egSolSols.GetDC();
								int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
								int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

								long nLeft = ::MulDiv(m_egSolSols.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
								long nTop  = ::MulDiv(m_egSolSols.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

								CRect rc;
								m_egSolSols.GetWindowRect(rc);

								myPopup.pt.x = rc.left + nLeft;
								myPopup.pt.y = rc.top + nTop + 10;
							}
							switch (IsContextHelp() ? m_egSolSols.GetMouseCol() : m_egSolSols.GetCol())
							{
							case 6 :
								strLoad.LoadString(IDS_ACT_COEF1_230);
								break;
							case 7 :
								strLoad.LoadString(IDS_ACT_COEF2_231);
								break;
							case 8 :
								strLoad.LoadString(IDS_ACT_COEF3_232);
								break;
							case 9 :
								strLoad.LoadString(IDS_ACT_COEF4_233);
								break;
							}
							break;
						case CS_S::IC_DISTRIBUTION_COEFFICIENTS :
							if (!IsContextHelp())
							{
								if (!(m_egSolSols.GetRowIsVisible(m_egSolSols.GetRow()) && m_egSolSols.GetColIsVisible(m_egSolSols.GetCol())))
								{
									::MessageBeep((UINT)-1);
									return TRUE;
								}

								// modify placement
								CDC* pDC = m_egSolSols.GetDC();
								int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
								int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

								long nLeft = ::MulDiv(m_egSolSols.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
								long nTop  = ::MulDiv(m_egSolSols.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

								CRect rc;
								m_egSolSols.GetWindowRect(rc);

								myPopup.pt.x = rc.left + nLeft;
								myPopup.pt.y = rc.top + nTop + 10;
							}
							switch (IsContextHelp() ? m_egSolSols.GetMouseCol() : m_egSolSols.GetCol())
							{
							case 6 :
								strLoad.LoadString(IDS_DIST_COEF1_234);
								break;
							case 7 :
								strLoad.LoadString(IDS_DIST_COEF2_235);
								break;
							case 8 :
								strLoad.LoadString(IDS_DIST_COEF3_236);
								break;
							case 9 :
								strLoad.LoadString(IDS_DIST_COEF4_237);
								break;
							}
							break;
						}
					}
				}
				strRes = strLoad;
			}
			else
			{
				// No help topic is associated with this item. 
				strRes.LoadString(IDS_STRING441);
				break;
			}
		}
		break;
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
			AfxFormatString1(strRes, IDS_N_USER_140, _T("solid-solution assemblage") );
			break;
		case 1 :
			AfxFormatString1(strRes, IDS_N_USER_END_141, _T("solid-solution assemblage") );
			break;
		case 2 :
			AfxFormatString1(strRes, IDS_DESC_STR_142, _T("solid-solution assemblage") );
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
