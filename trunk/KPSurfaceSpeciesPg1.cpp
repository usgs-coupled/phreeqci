// KPSurfaceSpeciesPg1.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "KPSurfaceSpeciesPg1.h"

#include "DDX_DDV.h"
#include "EqnChecker2.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

// unused
#undef IDC_RADIO_DAVIES
#undef IDC_RADIO_DH
#undef IDC_RADIO_LLNL_CO2
#undef IDC_RADIO_LLNL_DH
#undef IDC_STATIC_DHA
#undef IDC_EDIT_DHA
#undef IDC_STATIC_DHB
#undef IDC_EDIT_DHB
#undef IDC_EDIT_LLNL_DHA
#undef IDC_GB_ACT_COEF
#undef IDC_STATIC_LLNL_DHA


IMPLEMENT_DYNCREATE(CKPSurfaceSpeciesPg1, baseCKPSurfaceSpeciesPg1)

const long NCOL_IMAGE          = 0;
const long NCOL_RXN            = 1;
const long NCOL_LOGK           = 2;
const long NCOL_DELTA_H        = 3;
const long NCOL_DELTA_H_UNITS  = 4;
const long NCOL_A1             = 5;
const long NCOL_A2             = 6;
const long NCOL_A3             = 7;
const long NCOL_A4             = 8;
const long NCOL_A5             = 9;
const long NCOL_CHECK          = 10;
const long NCOL_MOLE_BAL       = 11;
const long NCOL_Z0             = 12;
const long NCOL_Z1             = 13;
const long NCOL_Z2             = 14;
const long NCOL_F              = 15;
const long NCOL_CHARGE         = 16;


/////////////////////////////////////////////////////////////////////////////
// CKPSurfaceSpeciesPg1 property page

CKPSurfaceSpeciesPg1::CKPSurfaceSpeciesPg1() : baseCKPSurfaceSpeciesPg1(CKPSurfaceSpeciesPg1::IDD)
{
	//{{AFX_DATA_INIT(CKPSurfaceSpeciesPg1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pictHolderRightArrow.CreateFromIcon(IDI_RIGHT_ARROW2);
	m_pictHolderStar.CreateFromIcon(IDI_STAR);

	m_hWndLastControl  = NULL;
	m_bEditLastControl = FALSE;

	m_bIgnoreChanges = false;

}

CKPSurfaceSpeciesPg1::~CKPSurfaceSpeciesPg1()
{
}

void CKPSurfaceSpeciesPg1::DoDataExchange(CDataExchange* pDX)
{
	baseCKPSurfaceSpeciesPg1::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKPSurfaceSpeciesPg1)
	DDX_Control(pDX, IDC_EDIT_ASSOC_RXN, m_ctrlAssocRxn);
	DDX_Control(pDX, IDC_CHECK_EQUATION, m_ctrlCheckEqn);
	DDX_Control(pDX, IDC_CHECK_CD_MUSIC, m_ctrlCheckCD_MUSIC);	
	DDX_Control(pDX, IDC_EDIT_LOGK, m_ctrlLogK);
	DDX_Control(pDX, IDC_EDIT_DELTA_H, m_ctrlDeltaH);
	DDX_Control(pDX, IDC_COMBO_DELTA_H_UNITS, m_ctrlDeltaHUnits);
	DDX_Control(pDX, IDC_EDIT_MOLE_BAL, m_ctrlMoleBal);
	DDX_Control(pDX, IDC_EDIT_A1, m_ctrlA1);
	DDX_Control(pDX, IDC_EDIT_A2, m_ctrlA2);
	DDX_Control(pDX, IDC_EDIT_A3, m_ctrlA3);
	DDX_Control(pDX, IDC_EDIT_A4, m_ctrlA4);
	DDX_Control(pDX, IDC_EDIT_A5, m_ctrlA5);
	DDX_Control(pDX, IDC_EDIT_Z0, m_ctrlZ0);
	DDX_Control(pDX, IDC_EDIT_Z1, m_ctrlZ1);
	DDX_Control(pDX, IDC_EDIT_Z2, m_ctrlZ2);
	DDX_Control(pDX, IDC_EDIT_F, m_ctrlF);
	DDX_Control(pDX, IDC_EDIT_CHARGE, m_ctrlCharge);
	DDX_Control(pDX, IDC_SRCDBPGCTRL1, m_pager);
	DDX_Control(pDX, IDC_MSHFLEXGRID1, m_ctrlGrid);
	DDX_Control(pDX, IDC_DESCRIPTION, m_eInputDesc);
	//}}AFX_DATA_MAP

	if (m_bFirstSetActive)
	{
		InitGrid(pDX, IDC_MSHFLEXGRID1);
	}

	if (pDX->m_bSaveAndValidate)
	{
		CWaitCursor wait;
		CEqnChecker2 checker;

		int nCurrentTextBox = 0;
		std::list<CSpecies> listSpecies;
		CString strDeltaHUnits;

		for (long nRow = 1; nRow < m_ctrlGrid.GetRows(); ++nRow)
		{
			OnLeaveCellGrid();

			CSpecies spec;

			if (m_ctrlGrid.GetTextMatrix(nRow, NCOL_RXN).IsEmpty()) continue;

			// Line 7: mole balance
			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_MOLE_BAL, spec.m_strMoleBalance);

			//
			// Line 6 -no_check
			//
			CString strCheck;
			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_CHECK, strCheck);
			strCheck.TrimLeft();
			strCheck.TrimRight();
			if (strCheck.IsEmpty())
			{
				if (spec.m_strMoleBalance.IsEmpty())
				{
					strCheck = _T("Yes");
				}
				else
				{
					strCheck = _T("No");
				}
			}
			switch (strCheck.GetAt(0))
			{
			case _T('n') : case _T('N') : case _T('f') : case _T('F') :
				spec.m_bCheckEqn = false;
				break;
			case _T('y') : case _T('Y') : case _T('t') : case _T('T') :
				spec.m_bCheckEqn = true;
				break;
			default:
				::AfxMessageBox(_T("Invalid value for check equation."));
				pDX->Fail();
				break;
			}

			//
			// Line 1: Assoc. Rxn
			//
			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_RXN, spec.m_strEqn);
			ASSERT(!spec.m_strEqn.IsEmpty());

			if (!checker.CheckAssociationRxn(spec.m_strEqn, spec.m_bCheckEqn))
			{
				m_ctrlGrid.SetRow(nRow);
				OnRowColChangeGrid();
				pDX->PrepareEditCtrl(IDC_EDIT_ASSOC_RXN);
				::AfxMessageBox(checker.GetLastError());
				pDX->Fail();
			}

			try
			{
				// Line 2: log_k
				nCurrentTextBox = IDC_EDIT_LOGK;
				DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_LOGK, spec.m_dLogK);

				// Line 3: delta_h
				nCurrentTextBox = IDC_EDIT_DELTA_H;
				DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_DELTA_H, spec.m_dDeltaH);
			}
			catch(CUserException* pE)
			{
				OnLeaveCellGrid();
				pE->Delete();
				m_ctrlGrid.SetRow(nRow);
				OnRowColChangeGrid();
				pDX->PrepareEditCtrl(nCurrentTextBox);
				pDX->Fail();
			}

			// Line 3 delta_h [units]
			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_DELTA_H_UNITS, strDeltaHUnits);
			if (strDeltaHUnits.IsEmpty())
			{
				spec.m_nDeltaHUnits = kjoules;
			}
			else
			{
				strDeltaHUnits.MakeLower();
				if (strDeltaHUnits.Find(_T("kcal")) == 0)
				{
					spec.m_nDeltaHUnits = kcal;
				}
				else if (strDeltaHUnits.Find(_T("cal"), 0) == 0)
				{
					spec.m_nDeltaHUnits = cal;
				}
				else if (strDeltaHUnits.Find(_T("kj"), 0) == 0)
				{
					spec.m_nDeltaHUnits = kjoules;
				}
				else if (strDeltaHUnits.Find(_T("j"), 0) == 0)
				{
					spec.m_nDeltaHUnits = joules;
				}
				else
				{
					::AfxMessageBox(_T("Invalid delta H units."));
					pDX->Fail();
				}
			}

			// -analytical_expression
			try
			{
				nCurrentTextBox = IDC_EDIT_A1;
				DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_A1, spec.m_dA1);
				nCurrentTextBox = IDC_EDIT_A2;
				DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_A2, spec.m_dA2);
				nCurrentTextBox = IDC_EDIT_A3;
				DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_A3, spec.m_dA3);
				nCurrentTextBox = IDC_EDIT_A4;
				DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_A4, spec.m_dA4);
				nCurrentTextBox = IDC_EDIT_A5;
				DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_A5, spec.m_dA5);
			}
			catch(CUserException* pE)
			{
				OnLeaveCellGrid();
				pE->Delete();
				m_ctrlGrid.SetRow(nRow);
				OnRowColChangeGrid();
				pDX->PrepareEditCtrl(nCurrentTextBox);
				pDX->Fail();
			}

			if (spec.m_dA1 == std::numeric_limits<double>::signaling_NaN())
			{
				spec.m_dA1 = 0;
			}
			else
			{
				spec.m_bHasAnalExp = true;
			}
			if (spec.m_dA2 == std::numeric_limits<double>::signaling_NaN())
			{
				spec.m_dA2 = 0;
			}
			else
			{
				spec.m_bHasAnalExp = true;
			}
			if (spec.m_dA3 == std::numeric_limits<double>::signaling_NaN())
			{
				spec.m_dA3 = 0;
			}
			else
			{
				spec.m_bHasAnalExp = true;
			}
			if (spec.m_dA4 == std::numeric_limits<double>::signaling_NaN())
			{
				spec.m_dA4 = 0;
			}
			else
			{
				spec.m_bHasAnalExp = true;
			}
			if (spec.m_dA5 == std::numeric_limits<double>::signaling_NaN())
			{
				spec.m_dA5 = 0;
			}
			else
			{
				spec.m_bHasAnalExp = true;
			}

			// -cd_music
			try
			{
				nCurrentTextBox = IDC_EDIT_Z0;
				DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_Z0,     spec.m_dCDMusic[0]);
				nCurrentTextBox = IDC_EDIT_Z1;
				DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_Z1,     spec.m_dCDMusic[1]);
				nCurrentTextBox = IDC_EDIT_Z2;
				DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_Z2,     spec.m_dCDMusic[2]);
				nCurrentTextBox = IDC_EDIT_F;
				DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_F,      spec.m_dCDMusic[3]);
				if (spec.m_dCDMusic[3] < 0.0 || spec.m_dCDMusic[3] > 1.0) {
					::AfxMessageBox("F must be between 0 and 1");
					pDX->Fail();
				}
				nCurrentTextBox = IDC_EDIT_CHARGE;
				DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_CHARGE, spec.m_dCDMusic[4]);
			}
			catch(CUserException* pE)
			{
				OnLeaveCellGrid();
				pE->Delete();
				m_ctrlGrid.SetRow(nRow);
				OnRowColChangeGrid();
				pDX->PrepareEditCtrl(nCurrentTextBox);
				pDX->Fail();
			}

			for (int i = 0; i < 5; ++i)
			{
				if (spec.m_dCDMusic[i] == std::numeric_limits<double>::signaling_NaN())
				{
					spec.m_dCDMusic[i] = 0;
				}
				else
				{
					spec.m_bHasCDMusic = true;
				}
			}

			// must define either log_k or anan expression
			if (!spec.m_bHasAnalExp && spec.m_dLogK == std::numeric_limits<double>::signaling_NaN())
			{
				m_ctrlGrid.SetRow(nRow);
				OnRowColChangeGrid();
				pDX->PrepareEditCtrl(IDC_EDIT_LOGK);
				::AfxMessageBox(_T("Either a Log K or an analytical expression must be defined."));
				pDX->Fail();
			}

			listSpecies.push_back(spec);
		}
		// if here list is valid and can be assigned to the member variable
		m_listSpecies.assign(listSpecies.begin(), listSpecies.end());
	}
	else
	{
		// size grid
		m_ctrlGrid.SetRows(m_listSpecies.size() + 2);

#ifdef _DEBUG
		CEqnChecker2 testChecker;
#endif
		CString strDeltaHUnits;
		CString strCheck;
		CString strActType;

		std::list<CSpecies>::const_iterator cIter = m_listSpecies.begin();
		for (long nRow = 1; cIter != m_listSpecies.end(); ++cIter, ++nRow)
		{
			CSpecies spec(*cIter);
			ASSERT(testChecker.CheckAssociationRxn(spec.m_strEqn, spec.m_bCheckEqn));

			// association reaction
			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_RXN, spec.m_strEqn);

			// log_k
			DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_LOGK, spec.m_dLogK);

			// delta_h
			DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_DELTA_H, spec.m_dDeltaH);

			// delta_h [units]
			switch (spec.m_nDeltaHUnits)
			{
			case kcal:
				strDeltaHUnits = _T("kcal/mol");
				break;
			case cal:
				strDeltaHUnits = _T("cal/mol");
				break;
			case kjoules:
				strDeltaHUnits = _T("kJ/mol");
				break;
			case joules:
				strDeltaHUnits = _T("J/mol");
				break;
			}
			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_DELTA_H_UNITS, strDeltaHUnits);

			// -analytical_expression
			if (spec.m_bHasAnalExp)
			{
				DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_A1, spec.m_dA1);
				DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_A2, spec.m_dA2);
				DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_A3, spec.m_dA3);
				DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_A4, spec.m_dA4);
				DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_A5, spec.m_dA5);
			}

			// -cd_music
			if (spec.m_bHasCDMusic)
			{
				m_ctrlCheckCD_MUSIC.SetCheck(BST_CHECKED);
				DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_Z0,     spec.m_dCDMusic[0]);
				DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_Z1,     spec.m_dCDMusic[1]);
				DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_Z2,     spec.m_dCDMusic[2]);
				DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_F,      spec.m_dCDMusic[3]);
				DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_CHARGE, spec.m_dCDMusic[4]);
			}
			else
			{
				m_ctrlCheckCD_MUSIC.SetCheck(BST_UNCHECKED);
			}

			// no_check
			strCheck =  (spec.m_bCheckEqn) ? _T("Yes") : _T("No");
			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_CHECK, strCheck);

			// mole balance
			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_MOLE_BAL, spec.m_strMoleBalance);

		}

		OnCheckCD_MUSIC();

		m_ctrlGrid.SetCol(0);
		m_ctrlGrid.SetRow(m_listSpecies.size() + 1);
		m_ctrlGrid.SetCellPictureAlignment(flexAlignCenterCenter);
		m_ctrlGrid.SetRefCellPicture(m_pictHolderStar.GetPictureDispatch());
		// set initial pos
		m_ctrlGrid.SetRow(1);
		m_ctrlGrid.SetCol(1);
		OnRowColChangeGrid();
	}
}


BEGIN_MESSAGE_MAP(CKPSurfaceSpeciesPg1, baseCKPSurfaceSpeciesPg1)
	//{{AFX_MSG_MAP(CKPSurfaceSpeciesPg1)
	ON_EN_CHANGE(IDC_EDIT_ASSOC_RXN, OnChangeEditAssocRxn)
	ON_EN_CHANGE(IDC_EDIT_LOGK, OnChangeEditLogk)
	ON_EN_CHANGE(IDC_EDIT_DELTA_H, OnChangeEditDeltaH)
	ON_EN_CHANGE(IDC_EDIT_A1, OnChangeEditA1)
	ON_EN_CHANGE(IDC_EDIT_A2, OnChangeEditA2)
	ON_EN_CHANGE(IDC_EDIT_A3, OnChangeEditA3)
	ON_EN_CHANGE(IDC_EDIT_A4, OnChangeEditA4)
	ON_EN_CHANGE(IDC_EDIT_A5, OnChangeEditA5)
	ON_EN_CHANGE(IDC_EDIT_Z0, OnChangeEditZ0)
	ON_EN_CHANGE(IDC_EDIT_Z1, OnChangeEditZ1)
	ON_EN_CHANGE(IDC_EDIT_Z2, OnChangeEditZ2)
	ON_EN_CHANGE(IDC_EDIT_F, OnChangeEditF)
	ON_EN_CHANGE(IDC_EDIT_CHARGE, OnChangeEditCharge)
	ON_EN_KILLFOCUS(IDC_EDIT_ASSOC_RXN, OnKillfocusEditAssocRxn)
	ON_EN_KILLFOCUS(IDC_EDIT_LOGK, OnKillfocusEditLogk)
	ON_EN_KILLFOCUS(IDC_EDIT_DELTA_H, OnKillfocusEditDeltaH)
	ON_EN_KILLFOCUS(IDC_EDIT_A1, OnKillfocusEditA1)
	ON_EN_KILLFOCUS(IDC_EDIT_A2, OnKillfocusEditA2)
	ON_EN_KILLFOCUS(IDC_EDIT_A3, OnKillfocusEditA3)
	ON_EN_KILLFOCUS(IDC_EDIT_A4, OnKillfocusEditA4)
	ON_EN_KILLFOCUS(IDC_EDIT_A5, OnKillfocusEditA5)
	ON_EN_KILLFOCUS(IDC_EDIT_Z0, OnKillfocusEditZ0)
	ON_EN_KILLFOCUS(IDC_EDIT_Z1, OnKillfocusEditZ1)
	ON_EN_KILLFOCUS(IDC_EDIT_Z2, OnKillfocusEditZ2)
	ON_EN_KILLFOCUS(IDC_EDIT_F, OnKillfocusEditF)
	ON_EN_KILLFOCUS(IDC_EDIT_CHARGE, OnKillfocusEditCharge)
	ON_EN_SETFOCUS(IDC_EDIT_ASSOC_RXN, OnSetfocusEditAssocRxn)
	ON_EN_SETFOCUS(IDC_EDIT_LOGK, OnSetfocusEditLogk)
	ON_EN_SETFOCUS(IDC_EDIT_DELTA_H, OnSetfocusEditDeltaH)
	ON_EN_SETFOCUS(IDC_EDIT_A1, OnSetfocusEditA1)
	ON_EN_SETFOCUS(IDC_EDIT_A2, OnSetfocusEditA2)
	ON_EN_SETFOCUS(IDC_EDIT_A3, OnSetfocusEditA3)
	ON_EN_SETFOCUS(IDC_EDIT_A4, OnSetfocusEditA4)
	ON_EN_SETFOCUS(IDC_EDIT_A5, OnSetfocusEditA5)
	ON_EN_SETFOCUS(IDC_EDIT_Z0, OnSetfocusEditZ0)
	ON_EN_SETFOCUS(IDC_EDIT_Z1, OnSetfocusEditZ1)
	ON_EN_SETFOCUS(IDC_EDIT_Z2, OnSetfocusEditZ2)
	ON_EN_SETFOCUS(IDC_EDIT_F, OnSetfocusEditF)
	ON_EN_SETFOCUS(IDC_EDIT_CHARGE, OnSetfocusEditCharge)
	ON_EN_CHANGE(IDC_EDIT_MOLE_BAL, OnChangeEditMoleBal)
	ON_EN_KILLFOCUS(IDC_EDIT_MOLE_BAL, OnKillfocusEditMoleBal)
	ON_EN_SETFOCUS(IDC_EDIT_MOLE_BAL, OnSetfocusEditMoleBal)
	ON_WM_HELPINFO()
	ON_CBN_SELCHANGE(IDC_COMBO_DELTA_H_UNITS, OnSelchangeComboDeltaHUnits)
	ON_CBN_SETFOCUS(IDC_COMBO_DELTA_H_UNITS, OnSetfocusComboDeltaHUnits)
	ON_BN_CLICKED(IDC_CHECK_EQUATION, OnCheckEquation)
	ON_BN_CLICKED(IDC_CHECK_CD_MUSIC, OnCheckCD_MUSIC)
	ON_BN_SETFOCUS(IDC_CHECK_EQUATION, OnSetFocusCheckEquation)
	ON_BN_SETFOCUS(IDC_CHECK_CD_MUSIC, OnSetFocusCheckCD_MUSIC)
	//}}AFX_MSG_MAP

	// custom edit grid notifications
	ON_MESSAGE(EGN_BEGINCELLEDIT, OnBeginCellEdit)
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
	ON_MESSAGE(EGN_SETFOCUS, OnSetfocusGrid)
	ON_MESSAGE(EGN_CHANGE, OnChange)

END_MESSAGE_MAP()

void CKPSurfaceSpeciesPg1::InitGrid(CDataExchange* pDX, int nIDC)
{
	UNUSED(pDX);
	ASSERT(nIDC == m_ctrlGrid.GetDlgCtrlID());
	UNUSED(nIDC);   // unused in release builds

	// set column widths
	m_ctrlGrid.SetColWidth(NCOL_IMAGE, 0, 300);
	m_ctrlGrid.SetColWidth(NCOL_RXN, 0, 1400);

	// set grid titles
    m_ctrlGrid.SetTextMatrix( 0, NCOL_RXN,           _T("Assoc. Rxn"));
    m_ctrlGrid.SetTextMatrix( 0, NCOL_LOGK,          _T("Log K"));
    m_ctrlGrid.SetTextMatrix( 0, NCOL_DELTA_H,       _T("Delta H"));
    m_ctrlGrid.SetTextMatrix( 0, NCOL_DELTA_H_UNITS, _T("D-H units"));
    m_ctrlGrid.SetTextMatrix( 0, NCOL_A1,            _T("A1"));
    m_ctrlGrid.SetTextMatrix( 0, NCOL_A2,            _T("A2"));
    m_ctrlGrid.SetTextMatrix( 0, NCOL_A3,            _T("A3"));
    m_ctrlGrid.SetTextMatrix( 0, NCOL_A4,            _T("A4"));
    m_ctrlGrid.SetTextMatrix( 0, NCOL_A5,            _T("A5"));
    m_ctrlGrid.SetTextMatrix( 0, NCOL_CHECK,         _T("Check"));
    m_ctrlGrid.SetTextMatrix( 0, NCOL_MOLE_BAL,      _T("Mol bal"));
    m_ctrlGrid.SetTextMatrix( 0, NCOL_Z0,            _T("Z0"));
    m_ctrlGrid.SetTextMatrix( 0, NCOL_Z1,            _T("Z1"));
    m_ctrlGrid.SetTextMatrix( 0, NCOL_Z2,            _T("Z2"));
    m_ctrlGrid.SetTextMatrix( 0, NCOL_F,             _T("F"));
    m_ctrlGrid.SetTextMatrix( 0, NCOL_CHARGE,        _T("Charge")); 

	// set alignment
	m_ctrlGrid.SetColAlignment(NCOL_RXN, flexAlignLeftCenter);

	// set starting position
	m_ctrlGrid.SetRow(1);
	m_ctrlGrid.SetCol(1);
}


BOOL CKPSurfaceSpeciesPg1::OnInitDialog() 
{
	baseCKPSurfaceSpeciesPg1::OnInitDialog();
	
	// set layout
	CreateRoot(VERTICAL, 5, 6) 

		<< (paneCtrl(IDC_STATIC_MAIN, HORIZONTAL, ABSOLUTE_VERT, nDefaultBorder, 10, 10)
			<< (pane(VERTICAL, GREEDY)
				<< (pane(HORIZONTAL, GREEDY)
					<< item(IDC_STATIC_ASSOC_RXN, ABSOLUTE_VERT)
					)
				<< (pane(HORIZONTAL, GREEDY)
					<< item(IDC_EDIT_ASSOC_RXN, ABSOLUTE_VERT)
					<< itemFixed(HORIZONTAL, 1)
					<< item(IDC_CHECK_EQUATION, NORESIZE | ALIGN_CENTER)
					)
				<< (pane(HORIZONTAL, GREEDY)
					<< item(IDC_STATIC_LOGK, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 1)
					<< item(IDC_EDIT_LOGK, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 6)
					<< item(IDC_STATIC_DELTA_H_UNITS, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 239)
					<< itemGrowing(HORIZONTAL)
					<< item(IDC_STATIC_MOLE_BAL, NORESIZE | ALIGN_CENTER)
					)
				<< (pane(HORIZONTAL, GREEDY)
					<< item(IDC_STATIC_DELTA_H, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 1)
					<< item(IDC_EDIT_DELTA_H, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 6)
					<< item(IDC_COMBO_DELTA_H_UNITS, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 239)
					<< itemGrowing(HORIZONTAL)
					<< item(IDC_EDIT_MOLE_BAL, NORESIZE | ALIGN_CENTER)
					)
				<< (paneCtrl(IDC_STATIC_ANAL, VERTICAL, ABSOLUTE_VERT, nDefaultBorder, /*sizeExtraBorder*/10, /*sizeTopExtra*/10, /*sizeSecondary*/0)
					<< (pane(HORIZONTAL)
						<< item(IDC_STATIC_A1, NORESIZE | ALIGN_CENTER)
						<< item(IDC_EDIT_A1, ABSOLUTE_VERT | ALIGN_CENTER)
						<< item(IDC_STATIC_A2, NORESIZE | ALIGN_CENTER)
						<< item(IDC_EDIT_A2, ABSOLUTE_VERT | ALIGN_CENTER)
						<< item(IDC_STATIC_A3, NORESIZE | ALIGN_CENTER)
						<< item(IDC_EDIT_A3, ABSOLUTE_VERT | ALIGN_CENTER)
						<< item(IDC_STATIC_A4, NORESIZE | ALIGN_CENTER)
						<< item(IDC_EDIT_A4, ABSOLUTE_VERT | ALIGN_CENTER)
						<< item(IDC_STATIC_A5, NORESIZE | ALIGN_CENTER)
						<< item(IDC_EDIT_A5, ABSOLUTE_VERT | ALIGN_CENTER)
						)
					)

				<< (paneCtrl(IDC_STATIC_CD_GB, VERTICAL, ABSOLUTE_VERT, nDefaultBorder, /*sizeExtraBorder*/10, /*sizeTopExtra*/10, /*sizeSecondary*/0)
					<< (pane(HORIZONTAL)
						<< item(IDC_STATIC_Z0, NORESIZE | ALIGN_CENTER)
						<< item(IDC_EDIT_Z0, ABSOLUTE_VERT | ALIGN_CENTER)
						<< item(IDC_STATIC_Z1, NORESIZE | ALIGN_CENTER)
						<< item(IDC_EDIT_Z1, ABSOLUTE_VERT | ALIGN_CENTER)
						<< item(IDC_STATIC_Z2, NORESIZE | ALIGN_CENTER)
						<< item(IDC_EDIT_Z2, ABSOLUTE_VERT | ALIGN_CENTER)
						<< item(IDC_STATIC_F, NORESIZE | ALIGN_CENTER)
						<< item(IDC_EDIT_F, ABSOLUTE_VERT | ALIGN_CENTER)
						<< item(IDC_STATIC_CHARGE, NORESIZE | ALIGN_CENTER)
						<< item(IDC_EDIT_CHARGE, ABSOLUTE_VERT | ALIGN_CENTER)
						)
					)

// COMMENT: {8/16/2001 1:03:28 PM}				<< (paneCtrl(IDC_GB_ACT_COEF, VERTICAL, ABSOLUTE_VERT, nDefaultBorder, 10, 10)
// COMMENT: {8/16/2001 1:03:28 PM}					<< (pane(HORIZONTAL, GREEDY)
// COMMENT: {8/16/2001 1:03:28 PM}						<< item(IDC_RADIO_DAVIES, NORESIZE | ALIGN_CENTER)
// COMMENT: {8/16/2001 1:03:28 PM}						<< item((CWnd*)NULL, NORESIZE | ALIGN_CENTER, 12, 13, 12, 13)
// COMMENT: {8/16/2001 1:03:28 PM}						<< item((CWnd*)NULL, ABSOLUTE_VERT | ALIGN_CENTER, 54, 20, 54, 20)
// COMMENT: {8/16/2001 1:03:28 PM}						<< item((CWnd*)NULL, NORESIZE | ALIGN_CENTER, 12, 13, 12, 13)
// COMMENT: {8/16/2001 1:03:28 PM}						<< item((CWnd*)NULL, ABSOLUTE_VERT | ALIGN_CENTER, 54, 20, 54, 20)
// COMMENT: {8/16/2001 1:03:28 PM}						<< itemGrowing(HORIZONTAL)
// COMMENT: {8/16/2001 1:03:28 PM}						<< item(IDC_RADIO_LLNL_CO2, NORESIZE | ALIGN_CENTER)
// COMMENT: {8/16/2001 1:03:28 PM}						<< item((CWnd*)NULL, NORESIZE | ALIGN_CENTER, 12, 13, 12, 13)
// COMMENT: {8/16/2001 1:03:28 PM}						<< item((CWnd*)NULL, ABSOLUTE_VERT | ALIGN_CENTER, 55, 20, 55, 20)
// COMMENT: {8/16/2001 1:03:28 PM}						)
// COMMENT: {8/16/2001 1:03:28 PM}					<< (pane(HORIZONTAL, GREEDY)
// COMMENT: {8/16/2001 1:03:28 PM}						<< item(IDC_RADIO_DH, NORESIZE | ALIGN_CENTER)
// COMMENT: {8/16/2001 1:03:28 PM}						<< item(IDC_STATIC_DHA, NORESIZE | ALIGN_CENTER)
// COMMENT: {8/16/2001 1:03:28 PM}						<< item(IDC_EDIT_DHA, ABSOLUTE_VERT | ALIGN_CENTER)
// COMMENT: {8/16/2001 1:03:28 PM}						<< item(IDC_STATIC_DHB, NORESIZE | ALIGN_CENTER)
// COMMENT: {8/16/2001 1:03:28 PM}						<< item(IDC_EDIT_DHB, ABSOLUTE_VERT | ALIGN_CENTER)
// COMMENT: {8/16/2001 1:03:28 PM}						<< itemGrowing(HORIZONTAL)
// COMMENT: {8/16/2001 1:03:28 PM}						<< item(IDC_RADIO_LLNL_DH, NORESIZE | ALIGN_CENTER)
// COMMENT: {8/16/2001 1:03:28 PM}						<< item(IDC_STATIC_LLNL_DHA, NORESIZE | ALIGN_CENTER)
// COMMENT: {8/16/2001 1:03:28 PM}						<< item(IDC_EDIT_LLNL_DHA, ABSOLUTE_VERT | ALIGN_CENTER)
// COMMENT: {8/16/2001 1:03:28 PM}						)
// COMMENT: {8/16/2001 1:03:28 PM}					)
				)
			)
		<< item(IDC_SRCDBPGCTRL1, ABSOLUTE_VERT | ALIGN_CENTER)
		<< item(IDC_MSHFLEXGRID1, GREEDY | ALIGN_CENTER)

		<< (paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, ABSOLUTE_VERT, nDefaultBorder, 10, 10)
			<< item(IDC_DESCRIPTION, GREEDY)
			)
	;
	UpdateLayout();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CKPSurfaceSpeciesPg1::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
{
	TRACE("OnBeginCellEdit\n");

	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bDenyEdit = FALSE;	// allow edit

	const LPCTSTR arrStrCheckEqnOptions[] = { _T("Yes"), _T("No") };

	if (pInfo->item.hWndCombo)
	{
		ASSERT(m_bIgnoreChanges == false);
		m_bIgnoreChanges = true;
		TRACE("Setting m_bIgnoreChanges = true in OnBeginCellEdit\n");
	}

	switch (nID)
	{
	case IDC_MSHFLEXGRID1 :
		switch ( pInfo->item.iCol )
		{
		case NCOL_RXN: case NCOL_LOGK: case NCOL_DELTA_H:
			break;

		case NCOL_DELTA_H_UNITS:
			if (pInfo->item.hWndCombo)
			{
				CComboBox* pCombo = (CComboBox*)CWnd::FromHandle(pInfo->item.hWndCombo);
				pCombo->ResetContent();
				CString str;
				for (int i = 0; pCombo != NULL && i < m_ctrlDeltaHUnits.GetCount(); ++i)
				{
					m_ctrlDeltaHUnits.GetLBText(i, str);
					pCombo->AddString(str);
				}
				pInfo->item.bUseCombo = (pCombo != NULL && pCombo->GetCount());
			}
			else
			{
				pInfo->item.bUseCombo = TRUE;
			}
			break;

		case NCOL_A1: case NCOL_A2: case NCOL_A3: case NCOL_A4: case NCOL_A5:
			break;

		case NCOL_CHECK :
			if (pInfo->item.hWndCombo)
			{
				CComboBox* pCombo = (CComboBox*)CWnd::FromHandle(pInfo->item.hWndCombo);
				pCombo->ResetContent();
				for (int i = 0; pCombo != NULL && i < sizeof(arrStrCheckEqnOptions) / sizeof(LPCTSTR); ++i)
				{
					pCombo->AddString(arrStrCheckEqnOptions[i]);
				}
				pInfo->item.bUseCombo = (pCombo != NULL && pCombo->GetCount());
			}
			else
			{
				pInfo->item.bUseCombo = TRUE;
			}
			break;

		case NCOL_MOLE_BAL : // Mol bal
			break;

		case NCOL_Z0: case NCOL_Z1: case NCOL_Z2: case NCOL_F: case NCOL_CHARGE:
			break;

		default :
			ASSERT(FALSE);
			break;
		}
		break;
	}

	return bDenyEdit;
}

LRESULT CKPSurfaceSpeciesPg1::OnChange(WPARAM wParam, LPARAM lParam)
{
	TRACE("CKPSurfaceSpeciesPg1::OnChange\n");

	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	ASSERT(m_bIgnoreChanges == true); // should have vbeen set in OnBeginCellEdit

	switch (nID)
	{
	case IDC_MSHFLEXGRID1 :
		switch ( pInfo->item.iCol )
		{
		case NCOL_RXN:  // Assoc. Rxn
			m_ctrlAssocRxn.SetWindowText(pInfo->item.pszText);
			break;

		case NCOL_LOGK:  // Log K
			m_ctrlLogK.SetWindowText(pInfo->item.pszText);
			break;

		case NCOL_DELTA_H:  // Delta H
			m_ctrlDeltaH.SetWindowText(pInfo->item.pszText);
			break;

		case NCOL_DELTA_H_UNITS:  // Delta H Units
			{
				int nSelect = m_ctrlDeltaHUnits.FindStringExact(0, pInfo->item.pszText);
				if (nSelect != CB_ERR)
				{
					m_ctrlDeltaHUnits.SetCurSel(nSelect);
				}
			}
			break;

		case NCOL_A1:  // A1
			m_ctrlA1.SetWindowText(pInfo->item.pszText);
			break;

		case NCOL_A2:  // A2
			m_ctrlA2.SetWindowText(pInfo->item.pszText);
			break;

		case NCOL_A3:  // A3
			m_ctrlA3.SetWindowText(pInfo->item.pszText);
			break;

		case NCOL_A4:  // A4
			m_ctrlA4.SetWindowText(pInfo->item.pszText);
			break;

		case NCOL_A5:  // A5
			m_ctrlA5.SetWindowText(pInfo->item.pszText);
			break;

		case NCOL_CHECK: // Check eqn
			{
				CString strCheck = pInfo->item.pszText;
				CWnd* pStatic = GetDlgItem(IDC_STATIC_MOLE_BAL);
				if (strCheck.GetLength() > 0)
				{
					switch (strCheck.GetAt(0))
					{
					case _T('n') : case _T('N') : case _T('f') : case _T('F') :
						m_ctrlCheckEqn.SetCheck(BST_UNCHECKED);
						m_ctrlMoleBal.EnableWindow(TRUE);
						pStatic->EnableWindow(TRUE);
						break;
					case _T('y') : case _T('Y') : case _T('t') : case _T('T') :
						m_ctrlCheckEqn.SetCheck(BST_CHECKED);
						m_ctrlMoleBal.EnableWindow(FALSE);
						pStatic->EnableWindow(FALSE);
						break;
					default:
						m_ctrlCheckEqn.SetCheck(BST_CHECKED);
						OnCheckEquation();
						break;
					}
				}
			}
			break;

		case NCOL_MOLE_BAL: // Mol bal
			m_ctrlMoleBal.SetWindowText(pInfo->item.pszText);
			break;

		case NCOL_Z0:
			m_ctrlZ0.SetWindowText(pInfo->item.pszText);
			break;

		case NCOL_Z1:
			m_ctrlZ1.SetWindowText(pInfo->item.pszText);
			break;

		case NCOL_Z2:
			m_ctrlZ2.SetWindowText(pInfo->item.pszText);
			break;

		case NCOL_F:
			m_ctrlF.SetWindowText(pInfo->item.pszText);
			break;

		case NCOL_CHARGE:
			m_ctrlCharge.SetWindowText(pInfo->item.pszText);
			break;

		default :
			ASSERT(FALSE);
			break;
		}
		break;
	}
	return 0;

}

LRESULT CKPSurfaceSpeciesPg1::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
{
	TRACE("OnEndCellEdit\n");

	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	UNUSED(pInfo);

	BOOL bMakeChange = TRUE;

	if (!m_bIgnoreChanges)
	{
		// this is a cut or paste
		m_bIgnoreChanges = true;
	}

	if (pInfo->item.iRow != m_ctrlGrid.GetRow())
	{
		m_bIgnoreChanges = false;
		return bMakeChange;
	}

	switch (nID)
	{
	case IDC_MSHFLEXGRID1 :
		switch ( pInfo->item.iCol )
		{
		case NCOL_RXN:  // Assoc. Rxn
			if (pInfo->item.pszText == NULL)
			{
				// edit cancelled
				m_ctrlAssocRxn.SetWindowText(m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol));
			}
			else
			{
				m_ctrlAssocRxn.SetWindowText(pInfo->item.pszText);
				if (strlen(pInfo->item.pszText) > 0)
				{
					if (m_ctrlGrid.GetRows() - 1 == pInfo->item.iRow)
					{
						// add new row
						m_ctrlGrid.SetRows(m_ctrlGrid.GetRows() + 1);

						m_ctrlGrid.SetCol(0);
						m_ctrlGrid.SetRow(m_ctrlGrid.GetRows() - 1);
						m_ctrlGrid.SetCellPictureAlignment(flexAlignCenterCenter);
						m_ctrlGrid.SetRefCellPicture(m_pictHolderStar.GetPictureDispatch());
						m_ctrlGrid.SetCol(pInfo->item.iCol);
						m_ctrlGrid.SetRow(pInfo->item.iRow);
					}
				}
			}
			break;

		case NCOL_LOGK:  // Log K
			if (pInfo->item.pszText == NULL)
			{
				// edit cancelled
				m_ctrlLogK.SetWindowText(m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol));
			}
			else
			{
				m_ctrlLogK.SetWindowText(pInfo->item.pszText);
			}
			break;

		case NCOL_DELTA_H:  // Delta H
			if (pInfo->item.pszText == NULL)
			{
				// edit cancelled
				m_ctrlDeltaH.SetWindowText(m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol));
			}
			else
			{
				m_ctrlDeltaH.SetWindowText(pInfo->item.pszText);
			}
			break;

		case NCOL_DELTA_H_UNITS : // Delta H Units
			if (pInfo->item.pszText == NULL)
			{
				// edit cancelled
				int nSelect = m_ctrlDeltaHUnits.FindStringExact(0, m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol));
				if (nSelect != CB_ERR)
				{
					m_ctrlDeltaHUnits.SetCurSel(nSelect);
				}
			}
			else
			{
				int nSelect = m_ctrlDeltaHUnits.FindStringExact(0, pInfo->item.pszText);
				if (nSelect != CB_ERR)
				{
					m_ctrlDeltaHUnits.SetCurSel(nSelect);
				}
			}
			break;

		case NCOL_A1: // A1
			if (pInfo->item.pszText == NULL)
			{
				// edit cancelled
				m_ctrlA1.SetWindowText(m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol));
			}
			else
			{
				m_ctrlA1.SetWindowText(pInfo->item.pszText);
			}
			break;

		case NCOL_A2: // A2
			if (pInfo->item.pszText == NULL)
			{
				// edit cancelled
				m_ctrlA2.SetWindowText(m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol));
			}
			else
			{
				m_ctrlA2.SetWindowText(pInfo->item.pszText);
			}
			break;

		case NCOL_A3: // A3
			if (pInfo->item.pszText == NULL)
			{
				// edit cancelled
				m_ctrlA3.SetWindowText(m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol));
			}
			else
			{
				m_ctrlA3.SetWindowText(pInfo->item.pszText);
			}
			break;

		case NCOL_A4: // A4
			if (pInfo->item.pszText == NULL)
			{
				// edit cancelled
				m_ctrlA4.SetWindowText(m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol));
			}
			else
			{
				m_ctrlA4.SetWindowText(pInfo->item.pszText);
			}
			break;

		case NCOL_A5: // A5
			if (pInfo->item.pszText == NULL)
			{
				// edit cancelled
				m_ctrlA5.SetWindowText(m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol));
			}
			else
			{
				m_ctrlA5.SetWindowText(pInfo->item.pszText);
			}
			break;

		case NCOL_CHECK: // Check eqn
			{
				CString strCheck;
				CWnd* pStatic = GetDlgItem(IDC_STATIC_MOLE_BAL);
				if (pInfo->item.pszText == NULL)
				{
					// edit cancelled
					strCheck = m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol);
				}
				else
				{
					strCheck = pInfo->item.pszText;
				}
				if (strCheck.GetLength() > 0)
				{
					switch (strCheck.GetAt(0))
					{
					case _T('n') : case _T('N') : case _T('f') : case _T('F') :
						m_ctrlCheckEqn.SetCheck(BST_UNCHECKED);
						m_ctrlMoleBal.EnableWindow(TRUE);
						pStatic->EnableWindow(TRUE);
						break;
					case _T('y') : case _T('Y') : case _T('t') : case _T('T') :
						m_ctrlCheckEqn.SetCheck(BST_CHECKED);
						m_ctrlMoleBal.EnableWindow(FALSE);
						pStatic->EnableWindow(FALSE);
						break;
					default:
						m_ctrlCheckEqn.SetCheck(BST_CHECKED);
						OnCheckEquation();
						break;
					}
				}
				else
				{
					m_ctrlCheckEqn.SetCheck(BST_CHECKED);
					OnCheckEquation();
				}
			}
			break;

		case NCOL_MOLE_BAL: // Mol bal
			if (pInfo->item.pszText == NULL)
			{
				// edit cancelled
				m_ctrlMoleBal.SetWindowText(m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol));
			}
			else
			{
				m_ctrlMoleBal.SetWindowText(pInfo->item.pszText);
			}
			break;

		case NCOL_Z0:
			if (pInfo->item.pszText == NULL)
			{
				// edit cancelled
				m_ctrlZ0.SetWindowText(m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol));
			}
			else
			{
				m_ctrlZ0.SetWindowText(pInfo->item.pszText);
			}
			break;

		case NCOL_Z1:
			if (pInfo->item.pszText == NULL)
			{
				// edit cancelled
				m_ctrlZ1.SetWindowText(m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol));
			}
			else
			{
				m_ctrlZ1.SetWindowText(pInfo->item.pszText);
			}
			break;

		case NCOL_Z2:
			if (pInfo->item.pszText == NULL)
			{
				// edit cancelled
				m_ctrlZ2.SetWindowText(m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol));
			}
			else
			{
				m_ctrlZ2.SetWindowText(pInfo->item.pszText);
			}
			break;

		case NCOL_F:
			if (pInfo->item.pszText == NULL)
			{
				// edit cancelled
				m_ctrlF.SetWindowText(m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol));
			}
			else
			{
				m_ctrlF.SetWindowText(pInfo->item.pszText);
			}
			break;

		case NCOL_CHARGE:
			if (pInfo->item.pszText == NULL)
			{
				// edit cancelled
				m_ctrlCharge.SetWindowText(m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol));
			}
			else
			{
				m_ctrlCharge.SetWindowText(pInfo->item.pszText);
			}
			break;

		default:
			ASSERT(FALSE);
			break;
		}
		break;
	}

	ASSERT(m_bIgnoreChanges == true);
	m_bIgnoreChanges = false;
	TRACE("Setting m_bIgnoreChanges = false in OnEndCellEdit\n");

	return bMakeChange;
}

void CKPSurfaceSpeciesPg1::OnChangeEditAssocRxn() 
{
	TRACE("OnChangeEditAssocRxn\n");

	if (!m_bIgnoreChanges)
	{
		CString str;
		m_ctrlAssocRxn.GetWindowText(str);
		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_RXN, str);
		if (str.GetLength() > 0)
		{
			if (m_ctrlGrid.GetRows() - 1 == m_ctrlGrid.GetRow())
			{
				long nRow = m_ctrlGrid.GetRow();
				long nCol = m_ctrlGrid.GetCol();

				// add new row
				m_ctrlGrid.SetRows(m_ctrlGrid.GetRows() + 1);

				m_ctrlGrid.SetCol(0);
				m_ctrlGrid.SetRow(m_ctrlGrid.GetRows() - 1);
				m_ctrlGrid.SetCellPictureAlignment(flexAlignCenterCenter);
				m_ctrlGrid.SetRefCellPicture(m_pictHolderStar.GetPictureDispatch());
				m_ctrlGrid.SetRow(nRow);
				m_ctrlGrid.SetCol(nCol);
			}
		}
	}
}

void CKPSurfaceSpeciesPg1::OnChangeEditLogk() 
{
	TRACE("OnChangeEditLogk\n");

	if (!m_bIgnoreChanges)
	{
		CString str;
		m_ctrlLogK.GetWindowText(str);
		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_LOGK, str);
	}
}

void CKPSurfaceSpeciesPg1::OnChangeEditDeltaH() 
{
	TRACE("OnChangeEditDeltaH\n");

	if (!m_bIgnoreChanges)
	{
		CString str;
		m_ctrlDeltaH.GetWindowText(str);
		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_DELTA_H, str);	
	}
}

void CKPSurfaceSpeciesPg1::OnChangeEditA1() 
{
	TRACE("OnChangeEditA1\n");

	if (!m_bIgnoreChanges)
	{
		CString str;
		m_ctrlA1.GetWindowText(str);
		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_A1, str);
	}
}

void CKPSurfaceSpeciesPg1::OnChangeEditA2() 
{
	TRACE("OnChangeEditA2\n");

	if (!m_bIgnoreChanges)
	{
		CString str;
		m_ctrlA2.GetWindowText(str);
		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_A2, str);
	}
}

void CKPSurfaceSpeciesPg1::OnChangeEditA3() 
{
	TRACE("OnChangeEditA3\n");

	if (!m_bIgnoreChanges)
	{
		CString str;
		m_ctrlA3.GetWindowText(str);
		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_A3, str);
	}
}

void CKPSurfaceSpeciesPg1::OnChangeEditA4() 
{
	TRACE("OnChangeEditA4\n");

	if (!m_bIgnoreChanges)
	{
		CString str;
		m_ctrlA4.GetWindowText(str);
		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_A4, str);
	}
}

void CKPSurfaceSpeciesPg1::OnChangeEditA5() 
{
	TRACE("OnChangeEditA5\n");

	if (!m_bIgnoreChanges)
	{
		CString str;
		m_ctrlA5.GetWindowText(str);
		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_A5, str);
	}
}

void CKPSurfaceSpeciesPg1::OnChangeEditZ0() 
{
	TRACE("OnChangeEditZ0\n");

	if (!m_bIgnoreChanges)
	{
		CString str;
		m_ctrlZ0.GetWindowText(str);
		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_Z0, str);
	}
}

void CKPSurfaceSpeciesPg1::OnChangeEditZ1() 
{
	TRACE("OnChangeEditZ1\n");

	if (!m_bIgnoreChanges)
	{
		CString str;
		m_ctrlZ1.GetWindowText(str);
		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_Z1, str);
	}
}

void CKPSurfaceSpeciesPg1::OnChangeEditZ2() 
{
	TRACE("OnChangeEditZ2\n");

	if (!m_bIgnoreChanges)
	{
		CString str;
		m_ctrlZ2.GetWindowText(str);
		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_Z2, str);
	}
}

void CKPSurfaceSpeciesPg1::OnChangeEditF() 
{
	TRACE("OnChangeEditF\n");

	if (!m_bIgnoreChanges)
	{
		CString str;
		m_ctrlF.GetWindowText(str);
		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_F, str);
	}
}

void CKPSurfaceSpeciesPg1::OnChangeEditCharge() 
{
	TRACE("OnChangeEditCharge\n");

	if (!m_bIgnoreChanges)
	{
		CString str;
		m_ctrlCharge.GetWindowText(str);
		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_CHARGE, str);
	}
}

void CKPSurfaceSpeciesPg1::OnChangeEditMoleBal() 
{
	TRACE("OnChangeEditMoleBal\n");

	if (!m_bIgnoreChanges)
	{
		CString str;
		m_ctrlMoleBal.GetWindowText(str);
		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_MOLE_BAL, str);
	}
}

void CKPSurfaceSpeciesPg1::OnKillfocusEditAssocRxn() 
{
	TRACE("OnKillfocusEditAssocRxn\n");
	m_hWndLastControl = m_ctrlAssocRxn.m_hWnd;
	m_bEditLastControl = TRUE;	
}

void CKPSurfaceSpeciesPg1::OnKillfocusEditLogk() 
{
	m_hWndLastControl = m_ctrlLogK.m_hWnd;
	m_bEditLastControl = TRUE;	
}

void CKPSurfaceSpeciesPg1::OnKillfocusEditDeltaH() 
{
	m_hWndLastControl = m_ctrlDeltaH.m_hWnd;
	m_bEditLastControl = TRUE;	
}

void CKPSurfaceSpeciesPg1::OnKillfocusEditA1() 
{
	m_hWndLastControl = m_ctrlA1.m_hWnd;
	m_bEditLastControl = TRUE;	
}

void CKPSurfaceSpeciesPg1::OnKillfocusEditA2() 
{
	m_hWndLastControl = m_ctrlA2.m_hWnd;
	m_bEditLastControl = TRUE;	
}

void CKPSurfaceSpeciesPg1::OnKillfocusEditA3() 
{
	m_hWndLastControl = m_ctrlA3.m_hWnd;
	m_bEditLastControl = TRUE;	
}

void CKPSurfaceSpeciesPg1::OnKillfocusEditA4() 
{
	m_hWndLastControl = m_ctrlA4.m_hWnd;
	m_bEditLastControl = TRUE;	
}

void CKPSurfaceSpeciesPg1::OnKillfocusEditA5() 
{
	m_hWndLastControl = m_ctrlA5.m_hWnd;
	m_bEditLastControl = TRUE;	
}

void CKPSurfaceSpeciesPg1::OnKillfocusEditZ0() 
{
	m_hWndLastControl = m_ctrlZ0.m_hWnd;
	m_bEditLastControl = TRUE;	
}

void CKPSurfaceSpeciesPg1::OnKillfocusEditZ1() 
{
	m_hWndLastControl = m_ctrlZ1.m_hWnd;
	m_bEditLastControl = TRUE;	
}

void CKPSurfaceSpeciesPg1::OnKillfocusEditZ2() 
{
	m_hWndLastControl = m_ctrlZ2.m_hWnd;
	m_bEditLastControl = TRUE;	
}

void CKPSurfaceSpeciesPg1::OnKillfocusEditF() 
{
	m_hWndLastControl = m_ctrlF.m_hWnd;
	m_bEditLastControl = TRUE;	
}

void CKPSurfaceSpeciesPg1::OnKillfocusEditCharge() 
{
	m_hWndLastControl = m_ctrlCharge.m_hWnd;
	m_bEditLastControl = TRUE;	
}

void CKPSurfaceSpeciesPg1::OnKillfocusEditMoleBal() 
{
	// do nothing	
}

void CKPSurfaceSpeciesPg1::OnSetfocusEditAssocRxn() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING576);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPSurfaceSpeciesPg1::OnSetfocusEditLogk() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING574);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPSurfaceSpeciesPg1::OnSetfocusEditDeltaH() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING549);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPSurfaceSpeciesPg1::OnSetfocusEditA1() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING551);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPSurfaceSpeciesPg1::OnSetfocusEditA2() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING552);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPSurfaceSpeciesPg1::OnSetfocusEditA3() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING553);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPSurfaceSpeciesPg1::OnSetfocusEditA4() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING554);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPSurfaceSpeciesPg1::OnSetfocusEditA5() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING555);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPSurfaceSpeciesPg1::OnSetfocusEditZ0() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING630);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPSurfaceSpeciesPg1::OnSetfocusEditZ1() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING631);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPSurfaceSpeciesPg1::OnSetfocusEditZ2() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING632);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPSurfaceSpeciesPg1::OnSetfocusEditF() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING633);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPSurfaceSpeciesPg1::OnSetfocusEditCharge() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING634);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPSurfaceSpeciesPg1::OnSetFocusCheckEquation() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING565);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPSurfaceSpeciesPg1::OnSetFocusCheckCD_MUSIC() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING635);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPSurfaceSpeciesPg1::OnSetfocusEditMoleBal() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING563);
	m_eInputDesc.SetWindowText(strRes);	
}

BOOL CKPSurfaceSpeciesPg1::OnHelpInfo(HELPINFO* pHelpInfo) 
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
	UINT nResID = 0;

	switch (pHelpInfo->iCtrlId)
	{
	case IDC_EDIT_ASSOC_RXN: case IDC_STATIC_ASSOC_RXN:
		strRes.LoadString(IDS_STRING576);
		break;
	case IDC_EDIT_LOGK: case IDC_STATIC_LOGK:
		strRes.LoadString(IDS_STRING574);
		break;
	case IDC_EDIT_DELTA_H: case IDC_STATIC_DELTA_H:
		strRes.LoadString(IDS_STRING549);
		break;
	case IDC_COMBO_DELTA_H_UNITS: case IDC_STATIC_DELTA_H_UNITS:
		strRes.LoadString(IDS_STRING550);
		break;
	case IDC_EDIT_A1: case IDC_STATIC_A1:
		strRes.LoadString(IDS_STRING551);
		break;
	case IDC_EDIT_A2: case IDC_STATIC_A2:
		strRes.LoadString(IDS_STRING552);
		break;
	case IDC_EDIT_A3: case IDC_STATIC_A3:
		strRes.LoadString(IDS_STRING553);
		break;
	case IDC_EDIT_A4: case IDC_STATIC_A4:
		strRes.LoadString(IDS_STRING554);
		break;
	case IDC_EDIT_A5: case IDC_STATIC_A5:
		strRes.LoadString(IDS_STRING555);
		break;
	case IDC_EDIT_MOLE_BAL: case IDC_STATIC_MOLE_BAL:
		strRes.LoadString(IDS_STRING563);
		break;
	case IDC_EDIT_Z0: case IDC_STATIC_Z0:
		strRes.LoadString(IDS_STRING630);
		break;
	case IDC_EDIT_Z1: case IDC_STATIC_Z1:
		strRes.LoadString(IDS_STRING631);
		break;
	case IDC_EDIT_Z2: case IDC_STATIC_Z2:
		strRes.LoadString(IDS_STRING632);
		break;
	case IDC_EDIT_F: case IDC_STATIC_F:
		strRes.LoadString(IDS_STRING633);
		break;
	case IDC_EDIT_CHARGE: case IDC_STATIC_CHARGE:
		strRes.LoadString(IDS_STRING634);
		break;
	case IDC_CHECK_CD_MUSIC:
		strRes.LoadString(IDS_STRING635);
		break;
	case IDC_CHECK_EQUATION:
		strRes.LoadString(IDS_STRING565);
		break;
	case IDC_SRCDBPGCTRL1:
		// No help topic is associated with this item. 
		strRes.LoadString(IDS_STRING441);
		break;
	case IDC_MSHFLEXGRID1:
		if (!IsContextHelp())
		{
			if (!(m_ctrlGrid.GetRowIsVisible(m_ctrlGrid.GetRow()) && m_ctrlGrid.GetColIsVisible(m_ctrlGrid.GetCol())))
			{
				::MessageBeep((UINT)-1);
				return TRUE;
			}

			// modify placement
			CDC* pDC = m_ctrlGrid.GetDC();
			int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
			int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

			long nLeft = ::MulDiv(m_ctrlGrid.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
			long nTop  = ::MulDiv(m_ctrlGrid.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

			CRect rc;
			m_ctrlGrid.GetWindowRect(rc);

			myPopup.pt.x = rc.left + nLeft;
			myPopup.pt.y = rc.top + nTop + 10;
		}
		switch (IsContextHelp() ? m_ctrlGrid.GetMouseCol() : m_ctrlGrid.GetCol())
		{
		case NCOL_RXN:
			nResID = IDS_STRING576;
			break;
		case NCOL_LOGK:
			nResID = IDS_STRING574;
			break;
		case NCOL_DELTA_H:
			nResID = IDS_STRING549;
			break;
		case NCOL_DELTA_H_UNITS:
			nResID = IDS_STRING550;
			break;
		case NCOL_A1:
			nResID = IDS_STRING551;
			break;
		case NCOL_A2:
			nResID = IDS_STRING552;
			break;
		case NCOL_A3:
			nResID = IDS_STRING553;
			break;
		case NCOL_A4:
			nResID = IDS_STRING554;
			break;
		case NCOL_A5:
			nResID = IDS_STRING555;
			break;
		case NCOL_CHECK:
			nResID = IDS_STRING564;
			break;
		case NCOL_MOLE_BAL:
			nResID = IDS_STRING563;
			break;
		case NCOL_Z0:
			nResID = IDS_STRING630;
			break;
		case NCOL_Z1:
			nResID = IDS_STRING631;
			break;
		case NCOL_Z2:
			nResID = IDS_STRING632;
			break;
		case NCOL_F:
			nResID = IDS_STRING633;
			break;
		case NCOL_CHARGE:
			nResID = IDS_STRING634;
			break;
		}
		if (nResID != 0)
		{
			strRes.LoadString(nResID);
		}
		else
		{
			// No help topic is associated with this item. 
			strRes.LoadString(IDS_STRING441);
		}
		break;
	default:
		// No help topic is associated with this item. 
		strRes.LoadString(IDS_STRING441);
		break;
	}
	myPopup.pszText = strRes;
	return HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}

BEGIN_EVENTSINK_MAP(CKPSurfaceSpeciesPg1, baseCKPSurfaceSpeciesPg1)
    //{{AFX_EVENTSINK_MAP(CKPSurfaceSpeciesPg1)
	ON_EVENT(CKPSurfaceSpeciesPg1, IDC_MSHFLEXGRID1, 71 /* EnterCell */, OnEnterCellGrid, VTS_NONE)
	ON_EVENT(CKPSurfaceSpeciesPg1, IDC_MSHFLEXGRID1, -601 /* DblClick */, OnDblClickGrid, VTS_NONE)
	ON_EVENT(CKPSurfaceSpeciesPg1, IDC_MSHFLEXGRID1, 72 /* LeaveCell */, OnLeaveCellGrid, VTS_NONE)
	ON_EVENT(CKPSurfaceSpeciesPg1, IDC_MSHFLEXGRID1, 70 /* RowColChange */, OnRowColChangeGrid, VTS_NONE)
	ON_EVENT(CKPSurfaceSpeciesPg1, IDC_SRCDBPGCTRL1, 1 /* GotoFirstClicked */, OnGotoFirstClicked, VTS_NONE)
	ON_EVENT(CKPSurfaceSpeciesPg1, IDC_SRCDBPGCTRL1, 4 /* GotoLastClicked */, OnGotoLastClicked, VTS_NONE)
	ON_EVENT(CKPSurfaceSpeciesPg1, IDC_SRCDBPGCTRL1, 5 /* GotoNewClicked */, OnGotoNewClicked, VTS_NONE)
	ON_EVENT(CKPSurfaceSpeciesPg1, IDC_SRCDBPGCTRL1, 3 /* GotoNextClicked */, OnGotoNextClicked, VTS_NONE)
	ON_EVENT(CKPSurfaceSpeciesPg1, IDC_SRCDBPGCTRL1, 2 /* GotoPrevClicked */, OnGotoPrevClicked, VTS_NONE)
	ON_EVENT(CKPSurfaceSpeciesPg1, IDC_MSHFLEXGRID1, -602 /* KeyDown */, OnKeyDownGrid, VTS_PI2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CKPSurfaceSpeciesPg1::OnEnterCellGrid() 
{
	UINT nResID = 0;
	switch (m_ctrlGrid.GetCol())
	{
	case NCOL_RXN:
		nResID = IDS_STRING576;
		break;
	case NCOL_LOGK:
		nResID = IDS_STRING574;
		break;
	case NCOL_DELTA_H:
		nResID = IDS_STRING549;
		break;
	case NCOL_DELTA_H_UNITS:
		nResID = IDS_STRING550;
		break;
	case NCOL_A1:
		nResID = IDS_STRING551;
		break;
	case NCOL_A2:
		nResID = IDS_STRING552;
		break;
	case NCOL_A3:
		nResID = IDS_STRING553;
		break;
	case NCOL_A4:
		nResID = IDS_STRING554;
		break;
	case NCOL_A5:
		nResID = IDS_STRING555;
		break;
	case NCOL_CHECK:
		nResID = IDS_STRING564;
		break;
	case NCOL_MOLE_BAL:
		nResID = IDS_STRING563;
		break;
	case NCOL_Z0:
		nResID = IDS_STRING630;
		break;
	case NCOL_Z1:
		nResID = IDS_STRING631;
		break;
	case NCOL_Z2:
		nResID = IDS_STRING632;
		break;
	case NCOL_F:
		nResID = IDS_STRING633;
		break;
	case NCOL_CHARGE:
		nResID = IDS_STRING634;
		break;
	}

	if (nResID != 0)
	{
		CString strRes;
		strRes.LoadString(nResID);
		m_eInputDesc.SetWindowText(strRes);
	}
	else
	{
		m_eInputDesc.SetWindowText(_T(""));
	}
}

void CKPSurfaceSpeciesPg1::OnSelchangeComboDeltaHUnits() 
{
	TRACE("OnSelchangeComboDeltaHUnits\n");

	CString strUnits;
	m_ctrlDeltaHUnits.GetWindowText(strUnits);
	m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_DELTA_H_UNITS, strUnits);
}

void CKPSurfaceSpeciesPg1::OnSetfocusComboDeltaHUnits() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING550);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPSurfaceSpeciesPg1::OnDblClickGrid() 
{
	if (!m_ctrlGrid.GetMouseRow())
	{
		// resize column to widest or to fit WWW
		long nCol = m_ctrlGrid.GetMouseCol();
		if (nCol == 0) return;

		CDC* pDC = m_ctrlGrid.GetDC();
		int nPixelsPerInchX = pDC->GetDeviceCaps(LOGPIXELSX);

		CSize size;
		long nWidest = pDC->GetTextExtent(_T("WWW"), 3).cx;
		for (long i = 0; i < m_ctrlGrid.GetRows(); ++i)
		{
			size = pDC->GetTextExtent(m_ctrlGrid.GetTextMatrix(i, nCol));
			if (size.cx > nWidest)
			{
				nWidest = size.cx;
			}
		}
		// limit col width to client rect minus imagewidth(col 0)
		CRect rect;
		m_ctrlGrid.GetClientRect(&rect);
		long nImageWidth = ::MulDiv(nPixelsPerInchX, m_ctrlGrid.GetColWidth(NCOL_IMAGE, 0), TWIPS_PER_INCH);
		nWidest = __min(nWidest, rect.right - 20 - nImageWidth);

		ASSERT(m_ctrlGrid.GetColWidth(NCOL_IMAGE, 0) >= 0);
		if (nWidest > 5)
		{
			m_ctrlGrid.SetColWidth(nCol, 0, ::MulDiv(nWidest, TWIPS_PER_INCH, nPixelsPerInchX));
		}
	}
}

void CKPSurfaceSpeciesPg1::OnLeaveCellGrid() 
{
	TRACE("OnLeaveCell\n");

	long nColSave = m_ctrlGrid.GetCol();

	CPictureHolder pictHolder;
	pictHolder.CreateEmpty();

	m_ctrlGrid.SetCol(0);
	if (m_ctrlGrid.GetRow() == m_ctrlGrid.GetRows() - 1)
	{
		m_ctrlGrid.SetCellPictureAlignment(flexAlignCenterCenter);
		m_ctrlGrid.SetRefCellPicture(m_pictHolderStar.GetPictureDispatch());
	}
	else
	{
		m_ctrlGrid.SetRefCellPicture(pictHolder.GetPictureDispatch());
	}	
	m_ctrlGrid.SetCol(nColSave);
}

void CKPSurfaceSpeciesPg1::OnRowColChangeGrid() 
{
	TRACE("OnRowColChangeGrid\n");

	ASSERT(m_bIgnoreChanges == false);
	m_bIgnoreChanges = true;
	TRACE("Setting m_bIgnoreChanges = true in OnRowColChangeGrid\n");

	long nRow = m_ctrlGrid.GetRow();
	long nCol = m_ctrlGrid.GetCol();

	// rxn
	m_ctrlAssocRxn.SetWindowText(m_ctrlGrid.GetTextMatrix(nRow, NCOL_RXN));  // implicit OnChangeEditAssocRxn

	// Log K
	m_ctrlLogK.SetWindowText(m_ctrlGrid.GetTextMatrix(nRow, NCOL_LOGK));  // implicit OnChangeEditLogk

	// Delta H
	m_ctrlDeltaH.SetWindowText(m_ctrlGrid.GetTextMatrix(nRow, NCOL_DELTA_H));  // implicit OnChangeEditDeltaH

	// Delta H units
	int nSelect = m_ctrlDeltaHUnits.FindStringExact(0, m_ctrlGrid.GetTextMatrix(nRow, NCOL_DELTA_H_UNITS));
	if (nSelect == CB_ERR)
	{
		nSelect = m_ctrlDeltaHUnits.FindStringExact(0, _T("kJ/mol"));
		m_ctrlDeltaHUnits.SetCurSel(nSelect); // no implicit OnSelchangeComboDeltaHUnits
		m_ctrlGrid.SetTextMatrix(nRow, NCOL_DELTA_H_UNITS, _T("kJ/mol"));
	}
	else
	{
		m_ctrlDeltaHUnits.SetCurSel(nSelect); // no implicit OnSelchangeComboDeltaHUnits
	}


	// analytical expression
	m_ctrlA1.SetWindowText(m_ctrlGrid.GetTextMatrix(nRow, NCOL_A1)); // implicit OnChangeEditA1
	m_ctrlA2.SetWindowText(m_ctrlGrid.GetTextMatrix(nRow, NCOL_A2)); // implicit OnChangeEditA2
	m_ctrlA3.SetWindowText(m_ctrlGrid.GetTextMatrix(nRow, NCOL_A3)); // implicit OnChangeEditA3
	m_ctrlA4.SetWindowText(m_ctrlGrid.GetTextMatrix(nRow, NCOL_A4)); // implicit OnChangeEditA4
	m_ctrlA5.SetWindowText(m_ctrlGrid.GetTextMatrix(nRow, NCOL_A5)); // implicit OnChangeEditA5

	// cd_music
	m_ctrlZ0.SetWindowText(m_ctrlGrid.GetTextMatrix(nRow, NCOL_Z0)); // implicit
	m_ctrlZ1.SetWindowText(m_ctrlGrid.GetTextMatrix(nRow, NCOL_Z1)); // implicit
	m_ctrlZ2.SetWindowText(m_ctrlGrid.GetTextMatrix(nRow, NCOL_Z2)); // implicit
	m_ctrlF.SetWindowText(m_ctrlGrid.GetTextMatrix(nRow, NCOL_F)); // implicit
	m_ctrlCharge.SetWindowText(m_ctrlGrid.GetTextMatrix(nRow, NCOL_CHARGE)); // implicit

	// set arrow image
	m_ctrlGrid.SetCol(0);
	m_ctrlGrid.SetRefCellPicture(m_pictHolderRightArrow.GetPictureDispatch());
	m_ctrlGrid.SetCellPictureAlignment(flexAlignCenterCenter);

	// check eqn
	CString strCheck = m_ctrlGrid.GetTextMatrix(nRow, NCOL_CHECK);
	CWnd* pStatic = GetDlgItem(IDC_STATIC_MOLE_BAL);
	if (strCheck.GetLength() > 0)
	{
		switch (strCheck.GetAt(0))
		{
		case _T('n') : case _T('N') : case _T('f') : case _T('F') :
			m_ctrlCheckEqn.SetCheck(BST_UNCHECKED);
			m_ctrlMoleBal.EnableWindow(TRUE);
			pStatic->EnableWindow(TRUE);
			break;
		case _T('y') : case _T('Y') : case _T('t') : case _T('T') :
			m_ctrlCheckEqn.SetCheck(BST_CHECKED);
			m_ctrlMoleBal.EnableWindow(FALSE);
			pStatic->EnableWindow(FALSE);
			break;
		default:
			m_ctrlCheckEqn.SetCheck(BST_CHECKED);
			OnCheckEquation();
			break;
		}
	}
	else
	{
		m_ctrlCheckEqn.SetCheck(BST_CHECKED);
		OnCheckEquation();
	}

	// mole balance
	m_ctrlMoleBal.SetWindowText(m_ctrlGrid.GetTextMatrix(nRow, NCOL_MOLE_BAL)); // implicit OnChangeEditMoleBal

	// reset column
	m_ctrlGrid.SetCol(nCol);

	// update pager
	CString str;
	str.Format(_T("%d"), nRow);
	m_pager.SetCaption(str);
	if (nRow == 1)
	{
		m_pager.SetGotoPrevEnabled(FALSE);
	}
	else
	{
		m_pager.SetGotoPrevEnabled(TRUE);
	}
	if (nRow == m_ctrlGrid.GetRows() - 1)
	{
		m_pager.SetGotoNextEnabled(FALSE);
	}
	else
	{
		m_pager.SetGotoNextEnabled(TRUE);
	}

	ASSERT(m_bIgnoreChanges == true);
	m_bIgnoreChanges = false;
	TRACE("Setting m_bIgnoreChanges = false in OnRowColChangeGrid\n");
}

void CKPSurfaceSpeciesPg1::OnCheckEquation() 
{
	TRACE("OnCheckEquation\n");

	int nCheck = m_ctrlCheckEqn.GetCheck();
	CString strCheck = _T("Yes");
	CWnd* pStatic = GetDlgItem(IDC_STATIC_MOLE_BAL);
	switch (nCheck)
	{
	case BST_CHECKED:
		m_ctrlMoleBal.EnableWindow(FALSE);
		pStatic->EnableWindow(FALSE);
		break;
	case BST_UNCHECKED:
		m_ctrlMoleBal.EnableWindow(TRUE);
		pStatic->EnableWindow(TRUE);
		strCheck = _T("No");
		break;
	default:
		ASSERT(FALSE);
		break;
	}
	m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_CHECK, strCheck);
}

void CKPSurfaceSpeciesPg1::OnCheckCD_MUSIC() 
{
	TRACE("OnCheckCD_MUSIC\n");

	int nCheck = m_ctrlCheckCD_MUSIC.GetCheck();

	BOOL bEnable = (nCheck == BST_CHECKED);

	static int ids[] = 
	{
		IDC_STATIC_Z0,
		IDC_STATIC_Z1,
		IDC_STATIC_Z2,
		IDC_STATIC_F,
		IDC_STATIC_CHARGE,
		IDC_EDIT_Z0,
		IDC_EDIT_Z1,
		IDC_EDIT_Z2,
		IDC_EDIT_F,
		IDC_EDIT_CHARGE,
	};

	for (int i = 0; i < sizeof(ids) / sizeof(ids[0]); ++i)
	{
		if (CWnd *pWnd = this->GetDlgItem(ids[i]))
		{
			pWnd->EnableWindow(bEnable);
		}
	}
}

void CKPSurfaceSpeciesPg1::OnGotoFirstClicked() 
{
	TRACE("OnGotoFirstClicked\n");

	OnLeaveCellGrid();
	m_ctrlGrid.SetRow(1);
	m_ctrlGrid.GetCellLeft();
	OnRowColChangeGrid();

	ResetFocus();
}

void CKPSurfaceSpeciesPg1::OnGotoLastClicked() 
{
	TRACE("OnGotoLastClicked\n");

	OnLeaveCellGrid();
	for (long nRow = 1; nRow < m_ctrlGrid.GetRows(); ++nRow)
	{
		if (m_ctrlGrid.GetTextMatrix(nRow, NCOL_RXN).IsEmpty())
			break;
	}
	m_ctrlGrid.SetRow(__max(nRow - 1, 1));
	m_ctrlGrid.GetCellLeft();
	OnRowColChangeGrid();
	ResetFocus();
}

void CKPSurfaceSpeciesPg1::OnGotoNewClicked() 
{
	TRACE("OnGotoNewClicked\n");

	OnLeaveCellGrid();

	if (m_ctrlGrid.GetRows() == m_ctrlGrid.GetRow() + 1)
	{
		m_ctrlGrid.SetRows(m_ctrlGrid.GetRows() + 1);
	}
	long nRow = m_ctrlGrid.GetRows() - 1;

	OnLeaveCellGrid();
	m_ctrlGrid.SetRow(__max(nRow, 1));
	m_ctrlGrid.GetCellLeft();
	OnRowColChangeGrid();

	m_ctrlAssocRxn.SetFocus();
	m_ctrlAssocRxn.SetSel(0, -1);
}

void CKPSurfaceSpeciesPg1::OnGotoNextClicked() 
{
	TRACE("OnGotoNextClicked\n");

	long nRow = m_ctrlGrid.GetRow();
	if (nRow != m_ctrlGrid.GetRows() - 1)
	{
		OnLeaveCellGrid();
		m_ctrlGrid.SetRow(__min(nRow + 1, m_ctrlGrid.GetRows() - 1));
		m_ctrlGrid.GetCellLeft();
		OnRowColChangeGrid();
	}

	ResetFocus();
}

void CKPSurfaceSpeciesPg1::OnGotoPrevClicked() 
{
	TRACE("OnGotoPrevClicked\n");

	long nRow = m_ctrlGrid.GetRow();
	if (nRow != 1)
	{
		OnLeaveCellGrid();
		m_ctrlGrid.SetRow(__max(nRow - 1, 1));
		m_ctrlGrid.GetCellLeft(); // scroll into view
		OnRowColChangeGrid();
	}

	ResetFocus();
}

void CKPSurfaceSpeciesPg1::ResetFocus()
{
	if (m_hWndLastControl != NULL)
	{
		// restore focus and selection last control
		::SetFocus(m_hWndLastControl);
		if (m_bEditLastControl) // select edit item
			::SendMessage(m_hWndLastControl, EM_SETSEL, 0, -1);
	}
}

LRESULT CKPSurfaceSpeciesPg1::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
{
	UINT nID = wParam;
	UNUSED_ALWAYS(lParam);
	switch (nID)
	{
	case IDC_MSHFLEXGRID1:
		OnEnterCellGrid();
		break;
	}
	return 0;
}

void CKPSurfaceSpeciesPg1::OnKeyDownGrid(short FAR* KeyCode, short Shift) 
{
	// Add your control notification handler code here
	UNUSED(Shift);

	switch (*KeyCode)
	{
	case VK_DELETE:
		m_ctrlGrid.ClearContents();	
		break;
	}	
}
