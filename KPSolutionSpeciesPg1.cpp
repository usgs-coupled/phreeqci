// KPSolutionSpeciesPg1.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "KPSolutionSpeciesPg1.h"

#include "DDX_DDV.h"
#include "EqnChecker2.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CKPSolutionSpeciesPg1, baseCKPSolutionSpeciesPg1)


/////////////////////////////////////////////////////////////////////////////
// CKPSolutionSpeciesPg1 property page

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
const long NCOL_A6             = 10;
const long NCOL_ACT_TYPE       = 11;
const long NCOL_DHA            = 12;
const long NCOL_DHB            = 13;
const long NCOL_CHECK          = 14;
const long NCOL_MOLE_BAL       = 15;
const long NCOL_DW             = 16;
const long NCOL_ERM_DLL        = 17;
const long NCOL_MILLERO_0      = 18;
const long NCOL_MILLERO_1      = 19;
const long NCOL_MILLERO_2      = 20;
const long NCOL_MILLERO_3      = 21;
const long NCOL_MILLERO_4      = 22;
const long NCOL_MILLERO_5      = 23;

CKPSolutionSpeciesPg1::CKPSolutionSpeciesPg1() : baseCKPSolutionSpeciesPg1(CKPSolutionSpeciesPg1::IDD)
{
	//{{AFX_DATA_INIT(CKPSolutionSpeciesPg1)
	//}}AFX_DATA_INIT

	m_pictHolderRightArrow.CreateFromIcon(IDI_RIGHT_ARROW2);
	m_pictHolderStar.CreateFromIcon(IDI_STAR);

	m_hWndLastControl  = NULL;
	m_bEditLastControl = FALSE;

	m_bIgnoreChanges = false;

	m_mapStrToActType[_T("DAVIES")]   = CSpecies::AT_DAVIES;
	m_mapStrToActType[_T("D_H")]      = CSpecies::AT_DEBYE_HUCKEL;
	m_mapStrToActType[_T("LLNL_CO2")] = CSpecies::AT_LLNL_DH_CO2;
	m_mapStrToActType[_T("LLNL_DH")]  = CSpecies::AT_LLNL_DH;

	m_mapActTypeToStr[CSpecies::AT_DAVIES]       = _T("DAVIES");
	m_mapActTypeToStr[CSpecies::AT_DEBYE_HUCKEL] = _T("D_H");
	m_mapActTypeToStr[CSpecies::AT_LLNL_DH_CO2]  = _T("LLNL_CO2");
	m_mapActTypeToStr[CSpecies::AT_LLNL_DH]      = _T("LLNL_DH");

	EnableLLNL(false);
}

CKPSolutionSpeciesPg1::~CKPSolutionSpeciesPg1()
{
}

void CKPSolutionSpeciesPg1::DoDataExchange(CDataExchange* pDX)
{
	baseCKPSolutionSpeciesPg1::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKPSolutionSpeciesPg1)
	DDX_Control(pDX, IDC_EDIT_ASSOC_RXN, m_ctrlAssocRxn);
	DDX_Control(pDX, IDC_CHECK_EQUATION, m_ctrlCheckEqn);
	DDX_Control(pDX, IDC_EDIT_LOGK, m_ctrlLogK);
	DDX_Control(pDX, IDC_EDIT_DELTA_H, m_ctrlDeltaH);
	DDX_Control(pDX, IDC_COMBO_DELTA_H_UNITS, m_ctrlDeltaHUnits);
	DDX_Control(pDX, IDC_EDIT_MOLE_BAL, m_ctrlMoleBal);
	DDX_Control(pDX, IDC_EDIT_A1, m_ctrlA1);
	DDX_Control(pDX, IDC_EDIT_A2, m_ctrlA2);
	DDX_Control(pDX, IDC_EDIT_A3, m_ctrlA3);
	DDX_Control(pDX, IDC_EDIT_A4, m_ctrlA4);
	DDX_Control(pDX, IDC_EDIT_A5, m_ctrlA5);
	DDX_Control(pDX, IDC_EDIT_A6, m_ctrlA6);
	DDX_Control(pDX, IDC_EDIT_DHB, m_ctrlDHb);
	DDX_Control(pDX, IDC_EDIT_DHA, m_ctrlDHa);
	DDX_Control(pDX, IDC_EDIT_LLNL_DHA, m_ctrlLLNLa);
	DDX_Control(pDX, IDC_SRCDBPGCTRL1, m_pager);
	DDX_Control(pDX, IDC_MSHFLEXGRID1, m_ctrlGrid);
	DDX_Control(pDX, IDC_DESCRIPTION, m_eInputDesc);
	DDX_Control(pDX, IDC_EDIT_DW, m_ctrlDW);
	DDX_Control(pDX, IDC_EDIT_ERM_DLL, m_ctrlERM_DLL);
	DDX_Control(pDX, IDC_EDIT_MILL_0, m_ctrlM0);
	DDX_Control(pDX, IDC_EDIT_MILL_1, m_ctrlM1);
	DDX_Control(pDX, IDC_EDIT_MILL_2, m_ctrlM2);
	DDX_Control(pDX, IDC_EDIT_MILL_3, m_ctrlM3);
	DDX_Control(pDX, IDC_EDIT_MILL_4, m_ctrlM4);
	DDX_Control(pDX, IDC_EDIT_MILL_5, m_ctrlM5);
	//}}AFX_DATA_MAP

	if (m_bFirstSetActive)
	{
		InitGrid(pDX, IDC_MSHFLEXGRID1);
	}

	if (pDX->m_bSaveAndValidate)
	{
		CWaitCursor wait;
		CEqnChecker2 checker;

		//{{TEST CEqnChecker2
		ASSERT(!checker.CheckAssociationRxn(_T("&SO4"), true));
		ASSERT(_tcsncmp(LPCTSTR(checker.GetLastError()), _T("Character is not allowed,"), 25) == 0);
		ASSERT(!checker.CheckAssociationRxn(_T("SO4-2"), true));
		ASSERT(_tcsncmp(LPCTSTR(checker.GetLastError()), _T("Equation has no equal sign.\n\t"), 29) == 0);
		ASSERT(!checker.CheckAssociationRxn(_T("HS- = S2-2 + H+"), true));
		ASSERT(_tcsncmp(LPCTSTR(checker.GetLastError()), _T("Equation does not balance for element, S."), 41) == 0);
		ASSERT(checker.CheckAssociationRxn(_T("HS- = S2-2 + H+"), false));
		//}}TEST CEqnChecker2

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

			// set default
			spec.m_nActType = CSpecies::AT_DAVIES;
			CString strActType; // Davies; D-H; LLNL-CO2; LLNL-DH
			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_ACT_TYPE, strActType);
			if (!strActType.IsEmpty())
			{
				strActType.MakeUpper();
				std::map<CString, CSpecies::ActType>::iterator iter = m_mapStrToActType.find(strActType);
				if (iter != m_mapStrToActType.end())
				{
					spec.m_nActType = iter->second;
				}
				else
				{
					::AfxMessageBox("Invalid activity type");
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
				nCurrentTextBox = IDC_EDIT_A6;
				DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_A6, spec.m_dA6);
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

			if (spec.m_dA1 != spec.m_dA1)
			{
				spec.m_dA1 = 0;
			}
			else
			{
				spec.m_bHasAnalExp = true;
			}
			if (spec.m_dA2 != spec.m_dA2)
			{
				spec.m_dA2 = 0;
			}
			else
			{
				spec.m_bHasAnalExp = true;
			}
			if (spec.m_dA3 != spec.m_dA3)
			{
				spec.m_dA3 = 0;
			}
			else
			{
				spec.m_bHasAnalExp = true;
			}
			if (spec.m_dA4 != spec.m_dA4)
			{
				spec.m_dA4 = 0;
			}
			else
			{
				spec.m_bHasAnalExp = true;
			}
			if (spec.m_dA5 != spec.m_dA5)
			{
				spec.m_dA5 = 0;
			}
			else
			{
				spec.m_bHasAnalExp = true;
			}
			if (spec.m_dA6 != spec.m_dA6)
			{
				spec.m_dA6 = 0;
			}
			else
			{
				spec.m_bHasAnalExp = true;
			}

			// must define either log_k or anan expression
			if (!spec.m_bHasAnalExp && (spec.m_dLogK != spec.m_dLogK))
			{
				m_ctrlGrid.SetRow(nRow);
				OnRowColChangeGrid();
				pDX->PrepareEditCtrl(IDC_EDIT_LOGK);
				::AfxMessageBox(_T("Either a Log K or an analytical expression must be defined."));
				pDX->Fail();
			}

			// Line 5: -gamma
			// Debye-Huckel a
			if (spec.m_nActType == CSpecies::AT_DEBYE_HUCKEL)
			{
				try
				{
					// Debye-Huckel a
					nCurrentTextBox = IDC_EDIT_DHA;
					DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_DHA, spec.m_dDHa);

					// Debye-Huckel b
					nCurrentTextBox = IDC_EDIT_DHB;
					DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_DHB, spec.m_dDHb);
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
			}
			else if (spec.m_nActType == CSpecies::AT_LLNL_DH)
			{
				try
				{
					// LLNL Debye-Huckel a
					nCurrentTextBox = IDC_EDIT_LLNL_DHA;
					DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_DHA, spec.m_dDHa);
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
			}

			// dw
			try
			{
				nCurrentTextBox = IDC_EDIT_DW;
				DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_DW, spec.m_dw);
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

			// erm_dll
			try
			{
				nCurrentTextBox = IDC_EDIT_ERM_DLL;
				DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_ERM_DLL, spec.m_erm_ddl);
				if (spec.m_erm_ddl < 0)
				{
					pDX->m_bEditLastControl = TRUE;
					pDX->m_idLastControl = IDC_EDIT_ERM_DLL;
					::AfxMessageBox("Expecting enrichment factor > 0");
					pDX->Fail();
				}
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

			// -millero
			try
			{
				nCurrentTextBox = IDC_EDIT_MILL_0;
				DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_MILLERO_0, spec.m_millero[0]);
				nCurrentTextBox = IDC_EDIT_MILL_1;
				DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_MILLERO_1, spec.m_millero[1]);
				nCurrentTextBox = IDC_EDIT_MILL_2;
				DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_MILLERO_2, spec.m_millero[2]);
				nCurrentTextBox = IDC_EDIT_MILL_3;
				DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_MILLERO_3, spec.m_millero[3]);
				nCurrentTextBox = IDC_EDIT_MILL_4;
				DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_MILLERO_4, spec.m_millero[4]);
				nCurrentTextBox = IDC_EDIT_MILL_5;
				DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_MILLERO_5, spec.m_millero[5]);
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
			for (int i = 0; i < 6; ++i)
			{
				if (spec.m_millero[i] != spec.m_millero[i])
				{
					spec.m_millero[i] = 0;
				}
				else
				{
					spec.m_bHasMillero = true;
				}
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
				DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_A6, spec.m_dA6);
			}

			// no_check
			strCheck =  (spec.m_bCheckEqn) ? _T("Yes") : _T("No");
			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_CHECK, strCheck);

			// mole balance
			DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_MOLE_BAL, spec.m_strMoleBalance);


			// Activity Type
			CSpecies::ActType nType = spec.m_nActType;
			switch (nType)
			{
			case CSpecies::AT_DAVIES:
				break;

			case CSpecies::AT_DEBYE_HUCKEL:
				DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_DHA, spec.m_dDHa);
				DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_DHB, spec.m_dDHb);
				break;

			case CSpecies::AT_LLNL_DH:
				DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_DHA, spec.m_dDHa);
				if (!m_bEnableLLNL) EnableLLNL(true);
				break;

			case CSpecies::AT_LLNL_DH_CO2:
				if (!m_bEnableLLNL) EnableLLNL(true);
				break;

			default:
				ASSERT(FALSE);
				nType = CSpecies::AT_DAVIES;
				break;
			}
			std::map<CSpecies::ActType, CString>::iterator iterMap = m_mapActTypeToStr.find(nType);
			if (iterMap != m_mapActTypeToStr.end())
			{
				DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_ACT_TYPE, iterMap->second);
			}

			// tracer diffusion coefficient
			DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_DW, spec.m_dw);

			// enrichment factor in DDL
			DDX_GridTextNaN(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_ERM_DLL, spec.m_erm_ddl);

			// -millero
			if (spec.m_bHasMillero)
			{
				DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_MILLERO_0, spec.m_millero[0]);
				DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_MILLERO_1, spec.m_millero[1]);
				DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_MILLERO_2, spec.m_millero[2]);
				DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_MILLERO_3, spec.m_millero[3]);
				DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_MILLERO_4, spec.m_millero[4]);
				DDX_GridText(pDX, IDC_MSHFLEXGRID1, nRow, NCOL_MILLERO_5, spec.m_millero[5]);
			}
		}

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

BEGIN_MESSAGE_MAP(CKPSolutionSpeciesPg1, baseCKPSolutionSpeciesPg1)
	//{{AFX_MSG_MAP(CKPSolutionSpeciesPg1)
	ON_EN_CHANGE(IDC_EDIT_ASSOC_RXN, OnChangeEditAssocRxn)
	ON_EN_CHANGE(IDC_EDIT_LOGK, OnChangeEditLogk)
	ON_EN_KILLFOCUS(IDC_EDIT_ASSOC_RXN, OnKillfocusEditAssocRxn)
	ON_EN_CHANGE(IDC_EDIT_DHA, OnChangeEditDha)
	ON_EN_CHANGE(IDC_EDIT_DHB, OnChangeEditDhb)
	ON_EN_CHANGE(IDC_EDIT_LLNL_DHA, OnChangeEditLlnlDha)
	ON_EN_CHANGE(IDC_EDIT_DELTA_H, OnChangeEditDeltaH)
	ON_CBN_SELCHANGE(IDC_COMBO_DELTA_H_UNITS, OnSelchangeComboDeltaHUnits)
	ON_BN_CLICKED(IDC_CHECK_EQUATION, OnCheckEquation)
	ON_EN_CHANGE(IDC_EDIT_MOLE_BAL, OnChangeEditMoleBal)
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
	ON_EN_SETFOCUS(IDC_EDIT_ASSOC_RXN, OnSetfocusEditAssocRxn)
	ON_EN_SETFOCUS(IDC_EDIT_LOGK, OnSetfocusEditLogk)
	ON_EN_SETFOCUS(IDC_EDIT_DELTA_H, OnSetfocusEditDeltaH)
	ON_CBN_SETFOCUS(IDC_COMBO_DELTA_H_UNITS, OnSetfocusComboDeltaHUnits)
	ON_EN_SETFOCUS(IDC_EDIT_A1, OnSetfocusEditA1)
	ON_EN_SETFOCUS(IDC_EDIT_A2, OnSetfocusEditA2)
	ON_EN_SETFOCUS(IDC_EDIT_A3, OnSetfocusEditA3)
	ON_EN_SETFOCUS(IDC_EDIT_A4, OnSetfocusEditA4)
	ON_EN_SETFOCUS(IDC_EDIT_A5, OnSetfocusEditA5)
	ON_EN_SETFOCUS(IDC_EDIT_A6, OnSetfocusEditA6)
	ON_EN_SETFOCUS(IDC_EDIT_DHA, OnSetfocusEditDha)
	ON_EN_SETFOCUS(IDC_EDIT_DHB, OnSetfocusEditDhb)
	ON_EN_SETFOCUS(IDC_EDIT_LLNL_DHA, OnSetfocusEditLlnlDha)
	ON_WM_HELPINFO()
	ON_EN_SETFOCUS(IDC_EDIT_MOLE_BAL, OnSetfocusEditMoleBal)
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


	// custom radio notifications
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_DAVIES, IDC_RADIO_LLNL_CO2, OnActCoefRadioClicked)

	ON_CONTROL_RANGE(BN_SETFOCUS, IDC_RADIO_DAVIES, IDC_RADIO_LLNL_CO2, OnSetfocusRadio)

	ON_EN_CHANGE(IDC_EDIT_DW, &CKPSolutionSpeciesPg1::OnEnChangeEditDw)
	ON_EN_KILLFOCUS(IDC_EDIT_DW, &CKPSolutionSpeciesPg1::OnEnKillfocusEditDw)
	ON_EN_CHANGE(IDC_EDIT_ERM_DLL, &CKPSolutionSpeciesPg1::OnEnChangeEditErmDll)
	ON_EN_KILLFOCUS(IDC_EDIT_ERM_DLL, &CKPSolutionSpeciesPg1::OnEnKillfocusEditErmDll)

	ON_EN_CHANGE(IDC_EDIT_MILL_0, &CKPSolutionSpeciesPg1::OnEnChangeEditMill0)
	ON_EN_KILLFOCUS(IDC_EDIT_MILL_0, &CKPSolutionSpeciesPg1::OnEnKillfocusEditMill0)
	ON_EN_SETFOCUS(IDC_EDIT_MILL_0, &CKPSolutionSpeciesPg1::OnEnSetfocusEditMill0)
	ON_EN_CHANGE(IDC_EDIT_MILL_1, &CKPSolutionSpeciesPg1::OnEnChangeEditMill1)
	ON_EN_KILLFOCUS(IDC_EDIT_MILL_1, &CKPSolutionSpeciesPg1::OnEnKillfocusEditMill1)
	ON_EN_SETFOCUS(IDC_EDIT_MILL_1, &CKPSolutionSpeciesPg1::OnEnSetfocusEditMill1)
	ON_EN_CHANGE(IDC_EDIT_MILL_2, &CKPSolutionSpeciesPg1::OnEnChangeEditMill2)
	ON_EN_KILLFOCUS(IDC_EDIT_MILL_2, &CKPSolutionSpeciesPg1::OnEnKillfocusEditMill2)
	ON_EN_SETFOCUS(IDC_EDIT_MILL_2, &CKPSolutionSpeciesPg1::OnEnSetfocusEditMill2)
	ON_EN_CHANGE(IDC_EDIT_MILL_3, &CKPSolutionSpeciesPg1::OnEnChangeEditMill3)
	ON_EN_KILLFOCUS(IDC_EDIT_MILL_3, &CKPSolutionSpeciesPg1::OnEnKillfocusEditMill3)
	ON_EN_SETFOCUS(IDC_EDIT_MILL_3, &CKPSolutionSpeciesPg1::OnEnSetfocusEditMill3)
	ON_EN_CHANGE(IDC_EDIT_MILL_4, &CKPSolutionSpeciesPg1::OnEnChangeEditMill4)
	ON_EN_KILLFOCUS(IDC_EDIT_MILL_4, &CKPSolutionSpeciesPg1::OnEnKillfocusEditMill4)
	ON_EN_SETFOCUS(IDC_EDIT_MILL_4, &CKPSolutionSpeciesPg1::OnEnSetfocusEditMill4)
	ON_EN_CHANGE(IDC_EDIT_MILL_5, &CKPSolutionSpeciesPg1::OnEnChangeEditMill5)
	ON_EN_KILLFOCUS(IDC_EDIT_MILL_5, &CKPSolutionSpeciesPg1::OnEnKillfocusEditMill5)
	ON_EN_SETFOCUS(IDC_EDIT_MILL_5, &CKPSolutionSpeciesPg1::OnEnSetfocusEditMill5)
	ON_EN_SETFOCUS(IDC_EDIT_DW, &CKPSolutionSpeciesPg1::OnEnSetfocusEditDw)
	ON_EN_SETFOCUS(IDC_EDIT_ERM_DLL, &CKPSolutionSpeciesPg1::OnEnSetfocusEditErmDll)
	ON_BN_CLICKED(IDC_CHECK_ANAL_EXP, &CKPSolutionSpeciesPg1::OnBnClickedCheckAnalExp)
	ON_BN_CLICKED(IDC_CHECK_DIFF_PARAM, &CKPSolutionSpeciesPg1::OnBnClickedCheckDiffParam)
	ON_BN_CLICKED(IDC_CHECK_MILLERO, &CKPSolutionSpeciesPg1::OnBnClickedCheckMillero)
END_MESSAGE_MAP()

void CKPSolutionSpeciesPg1::InitGrid(CDataExchange* pDX, int nIDC)
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
    m_ctrlGrid.SetTextMatrix( 0, NCOL_A6,            _T("A6"));
    m_ctrlGrid.SetTextMatrix( 0, NCOL_ACT_TYPE,      _T("Activity")); // Davies; D-H; LLNL-CO2; LLNL-DH
    m_ctrlGrid.SetTextMatrix( 0, NCOL_DHA,           _T("Act. a"));
    m_ctrlGrid.SetTextMatrix( 0, NCOL_DHB,           _T("Act. b"));
    m_ctrlGrid.SetTextMatrix( 0, NCOL_CHECK,         _T("Check"));
    m_ctrlGrid.SetTextMatrix( 0, NCOL_MOLE_BAL,      _T("Mol bal"));
    m_ctrlGrid.SetTextMatrix( 0, NCOL_DW,            _T("Dw"));
    m_ctrlGrid.SetTextMatrix( 0, NCOL_ERM_DLL,       _T("ERM ddl"));
    m_ctrlGrid.SetTextMatrix( 0, NCOL_MILLERO_0,     _T("A"));
    m_ctrlGrid.SetTextMatrix( 0, NCOL_MILLERO_1,     _T("B"));
    m_ctrlGrid.SetTextMatrix( 0, NCOL_MILLERO_2,     _T("C"));
    m_ctrlGrid.SetTextMatrix( 0, NCOL_MILLERO_3,     _T("D"));
    m_ctrlGrid.SetTextMatrix( 0, NCOL_MILLERO_4,     _T("D"));
    m_ctrlGrid.SetTextMatrix( 0, NCOL_MILLERO_5,     _T("F"));

	// set alignment
	m_ctrlGrid.SetColAlignment(NCOL_RXN, flexAlignLeftCenter);

	// set starting position
	m_ctrlGrid.SetRow(1);
	m_ctrlGrid.SetCol(1);
}

BEGIN_EVENTSINK_MAP(CKPSolutionSpeciesPg1, baseCKPSolutionSpeciesPg1)
    //{{AFX_EVENTSINK_MAP(CKPSolutionSpeciesPg1)
	ON_EVENT(CKPSolutionSpeciesPg1, IDC_MSHFLEXGRID1, 70 /* RowColChange */, OnRowColChangeGrid, VTS_NONE)
	ON_EVENT(CKPSolutionSpeciesPg1, IDC_SRCDBPGCTRL1, 3 /* GotoNextClicked */, OnGotoNextClicked, VTS_NONE)
	ON_EVENT(CKPSolutionSpeciesPg1, IDC_MSHFLEXGRID1, 72 /* LeaveCell */, OnLeaveCellGrid, VTS_NONE)
	ON_EVENT(CKPSolutionSpeciesPg1, IDC_SRCDBPGCTRL1, 1 /* GotoFirstClicked */, OnGotoFirstClicked, VTS_NONE)
	ON_EVENT(CKPSolutionSpeciesPg1, IDC_SRCDBPGCTRL1, 4 /* GotoLastClicked */, OnGotoLastClicked, VTS_NONE)
	ON_EVENT(CKPSolutionSpeciesPg1, IDC_SRCDBPGCTRL1, 5 /* GotoNewClicked */, OnGotoNewClicked, VTS_NONE)
	ON_EVENT(CKPSolutionSpeciesPg1, IDC_SRCDBPGCTRL1, 2 /* GotoPrevClicked */, OnGotoPrevClicked, VTS_NONE)
	ON_EVENT(CKPSolutionSpeciesPg1, IDC_MSHFLEXGRID1, -601 /* DblClick */, OnDblClickGrid, VTS_NONE)
	ON_EVENT(CKPSolutionSpeciesPg1, IDC_MSHFLEXGRID1, 71 /* EnterCell */, OnEnterCellGrid, VTS_NONE)
	ON_EVENT(CKPSolutionSpeciesPg1, IDC_MSHFLEXGRID1, -602 /* KeyDown */, OnKeyDownGrid, VTS_PI2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CKPSolutionSpeciesPg1::OnRowColChangeGrid() 
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
	m_ctrlA6.SetWindowText(m_ctrlGrid.GetTextMatrix(nRow, NCOL_A6)); // implicit OnChangeEditA6

	bool bAnalExp = false;
	for (long col = NCOL_A1; col <= NCOL_A6; ++col)
	{
		if (!m_ctrlGrid.GetTextMatrix(nRow, col).IsEmpty())
		{
			bAnalExp = true;
			break;
		}
	}
	if (bAnalExp)
	{
		this->CheckDlgButton(IDC_CHECK_ANAL_EXP, BST_CHECKED);
	}
	else
	{
		this->CheckDlgButton(IDC_CHECK_ANAL_EXP, BST_UNCHECKED);
	}
	this->OnBnClickedCheckAnalExp();

	// set arrow image
	m_ctrlGrid.SetCol(0);
	m_ctrlGrid.SetRefCellPicture(m_pictHolderRightArrow.GetPictureDispatch());
	m_ctrlGrid.SetCellPictureAlignment(flexAlignCenterCenter);

	// activity type
	CSpecies::ActType nType = CSpecies::AT_DAVIES;
	CString strActType = m_ctrlGrid.GetTextMatrix(nRow, NCOL_ACT_TYPE);
	if (!strActType.IsEmpty())
	{
		strActType.MakeUpper();
		std::map<CString, CSpecies::ActType>::iterator iter = m_mapStrToActType.find(strActType);
		if (iter != m_mapStrToActType.end())
		{
			nType = iter->second;
		}
	}
	else
	{
		m_ctrlGrid.SetTextMatrix(nRow, NCOL_ACT_TYPE, m_mapActTypeToStr[CSpecies::AT_DAVIES]);
	}
	UINT nID = IDC_RADIO_DAVIES;
	switch (nType)
	{
	case CSpecies::AT_DAVIES:
		nID = IDC_RADIO_DAVIES;
		break;
	case CSpecies::AT_DEBYE_HUCKEL:
		nID = IDC_RADIO_DH;
		break;
	case CSpecies::AT_LLNL_DH:
		nID = IDC_RADIO_LLNL_DH;
		if (!m_bEnableLLNL) EnableLLNL(true);
		break;
	case CSpecies::AT_LLNL_DH_CO2:
		nID = IDC_RADIO_LLNL_CO2;
		if (!m_bEnableLLNL) EnableLLNL(true);
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	CheckRadioButton(IDC_RADIO_DAVIES, IDC_RADIO_LLNL_CO2, nID);				
	GetDlgItem(IDC_STATIC_DHA)->EnableWindow(nType == CSpecies::AT_DEBYE_HUCKEL);
	GetDlgItem(IDC_EDIT_DHA)->EnableWindow(nType == CSpecies::AT_DEBYE_HUCKEL);
	GetDlgItem(IDC_STATIC_DHB)->EnableWindow(nType == CSpecies::AT_DEBYE_HUCKEL);
	GetDlgItem(IDC_EDIT_DHB)->EnableWindow(nType == CSpecies::AT_DEBYE_HUCKEL);
	GetDlgItem(IDC_STATIC_LLNL_DHA)->EnableWindow(nType == CSpecies::AT_LLNL_DH);
	GetDlgItem(IDC_EDIT_LLNL_DHA)->EnableWindow(nType == CSpecies::AT_LLNL_DH);

	// D-H parameter a
	m_ctrlDHa.SetWindowText(m_ctrlGrid.GetTextMatrix(nRow, NCOL_DHA)); // implicit OnChangeEditDha

	// D-H parameter b
	m_ctrlDHb.SetWindowText(m_ctrlGrid.GetTextMatrix(nRow, NCOL_DHB)); // implicit OnChangeEditDhb

	// LLNL parameter a
	m_ctrlLLNLa.SetWindowText(m_ctrlGrid.GetTextMatrix(nRow, NCOL_DHA));

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

	// tracer diffusion coefficient
	m_ctrlDW.SetWindowText(m_ctrlGrid.GetTextMatrix(nRow, NCOL_DW)); // implicit OnEnChangeEditDw

	// enrichment factor in DDL
	m_ctrlERM_DLL.SetWindowText(m_ctrlGrid.GetTextMatrix(nRow, NCOL_ERM_DLL)); // implicit OnEnChangeEditErmDll

	bool bDiffParams = false;
	for (long col = NCOL_DW; col <= NCOL_ERM_DLL; ++col)
	{
		if (!m_ctrlGrid.GetTextMatrix(nRow, col).IsEmpty())
		{
			bDiffParams = true;
			break;
		}
	}
	if (bDiffParams)
	{
		this->CheckDlgButton(IDC_CHECK_DIFF_PARAM, BST_CHECKED);
	}
	else
	{
		this->CheckDlgButton(IDC_CHECK_DIFF_PARAM, BST_UNCHECKED);
	}
	this->OnBnClickedCheckDiffParam();


	// millero
	m_ctrlM0.SetWindowText(m_ctrlGrid.GetTextMatrix(nRow, NCOL_MILLERO_0)); // implicit OnEnChangeEditMill0
	m_ctrlM1.SetWindowText(m_ctrlGrid.GetTextMatrix(nRow, NCOL_MILLERO_1)); // implicit OnEnChangeEditMill1
	m_ctrlM2.SetWindowText(m_ctrlGrid.GetTextMatrix(nRow, NCOL_MILLERO_2)); // implicit OnEnChangeEditMill2
	m_ctrlM3.SetWindowText(m_ctrlGrid.GetTextMatrix(nRow, NCOL_MILLERO_3)); // implicit OnEnChangeEditMill3
	m_ctrlM4.SetWindowText(m_ctrlGrid.GetTextMatrix(nRow, NCOL_MILLERO_4)); // implicit OnEnChangeEditMill4
	m_ctrlM5.SetWindowText(m_ctrlGrid.GetTextMatrix(nRow, NCOL_MILLERO_5)); // implicit OnEnChangeEditMill5

	bool bMillero = false;
	for (long col = NCOL_MILLERO_0; col <= NCOL_MILLERO_5; ++col)
	{
		if (!m_ctrlGrid.GetTextMatrix(nRow, col).IsEmpty())
		{
			bMillero = true;
			break;
		}
	}
	if (bMillero)
	{
		this->CheckDlgButton(IDC_CHECK_MILLERO, BST_CHECKED);
	}
	else
	{
		this->CheckDlgButton(IDC_CHECK_MILLERO, BST_UNCHECKED);
	}
	this->OnBnClickedCheckMillero();

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

void CKPSolutionSpeciesPg1::OnGotoNextClicked() 
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

void CKPSolutionSpeciesPg1::OnLeaveCellGrid() 
{
	TRACE("OnLeaveCellGrid\n");

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

void CKPSolutionSpeciesPg1::OnGotoFirstClicked() 
{
	TRACE("OnGotoFirstClicked\n");

	OnLeaveCellGrid();
	m_ctrlGrid.SetRow(1);
	m_ctrlGrid.GetCellLeft();
	OnRowColChangeGrid();

	ResetFocus();
}

void CKPSolutionSpeciesPg1::OnGotoLastClicked() 
{
	TRACE("OnGotoLastClicked\n");

	OnLeaveCellGrid();
	long nRow = 1;
	for (; nRow < m_ctrlGrid.GetRows(); ++nRow)
	{
		if (m_ctrlGrid.GetTextMatrix(nRow, NCOL_RXN).IsEmpty())
			break;
	}
	m_ctrlGrid.SetRow(__max(nRow - 1, 1));
	m_ctrlGrid.GetCellLeft();
	OnRowColChangeGrid();
	ResetFocus();
}

void CKPSolutionSpeciesPg1::OnGotoNewClicked() 
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

void CKPSolutionSpeciesPg1::OnGotoPrevClicked() 
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

void CKPSolutionSpeciesPg1::OnChangeEditAssocRxn() 
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

void CKPSolutionSpeciesPg1::OnChangeEditLogk() 
{
	TRACE("OnChangeEditLogk\n");

	if (!m_bIgnoreChanges)
	{
		CString str;
		m_ctrlLogK.GetWindowText(str);
		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_LOGK, str);
	}
}

void CKPSolutionSpeciesPg1::OnKillfocusEditAssocRxn() 
{
	TRACE("OnKillfocusEditAssocRxn\n");
	m_hWndLastControl = m_ctrlAssocRxn.m_hWnd;
	m_bEditLastControl = TRUE;	
}

void CKPSolutionSpeciesPg1::OnEnKillfocusEditDw()
{
	TRACE("OnEnKillfocusEditDw\n");
	m_hWndLastControl = m_ctrlDW.m_hWnd;
	m_bEditLastControl = TRUE;	
}

void CKPSolutionSpeciesPg1::OnEnKillfocusEditErmDll()
{
	TRACE("OnEnKillfocusEditErmDll\n");
	m_hWndLastControl = m_ctrlERM_DLL.m_hWnd;
	m_bEditLastControl = TRUE;	
}

void CKPSolutionSpeciesPg1::OnActCoefRadioClicked(UINT nID)
{
	TRACE("OnActCoefRadioClicked\n");

	GetDlgItem(IDC_EDIT_DHA)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_DHB)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_DHA)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_DHB)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_LLNL_DHA)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_LLNL_DHA)->EnableWindow(FALSE);

	long nRow = m_ctrlGrid.GetRow();
	switch (nID)
	{
	case IDC_RADIO_DAVIES:
		m_ctrlGrid.SetTextMatrix(nRow, NCOL_ACT_TYPE, m_mapActTypeToStr[CSpecies::AT_DAVIES]);
		break;

	case IDC_RADIO_LLNL_CO2:
		m_ctrlGrid.SetTextMatrix(nRow, NCOL_ACT_TYPE, m_mapActTypeToStr[CSpecies::AT_LLNL_DH_CO2]);
		break;

	case IDC_RADIO_LLNL_DH:
		m_ctrlGrid.SetTextMatrix(nRow, NCOL_ACT_TYPE, m_mapActTypeToStr[CSpecies::AT_LLNL_DH]);
		GetDlgItem(IDC_STATIC_LLNL_DHA)->EnableWindow(TRUE);
		m_ctrlLLNLa.EnableWindow(TRUE);
		break;

	case IDC_RADIO_DH:
		m_ctrlGrid.SetTextMatrix(nRow, NCOL_ACT_TYPE, m_mapActTypeToStr[CSpecies::AT_DEBYE_HUCKEL]);
		GetDlgItem(IDC_STATIC_DHA)->EnableWindow(TRUE);
		m_ctrlDHa.EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_DHB)->EnableWindow(TRUE);
		m_ctrlDHb.EnableWindow(TRUE);
		break;

	default:
		ASSERT(FALSE);
		break;
	}
}

void CKPSolutionSpeciesPg1::OnChangeEditDha() 
{
	TRACE("OnChangeEditDha\n");

	if (!m_bIgnoreChanges)
	{
		CString str;
		m_ctrlDHa.GetWindowText(str);
		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_DHA, str);

		CString str2;
		m_ctrlLLNLa.GetWindowText(str2);
		if (str != str2) m_ctrlLLNLa.SetWindowText(str);
	}
}

void CKPSolutionSpeciesPg1::OnChangeEditDhb() 
{
	TRACE("OnChangeEditDhb\n");

	CString str;
	m_ctrlDHb.GetWindowText(str);
	m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_DHB, str);
}

void CKPSolutionSpeciesPg1::OnChangeEditLlnlDha() 
{
	TRACE("OnChangeEditLlnlDha\n");

	if (!m_bIgnoreChanges)
	{
		CString str;
		m_ctrlLLNLa.GetWindowText(str);
		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_DHA, str);

		CString str2;
		m_ctrlDHa.GetWindowText(str2);
		if (str != str2) m_ctrlDHa.SetWindowText(str);
	}
}

void CKPSolutionSpeciesPg1::OnChangeEditDeltaH() 
{
	TRACE("OnChangeEditDeltaH\n");

	if (!m_bIgnoreChanges)
	{
		CString str;
		m_ctrlDeltaH.GetWindowText(str);
		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_DELTA_H, str);	
	}
}

void CKPSolutionSpeciesPg1::OnSelchangeComboDeltaHUnits() 
{
	TRACE("OnSelchangeComboDeltaHUnits\n");

	CString strUnits;
	m_ctrlDeltaHUnits.GetWindowText(strUnits);
	m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_DELTA_H_UNITS, strUnits);
}

void CKPSolutionSpeciesPg1::OnCheckEquation() 
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

void CKPSolutionSpeciesPg1::OnChangeEditMoleBal() 
{
	TRACE("OnChangeEditMoleBal\n");

	if (!m_bIgnoreChanges)
	{
		CString str;
		m_ctrlMoleBal.GetWindowText(str);
		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_MOLE_BAL, str);
	}
}

void CKPSolutionSpeciesPg1::OnChangeEditA1() 
{
	TRACE("OnChangeEditA1\n");

	if (!m_bIgnoreChanges)
	{
		CString str;
		m_ctrlA1.GetWindowText(str);
		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_A1, str);
	}
}

void CKPSolutionSpeciesPg1::OnChangeEditA2() 
{
	TRACE("OnChangeEditA2\n");

	if (!m_bIgnoreChanges)
	{
		CString str;
		m_ctrlA2.GetWindowText(str);
		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_A2, str);
	}
}

void CKPSolutionSpeciesPg1::OnChangeEditA3() 
{
	TRACE("OnChangeEditA3\n");

	if (!m_bIgnoreChanges)
	{
		CString str;
		m_ctrlA3.GetWindowText(str);
		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_A3, str);
	}
}

void CKPSolutionSpeciesPg1::OnChangeEditA4() 
{
	TRACE("OnChangeEditA4\n");

	if (!m_bIgnoreChanges)
	{
		CString str;
		m_ctrlA4.GetWindowText(str);
		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_A4, str);
	}
}

void CKPSolutionSpeciesPg1::OnChangeEditA5() 
{
	TRACE("OnChangeEditA5\n");

	if (!m_bIgnoreChanges)
	{
		CString str;
		m_ctrlA5.GetWindowText(str);
		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_A5, str);
	}
}

void CKPSolutionSpeciesPg1::OnChangeEditA6() 
{
	TRACE("OnChangeEditA6\n");

	if (!m_bIgnoreChanges)
	{
		CString str;
		m_ctrlA6.GetWindowText(str);
		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_A6, str);
	}
}

LRESULT CKPSolutionSpeciesPg1::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
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

		case NCOL_A1: case NCOL_A2: case NCOL_A3: case NCOL_A4: case NCOL_A5: case NCOL_A6:
			break;

		case NCOL_ACT_TYPE :
			if (pInfo->item.hWndCombo)
			{
				CComboBox* pCombo = (CComboBox*)CWnd::FromHandle(pInfo->item.hWndCombo);
				pCombo->ResetContent();
				std::map<CString, CSpecies::ActType>::iterator cIter = m_mapStrToActType.begin();
				for (; cIter != m_mapStrToActType.end(); ++cIter)
				{
					if (!m_bEnableLLNL)
					{
						if (cIter->second == CSpecies::AT_LLNL_DH) continue;
						if (cIter->second == CSpecies::AT_LLNL_DH_CO2) continue;
					}
					pCombo->AddString(cIter->first);
				}
				pInfo->item.bUseCombo = (pCombo != NULL && pCombo->GetCount());
			}
			else
			{
				pInfo->item.bUseCombo = TRUE;
			}
			break;

		case NCOL_DHA: case NCOL_DHB:
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

		case NCOL_DW :       // dw
			break;

		case NCOL_ERM_DLL :  // erm_dll
			break;

		case NCOL_MILLERO_0: case NCOL_MILLERO_1: case NCOL_MILLERO_2: case NCOL_MILLERO_3: case NCOL_MILLERO_4: case NCOL_MILLERO_5:
			break;

		default :
			ASSERT(FALSE);
			break;
		}
		break;
	}

	return bDenyEdit;
}

LRESULT CKPSolutionSpeciesPg1::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
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

		case NCOL_ACT_TYPE: // Activity options
			{
				CSpecies::ActType nType = CSpecies::AT_DAVIES;

				// pInfo->item.pszText is NULL if edit is cancelled
				CString strActType = (pInfo->item.pszText) ? pInfo->item.pszText : m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol);

				if (!strActType.IsEmpty())
				{
					strActType.MakeUpper();
					std::map<CString, CSpecies::ActType>::iterator iter = m_mapStrToActType.find(strActType);
					if (iter != m_mapStrToActType.end())
					{
						nType = iter->second;
					}
				}
				else
				{
					m_ctrlGrid.SetTextMatrix(pInfo->item.iRow, pInfo->item.iCol, m_mapActTypeToStr[CSpecies::AT_DAVIES]);
				}

				UINT nID = IDC_RADIO_DAVIES;
				switch (nType)
				{
				case CSpecies::AT_DAVIES:
					nID = IDC_RADIO_DAVIES;
					break;
				case CSpecies::AT_DEBYE_HUCKEL:
					nID = IDC_RADIO_DH;
					break;
				case CSpecies::AT_LLNL_DH:
					nID = IDC_RADIO_LLNL_DH;
					if (!m_bEnableLLNL) EnableLLNL(true);
					break;
				case CSpecies::AT_LLNL_DH_CO2:
					nID = IDC_RADIO_LLNL_CO2;
					if (!m_bEnableLLNL) EnableLLNL(true);
					break;
				}

				CheckRadioButton(IDC_RADIO_DAVIES, IDC_RADIO_LLNL_CO2, nID);				
				GetDlgItem(IDC_STATIC_DHA)->EnableWindow(nType == CSpecies::AT_DEBYE_HUCKEL);
				GetDlgItem(IDC_EDIT_DHA)->EnableWindow(nType == CSpecies::AT_DEBYE_HUCKEL);
				GetDlgItem(IDC_STATIC_DHB)->EnableWindow(nType == CSpecies::AT_DEBYE_HUCKEL);
				GetDlgItem(IDC_EDIT_DHB)->EnableWindow(nType == CSpecies::AT_DEBYE_HUCKEL);
				GetDlgItem(IDC_STATIC_LLNL_DHA)->EnableWindow(nType == CSpecies::AT_LLNL_DH);
				GetDlgItem(IDC_EDIT_LLNL_DHA)->EnableWindow(nType == CSpecies::AT_LLNL_DH);
			}
			break;

		case NCOL_DHA: // Act. a
			if (pInfo->item.pszText == NULL)
			{
				// edit cancelled
				m_ctrlDHa.SetWindowText(m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol));
				m_ctrlLLNLa.SetWindowText(m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol));
			}
			else
			{
				m_ctrlDHa.SetWindowText(pInfo->item.pszText);
				m_ctrlLLNLa.SetWindowText(pInfo->item.pszText);
			}
			break;

		case NCOL_DHB: // Act. b
			if (pInfo->item.pszText == NULL)
			{
				// edit cancelled
				m_ctrlDHb.SetWindowText(m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol));
			}
			else
			{
				m_ctrlDHb.SetWindowText(pInfo->item.pszText);
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

		case NCOL_DW: // dw
			if (pInfo->item.pszText == NULL)
			{
				// edit cancelled
				m_ctrlDW.SetWindowText(m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol));
			}
			else
			{
				m_ctrlDW.SetWindowText(pInfo->item.pszText);
			}
			break;

		case NCOL_ERM_DLL: // erm_dll
			if (pInfo->item.pszText == NULL)
			{
				// edit cancelled
				m_ctrlERM_DLL.SetWindowText(m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol));
			}
			else
			{
				m_ctrlERM_DLL.SetWindowText(pInfo->item.pszText);
			}
			break;

		case NCOL_MILLERO_0: // millero
			if (pInfo->item.pszText == NULL)
			{
				// edit cancelled
				m_ctrlM0.SetWindowText(m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol));
			}
			else
			{
				m_ctrlM0.SetWindowText(pInfo->item.pszText);
			}
			break;

		case NCOL_MILLERO_1: // millero
			if (pInfo->item.pszText == NULL)
			{
				// edit cancelled
				m_ctrlM1.SetWindowText(m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol));
			}
			else
			{
				m_ctrlM1.SetWindowText(pInfo->item.pszText);
			}
			break;

		case NCOL_MILLERO_2: // millero
			if (pInfo->item.pszText == NULL)
			{
				// edit cancelled
				m_ctrlM2.SetWindowText(m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol));
			}
			else
			{
				m_ctrlM2.SetWindowText(pInfo->item.pszText);
			}
			break;

		case NCOL_MILLERO_3: // millero
			if (pInfo->item.pszText == NULL)
			{
				// edit cancelled
				m_ctrlM3.SetWindowText(m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol));
			}
			else
			{
				m_ctrlM3.SetWindowText(pInfo->item.pszText);
			}
			break;

		case NCOL_MILLERO_4: // millero
			if (pInfo->item.pszText == NULL)
			{
				// edit cancelled
				m_ctrlM4.SetWindowText(m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol));
			}
			else
			{
				m_ctrlM4.SetWindowText(pInfo->item.pszText);
			}
			break;

		case NCOL_MILLERO_5: // millero
			if (pInfo->item.pszText == NULL)
			{
				// edit cancelled
				m_ctrlM5.SetWindowText(m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol));
			}
			else
			{
				m_ctrlM5.SetWindowText(pInfo->item.pszText);
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

void CKPSolutionSpeciesPg1::OnKillfocusEditLogk() 
{
	m_hWndLastControl = m_ctrlLogK.m_hWnd;
	m_bEditLastControl = TRUE;	
}

void CKPSolutionSpeciesPg1::OnKillfocusEditDeltaH() 
{
	m_hWndLastControl = m_ctrlDeltaH.m_hWnd;
	m_bEditLastControl = TRUE;	
}

void CKPSolutionSpeciesPg1::OnKillfocusEditA1() 
{
	m_hWndLastControl = m_ctrlA1.m_hWnd;
	m_bEditLastControl = TRUE;	
}

void CKPSolutionSpeciesPg1::OnKillfocusEditA2() 
{
	m_hWndLastControl = m_ctrlA2.m_hWnd;
	m_bEditLastControl = TRUE;	
}

void CKPSolutionSpeciesPg1::OnKillfocusEditA3() 
{
	m_hWndLastControl = m_ctrlA3.m_hWnd;
	m_bEditLastControl = TRUE;	
}

void CKPSolutionSpeciesPg1::OnKillfocusEditA4() 
{
	m_hWndLastControl = m_ctrlA4.m_hWnd;
	m_bEditLastControl = TRUE;	
}

void CKPSolutionSpeciesPg1::OnKillfocusEditA5() 
{
	m_hWndLastControl = m_ctrlA5.m_hWnd;
	m_bEditLastControl = TRUE;	
}

void CKPSolutionSpeciesPg1::OnKillfocusEditA6() 
{
	m_hWndLastControl = m_ctrlA6.m_hWnd;
	m_bEditLastControl = TRUE;	
}

void CKPSolutionSpeciesPg1::ResetFocus()
{
	if (m_hWndLastControl != NULL)
	{
		// restore focus and selection last control
		::SetFocus(m_hWndLastControl);
		if (m_bEditLastControl) // select edit item
			::SendMessage(m_hWndLastControl, EM_SETSEL, 0, -1);
	}
}

LRESULT CKPSolutionSpeciesPg1::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
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

LRESULT CKPSolutionSpeciesPg1::OnChange(WPARAM wParam, LPARAM lParam)
{
	TRACE("CKPSolutionSpeciesPg1::OnChange\n");

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

		case NCOL_A6:  // A6
			m_ctrlA6.SetWindowText(pInfo->item.pszText);
			break;

		case NCOL_ACT_TYPE: // Activity
			{
				CSpecies::ActType nType = CSpecies::AT_DAVIES;
				CString strActType = pInfo->item.pszText;
				if (!strActType.IsEmpty())
				{
					strActType.MakeUpper();
					std::map<CString, CSpecies::ActType>::iterator iter = m_mapStrToActType.find(strActType);
					if (iter != m_mapStrToActType.end())
					{
						nType = iter->second;
					}
				}
				else
				{
					m_ctrlGrid.SetTextMatrix(pInfo->item.iRow, NCOL_ACT_TYPE, m_mapActTypeToStr[CSpecies::AT_DAVIES]);
				}
				UINT nID = IDC_RADIO_DAVIES;
				switch (nType)
				{
				case CSpecies::AT_DAVIES:
					nID = IDC_RADIO_DAVIES;
					break;
				case CSpecies::AT_DEBYE_HUCKEL:
					nID = IDC_RADIO_DH;
					break;
				case CSpecies::AT_LLNL_DH:
					nID = IDC_RADIO_LLNL_DH;
					if (!m_bEnableLLNL) EnableLLNL(true);
					break;
				case CSpecies::AT_LLNL_DH_CO2:
					nID = IDC_RADIO_LLNL_CO2;
					if (!m_bEnableLLNL) EnableLLNL(true);
					break;
				default:
					ASSERT(FALSE);
					break;
				}
				CheckRadioButton(IDC_RADIO_DAVIES, IDC_RADIO_LLNL_CO2, nID);				
				GetDlgItem(IDC_STATIC_DHA)->EnableWindow(nType == CSpecies::AT_DEBYE_HUCKEL);
				GetDlgItem(IDC_EDIT_DHA)->EnableWindow(nType == CSpecies::AT_DEBYE_HUCKEL);
				GetDlgItem(IDC_STATIC_DHB)->EnableWindow(nType == CSpecies::AT_DEBYE_HUCKEL);
				GetDlgItem(IDC_EDIT_DHB)->EnableWindow(nType == CSpecies::AT_DEBYE_HUCKEL);
				GetDlgItem(IDC_STATIC_LLNL_DHA)->EnableWindow(nType == CSpecies::AT_LLNL_DH);
				GetDlgItem(IDC_EDIT_LLNL_DHA)->EnableWindow(nType == CSpecies::AT_LLNL_DH);
			}
			break;

		case NCOL_DHA: // Act. a
			m_ctrlDHa.SetWindowText(pInfo->item.pszText);
			m_ctrlLLNLa.SetWindowText(pInfo->item.pszText);
			break;

		case NCOL_DHB: // Act. b
			m_ctrlDHb.SetWindowText(pInfo->item.pszText);
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

		case NCOL_DW:  // dw
			m_ctrlDW.SetWindowText(pInfo->item.pszText);
			break;

		case NCOL_ERM_DLL:  // erm_dll
			m_ctrlERM_DLL.SetWindowText(pInfo->item.pszText);
			break;

		case NCOL_MILLERO_0:  // millero 
			m_ctrlM0.SetWindowText(pInfo->item.pszText);
			break;

		case NCOL_MILLERO_1:  // millero 
			m_ctrlM1.SetWindowText(pInfo->item.pszText);
			break;

		case NCOL_MILLERO_2:  // millero 
			m_ctrlM2.SetWindowText(pInfo->item.pszText);
			break;

		case NCOL_MILLERO_3:  // millero 
			m_ctrlM3.SetWindowText(pInfo->item.pszText);
			break;

		case NCOL_MILLERO_4:  // millero 
			m_ctrlM4.SetWindowText(pInfo->item.pszText);
			break;

		case NCOL_MILLERO_5:  // millero 
			m_ctrlM5.SetWindowText(pInfo->item.pszText);
			break;

		default :
			ASSERT(FALSE);
			break;
		}
		break;
	}
	return 0;

}
LRESULT CKPSolutionSpeciesPg1::OnAllowDeleteRow(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	ASSERT(nID == IDC_MSHFLEXGRID1);

	return TRUE;  // allow
}

LRESULT CKPSolutionSpeciesPg1::OnDeleteRow(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	ASSERT(nID == IDC_MSHFLEXGRID1);

	m_ctrlGrid.DeleteRow(pInfo->item.iRow);	
	return TRUE; // handled locally
}

LRESULT CKPSolutionSpeciesPg1::OnAllowInsertRow(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	ASSERT(nID == IDC_MSHFLEXGRID1);

	return FALSE;  // disallow for now
}

LRESULT CKPSolutionSpeciesPg1::OnInsertRow(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	ASSERT(nID == IDC_MSHFLEXGRID1);
	
	ASSERT(FALSE); // OnAllowInsertRow disallowed

	return TRUE; // handled locally

	return FALSE; // allow default handler
}

void CKPSolutionSpeciesPg1::OnDblClickGrid() 
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


BOOL CKPSolutionSpeciesPg1::OnInitDialog() 
{
	baseCKPSolutionSpeciesPg1::OnInitDialog();
	
	// Add extra initialization here
	EnableLLNL(m_bEnableLLNL);

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
						<< item(IDC_STATIC_A6, NORESIZE | ALIGN_CENTER)
						<< item(IDC_EDIT_A6, ABSOLUTE_VERT | ALIGN_CENTER)
						)
					)

				<< (paneCtrl(IDC_GB_ACT_COEF, VERTICAL, ABSOLUTE_VERT, nDefaultBorder, 10, 10)
					<< (pane(HORIZONTAL, GREEDY)
						<< item(IDC_RADIO_DAVIES, NORESIZE | ALIGN_CENTER)
						<< item((CWnd*)NULL, NORESIZE | ALIGN_CENTER, 12, 13, 12, 13)
						<< item((CWnd*)NULL, ABSOLUTE_VERT | ALIGN_CENTER, 54, 20, 54, 20)
						<< item((CWnd*)NULL, NORESIZE | ALIGN_CENTER, 12, 13, 12, 13)
						<< item((CWnd*)NULL, ABSOLUTE_VERT | ALIGN_CENTER, 54, 20, 54, 20)
						<< itemGrowing(HORIZONTAL)
						<< item(IDC_RADIO_LLNL_CO2, NORESIZE | ALIGN_CENTER)
						<< item((CWnd*)NULL, NORESIZE | ALIGN_CENTER, 12, 13, 12, 13)
						<< item((CWnd*)NULL, ABSOLUTE_VERT | ALIGN_CENTER, 55, 20, 55, 20)
						)
					<< (pane(HORIZONTAL, GREEDY)
						<< item(IDC_RADIO_DH, NORESIZE | ALIGN_CENTER)
						<< item(IDC_STATIC_DHA, NORESIZE | ALIGN_CENTER)
						<< item(IDC_EDIT_DHA, ABSOLUTE_VERT | ALIGN_CENTER)
						<< item(IDC_STATIC_DHB, NORESIZE | ALIGN_CENTER)
						<< item(IDC_EDIT_DHB, ABSOLUTE_VERT | ALIGN_CENTER)
						<< itemGrowing(HORIZONTAL)
						<< item(IDC_RADIO_LLNL_DH, NORESIZE | ALIGN_CENTER)
						<< item(IDC_STATIC_LLNL_DHA, NORESIZE | ALIGN_CENTER)
						<< item(IDC_EDIT_LLNL_DHA, ABSOLUTE_VERT | ALIGN_CENTER)
						)
					)

				<< (paneCtrl(IDC_STATIC_DIFFC, VERTICAL, ABSOLUTE_VERT, nDefaultBorder, /*sizeExtraBorder*/10, /*sizeTopExtra*/10, /*sizeSecondary*/0)
					<< (pane(HORIZONTAL)
						<< item(IDC_STATIC_DW, NORESIZE | ALIGN_CENTER)
						<< item(IDC_EDIT_DW, ABSOLUTE_VERT | ALIGN_CENTER)
						<< itemGrowing(HORIZONTAL)
						<< item(IDC_STATIC_ERM_DLL, NORESIZE | ALIGN_CENTER)
						<< item(IDC_EDIT_ERM_DLL, ABSOLUTE_VERT | ALIGN_CENTER)
						<< itemGrowing(HORIZONTAL)
						)
					)

				<< (paneCtrl(IDC_STATIC_MILLERO, VERTICAL, ABSOLUTE_VERT, nDefaultBorder, /*sizeExtraBorder*/10, /*sizeTopExtra*/10, /*sizeSecondary*/0)
					<< (pane(HORIZONTAL)
						<< item(IDC_ST_MILL_0, NORESIZE | ALIGN_CENTER)
						<< item(IDC_EDIT_MILL_0, ABSOLUTE_VERT | ALIGN_CENTER)
						<< item(IDC_ST_MILL_1, NORESIZE | ALIGN_CENTER)
						<< item(IDC_EDIT_MILL_1, ABSOLUTE_VERT | ALIGN_CENTER)
						<< item(IDC_ST_MILL_2, NORESIZE | ALIGN_CENTER)
						<< item(IDC_EDIT_MILL_2, ABSOLUTE_VERT | ALIGN_CENTER)
						<< item(IDC_ST_MILL_3, NORESIZE | ALIGN_CENTER)
						<< item(IDC_EDIT_MILL_3, ABSOLUTE_VERT | ALIGN_CENTER)
						<< item(IDC_ST_MILL_4, NORESIZE | ALIGN_CENTER)
						<< item(IDC_EDIT_MILL_4, ABSOLUTE_VERT | ALIGN_CENTER)
						<< item(IDC_ST_MILL_5, NORESIZE | ALIGN_CENTER)
						<< item(IDC_EDIT_MILL_5, ABSOLUTE_VERT | ALIGN_CENTER)
						)
					)

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

void CKPSolutionSpeciesPg1::EnableLLNL(bool bEnableLLNL)
{
	m_bEnableLLNL = bEnableLLNL;

	if (m_hWnd && ::IsWindow(m_hWnd))
	{
		CWnd* pWnd = NULL;
		if (m_bEnableLLNL)
		{
			pWnd = GetDlgItem(IDC_RADIO_LLNL_CO2);
			if (pWnd)
			{
				pWnd->EnableWindow(TRUE);
				pWnd->ModifyStyle(0, WS_VISIBLE);
				pWnd->Invalidate();
			}

			pWnd = GetDlgItem(IDC_RADIO_LLNL_DH);
			if (pWnd)
			{
				pWnd->EnableWindow(TRUE);
				pWnd->ModifyStyle(0, WS_VISIBLE);
				pWnd->Invalidate();
			}

			pWnd = GetDlgItem(IDC_STATIC_LLNL_DHA);
			if (pWnd)
			{
				pWnd->ModifyStyle(0, WS_VISIBLE);
				pWnd->Invalidate();
			}

			pWnd = GetDlgItem(IDC_EDIT_LLNL_DHA);
			if (pWnd)
			{
				pWnd->ModifyStyle(0, WS_VISIBLE);
				pWnd->Invalidate();
			}
		}
		else
		{
			pWnd = GetDlgItem(IDC_RADIO_LLNL_CO2);
			if (pWnd)
			{
				pWnd->EnableWindow(FALSE);
				pWnd->ModifyStyle(WS_VISIBLE, 0);
			}

			pWnd = GetDlgItem(IDC_RADIO_LLNL_DH);
			if (pWnd)
			{
				pWnd->EnableWindow(FALSE);
				pWnd->ModifyStyle(WS_VISIBLE, 0);
			}

			pWnd = GetDlgItem(IDC_STATIC_LLNL_DHA);
			if (pWnd)
			{
				pWnd->ModifyStyle(WS_VISIBLE, 0);
			}

			pWnd = GetDlgItem(IDC_EDIT_LLNL_DHA);
			if (pWnd)
			{
				pWnd->ModifyStyle(WS_VISIBLE, 0);
			}
		}
	}
}

void CKPSolutionSpeciesPg1::OnSetfocusEditAssocRxn() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING547);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPSolutionSpeciesPg1::OnSetfocusEditLogk() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING548);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPSolutionSpeciesPg1::OnSetfocusEditDeltaH() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING549);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPSolutionSpeciesPg1::OnSetfocusComboDeltaHUnits() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING550);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPSolutionSpeciesPg1::OnSetfocusEditA1() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING551);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPSolutionSpeciesPg1::OnSetfocusEditA2() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING552);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPSolutionSpeciesPg1::OnSetfocusEditA3() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING553);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPSolutionSpeciesPg1::OnSetfocusEditA4() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING554);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPSolutionSpeciesPg1::OnSetfocusEditA5() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING555);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPSolutionSpeciesPg1::OnSetfocusEditA6() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING648);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPSolutionSpeciesPg1::OnSetfocusEditDha() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING556);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPSolutionSpeciesPg1::OnSetfocusEditDhb() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING557);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPSolutionSpeciesPg1::OnSetfocusEditLlnlDha() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING558);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPSolutionSpeciesPg1::OnSetfocusRadio(UINT nID) 
{
	UINT nResID = nID - IDC_RADIO_DAVIES + IDS_STRING559;

	CString strRes;
	strRes.LoadString(nResID);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPSolutionSpeciesPg1::OnSetfocusEditMoleBal() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING563);
	m_eInputDesc.SetWindowText(strRes);	
}


BOOL CKPSolutionSpeciesPg1::OnHelpInfo(HELPINFO* pHelpInfo) 
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
		strRes.LoadString(IDS_STRING547);
		break;
	case IDC_EDIT_LOGK: case IDC_STATIC_LOGK:
		strRes.LoadString(IDS_STRING548);
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
	case IDC_RADIO_DAVIES:
		strRes.LoadString(IDS_STRING559);
		break;
	case IDC_RADIO_DH:
		strRes.LoadString(IDS_STRING560);
		break;
	case IDC_EDIT_DHA: case IDC_STATIC_DHA:
		strRes.LoadString(IDS_STRING556);
		break;
	case IDC_EDIT_DHB: case IDC_STATIC_DHB:
		strRes.LoadString(IDS_STRING557);
		break;
	case IDC_RADIO_LLNL_CO2:
		strRes.LoadString(IDS_STRING561);
		break;
	case IDC_RADIO_LLNL_DH:
		strRes.LoadString(IDS_STRING562);
		break;
	case IDC_EDIT_LLNL_DHA: case IDC_STATIC_LLNL_DHA:
		strRes.LoadString(IDS_STRING558);
		break;
	case IDC_EDIT_MOLE_BAL: case IDC_STATIC_MOLE_BAL:
		strRes.LoadString(IDS_STRING563);
		break;
	case IDC_CHECK_EQUATION:
		strRes.LoadString(IDS_STRING565);
		break;
	case IDC_STATIC_DW: case IDC_EDIT_DW:
		strRes.LoadString(IDS_STRING673);
		break;
	case IDC_STATIC_ERM_DLL: case IDC_EDIT_ERM_DLL:
		strRes.LoadString(IDS_STRING674);
		break;
	case IDC_ST_MILL_0: case IDC_EDIT_MILL_0:
		strRes.LoadString(IDS_STRING675);
		break;
	case IDC_ST_MILL_1: case IDC_EDIT_MILL_1:
		strRes.LoadString(IDS_STRING676);
		break;
	case IDC_ST_MILL_2: case IDC_EDIT_MILL_2:
		strRes.LoadString(IDS_STRING677);
		break;
	case IDC_ST_MILL_3: case IDC_EDIT_MILL_3:
		strRes.LoadString(IDS_STRING678);
		break;
	case IDC_ST_MILL_4: case IDC_EDIT_MILL_4:
		strRes.LoadString(IDS_STRING679);
		break;
	case IDC_ST_MILL_5: case IDC_EDIT_MILL_5:
		strRes.LoadString(IDS_STRING680);
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
			nResID = IDS_STRING547;
			break;
		case NCOL_LOGK:
			nResID = IDS_STRING548;
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
		case NCOL_ACT_TYPE:
			// TODO
			break;
		case NCOL_DHA:
			{
				CString strActType = m_ctrlGrid.GetTextMatrix(m_ctrlGrid.GetRow(), NCOL_ACT_TYPE);
				strActType.MakeUpper();
				std::map<CString, CSpecies::ActType>::iterator iter = m_mapStrToActType.find(strActType);
				if (iter != m_mapStrToActType.end())
				{
					switch (iter->second)
					{
					case CSpecies::AT_DEBYE_HUCKEL:
						nResID = IDS_STRING556;
						break;
					case CSpecies::AT_LLNL_DH:
						nResID = IDS_STRING558;
						break;
					}
				}
			}
			break;
		case NCOL_DHB:
			{
				CString strActType = m_ctrlGrid.GetTextMatrix(m_ctrlGrid.GetRow(), NCOL_ACT_TYPE);
				strActType.MakeUpper();
				std::map<CString, CSpecies::ActType>::iterator iter = m_mapStrToActType.find(strActType);
				if (iter != m_mapStrToActType.end())
				{
					switch (iter->second)
					{
					case CSpecies::AT_DEBYE_HUCKEL:
						nResID = IDS_STRING557;
						break;
					}
				}
			}
			break;
		case NCOL_CHECK:
			nResID = IDS_STRING564;
			break;
		case NCOL_MOLE_BAL:
			nResID = IDS_STRING563;
			break;
		case NCOL_DW:
			nResID = IDS_STRING673;
			break;
		case NCOL_ERM_DLL:
			nResID = IDS_STRING674;
			break;
		case NCOL_MILLERO_0:
			nResID = IDS_STRING675;
			break;
		case NCOL_MILLERO_1:
			nResID = IDS_STRING676;
			break;
		case NCOL_MILLERO_2:
			nResID = IDS_STRING677;
			break;
		case NCOL_MILLERO_3:
			nResID = IDS_STRING678;
			break;
		case NCOL_MILLERO_4:
			nResID = IDS_STRING679;
			break;
		case NCOL_MILLERO_5:
			nResID = IDS_STRING680;
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

void CKPSolutionSpeciesPg1::OnEnterCellGrid() 
{
	UINT nResID = 0;
	switch (m_ctrlGrid.GetCol())
	{
	case NCOL_RXN:
		nResID = IDS_STRING547;
		break;
	case NCOL_LOGK:
		nResID = IDS_STRING548;
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
	case NCOL_ACT_TYPE:
		// TODO
		break;
	case NCOL_DHA:
		{
			CString strActType = m_ctrlGrid.GetTextMatrix(m_ctrlGrid.GetRow(), NCOL_ACT_TYPE);
			strActType.MakeUpper();
			std::map<CString, CSpecies::ActType>::iterator iter = m_mapStrToActType.find(strActType);
			if (iter != m_mapStrToActType.end())
			{
				switch (iter->second)
				{
				case CSpecies::AT_DEBYE_HUCKEL:
					nResID = IDS_STRING556;
					break;
				case CSpecies::AT_LLNL_DH:
					nResID = IDS_STRING558;
					break;
				}
			}
		}
		break;
	case NCOL_DHB:
		{
			CString strActType = m_ctrlGrid.GetTextMatrix(m_ctrlGrid.GetRow(), NCOL_ACT_TYPE);
			strActType.MakeUpper();
			std::map<CString, CSpecies::ActType>::iterator iter = m_mapStrToActType.find(strActType);
			if (iter != m_mapStrToActType.end())
			{
				switch (iter->second)
				{
				case CSpecies::AT_DEBYE_HUCKEL:
					nResID = IDS_STRING557;
					break;
				}
			}
		}
		break;
	case NCOL_CHECK:
		nResID = IDS_STRING564;
		break;
	case NCOL_MOLE_BAL:
		nResID = IDS_STRING563;
		break;
	case NCOL_DW:
		nResID = IDS_STRING673;
		break;
	case NCOL_ERM_DLL:
		nResID = IDS_STRING674;
		break;
	case NCOL_MILLERO_0:
		nResID = IDS_STRING675;
		break;
	case NCOL_MILLERO_1:
		nResID = IDS_STRING676;
		break;
	case NCOL_MILLERO_2:
		nResID = IDS_STRING677;
		break;
	case NCOL_MILLERO_3:
		nResID = IDS_STRING678;
		break;
	case NCOL_MILLERO_4:
		nResID = IDS_STRING679;
		break;
	case NCOL_MILLERO_5:
		nResID = IDS_STRING680;
		break;
	default:
		ASSERT(FALSE);
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

void CKPSolutionSpeciesPg1::OnKeyDownGrid(short FAR* KeyCode, short Shift) 
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

void CKPSolutionSpeciesPg1::OnEnChangeEditDw()
{
	TRACE("OnEnChangeEditDw\n");
	if (!m_bIgnoreChanges)
	{
		CString str;
		m_ctrlDW.GetWindowText(str);
		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_DW, str);
	}
}

void CKPSolutionSpeciesPg1::OnEnChangeEditErmDll()
{
	TRACE("OnEnChangeEditErmDll\n");
	if (!m_bIgnoreChanges)
	{
		CString str;
		m_ctrlERM_DLL.GetWindowText(str);
		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_ERM_DLL, str);
	}
}

void CKPSolutionSpeciesPg1::OnEnSetfocusEditDw()
{
	CString strRes;
	strRes.LoadString(IDS_STRING673);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPSolutionSpeciesPg1::OnEnSetfocusEditErmDll()
{
	CString strRes;
	strRes.LoadString(IDS_STRING674);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPSolutionSpeciesPg1::OnEnChangeEditMill0()
{
	TRACE("OnEnChangeEditMill0\n");
	if (!m_bIgnoreChanges)
	{
		CString str;
		m_ctrlM0.GetWindowText(str);
		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_MILLERO_0, str);
	}
}

void CKPSolutionSpeciesPg1::OnEnKillfocusEditMill0()
{
	m_hWndLastControl = m_ctrlM0.m_hWnd;
	m_bEditLastControl = TRUE;	
}

void CKPSolutionSpeciesPg1::OnEnSetfocusEditMill0()
{
	CString strRes;
	strRes.LoadString(IDS_STRING675);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPSolutionSpeciesPg1::OnEnChangeEditMill1()
{
	TRACE("OnEnChangeEditMill1\n");
	if (!m_bIgnoreChanges)
	{
		CString str;
		m_ctrlM1.GetWindowText(str);
		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_MILLERO_1, str);
	}
}

void CKPSolutionSpeciesPg1::OnEnKillfocusEditMill1()
{
	m_hWndLastControl = m_ctrlM1.m_hWnd;
	m_bEditLastControl = TRUE;	
}

void CKPSolutionSpeciesPg1::OnEnSetfocusEditMill1()
{
	CString strRes;
	strRes.LoadString(IDS_STRING676);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPSolutionSpeciesPg1::OnEnChangeEditMill2()
{
	TRACE("OnEnChangeEditMill2\n");
	if (!m_bIgnoreChanges)
	{
		CString str;
		m_ctrlM2.GetWindowText(str);
		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_MILLERO_2, str);
	}
}

void CKPSolutionSpeciesPg1::OnEnKillfocusEditMill2()
{
	m_hWndLastControl = m_ctrlM2.m_hWnd;
	m_bEditLastControl = TRUE;	
}

void CKPSolutionSpeciesPg1::OnEnSetfocusEditMill2()
{
	CString strRes;
	strRes.LoadString(IDS_STRING677);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPSolutionSpeciesPg1::OnEnChangeEditMill3()
{
	TRACE("OnEnChangeEditMill3\n");
	if (!m_bIgnoreChanges)
	{
		CString str;
		m_ctrlM3.GetWindowText(str);
		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_MILLERO_3, str);
	}
}

void CKPSolutionSpeciesPg1::OnEnKillfocusEditMill3()
{
	m_hWndLastControl = m_ctrlM3.m_hWnd;
	m_bEditLastControl = TRUE;	
}

void CKPSolutionSpeciesPg1::OnEnSetfocusEditMill3()
{
	CString strRes;
	strRes.LoadString(IDS_STRING678);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPSolutionSpeciesPg1::OnEnChangeEditMill4()
{
	TRACE("OnEnChangeEditMill4\n");
	if (!m_bIgnoreChanges)
	{
		CString str;
		m_ctrlM4.GetWindowText(str);
		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_MILLERO_4, str);
	}
}

void CKPSolutionSpeciesPg1::OnEnKillfocusEditMill4()
{
	m_hWndLastControl = m_ctrlM4.m_hWnd;
	m_bEditLastControl = TRUE;	
}

void CKPSolutionSpeciesPg1::OnEnSetfocusEditMill4()
{
	CString strRes;
	strRes.LoadString(IDS_STRING679);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPSolutionSpeciesPg1::OnEnChangeEditMill5()
{
	TRACE("OnEnChangeEditMill5\n");
	if (!m_bIgnoreChanges)
	{
		CString str;
		m_ctrlM5.GetWindowText(str);
		m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRow(), NCOL_MILLERO_5, str);
	}
}

void CKPSolutionSpeciesPg1::OnEnKillfocusEditMill5()
{
	m_hWndLastControl = m_ctrlM5.m_hWnd;
	m_bEditLastControl = TRUE;	
}

void CKPSolutionSpeciesPg1::OnEnSetfocusEditMill5()
{
	CString strRes;
	strRes.LoadString(IDS_STRING680);
	m_eInputDesc.SetWindowText(strRes);	
}



void CKPSolutionSpeciesPg1::OnBnClickedCheckAnalExp()
{
	BOOL bEnable = (this->IsDlgButtonChecked(IDC_CHECK_ANAL_EXP) == BST_CHECKED);
	static int ids[] = {
		IDC_STATIC_A1,
		IDC_EDIT_A1,
		IDC_STATIC_A2,
		IDC_EDIT_A2,
		IDC_STATIC_A3,
		IDC_EDIT_A3,
		IDC_STATIC_A4,
		IDC_EDIT_A4,
		IDC_STATIC_A5,
		IDC_EDIT_A5,
		IDC_STATIC_A6,
		IDC_EDIT_A6
	};

	CWnd* pWnd = NULL;
	if (bEnable)
	{
		for (int i = 0; i < sizeof(ids)/sizeof(ids[0]); ++i)
		{
			pWnd = GetDlgItem(ids[i]);
			if (pWnd)
			{
				pWnd->EnableWindow(TRUE);
			}
		}
	}
	else
	{
		for (int i = 0; i < sizeof(ids)/sizeof(ids[0]); ++i)
		{
			pWnd = GetDlgItem(ids[i]);
			if (pWnd)
			{
				pWnd->EnableWindow(FALSE);
			}
		}
	}
}

void CKPSolutionSpeciesPg1::OnBnClickedCheckDiffParam()
{
	BOOL bEnable = (this->IsDlgButtonChecked(IDC_CHECK_DIFF_PARAM) == BST_CHECKED);
	static int ids[] = {
		IDC_STATIC_DW,
		IDC_EDIT_DW,
		IDC_STATIC_ERM_DLL,
		IDC_EDIT_ERM_DLL
	};

	CWnd* pWnd = NULL;
	if (bEnable)
	{
		for (int i = 0; i < sizeof(ids)/sizeof(ids[0]); ++i)
		{
			pWnd = GetDlgItem(ids[i]);
			if (pWnd)
			{
				pWnd->EnableWindow(TRUE);
			}
		}
	}
	else
	{
		for (int i = 0; i < sizeof(ids)/sizeof(ids[0]); ++i)
		{
			pWnd = GetDlgItem(ids[i]);
			if (pWnd)
			{
				pWnd->EnableWindow(FALSE);
			}
		}
	}
}

void CKPSolutionSpeciesPg1::OnBnClickedCheckMillero()
{
	BOOL bEnable = (this->IsDlgButtonChecked(IDC_CHECK_MILLERO) == BST_CHECKED);
	static int ids[] = {
		IDC_ST_MILL_0,
		IDC_EDIT_MILL_0,
		IDC_ST_MILL_1,
		IDC_EDIT_MILL_1,
		IDC_ST_MILL_2,
		IDC_EDIT_MILL_2,
		IDC_ST_MILL_3,
		IDC_EDIT_MILL_3,
		IDC_ST_MILL_4,
		IDC_EDIT_MILL_4,
		IDC_ST_MILL_5,
		IDC_EDIT_MILL_5,
	};

	CWnd* pWnd = NULL;
	if (bEnable)
	{
		for (int i = 0; i < sizeof(ids)/sizeof(ids[0]); ++i)
		{
			pWnd = GetDlgItem(ids[i]);
			if (pWnd)
			{
				pWnd->EnableWindow(TRUE);
			}
		}
	}
	else
	{
		for (int i = 0; i < sizeof(ids)/sizeof(ids[0]); ++i)
		{
			pWnd = GetDlgItem(ids[i]);
			if (pWnd)
			{
				pWnd->EnableWindow(FALSE);
			}
		}
	}
}
