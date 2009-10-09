// KPPhasesPg1.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "KPPhasesPg1.h"

#include "DDX_DDV.h"
#include "EqnChecker2.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CKPPhasesPg1, baseCKPPhasesPg1)

// unused
#undef IDC_STATIC_ASSOC_RXN
#undef IDC_EDIT_ASSOC_RXN
#undef IDC_STATIC_MOLE_BAL
#undef IDC_EDIT_MOLE_BAL
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

/////////////////////////////////////////////////////////////////////////////
// CKPPhasesPg1 property page
const long NCOL_IMAGE          = 0;
const long NCOL_PHASE_NAME     = 1;
const long NCOL_RXN            = 2;
const long NCOL_LOGK           = 3;
const long NCOL_DELTA_H        = 4;
const long NCOL_DELTA_H_UNITS  = 5;
const long NCOL_A1             = 6;
const long NCOL_A2             = 7;
const long NCOL_A3             = 8;
const long NCOL_A4             = 9;
const long NCOL_A5             = 10;
const long NCOL_A6             = 11;
const long NCOL_CHECK          = 12;

CKPPhasesPg1::CKPPhasesPg1() : baseCKPPhasesPg1(CKPPhasesPg1::IDD)
{
	//{{AFX_DATA_INIT(CKPPhasesPg1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pictHolderRightArrow.CreateFromIcon(IDI_RIGHT_ARROW2);
	m_pictHolderStar.CreateFromIcon(IDI_STAR);

	m_hWndLastControl  = NULL;
	m_bEditLastControl = FALSE;

	m_bIgnoreChanges = false;
}

CKPPhasesPg1::~CKPPhasesPg1()
{
}

void CKPPhasesPg1::DoDataExchange(CDataExchange* pDX)
{
	baseCKPPhasesPg1::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKPPhasesPg1)
	DDX_Control(pDX, IDC_EDIT_PHASE_NAME, m_ctrlName);
	DDX_Control(pDX, IDC_DESCRIPTION, m_eInputDesc);
	DDX_Control(pDX, IDC_CHECK_EQUATION, m_ctrlCheckEqn);
	DDX_Control(pDX, IDC_COMBO_DELTA_H_UNITS, m_ctrlDeltaHUnits);
	DDX_Control(pDX, IDC_EDIT_DELTA_H, m_ctrlDeltaH);
	DDX_Control(pDX, IDC_EDIT_RXN, m_ctrlRxn);
	DDX_Control(pDX, IDC_EDIT_LOGK, m_ctrlLogK);
	DDX_Control(pDX, IDC_EDIT_A1, m_ctrlA1);
	DDX_Control(pDX, IDC_EDIT_A2, m_ctrlA2);
	DDX_Control(pDX, IDC_EDIT_A3, m_ctrlA3);
	DDX_Control(pDX, IDC_EDIT_A4, m_ctrlA4);
	DDX_Control(pDX, IDC_EDIT_A5, m_ctrlA5);
	DDX_Control(pDX, IDC_EDIT_A6, m_ctrlA6);
	DDX_Control(pDX, IDC_MSHFLEXGRID1, m_ctrlGrid);
	DDX_Control(pDX, IDC_SRCDBPGCTRL1, m_pager);
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
		std::list<CPhase> listPhase;
		CString strDeltaHUnits;

		for (long nRow = 1; nRow < m_ctrlGrid.GetRows(); ++nRow)
		{
			OnLeaveCellGrid();

			CPhase phase;

			// Line 1 phase name
			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_PHASE_NAME, phase.m_strName);
			if (phase.m_strName.IsEmpty()) continue;
			phase.m_strName.Replace(_T(' '), _T('_'));

// COMMENT: {8/17/2001 12:50:55 PM}			// Line 7: mole balance
// COMMENT: {8/17/2001 12:50:55 PM}			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_MOLE_BAL, phase.m_strMoleBalance);

			//
			// Line 2: Diss. Rxn
			//
			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_RXN, phase.m_strEqn);
			if (phase.m_strEqn.IsEmpty())
			{
				m_ctrlGrid.SetRow(nRow);
				OnRowColChangeGrid();
				pDX->PrepareEditCtrl(IDC_EDIT_RXN);
				::AfxMessageBox(_T("No dissolution equation defined."));
				pDX->Fail();
			}


			//
			// Line 6 -no_check
			//
			CString strCheck;
			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_CHECK, strCheck);
			strCheck.TrimLeft();
			strCheck.TrimRight();
			if (strCheck.IsEmpty())
			{
				if (phase.m_strMoleBalance.IsEmpty())
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
				phase.m_bCheckEqn = false;
				break;
			case _T('y') : case _T('Y') : case _T('t') : case _T('T') :
				phase.m_bCheckEqn = true;
				break;
			default:
				::AfxMessageBox(_T("Invalid value for check equation."));
				pDX->Fail();
				break;
			}

			//
			// Line 2: Diss. Rxn
			//
			if (!checker.CheckDissociationRxn(phase.m_strEqn, phase.m_bCheckEqn))
			{
				m_ctrlGrid.SetRow(nRow);
				OnRowColChangeGrid();
				pDX->PrepareEditCtrl(IDC_EDIT_RXN);
				::AfxMessageBox(checker.GetLastError());
				pDX->Fail();
			}

			try
			{
				// Line 2: log_k
				nCurrentTextBox = IDC_EDIT_LOGK;
				DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_LOGK, phase.m_dLogK);

				// Line 3: delta_h
				nCurrentTextBox = IDC_EDIT_DELTA_H;
				DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_DELTA_H, phase.m_dDeltaH);
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
				phase.m_nDeltaHUnits = kjoules;
			}
			else
			{
				strDeltaHUnits.MakeLower();
				if (strDeltaHUnits.Find(_T("kcal")) == 0)
				{
					phase.m_nDeltaHUnits = kcal;
				}
				else if (strDeltaHUnits.Find(_T("cal"), 0) == 0)
				{
					phase.m_nDeltaHUnits = cal;
				}
				else if (strDeltaHUnits.Find(_T("kj"), 0) == 0)
				{
					phase.m_nDeltaHUnits = kjoules;
				}
				else if (strDeltaHUnits.Find(_T("j"), 0) == 0)
				{
					phase.m_nDeltaHUnits = joules;
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
				DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_A1, phase.m_dA1);
				nCurrentTextBox = IDC_EDIT_A2;
				DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_A2, phase.m_dA2);
				nCurrentTextBox = IDC_EDIT_A3;
				DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_A3, phase.m_dA3);
				nCurrentTextBox = IDC_EDIT_A4;
				DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_A4, phase.m_dA4);
				nCurrentTextBox = IDC_EDIT_A5;
				DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_A5, phase.m_dA5);
				nCurrentTextBox = IDC_EDIT_A6;
				DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_A6, phase.m_dA6);
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

			if (phase.m_dA1 != phase.m_dA1)
			{
				phase.m_dA1 = 0;
			}
			else
			{
				phase.m_bHasAnalExp = true;
			}
			if (phase.m_dA2 != phase.m_dA2)
			{
				phase.m_dA2 = 0;
			}
			else
			{
				phase.m_bHasAnalExp = true;
			}
			if (phase.m_dA3 != phase.m_dA3)
			{
				phase.m_dA3 = 0;
			}
			else
			{
				phase.m_bHasAnalExp = true;
			}
			if (phase.m_dA4 != phase.m_dA4)
			{
				phase.m_dA4 = 0;
			}
			else
			{
				phase.m_bHasAnalExp = true;
			}
			if (phase.m_dA5 != phase.m_dA5)
			{
				phase.m_dA5 = 0;
			}
			else
			{
				phase.m_bHasAnalExp = true;
			}
			if (phase.m_dA6 != phase.m_dA6)
			{
				phase.m_dA6 = 0;
			}
			else
			{
				phase.m_bHasAnalExp = true;
			}

			// must define either log_k or anal. expression
			if (!phase.m_bHasAnalExp && (phase.m_dLogK != phase.m_dLogK))
			{
				m_ctrlGrid.SetRow(nRow);
				OnRowColChangeGrid();
				pDX->PrepareEditCtrl(IDC_EDIT_LOGK);
				::AfxMessageBox(_T("Either a Log K or an analytical expression must be defined."));
				pDX->Fail();
			}

// COMMENT: {8/17/2001 12:51:35 PM}			// Line 5: -gamma
// COMMENT: {8/17/2001 12:51:35 PM}			// Debye-Huckel a
// COMMENT: {8/17/2001 12:51:35 PM}			if (phase.m_nActType == CPhase::AT_DEBYE_HUCKEL)
// COMMENT: {8/17/2001 12:51:35 PM}			{
// COMMENT: {8/17/2001 12:51:35 PM}				try
// COMMENT: {8/17/2001 12:51:35 PM}				{
// COMMENT: {8/17/2001 12:51:35 PM}					// Debye-Huckel a
// COMMENT: {8/17/2001 12:51:35 PM}					nCurrentTextBox = IDC_EDIT_DHA;
// COMMENT: {8/17/2001 12:51:35 PM}					DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_DHA, phase.m_dDHa);
// COMMENT: {8/17/2001 12:51:35 PM}
// COMMENT: {8/17/2001 12:51:35 PM}					// Debye-Huckel b
// COMMENT: {8/17/2001 12:51:35 PM}					nCurrentTextBox = IDC_EDIT_DHB;
// COMMENT: {8/17/2001 12:51:35 PM}					DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_DHB, phase.m_dDHb);
// COMMENT: {8/17/2001 12:51:35 PM}				}
// COMMENT: {8/17/2001 12:51:35 PM}				catch(CUserException* pE)
// COMMENT: {8/17/2001 12:51:35 PM}				{
// COMMENT: {8/17/2001 12:51:35 PM}					OnLeaveCellGrid();
// COMMENT: {8/17/2001 12:51:35 PM}					pE->Delete();
// COMMENT: {8/17/2001 12:51:35 PM}					m_ctrlGrid.SetRow(nRow);
// COMMENT: {8/17/2001 12:51:35 PM}					OnRowColChangeGrid();
// COMMENT: {8/17/2001 12:51:35 PM}					pDX->PrepareEditCtrl(nCurrentTextBox);
// COMMENT: {8/17/2001 12:51:35 PM}					pDX->Fail();
// COMMENT: {8/17/2001 12:51:35 PM}				}
// COMMENT: {8/17/2001 12:51:35 PM}			}
// COMMENT: {8/17/2001 12:51:35 PM}			else if (phase.m_nActType == CPhase::AT_LLNL_DH)
// COMMENT: {8/17/2001 12:51:35 PM}			{
// COMMENT: {8/17/2001 12:51:35 PM}				try
// COMMENT: {8/17/2001 12:51:35 PM}				{
// COMMENT: {8/17/2001 12:51:35 PM}					// LLNL Debye-Huckel a
// COMMENT: {8/17/2001 12:51:35 PM}					nCurrentTextBox = IDC_EDIT_LLNL_DHA;
// COMMENT: {8/17/2001 12:51:35 PM}					DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_DHA, phase.m_dDHa);
// COMMENT: {8/17/2001 12:51:35 PM}				}
// COMMENT: {8/17/2001 12:51:35 PM}				catch(CUserException* pE)
// COMMENT: {8/17/2001 12:51:35 PM}				{
// COMMENT: {8/17/2001 12:51:35 PM}					OnLeaveCellGrid();
// COMMENT: {8/17/2001 12:51:35 PM}					pE->Delete();
// COMMENT: {8/17/2001 12:51:35 PM}					m_ctrlGrid.SetRow(nRow);
// COMMENT: {8/17/2001 12:51:35 PM}					OnRowColChangeGrid();
// COMMENT: {8/17/2001 12:51:35 PM}					pDX->PrepareEditCtrl(nCurrentTextBox);
// COMMENT: {8/17/2001 12:51:35 PM}					pDX->Fail();
// COMMENT: {8/17/2001 12:51:35 PM}				}
// COMMENT: {8/17/2001 12:51:35 PM}			}

			listPhase.push_back(phase);
		}
		// if here list is valid and can be assigned to the member variable
		m_listPhase.assign(listPhase.begin(), listPhase.end());
	}
	else
	{
		// size grid
		m_ctrlGrid.SetRows(m_listPhase.size() + 2);

#ifdef _DEBUG
		CEqnChecker2 testChecker;
#endif
		CString strDeltaHUnits;
		CString strCheck;
		CString strActType;

		std::list<CPhase>::const_iterator cIter = m_listPhase.begin();
		for (long nRow = 1; cIter != m_listPhase.end(); ++cIter, ++nRow)
		{
			CPhase phase(*cIter);
			ASSERT(testChecker.CheckDissociationRxn(phase.m_strEqn, phase.m_bCheckEqn));

			// phase name
			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_PHASE_NAME, phase.m_strName);

			// diss. reaction
			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_RXN, phase.m_strEqn);

			// log_k
			DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_LOGK, phase.m_dLogK);

			// delta_h
			DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_DELTA_H, phase.m_dDeltaH);

			// delta_h [units]
			switch (phase.m_nDeltaHUnits)
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
			if (phase.m_bHasAnalExp)
			{
				DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_A1, phase.m_dA1);
				DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_A2, phase.m_dA2);
				DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_A3, phase.m_dA3);
				DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_A4, phase.m_dA4);
				DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_A5, phase.m_dA5);
				DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_A6, phase.m_dA6);
			}

			// no_check
			strCheck =  (phase.m_bCheckEqn) ? _T("Yes") : _T("No");
			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_CHECK, strCheck);

// COMMENT: {8/17/2001 12:55:13 PM}			// mole balance
// COMMENT: {8/17/2001 12:55:13 PM}			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_MOLE_BAL, phase.m_strMoleBalance);
// COMMENT: {8/17/2001 12:55:13 PM}

// COMMENT: {8/17/2001 12:55:22 PM}			// Activity Type
// COMMENT: {8/17/2001 12:55:22 PM}			CPhase::ActType nType = phase.m_nActType;
// COMMENT: {8/17/2001 12:55:22 PM}			switch (nType)
// COMMENT: {8/17/2001 12:55:22 PM}			{
// COMMENT: {8/17/2001 12:55:22 PM}			case CPhase::AT_DAVIES:
// COMMENT: {8/17/2001 12:55:22 PM}				break;
// COMMENT: {8/17/2001 12:55:22 PM}
// COMMENT: {8/17/2001 12:55:22 PM}			case CPhase::AT_DEBYE_HUCKEL:
// COMMENT: {8/17/2001 12:55:22 PM}				DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_DHA, phase.m_dDHa);
// COMMENT: {8/17/2001 12:55:22 PM}				DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_DHB, phase.m_dDHb);
// COMMENT: {8/17/2001 12:55:22 PM}				break;
// COMMENT: {8/17/2001 12:55:22 PM}
// COMMENT: {8/17/2001 12:55:22 PM}			case CPhase::AT_LLNL_DH:
// COMMENT: {8/17/2001 12:55:22 PM}				DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_DHA, phase.m_dDHa);
// COMMENT: {8/17/2001 12:55:22 PM}				if (!m_bEnableLLNL) EnableLLNL(true);
// COMMENT: {8/17/2001 12:55:22 PM}				break;
// COMMENT: {8/17/2001 12:55:22 PM}
// COMMENT: {8/17/2001 12:55:22 PM}			case CPhase::AT_LLNL_DH_CO2:
// COMMENT: {8/17/2001 12:55:22 PM}				if (!m_bEnableLLNL) EnableLLNL(true);
// COMMENT: {8/17/2001 12:55:22 PM}				break;
// COMMENT: {8/17/2001 12:55:22 PM}
// COMMENT: {8/17/2001 12:55:22 PM}			default:
// COMMENT: {8/17/2001 12:55:22 PM}				ASSERT(FALSE);
// COMMENT: {8/17/2001 12:55:22 PM}				nType = CPhase::AT_DAVIES;
// COMMENT: {8/17/2001 12:55:22 PM}				break;
// COMMENT: {8/17/2001 12:55:22 PM}			}
// COMMENT: {8/17/2001 12:55:22 PM}			std::map<CPhase::ActType, CString>::iterator iterMap = m_mapActTypeToStr.find(nType);
// COMMENT: {8/17/2001 12:55:22 PM}			if (iterMap != m_mapActTypeToStr.end())
// COMMENT: {8/17/2001 12:55:22 PM}			{
// COMMENT: {8/17/2001 12:55:22 PM}				DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_ACT_TYPE, iterMap->second);
// COMMENT: {8/17/2001 12:55:22 PM}			}
		}

		m_ctrlGrid.SetCol(0);
		m_ctrlGrid.SetRow(m_listPhase.size() + 1);
		m_ctrlGrid.SetCellPictureAlignment(flexAlignCenterCenter);
		m_ctrlGrid.SetRefCellPicture(m_pictHolderStar.GetPictureDispatch());
		// set initial pos
		m_ctrlGrid.SetRow(1);
		m_ctrlGrid.SetCol(1);
		OnRowColChangeGrid();
	}
}


BEGIN_MESSAGE_MAP(CKPPhasesPg1, baseCKPPhasesPg1)
	//{{AFX_MSG_MAP(CKPPhasesPg1)
	ON_EN_CHANGE(IDC_EDIT_RXN, OnChangeEditRxn)
	ON_EN_CHANGE(IDC_EDIT_LOGK, OnChangeEditLogk)
	ON_EN_KILLFOCUS(IDC_EDIT_RXN, OnKillfocusEditRxn)
	ON_EN_CHANGE(IDC_EDIT_DELTA_H, OnChangeEditDeltaH)
	ON_CBN_SELCHANGE(IDC_COMBO_DELTA_H_UNITS, OnSelchangeComboDeltaHUnits)
	ON_BN_CLICKED(IDC_CHECK_EQUATION, OnCheckEquation)
	ON_EN_CHANGE(IDC_EDIT_A1, OnChangeEditA1)
	ON_EN_CHANGE(IDC_EDIT_A2, OnChangeEditA2)
	ON_EN_CHANGE(IDC_EDIT_A3, OnChangeEditA3)
	ON_EN_CHANGE(IDC_EDIT_A4, OnChangeEditA4)
	ON_EN_CHANGE(IDC_EDIT_A5, OnChangeEditA5)
	ON_EN_CHANGE(IDC_EDIT_A6, OnChangeEditA6)
	ON_EN_KILLFOCUS(IDC_EDIT_LOGK, OnKillfocusEditLogk)
	ON_EN_KILLFOCUS(IDC_EDIT_DELTA_H, OnKillfocusEditDeltaH)
	ON_EN_KILLFOCUS(IDC_EDIT_A1, OnKillfocusEditA1)
	ON_EN_KILLFOCUS(IDC_EDIT_A2, OnKillfocusEditA2)
	ON_EN_KILLFOCUS(IDC_EDIT_A3, OnKillfocusEditA3)
	ON_EN_KILLFOCUS(IDC_EDIT_A4, OnKillfocusEditA4)
	ON_EN_KILLFOCUS(IDC_EDIT_A5, OnKillfocusEditA5)
	ON_EN_KILLFOCUS(IDC_EDIT_A6, OnKillfocusEditA6)
	ON_EN_SETFOCUS(IDC_EDIT_RXN, OnSetfocusEditRxn)
	ON_EN_SETFOCUS(IDC_EDIT_LOGK, OnSetfocusEditLogk)
	ON_EN_SETFOCUS(IDC_EDIT_DELTA_H, OnSetfocusEditDeltaH)
	ON_CBN_SETFOCUS(IDC_COMBO_DELTA_H_UNITS, OnSetfocusComboDeltaHUnits)
	ON_EN_SETFOCUS(IDC_EDIT_A1, OnSetfocusEditA1)
	ON_EN_SETFOCUS(IDC_EDIT_A2, OnSetfocusEditA2)
	ON_EN_SETFOCUS(IDC_EDIT_A3, OnSetfocusEditA3)
	ON_EN_SETFOCUS(IDC_EDIT_A4, OnSetfocusEditA4)
	ON_EN_SETFOCUS(IDC_EDIT_A5, OnSetfocusEditA5)
	ON_EN_SETFOCUS(IDC_EDIT_A6, OnSetfocusEditA6)
	ON_WM_HELPINFO()
	ON_EN_CHANGE(IDC_EDIT_PHASE_NAME, OnChangeEditPhaseName)
	ON_EN_KILLFOCUS(IDC_EDIT_PHASE_NAME, OnKillfocusEditPhaseName)
	ON_EN_SETFOCUS(IDC_EDIT_PHASE_NAME, OnSetfocusEditPhaseName)
	//}}AFX_MSG_MAP
	// custom edit grid notifications
	ON_MESSAGE(EGN_BEGINCELLEDIT, OnBeginCellEdit)
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
	ON_MESSAGE(EGN_SETFOCUS, OnSetfocusGrid)
	ON_MESSAGE(EGN_CHANGE, OnChange)
	ON_MESSAGE(EGN_ALLOW_INSERT_ROW, OnAllowInsertRow)
	ON_MESSAGE(EGN_INSERT_ROW, OnInsertRow)
	ON_MESSAGE(EGN_ALLOW_DELETE_ROW, OnAllowDeleteRow)
	ON_MESSAGE(EGN_DELETE_ROW, OnDeleteRow)
// COMMENT: {8/16/2001 6:00:18 PM}	// custom radio notifications
// COMMENT: {8/16/2001 6:00:18 PM}	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_DAVIES, IDC_RADIO_LLNL_CO2, OnActCoefRadioClicked)
// COMMENT: {8/16/2001 6:00:18 PM}	ON_CONTROL_RANGE(BN_SETFOCUS, IDC_RADIO_DAVIES, IDC_RADIO_LLNL_CO2, OnSetfocusRadio)
END_MESSAGE_MAP()

void CKPPhasesPg1::InitGrid(CDataExchange* pDX, int nIDC)
{
	UNUSED(pDX);
	ASSERT(nIDC == m_ctrlGrid.GetDlgCtrlID());
	UNUSED(nIDC);   // unused in release builds

	// set column widths
	m_ctrlGrid.SetColWidth(NCOL_IMAGE, 0, 300);
	m_ctrlGrid.SetColWidth(NCOL_PHASE_NAME, 0, 1100);
	m_ctrlGrid.SetColWidth(NCOL_RXN, 0, 1400);

	// set grid titles
    m_ctrlGrid.SetTextMatrix( 0, NCOL_PHASE_NAME,    _T("Phase name"));
    m_ctrlGrid.SetTextMatrix( 0, NCOL_RXN,           _T("Diss. Rxn"));
    m_ctrlGrid.SetTextMatrix( 0, NCOL_LOGK,          _T("Log K"));
    m_ctrlGrid.SetTextMatrix( 0, NCOL_DELTA_H,       _T("Delta H"));
    m_ctrlGrid.SetTextMatrix( 0, NCOL_DELTA_H_UNITS, _T("D-H units"));
    m_ctrlGrid.SetTextMatrix( 0, NCOL_A1,            _T("A1"));
    m_ctrlGrid.SetTextMatrix( 0, NCOL_A2,            _T("A2"));
    m_ctrlGrid.SetTextMatrix( 0, NCOL_A3,            _T("A3"));
    m_ctrlGrid.SetTextMatrix( 0, NCOL_A4,            _T("A4"));
    m_ctrlGrid.SetTextMatrix( 0, NCOL_A5,            _T("A5"));
    m_ctrlGrid.SetTextMatrix( 0, NCOL_A6,            _T("A6"));
// COMMENT: {8/16/2001 6:08:07 PM}    m_ctrlGrid.SetTextMatrix( 0, NCOL_ACT_TYPE,      _T("Activity")); // Davies; D-H; LLNL-CO2; LLNL-DH
// COMMENT: {8/16/2001 6:08:07 PM}    m_ctrlGrid.SetTextMatrix( 0, NCOL_DHA,           _T("Act. a"));
// COMMENT: {8/16/2001 6:08:07 PM}    m_ctrlGrid.SetTextMatrix( 0, NCOL_DHB,           _T("Act. b"));
    m_ctrlGrid.SetTextMatrix( 0, NCOL_CHECK,         _T("Check"));
// COMMENT: {8/16/2001 6:18:26 PM}    m_ctrlGrid.SetTextMatrix( 0, NCOL_MOLE_BAL,      _T("Mol bal"));

	// set alignment
	m_ctrlGrid.SetColAlignment(NCOL_PHASE_NAME, flexAlignLeftCenter);
	m_ctrlGrid.SetColAlignment(NCOL_RXN, flexAlignLeftCenter);

	// set starting position
	m_ctrlGrid.SetRow(1);
	m_ctrlGrid.SetCol(1);
}

BEGIN_EVENTSINK_MAP(CKPPhasesPg1, baseCKPPhasesPg1)
    //{{AFX_EVENTSINK_MAP(CKPPhasesPg1)
	ON_EVENT(CKPPhasesPg1, IDC_MSHFLEXGRID1, 70 /* RowColChange */, OnRowColChangeGrid, VTS_NONE)
	ON_EVENT(CKPPhasesPg1, IDC_SRCDBPGCTRL1, 3 /* GotoNextClicked */, OnGotoNextClicked, VTS_NONE)
	ON_EVENT(CKPPhasesPg1, IDC_MSHFLEXGRID1, 72 /* LeaveCell */, OnLeaveCellGrid, VTS_NONE)
	ON_EVENT(CKPPhasesPg1, IDC_SRCDBPGCTRL1, 1 /* GotoFirstClicked */, OnGotoFirstClicked, VTS_NONE)
	ON_EVENT(CKPPhasesPg1, IDC_SRCDBPGCTRL1, 4 /* GotoLastClicked */, OnGotoLastClicked, VTS_NONE)
	ON_EVENT(CKPPhasesPg1, IDC_SRCDBPGCTRL1, 5 /* GotoNewClicked */, OnGotoNewClicked, VTS_NONE)
	ON_EVENT(CKPPhasesPg1, IDC_SRCDBPGCTRL1, 2 /* GotoPrevClicked */, OnGotoPrevClicked, VTS_NONE)
	ON_EVENT(CKPPhasesPg1, IDC_MSHFLEXGRID1, -601 /* DblClick */, OnDblClickGrid, VTS_NONE)
	ON_EVENT(CKPPhasesPg1, IDC_MSHFLEXGRID1, 71 /* EnterCell */, OnEnterCellGrid, VTS_NONE)
	ON_EVENT(CKPPhasesPg1, IDC_MSHFLEXGRID1, -602 /* KeyDown */, OnKeyDownGrid, VTS_PI2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CKPPhasesPg1::OnRowColChangeGrid() 
{
	TRACE("OnRowColChangeGrid\n");

	ASSERT(m_bIgnoreChanges == false);
	m_bIgnoreChanges = true;
	TRACE("Setting m_bIgnoreChanges = true in OnRowColChangeGrid\n");

	long nRow = m_ctrlGrid.GetRow();
	long nCol = m_ctrlGrid.GetCol();

	// phase name
	m_ctrlName.SetWindowText(m_ctrlGrid.GetTextMatrix(nRow, NCOL_PHASE_NAME));

	// rxn
	m_ctrlRxn.SetWindowText(m_ctrlGrid.GetTextMatrix(nRow, NCOL_RXN));  // implicit OnChangeEditRxn

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
	m_ctrlA6.SetWindowText(m_ctrlGrid.GetTextMatrix(nRow, NCOL_A6)); // implicit OnChangeEditA6

	// set arrow image
	m_ctrlGrid.SetCol(0);
	m_ctrlGrid.SetRefCellPicture(m_pictHolderRightArrow.GetPictureDispatch());
	m_ctrlGrid.SetCellPictureAlignment(flexAlignCenterCenter);

// COMMENT: {8/16/2001 6:08:59 PM}	// activity type
// COMMENT: {8/16/2001 6:08:59 PM}	CSpecies::ActType nType = CSpecies::AT_DAVIES;
// COMMENT: {8/16/2001 6:08:59 PM}	CString strActType = m_ctrlGrid.GetTextMatrix(nRow, NCOL_ACT_TYPE);
// COMMENT: {8/16/2001 6:08:59 PM}	if (!strActType.IsEmpty())
// COMMENT: {8/16/2001 6:08:59 PM}	{
// COMMENT: {8/16/2001 6:08:59 PM}		strActType.MakeUpper();
// COMMENT: {8/16/2001 6:08:59 PM}		std::map<CString, CSpecies::ActType>::iterator iter = m_mapStrToActType.find(strActType);
// COMMENT: {8/16/2001 6:08:59 PM}		if (iter != m_mapStrToActType.end())
// COMMENT: {8/16/2001 6:08:59 PM}		{
// COMMENT: {8/16/2001 6:08:59 PM}			nType = iter->second;
// COMMENT: {8/16/2001 6:08:59 PM}		}
// COMMENT: {8/16/2001 6:08:59 PM}	}
// COMMENT: {8/16/2001 6:08:59 PM}	else
// COMMENT: {8/16/2001 6:08:59 PM}	{
// COMMENT: {8/16/2001 6:08:59 PM}		m_ctrlGrid.SetTextMatrix(nRow, NCOL_ACT_TYPE, m_mapActTypeToStr[CSpecies::AT_DAVIES]);
// COMMENT: {8/16/2001 6:08:59 PM}	}
// COMMENT: {8/16/2001 6:08:59 PM}	UINT nID = IDC_RADIO_DAVIES;
// COMMENT: {8/16/2001 6:08:59 PM}	switch (nType)
// COMMENT: {8/16/2001 6:08:59 PM}	{
// COMMENT: {8/16/2001 6:08:59 PM}	case CSpecies::AT_DAVIES:
// COMMENT: {8/16/2001 6:08:59 PM}		nID = IDC_RADIO_DAVIES;
// COMMENT: {8/16/2001 6:08:59 PM}		break;
// COMMENT: {8/16/2001 6:08:59 PM}	case CSpecies::AT_DEBYE_HUCKEL:
// COMMENT: {8/16/2001 6:08:59 PM}		nID = IDC_RADIO_DH;
// COMMENT: {8/16/2001 6:08:59 PM}		break;
// COMMENT: {8/16/2001 6:08:59 PM}	case CSpecies::AT_LLNL_DH:
// COMMENT: {8/16/2001 6:08:59 PM}		nID = IDC_RADIO_LLNL_DH;
// COMMENT: {8/16/2001 6:08:59 PM}		if (!m_bEnableLLNL) EnableLLNL(true);
// COMMENT: {8/16/2001 6:08:59 PM}		break;
// COMMENT: {8/16/2001 6:08:59 PM}	case CSpecies::AT_LLNL_DH_CO2:
// COMMENT: {8/16/2001 6:08:59 PM}		nID = IDC_RADIO_LLNL_CO2;
// COMMENT: {8/16/2001 6:08:59 PM}		if (!m_bEnableLLNL) EnableLLNL(true);
// COMMENT: {8/16/2001 6:08:59 PM}		break;
// COMMENT: {8/16/2001 6:08:59 PM}	default:
// COMMENT: {8/16/2001 6:08:59 PM}		ASSERT(FALSE);
// COMMENT: {8/16/2001 6:08:59 PM}		break;
// COMMENT: {8/16/2001 6:08:59 PM}	}
// COMMENT: {8/16/2001 6:08:59 PM}
// COMMENT: {8/16/2001 6:08:59 PM}	CheckRadioButton(IDC_RADIO_DAVIES, IDC_RADIO_LLNL_CO2, nID);				
// COMMENT: {8/16/2001 6:08:59 PM}	GetDlgItem(IDC_STATIC_DHA)->EnableWindow(nType == CSpecies::AT_DEBYE_HUCKEL);
// COMMENT: {8/16/2001 6:08:59 PM}	GetDlgItem(IDC_EDIT_DHA)->EnableWindow(nType == CSpecies::AT_DEBYE_HUCKEL);
// COMMENT: {8/16/2001 6:08:59 PM}	GetDlgItem(IDC_STATIC_DHB)->EnableWindow(nType == CSpecies::AT_DEBYE_HUCKEL);
// COMMENT: {8/16/2001 6:08:59 PM}	GetDlgItem(IDC_EDIT_DHB)->EnableWindow(nType == CSpecies::AT_DEBYE_HUCKEL);
// COMMENT: {8/16/2001 6:08:59 PM}	GetDlgItem(IDC_STATIC_LLNL_DHA)->EnableWindow(nType == CSpecies::AT_LLNL_DH);
// COMMENT: {8/16/2001 6:08:59 PM}	GetDlgItem(IDC_EDIT_LLNL_DHA)->EnableWindow(nType == CSpecies::AT_LLNL_DH);
// COMMENT: {8/16/2001 6:08:59 PM}
// COMMENT: {8/16/2001 6:08:59 PM}	// D-H parameter a
// COMMENT: {8/16/2001 6:08:59 PM}	m_ctrlDHa.SetWindowText(m_ctrlGrid.GetTextMatrix(nRow, NCOL_DHA)); // implicit OnChangeEditDha
// COMMENT: {8/16/2001 6:08:59 PM}
// COMMENT: {8/16/2001 6:08:59 PM}	// D-H parameter b
// COMMENT: {8/16/2001 6:08:59 PM}	m_ctrlDHb.SetWindowText(m_ctrlGrid.GetTextMatrix(nRow, NCOL_DHB)); // implicit OnChangeEditDhb
// COMMENT: {8/16/2001 6:08:59 PM}
// COMMENT: {8/16/2001 6:08:59 PM}	// LLNL parameter a
// COMMENT: {8/16/2001 6:08:59 PM}	m_ctrlLLNLa.SetWindowText(m_ctrlGrid.GetTextMatrix(nRow, NCOL_DHA));

	// check eqn
	CString strCheck = m_ctrlGrid.GetTextMatrix(nRow, NCOL_CHECK);
// COMMENT: {8/16/2001 6:18:57 PM}	CWnd* pStatic = GetDlgItem(IDC_STATIC_MOLE_BAL);
	if (strCheck.GetLength() > 0)
	{
		switch (strCheck.GetAt(0))
		{
		case _T('n') : case _T('N') : case _T('f') : case _T('F') :
			m_ctrlCheckEqn.SetCheck(BST_UNCHECKED);
// COMMENT: {8/16/2001 6:18:49 PM}			m_ctrlMoleBal.EnableWindow(TRUE);
// COMMENT: {8/16/2001 6:18:49 PM}			pStatic->EnableWindow(TRUE);
			break;
		case _T('y') : case _T('Y') : case _T('t') : case _T('T') :
			m_ctrlCheckEqn.SetCheck(BST_CHECKED);
// COMMENT: {8/16/2001 6:18:53 PM}			m_ctrlMoleBal.EnableWindow(FALSE);
// COMMENT: {8/16/2001 6:18:53 PM}			pStatic->EnableWindow(FALSE);
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

// COMMENT: {8/16/2001 6:19:08 PM}	// mole balance
// COMMENT: {8/16/2001 6:19:08 PM}	m_ctrlMoleBal.SetWindowText(m_ctrlGrid.GetTextMatrix(nRow, NCOL_MOLE_BAL)); // implicit OnChangeEditMoleBal

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

void CKPPhasesPg1::OnGotoNextClicked() 
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

void CKPPhasesPg1::OnLeaveCellGrid() 
{
	TRACE("OnLeaveCellGrid\n");

	long nColSave = m_ctrlGrid.GetCol();

	m_ctrlGrid.SetCol(0);
	if (m_ctrlGrid.GetRow() == m_ctrlGrid.GetRows() - 1)
	{
		m_ctrlGrid.SetCellPictureAlignment(flexAlignCenterCenter);
		m_ctrlGrid.SetRefCellPicture(m_pictHolderStar.GetPictureDispatch());
	}
	else
	{
		CPictureHolder pictHolder;
		pictHolder.CreateEmpty();
		m_ctrlGrid.SetRefCellPicture(pictHolder.GetPictureDispatch());
	}	
	m_ctrlGrid.SetCol(nColSave);
}

void CKPPhasesPg1::OnGotoFirstClicked() 
{
	TRACE("OnGotoFirstClicked\n");

	OnLeaveCellGrid();
	m_ctrlGrid.SetRow(1);
	m_ctrlGrid.GetCellLeft();
	OnRowColChangeGrid();

	ResetFocus();
}

void CKPPhasesPg1::OnGotoLastClicked() 
{
	TRACE("OnGotoLastClicked\n");

	OnLeaveCellGrid();
	long nRow = 1;
	for (; nRow < m_ctrlGrid.GetRows(); ++nRow)
	{
		if (m_ctrlGrid.GetTextMatrix(nRow, NCOL_PHASE_NAME).IsEmpty())
			break;
	}
	m_ctrlGrid.SetRow(__max(nRow - 1, 1));
	m_ctrlGrid.GetCellLeft();
	OnRowColChangeGrid();
	ResetFocus();
}

void CKPPhasesPg1::OnGotoNewClicked() 
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

	m_ctrlName.SetFocus();
	m_ctrlName.SetSel(0, -1);
}

void CKPPhasesPg1::OnGotoPrevClicked() 
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

void CKPPhasesPg1::OnChangeEditPhaseName() 
{
	TRACE("OnChangeEditPhaseName\n");

	if (!m_bIgnoreChanges)
	{
		CString str;
		m_ctrlName.GetWindowText(str);
		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_PHASE_NAME, str);
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

void CKPPhasesPg1::OnChangeEditRxn() 
{
	TRACE("OnChangeEditRxn\n");

	if (!m_bIgnoreChanges)
	{
		CString str;
		m_ctrlRxn.GetWindowText(str);
		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_RXN, str);
// COMMENT: {8/17/2001 1:45:14 PM}		if (str.GetLength() > 0)
// COMMENT: {8/17/2001 1:45:14 PM}		{
// COMMENT: {8/17/2001 1:45:14 PM}			if (m_ctrlGrid.GetRows() - 1 == m_ctrlGrid.GetRow())
// COMMENT: {8/17/2001 1:45:14 PM}			{
// COMMENT: {8/17/2001 1:45:14 PM}				long nRow = m_ctrlGrid.GetRow();
// COMMENT: {8/17/2001 1:45:14 PM}				long nCol = m_ctrlGrid.GetCol();
// COMMENT: {8/17/2001 1:45:14 PM}
// COMMENT: {8/17/2001 1:45:14 PM}				// add new row
// COMMENT: {8/17/2001 1:45:14 PM}				m_ctrlGrid.SetRows(m_ctrlGrid.GetRows() + 1);
// COMMENT: {8/17/2001 1:45:14 PM}
// COMMENT: {8/17/2001 1:45:14 PM}				m_ctrlGrid.SetCol(0);
// COMMENT: {8/17/2001 1:45:14 PM}				m_ctrlGrid.SetRow(m_ctrlGrid.GetRows() - 1);
// COMMENT: {8/17/2001 1:45:14 PM}				m_ctrlGrid.SetCellPictureAlignment(flexAlignCenterCenter);
// COMMENT: {8/17/2001 1:45:14 PM}				m_ctrlGrid.SetRefCellPicture(m_pictHolderStar.GetPictureDispatch());
// COMMENT: {8/17/2001 1:45:14 PM}				m_ctrlGrid.SetRow(nRow);
// COMMENT: {8/17/2001 1:45:14 PM}				m_ctrlGrid.SetCol(nCol);
// COMMENT: {8/17/2001 1:45:14 PM}			}
// COMMENT: {8/17/2001 1:45:14 PM}		}
	}
}

void CKPPhasesPg1::OnChangeEditLogk() 
{
	TRACE("OnChangeEditLogk\n");

	if (!m_bIgnoreChanges)
	{
		CString str;
		m_ctrlLogK.GetWindowText(str);
		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_LOGK, str);
	}
}

void CKPPhasesPg1::OnKillfocusEditPhaseName() 
{
	TRACE("OnKillfocusEditPhaseName\n");
	m_hWndLastControl = m_ctrlName.m_hWnd;
	m_bEditLastControl = TRUE;	
}

void CKPPhasesPg1::OnKillfocusEditRxn() 
{
	TRACE("OnKillfocusEditRxn\n");
	m_hWndLastControl = m_ctrlRxn.m_hWnd;
	m_bEditLastControl = TRUE;	
}

//DEL void CKPPhasesPg1::OnActCoefRadioClicked(UINT nID)
//DEL {
//DEL 	TRACE("OnActCoefRadioClicked\n");
//DEL 
//DEL 	GetDlgItem(IDC_EDIT_DHA)->EnableWindow(FALSE);
//DEL 	GetDlgItem(IDC_EDIT_DHB)->EnableWindow(FALSE);
//DEL 	GetDlgItem(IDC_STATIC_DHA)->EnableWindow(FALSE);
//DEL 	GetDlgItem(IDC_STATIC_DHB)->EnableWindow(FALSE);
//DEL 	GetDlgItem(IDC_EDIT_LLNL_DHA)->EnableWindow(FALSE);
//DEL 	GetDlgItem(IDC_STATIC_LLNL_DHA)->EnableWindow(FALSE);
//DEL 
//DEL 	long nRow = m_ctrlGrid.GetRow();
//DEL 	switch (nID)
//DEL 	{
//DEL 	case IDC_RADIO_DAVIES:
//DEL 		m_ctrlGrid.SetTextMatrix(nRow, NCOL_ACT_TYPE, m_mapActTypeToStr[CSpecies::AT_DAVIES]);
//DEL 		break;
//DEL 
//DEL 	case IDC_RADIO_LLNL_CO2:
//DEL 		m_ctrlGrid.SetTextMatrix(nRow, NCOL_ACT_TYPE, m_mapActTypeToStr[CSpecies::AT_LLNL_DH_CO2]);
//DEL 		break;
//DEL 
//DEL 	case IDC_RADIO_LLNL_DH:
//DEL 		m_ctrlGrid.SetTextMatrix(nRow, NCOL_ACT_TYPE, m_mapActTypeToStr[CSpecies::AT_LLNL_DH]);
//DEL 		GetDlgItem(IDC_STATIC_LLNL_DHA)->EnableWindow(TRUE);
//DEL 		m_ctrlLLNLa.EnableWindow(TRUE);
//DEL 		break;
//DEL 
//DEL 	case IDC_RADIO_DH:
//DEL 		m_ctrlGrid.SetTextMatrix(nRow, NCOL_ACT_TYPE, m_mapActTypeToStr[CSpecies::AT_DEBYE_HUCKEL]);
//DEL 		GetDlgItem(IDC_STATIC_DHA)->EnableWindow(TRUE);
//DEL 		m_ctrlDHa.EnableWindow(TRUE);
//DEL 		GetDlgItem(IDC_STATIC_DHB)->EnableWindow(TRUE);
//DEL 		m_ctrlDHb.EnableWindow(TRUE);
//DEL 		break;
//DEL 
//DEL 	default:
//DEL 		ASSERT(FALSE);
//DEL 		break;
//DEL 	}
//DEL }

//DEL void CKPPhasesPg1::OnChangeEditDha() 
//DEL {
//DEL 	TRACE("OnChangeEditDha\n");
//DEL 
//DEL 	if (!m_bIgnoreChanges)
//DEL 	{
//DEL 		CString str;
//DEL 		m_ctrlDHa.GetWindowText(str);
//DEL 		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_DHA, str);
//DEL 
//DEL 		CString str2;
//DEL 		m_ctrlLLNLa.GetWindowText(str2);
//DEL 		if (str != str2) m_ctrlLLNLa.SetWindowText(str);
//DEL 	}
//DEL }

//DEL void CKPPhasesPg1::OnChangeEditDhb() 
//DEL {
//DEL 	TRACE("OnChangeEditDhb\n");
//DEL 
//DEL 	CString str;
//DEL 	m_ctrlDHb.GetWindowText(str);
//DEL 	m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_DHB, str);
//DEL }

//DEL void CKPPhasesPg1::OnChangeEditLlnlDha() 
//DEL {
//DEL 	TRACE("OnChangeEditLlnlDha\n");
//DEL 
//DEL 	if (!m_bIgnoreChanges)
//DEL 	{
//DEL 		CString str;
//DEL 		m_ctrlLLNLa.GetWindowText(str);
//DEL 		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_DHA, str);
//DEL 
//DEL 		CString str2;
//DEL 		m_ctrlDHa.GetWindowText(str2);
//DEL 		if (str != str2) m_ctrlDHa.SetWindowText(str);
//DEL 	}
//DEL }

void CKPPhasesPg1::OnChangeEditDeltaH() 
{
	TRACE("OnChangeEditDeltaH\n");

	if (!m_bIgnoreChanges)
	{
		CString str;
		m_ctrlDeltaH.GetWindowText(str);
		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_DELTA_H, str);	
	}
}

void CKPPhasesPg1::OnSelchangeComboDeltaHUnits() 
{
	TRACE("OnSelchangeComboDeltaHUnits\n");

	CString strUnits;
	m_ctrlDeltaHUnits.GetWindowText(strUnits);
	m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_DELTA_H_UNITS, strUnits);
}

void CKPPhasesPg1::OnCheckEquation() 
{
	TRACE("OnCheckEquation\n");

	int nCheck = m_ctrlCheckEqn.GetCheck();
	CString strCheck = _T("Yes");
	switch (nCheck)
	{
	case BST_CHECKED:
		break;
	case BST_UNCHECKED:
		strCheck = _T("No");
		break;
	default:
		ASSERT(FALSE);
		break;
	}
	m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_CHECK, strCheck);
}

//DEL void CKPPhasesPg1::OnChangeEditMoleBal() 
//DEL {
//DEL 	TRACE("OnChangeEditMoleBal\n");
//DEL 
//DEL 	if (!m_bIgnoreChanges)
//DEL 	{
//DEL 		CString str;
//DEL 		m_ctrlMoleBal.GetWindowText(str);
//DEL 		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_MOLE_BAL, str);
//DEL 	}
//DEL }

void CKPPhasesPg1::OnChangeEditA1() 
{
	TRACE("OnChangeEditA1\n");

	if (!m_bIgnoreChanges)
	{
		CString str;
		m_ctrlA1.GetWindowText(str);
		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_A1, str);
	}
}

void CKPPhasesPg1::OnChangeEditA2() 
{
	TRACE("OnChangeEditA2\n");

	if (!m_bIgnoreChanges)
	{
		CString str;
		m_ctrlA2.GetWindowText(str);
		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_A2, str);
	}
}

void CKPPhasesPg1::OnChangeEditA3() 
{
	TRACE("OnChangeEditA3\n");

	if (!m_bIgnoreChanges)
	{
		CString str;
		m_ctrlA3.GetWindowText(str);
		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_A3, str);
	}
}

void CKPPhasesPg1::OnChangeEditA4() 
{
	TRACE("OnChangeEditA4\n");

	if (!m_bIgnoreChanges)
	{
		CString str;
		m_ctrlA4.GetWindowText(str);
		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_A4, str);
	}
}

void CKPPhasesPg1::OnChangeEditA5() 
{
	TRACE("OnChangeEditA5\n");

	if (!m_bIgnoreChanges)
	{
		CString str;
		m_ctrlA5.GetWindowText(str);
		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_A5, str);
	}
}

void CKPPhasesPg1::OnChangeEditA6() 
{
	TRACE("OnChangeEditA6\n");

	if (!m_bIgnoreChanges)
	{
		CString str;
		m_ctrlA6.GetWindowText(str);
		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_A6, str);
	}
}

LRESULT CKPPhasesPg1::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
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
		case NCOL_PHASE_NAME:
			break;
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

		case NCOL_A1: case NCOL_A2: case NCOL_A3: case NCOL_A4: case NCOL_A5: case NCOL_A6:
			break;

// COMMENT: {8/16/2001 6:10:11 PM}		case NCOL_ACT_TYPE :
// COMMENT: {8/16/2001 6:10:11 PM}			if (pInfo->item.hWndCombo)
// COMMENT: {8/16/2001 6:10:11 PM}			{
// COMMENT: {8/16/2001 6:10:11 PM}				CComboBox* pCombo = (CComboBox*)CWnd::FromHandle(pInfo->item.hWndCombo);
// COMMENT: {8/16/2001 6:10:11 PM}				pCombo->ResetContent();
// COMMENT: {8/16/2001 6:10:11 PM}				std::map<CString, CSpecies::ActType>::iterator cIter = m_mapStrToActType.begin();
// COMMENT: {8/16/2001 6:10:11 PM}				for (; cIter != m_mapStrToActType.end(); ++cIter)
// COMMENT: {8/16/2001 6:10:11 PM}				{
// COMMENT: {8/16/2001 6:10:11 PM}					if (!m_bEnableLLNL)
// COMMENT: {8/16/2001 6:10:11 PM}					{
// COMMENT: {8/16/2001 6:10:11 PM}						if (cIter->second == CSpecies::AT_LLNL_DH) continue;
// COMMENT: {8/16/2001 6:10:11 PM}						if (cIter->second == CSpecies::AT_LLNL_DH_CO2) continue;
// COMMENT: {8/16/2001 6:10:11 PM}					}
// COMMENT: {8/16/2001 6:10:11 PM}					pCombo->AddString(cIter->first);
// COMMENT: {8/16/2001 6:10:11 PM}				}
// COMMENT: {8/16/2001 6:10:11 PM}				pInfo->item.bUseCombo = (pCombo != NULL && pCombo->GetCount());
// COMMENT: {8/16/2001 6:10:11 PM}			}
// COMMENT: {8/16/2001 6:10:11 PM}			else
// COMMENT: {8/16/2001 6:10:11 PM}			{
// COMMENT: {8/16/2001 6:10:11 PM}				pInfo->item.bUseCombo = TRUE;
// COMMENT: {8/16/2001 6:10:11 PM}			}
// COMMENT: {8/16/2001 6:10:11 PM}			break;
// COMMENT: {8/16/2001 6:10:11 PM}
// COMMENT: {8/16/2001 6:10:11 PM}		case NCOL_DHA: case NCOL_DHB:
// COMMENT: {8/16/2001 6:10:11 PM}			break;

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

// COMMENT: {8/16/2001 6:19:16 PM}		case NCOL_MOLE_BAL : // Mol bal
// COMMENT: {8/16/2001 6:19:16 PM}			break;

		default :
			ASSERT(FALSE);
			break;
		}
		break;
	}

	return bDenyEdit;
}

LRESULT CKPPhasesPg1::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
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
		case NCOL_PHASE_NAME:
			if (pInfo->item.pszText == NULL)
			{
				// edit cancelled
				m_ctrlName.SetWindowText(m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol));
			}
			else
			{
				m_ctrlName.SetWindowText(pInfo->item.pszText);
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

		case NCOL_RXN:  // Diss. Rxn
			if (pInfo->item.pszText == NULL)
			{
				// edit cancelled
				m_ctrlRxn.SetWindowText(m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol));
			}
			else
			{
				m_ctrlRxn.SetWindowText(pInfo->item.pszText);
// COMMENT: {8/17/2001 1:48:29 PM}				if (strlen(pInfo->item.pszText) > 0)
// COMMENT: {8/17/2001 1:48:29 PM}				{
// COMMENT: {8/17/2001 1:48:29 PM}					if (m_ctrlGrid.GetRows() - 1 == pInfo->item.iRow)
// COMMENT: {8/17/2001 1:48:29 PM}					{
// COMMENT: {8/17/2001 1:48:29 PM}						// add new row
// COMMENT: {8/17/2001 1:48:29 PM}						m_ctrlGrid.SetRows(m_ctrlGrid.GetRows() + 1);
// COMMENT: {8/17/2001 1:48:29 PM}
// COMMENT: {8/17/2001 1:48:29 PM}						m_ctrlGrid.SetCol(0);
// COMMENT: {8/17/2001 1:48:29 PM}						m_ctrlGrid.SetRow(m_ctrlGrid.GetRows() - 1);
// COMMENT: {8/17/2001 1:48:29 PM}						m_ctrlGrid.SetCellPictureAlignment(flexAlignCenterCenter);
// COMMENT: {8/17/2001 1:48:29 PM}						m_ctrlGrid.SetRefCellPicture(m_pictHolderStar.GetPictureDispatch());
// COMMENT: {8/17/2001 1:48:29 PM}						m_ctrlGrid.SetCol(pInfo->item.iCol);
// COMMENT: {8/17/2001 1:48:29 PM}						m_ctrlGrid.SetRow(pInfo->item.iRow);
// COMMENT: {8/17/2001 1:48:29 PM}					}
// COMMENT: {8/17/2001 1:48:29 PM}				}
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

		case NCOL_A6: // A6
			if (pInfo->item.pszText == NULL)
			{
				// edit cancelled
				m_ctrlA6.SetWindowText(m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol));
			}
			else
			{
				m_ctrlA6.SetWindowText(pInfo->item.pszText);
			}
			break;

// COMMENT: {8/16/2001 6:10:26 PM}		case NCOL_ACT_TYPE: // Activity options
// COMMENT: {8/16/2001 6:10:26 PM}			{
// COMMENT: {8/16/2001 6:10:26 PM}				CSpecies::ActType nType = CSpecies::AT_DAVIES;
// COMMENT: {8/16/2001 6:10:26 PM}
// COMMENT: {8/16/2001 6:10:26 PM}				// pInfo->item.pszText is NULL if edit is cancelled
// COMMENT: {8/16/2001 6:10:26 PM}				CString strActType = (pInfo->item.pszText) ? pInfo->item.pszText : m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol);
// COMMENT: {8/16/2001 6:10:26 PM}
// COMMENT: {8/16/2001 6:10:26 PM}				if (!strActType.IsEmpty())
// COMMENT: {8/16/2001 6:10:26 PM}				{
// COMMENT: {8/16/2001 6:10:26 PM}					strActType.MakeUpper();
// COMMENT: {8/16/2001 6:10:26 PM}					std::map<CString, CSpecies::ActType>::iterator iter = m_mapStrToActType.find(strActType);
// COMMENT: {8/16/2001 6:10:26 PM}					if (iter != m_mapStrToActType.end())
// COMMENT: {8/16/2001 6:10:26 PM}					{
// COMMENT: {8/16/2001 6:10:26 PM}						nType = iter->second;
// COMMENT: {8/16/2001 6:10:26 PM}					}
// COMMENT: {8/16/2001 6:10:26 PM}				}
// COMMENT: {8/16/2001 6:10:26 PM}				else
// COMMENT: {8/16/2001 6:10:26 PM}				{
// COMMENT: {8/16/2001 6:10:26 PM}					m_ctrlGrid.SetTextMatrix(pInfo->item.iRow, pInfo->item.iCol, m_mapActTypeToStr[CSpecies::AT_DAVIES]);
// COMMENT: {8/16/2001 6:10:26 PM}				}
// COMMENT: {8/16/2001 6:10:26 PM}
// COMMENT: {8/16/2001 6:10:26 PM}				UINT nID = IDC_RADIO_DAVIES;
// COMMENT: {8/16/2001 6:10:26 PM}				switch (nType)
// COMMENT: {8/16/2001 6:10:26 PM}				{
// COMMENT: {8/16/2001 6:10:26 PM}				case CSpecies::AT_DAVIES:
// COMMENT: {8/16/2001 6:10:26 PM}					nID = IDC_RADIO_DAVIES;
// COMMENT: {8/16/2001 6:10:26 PM}					break;
// COMMENT: {8/16/2001 6:10:26 PM}				case CSpecies::AT_DEBYE_HUCKEL:
// COMMENT: {8/16/2001 6:10:26 PM}					nID = IDC_RADIO_DH;
// COMMENT: {8/16/2001 6:10:26 PM}					break;
// COMMENT: {8/16/2001 6:10:26 PM}				case CSpecies::AT_LLNL_DH:
// COMMENT: {8/16/2001 6:10:26 PM}					nID = IDC_RADIO_LLNL_DH;
// COMMENT: {8/16/2001 6:10:26 PM}					if (!m_bEnableLLNL) EnableLLNL(true);
// COMMENT: {8/16/2001 6:10:26 PM}					break;
// COMMENT: {8/16/2001 6:10:26 PM}				case CSpecies::AT_LLNL_DH_CO2:
// COMMENT: {8/16/2001 6:10:26 PM}					nID = IDC_RADIO_LLNL_CO2;
// COMMENT: {8/16/2001 6:10:26 PM}					if (!m_bEnableLLNL) EnableLLNL(true);
// COMMENT: {8/16/2001 6:10:26 PM}					break;
// COMMENT: {8/16/2001 6:10:26 PM}				}
// COMMENT: {8/16/2001 6:10:26 PM}
// COMMENT: {8/16/2001 6:10:26 PM}				CheckRadioButton(IDC_RADIO_DAVIES, IDC_RADIO_LLNL_CO2, nID);				
// COMMENT: {8/16/2001 6:10:26 PM}				GetDlgItem(IDC_STATIC_DHA)->EnableWindow(nType == CSpecies::AT_DEBYE_HUCKEL);
// COMMENT: {8/16/2001 6:10:26 PM}				GetDlgItem(IDC_EDIT_DHA)->EnableWindow(nType == CSpecies::AT_DEBYE_HUCKEL);
// COMMENT: {8/16/2001 6:10:26 PM}				GetDlgItem(IDC_STATIC_DHB)->EnableWindow(nType == CSpecies::AT_DEBYE_HUCKEL);
// COMMENT: {8/16/2001 6:10:26 PM}				GetDlgItem(IDC_EDIT_DHB)->EnableWindow(nType == CSpecies::AT_DEBYE_HUCKEL);
// COMMENT: {8/16/2001 6:10:26 PM}				GetDlgItem(IDC_STATIC_LLNL_DHA)->EnableWindow(nType == CSpecies::AT_LLNL_DH);
// COMMENT: {8/16/2001 6:10:26 PM}				GetDlgItem(IDC_EDIT_LLNL_DHA)->EnableWindow(nType == CSpecies::AT_LLNL_DH);
// COMMENT: {8/16/2001 6:10:26 PM}			}
// COMMENT: {8/16/2001 6:10:26 PM}			break;
// COMMENT: {8/16/2001 6:10:26 PM}
// COMMENT: {8/16/2001 6:10:26 PM}		case NCOL_DHA: // Act. a
// COMMENT: {8/16/2001 6:10:26 PM}			if (pInfo->item.pszText == NULL)
// COMMENT: {8/16/2001 6:10:26 PM}			{
// COMMENT: {8/16/2001 6:10:26 PM}				// edit cancelled
// COMMENT: {8/16/2001 6:10:26 PM}				m_ctrlDHa.SetWindowText(m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol));
// COMMENT: {8/16/2001 6:10:26 PM}				m_ctrlLLNLa.SetWindowText(m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol));
// COMMENT: {8/16/2001 6:10:26 PM}			}
// COMMENT: {8/16/2001 6:10:26 PM}			else
// COMMENT: {8/16/2001 6:10:26 PM}			{
// COMMENT: {8/16/2001 6:10:26 PM}				m_ctrlDHa.SetWindowText(pInfo->item.pszText);
// COMMENT: {8/16/2001 6:10:26 PM}				m_ctrlLLNLa.SetWindowText(pInfo->item.pszText);
// COMMENT: {8/16/2001 6:10:26 PM}			}
// COMMENT: {8/16/2001 6:10:26 PM}			break;
// COMMENT: {8/16/2001 6:10:26 PM}
// COMMENT: {8/16/2001 6:10:26 PM}		case NCOL_DHB: // Act. b
// COMMENT: {8/16/2001 6:10:26 PM}			if (pInfo->item.pszText == NULL)
// COMMENT: {8/16/2001 6:10:26 PM}			{
// COMMENT: {8/16/2001 6:10:26 PM}				// edit cancelled
// COMMENT: {8/16/2001 6:10:26 PM}				m_ctrlDHb.SetWindowText(m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol));
// COMMENT: {8/16/2001 6:10:26 PM}			}
// COMMENT: {8/16/2001 6:10:26 PM}			else
// COMMENT: {8/16/2001 6:10:26 PM}			{
// COMMENT: {8/16/2001 6:10:26 PM}				m_ctrlDHb.SetWindowText(pInfo->item.pszText);
// COMMENT: {8/16/2001 6:10:26 PM}			}
// COMMENT: {8/16/2001 6:10:26 PM}			break;


		case NCOL_CHECK: // Check eqn
			{
				CString strCheck;
// COMMENT: {8/16/2001 6:19:29 PM}				CWnd* pStatic = GetDlgItem(IDC_STATIC_MOLE_BAL);
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
// COMMENT: {8/16/2001 6:19:36 PM}						m_ctrlMoleBal.EnableWindow(TRUE);
// COMMENT: {8/16/2001 6:19:36 PM}						pStatic->EnableWindow(TRUE);
						break;
					case _T('y') : case _T('Y') : case _T('t') : case _T('T') :
						m_ctrlCheckEqn.SetCheck(BST_CHECKED);
// COMMENT: {8/16/2001 6:19:40 PM}						m_ctrlMoleBal.EnableWindow(FALSE);
// COMMENT: {8/16/2001 6:19:40 PM}						pStatic->EnableWindow(FALSE);
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

// COMMENT: {8/16/2001 6:19:50 PM}		case NCOL_MOLE_BAL: // Mol bal
// COMMENT: {8/16/2001 6:19:50 PM}			if (pInfo->item.pszText == NULL)
// COMMENT: {8/16/2001 6:19:50 PM}			{
// COMMENT: {8/16/2001 6:19:50 PM}				// edit cancelled
// COMMENT: {8/16/2001 6:19:50 PM}				m_ctrlMoleBal.SetWindowText(m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol));
// COMMENT: {8/16/2001 6:19:50 PM}			}
// COMMENT: {8/16/2001 6:19:50 PM}			else
// COMMENT: {8/16/2001 6:19:50 PM}			{
// COMMENT: {8/16/2001 6:19:50 PM}				m_ctrlMoleBal.SetWindowText(pInfo->item.pszText);
// COMMENT: {8/16/2001 6:19:50 PM}			}
// COMMENT: {8/16/2001 6:19:50 PM}			break;

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

void CKPPhasesPg1::OnKillfocusEditLogk() 
{
	m_hWndLastControl = m_ctrlLogK.m_hWnd;
	m_bEditLastControl = TRUE;	
}

void CKPPhasesPg1::OnKillfocusEditDeltaH() 
{
	m_hWndLastControl = m_ctrlDeltaH.m_hWnd;
	m_bEditLastControl = TRUE;	
}

void CKPPhasesPg1::OnKillfocusEditA1() 
{
	m_hWndLastControl = m_ctrlA1.m_hWnd;
	m_bEditLastControl = TRUE;	
}

void CKPPhasesPg1::OnKillfocusEditA2() 
{
	m_hWndLastControl = m_ctrlA2.m_hWnd;
	m_bEditLastControl = TRUE;	
}

void CKPPhasesPg1::OnKillfocusEditA3() 
{
	m_hWndLastControl = m_ctrlA3.m_hWnd;
	m_bEditLastControl = TRUE;	
}

void CKPPhasesPg1::OnKillfocusEditA4() 
{
	m_hWndLastControl = m_ctrlA4.m_hWnd;
	m_bEditLastControl = TRUE;	
}

void CKPPhasesPg1::OnKillfocusEditA5() 
{
	m_hWndLastControl = m_ctrlA5.m_hWnd;
	m_bEditLastControl = TRUE;	
}

void CKPPhasesPg1::OnKillfocusEditA6() 
{
	m_hWndLastControl = m_ctrlA6.m_hWnd;
	m_bEditLastControl = TRUE;	
}

void CKPPhasesPg1::ResetFocus()
{
	if (m_hWndLastControl != NULL)
	{
		// restore focus and selection last control
		::SetFocus(m_hWndLastControl);
		if (m_bEditLastControl) // select edit item
			::SendMessage(m_hWndLastControl, EM_SETSEL, 0, -1);
	}
}

LRESULT CKPPhasesPg1::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
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

LRESULT CKPPhasesPg1::OnChange(WPARAM wParam, LPARAM lParam)
{
	TRACE("CKPPhasesPg1::OnChange\n");

	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	ASSERT(m_bIgnoreChanges == true); // should have vbeen set in OnBeginCellEdit

	switch (nID)
	{
	case IDC_MSHFLEXGRID1 :
		switch ( pInfo->item.iCol )
		{
		case NCOL_PHASE_NAME: // phase name
			m_ctrlName.SetWindowText(pInfo->item.pszText);
			break;

		case NCOL_RXN:  // Diss. Rxn
			m_ctrlRxn.SetWindowText(pInfo->item.pszText);
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

		case NCOL_A6:  // A6
			m_ctrlA6.SetWindowText(pInfo->item.pszText);
			break;

// COMMENT: {8/16/2001 6:10:42 PM}		case NCOL_ACT_TYPE: // Activity
// COMMENT: {8/16/2001 6:10:42 PM}			{
// COMMENT: {8/16/2001 6:10:42 PM}				CSpecies::ActType nType = CSpecies::AT_DAVIES;
// COMMENT: {8/16/2001 6:10:42 PM}				CString strActType = pInfo->item.pszText;
// COMMENT: {8/16/2001 6:10:42 PM}				if (!strActType.IsEmpty())
// COMMENT: {8/16/2001 6:10:42 PM}				{
// COMMENT: {8/16/2001 6:10:42 PM}					strActType.MakeUpper();
// COMMENT: {8/16/2001 6:10:42 PM}					std::map<CString, CSpecies::ActType>::iterator iter = m_mapStrToActType.find(strActType);
// COMMENT: {8/16/2001 6:10:42 PM}					if (iter != m_mapStrToActType.end())
// COMMENT: {8/16/2001 6:10:42 PM}					{
// COMMENT: {8/16/2001 6:10:42 PM}						nType = iter->second;
// COMMENT: {8/16/2001 6:10:42 PM}					}
// COMMENT: {8/16/2001 6:10:42 PM}				}
// COMMENT: {8/16/2001 6:10:42 PM}				else
// COMMENT: {8/16/2001 6:10:42 PM}				{
// COMMENT: {8/16/2001 6:10:42 PM}					m_ctrlGrid.SetTextMatrix(pInfo->item.iRow, NCOL_ACT_TYPE, m_mapActTypeToStr[CSpecies::AT_DAVIES]);
// COMMENT: {8/16/2001 6:10:42 PM}				}
// COMMENT: {8/16/2001 6:10:42 PM}				UINT nID = IDC_RADIO_DAVIES;
// COMMENT: {8/16/2001 6:10:42 PM}				switch (nType)
// COMMENT: {8/16/2001 6:10:42 PM}				{
// COMMENT: {8/16/2001 6:10:42 PM}				case CSpecies::AT_DAVIES:
// COMMENT: {8/16/2001 6:10:42 PM}					nID = IDC_RADIO_DAVIES;
// COMMENT: {8/16/2001 6:10:42 PM}					break;
// COMMENT: {8/16/2001 6:10:42 PM}				case CSpecies::AT_DEBYE_HUCKEL:
// COMMENT: {8/16/2001 6:10:42 PM}					nID = IDC_RADIO_DH;
// COMMENT: {8/16/2001 6:10:42 PM}					break;
// COMMENT: {8/16/2001 6:10:42 PM}				case CSpecies::AT_LLNL_DH:
// COMMENT: {8/16/2001 6:10:42 PM}					nID = IDC_RADIO_LLNL_DH;
// COMMENT: {8/16/2001 6:10:42 PM}					if (!m_bEnableLLNL) EnableLLNL(true);
// COMMENT: {8/16/2001 6:10:42 PM}					break;
// COMMENT: {8/16/2001 6:10:42 PM}				case CSpecies::AT_LLNL_DH_CO2:
// COMMENT: {8/16/2001 6:10:42 PM}					nID = IDC_RADIO_LLNL_CO2;
// COMMENT: {8/16/2001 6:10:42 PM}					if (!m_bEnableLLNL) EnableLLNL(true);
// COMMENT: {8/16/2001 6:10:42 PM}					break;
// COMMENT: {8/16/2001 6:10:42 PM}				default:
// COMMENT: {8/16/2001 6:10:42 PM}					ASSERT(FALSE);
// COMMENT: {8/16/2001 6:10:42 PM}					break;
// COMMENT: {8/16/2001 6:10:42 PM}				}
// COMMENT: {8/16/2001 6:10:42 PM}				CheckRadioButton(IDC_RADIO_DAVIES, IDC_RADIO_LLNL_CO2, nID);				
// COMMENT: {8/16/2001 6:10:42 PM}				GetDlgItem(IDC_STATIC_DHA)->EnableWindow(nType == CSpecies::AT_DEBYE_HUCKEL);
// COMMENT: {8/16/2001 6:10:42 PM}				GetDlgItem(IDC_EDIT_DHA)->EnableWindow(nType == CSpecies::AT_DEBYE_HUCKEL);
// COMMENT: {8/16/2001 6:10:42 PM}				GetDlgItem(IDC_STATIC_DHB)->EnableWindow(nType == CSpecies::AT_DEBYE_HUCKEL);
// COMMENT: {8/16/2001 6:10:42 PM}				GetDlgItem(IDC_EDIT_DHB)->EnableWindow(nType == CSpecies::AT_DEBYE_HUCKEL);
// COMMENT: {8/16/2001 6:10:42 PM}				GetDlgItem(IDC_STATIC_LLNL_DHA)->EnableWindow(nType == CSpecies::AT_LLNL_DH);
// COMMENT: {8/16/2001 6:10:42 PM}				GetDlgItem(IDC_EDIT_LLNL_DHA)->EnableWindow(nType == CSpecies::AT_LLNL_DH);
// COMMENT: {8/16/2001 6:10:42 PM}			}
// COMMENT: {8/16/2001 6:10:42 PM}			break;
// COMMENT: {8/16/2001 6:10:42 PM}
// COMMENT: {8/16/2001 6:10:42 PM}		case NCOL_DHA: // Act. a
// COMMENT: {8/16/2001 6:10:42 PM}			m_ctrlDHa.SetWindowText(pInfo->item.pszText);
// COMMENT: {8/16/2001 6:10:42 PM}			m_ctrlLLNLa.SetWindowText(pInfo->item.pszText);
// COMMENT: {8/16/2001 6:10:42 PM}			break;
// COMMENT: {8/16/2001 6:10:42 PM}
// COMMENT: {8/16/2001 6:10:42 PM}		case NCOL_DHB: // Act. b
// COMMENT: {8/16/2001 6:10:42 PM}			m_ctrlDHb.SetWindowText(pInfo->item.pszText);
// COMMENT: {8/16/2001 6:10:42 PM}			break;

		case NCOL_CHECK: // Check eqn
			{
				CString strCheck = pInfo->item.pszText;
// COMMENT: {8/16/2001 6:20:35 PM}				CWnd* pStatic = GetDlgItem(IDC_STATIC_MOLE_BAL);
				if (strCheck.GetLength() > 0)
				{
					switch (strCheck.GetAt(0))
					{
					case _T('n') : case _T('N') : case _T('f') : case _T('F') :
						m_ctrlCheckEqn.SetCheck(BST_UNCHECKED);
// COMMENT: {8/16/2001 6:20:40 PM}						m_ctrlMoleBal.EnableWindow(TRUE);
// COMMENT: {8/16/2001 6:20:40 PM}						pStatic->EnableWindow(TRUE);
						break;
					case _T('y') : case _T('Y') : case _T('t') : case _T('T') :
						m_ctrlCheckEqn.SetCheck(BST_CHECKED);
// COMMENT: {8/16/2001 6:20:44 PM}						m_ctrlMoleBal.EnableWindow(FALSE);
// COMMENT: {8/16/2001 6:20:44 PM}						pStatic->EnableWindow(FALSE);
						break;
					default:
						m_ctrlCheckEqn.SetCheck(BST_CHECKED);
						OnCheckEquation();
						break;
					}
				}
			}
			break;

// COMMENT: {8/16/2001 6:20:52 PM}		case NCOL_MOLE_BAL: // Mol bal
// COMMENT: {8/16/2001 6:20:52 PM}			m_ctrlMoleBal.SetWindowText(pInfo->item.pszText);
// COMMENT: {8/16/2001 6:20:52 PM}			break;

		default :
			ASSERT(FALSE);
			break;
		}
		break;
	}
	return 0;

}

LRESULT CKPPhasesPg1::OnAllowDeleteRow(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	ASSERT(nID == IDC_MSHFLEXGRID1);

	return TRUE;  // allow
}

LRESULT CKPPhasesPg1::OnDeleteRow(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	ASSERT(nID == IDC_MSHFLEXGRID1);

	m_ctrlGrid.DeleteRow(pInfo->item.iRow);	
	return TRUE; // handled locally
}

LRESULT CKPPhasesPg1::OnAllowInsertRow(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	ASSERT(nID == IDC_MSHFLEXGRID1);

	return FALSE;  // disallow for now
}

LRESULT CKPPhasesPg1::OnInsertRow(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	ASSERT(nID == IDC_MSHFLEXGRID1);
	
	ASSERT(FALSE); // OnAllowInsertRow disallowed

	return TRUE; // handled locally

	return FALSE; // allow default handler
}

void CKPPhasesPg1::OnDblClickGrid() 
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


BOOL CKPPhasesPg1::OnInitDialog() 
{
	baseCKPPhasesPg1::OnInitDialog();
	
	// Add extra initialization here
// COMMENT: {8/16/2001 6:33:39 PM}	EnableLLNL(m_bEnableLLNL);

	// set layout
	CreateRoot(VERTICAL, 5, 6) 

		<< (paneCtrl(IDC_STATIC_MAIN, HORIZONTAL, ABSOLUTE_VERT, nDefaultBorder, 10, 10)
			<< (pane(VERTICAL, GREEDY)
				//{{
				<< (pane(HORIZONTAL, GREEDY)
					<< item(IDC_STATIC_PHASE_NAME, ABSOLUTE_VERT)
					)
				<< (pane(HORIZONTAL, GREEDY)
					<< item(IDC_EDIT_PHASE_NAME, ABSOLUTE_VERT)
					)
				//}}
				<< (pane(HORIZONTAL, GREEDY)
					<< item(IDC_STATIC_RXN, ABSOLUTE_VERT)
					)
				<< (pane(HORIZONTAL, GREEDY)
					<< item(IDC_EDIT_RXN, ABSOLUTE_VERT)
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
// COMMENT: {8/16/2001 6:17:29 PM}					<< item(IDC_STATIC_MOLE_BAL, NORESIZE | ALIGN_CENTER)
					)
				<< (pane(HORIZONTAL, GREEDY)
					<< item(IDC_STATIC_DELTA_H, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 1)
					<< item(IDC_EDIT_DELTA_H, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 6)
					<< item(IDC_COMBO_DELTA_H_UNITS, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 239)
					<< itemGrowing(HORIZONTAL)
// COMMENT: {8/16/2001 6:17:36 PM}					<< item(IDC_EDIT_MOLE_BAL, NORESIZE | ALIGN_CENTER)
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
						<< item(IDC_STATIC_A6, NORESIZE | ALIGN_CENTER)
						<< item(IDC_EDIT_A6, ABSOLUTE_VERT | ALIGN_CENTER)
						)
					)

// COMMENT: {8/16/2001 6:32:58 PM}				<< (paneCtrl(IDC_GB_ACT_COEF, VERTICAL, ABSOLUTE_VERT, nDefaultBorder, 10, 10)
// COMMENT: {8/16/2001 6:32:58 PM}					<< (pane(HORIZONTAL, GREEDY)
// COMMENT: {8/16/2001 6:32:58 PM}						<< item(IDC_RADIO_DAVIES, NORESIZE | ALIGN_CENTER)
// COMMENT: {8/16/2001 6:32:58 PM}						<< item((CWnd*)NULL, NORESIZE | ALIGN_CENTER, 12, 13, 12, 13)
// COMMENT: {8/16/2001 6:32:58 PM}						<< item((CWnd*)NULL, ABSOLUTE_VERT | ALIGN_CENTER, 54, 20, 54, 20)
// COMMENT: {8/16/2001 6:32:58 PM}						<< item((CWnd*)NULL, NORESIZE | ALIGN_CENTER, 12, 13, 12, 13)
// COMMENT: {8/16/2001 6:32:58 PM}						<< item((CWnd*)NULL, ABSOLUTE_VERT | ALIGN_CENTER, 54, 20, 54, 20)
// COMMENT: {8/16/2001 6:32:58 PM}						<< itemGrowing(HORIZONTAL)
// COMMENT: {8/16/2001 6:32:58 PM}						<< item(IDC_RADIO_LLNL_CO2, NORESIZE | ALIGN_CENTER)
// COMMENT: {8/16/2001 6:32:58 PM}						<< item((CWnd*)NULL, NORESIZE | ALIGN_CENTER, 12, 13, 12, 13)
// COMMENT: {8/16/2001 6:32:58 PM}						<< item((CWnd*)NULL, ABSOLUTE_VERT | ALIGN_CENTER, 55, 20, 55, 20)
// COMMENT: {8/16/2001 6:32:58 PM}						)
// COMMENT: {8/16/2001 6:32:58 PM}					<< (pane(HORIZONTAL, GREEDY)
// COMMENT: {8/16/2001 6:32:58 PM}						<< item(IDC_RADIO_DH, NORESIZE | ALIGN_CENTER)
// COMMENT: {8/16/2001 6:32:58 PM}						<< item(IDC_STATIC_DHA, NORESIZE | ALIGN_CENTER)
// COMMENT: {8/16/2001 6:32:58 PM}						<< item(IDC_EDIT_DHA, ABSOLUTE_VERT | ALIGN_CENTER)
// COMMENT: {8/16/2001 6:32:58 PM}						<< item(IDC_STATIC_DHB, NORESIZE | ALIGN_CENTER)
// COMMENT: {8/16/2001 6:32:58 PM}						<< item(IDC_EDIT_DHB, ABSOLUTE_VERT | ALIGN_CENTER)
// COMMENT: {8/16/2001 6:32:58 PM}						<< itemGrowing(HORIZONTAL)
// COMMENT: {8/16/2001 6:32:58 PM}						<< item(IDC_RADIO_LLNL_DH, NORESIZE | ALIGN_CENTER)
// COMMENT: {8/16/2001 6:32:58 PM}						<< item(IDC_STATIC_LLNL_DHA, NORESIZE | ALIGN_CENTER)
// COMMENT: {8/16/2001 6:32:58 PM}						<< item(IDC_EDIT_LLNL_DHA, ABSOLUTE_VERT | ALIGN_CENTER)
// COMMENT: {8/16/2001 6:32:58 PM}						)
// COMMENT: {8/16/2001 6:32:58 PM}					)
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

//DEL void CKPPhasesPg1::EnableLLNL(bool bEnableLLNL)
//DEL {
//DEL 	m_bEnableLLNL = bEnableLLNL;
//DEL 
//DEL 	if (m_hWnd && ::IsWindow(m_hWnd))
//DEL 	{
//DEL 		CWnd* pWnd = NULL;
//DEL 		if (m_bEnableLLNL)
//DEL 		{
//DEL 			pWnd = GetDlgItem(IDC_RADIO_LLNL_CO2);
//DEL 			if (pWnd)
//DEL 			{
//DEL 				pWnd->EnableWindow(TRUE);
//DEL 				pWnd->ModifyStyle(0, WS_VISIBLE);
//DEL 				pWnd->Invalidate();
//DEL 			}
//DEL 
//DEL 			pWnd = GetDlgItem(IDC_RADIO_LLNL_DH);
//DEL 			if (pWnd)
//DEL 			{
//DEL 				pWnd->EnableWindow(TRUE);
//DEL 				pWnd->ModifyStyle(0, WS_VISIBLE);
//DEL 				pWnd->Invalidate();
//DEL 			}
//DEL 
//DEL 			pWnd = GetDlgItem(IDC_STATIC_LLNL_DHA);
//DEL 			if (pWnd)
//DEL 			{
//DEL 				pWnd->ModifyStyle(0, WS_VISIBLE);
//DEL 				pWnd->Invalidate();
//DEL 			}
//DEL 
//DEL 			pWnd = GetDlgItem(IDC_EDIT_LLNL_DHA);
//DEL 			if (pWnd)
//DEL 			{
//DEL 				pWnd->ModifyStyle(0, WS_VISIBLE);
//DEL 				pWnd->Invalidate();
//DEL 			}
//DEL 		}
//DEL 		else
//DEL 		{
//DEL 			pWnd = GetDlgItem(IDC_RADIO_LLNL_CO2);
//DEL 			if (pWnd)
//DEL 			{
//DEL 				pWnd->EnableWindow(FALSE);
//DEL 				pWnd->ModifyStyle(WS_VISIBLE, 0);
//DEL 			}
//DEL 
//DEL 			pWnd = GetDlgItem(IDC_RADIO_LLNL_DH);
//DEL 			if (pWnd)
//DEL 			{
//DEL 				pWnd->EnableWindow(FALSE);
//DEL 				pWnd->ModifyStyle(WS_VISIBLE, 0);
//DEL 			}
//DEL 
//DEL 			pWnd = GetDlgItem(IDC_STATIC_LLNL_DHA);
//DEL 			if (pWnd)
//DEL 			{
//DEL 				pWnd->ModifyStyle(WS_VISIBLE, 0);
//DEL 			}
//DEL 
//DEL 			pWnd = GetDlgItem(IDC_EDIT_LLNL_DHA);
//DEL 			if (pWnd)
//DEL 			{
//DEL 				pWnd->ModifyStyle(WS_VISIBLE, 0);
//DEL 			}
//DEL 		}
//DEL 	}
//DEL }

void CKPPhasesPg1::OnSetfocusEditPhaseName() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING572);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPPhasesPg1::OnSetfocusEditRxn() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING571);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPPhasesPg1::OnSetfocusEditLogk() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING573);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPPhasesPg1::OnSetfocusEditDeltaH() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING549);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPPhasesPg1::OnSetfocusComboDeltaHUnits() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING550);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPPhasesPg1::OnSetfocusEditA1() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING551);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPPhasesPg1::OnSetfocusEditA2() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING552);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPPhasesPg1::OnSetfocusEditA3() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING553);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPPhasesPg1::OnSetfocusEditA4() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING554);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPPhasesPg1::OnSetfocusEditA5() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING555);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPPhasesPg1::OnSetfocusEditA6() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING648);
	m_eInputDesc.SetWindowText(strRes);	
}

//DEL void CKPPhasesPg1::OnSetfocusEditDha() 
//DEL {
//DEL 	CString strRes;
//DEL 	strRes.LoadString(IDS_STRING556);
//DEL 	m_eInputDesc.SetWindowText(strRes);	
//DEL }

//DEL void CKPPhasesPg1::OnSetfocusEditDhb() 
//DEL {
//DEL 	CString strRes;
//DEL 	strRes.LoadString(IDS_STRING557);
//DEL 	m_eInputDesc.SetWindowText(strRes);	
//DEL }

//DEL void CKPPhasesPg1::OnSetfocusEditLlnlDha() 
//DEL {
//DEL 	CString strRes;
//DEL 	strRes.LoadString(IDS_STRING558);
//DEL 	m_eInputDesc.SetWindowText(strRes);	
//DEL }

// COMMENT: {8/16/2001 6:34:13 PM}void CKPPhasesPg1::OnSetfocusRadio(UINT nID) 
// COMMENT: {8/16/2001 6:34:13 PM}{
// COMMENT: {8/16/2001 6:34:13 PM}	UINT nResID = nID - IDC_RADIO_DAVIES + IDS_STRING559;
// COMMENT: {8/16/2001 6:34:13 PM}
// COMMENT: {8/16/2001 6:34:13 PM}	CString strRes;
// COMMENT: {8/16/2001 6:34:13 PM}	strRes.LoadString(nResID);
// COMMENT: {8/16/2001 6:34:13 PM}	m_eInputDesc.SetWindowText(strRes);	
// COMMENT: {8/16/2001 6:34:13 PM}}

//DEL void CKPPhasesPg1::OnSetfocusEditMoleBal() 
//DEL {
//DEL 	CString strRes;
//DEL 	strRes.LoadString(IDS_STRING563);
//DEL 	m_eInputDesc.SetWindowText(strRes);	
//DEL }


BOOL CKPPhasesPg1::OnHelpInfo(HELPINFO* pHelpInfo) 
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
	case IDC_EDIT_PHASE_NAME: case IDC_STATIC_PHASE_NAME:
		strRes.LoadString(IDS_STRING572);
		break;
	case IDC_EDIT_RXN: case IDC_STATIC_RXN:
		strRes.LoadString(IDS_STRING571);
		break;
	case IDC_EDIT_LOGK: case IDC_STATIC_LOGK:
		strRes.LoadString(IDS_STRING573);
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
	case IDC_EDIT_A6: case IDC_STATIC_A6:
		strRes.LoadString(IDS_STRING648);
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
		case NCOL_PHASE_NAME:
			nResID = IDS_STRING572;
			break;
		case NCOL_RXN:
			nResID = IDS_STRING571;
			break;
		case NCOL_LOGK:
			nResID = IDS_STRING573;
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
		case NCOL_A6:
			nResID = IDS_STRING648;
			break;
		case NCOL_CHECK:
			nResID = IDS_STRING564;
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
	return ::HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}

void CKPPhasesPg1::OnEnterCellGrid() 
{
	UINT nResID = 0;
	switch (m_ctrlGrid.GetCol())
	{
	case NCOL_PHASE_NAME:
		nResID = IDS_STRING572;
		break;
	case NCOL_RXN:
		nResID = IDS_STRING571;
		break;
	case NCOL_LOGK:
		nResID = IDS_STRING573;
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
	case NCOL_A6:
		nResID = IDS_STRING648;
		break;
	case NCOL_CHECK:
		nResID = IDS_STRING564;
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


void CKPPhasesPg1::OnKeyDownGrid(short FAR* KeyCode, short Shift) 
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
