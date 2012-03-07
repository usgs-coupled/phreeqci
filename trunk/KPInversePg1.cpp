// KPInversePg1.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "KPInversePg1.h"

#include "CKSInverse.h"
#include "Util.h"
#include "DDX_DDV.h"
#include "DelayUpdate.h"
#include "KeywordPageListItems.h"


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CKPInversePg1A, baseKPInversePg1A)
IMPLEMENT_DYNCREATE(CKPInversePg1B, CPropertyPage)
IMPLEMENT_DYNCREATE(CKPInversePg2, baseKPInversePg2)
IMPLEMENT_DYNCREATE(CKPInversePg3, baseKPInversePg3)

//{{CKPInversePg1A
/////////////////////////////////////////////////////////////////////////////
// CKPInversePg1A property page

CKPInversePg1A::CKPInversePg1A() : baseKPInversePg1A(CKPInversePg1A::IDD)
, m_glDocInit(&m_egInitSolns, &m_clcInitSolns, 1)
{
	m_bRange = FALSE;
	m_dRange = 1000;
	//{{AFX_DATA_INIT(CKPInversePg1A)
	m_bMinimal = FALSE;
	m_bMineralWater = TRUE;
	m_dTol = 1e-10;
	m_bMPSolve = FALSE;
	m_dMPTol = 1e-12;
	m_dMPCensor = 1e-20;
	m_bLonNetpath = FALSE;
	//}}AFX_DATA_INIT
	m_sLonPrefix = "netpath";
	m_sPatPrefix = "netpath";		
}

CKPInversePg1A::~CKPInversePg1A()
{
}

void CKPInversePg1A::DoDataExchange(CDataExchange* pDX)
{
	baseKPInversePg1A::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CKPInversePg1A)
	DDX_Control(pDX, IDC_CLC_FINALSOLN, m_clcFinalSoln);
	DDX_Control(pDX, IDC_CLC_INITSOLNS, m_clcInitSolns);
	DDX_Control(pDX, IDC_MSHFG_INITSOLNS, m_egInitSolns);
	DDX_Control(pDX, IDC_MSHFG_FINALSOLN, m_egFinalSoln);
	DDX_Check(pDX, IDC_B_MINIMAL, m_bMinimal);
	DDX_Check(pDX, IDC_B_MINERAL_WATER, m_bMineralWater);
	DDX_Text(pDX, IDC_E_TOL, m_dTol);
	DDX_Check(pDX, IDC_MP_SOLVE_CHECK, m_bMPSolve);
	DDX_Check(pDX, IDC_B_WRITE_LON, m_bLonNetpath);
	DDX_Check(pDX, IDC_B_WRITE_PAT, m_bPatNetpath);
	//}}AFX_DATA_MAP

	// Init NumDesc (INVERSE_MODELING doesn't support a range)
	if (m_bFirstSetActive)
	{
		// reduce grid flicker
		CDelayUpdate update(this, IDC_MSHFG_NUM_DESC/*nIDC*/);

		m_egNumDesc.SetColWidth(0, 0, 1100);
		m_egNumDesc.SetTextMatrix(0, 0, _T("Number"));
		m_egNumDesc.SetRowHeight(1, 0);

		// Init solution checklists
		CUtil::InsertRange(&m_clcInitSolns, GetSheet()->m_setSolutions);
		CUtil::InsertRange(&m_clcFinalSoln, GetSheet()->m_setSolutions);

		// Init multiple precision
		OnMPSolveCheck();

		// init lon
		OnBWriteLon();

		// init pat
		OnBWritePat();
	}

	// multiple prec.
	if (pDX->m_bSaveAndValidate)
	{
		if (m_bMPSolve)
		{
			DDX_Text(pDX, IDC_E_MP_TOL, m_dMPTol);
			DDX_Text(pDX, IDC_E_MP_CENSOR, m_dMPCensor);
		}
	}
	else
	{
		DDX_Text(pDX, IDC_E_MP_TOL, m_dMPTol);
		DDX_Text(pDX, IDC_E_MP_CENSOR, m_dMPCensor);
	}

	// lon_netpath
	if (pDX->m_bSaveAndValidate)
	{
		if (m_bLonNetpath)
		{
			DDX_Text(pDX, IDC_E_LON_PREFIX, m_sLonPrefix);
		}
		if (m_sLonPrefix.IsEmpty())
		{
			m_sLonPrefix = "netpath";
		}
	}
	else
	{
		DDX_Text(pDX, IDC_E_LON_PREFIX, m_sLonPrefix);
	}

	// pat_netpath
	if (pDX->m_bSaveAndValidate)
	{
		if (m_bPatNetpath)
		{
			DDX_Text(pDX, IDC_E_PAT_PREFIX, m_sPatPrefix);
		}
		if (m_sPatPrefix.IsEmpty())
		{
			m_sPatPrefix = "netpath";
		}
	}
	else
	{
		DDX_Text(pDX, IDC_E_PAT_PREFIX, m_sPatPrefix);
	}

	DDX_Grids(pDX);

	// exchange/validate range
	DDX_Range(pDX);

	if (pDX->m_bSaveAndValidate)
	{
		// as last step remove all entries from m_glDoc;
		m_glDocInit.DeleteAllRows();
	}
}

BEGIN_MESSAGE_MAP(CKPInversePg1A, baseKPInversePg1A)
	//{{AFX_MSG_MAP(CKPInversePg1A)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CLC_INITSOLNS, OnItemchangedClcInitSolns)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CLC_FINALSOLN, OnItemchangedClcFinalSoln)
	ON_BN_CLICKED(IDC_B_RANGE, OnBRange)
	ON_EN_SETFOCUS(IDC_E_TOL, OnSetfocusETol)
	ON_NOTIFY(NM_SETFOCUS, IDC_CLC_INITSOLNS, OnSetfocusClcInitsolns)
	ON_NOTIFY(NM_SETFOCUS, IDC_CLC_FINALSOLN, OnSetfocusClcFinalsoln)
	ON_WM_HELPINFO()
	ON_EN_SETFOCUS(IDC_E_RANGE, OnSetfocusERange)
	ON_BN_CLICKED(IDC_MP_SOLVE_CHECK, OnMPSolveCheck)
	ON_EN_SETFOCUS(IDC_E_MP_TOL, OnSetfocusEMPTol)
	ON_EN_SETFOCUS(IDC_E_MP_CENSOR, OnSetfocusEMPCensor)
	//}}AFX_MSG_MAP
	ON_BN_SETFOCUS(IDC_B_RANGE, OnSetfocusBRange)
	ON_BN_SETFOCUS(IDC_B_MINERAL_WATER, OnSetfocusBMineralWater)
	ON_BN_SETFOCUS(IDC_B_MINIMAL, OnSetfocusBMinimal)
	// custom edit grid notifications
	ON_MESSAGE(EGN_BEGINCELLEDIT, OnBeginCellEdit)
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
	ON_MESSAGE(EGN_SETFOCUS, OnSetfocusGrid)

	ON_BN_CLICKED(IDC_B_WRITE_LON, OnBWriteLon)
	ON_BN_CLICKED(IDC_B_WRITE_PAT, OnBWritePat)

	ON_EN_SETFOCUS(IDC_E_LON_PREFIX, OnSetfocusELonPrefix)
	ON_EN_SETFOCUS(IDC_E_PAT_PREFIX, OnSetfocusEPatPrefix)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CKPInversePg1A, baseKPInversePg1A)
    //{{AFX_EVENTSINK_MAP(CKPInversePg1A)
	ON_EVENT(CKPInversePg1A, IDC_MSHFG_NUM_DESC, 71 /* EnterCell */, OnEnterCellMshfgNumDesc, VTS_NONE)
	ON_EVENT(CKPInversePg1A, IDC_MSHFG_INITSOLNS, 71 /* EnterCell */, OnEnterCellMshfgInitsolns, VTS_NONE)
	ON_EVENT(CKPInversePg1A, IDC_MSHFG_FINALSOLN, 71 /* EnterCell */, OnEnterCellMshfgFinalsoln, VTS_NONE)
	ON_EVENT(CKPInversePg1A, IDC_MSHFG_INITSOLNS, -602 /* KeyDown */, OnKeyDownMshfgInitSolns, VTS_PI2 VTS_I2)
	ON_EVENT(CKPInversePg1A, IDC_MSHFG_FINALSOLN, -602 /* KeyDown */, OnKeyDownMshfgFinalSoln, VTS_PI2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKPInversePg1A message handlers

BOOL CKPInversePg1A::OnInitDialog() 
{
	baseKPInversePg1A::OnInitDialog();

	//{{
	/*
	IDC_S_INITSOLNS
	IDC_S_FINALSOLN
	IDC_CLC_INITSOLNS
	IDC_CLC_FINALSOLN
	IDC_MSHFG_INITSOLNS
	IDC_MSHFG_FINALSOLN
	*/
	//}}
	
	CreateRoot(VERTICAL, 5, 6) 
		<< (pane(HORIZONTAL, ABSOLUTE_VERT)
			<< item(IDC_MSHFG_NUM_DESC, ABSOLUTE_VERT)
			)
			<< (paneCtrl(IDC_S_OPTIONS, HORIZONTAL, ABSOLUTE_VERT, nDefaultBorder, 4, 15, 0)
				<< (pane(VERTICAL, GREEDY)
					<< (pane(HORIZONTAL, ABSOLUTE_VERT)
						<< itemFixed(HORIZONTAL, 15)
						<< item(IDC_B_MINIMAL, NORESIZE)
						<< itemFixed(HORIZONTAL, 15)
						<< item(IDC_B_RANGE, NORESIZE)
						<< item(IDC_E_RANGE, NORESIZE)
						<< itemFixed(HORIZONTAL, 15)
						<< item(IDC_S_TOL, NORESIZE)
						<< item(IDC_E_TOL, NORESIZE)
						<< itemFixed(HORIZONTAL, 15)
						<< item(IDC_B_MINERAL_WATER, NORESIZE)
						)
					<< (pane(HORIZONTAL, GREEDY)
						<< itemFixed(HORIZONTAL, 15)
						<< item(IDC_MP_SOLVE_CHECK, NORESIZE)
						<< itemFixed(HORIZONTAL, 15)
						<< item(IDC_S_MP_TOL, NORESIZE)
						<< item(IDC_E_MP_TOL, NORESIZE)
						<< itemFixed(HORIZONTAL, 15)
						<< item(IDC_S_MP_CENSOR, NORESIZE)
						<< item(IDC_E_MP_CENSOR, NORESIZE)
						)
					<< (pane(HORIZONTAL, GREEDY)
						<< itemFixed(HORIZONTAL, 15)
						<< item(IDC_B_WRITE_LON, NORESIZE)
						<< item(IDC_E_LON_PREFIX, NORESIZE)
						<< itemFixed(HORIZONTAL, 30)
						<< item(IDC_B_WRITE_PAT, NORESIZE)
						<< item(IDC_E_PAT_PREFIX, NORESIZE)
						)
					)
				)
		<< (pane(HORIZONTAL, GREEDY)
			<< (paneCtrl(IDC_S_INITSOLNS, VERTICAL, GREEDY, nDefaultBorder, 4, 15, 0)
				<< item(IDC_CLC_INITSOLNS, ABSOLUTE_VERT)
				<< item(IDC_MSHFG_INITSOLNS, GREEDY)
				)

			<< (paneCtrl(IDC_S_FINALSOLN, VERTICAL, GREEDY, nDefaultBorder, 4, 15, 0)
				<< item(IDC_CLC_FINALSOLN, ABSOLUTE_VERT)
				<< item(IDC_MSHFG_FINALSOLN, GREEDY)
				)
			)

		<< (paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, GREEDY, nDefaultBorder, 10, 10)
			<< item(IDC_E_DESC_INPUT, ABSOLUTE_VERT)
			);
	UpdateLayout();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CKPInversePg1A::DDX_Grids(CDataExchange* pDX)
{
	// CDelayUpdate delay(&m_egInitSolns);

	if (m_bFirstSetActive)
	{
		InitGrids();
	}

	if (pDX->m_bSaveAndValidate)
	{
		std::list<InvSol> listInvSol;
		InvSol invSolFinal;
		invSolFinal.nSol = -1;

		for (long nRow = m_egInitSolns.GetFixedRows(); nRow != m_egInitSolns.GetRows(); ++nRow)
		{
			InvSol theInvSol;
			CString strDummy;

			DDX_GridText(pDX, IDC_MSHFG_INITSOLNS, nRow, 1, strDummy);
			if (strDummy.IsEmpty()) continue;
			
			// sol #
			DDX_GridText(pDX, IDC_MSHFG_INITSOLNS, nRow, 1, theInvSol.nSol);
			if (theInvSol.nSol < 0)
			{
				// Solution # must be greater than zero.
				DDX_GridFail(pDX, IDS_INVERSE_274);
			}

			// uncertainty
			DDX_GridText(pDX, IDC_MSHFG_INITSOLNS, nRow, 2, strDummy);
			if (strDummy.IsEmpty())
			{
				// set default
				theInvSol.dUncertainty = 0.05;
			}
			else
			{
				DDX_GridText(pDX, IDC_MSHFG_INITSOLNS, nRow, 2, theInvSol.dUncertainty);
			}

			// force
			theInvSol.bForce = false; // default
			DDX_GridText(pDX, IDC_MSHFG_INITSOLNS, nRow, 3, strDummy);
			strDummy.TrimLeft();
			if (!strDummy.IsEmpty())
			{
				switch (strDummy[0])
				{
				case _T('T') : case _T('t') : case _T('Y') : case _T('y') :
					theInvSol.bForce = true;
					break;

				case _T('F') : case _T('f') : case _T('N') : case _T('n') :
					theInvSol.bForce = false;
					break;

				default :
					// The force option should be "true", "false" or empty.
					DDX_GridFail(pDX, IDS_INVERSE_275);
					break;
				}
			}
			
			// add to temporary list
			listInvSol.push_back(theInvSol);
		}

		// check for at least one initial soln
		if (listInvSol.size() == 0)
		{
			// At least one initial solution must be defined.
			CString strTemp;
			DDX_GridText(pDX, IDC_MSHFG_INITSOLNS, m_egInitSolns.GetFixedRows(), m_egInitSolns.GetFixedCols(), strTemp);
			DDX_GridFail(pDX, IDS_INVERSE_277);
		}

		//
		// final solution
		//

		CString strDummy;
		DDX_GridText(pDX, IDC_MSHFG_FINALSOLN, 1, 0, strDummy);
		if (strDummy.IsEmpty())
		{
			// A final solution must be defined.
			DDX_GridFail(pDX, IDS_INVERSE_356);
		}
		else
		{
			// final sol #
			DDX_GridText(pDX, IDC_MSHFG_FINALSOLN, 1, 0, invSolFinal.nSol);
			if (invSolFinal.nSol < 0)
			{
				// Solution # must be greater than zero.
				DDX_GridFail(pDX, IDS_INVERSE_274);
			}
		}

		// uncertainty for final solution
		DDX_GridText(pDX, IDC_MSHFG_FINALSOLN, 1, 1, strDummy);
		if (strDummy.IsEmpty())
		{
			// same as last initial soln
			std::list<InvSol>::const_iterator lastInvSol = listInvSol.end();
			lastInvSol--;
			invSolFinal.dUncertainty = lastInvSol->dUncertainty;
		}
		else
		{
			DDX_GridText(pDX, IDC_MSHFG_FINALSOLN, 1, 1, invSolFinal.dUncertainty);
		}

		// force for final solution
		invSolFinal.bForce = false; // default
		DDX_GridText(pDX, IDC_MSHFG_FINALSOLN, 1, 2, strDummy);
		strDummy.TrimLeft();
		if (!strDummy.IsEmpty())
		{
			switch (strDummy[0])
			{
			case _T('T') : case _T('t') : case _T('Y') : case _T('y') :
				invSolFinal.bForce = true;
				break;

			case _T('F') : case _T('f') : case _T('N') : case _T('n') :
				invSolFinal.bForce = false;
				break;

			default :
				// The force option should be "true", "false" or empty.
				DDX_GridFail(pDX, IDS_INVERSE_275);
				break;
			}
		}

		// check that final soln differs from initial solns
		std::list<InvSol>::const_iterator iter = listInvSol.begin();
		for (; iter != listInvSol.end(); ++iter)
		{
			if (iter->nSol == invSolFinal.nSol)
			{
				DDX_GridText(pDX, IDC_MSHFG_FINALSOLN, 1, 0, strDummy);
				DDX_GridFail(pDX, _T("The list of initial solutions cannot contain the final solution."), _T("Invalid final solution"));
			}
		}

		//
		// change the list of balances for each solution uncertainty that has changed
		//

		// temporarily add final solution to lists in order to update uncertainties
		GetSheet()->m_listInvSol.push_back(GetSheet()->m_invSolFinal);
		listInvSol.push_back(invSolFinal);

		// foreach initial soln
		std::list<InvSol>::iterator iterSolNew = listInvSol.begin();
		for (; iterSolNew != listInvSol.end(); ++iterSolNew)
		{
			// foreach previous initial soln
			std::list<InvSol>::iterator iterSolPrev = GetSheet()->m_listInvSol.begin();
			for (; iterSolPrev != GetSheet()->m_listInvSol.end(); ++iterSolPrev)
			{
				// skip if not same soln
				if (iterSolNew->nSol != iterSolPrev->nSol)
					continue;

				// skip if uncertainty is equal
				if (iterSolNew->dUncertainty == iterSolPrev->dUncertainty)
					continue;

				// if here than the uncertainties are different and need to
				// be updated for each element (except pH)

				// foreach balance element
				std::list<CInvElem>::iterator iterElem = GetSheet()->m_listInvElem.begin();
				for (; iterElem != GetSheet()->m_listInvElem.end(); ++iterElem)
				{
					// skip pH
					if (iterElem->m_strName.CompareNoCase(_T("pH")) == 0) 
						continue;

					// check if previous uncertainty exists
					std::map<int, double>::iterator lookup = iterElem->m_mapSol2Unc.find(iterSolNew->nSol);
					if (lookup != iterElem->m_mapSol2Unc.end())
					{
						// check if previous uncertainty was default
						if (lookup->second == iterSolPrev->dUncertainty)
						{
							// reset uncertainty
							iterElem->m_mapSol2Unc[iterSolNew->nSol] = iterSolNew->dUncertainty;
						}
					}
				}
			}
		}

		// remove final solution from lists
		GetSheet()->m_listInvSol.pop_back();
		listInvSol.pop_back();		

		//
		// all OK
		//
		GetSheet()->m_listInvSol.assign(listInvSol.begin(), listInvSol.end());
		GetSheet()->m_invSolFinal = invSolFinal;
	}
	else
	{
		std::list<InvSol>::const_iterator iter = GetSheet()->m_listInvSol.begin();
		CString strFormat;
		for (; iter != GetSheet()->m_listInvSol.end(); ++iter)
		{
			// solution number
			strFormat.Format(GetSheet()->m_strNumFormat, (*iter).nSol);
			int nRow = m_glDocInit.AddItem(strFormat).m_nRow;
			
			// trim space
			strFormat.TrimLeft();
			DDX_GridText(pDX, IDC_MSHFG_INITSOLNS, nRow, 1, strFormat);

			// uncertainty
			double d = iter->dUncertainty;
			DDX_GridText(pDX, IDC_MSHFG_INITSOLNS, nRow, 2, d);

			// force
			CString strTrue = (iter->bForce) ? _T("true") : _T("");
			DDX_GridText(pDX, IDC_MSHFG_INITSOLNS, nRow, 3, strTrue);
		}

		// turn off checkbox for each initial soln
		for (int i = 0; i < m_clcInitSolns.GetItemCount(); ++i)
		{
			if (m_clcInitSolns.GetCheckBox(i) == BST_CHECKED)
			{
				m_clcFinalSoln.SetCheckBox(i, BST_INDETERMINATE);
			}
		}

		// final solution
		if (GetSheet()->m_invSolFinal.nSol > 0)
		{
			// set checkboxes state 
			// Note: no notifications are sent from DoDataExchange
			strFormat.Format(GetSheet()->m_strNumFormat, GetSheet()->m_invSolFinal.nSol);
			LVFINDINFO findInfo;
			findInfo.flags = LVFI_STRING;
			findInfo.psz = strFormat;

			int n = m_clcFinalSoln.FindItem(&findInfo);
			if (n != -1)
			{
				m_clcFinalSoln.SetCheckBox(n, BST_CHECKED);
			}

			int nItem = m_clcInitSolns.FindItem(&findInfo);
			if (nItem != -1)
			{
				m_clcInitSolns.SetCheckBox(nItem, BST_INDETERMINATE);
			}

			//
			// set grid
			//

			// sol #
			DDX_GridText(pDX, IDC_MSHFG_FINALSOLN, 1, 0, GetSheet()->m_invSolFinal.nSol);

			// uncertainty
			DDX_GridText(pDX, IDC_MSHFG_FINALSOLN, 1, 1, GetSheet()->m_invSolFinal.dUncertainty);

			// force
			CString strTrue = (GetSheet()->m_invSolFinal.bForce) ? _T("true") : _T("");
			DDX_GridText(pDX, IDC_MSHFG_FINALSOLN, 1, 2, strTrue);
		}
	}

}

void CKPInversePg1A::DDX_Range(CDataExchange* pDX)
{
	// get/set m_bRange
	DDX_Check(pDX, IDC_B_RANGE, m_bRange);

	// enable/disable range textbox
	EnableRange(m_bRange);

	// get/set m_dRange
	if (pDX->m_bSaveAndValidate)
	{
		if (m_bRange)
		{
			DDX_Text(pDX, IDC_E_RANGE, m_dRange);
		}
	}
	else
	{
		DDX_Text(pDX, IDC_E_RANGE, m_dRange);
	}
}

void CKPInversePg1A::InitGrids()
{
	ASSERT(m_egInitSolns.GetSafeHwnd() != NULL);
	ASSERT(m_egFinalSoln.GetSafeHwnd() != NULL);

	//
	// init initial solns grid
	//

	// set title column widths
	m_egInitSolns.SetColWidth(1, 0, 900);
	m_egInitSolns.SetColWidth(2, 0, 1100);
	m_egInitSolns.SetColWidth(3, 0, 900);

	// center align the column headers
	m_egInitSolns.SetFillStyle(flexFillRepeat);
	m_egInitSolns.SetRow(0);
	m_egInitSolns.SetCol(0);
	m_egInitSolns.SetColSel(m_egInitSolns.GetCols(0) - 1);
	m_egInitSolns.SetCellAlignment(flexAlignCenterCenter);
	m_egInitSolns.SetFillStyle(flexFillSingle);

	// center align true/false columns
	m_egInitSolns.SetColAlignment(3, flexAlignCenterCenter);

	// set wordwrap
	m_egInitSolns.SetWordWrap(TRUE);
	m_egInitSolns.SetRowHeight(0, (long)(m_egInitSolns.GetRowHeight(1) * 2.0));

	// set grid titles
    m_egInitSolns.SetTextMatrix(0, 1, _T("Solution\r\nNo."));
    m_egInitSolns.SetTextMatrix(0, 2, _T("Global\r\nuncertainty"));
    m_egInitSolns.SetTextMatrix(0, 3, _T("Force"));


	// add row numbers
	m_egInitSolns.AddRowHeaders();

	// set bold titles
	m_egInitSolns.SetRow(0), m_egInitSolns.SetCol(1);
	m_egInitSolns.SetCellFontBold(TRUE);
	m_egInitSolns.SetRow(0), m_egInitSolns.SetCol(2);
	m_egInitSolns.SetCellFontBold(TRUE);
	m_egInitSolns.SetRow(1), m_egInitSolns.SetCol(1);

	//
	// init final soln grid
	//

	// set title column widths
	m_egFinalSoln.SetColWidth(0, 0, 900);
	m_egFinalSoln.SetColWidth(1, 0, 1100);
	m_egFinalSoln.SetColWidth(2, 0, 900);

	// center align the column headers
	m_egFinalSoln.SetFillStyle(flexFillRepeat);
	m_egFinalSoln.SetRow(0);
	m_egFinalSoln.SetCol(0);
	m_egFinalSoln.SetColSel(m_egFinalSoln.GetCols(0) - 1);
	m_egFinalSoln.SetCellAlignment(flexAlignCenterCenter);
	m_egFinalSoln.SetFillStyle(flexFillSingle);

	// center align true/false columns
	m_egFinalSoln.SetColAlignment(2, flexAlignCenterCenter);
	// m_egFinalSoln.SetColAlignment(3, flexAlignCenterCenter);

	// set wordwrap
	m_egFinalSoln.SetWordWrap(TRUE);
	m_egFinalSoln.SetRowHeight(0, (long)(m_egFinalSoln.GetRowHeight(1) * 2.0));

	// set grid titles
    m_egFinalSoln.SetTextMatrix(0, 0, _T("Solution\r\nNo."));
    m_egFinalSoln.SetTextMatrix(0, 1, _T("Global\r\nuncertainty"));
    m_egFinalSoln.SetTextMatrix(0, 2, _T("Force"));

	// add row numbers
	/// m_egFinalSoln.AddRowHeaders();

	// set bold titles
	m_egFinalSoln.SetRow(0), m_egFinalSoln.SetCol(0);
	m_egFinalSoln.SetCellFontBold(TRUE);
	m_egFinalSoln.SetRow(0), m_egFinalSoln.SetCol(1);
	m_egFinalSoln.SetCellFontBold(TRUE);
	m_egFinalSoln.SetRow(1), m_egFinalSoln.SetCol(0);

}

LRESULT CKPInversePg1A::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bDenyEdit = FALSE;	// allow edit	

	switch (nID)
	{
	case IDC_MSHFG_INITSOLNS :
		switch ( pInfo->item.iCol )
		{
		case 1 : // soln number
			// only fill if less than 500 items
			if (GetSheet()->m_setSolutions.size() < 501)
			{
				if (pInfo->item.hWndCombo)
				{
					CComboBox* pCombo = (CComboBox*)CWnd::FromHandle(pInfo->item.hWndCombo);
					ASSERT_KINDOF(CComboBox, pCombo);
					ASSERT(::IsWindow(pInfo->item.hWndCombo));
					if (pCombo)
					{
						pCombo->ResetContent();
						for (int i = 0; i < m_clcInitSolns.GetItemCount(); ++i)
						{
							if (m_clcInitSolns.GetCheckBox(i) == BST_UNCHECKED)
							{
								CString str = m_clcInitSolns.GetItemText(i, 0);
								pCombo->AddString(str);
							}
						}
						pInfo->item.bUseCombo = (pCombo->GetCount() > 0);
					}
				}
				else
				{
					pInfo->item.bUseCombo = (m_clcInitSolns.GetItemCount() > 0);
				}
			}
			break;

		case 3 : // force
			{ 
				if (pInfo->item.hWndCombo)
				{
					CComboBox* pCombo = (CComboBox*)CWnd::FromHandle(pInfo->item.hWndCombo);
					ASSERT_KINDOF(CComboBox, pCombo);	// must be combobox
					if (pCombo)
					{
						// remove prev items
						pCombo->ResetContent();
						pCombo->AddString(_T(""));
						pCombo->AddString(_T("true"));

						// set drop width
						CDC* pDC = pCombo->GetDC();
						CSize size = pDC->GetTextExtent(_T("true"));
						pCombo->SetDroppedWidth(size.cx);
						pInfo->item.bUseCombo = (pCombo->GetCount() > 0);
					}
				}
				else
				{
					pInfo->item.bUseCombo = TRUE;
				}
			}
			break;

		default :
			break;
		}

		break;
	case IDC_MSHFG_FINALSOLN :
		switch ( pInfo->item.iCol )
		{
		case 0 : // soln number
			// only fill if less than 500 items
			if (GetSheet()->m_setSolutions.size() < 501)
			{
				if (pInfo->item.hWndCombo)
				{
					CComboBox* pCombo = (CComboBox*)CWnd::FromHandle(pInfo->item.hWndCombo);
					ASSERT_KINDOF(CComboBox, pCombo);
					ASSERT(::IsWindow(pInfo->item.hWndCombo));
					if (pCombo)
					{
						pCombo->ResetContent();
						for (int i = 0; i < m_clcInitSolns.GetItemCount(); ++i)
						{
							if (m_clcInitSolns.GetCheckBox(i) == BST_UNCHECKED)
							{
								CString str = m_clcInitSolns.GetItemText(i, 0);
								pCombo->AddString(str);
							}
						}
						pInfo->item.bUseCombo = (pCombo->GetCount() > 0);
					}
				}
				else
				{
					for (int i = 0; i < m_clcInitSolns.GetItemCount(); ++i)
					{
						if (m_clcInitSolns.GetCheckBox(i) == BST_UNCHECKED)
						{
							pInfo->item.bUseCombo = TRUE;
							break;
						}
					}
				}
			}
			break;

		case 2 : // force
			if (pInfo->item.hWndCombo)
			{ 
				CComboBox* pCombo = (CComboBox*)CWnd::FromHandle(pInfo->item.hWndCombo);
				ASSERT_KINDOF(CComboBox, pCombo);	// must be combobox
				if (pCombo)
				{
					// remove prev items
					pCombo->ResetContent();
					pCombo->AddString(_T(""));
					pCombo->AddString(_T("true"));

					// set drop width
					CDC* pDC = pCombo->GetDC();
					CSize size = pDC->GetTextExtent(_T("true"));
					pCombo->SetDroppedWidth(size.cx);
					pInfo->item.bUseCombo = (pCombo->GetCount() > 0);
				}
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

LRESULT CKPInversePg1A::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bMakeChange = TRUE;

	// ignore if edit is cancelled
	if ( pInfo->item.pszText == NULL ) return bMakeChange;

	switch (nID)
	{
	case IDC_MSHFG_INITSOLNS :
		{
			// fill defaults
			if (pInfo->item.iCol == 1)
			{
				int n;
				// reformat original string to match 
				// string contained in CheckListCtrl
				CString strOld = m_egInitSolns.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol);
				if (!strOld.IsEmpty() && ::_stscanf(strOld, "%d", &n))
				{
					CString strFormat;
					strFormat.Format(GetSheet()->m_strNumFormat, n);
					m_egInitSolns.SetTextMatrix(pInfo->item.iRow, pInfo->item.iCol, strFormat);
				}

				// check if number was entered
				// if it was, reformat it to match
				// string contained in CheckListCtrl
				if (::_stscanf(pInfo->item.pszText, "%d", &n))
				{
					CString strFormat;
					strFormat.Format(GetSheet()->m_strNumFormat, n);
					free(pInfo->item.pszText);
					pInfo->item.pszText = _tcsdup(strFormat);

					// check if valid entry
					LVFINDINFO findInfo;
					findInfo.flags = LVFI_STRING;
					findInfo.psz = strFormat;
					int nItem = m_clcInitSolns.FindItem(&findInfo);
					if (nItem != -1)
					{
						if (m_clcInitSolns.GetCheckBox(nItem) == BST_INDETERMINATE)
						{
							::AfxMessageBox("Initial solutions cannot be the same as the final solution.");
							return FALSE;
						}
					}

					CString str(pInfo->item.pszText);
					str.TrimLeft();
					if (!str.IsEmpty() && m_egInitSolns.GetTextMatrix(pInfo->item.iRow, 2).IsEmpty())
					{
						m_egInitSolns.SetTextMatrix(pInfo->item.iRow, 2, _T("0.05"));
					}
				}
			}

			return m_glDocInit.OnEndCellEdit(wParam, lParam);
		}
		break;

	case IDC_MSHFG_FINALSOLN :
		if (pInfo->item.iCol == 0)
		{
			// handle directly
			// m_egFinalSoln.SendMessage(EGM_ENDEDITCELLNOW, TRUE, 0);
			ASSERT(pInfo && pInfo->item.pszText);
			CString str = pInfo->item.pszText;
			str.TrimLeft();
			str.TrimRight();

			// this must be done by hand since DDX_GridXXX functions
			// send a EGM_ENDEDITCELLNOW automatically which causes
			// a recursive EndCellEdit within the editgrid

			if (str.IsEmpty())
			{
				// turn off any checkmarks
				for (int i = 0; i < m_clcFinalSoln.GetItemCount(); ++i)
				{
					if (m_clcFinalSoln.GetCheckBox(i) == BST_CHECKED)
					{
						m_clcFinalSoln.SetCheckBox(i, BST_UNCHECKED);
					}
				}
			}
			else
			{
				// set checkmark if in list
				char *ptr;
				long nSol = _tcstol(pInfo->item.pszText, &ptr, 10);
				if (nSol > 0 && (nSol != LONG_MAX && nSol != LONG_MIN) && *ptr == 0)
				{
					CString str;
					str.Format(GetSheet()->m_strNumFormat, nSol);
					LVFINDINFO findInfo;
					findInfo.flags = LVFI_STRING;
					findInfo.psz = str;
					int nItem = m_clcFinalSoln.FindItem(&findInfo);
					if (nItem != -1)
					{
						if (m_clcFinalSoln.GetCheckBox(nItem) == BST_INDETERMINATE)
						{
							::AfxMessageBox("Initial solutions cannot be the same as the final solution.");
							return FALSE;
						}
						else
						{
							m_clcFinalSoln.SetCheckBox(nItem, BST_CHECKED);
						}
					}
				}
				else
				{
					// turn off any checkmarks if number cannot be converted
					for (int i = 0; i < m_clcFinalSoln.GetItemCount(); ++i)
					{
						if (m_clcFinalSoln.GetCheckBox(i) == BST_CHECKED)
						{
							m_clcFinalSoln.SetCheckBox(i, BST_UNCHECKED);
						}
					}
				}
				if (m_egFinalSoln.GetTextMatrix(pInfo->item.iRow, 1).IsEmpty())
				{
					m_egFinalSoln.SetTextMatrix(pInfo->item.iRow, 1, _T("0.05"));
				}
			}
			m_egFinalSoln.SetTextMatrix(pInfo->item.iRow, pInfo->item.iCol, str);
			return FALSE; // already handled
		}
		break;
	}

	return bMakeChange;
}

void CKPInversePg1A::OnKeyDownMshfgInitSolns(short FAR* KeyCode, short Shift) 
{
	// mustn't have any disabled cells
	m_glDocInit.OnKeyDown(KeyCode, Shift);

}

void CKPInversePg1A::OnKeyDownMshfgFinalSoln(short FAR* KeyCode, short Shift) 
{
	UNUSED(Shift);
	switch (*KeyCode)
	{
	case VK_DELETE :
		{
			m_egFinalSoln.SetRedraw(FALSE);
			long nCol = m_egFinalSoln.GetCol();
			long nColSel = m_egFinalSoln.GetColSel();

			// check if col 0 (solution number) is selected
			if ((min(nCol, nColSel) <= 0) && (0 <= max(nCol, nColSel)))
			{
				int n;
				CString strOld = m_egFinalSoln.GetTextMatrix(1, 0);
				if (!strOld.IsEmpty() && ::_stscanf(strOld, "%d", &n))
				{
					CString strFormat;
					strFormat.Format(GetSheet()->m_strNumFormat, n);

					// see if checkmarked
					LVFINDINFO findInfo;
					findInfo.flags = LVFI_STRING;
					findInfo.psz = strFormat;

					int nItem = m_clcFinalSoln.FindItem(&findInfo);
					if (nItem != -1)
					{
						// clear checkmark
						ASSERT(m_clcFinalSoln.GetCheckBox(nItem) == BST_CHECKED);
						m_clcFinalSoln.SetCheckBox(nItem, BST_UNCHECKED);
					}
				}
			}

			// clear selection
			m_egFinalSoln.SetFillStyle(flexFillRepeat);
			m_egFinalSoln.SetText(_T(""));
			m_egFinalSoln.SetFillStyle(flexFillSingle);

			m_egFinalSoln.SetRedraw(TRUE);
		}
		break;
	}
}



void CKPInversePg1A::OnBRange() 
{
	// mimick UpdateData(TRUE) with just the range checkbox
	CDataExchange dx(this, TRUE);
	try
	{
		DDX_Check(&dx, IDC_B_RANGE, m_bRange);
		EnableRange(m_bRange);
	}
	catch (CUserException* e)
	{
		e->ReportError(MB_OK);
		ASSERT(FALSE);
		e->Delete();
	}
}

void CKPInversePg1A::EnableRange(BOOL bEnable)
{
	// enable/disable range textbox
	CWnd* pWnd = GetDlgItem(IDC_E_RANGE);
	ASSERT(pWnd);
	if (pWnd)
	{
		pWnd->EnableWindow(bEnable);
	}
}


void CKPInversePg1A::OnItemchangedClcFinalSoln(NMHDR* pNMHDR, LRESULT* pResult) 
{
#ifdef FORCE_UNCHECK
	static bool bIgnoreChanges = false;

	// enable/disable the corresponding item in init solns
	NMLISTVIEW* pNMListView = (NMLISTVIEW*)pNMHDR;
	if ((pNMListView->uNewState ^ pNMListView->uOldState) & LVIS_STATEIMAGEMASK && !bIgnoreChanges)
	{
		int nItem = pNMListView->iItem;
		int i;
		switch (m_clcFinalSoln.GetCheckBox(nItem))
		{
		case BST_CHECKED :
			ASSERT(m_clcInitSolns.GetCheckBox(nItem) == BST_UNCHECKED);
			m_clcInitSolns.SetCheckBox(nItem, BST_INDETERMINATE);
			for (i = 0; i < m_clcFinalSoln.GetItemCount(); ++i)
			{
				if (i != nItem)
				{
					m_clcFinalSoln.SetCheckBox(i, BST_INDETERMINATE);
				}
			}
			break;
		case BST_UNCHECKED :
			// check prev state
			switch (pNMListView->uOldState)
			{
			case INDEXTOSTATEIMAGEMASK(BST_CHECKED + 1) :
				// caused by the user unchecking a checkbox
				ASSERT(m_clcInitSolns.GetCheckBox(nItem) == BST_INDETERMINATE);
				m_clcInitSolns.SetCheckBox(nItem, BST_UNCHECKED);
				bIgnoreChanges = true;
				for (i = 0; i < m_clcFinalSoln.GetItemCount(); ++i)
				{
					if (i != nItem)
					{
						m_clcFinalSoln.SetCheckBox(i, BST_UNCHECKED);
					}
				}
				bIgnoreChanges = false;
				break;
			case INDEXTOSTATEIMAGEMASK(BST_UNCHECKED + 1) :
				// bad state
				ASSERT(FALSE);
				break;
			case INDEXTOSTATEIMAGEMASK(BST_INDETERMINATE + 1) :
				// caused by a call to SetCheckBox
				// do nothing (already handled)
				ASSERT(m_clcInitSolns.GetCheckBox(nItem) == BST_UNCHECKED);				
				break;
			}
			break;
		case BST_INDETERMINATE :
			// caused by a call to SetCheckBox
			// by either OnItemchangedClcInitSolns or OnItemchangedClcFinalSoln
			// do nothing (already handled)
			break;
		}
	}	
	*pResult = 0;

#else

	static bool bIgnoreChanges = false;

	// enable/disable the corresponding item in init solns
	NMLISTVIEW* pNMListView = (NMLISTVIEW*)pNMHDR;
	if ((pNMListView->uNewState ^ pNMListView->uOldState) & LVIS_STATEIMAGEMASK && !bIgnoreChanges)
	{
		int nItem = pNMListView->iItem;
		int i;
		switch (m_clcFinalSoln.GetCheckBox(nItem))
		{
		case BST_CHECKED :
			ASSERT(m_clcInitSolns.GetCheckBox(nItem) == BST_UNCHECKED);
			m_clcInitSolns.SetCheckBox(nItem, BST_INDETERMINATE);
			bIgnoreChanges = true;
			for (i = 0; i < m_clcFinalSoln.GetItemCount(); ++i)
			{
				if (i != nItem && m_clcFinalSoln.GetCheckBox(i) == BST_CHECKED)
				{
					m_clcFinalSoln.SetCheckBox(i, BST_UNCHECKED);
					ASSERT(m_clcInitSolns.GetCheckBox(nItem) == BST_INDETERMINATE);
					m_clcInitSolns.SetCheckBox(i, BST_UNCHECKED);
				}
			}
			bIgnoreChanges = false;
			m_egFinalSoln.SetTextMatrix(1, 0, m_clcFinalSoln.GetItemText(nItem, 0));
			if (m_egFinalSoln.GetTextMatrix(1, 1).IsEmpty())
			{
				m_egFinalSoln.SetTextMatrix(1, 1, _T("0.05"));
			}
			break;

		case BST_UNCHECKED :
			// check prev state
			switch (pNMListView->uOldState)
			{
			case INDEXTOSTATEIMAGEMASK(BST_CHECKED + 1) :
				// caused by the user unchecking a checkbox
				ASSERT(m_clcInitSolns.GetCheckBox(nItem) == BST_INDETERMINATE);
				m_clcInitSolns.SetCheckBox(nItem, BST_UNCHECKED);
				//{{{
				m_egFinalSoln.SetTextMatrix(1, 0, _T(""));
				//}}}
// COMMENT: {2/1/2001 6:24:53 PM}				bIgnoreChanges = true;
// COMMENT: {2/1/2001 6:24:53 PM}				for (i = 0; i < m_clcFinalSoln.GetItemCount(); ++i)
// COMMENT: {2/1/2001 6:24:53 PM}				{
// COMMENT: {2/1/2001 6:24:53 PM}					if (i != nItem)
// COMMENT: {2/1/2001 6:24:53 PM}					{
// COMMENT: {2/1/2001 6:24:53 PM}						m_clcFinalSoln.SetCheckBox(i, BST_UNCHECKED);
// COMMENT: {2/1/2001 6:24:53 PM}					}
// COMMENT: {2/1/2001 6:24:53 PM}				}
// COMMENT: {2/1/2001 6:24:53 PM}				bIgnoreChanges = false;
				break;

			case INDEXTOSTATEIMAGEMASK(BST_UNCHECKED + 1) :
				// bad state
				ASSERT(FALSE);
				break;

			case INDEXTOSTATEIMAGEMASK(BST_INDETERMINATE + 1) :
				// caused by a call to SetCheckBox
				// do nothing (already handled)
				ASSERT(m_clcInitSolns.GetCheckBox(nItem) == BST_UNCHECKED);				
				break;

			}
			break;
		case BST_INDETERMINATE :
			// caused by a call to SetCheckBox
			// by either OnItemchangedClcInitSolns or OnItemchangedClcFinalSoln
			// do nothing (already handled)
			break;
		default :
			ASSERT(FALSE);
		}
	}	
	*pResult = 0;
#endif
}

void CKPInversePg1A::OnItemchangedClcInitSolns(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// enable/disable the corresponding item in final soln

	NMLISTVIEW* pNMListView = (NMLISTVIEW*)pNMHDR;
	if ((pNMListView->uNewState ^ pNMListView->uOldState) & LVIS_STATEIMAGEMASK)
	{
		int nItem = pNMListView->iItem;

		long nRow = -1;
		switch (m_clcInitSolns.GetCheckBox(nItem))
		{
		case BST_CHECKED :
			// should only occur from user action
			// do default
			nRow = m_glDocInit.OnItemChanged(pNMHDR, pResult);
			ASSERT(m_clcFinalSoln.GetCheckBox(nItem) == BST_UNCHECKED ||
				m_clcFinalSoln.GetCheckBox(nItem) == BST_INDETERMINATE);
			m_clcFinalSoln.SetCheckBox(nItem, BST_INDETERMINATE);
			break;
		case BST_UNCHECKED :
			// check prev state
			switch (pNMListView->uOldState)
			{
			case INDEXTOSTATEIMAGEMASK(BST_CHECKED + 1) :
				// caused by the user unchecking a checkbox
				// do default
				nRow = m_glDocInit.OnItemChanged(pNMHDR, pResult);
				ASSERT(m_clcFinalSoln.GetCheckBox(nItem) == BST_INDETERMINATE);
				m_clcFinalSoln.SetCheckBox(nItem, BST_UNCHECKED);
				break;
			case INDEXTOSTATEIMAGEMASK(BST_UNCHECKED + 1) :
				// bad state
				ASSERT(FALSE);
				break;
			case INDEXTOSTATEIMAGEMASK(BST_INDETERMINATE + 1) :
				// caused by a call to SetCheckBox
				// do nothing (already handled)
				ASSERT(m_clcFinalSoln.GetCheckBox(nItem) == BST_UNCHECKED);
				break;
			}
			break;
		case BST_INDETERMINATE :
			// caused by a call to SetCheckBox
			// do nothing (already handled)
			ASSERT(m_clcFinalSoln.GetCheckBox(nItem) == BST_CHECKED);
			break;
		}

		if (nRow != -1 && m_egInitSolns.GetTextMatrix(nRow, 2).IsEmpty())
		{
			m_egInitSolns.SetTextMatrix(nRow, 2, _T("0.05"));
		}
	}
}
void CKPInversePg1A::OnEnterCellMshfgNumDesc() 
{
	CString strRes;

	switch (m_egNumDesc.GetRow())
	{
	case 0 :
		strRes.LoadString(IDS_INVERSE_NUM_264);
		break;
	case 2 :
		// Optional comment that describes the %1.
		AfxFormatString1(strRes, IDS_DESC_STR_142, _T("inverse-modeling calculation") ); 
		break;
	}
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPInversePg1A::OnEnterCellMshfgInitsolns() 
{
	CString strRes;

	switch (m_egInitSolns.GetCol())
	{
	case 1 :
		strRes.LoadString(IDS_INVERSE_265);
		break;

	case 2 :
		strRes.LoadString(IDS_INVERSE_266);
		break;

	case 3 :
		strRes.LoadString(IDS_INVERSE_268);
		break;

	default :
		ASSERT(FALSE);
		break;
	}
	m_eInputDesc.SetWindowText(strRes);
}

void CKPInversePg1A::OnEnterCellMshfgFinalsoln() 
{
	CString strRes;
	switch (m_egFinalSoln.GetCol())
	{
	case 0 :
		strRes.LoadString(IDS_INVERSE_265);
		break;

	case 1 :
		strRes.LoadString(IDS_INVERSE_266);
		break;

	case 2 :
		strRes.LoadString(IDS_INVERSE_268);
		break;

	default :
		ASSERT(FALSE);
		break;
	}
	m_eInputDesc.SetWindowText(strRes);
}

void CKPInversePg1A::OnSetfocusETol() 
{
	CString strRes;
	strRes.LoadString(IDS_INVERSE_279);
	m_eInputDesc.SetWindowText(strRes);
}

void CKPInversePg1A::OnSetfocusBRange() 
{
	CString strRes;
	strRes.LoadString(IDS_INVERSE_273);
	m_eInputDesc.SetWindowText(strRes);
}

void CKPInversePg1A::OnSetfocusBMineralWater() 
{
	CString strRes;
	strRes.LoadString(IDS_INVERSE_280);
	m_eInputDesc.SetWindowText(strRes);
}

void CKPInversePg1A::OnSetfocusBMinimal() 
{
	CString strRes;
	strRes.LoadString(IDS_INVERSE_281);
	m_eInputDesc.SetWindowText(strRes);
}

LRESULT CKPInversePg1A::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
{
	UINT nID = wParam;
	UNUSED_ALWAYS(lParam);

	switch (nID)
	{
	case IDC_MSHFG_NUM_DESC :
		OnEnterCellMshfgNumDesc();
		break;

	case IDC_MSHFG_INITSOLNS :
		OnEnterCellMshfgInitsolns();
		break;

	case IDC_MSHFG_FINALSOLN :
		OnEnterCellMshfgFinalsoln();
		break;

	default :
		ASSERT(FALSE);
		break;
	}
	return 0;
}

void CKPInversePg1A::OnSetfocusClcInitsolns(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNUSED(pNMHDR);
	CString strRes;

	AfxFormatString1(strRes, IDS_CL_167, _T("solution number") ); 

	m_eInputDesc.SetWindowText(strRes);
	
	// set default selection
	// this gives user feedback when the list recieves the focus
	if (m_clcInitSolns.GetItemCount() && m_clcInitSolns.GetFirstSelectedItemPosition() == NULL)
	{
		m_clcInitSolns.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}
	
	*pResult = 0;
}

void CKPInversePg1A::OnSetfocusClcFinalsoln(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNUSED(pNMHDR);
	CString strRes;

	AfxFormatString1(strRes, IDS_CL_167, _T("solution number") ); 

	m_eInputDesc.SetWindowText(strRes);
	
	// set default selection
	// this gives user feedback when the list recieves the focus
	if (m_clcFinalSoln.GetItemCount() && m_clcFinalSoln.GetFirstSelectedItemPosition() == NULL)
	{
		m_clcFinalSoln.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}
	
	*pResult = 0;
}

//}}CKPInversePg1A

//{{CKPInversePg1B
/////////////////////////////////////////////////////////////////////////////

// CKPInversePg1B property page
const long CKPInversePg1B::nNameCol       = 1;
const long CKPInversePg1B::nDissOrPrecCol = 2;
const long CKPInversePg1B::nForceCol      = 3;

CKPInversePg1B::CKPInversePg1B() : baseKPInversePg1B(CKPInversePg1B::IDD)
, m_glDoc(&m_egPhaseGrid, &m_clcPhases, 1)
{
	//{{AFX_DATA_INIT(CKPInversePg1B)
	//}}AFX_DATA_INIT
}

CKPInversePg1B::~CKPInversePg1B()
{
}

void CKPInversePg1B::DoDataExchange(CDataExchange* pDX)
{
	baseKPInversePg1B::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CKPInversePg1B)
	DDX_Control(pDX, IDC_E_DESC_INPUT, m_eInputDesc);
	DDX_Control(pDX, IDC_CL_PHASES, m_clcPhases);
	DDX_Control(pDX, IDC_MSHFG_PHASEGRID, m_egPhaseGrid);
	//}}AFX_DATA_MAP
	if (m_bFirstSetActive)
	{
		CUtil::InsertPhasesExchangeSpecies(&m_clcPhases, GetDatabase());
	}
	DDX_PhaseGrid(pDX);
	if (pDX->m_bSaveAndValidate)
	{
		// as last step remove all entries from m_glDoc;
		m_glDoc.DeleteAllRows();
	}
}

BEGIN_MESSAGE_MAP(CKPInversePg1B, baseKPInversePg1B)
	//{{AFX_MSG_MAP(CKPInversePg1B)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CL_PHASES, OnItemchangedClPhases)
	ON_NOTIFY(NM_SETFOCUS, IDC_CL_PHASES, OnSetfocusClPhases)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
	// custom edit grid notifications
	ON_MESSAGE(EGN_BEGINCELLEDIT, OnBeginCellEdit)
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
	ON_MESSAGE(EGN_SETFOCUS, OnSetfocusGrid)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CKPInversePg1B, baseKPInversePg1B)
    //{{AFX_EVENTSINK_MAP(CKPInversePg1B)
	ON_EVENT(CKPInversePg1B, IDC_MSHFG_PHASEGRID, -602 /* KeyDown */, OnKeyDownMshfgPhasegrid, VTS_PI2 VTS_I2)
	ON_EVENT(CKPInversePg1B, IDC_MSHFG_PHASEGRID, 71 /* EnterCell */, OnEnterCellMshfgPhasegrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKPInversePg1B message handlers

BOOL CKPInversePg1B::OnInitDialog() 
{
	baseKPInversePg1B::OnInitDialog();	

	CreateRoot(VERTICAL, 5, 6)
		<< (pane(HORIZONTAL, ABSOLUTE_VERT)
			<< item(IDC_S_DEF_PHASES, RELATIVE_HORZ, 38)
			<< item(IDC_S_PHASES, RELATIVE_HORZ, 62)
			)
		<< (pane(HORIZONTAL, GREEDY)
			<< item(IDC_CL_PHASES, RELATIVE_HORZ, 38)
			<< item(IDC_MSHFG_PHASEGRID, RELATIVE_HORZ, 62)
			)
			/*
		<< (pane(HORIZONTAL, ABSOLUTE_VERT)
			<< item(IDC_B_WATER, NORESIZE)
			<< item(IDC_E_WATER, NORESIZE)
			<< item(paneNull, RELATIVE_HORZ, 38)
			<< item(IDC_B_FREEZE, NORESIZE)
			<< item(paneNull, RELATIVE_HORZ, 62)
			<< itemSpaceBetween(HORIZONTAL, IDC_B_FREEZE, IDC_B_RESET)
			<< itemFixed(HORIZONTAL, 40)
			<< item(IDC_B_RESET, NORESIZE)
			)
			*/
		<< (paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, GREEDY, nDefaultBorder, 10, 10)
			<< item(IDC_E_DESC_INPUT, ABSOLUTE_VERT)
			);
	UpdateLayout();
	//}}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKPInversePg1B::DDX_PhaseGrid(CDataExchange* pDX)
{
	CDelayUpdate delay(&m_egPhaseGrid);

	if (m_bFirstSetActive)
	{
		InitPhaseGrid();
	}

	if (pDX->m_bSaveAndValidate)
	{
		std::list<CInvPhase> listInvPhase;
		for (long nRow = m_egPhaseGrid.GetFixedRows(); nRow < m_egPhaseGrid.GetRows(); ++nRow)
		{
			CInvPhase invPhase;

			// phase name
			DDX_GridText(pDX, IDC_MSHFG_PHASEGRID, nRow, nNameCol, invPhase.m_strName);
			if (invPhase.m_strName.IsEmpty())
				continue;

			// constraint
			CString strDummy;
			DDX_GridText(pDX, IDC_MSHFG_PHASEGRID, nRow, nDissOrPrecCol, strDummy);
			if (!strDummy.IsEmpty())
			{
				switch (strDummy[0])
				{
				case _T('P') : case _T('p') :
					invPhase.m_nConstraint = CInvPhase::IPC_PRECIPITATE;
					break;

				case _T('D') : case _T('d') :
					invPhase.m_nConstraint = CInvPhase::IPC_DISSOLVE;
					break;

				default :
					// Unknown option for inverse modeling phase.
					DDX_GridFail(pDX, IDS_INVERSE_278);
					break;
				}
			}

			// force
			DDX_GridText(pDX, IDC_MSHFG_PHASEGRID, nRow, nForceCol, strDummy);
			strDummy.TrimLeft();
			if (!strDummy.IsEmpty())
			{
				switch (strDummy[0])
				{
				case _T('T') : case _T('t') : case _T('Y') : case _T('y') :
					invPhase.m_bForce = true;
					break;

				case _T('F') : case _T('f') : case _T('N') : case _T('n') :
					invPhase.m_bForce = false;
					break;

				default :
					// The force option should be "true", "false" or empty.
					DDX_GridFail(pDX, IDS_INVERSE_275);
					break;
				}
			}
			listInvPhase.push_back(invPhase);
		}

		//
		// swap map of isotopes for each phase

		// foreach phase
		std::list<CInvPhase>::iterator iterInvPhase = listInvPhase.begin();
		for (; iterInvPhase != listInvPhase.end(); ++iterInvPhase)
		{
			// foreach previous phase
			std::list<CInvPhase>::iterator iterInvPhasePrev = m_listInvPhase.begin();
			for (; iterInvPhasePrev != m_listInvPhase.end(); ++iterInvPhasePrev)
			{
				if (iterInvPhasePrev->m_strName.CompareNoCase(iterInvPhase->m_strName) == 0)
				{
					iterInvPhase->m_mapIsotopes.swap(iterInvPhasePrev->m_mapIsotopes);
					break;
				}
			}
		}

		// if here listInvPhase is valid and can be assigned to the member variable
		m_listInvPhase.assign(listInvPhase.begin(), listInvPhase.end());
	}
	else
	{
		std::list<CInvPhase>::iterator iter = m_listInvPhase.begin();
		for (; iter != m_listInvPhase.end(); ++iter)
		{
			long nRow = m_glDoc.AddItem(iter->m_strName).m_nRow;

			// phase name
			DDX_GridText(pDX, IDC_MSHFG_PHASEGRID, nRow, nNameCol, iter->m_strName);

			// constraint
			switch (iter->m_nConstraint)
			{
			case CInvPhase::IPC_PRECIPITATE :
				m_egPhaseGrid.SetTextMatrix(nRow, nDissOrPrecCol, _T("precip"));
				break;

			case CInvPhase::IPC_DISSOLVE :
				m_egPhaseGrid.SetTextMatrix(nRow, nDissOrPrecCol, _T("dissolve"));
				break;

			case CInvPhase::IPC_EITHER :
				m_egPhaseGrid.SetTextMatrix(nRow, nDissOrPrecCol, _T(""));
				break;

			default :
				ASSERT(FALSE);
				m_egPhaseGrid.SetTextMatrix(nRow, nDissOrPrecCol, _T(""));
				break;
			}

			// force
			if (iter->m_bForce)
			{
				m_egPhaseGrid.SetTextMatrix(nRow, nForceCol, _T("true"));
			}
			else
			{
				m_egPhaseGrid.SetTextMatrix(nRow, nForceCol, _T(""));
			}
		}
	}
}

void CKPInversePg1B::InitPhaseGrid()
{
	ASSERT(m_egPhaseGrid.GetSafeHwnd() != NULL);

	// set title column widths
	m_egPhaseGrid.SetColWidth(nNameCol, 0, 1000);
	m_egPhaseGrid.SetColWidth(nDissOrPrecCol, 0, 1100);
	m_egPhaseGrid.SetColWidth(nForceCol, 0, 900);

	// add row numbers
	m_egPhaseGrid.AddRowHeaders();

	// center align the column headers
	m_egPhaseGrid.SetFillStyle(flexFillRepeat);
	m_egPhaseGrid.SetRow(0);
	m_egPhaseGrid.SetCol(0);
	m_egPhaseGrid.SetColSel(m_egPhaseGrid.GetCols(0) - 1);
	m_egPhaseGrid.SetCellAlignment(flexAlignCenterCenter);
	m_egPhaseGrid.SetFillStyle(flexFillSingle);

	// center align true/false columns
	m_egPhaseGrid.SetColAlignment(2, flexAlignCenterCenter);

	// set wordwrap
	m_egPhaseGrid.SetWordWrap(TRUE);
	m_egPhaseGrid.SetRowHeight(0, (long)(m_egPhaseGrid.GetRowHeight(1) * 2.5));

	// set grid titles
    m_egPhaseGrid.SetTextMatrix(0, nNameCol, _T("Name"));
    m_egPhaseGrid.SetTextMatrix(0, nDissOrPrecCol, _T("Dissolve/\r\nprecipitate"));
    m_egPhaseGrid.SetTextMatrix(0, nForceCol, _T("Force"));

	// set bold titles
	m_egPhaseGrid.SetRow(0), m_egPhaseGrid.SetCol(nNameCol);
	m_egPhaseGrid.SetCellFontBold(TRUE);

	// set initial cell
	m_egPhaseGrid.SetRow(1), m_egPhaseGrid.SetCol(nNameCol);
}

void CKPInversePg1B::OnItemchangedClPhases(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_glDoc.OnItemChanged(pNMHDR, pResult);
}

void CKPInversePg1B::OnKeyDownMshfgPhasegrid(short FAR* KeyCode, short Shift) 
{
	m_glDoc.OnKeyDown(KeyCode, Shift);	
}

void CKPInversePg1B::OnEnterCellMshfgPhasegrid() 
{
	CString strRes;

	switch (m_egPhaseGrid.GetCol())
	{
	case nNameCol:
		strRes.LoadString(IDS_INVERSE_269);
		break;
	case nDissOrPrecCol:
		strRes.LoadString(IDS_INVERSE_270);
		break;
	case nForceCol:
		strRes.LoadString(IDS_INVERSE_271);
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	m_eInputDesc.SetWindowText(strRes);
}

LRESULT CKPInversePg1B::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
{
	UINT nID = wParam;
	UNUSED_ALWAYS(lParam);

	switch (nID)
	{
	case IDC_MSHFG_PHASEGRID :
		OnEnterCellMshfgPhasegrid();
		break;

	default :
		ASSERT(FALSE);
		break;
	}
	return 0;
}

void CKPInversePg1B::OnSetfocusClPhases(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNUSED(pNMHDR);
	CString strRes;

	// Select from the list the %1 in which to include in this keyword block.  (The space bar toggles the state of the check box).
	AfxFormatString1(strRes, IDS_CL_167, _T("phase(s)") ); 

	m_eInputDesc.SetWindowText(strRes);
	
	// set default selection
	// this gives user feedback when the list recieves the focus
	if (m_clcPhases.GetItemCount() && m_clcPhases.GetFirstSelectedItemPosition() == NULL)
	{
		m_clcPhases.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}
	
	*pResult = 0;
}

LRESULT CKPInversePg1B::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bDenyEdit = FALSE;	// allow edit	

	switch (nID)
	{
	case IDC_MSHFG_PHASEGRID :
		switch ( pInfo->item.iCol )
		{
		case nNameCol :
			pInfo->item.bUseCombo = (CUtil::InsertPhasesExchangeSpecies(pInfo->item.hWndCombo, GetDatabase()) > 0);
			break;
		case nDissOrPrecCol :
			if (pInfo->item.hWndCombo)
			{
				CComboBox* pCombo = (CComboBox*)CWnd::FromHandle(pInfo->item.hWndCombo);
				ASSERT_KINDOF(CComboBox, pCombo);	// must be combobox
				if (pCombo)
				{
					// remove prev items
					pCombo->ResetContent();
					pCombo->AddString(_T(""));
					pCombo->AddString(_T("dissolve"));
					pCombo->AddString(_T("precip"));

					// set drop width
					CDC* pDC = pCombo->GetDC();
					CSize size = pDC->GetTextExtent(_T("dissolve"));
					pCombo->SetDroppedWidth(size.cx);
					pInfo->item.bUseCombo = (pCombo->GetCount() > 0);
				}
			}
			else
			{
				pInfo->item.bUseCombo = TRUE;
			}
			break;

		case nForceCol :
			if (pInfo->item.hWndCombo)
			{
				CComboBox* pCombo = (CComboBox*)CWnd::FromHandle(pInfo->item.hWndCombo);
				ASSERT_KINDOF(CComboBox, pCombo);	// must be combobox
				if (pCombo)
				{
					// remove prev items
					pCombo->ResetContent();
					pCombo->AddString(_T(""));
					pCombo->AddString(_T("true"));

					// set drop width
					CDC* pDC = pCombo->GetDC();
					CSize size = pDC->GetTextExtent(_T("true"));
					pCombo->SetDroppedWidth(size.cx);
					pInfo->item.bUseCombo = (pCombo->GetCount() > 0);
				}
			}
			else
			{
				pInfo->item.bUseCombo = TRUE;
			}
			break;

		default :
			break;

		}
		break;

	}

	return bDenyEdit;
}

LRESULT CKPInversePg1B::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
{
	return m_glDoc.OnEndCellEdit(wParam, lParam);
}

/////////////////////////////////////////////////////////////////////////////
// CKPInversePg2 property page

CKPInversePg2::CKPInversePg2() : baseKPInversePg2(CKPInversePg2::IDD)
, m_glDoc(&m_egBalance, &m_clcElements, 0)
{
	//{{AFX_DATA_INIT(CKPInversePg2)
	m_dWaterUncert = 0.0;
	m_bWaterUncert = FALSE;
	m_bFrozen = FALSE;
	//}}AFX_DATA_INIT
	m_glDoc.SetUseRowHeaders(false);
}

CKPInversePg2::~CKPInversePg2()
{
}

void CKPInversePg2::DoDataExchange(CDataExchange* pDX)
{
	baseKPInversePg2::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKPInversePg2)
	DDX_Control(pDX, IDC_E_DESC_INPUT, m_eInputDesc);
	DDX_Control(pDX, IDC_CL_ELEMENTS, m_clcElements);
	DDX_Control(pDX, IDC_MSHFG_BALANCE, m_egBalance);
	DDX_Text(pDX, IDC_E_WATER, m_dWaterUncert);
	DDX_Check(pDX, IDC_B_WATER, m_bWaterUncert);
	DDX_Check(pDX, IDC_B_FREEZE, m_bFrozen);
	//}}AFX_DATA_MAP

	EnableWater(m_bWaterUncert);

	// set number of cols
	// => Element Col (1) + Initial Solutions + Final Solution (1)
	m_egBalance.SetCols(0, 1 + (long)GetSheet()->m_listInvSol.size() + 1);

	if (m_bFrozen)
	{
		m_egBalance.SetFixedCols(1);
		m_egBalance.SetColAlignmentFixed(0, flexAlignCenterCenter);
	}
	else
	{
		m_egBalance.SetFixedCols(0);
		m_egBalance.SetColAlignment(0, flexAlignCenterCenter);
	}


	if (m_bFirstSetActive)
	{
		CUtil::InsertAqElements(&m_clcElements, GetDatabase());
		int nItem;
		nItem = m_clcElements.InsertItem(0, _T("pH"));
		if (nItem != -1)
		{
			// add column items
			m_clcElements.SetItemText(nItem, 1, _T("pH"));
			m_clcElements.SetItemText(nItem, 2, _T("N/A"));
			m_clcElements.SetItemText(nItem, 3, _T("N/A"));
		}

		// don't show col or row selections
		m_egBalance.SetShowColSelection(false);
		m_egBalance.SetShowRowSelection(false);
	}

	// center align the column headers / and set bold
	m_egBalance.SetFillStyle(flexFillRepeat);
	m_egBalance.SetRow(0);
	m_egBalance.SetCol(0);
	m_egBalance.SetColSel(m_egBalance.GetCols(0) - 1);
	m_egBalance.SetCellAlignment(flexAlignCenterCenter);
	m_egBalance.SetCellFontBold(TRUE);
	m_egBalance.SetFillStyle(flexFillSingle);


	// set col headers
	m_egBalance.SetTextMatrix(0, 0, _T("Element"));
	
	// Add initial solns
	ASSERT(GetSheet()->m_listInvSol.size() > 0);
	CString str;
	std::list<InvSol>::iterator iter = GetSheet()->m_listInvSol.begin();
	long nCol = 1;
	for (; iter != GetSheet()->m_listInvSol.end(); ++iter, ++nCol)
	{
		str.Format(_T("%ld"), iter->nSol);
		m_egBalance.SetTextMatrix(0, nCol, str);
	}

	// Add final soln
	str.Format(_T("%ld (final)"), GetSheet()->m_invSolFinal.nSol);
	m_egBalance.SetTextMatrix(0, nCol, str);

	if (pDX->m_bSaveAndValidate)
	{
		CDelayUpdate delay(&m_egBalance);

		BOOL bFrozenSave = m_bFrozen;
		if (m_bFrozen)
		{
			// turn off frozen elements
			CWnd* pWnd = GetDlgItem(IDC_B_FREEZE);
			pWnd->SendMessage(BM_SETCHECK, (WPARAM)0, 0L);
			OnBFreeze();
		}
		ASSERT(!m_bFrozen);

		// temporary holder for list of elements
		std::list<CInvElem> listInvElem;

		// struct used find list items
		LVFINDINFO info;
		info.flags = LVFI_STRING;

		for (long nRow = m_egBalance.GetFixedRows(); nRow < m_egBalance.GetRows(); ++nRow)
		{
			// element name
			CString strDummy;
			DDX_GridText(pDX, IDC_MSHFG_BALANCE, nRow, 0, strDummy);
			strDummy.TrimLeft();
			strDummy.TrimRight();
			if (strDummy.IsEmpty())
				continue;

			CInvElem elem;

			// Find proper name in list
			info.psz = strDummy.GetBuffer(strDummy.GetLength() + 1);
			int nItem = m_clcElements.FindItem(&info, -1);
			if (nItem != -1)
			{
				elem.m_strName = m_clcElements.GetItemText(nItem, 1);
			}
			else
			{
				elem.m_strName = info.psz;
				TRACE(_T("WARNING: %s: not in list \n"), elem.m_strName);
			}
			strDummy.ReleaseBuffer();

			// TODO DAVID: Can isotopes be entered here (ie [Ca13] -balances option)
			if ((::_istupper((_TINT)elem.m_strName[0]) == 0) && elem.m_strName.CompareNoCase(_T("pH")) != 0) // TODO DAVID
			{
				DDX_GridFail(pDX, _T("Expected element to begin with a capital letter (exception: pH)."));				
			}

			//
			// uncertainties
			//
			double dValue = 0.0;
			for (long nCol = 1; nCol < m_egBalance.GetCols(0); ++nCol)
			{
				CString strSol = m_egBalance.GetTextMatrix(0, nCol);
				int nSol = atoi(strSol);
				ASSERT(nSol >= 0);

				if (nCol == 1)
				{
					// at least one uncertainty must be set
					DDX_GridText(pDX, IDC_MSHFG_BALANCE, nRow, nCol, dValue);
				}
				else
				{
					CString strDummy;
					DDX_GridText(pDX, IDC_MSHFG_BALANCE, nRow, nCol, strDummy);

					if (strDummy.IsEmpty())
					{
						// do nothing use previous
					}
					else
					{
						DDX_GridText(pDX, IDC_MSHFG_BALANCE, nRow, nCol, dValue);
					}
				}
				// handle special case pH
				if (elem.m_strName.CompareNoCase(_T("pH")) == 0)
				{
					elem.m_mapSol2Unc[nSol] = fabs(dValue);
				}
				else
				{
					elem.m_mapSol2Unc[nSol] = dValue;
				}
			}
			listInvElem.push_back(elem);
		}
		// if here listInvElem is valid and can be assigned to the member variable
		m_bFrozen = bFrozenSave;
		GetSheet()->m_listInvElem.assign(listInvElem.begin(), listInvElem.end());

		// as last step remove all entries from m_glDoc;
		ASSERT(m_egBalance.GetFixedCols() == 0);
		m_glDoc.DeleteAllRows();
	}
	else
	{
		ASSERT(m_glDoc.GetCount() == 0);

		std::list<CInvElem>::iterator iterElem = GetSheet()->m_listInvElem.begin();
		for (; iterElem != GetSheet()->m_listInvElem.end(); ++iterElem)
		{
			// CInvElem elem(*iterElem);

			// element name
			CGridListItem item; //  = m_glDoc.AddItem(iterElem->m_strName);
			
			// element name
			if (iterElem->m_strName.Find(_T("(")) == -1)
			{
				item = m_glDoc.AddItem(iterElem->m_strName);
			}
			else
			{
				item = m_glDoc.AddSubItem(1, iterElem->m_strName);
			}
			if (item.m_nRow == -1) continue;	// invalid

			//
			// uncertainties
			//

			// foreach init soln
			std::list<InvSol>::iterator iterSol = GetSheet()->m_listInvSol.begin();
			long nCol = 1;
			for (; iterSol != GetSheet()->m_listInvSol.end(); ++iterSol, ++nCol)
			{
				std::map<int, double>::iterator iterUnc = iterElem->m_mapSol2Unc.find(iterSol->nSol);
				if (iterUnc != iterElem->m_mapSol2Unc.end())
				{
					DDX_GridText(pDX, IDC_MSHFG_BALANCE, item.m_nRow, nCol, iterUnc->second);
				}
				else
				{
					if (iterElem->m_strName.CompareNoCase(_T("pH")) == 0)
					{
						double d = 0.05;
						DDX_GridText(pDX, IDC_MSHFG_BALANCE, item.m_nRow, nCol, d);
					}
					else
					{
						DDX_GridText(pDX, IDC_MSHFG_BALANCE, item.m_nRow, nCol, iterSol->dUncertainty);
					}
				}
			}

			// final soln
			std::map<int, double>::iterator iterUnc = iterElem->m_mapSol2Unc.find(GetSheet()->m_invSolFinal.nSol);
			if (iterUnc != iterElem->m_mapSol2Unc.end())
			{
				DDX_GridText(pDX, IDC_MSHFG_BALANCE, item.m_nRow, nCol, iterUnc->second);
			}
			else
			{
				if (iterElem->m_strName.CompareNoCase(_T("pH")) == 0)
				{
					double d = 0.05;
					DDX_GridText(pDX, IDC_MSHFG_BALANCE, item.m_nRow, nCol, d);
				}
				else
				{
					DDX_GridText(pDX, IDC_MSHFG_BALANCE, item.m_nRow, nCol, GetSheet()->m_invSolFinal.dUncertainty);
				}
			}			
		}
	}
}


BEGIN_MESSAGE_MAP(CKPInversePg2, baseKPInversePg2)
	//{{AFX_MSG_MAP(CKPInversePg2)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CL_ELEMENTS, OnItemchangedClElements)
	ON_BN_CLICKED(IDC_B_FREEZE, OnBFreeze)
	ON_BN_CLICKED(IDC_B_WATER, OnBWater)
	ON_BN_CLICKED(IDC_B_RESET, OnBReset)
	ON_NOTIFY(NM_SETFOCUS, IDC_CL_ELEMENTS, OnSetfocusClElements)
	ON_EN_SETFOCUS(IDC_E_WATER, OnSetfocusEWater)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
	// custom BN_SETFOCUS notifications
	ON_BN_SETFOCUS(IDC_B_WATER, OnSetfocusBWater)
	ON_BN_SETFOCUS(IDC_B_FREEZE, OnSetfocusBFreeze)
	ON_BN_SETFOCUS(IDC_B_RESET, OnSetfocusBReset)
	// custom edit grid notifications
	ON_MESSAGE(EGN_BEGINCELLEDIT, OnBeginCellEdit)
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
	ON_MESSAGE(EGN_SETFOCUS, OnSetfocusGrid)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CKPInversePg3 property page

CKPInversePg3::CKPInversePg3() : baseKPInversePg3(CKPInversePg3::IDD)
, m_glDocSol(&m_egSol, &m_clcIsoSol, 0)
, m_glDocPhase(&m_egPhase, &m_clcIsoPhase, 0)
{
	//{{AFX_DATA_INIT(CKPInversePg3)
	m_bFreezePhase = FALSE;
	m_bFreezeSol = FALSE;
	//}}AFX_DATA_INIT

	// load default uncertainties
	m_mapIso2DefaultUnc[_T("13C")]     = 1.0;
	m_mapIso2DefaultUnc[_T("13C(4)")]  = 1.0;
	m_mapIso2DefaultUnc[_T("13C(-4)")] = 5.0;
	m_mapIso2DefaultUnc[_T("34S")]     = 1.0;
	m_mapIso2DefaultUnc[_T("34S(6)")]  = 1.0;
	m_mapIso2DefaultUnc[_T("34S(-2)")] = 5.0;
	m_mapIso2DefaultUnc[_T("2H")]      = 1.0;
	m_mapIso2DefaultUnc[_T("18O")]     = 0.1;
	m_mapIso2DefaultUnc[_T("87Sr")]    = 0.01;
}

CKPInversePg3::~CKPInversePg3()
{
}

void CKPInversePg3::DoDataExchange(CDataExchange* pDX)
{
	baseKPInversePg3::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKPInversePg3)
	DDX_Control(pDX, IDC_E_DESC_INPUT, m_eInputDesc);
	DDX_Control(pDX, IDC_CL_ISO_PHASE, m_clcIsoPhase);
	DDX_Control(pDX, IDC_CL_ISO_SOL, m_clcIsoSol);
	DDX_Control(pDX, IDC_MSHFG_SOL, m_egSol);
	DDX_Control(pDX, IDC_MSHFG_PHASE, m_egPhase);
	DDX_Check(pDX, IDC_B_FREEZE_PHASE, m_bFreezePhase);
	DDX_Check(pDX, IDC_B_FREEZE_SOL, m_bFreezeSol);
	//}}AFX_DATA_MAP

	if (!pDX->m_bSaveAndValidate)
	{

		// freeze
		if (m_bFreezeSol)
		{
			m_egSol.SetFixedCols(1);
			m_egSol.SetColAlignmentFixed(0, flexAlignCenterCenter);
		}
		else
		{
			m_egSol.SetFixedCols(0);
			m_egSol.SetColAlignment(0, flexAlignCenterCenter);
		}
	}

	if (m_bFirstSetActive)
	{
		InitSoln();
		InitPhases();
	}
	

	DDX_Phases(pDX);
	DDX_Soln(pDX);

	ASSERT( m_bFreezePhase ? m_egPhase.GetFixedCols() == 1 : m_egPhase.GetFixedCols() == 0);
	ASSERT( m_bFreezeSol ? m_egSol.GetFixedCols() == 1 : m_egSol.GetFixedCols() == 0);

	if (pDX->m_bSaveAndValidate)
	{
		m_glDocPhase.DeleteAllRows();
		// as last step remove all entries from m_glDocSol;
	}
}

void CKPInversePg3::DDX_Phases(CDataExchange* pDX)
{
	if (pDX->m_bSaveAndValidate)
	{
		CDelayUpdate delay(&m_egPhase);

		BOOL bFreezePhaseSave = m_bFreezePhase;
		if (m_bFreezePhase)
		{
			// turn off frozen isotopes
			CWnd* pWnd = GetDlgItem(IDC_B_FREEZE_PHASE);
			ASSERT(pWnd);
			if (pWnd) pWnd->SendMessage(BM_SETCHECK, (WPARAM)0, 0L);
			OnBFreezePhase();
		}
		ASSERT(!m_bFreezePhase);


		// foreach phase
		std::list<CInvPhase>::iterator iterPhase = GetSheet()->m_Page1B.m_listInvPhase.begin();
		for (; iterPhase != GetSheet()->m_Page1B.m_listInvPhase.end(); ++iterPhase)
		{
			// clear isotope map for each phase
			iterPhase->m_mapIsotopes.clear();
		}

		// foreach row (isotope)
		for (long nRow = m_egPhase.GetFixedRows(); nRow < m_egPhase.GetRows(); ++nRow)
		{
			CIsotope isotope;

			//
			// Name
			//
			DDX_GridText(pDX, m_egPhase.GetDlgCtrlID(), nRow, 0, isotope.m_strName);
			if (isotope.m_strName.IsEmpty())
				continue;

			if (!(::_istdigit((int)isotope.m_strName[0])))
			{
				DDX_GridFail(pDX, _T("Expected isotope name to begin with an isotopic number."));
			}

			TCHAR* next_char;
			isotope.m_dIsotopeNumber = _tcstod(isotope.m_strName, &next_char);

			if (*next_char == _T('\0') || !::_istupper(*next_char))  // TODO DAVID
			{
				DDX_GridFail(pDX, _T("Expected element name to follow isotopic number."));
			}

			// foreach phase
			std::list<CInvPhase>::iterator iterPhase = GetSheet()->m_Page1B.m_listInvPhase.begin();
			for (int cPhase = 0; iterPhase != GetSheet()->m_Page1B.m_listInvPhase.end(); ++iterPhase, ++cPhase)
			{
				// check for entry
				CString strDummy;
				DDX_GridText(pDX, m_egPhase.GetDlgCtrlID(), nRow, (cPhase * 2) + 1, strDummy);
				strDummy.TrimLeft(); strDummy.TrimRight();
				if (strDummy.IsEmpty()) continue;

				//
				// Value
				//
				DDX_GridText(pDX, m_egPhase.GetDlgCtrlID(), nRow, (cPhase * 2) + 1, isotope.m_dRatio);


				//
				// Uncertainty limit (note uncertainty limit req'd)
				// 
				DDX_GridText(pDX, m_egPhase.GetDlgCtrlID(), nRow, (cPhase * 2) + 2, isotope.m_dRatioUncertainty);


				// isotope ok now add it to phase map
				iterPhase->m_mapIsotopes[isotope.m_strName] = isotope;
			}

		}

		ASSERT(m_egPhase.GetFixedCols() == 0);

		// reset freeze
		if (bFreezePhaseSave)
		{
			// turn off frozen isotopes
			CWnd* pWnd = GetDlgItem(IDC_B_FREEZE_PHASE);
			ASSERT(pWnd);
			if (pWnd) pWnd->SendMessage(BM_SETCHECK, (WPARAM)BST_CHECKED, 0L);
			OnBFreezePhase();
			ASSERT(m_bFreezePhase);
		}
	}
	else
	{
		// freeze
		if (m_bFreezePhase)
		{
			m_egPhase.SetFixedCols(1);
			m_egPhase.SetColAlignmentFixed(0, flexAlignCenterCenter);
		}
		else
		{
			m_egPhase.SetFixedCols(0);
			m_egPhase.SetColAlignment(0, flexAlignCenterCenter);
		}

		ASSERT(m_glDocPhase.GetCount() == 0);

		// set number of cols for the phase grid
		// Isotope Col (1) + 2 * Number of Phases
		m_egPhase.SetCols(0, 1 + 2 * GetSheet()->m_Page1B.m_listInvPhase.size());

		// temporary map used to hold isotopes
		std::map<CString, long> mapIso2Row;

		// foreach phase
		std::list<CInvPhase>::iterator iterPhase = GetSheet()->m_Page1B.m_listInvPhase.begin();
		for (int cPhase = 0; iterPhase != GetSheet()->m_Page1B.m_listInvPhase.end(); ++iterPhase, ++cPhase)
		{
			m_egPhase.SetTextMatrix(0, (cPhase * 2) + 1, iterPhase->m_strName);
			m_egPhase.SetTextMatrix(0, (cPhase * 2) + 2, iterPhase->m_strName);
			m_egPhase.SetTextMatrix(1, (cPhase * 2) + 1, _T("Value"));
			m_egPhase.SetTextMatrix(1, (cPhase * 2) + 2, _T("Uncert."));

			// foreach isotope
			std::map<CString, CIsotope>::iterator iterPair = iterPhase->m_mapIsotopes.begin();
			for (; iterPair != iterPhase->m_mapIsotopes.end(); ++iterPair)
			{
				std::map<CString, long>::iterator iterIso2Row = mapIso2Row.find(iterPair->second.m_strName);
				CString strNum;
				if (iterIso2Row == mapIso2Row.end())
				{
					// isotope has not been added to map/grid so do it
					CGridListItem item = m_glDocPhase.AddItem(iterPair->second.m_strName);
					ASSERT(item.m_nRow != -1);

					mapIso2Row[iterPair->second.m_strName] = item.m_nRow;

					iterIso2Row = mapIso2Row.find(iterPair->second.m_strName);
				}

				ASSERT(iterIso2Row != mapIso2Row.end());

				strNum.Format(_T("%.*g"), DBL_DIG, iterPair->second.m_dRatio);
				m_egPhase.SetTextMatrix(iterIso2Row->second, (cPhase * 2) + 1, strNum);
				strNum.Format(_T("%.*g"), DBL_DIG, iterPair->second.m_dRatioUncertainty);
				m_egPhase.SetTextMatrix(iterIso2Row->second, (cPhase * 2) + 2, strNum);
			}
		}

		// merge phase cells
		m_egPhase.SetMergeCells(flexMergeFree);
		m_egPhase.SetMergeRow(0, TRUE);

		// set column titles bold and centered
		m_egPhase.SetFillStyle(flexFillRepeat);
		m_egPhase.SetRow(0);
		m_egPhase.SetCol(0);
		m_egPhase.SetColSel(m_egPhase.GetCols(0) - 1);
		m_egPhase.SetRowSel(m_egPhase.GetFixedRows() - 1);
		m_egPhase.SetCellAlignment(flexAlignCenterCenter);
		m_egPhase.SetCellFontBold(TRUE);
		m_egPhase.SetFillStyle(flexFillSingle);

		// set initial cell
		m_egPhase.SetRow(2), m_egPhase.SetCol(0);

	}

	// enable/disable phases
	EnablePhases(GetSheet()->m_Page1B.m_listInvPhase.size() > 0);
}

void CKPInversePg3::DDX_Soln(CDataExchange* pDX)
{
	if (pDX->m_bSaveAndValidate)
	{
		CDelayUpdate delay(&m_egSol);

		BOOL bFreezeSolSave = m_bFreezeSol;
		if (m_bFreezeSol)
		{
			// turn off frozen isotopes
			CWnd* pWnd = GetDlgItem(IDC_B_FREEZE_SOL);
			ASSERT(pWnd);
			if (pWnd) pWnd->SendMessage(BM_SETCHECK, (WPARAM)0, 0L);
			OnBFreezeSol();
		}
		ASSERT(!m_bFreezeSol);

		// temp holder for isotopes
		std::list<CInvIsotope> listInvIsotopes;

		// foreach row (isotope)
		for (long nRow = m_egSol.GetFixedRows(); nRow < m_egSol.GetRows(); ++nRow)
		{
			CInvIsotope isotope;

			//
			// Name
			//
			DDX_GridText(pDX, m_egSol.GetDlgCtrlID(), nRow, 0, isotope.m_strName);

			if (isotope.m_strName.IsEmpty()) continue;

			if (!(::_istdigit((int)isotope.m_strName[0])))
			{
				DDX_GridFail(pDX, _T("Expected isotope name to begin with an isotopic number."));
			}

			TCHAR* next_char;
			isotope.m_dIsotopeNumber = _tcstod(isotope.m_strName, &next_char);

			if (*next_char == _T('\0') || !::_istupper(*next_char)) // TODO DAVID
			{
				DDX_GridFail(pDX, _T("Expected element name to follow isotopic number."));
			}

			// foreach init soln
			std::list<InvSol>::iterator iterSol = GetSheet()->m_listInvSol.begin();
			long nCol = 1;
			for (; iterSol != GetSheet()->m_listInvSol.end(); ++iterSol, ++nCol)
			{
				//
				// Uncertainty limit
				// 
				double dRatioUncertainty;
				if (nCol == 1)
				{
					// at least one uncertainty must be set
					DDX_GridText(pDX, m_egSol.GetDlgCtrlID(), nRow, nCol, dRatioUncertainty);
				}
				else
				{
					CString strDummy;
					DDX_GridText(pDX, m_egSol.GetDlgCtrlID(), nRow, nCol, strDummy);
					if (strDummy.IsEmpty())
					{
						// set to previous
						std::list<InvSol>::iterator iterPrev = iterSol;
						iterPrev--;
						dRatioUncertainty = isotope.m_mapSol2Unc[iterPrev->nSol];
					}
					else
					{
						DDX_GridText(pDX, m_egSol.GetDlgCtrlID(), nRow, nCol, dRatioUncertainty);
					}
				}
				isotope.m_mapSol2Unc[iterSol->nSol] = dRatioUncertainty;
			}

			// final soln
			CString strDummy;
			DDX_GridText(pDX, m_egSol.GetDlgCtrlID(), nRow, nCol, strDummy);
			double dRatioUncertainty;
			if (strDummy.IsEmpty())
			{
				// set to previous
				std::list<InvSol>::iterator iterPrev = GetSheet()->m_listInvSol.end();
				iterPrev--;
				dRatioUncertainty = isotope.m_mapSol2Unc[iterPrev->nSol];
			}
			else
			{
				DDX_GridText(pDX, m_egSol.GetDlgCtrlID(), nRow, nCol, dRatioUncertainty);
			}
			isotope.m_mapSol2Unc[GetSheet()->m_invSolFinal.nSol] = dRatioUncertainty;

			listInvIsotopes.push_back(isotope);
		}

		// as last step remove all entries from m_glDocSol;
		ASSERT(m_egSol.GetFixedCols() == 0);
		m_glDocSol.DeleteAllRows();

		// reset freeze
		if (bFreezeSolSave)
		{
			// turn off frozen isotopes
			CWnd* pWnd = GetDlgItem(IDC_B_FREEZE_SOL);
			ASSERT(pWnd);
			if (pWnd) pWnd->SendMessage(BM_SETCHECK, (WPARAM)BST_CHECKED, 0L);
			OnBFreezeSol();
			ASSERT(m_bFreezeSol);
		}

		// all OK set to member
		m_listInvIsotopes.assign(listInvIsotopes.begin(), listInvIsotopes.end());
	}
	else
	{
		// freeze
		if (m_bFreezeSol)
		{
			m_egSol.SetFixedCols(1);
			m_egSol.SetColAlignmentFixed(0, flexAlignCenterCenter);
		}
		else
		{
			m_egSol.SetFixedCols(0);
			m_egSol.SetColAlignment(0, flexAlignCenterCenter);
		}

		ASSERT(m_glDocSol.GetCount() == 0);

		// set number of cols
		// => Element Col (1) + Initial Solutions + Final Solution (1)
		m_egSol.SetCols(0, 1 + (long)GetSheet()->m_listInvSol.size() + 1);

		
		// Add initial solns
		ASSERT(GetSheet()->m_listInvSol.size() > 0);
		CString str;
		std::list<InvSol>::iterator iter = GetSheet()->m_listInvSol.begin();
		long nCol = 1;
		for (; iter != GetSheet()->m_listInvSol.end(); ++iter, ++nCol)
		{
			str.Format(_T("%ld"), iter->nSol);
			m_egSol.SetTextMatrix(0, nCol, str);
		}
		// Add final soln
		str.Format(_T("%ld (final)"), GetSheet()->m_invSolFinal.nSol);
		m_egSol.SetTextMatrix(0, nCol, str);


		// foreach inverse isotope
		std::list<CInvIsotope>::iterator iterElem = m_listInvIsotopes.begin();
		for (; iterElem != m_listInvIsotopes.end(); ++iterElem)
		{
			CGridListItem item = m_glDocSol.AddItem(iterElem->m_strName);
			ASSERT(item.m_nRow != -1);
			if (item.m_nRow == -1) continue;

			// foreach init soln
			std::list<InvSol>::iterator iterSol = GetSheet()->m_listInvSol.begin();
			long nCol = 1;
			for (; iterSol != GetSheet()->m_listInvSol.end(); ++iterSol, ++nCol)
			{
				std::map<int, double>::iterator iterUnc = iterElem->m_mapSol2Unc.find(iterSol->nSol);
				if (iterUnc != iterElem->m_mapSol2Unc.end())
				{
					DDX_GridText(pDX, IDC_MSHFG_SOL, item.m_nRow, nCol, iterUnc->second);
				}
				else
				{
					std::map<CString, double>::iterator iterUnc = m_mapIso2DefaultUnc.find(iterElem->m_strName);
					double d = (iterUnc != m_mapIso2DefaultUnc.end()) ? iterUnc->second : 0.0;
					DDX_GridText(pDX, IDC_MSHFG_SOL, item.m_nRow, nCol, d);
				}
			}

			// final soln
			std::map<int, double>::iterator iterUnc = iterElem->m_mapSol2Unc.find(GetSheet()->m_invSolFinal.nSol);
			if (iterUnc != iterElem->m_mapSol2Unc.end())
			{
				DDX_GridText(pDX, IDC_MSHFG_SOL, item.m_nRow, nCol, iterUnc->second);
			}
			else
			{
				std::map<CString, double>::iterator iterUnc = m_mapIso2DefaultUnc.find(iterElem->m_strName);
				double d = (iterUnc != m_mapIso2DefaultUnc.end()) ? iterUnc->second : 0.0;
				DDX_GridText(pDX, IDC_MSHFG_SOL, item.m_nRow, nCol, d);
			}			

		}

		// center align the column headers / and set bold
		m_egSol.SetFillStyle(flexFillRepeat);
		m_egSol.SetRow(0);
		m_egSol.SetCol(0);
		m_egSol.SetColSel(m_egSol.GetCols(0) - 1);
		m_egSol.SetCellAlignment(flexAlignCenterCenter);
		m_egSol.SetCellFontBold(TRUE);
		m_egSol.SetFillStyle(flexFillSingle);

		// set initial cell
		m_egSol.SetRow(1), m_egSol.SetCol(0);
	}
}

BEGIN_MESSAGE_MAP(CKPInversePg3, baseKPInversePg3)
	//{{AFX_MSG_MAP(CKPInversePg3)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CL_ISO_SOL, OnItemchangedClIsoSol)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CL_ISO_PHASE, OnItemchangedClIsoPhase)
	ON_BN_CLICKED(IDC_B_FREEZE_SOL, OnBFreezeSol)
	ON_BN_CLICKED(IDC_B_FREEZE_PHASE, OnBFreezePhase)
	ON_NOTIFY(NM_SETFOCUS, IDC_CL_ISO_SOL, OnSetfocusClIsoSol)
	ON_NOTIFY(NM_SETFOCUS, IDC_CL_ISO_PHASE, OnSetfocusClIsoPhase)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
	// custom button messages
	ON_BN_SETFOCUS(IDC_B_FREEZE_PHASE, OnSetfocusBFreezePhase)
	ON_BN_SETFOCUS(IDC_B_FREEZE_SOL, OnSetfocusBFreezeSol)

	// custom grid messages
	ON_MESSAGE(EGN_BEGINCELLEDIT, OnBeginCellEdit)
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
	ON_MESSAGE(EGN_SETFOCUS, OnSetfocusGrid)
END_MESSAGE_MAP()

void CKPInversePg2::OnItemchangedClElements(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLISTVIEW* pNMListView = (NMLISTVIEW*)pNMHDR;

	bool bCheckmarkChange = ((pNMListView->uNewState ^ pNMListView->uOldState) & LVIS_STATEIMAGEMASK) != 0;

	// ignore other changes
	if (!bCheckmarkChange) return;

	ASSERT_VALID(this);

	// used to hide fixed col change
	// nec since delete row doesn't remove
	// anything in a fixed column
	CDelayUpdate* pDelay = NULL;

	if (bCheckmarkChange && m_bFrozen)
	{
		pDelay = new CDelayUpdate(&m_egBalance);
		m_egBalance.SetFixedCols(0);
	}

	// shouldn't be any fixed columns
	ASSERT(m_egBalance.GetFixedCols() == 0);
	long nRow = m_glDoc.OnItemChanged(pNMHDR, pResult);

	if (nRow != -1)
	{
		// set default uncertainties
		CString str;
		std::list<InvSol>::iterator iter = GetSheet()->m_listInvSol.begin();
		long nCol = m_egBalance.GetFixedCols() + 1;
		for (; iter != GetSheet()->m_listInvSol.end(); ++iter, ++nCol)
		{
#ifdef _DEBUG
			CString strCheck;
			strCheck.Format(_T("%ld"), iter->nSol);
			ASSERT(strCheck == m_egBalance.GetTextMatrix(0, nCol));
#endif
			if (m_egBalance.GetTextMatrix(nRow, 0).CompareNoCase(_T("pH")) != 0)
			{
				str.Format(_T("%.*g"), DBL_DIG, iter->dUncertainty);
			}
			else
			{
				str.Format(_T("%.*g"), DBL_DIG, 0.05);
			}
			m_egBalance.SetTextMatrix(nRow, nCol, str);
		}
		// Add final soln
		if (m_egBalance.GetTextMatrix(nRow, 0).CompareNoCase(_T("pH")) != 0)
		{
			str.Format(_T("%.*g"), DBL_DIG, GetSheet()->m_invSolFinal.dUncertainty);
		}
		else
		{
			str.Format(_T("%.*g"), DBL_DIG, 0.05);
		}
		m_egBalance.SetTextMatrix(nRow, nCol, str);
	}


	if (bCheckmarkChange && m_bFrozen)
	{
		m_egBalance.SetFixedCols(1);
		ASSERT(pDelay);
		delete pDelay;
	}

	ASSERT_VALID(this);

	return;
}

#ifdef _DEBUG
CCKSInverse* CKPInversePg1A::GetSheet() // non-debug version is inline
{
	CCKSInverse* pSheet = (CCKSInverse*)GetParent();
	ASSERT_KINDOF(CCKSInverse, pSheet);
	return pSheet;
}

CCKSInverse* CKPInversePg1B::GetSheet() // non-debug version is inline
{
	CCKSInverse* pSheet = (CCKSInverse*)GetParent();
	ASSERT_KINDOF(CCKSInverse, pSheet);
	return pSheet;
}

CCKSInverse* CKPInversePg2::GetSheet() // non-debug version is inline
{
	CCKSInverse* pSheet = (CCKSInverse*)GetParent();
	ASSERT_KINDOF(CCKSInverse, pSheet);
	return pSheet;
}

CCKSInverse* CKPInversePg3::GetSheet() // non-debug version is inline
{
	CCKSInverse* pSheet = (CCKSInverse*)GetParent();
	ASSERT_KINDOF(CCKSInverse, pSheet);
	return pSheet;
}
#endif

void CKPInversePg2::OnBFreeze() 
{
	// mimick UpdateData(TRUE) with just the freeze elements checkbox
	CDataExchange dx(this, TRUE);
	
	bool bOk = false;
	try
	{
		DDX_Check(&dx, IDC_B_FREEZE, m_bFrozen);
		bOk = true;
	}
	catch (CUserException* e)
	{
		ASSERT(!bOk);
		ASSERT(FALSE);
		e->ReportError(MB_OK);
		e->Delete();
	}
	if (bOk)
	{
		if (m_bFrozen)
		{
			m_egBalance.SetFixedCols(1);
			m_egBalance.SetColAlignmentFixed(0, flexAlignCenterCenter);
		}
		else
		{
			m_egBalance.SetFixedCols(0);
			m_egBalance.SetColAlignment(0, flexAlignCenterCenter);
		}
	}
}

void CKPInversePg2::OnBWater()
{
	// mimick UpdateData(TRUE) with just the water uncertainty checkbox
	CDataExchange dx(this, TRUE);
	bool bOk = false;
	try
	{
		DDX_Check(&dx, IDC_B_WATER, m_bWaterUncert);
		bOk = true;
	}
	catch (CUserException* e)
	{
		ASSERT(!bOk);
		ASSERT(FALSE);
		e->ReportError(MB_OK);
		e->Delete();
	}
	if (bOk)
	{
		EnableWater(m_bWaterUncert);
	}
}

void CKPInversePg2::EnableWater(BOOL bEnable)
{
	// enable/disable water uncertainty textbox
	CWnd* pWnd = GetDlgItem(IDC_E_WATER);
	if (pWnd)
	{
		pWnd->EnableWindow(bEnable);
	}
}

void CKPInversePg2::OnBReset() 
{
	ASSERT_VALID(this);

	CString str;

	for (long nRow = m_egBalance.GetFixedRows(); nRow < m_egBalance.GetRows(); ++nRow)
	{
		if (!m_egBalance.GetTextMatrix(nRow, 0).IsEmpty())
		{
			// reset defaults
			CString strElem = m_egBalance.GetTextMatrix(nRow, 0);
			if (strElem.CompareNoCase(_T("pH")) != 0)
			{
				std::list<InvSol>::iterator iter = GetSheet()->m_listInvSol.begin();
				long nCol = 1;
				for (; iter != GetSheet()->m_listInvSol.end(); ++iter, ++nCol)
				{
					str.Format(_T("%.*g"), DBL_DIG, iter->dUncertainty);
					m_egBalance.SetTextMatrix(nRow, nCol, str);
				}
				// Add final soln
				ASSERT(nCol == m_egBalance.GetCols(0)  - 1);
				str.Format(_T("%.*g"), DBL_DIG, GetSheet()->m_invSolFinal.dUncertainty);
				m_egBalance.SetTextMatrix(nRow, nCol, str);
			}
			else
			{
				// pH defaults to 0.05
				for (long nCol = 1; nCol < m_egBalance.GetCols(0); ++nCol)
				{
					str.Format(_T("%.*g"), DBL_DIG, 0.05);
					m_egBalance.SetTextMatrix(nRow, nCol, str);
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CKPRatesPg1 diagnostics

#ifdef _DEBUG
void CKPInversePg2::AssertValid()/* const */
{
	baseKPInversePg2::AssertValid();

	if (m_bFrozen)
	{
		ASSERT(m_egBalance.GetFixedCols() == 1);
	}
	else
	{
		ASSERT(m_egBalance.GetFixedCols() == 0);
	}
}
#endif

LRESULT CKPInversePg2::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bDenyEdit = FALSE;	// allow edit	

	switch (nID)
	{
	case IDC_MSHFG_BALANCE :
		switch ( pInfo->item.iCol )
		{
		case 0 :
			CUtil::InsertAqElements(pInfo->item.hWndCombo, GetDatabase());
			if (pInfo->item.hWndCombo)
			{
				ASSERT(::IsWindow(pInfo->item.hWndCombo));
				::SendMessage(pInfo->item.hWndCombo, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)_T("pH"));
				pInfo->item.bUseCombo = (::SendMessage(pInfo->item.hWndCombo, CB_GETCOUNT, 0, 0) > 0);
			}
			else
			{
				pInfo->item.bUseCombo = TRUE;
			}
			break;
		}
		break;
	}
	return bDenyEdit;
}

LRESULT CKPInversePg2::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bMakeChange = TRUE;

	// ignore if edit is cancelled
	if ( pInfo->item.pszText == NULL ) return bMakeChange;

	switch (nID)
	{
	case IDC_MSHFG_BALANCE :
		if (pInfo->item.iCol == 0)
		{
			CString strElem = pInfo->item.pszText;

			// set default uncertainties
			CString str;
			std::list<InvSol>::iterator iter = GetSheet()->m_listInvSol.begin();
			long nCol = m_egBalance.GetFixedCols() + 1;
			for (; iter != GetSheet()->m_listInvSol.end(); ++iter, ++nCol)
			{
#ifdef _DEBUG
				CString strCheck;
				strCheck.Format(_T("%ld"), iter->nSol);
				ASSERT(strCheck == m_egBalance.GetTextMatrix(0, nCol));
#endif
				if (strElem.CompareNoCase(_T("pH")) != 0)
				{
					str.Format(_T("%.*g"), DBL_DIG, iter->dUncertainty);
				}
				else
				{
					str.Format(_T("%.*g"), DBL_DIG, 0.05);
				}
				if (m_egBalance.GetTextMatrix(pInfo->item.iRow, nCol).IsEmpty())
				{
					m_egBalance.SetTextMatrix(pInfo->item.iRow, nCol, str);
				}
			}
			// Add final soln
			if (strElem.CompareNoCase(_T("pH")) != 0)
			{
				str.Format(_T("%.*g"), DBL_DIG, GetSheet()->m_invSolFinal.dUncertainty);
			}
			else
			{
				str.Format(_T("%.*g"), DBL_DIG, 0.05);
			}
			if (m_egBalance.GetTextMatrix(pInfo->item.iRow, nCol).IsEmpty())
			{
				m_egBalance.SetTextMatrix(pInfo->item.iRow, nCol, str);
			}
		}

		return m_glDoc.OnEndCellEdit(wParam, lParam);
		break;
	}

	return bMakeChange;
}

BEGIN_EVENTSINK_MAP(CKPInversePg2, baseKPInversePg2)
    //{{AFX_EVENTSINK_MAP(CKPInversePg2)
	ON_EVENT(CKPInversePg2, IDC_MSHFG_BALANCE, -602 /* KeyDown */, OnKeyDownMshfgBalance, VTS_PI2 VTS_I2)
	ON_EVENT(CKPInversePg2, IDC_MSHFG_BALANCE, 71 /* EnterCell */, OnEnterCellMshfgBalance, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CKPInversePg2::OnKeyDownMshfgBalance(short FAR* KeyCode, short Shift) 
{
	// mustn't have any disabled cells
	m_glDoc.OnKeyDown(KeyCode, Shift);	
}

void CKPInversePg2::OnSetfocusClElements(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNUSED(pNMHDR);

	CString strRes;
	AfxFormatString1(strRes, IDS_CL_167, _T("element(s)") );
	// strRes += _T(" Right-click to display details");
	m_eInputDesc.SetWindowText(strRes);
	*pResult = 0;
}

void CKPInversePg2::OnEnterCellMshfgBalance() 
{
	CString strRes;

	switch (m_egBalance.GetCol())
	{
	case 0 :
		strRes.LoadString(IDS_INVERSE_282);
		break;

	default :
		strRes.LoadString(IDS_INVERSE_283);
		strRes += CString((LPCSTR)IDS_INVERSE_284);
		break;
	}

	m_eInputDesc.SetWindowText(strRes);
}

void CKPInversePg2::OnSetfocusEWater() 
{
	CString strRes;
	strRes.LoadString(IDS_INVERSE_285);
	m_eInputDesc.SetWindowText(strRes);
}

void CKPInversePg2::OnSetfocusBWater() 
{
	CString strRes;
	strRes.LoadString(IDS_INVERSE_286);
	m_eInputDesc.SetWindowText(strRes);
}
void CKPInversePg2::OnSetfocusBFreeze() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING498);
	m_eInputDesc.SetWindowText(strRes);
}
void CKPInversePg2::OnSetfocusBReset() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING499);
	m_eInputDesc.SetWindowText(strRes);
}

LRESULT CKPInversePg2::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
{
	UINT nID = wParam;
	UNUSED_ALWAYS(lParam);

	switch (nID)
	{
	case IDC_MSHFG_BALANCE :
		OnEnterCellMshfgBalance();
		break;
	}
	return 0;
}

BOOL CKPInversePg2::OnInitDialog() 
{
	baseKPInversePg2::OnInitDialog();	

	CreateRoot(VERTICAL, 5, 6)
		<< (pane(HORIZONTAL, ABSOLUTE_VERT)
			<< item(IDC_S_DEF_ELEMS, RELATIVE_HORZ, 38)
			<< item(IDC_S_SOLUTIONS, RELATIVE_HORZ, 62)
			)
		<< (pane(HORIZONTAL, GREEDY)
			<< item(IDC_CL_ELEMENTS, RELATIVE_HORZ, 38)
			<< item(IDC_MSHFG_BALANCE, RELATIVE_HORZ, 62)
			)
		<< (pane(HORIZONTAL, ABSOLUTE_VERT)
			<< item(IDC_B_WATER, NORESIZE)
			<< item(IDC_E_WATER, NORESIZE)
			<< item(paneNull, RELATIVE_HORZ, 38)
			<< item(IDC_B_FREEZE, NORESIZE)
			<< item(paneNull, RELATIVE_HORZ, 62)
			<< itemSpaceBetween(HORIZONTAL, IDC_B_FREEZE, IDC_B_RESET)
			<< itemFixed(HORIZONTAL, 40)
			<< item(IDC_B_RESET, NORESIZE)
			)
		<< (paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, GREEDY, nDefaultBorder, 10, 10)
			<< item(IDC_E_DESC_INPUT, ABSOLUTE_VERT)
			);
	UpdateLayout();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKPInversePg3::OnItemchangedClIsoSol(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLISTVIEW* pNMListView = (NMLISTVIEW*)pNMHDR;

	bool bCheckmarkChange = ((pNMListView->uNewState ^ pNMListView->uOldState) & LVIS_STATEIMAGEMASK) != 0;

	// ignore other changes
	if (!bCheckmarkChange) return;

	ASSERT_VALID(this);

	// used to hide fixed col change
	// nec since delete row doesn't remove
	// anything in a fixed column
	CDelayUpdate* pDelay = NULL;

	if (bCheckmarkChange && m_bFreezeSol)
	{
		pDelay = new CDelayUpdate(&m_egSol);
		m_egSol.SetFixedCols(0);
	}


	long nRow = m_glDocSol.OnItemChanged(pNMHDR, pResult);
	if (nRow != -1)
	{
		CString strIso = m_egSol.GetTextMatrix(nRow, 0);
		std::map<CString, double>::iterator item = m_mapIso2DefaultUnc.find(strIso);
		double nValue = (item != m_mapIso2DefaultUnc.end()) ? item->second : 0.0;
		CString strNum;
		strNum.Format(_T("%.*g"), DBL_DIG, nValue);
		for (long nCol = 1; nCol < m_egSol.GetCols(0); ++nCol)
		{
			if (m_egSol.GetTextMatrix(nRow, nCol).IsEmpty())
			{
				m_egSol.SetTextMatrix(nRow, nCol, strNum);
			}
		}
	}

	if (bCheckmarkChange && m_bFreezeSol)
	{
		m_egSol.SetFixedCols(1);
		ASSERT(pDelay);
		delete pDelay;
	}

	ASSERT_VALID(this);
}

void CKPInversePg3::OnItemchangedClIsoPhase(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLISTVIEW* pNMListView = (NMLISTVIEW*)pNMHDR;

	bool bCheckmarkChange = ((pNMListView->uNewState ^ pNMListView->uOldState) & LVIS_STATEIMAGEMASK) != 0;

	// ignore other changes
	if (!bCheckmarkChange) return;

	ASSERT_VALID(this);

	// used to hide fixed col change
	// nec since delete row doesn't remove
	// anything in a fixed column
	CDelayUpdate* pDelay = NULL;

	if (bCheckmarkChange && m_bFreezePhase)
	{
		pDelay = new CDelayUpdate(&m_egPhase);
		m_egPhase.SetFixedCols(0);
	}

	// shouldn't be any fixed columns
	ASSERT(m_egPhase.GetFixedCols() == 0);
	long nRow = m_glDocPhase.OnItemChanged(pNMHDR, pResult);

	if (nRow != -1)
	{
		// set default uncertainties
	}

	if (bCheckmarkChange && m_bFreezePhase)
	{
		m_egPhase.SetFixedCols(1);
		ASSERT(pDelay);
		delete pDelay;
	}

	ASSERT_VALID(this);
}

BEGIN_EVENTSINK_MAP(CKPInversePg3, baseKPInversePg3)
    //{{AFX_EVENTSINK_MAP(CKPInversePg3)
	ON_EVENT(CKPInversePg3, IDC_MSHFG_SOL, -602 /* KeyDown */, OnKeyDownMshfgSol, VTS_PI2 VTS_I2)
	ON_EVENT(CKPInversePg3, IDC_MSHFG_PHASE, -602 /* KeyDown */, OnKeyDownMshfgPhase, VTS_PI2 VTS_I2)
	ON_EVENT(CKPInversePg3, IDC_MSHFG_SOL, 71 /* EnterCell */, OnEnterCellMshfgSol, VTS_NONE)
	ON_EVENT(CKPInversePg3, IDC_MSHFG_PHASE, 71 /* EnterCell */, OnEnterCellMshfgPhase, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CKPInversePg3::OnKeyDownMshfgSol(short FAR* KeyCode, short Shift) 
{
	m_glDocSol.OnKeyDown(KeyCode, Shift);
}

void CKPInversePg3::OnKeyDownMshfgPhase(short FAR* KeyCode, short Shift) 
{
	////m_glDocPhase.OnKeyDown(KeyCode, Shift);
	UNUSED(Shift);

	switch (*KeyCode)
	{
	case VK_DELETE :
		m_egPhase.SetRedraw(FALSE);
		if (m_egPhase.GetCol() == 0)
		{
			m_glDocPhase.DeleteRow(m_egPhase.GetRow());
		}
		else 
		{
			// just fill with empty strings
			///m_egPhase.SetFillStyle(flexFillRepeat);
			m_egPhase.SetText(_T(""));
			///m_egPhase.SetFillStyle(flexFillSingle);
		}
		m_egPhase.SetRedraw(TRUE);
		break;
	}
}

LRESULT CKPInversePg3::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	ASSERT(nID == IDC_MSHFG_SOL || nID == IDC_MSHFG_PHASE);

	BOOL bDenyEdit = FALSE;

	switch (pInfo->item.iCol)
	{
	case 0 :	// name
		pInfo->item.bUseCombo = (CUtil::InsertIsotopes(pInfo->item.hWndCombo) > 0);
		break;
	}

	return bDenyEdit;	// allow edit
}

LRESULT CKPInversePg3::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bMakeChange = TRUE;

	// ignore if edit is cancelled
	if ( pInfo->item.pszText == NULL ) return bMakeChange;

	switch ( nID )
	{
	case IDC_MSHFG_SOL :
		if (pInfo->item.iCol == 0)
		{
			// set default values
			CString strIso = pInfo->item.pszText;
			std::map<CString, double>::iterator item = m_mapIso2DefaultUnc.find(strIso);
			double nValue = (item != m_mapIso2DefaultUnc.end()) ? item->second : 0.0;
			CString strNum;
			strNum.Format(_T("%.*g"), DBL_DIG, nValue);
			for (long nCol = 1; nCol < m_egSol.GetCols(0); ++nCol)
			{
				if (m_egSol.GetTextMatrix(pInfo->item.iRow, nCol).IsEmpty())
				{
					m_egSol.SetTextMatrix(pInfo->item.iRow, nCol, strNum);
				}
			}
		}
		return m_glDocSol.OnEndCellEdit(wParam, lParam);
		break;

	case IDC_MSHFG_PHASE :
		if (pInfo->item.iCol % 2 && pInfo->item.pszText && strlen(pInfo->item.pszText))
		{
			CString strIso = m_egPhase.GetTextMatrix(pInfo->item.iRow, 0);
			std::map<CString, double>::iterator item = m_mapIso2DefaultUnc.find(strIso);
			double nValue = (item != m_mapIso2DefaultUnc.end()) ? item->second : 0.0;
			CString strNum;
			strNum.Format(_T("%.*g"), DBL_DIG, nValue);

			if (m_egPhase.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol + 1).IsEmpty())
			{
				m_egPhase.SetTextMatrix(pInfo->item.iRow, pInfo->item.iCol + 1, strNum);
			}
		}
		return m_glDocPhase.OnEndCellEdit(wParam, lParam);
		break;

	default :
		ASSERT(FALSE);
	}
	return bMakeChange;
}

void CKPInversePg3::OnBFreezeSol() 
{
	// mimick UpdateData(TRUE) with just the freeze elements checkbox
	CDataExchange dx(this, TRUE);
	
	bool bOk = false;
	try
	{
		DDX_Check(&dx, IDC_B_FREEZE_SOL, m_bFreezeSol);
		bOk = true;
	}
	catch (CUserException* e)
	{
		ASSERT(!bOk);
		ASSERT(FALSE);
		e->ReportError(MB_OK);
		e->Delete();
	}
	if (bOk)
	{
		if (m_bFreezeSol)
		{
			m_egSol.SetFixedCols(1);
			m_egSol.SetColAlignmentFixed(0, flexAlignCenterCenter);
		}
		else
		{
			m_egSol.SetFixedCols(0);
			m_egSol.SetColAlignment(0, flexAlignCenterCenter);
		}
	}
	
}

void CKPInversePg3::OnBFreezePhase() 
{
	ASSERT( m_bFreezePhase ? m_egPhase.GetFixedCols() == 1 : m_egPhase.GetFixedCols() == 0);

	// mimick UpdateData(TRUE) with just the freeze elements checkbox
	CDataExchange dx(this, TRUE);
	
	bool bOk = false;
	try
	{
		DDX_Check(&dx, IDC_B_FREEZE_PHASE, m_bFreezePhase);
		bOk = true;
	}
	catch (CUserException* e)
	{
		ASSERT(!bOk);
		ASSERT(FALSE);
		e->ReportError(MB_OK);
		e->Delete();
	}
	if (bOk)
	{
		if (m_bFreezePhase)
		{
			m_egPhase.SetFixedCols(1);
			m_egPhase.SetColAlignmentFixed(0, flexAlignCenterCenter);
		}
		else
		{
			m_egPhase.SetFixedCols(0);
			m_egPhase.SetColAlignment(0, flexAlignCenterCenter);

// COMMENT: {5/29/2001 3:39:03 PM}			//{{
// COMMENT: {5/29/2001 3:39:03 PM}			m_egPhase.SetRow(2);
// COMMENT: {5/29/2001 3:39:03 PM}			m_egPhase.SetCol(1);
// COMMENT: {5/29/2001 3:39:03 PM}			COLORREF clrCellBackColor = m_egPhase.GetCellBackColor();
// COMMENT: {5/29/2001 3:39:03 PM}			COLORREF clrCellForeColor = m_egPhase.GetCellForeColor();
// COMMENT: {5/29/2001 3:39:03 PM}
// COMMENT: {5/29/2001 3:39:03 PM}			// save state
// COMMENT: {5/29/2001 3:39:03 PM}			long nRow       = m_egPhase.GetRow();
// COMMENT: {5/29/2001 3:39:03 PM}			long nCol       = m_egPhase.GetCol();
// COMMENT: {5/29/2001 3:39:03 PM}			long nRowSel    = m_egPhase.GetRowSel();
// COMMENT: {5/29/2001 3:39:03 PM}			long nColSel    = m_egPhase.GetColSel();
// COMMENT: {5/29/2001 3:39:03 PM}			long nFillStyle = m_egPhase.GetFillStyle();
// COMMENT: {5/29/2001 3:39:03 PM}			BOOL bRedraw    = m_egPhase.GetRedraw();
// COMMENT: {5/29/2001 3:39:03 PM}
// COMMENT: {5/29/2001 3:39:03 PM}			// avoid flashing
// COMMENT: {5/29/2001 3:39:03 PM}			m_egPhase.SetRedraw(FALSE);
// COMMENT: {5/29/2001 3:39:03 PM}
// COMMENT: {5/29/2001 3:39:03 PM}			m_egPhase.SetFillStyle(flexFillRepeat);
// COMMENT: {5/29/2001 3:39:03 PM}			m_egPhase.SetRow(1);
// COMMENT: {5/29/2001 3:39:03 PM}			m_egPhase.SetCol(0);
// COMMENT: {5/29/2001 3:39:03 PM}
// COMMENT: {5/29/2001 3:39:03 PM}			// clear col selection
// COMMENT: {5/29/2001 3:39:03 PM}			m_egPhase.SetRowSel(m_egPhase.GetRows() - 1);
// COMMENT: {5/29/2001 3:39:03 PM}			m_egPhase.SetColSel(0);
// COMMENT: {5/29/2001 3:39:03 PM}			m_egPhase.SetCellBackColor(clrCellBackColor);
// COMMENT: {5/29/2001 3:39:03 PM}			m_egPhase.SetCellForeColor(clrCellForeColor);
// COMMENT: {5/29/2001 3:39:03 PM}
// COMMENT: {5/29/2001 3:39:03 PM}			// restore state
// COMMENT: {5/29/2001 3:39:03 PM}			m_egPhase.SetRow(nRow);
// COMMENT: {5/29/2001 3:39:03 PM}			m_egPhase.SetCol(nCol);
// COMMENT: {5/29/2001 3:39:03 PM}			m_egPhase.SetRowSel(nRowSel);
// COMMENT: {5/29/2001 3:39:03 PM}			m_egPhase.SetColSel(nColSel);
// COMMENT: {5/29/2001 3:39:03 PM}			m_egPhase.SetFillStyle(nFillStyle);	
// COMMENT: {5/29/2001 3:39:03 PM}			if (bRedraw) m_egPhase.SetRedraw(TRUE);
// COMMENT: {5/29/2001 3:39:03 PM}			//}}
// COMMENT: {5/29/2001 3:39:03 PM}
		}
	}

	ASSERT( m_bFreezePhase ? m_egPhase.GetFixedCols() == 1 : m_egPhase.GetFixedCols() == 0);
}

BOOL CKPInversePg3::OnInitDialog() 
{
	baseKPInversePg3::OnInitDialog();
	
	CreateRoot(VERTICAL, 5, 6)
		<< (paneCtrl(IDC_S_SOL_UNC, HORIZONTAL, GREEDY, nDefaultBorder, 10, 10)
			<< item(IDC_CL_ISO_SOL, ABSOLUTE_HORZ)
			<< item(IDC_B_FREEZE_SOL, NORESIZE)
			<< item(IDC_MSHFG_SOL, GREEDY)
			)
		<< (paneCtrl(IDC_S_PHASES, HORIZONTAL, GREEDY, nDefaultBorder, 10, 10)
			<< item(IDC_CL_ISO_PHASE, ABSOLUTE_HORZ)
			<< item(IDC_B_FREEZE_PHASE, NORESIZE)
			<< item(IDC_MSHFG_PHASE, GREEDY)
			)
		<< (paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, GREEDY, nDefaultBorder, 10, 10)
			<< item(IDC_E_DESC_INPUT, ABSOLUTE_VERT)
			);
	UpdateLayout();

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKPInversePg3::InitSoln()
{
	ASSERT(m_egSol.GetSafeHwnd() != NULL);
	ASSERT(m_clcIsoSol.GetSafeHwnd() != NULL);

	// add isotopes to checklist control
	CUtil::InsertIsotopes(&m_clcIsoSol);

	// set grid titles
	m_egSol.SetTextMatrix(0, 0, _T("Isotope"));

	// center align isotope column
	m_egSol.SetColAlignment(0, flexAlignCenterCenter);

	// don't show col or row selections
	m_egSol.SetShowColSelection(false);
	m_egSol.SetShowRowSelection(false);

	// set initial cell
	m_egSol.SetRow(1), m_egSol.SetCol(0);
}

void CKPInversePg3::InitPhases()
{
	ASSERT(m_egPhase.GetSafeHwnd() != NULL);
	ASSERT(m_clcIsoPhase.GetSafeHwnd() != NULL);

	// add isotopes to checklist control
	CUtil::InsertIsotopes(&m_clcIsoPhase);

	// set grid titles
	m_egPhase.SetTextMatrix(0, 0, _T("Isotope"));
	m_egPhase.SetTextMatrix(1, 0, _T("Isotope"));

	// merge isotope cells
	m_egPhase.SetMergeCells(flexMergeFree);
	m_egPhase.SetMergeCol(0, TRUE);

	// center align isotope column
	m_egPhase.SetColAlignment(0, flexAlignCenterCenter);

	// don't show col or row selections
	m_egPhase.SetShowColSelection(false);
	m_egPhase.SetShowRowSelection(false);

	// set initial cell
	m_egPhase.SetRow(2), m_egPhase.SetCol(0);
}

void CKPInversePg3::EnablePhases(bool bEnable)
{
	CWnd* pWnd = GetDlgItem(IDC_S_PHASES);
	if (pWnd)
	{
		pWnd->EnableWindow(bEnable);
	}

	pWnd = GetDlgItem(IDC_B_FREEZE_PHASE);
	if (pWnd)
	{
		pWnd->EnableWindow(bEnable);
	}

	m_clcIsoPhase.EnableWindow(bEnable);

	m_egPhase.EnableWindow(bEnable);
}

void CKPInversePg3::OnSetfocusClIsoSol(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNUSED(pNMHDR);

	CString strRes;
	AfxFormatString1(strRes, IDS_CL_167, _T("isotope(s)") );
	m_eInputDesc.SetWindowText(strRes);

	// set default selection
	// this gives user feedback when the list recieves the focus
	if (m_clcIsoSol.GetFirstSelectedItemPosition() == NULL)
	{
		m_clcIsoSol.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}
	*pResult = 0;
}

void CKPInversePg3::OnSetfocusClIsoPhase(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNUSED(pNMHDR);

	CString strRes;
	AfxFormatString1(strRes, IDS_CL_167, _T("isotope(s)") );
	m_eInputDesc.SetWindowText(strRes);

	// set default selection
	// this gives user feedback when the list recieves the focus
	if (m_clcIsoPhase.GetFirstSelectedItemPosition() == NULL)
	{
		m_clcIsoPhase.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}
	*pResult = 0;
}

void CKPInversePg3::OnSetfocusBFreezePhase() 
{
	CString strRes;
	strRes.LoadString(IDS_INVERSE_287);
	m_eInputDesc.SetWindowText(strRes);
}

void CKPInversePg3::OnSetfocusBFreezeSol() 
{
	CString strRes;
	strRes.LoadString(IDS_INVERSE_287);
	m_eInputDesc.SetWindowText(strRes);
}

void CKPInversePg3::OnEnterCellMshfgSol() 
{
	CString strRes;

	switch (m_egSol.GetCol())
	{
	case 0 :
		strRes.LoadString(IDS_INVERSE_288);
		break;

	default :
		strRes.LoadString(IDS_INVERSE_289);
		break;
	}
	m_eInputDesc.SetWindowText(strRes);
	
}

void CKPInversePg3::OnEnterCellMshfgPhase() 
{
	CString strRes;
	switch (long nCol = m_egPhase.GetCol())
	{
	case 0 :
		strRes.LoadString(IDS_INVERSE_290);
		break;

	default :
		if (nCol % 2)
		{
			strRes.LoadString(IDS_INVERSE_291);
		}
		else
		{
			strRes.LoadString(IDS_INVERSE_292);
		}
		break;
	}
	m_eInputDesc.SetWindowText(strRes);
}

LRESULT CKPInversePg3::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
{
	UINT nID = wParam;
	UNUSED_ALWAYS(lParam);

	switch (nID)
	{
	case IDC_MSHFG_SOL :
		OnEnterCellMshfgSol();
		break;

	case IDC_MSHFG_PHASE :
		OnEnterCellMshfgPhase();
		break;

	default :
		ASSERT(FALSE);
		break;
	}
	return 0;
}

BOOL CKPInversePg1B::OnHelpInfo(HELPINFO* pHelpInfo) 
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
	case IDC_MSHFG_PHASEGRID :
		if (!IsContextHelp())
		{
			if (!(m_egPhaseGrid.GetRowIsVisible(m_egPhaseGrid.GetRow()) && m_egPhaseGrid.GetColIsVisible(m_egPhaseGrid.GetCol())))
			{
				::MessageBeep((UINT)-1);
				return TRUE;
			}

			// modify placement
			CDC* pDC = m_egPhaseGrid.GetDC();
			int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
			int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

			long nLeft = ::MulDiv(m_egPhaseGrid.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
			long nTop  = ::MulDiv(m_egPhaseGrid.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

			CRect rc;
			m_egPhaseGrid.GetWindowRect(rc);

			myPopup.pt.x = rc.left + nLeft;
			myPopup.pt.y = rc.top + nTop + 10;
		}
		switch (IsContextHelp() ? m_egPhaseGrid.GetMouseCol() : m_egPhaseGrid.GetCol())
		{
		case nNameCol:
			strRes.LoadString(IDS_INVERSE_269);
			break;
		case nDissOrPrecCol:
			strRes.LoadString(IDS_INVERSE_270);
			break;
		case nForceCol:
			strRes.LoadString(IDS_INVERSE_271);
			break;
		default:
			// No help topic is associated with this item. 
			strRes.LoadString(IDS_STRING441);
			break;
		}
		break;
	case IDC_CL_PHASES:
		AfxFormatString1(strRes, IDS_CL_167, _T("phase(s)") ); 
		break;
	default:
		// No help topic is associated with this item. 
		strRes.LoadString(IDS_STRING441);
		break;
	}
	myPopup.pszText = strRes;
	return ::HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}

BOOL CKPInversePg2::OnHelpInfo(HELPINFO* pHelpInfo) 
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
	case IDC_B_FREEZE:
		strRes.LoadString(IDS_STRING498);
		break;
	case IDC_B_RESET:
		strRes.LoadString(IDS_STRING499);
		break;
	case IDC_MSHFG_BALANCE:
		if (!IsContextHelp())
		{
			if (!(m_egBalance.GetRowIsVisible(m_egBalance.GetRow()) && m_egBalance.GetColIsVisible(m_egBalance.GetCol())))
			{
				::MessageBeep((UINT)-1);
				return TRUE;
			}

			// modify placement
			CDC* pDC = m_egBalance.GetDC();
			int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
			int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

			long nLeft = ::MulDiv(m_egBalance.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
			long nTop  = ::MulDiv(m_egBalance.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

			CRect rc;
			m_egBalance.GetWindowRect(rc);

			myPopup.pt.x = rc.left + nLeft;
			myPopup.pt.y = rc.top + nTop + 10;
		}
		switch (IsContextHelp() ? m_egBalance.GetMouseCol() : m_egBalance.GetCol())
		{
		case 0:
			strRes.LoadString(IDS_INVERSE_282);
			break;
		default:
			strRes.LoadString(IDS_INVERSE_283);
			strRes += CString((LPCSTR)IDS_INVERSE_284);
			break;
		}
		break;
	case IDC_E_WATER:
		strRes.LoadString(IDS_INVERSE_285);
		break;
	case IDC_CL_ELEMENTS:
		AfxFormatString1(strRes, IDS_CL_167, _T("element(s)") );
		break;
	case IDC_B_WATER:
		strRes.LoadString(IDS_INVERSE_286);
		break;
	default:
		// No help topic is associated with this item. 
		strRes.LoadString(IDS_STRING441);
		break;
	}
	myPopup.pszText = strRes;
	return ::HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}

BOOL CKPInversePg3::OnHelpInfo(HELPINFO* pHelpInfo) 
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
	case IDC_MSHFG_SOL :
		if (!IsContextHelp())
		{
			if (!(m_egSol.GetRowIsVisible(m_egSol.GetRow()) && m_egSol.GetColIsVisible(m_egSol.GetCol())))
			{
				::MessageBeep((UINT)-1);
				return TRUE;
			}

			// modify placement
			CDC* pDC = m_egSol.GetDC();
			int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
			int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

			long nLeft = ::MulDiv(m_egSol.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
			long nTop  = ::MulDiv(m_egSol.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

			CRect rc;
			m_egSol.GetWindowRect(rc);

			myPopup.pt.x = rc.left + nLeft;
			myPopup.pt.y = rc.top + nTop + 10;
		}
		switch (IsContextHelp() ? m_egSol.GetMouseCol() : m_egSol.GetCol())
		{
		case 0 :
			strRes.LoadString(IDS_INVERSE_288);
			break;
		default :
			strRes.LoadString(IDS_INVERSE_289);
			break;
		}
		break;
	case IDC_MSHFG_PHASE :
		if (!IsContextHelp())
		{
			if (!(m_egPhase.GetRowIsVisible(m_egPhase.GetRow()) && m_egPhase.GetColIsVisible(m_egPhase.GetCol())))
			{
				::MessageBeep((UINT)-1);
				return TRUE;
			}

			// modify placement
			CDC* pDC = m_egPhase.GetDC();
			int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
			int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

			long nLeft = ::MulDiv(m_egPhase.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
			long nTop  = ::MulDiv(m_egPhase.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

			CRect rc;
			m_egPhase.GetWindowRect(rc);

			myPopup.pt.x = rc.left + nLeft;
			myPopup.pt.y = rc.top + nTop + 10;
		}
		switch (long nCol = IsContextHelp() ? m_egPhase.GetMouseCol() : m_egPhase.GetCol())
		{
		case 0:
			strRes.LoadString(IDS_INVERSE_290);
			break;
		default :
			if (nCol % 2)
			{
				strRes.LoadString(IDS_INVERSE_291);
			}
			else
			{
				strRes.LoadString(IDS_INVERSE_292);
			}
			break;
		}
		break;
	case IDC_CL_ISO_SOL:
		AfxFormatString1(strRes, IDS_CL_167, _T("isotope(s)") );
		break;
	case IDC_CL_ISO_PHASE:
		AfxFormatString1(strRes, IDS_CL_167, _T("isotope(s)") );
		break;
	case IDC_B_FREEZE_SOL:
		strRes.LoadString(IDS_INVERSE_287);
		break;
	case IDC_B_FREEZE_PHASE:
		strRes.LoadString(IDS_INVERSE_287);
		break;
	default:
		// No help topic is associated with this item. 
		strRes.LoadString(IDS_STRING441);
		break;
	}
	myPopup.pszText = strRes;
	return ::HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}

BOOL CKPInversePg1A::OnHelpInfo(HELPINFO* pHelpInfo) 
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
	case IDC_MSHFG_NUM_DESC :
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
			strRes.LoadString(IDS_INVERSE_NUM_264);
			break;
		case 2 :
			// Optional comment that describes the %1.
			AfxFormatString1(strRes, IDS_DESC_STR_142, _T("inverse-modeling calculation") ); 
			break;
		}
		break;
	case IDC_MSHFG_INITSOLNS :
		if (!IsContextHelp())
		{
			if (!(m_egInitSolns.GetRowIsVisible(m_egInitSolns.GetRow()) && m_egInitSolns.GetColIsVisible(m_egInitSolns.GetCol())))
			{
				::MessageBeep((UINT)-1);
				return TRUE;
			}

			// modify placement
			CDC* pDC = m_egInitSolns.GetDC();
			int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
			int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

			long nLeft = ::MulDiv(m_egInitSolns.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
			long nTop  = ::MulDiv(m_egInitSolns.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

			CRect rc;
			m_egInitSolns.GetWindowRect(rc);

			myPopup.pt.x = rc.left + nLeft;
			myPopup.pt.y = rc.top + nTop + 10;
		}
		switch (IsContextHelp() ? m_egInitSolns.GetMouseCol() : m_egInitSolns.GetCol())
		{
		case 1 :
			strRes.LoadString(IDS_INVERSE_265);
			break;
		case 2 :
			strRes.LoadString(IDS_INVERSE_266);
			break;
		case 3 :
			strRes.LoadString(IDS_INVERSE_268);
			break;
		default:
			// No help topic is associated with this item. 
			strRes.LoadString(IDS_STRING441);
			break;
		}
		break;
	case IDC_MSHFG_FINALSOLN :
		if (!IsContextHelp())
		{
			if (!(m_egFinalSoln.GetRowIsVisible(m_egFinalSoln.GetRow()) && m_egFinalSoln.GetColIsVisible(m_egFinalSoln.GetCol())))
			{
				::MessageBeep((UINT)-1);
				return TRUE;
			}

			// modify placement
			CDC* pDC = m_egFinalSoln.GetDC();
			int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
			int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

			long nLeft = ::MulDiv(m_egFinalSoln.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
			long nTop  = ::MulDiv(m_egFinalSoln.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

			CRect rc;
			m_egFinalSoln.GetWindowRect(rc);

			myPopup.pt.x = rc.left + nLeft;
			myPopup.pt.y = rc.top + nTop + 10;
		}
		switch (IsContextHelp() ? m_egFinalSoln.GetMouseCol() : m_egFinalSoln.GetCol())
		{
		case 0 :
			strRes.LoadString(IDS_INVERSE_265);
			break;
		case 1 :
			strRes.LoadString(IDS_INVERSE_266);
			break;
		case 2 :
			strRes.LoadString(IDS_INVERSE_268);
			break;
		default:
			// No help topic is associated with this item. 
			strRes.LoadString(IDS_STRING441);
			break;
		}
		break;
	case IDC_E_TOL: case IDC_S_TOL:
		strRes.LoadString(IDS_INVERSE_279);
		break;
	case IDC_CLC_INITSOLNS:
		AfxFormatString1(strRes, IDS_CL_167, _T("solution number") ); 
		break;
	case IDC_CLC_FINALSOLN:
		AfxFormatString1(strRes, IDS_CL_167, _T("solution number") ); 
		break;
	case IDC_E_RANGE:
		strRes.LoadString(IDS_STRING497);
		break;
	case IDC_B_RANGE:
		strRes.LoadString(IDS_INVERSE_273);
		break;
	case IDC_B_MINIMAL:
		strRes.LoadString(IDS_INVERSE_281);
		break;
	case IDC_B_MINERAL_WATER:
		strRes.LoadString(IDS_INVERSE_280);
		break;
	case IDC_MP_SOLVE_CHECK:
		strRes.LoadString(IDS_STRING636);
		break;
	case IDC_S_MP_TOL: case IDC_E_MP_TOL:
		strRes.LoadString(IDS_STRING637);		
		break;
	case IDC_S_MP_CENSOR: case IDC_E_MP_CENSOR:
		strRes.LoadString(IDS_STRING638);		
		break;
	case IDC_B_WRITE_LON:
		strRes.LoadString(IDS_STRING639);		
		break;	
	case IDC_E_LON_PREFIX:
		strRes.LoadString(IDS_STRING640);		
		break;		
	case IDC_B_WRITE_PAT: 		
		strRes.LoadString(IDS_STRING641);		
		break;
	case IDC_E_PAT_PREFIX:
		strRes.LoadString(IDS_STRING642);		
		break;
	default:
		// No help topic is associated with this item. 
		strRes.LoadString(IDS_STRING441);
		break;
	}
	myPopup.pszText = strRes;
	return ::HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}

void CKPInversePg1A::OnSetfocusERange() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING497);
	m_eInputDesc.SetWindowText(strRes);
}

void CKPInversePg1A::OnMPSolveCheck() 
{
	int Ids[] = 
	{
		IDC_S_MP_TOL,
		IDC_E_MP_TOL,
		IDC_S_MP_CENSOR,
		IDC_E_MP_CENSOR,
	};

	for (int i = 0; i < sizeof(Ids) / sizeof(Ids[0]); ++i)
	{
		if (CWnd *pWnd = this->GetDlgItem(Ids[i]))
		{
			pWnd->EnableWindow(this->IsDlgButtonChecked(IDC_MP_SOLVE_CHECK) == BST_CHECKED);
		}
	}

}

void CKPInversePg1A::OnSetfocusEMPTol() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING624);
	m_eInputDesc.SetWindowText(strRes);
}

void CKPInversePg1A::OnSetfocusEMPCensor() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING625);
	m_eInputDesc.SetWindowText(strRes);
}

void CKPInversePg1A::OnBWriteLon() 
{
	int Ids[] = 
	{
		IDC_E_LON_PREFIX,
	};

	for (int i = 0; i < sizeof(Ids) / sizeof(Ids[0]); ++i)
	{
		if (CWnd *pWnd = this->GetDlgItem(Ids[i]))
		{
			pWnd->EnableWindow(this->IsDlgButtonChecked(IDC_B_WRITE_LON) == BST_CHECKED);
		}
	}
	CString strRes;
	strRes.LoadString(IDS_STRING639);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPInversePg1A::OnBWritePat() 
{
	int Ids[] = 
	{
		IDC_E_PAT_PREFIX,
	};

	for (int i = 0; i < sizeof(Ids) / sizeof(Ids[0]); ++i)
	{
		if (CWnd *pWnd = this->GetDlgItem(Ids[i]))
		{
			pWnd->EnableWindow(this->IsDlgButtonChecked(IDC_B_WRITE_PAT) == BST_CHECKED);
		}
	}
	CString strRes;
	strRes.LoadString(IDS_STRING641);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPInversePg1A::OnSetfocusELonPrefix()
{
	CString strRes;
	strRes.LoadString(IDS_STRING640);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPInversePg1A::OnSetfocusEPatPrefix()
{
	CString strRes;
	strRes.LoadString(IDS_STRING642);
	m_eInputDesc.SetWindowText(strRes);	
}
