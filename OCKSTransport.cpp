// OCKSTransport.cpp : implementation file
//
// $Date: 2004/10/23 01:35:36 $
// $Revision: 1.3 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "OCKSTransport.h"
#include "DefinedRanges.h"
#include "KeywordLoader2.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define DAYS_PER_YEAR 365
#define HOURS_PER_DAY 24
#define MINS_PER_HOUR 60
#define SECS_PER_MIN 60

/////////////////////////////////////////////////////////////////////////////
// COCKSTransport

IMPLEMENT_DYNAMIC(COCKSTransport, baseCOCKSTransport)

COCKSTransport::COCKSTransport(CWnd* pWndParent)
	 : baseCOCKSTransport(IDS_PROPSHT_CAPTION24, pWndParent)
{
	// Add all of the property pages here.  Note that
	// the order that they appear in here will be
	// the order they appear in on screen.  By default,
	// the first page of the set is the active one.
	// One way to make a different property page the 
	// active one is to call SetActivePage().

	m_bNeedDatabase = false;

	AddPage(&m_Page1);
	AddPage(&m_Page4);
	AddPage(&m_Page3);
	AddPage(&m_Page5);
	AddPage(&m_Page2);
}

COCKSTransport::~COCKSTransport()
{
}


BEGIN_MESSAGE_MAP(COCKSTransport, baseCOCKSTransport)
	//{{AFX_MSG_MAP(COCKSTransport)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COCKSTransport message handlers

CString COCKSTransport::GetString()
{
	/*
	Line 0: TRANSPORT
	Line 1:     -cells                 5
	Line 2:     -shifts                25
	Line 3:     -time_step             3.15e7
	Line 4:     -flow_direction        forward
	Line 5:     -boundary_conditions   flux constant
	Line 6:     -lengths               4*1.0 2.0
	Line 7:     -dispersivities        4*0.1 0.2
	Line 8:     -correct_disp          true
	Line 9:     -diffusion_coefficient 1.0e-9
	Line 10:    -stagnant              1  6.8e-6   0.3   0.1
	Line 11:    -thermal_diffusion     3.0   0.5e-6
	Line 12:    -initial_time          1000
	Line 13:    -print_cells           1-3 5
	Line 14:    -print_frequency       5
	Line 15:    -punch_cells           2-5
	Line 16:    -punch_frequency       5
	Line 17:    -dump                  dump.file
	Line 18:    -dump_frequency        10
	Line 19:    -dump_restart          20
	Line 20:    -warnings              false
	*/

	// Line 0
	CString strLines = _T("TRANSPORT");
	CString strFormat;

	// Line 1
	if (m_Page1.m_nCells > 0)
	{
		strFormat.Format(_T("%s%4c-cells                 %d"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			m_Page1.m_nCells
			);
		strLines += strFormat;
	}

	// Line 2
	if (m_Page1.m_nShifts > 0)
	{
		strFormat.Format(_T("%s%4c-shifts                %d"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			m_Page1.m_nShifts
			);
		strLines += strFormat;
	}

	// Line 3:      -time_step 3.15e7 # seconds = 1 yr.
	if (m_Page1.m_dTimeStep > 0)
	{
		double dTime = 0;
		char* pszUnits = 0;

		switch (m_Page1.m_tuTimeStep)
		{
		case CKPTransportPg1::TU_YEARS :
			dTime = m_Page1.m_dTimeStep * DAYS_PER_YEAR * HOURS_PER_DAY * MINS_PER_HOUR * SECS_PER_MIN;
			if (m_Page1.m_dTimeStep > 1.0)
			{
				pszUnits = _T("years");
			}
			else
			{
				pszUnits = _T("year");
			}
			break;
		case CKPTransportPg1::TU_DAYS :
			dTime = m_Page1.m_dTimeStep * HOURS_PER_DAY * MINS_PER_HOUR * SECS_PER_MIN;
			if (m_Page1.m_dTimeStep > 1.0)
			{
				pszUnits = _T("days");
			}
			else
			{
				pszUnits = _T("day");
			}
			break;
		case CKPTransportPg1::TU_HOURS :
			dTime = m_Page1.m_dTimeStep * MINS_PER_HOUR * SECS_PER_MIN;
			if (m_Page1.m_dTimeStep > 1.0)
			{
				pszUnits = _T("hours");
			}
			else
			{
				pszUnits = _T("hour");
			}
			break;
		case CKPTransportPg1::TU_MINUTES :
			dTime = m_Page1.m_dTimeStep * SECS_PER_MIN;
			if (m_Page1.m_dTimeStep > 1.0)
			{
				pszUnits = _T("minutes");
			}
			else
			{
				pszUnits = _T("minute");
			}
			break;
		}
		if (pszUnits)
		{
			strFormat.Format(_T("%s%4c-time_step             %.*g # seconds = %.*g %s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				DBL_DIG,
				dTime,
				DBL_DIG,
				m_Page1.m_dTimeStep,
				pszUnits
				);
		}
		else
		{
			strFormat.Format(_T("%s%4c-time_step             %.*g # seconds"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				DBL_DIG,
				m_Page1.m_dTimeStep
				);
		}
		strLines += strFormat;
	}

	// Line 4:     -flow_direction        forward
	CString strFD;
	switch (m_Page3.m_fdFD)
	{
	case CKPTransportPg3::FD_BACK:
		strFD = _T("back");
		break;
	case CKPTransportPg3::FD_DIFF_ONLY:
		strFD = _T("diffusion_only");
		break;
	case CKPTransportPg3::FD_FORWARD:
		strFD = _T("forward");
		break;
	default:
		ASSERT(FALSE); // need to add additional FD
	}
	strFormat.Format(_T("%s%4c-flow_direction        %s"),
		(LPCTSTR)s_strNewLine,
		_T(' '),
		LPCTSTR(strFD)
		);
	strLines += strFormat;


	// Line 5:     -boundary_conditions   flux constant
	CString strBC;
	switch (m_Page3.m_bcFirst)
	{
	case CKPTransportPg3::BC_CONSTANT:
		strBC = _T("constant");
		break;
	case CKPTransportPg3::BC_CLOSED:
		strBC = _T("closed");
		break;
	case CKPTransportPg3::BC_FLUX:
		strBC = _T("flux");
		break;
	default:
		ASSERT(FALSE); // need to add additional BC
	}
	switch (m_Page3.m_bcLast)
	{
	case CKPTransportPg3::BC_CONSTANT:
		strBC += _T(" constant");
		break;
	case CKPTransportPg3::BC_CLOSED:
		strBC += _T(" closed");
		break;
	case CKPTransportPg3::BC_FLUX:
		strBC += _T(" flux");
		break;
	default:
		ASSERT(FALSE); // need to add additional BC
	}
	strFormat.Format(_T("%s%4c-boundary_conditions   %s"),
		(LPCTSTR)s_strNewLine,
		_T(' '),
		LPCTSTR(strBC)
		);
	strLines += strFormat;


	// Line 6:     -lengths               4*1.0 2.0
	m_Page4.SetCells(m_Page1.m_nCells);
	std::list<CRepeat>::const_iterator clrIter = m_Page4.m_lrLengths.begin();
	if (clrIter == m_Page4.m_lrLengths.end())
	{
		// do nothing
	}
	else
	{
		strFormat.Format(_T("%s%4c-lengths              "),
			(LPCTSTR)s_strNewLine,
			_T(' ')
			);
		strLines += strFormat;
	}
	for (int i = 0; clrIter != m_Page4.m_lrLengths.end(); ++clrIter, ++i)
	{
		if ((i % 7) || i == 0)
		{
			strFormat.Format(_T(" %s"), (LPCTSTR)clrIter->toCString());
		}
		else
		{
			strFormat.Format(_T("%s%4c                       %s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				(LPCTSTR)(LPCTSTR)clrIter->toCString()
				);
		}
		strLines += strFormat;
	}

	// Line 7:     -dispersivities        4*0.1 0.2
	clrIter = m_Page4.m_lrDisps.begin();
	if (clrIter == m_Page4.m_lrDisps.end())
	{
		// do nothing
	}
	else
	{
		strFormat.Format(_T("%s%4c-dispersivities       "),
			(LPCTSTR)s_strNewLine,
			_T(' ')
			);
		strLines += strFormat;
	}
	for (i = 0; clrIter != m_Page4.m_lrDisps.end(); ++clrIter, ++i)
	{
		if ((i % 7) || i == 0)
		{
			strFormat.Format(_T(" %s"), (LPCTSTR)clrIter->toCString());
		}
		else
		{
			strFormat.Format(_T("%s%4c                       %s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				(LPCTSTR)(LPCTSTR)clrIter->toCString()
				);
		}
		strLines += strFormat;
	}

	// Line 8:     -correct_disp          true
	if (m_Page3.m_bCorrectDisp)
	{
		strFormat.Format(_T("%s%4c-correct_disp          true"),
			(LPCTSTR)s_strNewLine,
			_T(' ')
			);
		strLines += strFormat;
	}
	else
	{
		/*
		strFormat.Format(_T("%s%4c-correct_disp          false"),
			(LPCTSTR)s_strNewLine,
			_T(' ')
			);
		strLines += strFormat;
		*/
	}

	// Line 9:     -diffusion_coefficient 1.0e-9
	if (m_Page3.m_dDiffCoef != 0.3e-9)
	{
		strFormat.Format(_T("%s%4c-diffusion_coefficient %.*g"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			DBL_DIG,
			m_Page3.m_dDiffCoef
			);
		strLines += strFormat;
	}

	// Line 10:    -stagnant              1  6.8e-6   0.3   0.1
	if (m_Page5.m_bUseStagnant && m_Page5.m_nStagCells != 0)
	{
		if (m_Page5.m_nStagCells == 1)
		{
			strFormat.Format(_T("%s%4c-stagnant              %d  %.*g  %.*g  %.*g"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				m_Page5.m_nStagCells,
				DBL_DIG,
				m_Page5.m_dExchFactor,
				DBL_DIG,
				m_Page5.m_dThetaM,
				DBL_DIG,
				m_Page5.m_dThetaIM
				);
		}
		else
		{
			strFormat.Format(_T("%s%4c-stagnant              %d"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				m_Page5.m_nStagCells
				);
		}
		strLines += strFormat;
	}


	// Line 11:    -thermal_diffusion     3.0   0.5e-6
	if (m_Page3.m_bUseThermal /* && (m_Page3.m_dTRF != 2.0 || m_Page3.m_dTDC != m_Page3.m_dDiffCoef) */)
	{
		strFormat.Format(_T("%s%4c-thermal_diffusion     %.*g   %.*g"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			DBL_DIG,
			m_Page3.m_dTRF,
			DBL_DIG,
			m_Page3.m_dTDC
			);
		strLines += strFormat;
	}

	// Line 12
	if (m_Page1.m_dInitTime > 0)
	{
		double dTime = 0;
		char* pszUnits = 0;

		switch (m_Page1.m_tuInitTime)
		{
		case CKPTransportPg1::TU_YEARS :
			dTime = m_Page1.m_dInitTime * DAYS_PER_YEAR * HOURS_PER_DAY * MINS_PER_HOUR * SECS_PER_MIN;
			if (m_Page1.m_dInitTime > 1.0)
			{
				pszUnits = _T("years");
			}
			else
			{
				pszUnits = _T("year");
			}
			break;
		case CKPTransportPg1::TU_DAYS :
			dTime = m_Page1.m_dInitTime * HOURS_PER_DAY * MINS_PER_HOUR * SECS_PER_MIN;
			if (m_Page1.m_dInitTime > 1.0)
			{
				pszUnits = _T("days");
			}
			else
			{
				pszUnits = _T("day");
			}
			break;
		case CKPTransportPg1::TU_HOURS :
			dTime = m_Page1.m_dInitTime * MINS_PER_HOUR * SECS_PER_MIN;
			if (m_Page1.m_dInitTime > 1.0)
			{
				pszUnits = _T("hours");
			}
			else
			{
				pszUnits = _T("hour");
			}
			break;
		case CKPTransportPg1::TU_MINUTES :
			dTime = m_Page1.m_dInitTime * SECS_PER_MIN;
			if (m_Page1.m_dInitTime > 1.0)
			{
				pszUnits = _T("minutes");
			}
			else
			{
				pszUnits = _T("minute");
			}
			break;
		}
		if (pszUnits)
		{
			strFormat.Format(_T("%s%4c-initial_time          %.*g # seconds = %.*g %s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				DBL_DIG,
				dTime,
				DBL_DIG,
				m_Page1.m_dInitTime,
				pszUnits
				);
		}
		else
		{
			strFormat.Format(_T("%s%4c-initial_time          %.*g # seconds"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				DBL_DIG,
				m_Page1.m_dInitTime
				);
		}
		strLines += strFormat;
	}

	// Line 13
// COMMENT: {3/1/2004 7:28:37 PM}	m_Page2.SetCells(m_Page1.m_nCells);
	//{{3/1/2004 7:28:37 PM
	if (m_Page5.m_bUseStagnant) {
		// m_Page2.SetCells((m_Page5.m_nStagCells + 1) * m_Page1.m_nCells + 1);
		m_Page2.SetCells(m_Page1.m_nCells, m_Page5.m_nStagCells);
	}
	else {
		m_Page2.SetCells(m_Page1.m_nCells, 0);
	}
	//}}3/1/2004 7:28:37 PM
	std::list<CRange>::const_iterator cIter = m_Page2.m_listPrintRange.begin();
	if (cIter != m_Page2.m_listPrintRange.end())
	{
		strFormat.Format(_T("%s%4c-print_cells          "),
			(LPCTSTR)s_strNewLine,
			_T(' ')
			);
		strLines += strFormat;
	}
	for (i = 0; cIter != m_Page2.m_listPrintRange.end(); ++cIter, ++i)
	{
		if ((i % 5) || i == 0)
		{
			strFormat.Format(_T(" %s"), (LPCTSTR)cIter->toCString());
		}
		else
		{
			strFormat.Format(_T("%s%4c                       %s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				(LPCTSTR)(LPCTSTR)cIter->toCString()
				);
		}
		strLines += strFormat;
	}

	// Line 14:      -print_frequency 5
	if (m_Page2.m_nPrintModulus > 1)
	{
		strFormat.Format(_T("%s%4c-print_frequency       %d"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			m_Page2.m_nPrintModulus
			);
		strLines += strFormat;
	}

	// Line 15:      -punch_cells 2-5
	cIter = m_Page2.m_listPunchRange.begin();
	if (cIter != m_Page2.m_listPunchRange.end())
	{
		strFormat.Format(_T("%s%4c-punch_cells          "),
			(LPCTSTR)s_strNewLine,
			_T(' ')
			);
		strLines += strFormat;
	}
	for (i = 0; cIter != m_Page2.m_listPunchRange.end(); ++cIter, ++i)
	{
		if ((i % 5) || i == 0)
		{
			strFormat.Format(_T(" %s"), (LPCTSTR)cIter->toCString());
		}
		else
		{
			strFormat.Format(_T("%s%4c                       %s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				(LPCTSTR)(LPCTSTR)cIter->toCString()
				);
		}
		strLines += strFormat;
	}

	// Line 16:      -punch_frequency 5
	if (m_Page2.m_nPunchModulus > 1)
	{
		strFormat.Format(_T("%s%4c-punch_frequency       %d"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			m_Page2.m_nPunchModulus
			);
		strLines += strFormat;
	}

	if (m_Page5.m_bDumpToFile)
	{
		// Line 17:    -dump                  dump.file
		strFormat.Format(_T("%s%4c-dump                  %s"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			(LPCTSTR)m_Page5.m_strDumpFileName
			);
		strLines += strFormat;

		// Line 18:    -dump_frequency        10
		strFormat.Format(_T("%s%4c-dump_frequency        %d"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			m_Page5.m_nDumpModulus
			);
		strLines += strFormat;

		// Line 19:    -dump_restart          20
		strFormat.Format(_T("%s%4c-dump_restart          %d"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			m_Page5.m_nDumpRestart
			);
		strLines += strFormat;
	}

	// Line 20
	if (m_Page1.m_bPrintWarnings)
	{
		strFormat.Format(_T("%s%4c-warnings              true"),
			(LPCTSTR)s_strNewLine,
			_T(' ')
			);
		strLines += strFormat;
	}
	else
	{
		strFormat.Format(_T("%s%4c-warnings              false"),
			(LPCTSTR)s_strNewLine,
			_T(' ')
			);
		strLines += strFormat;
	}

	return strLines + s_strNewLine;
}

void COCKSTransport::Edit(CString& rStr)
{
	CKeywordLoader2 keywordLoader2(rStr);

	m_Page1.m_nCells         = count_cells;        // count_ad_cells;
	m_Page1.m_nShifts        = count_shifts;       // count_ad_shifts;
	m_Page1.m_dTimeStep      = timest;             // advection_kin_time;
	m_Page1.m_dInitTime      = initial_total_time; // initial_total_time; 
	m_Page1.m_bPrintWarnings = transport_warnings; // advection_warnings;

	CRange range;
	range.nMin = -1;
// COMMENT: {3/1/2004 7:59:59 PM}	for (int i = 0; i < count_cells; ++i)
	int max_cell;
	if (stag_data->count_stag) {
		max_cell = (stag_data->count_stag  + 1) * count_cells + 1;
	}
	else {
		max_cell = count_cells;
	}
	for (int i = 0; i < max_cell; ++i)
	{
		if (cell_data[i].punch == TRUE)
		{
			if (range.nMin == -1)
			{
				range.nMin = i + 1;
			}
			range.nMax = i + 1;
		}
		else
		{
			if (range.nMin != -1)
			{
				m_Page2.m_listPunchRange.push_back(range);
				range.nMin = -1;
			}
		}
	}
	if (range.nMin != -1)
	{
		// No ranges if all cells are selected
		if (range.nMin != 1 && range.nMax != count_ad_cells)
		{
			m_Page2.m_listPunchRange.push_back(range);
		}
	}

	m_Page2.m_nPrintModulus = print_modulus;

	range.nMin = -1;
// COMMENT: {3/1/2004 8:07:32 PM}	for (i = 0; i < count_cells; ++i)
	if (stag_data->count_stag) {
		max_cell = (stag_data->count_stag  + 1) * count_cells + 1;
	}
	else {
		max_cell = count_cells;
	}
	for (i = 0; i < max_cell; ++i)
	{
		if (cell_data[i].print == TRUE)
		{
			if (range.nMin == -1)
			{
				range.nMin = i + 1;
			}
			range.nMax = i + 1;
		}
		else
		{
			if (range.nMin != -1)
			{
				m_Page2.m_listPrintRange.push_back(range);
				range.nMin = -1;
			}
		}
	}
	if (range.nMin != -1)
	{
		// No ranges if all cells are selected
		if (range.nMin != 1 && range.nMax != count_ad_cells)
		{
			m_Page2.m_listPrintRange.push_back(range);
		}
	}

	m_Page2.m_nPunchModulus = punch_modulus;

	// Page 3 Boundary Conditions
	switch (bcon_first)
	{
	case 1:
		m_Page3.m_bcFirst = CKPTransportPg3::BC_CONSTANT;
		break;
	case 2:
		m_Page3.m_bcFirst = CKPTransportPg3::BC_CLOSED;
		break;
	case 3:
		m_Page3.m_bcFirst = CKPTransportPg3::BC_FLUX;
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	switch (bcon_last)
	{
	case 1:
		m_Page3.m_bcLast = CKPTransportPg3::BC_CONSTANT;
		break;
	case 2:
		m_Page3.m_bcLast = CKPTransportPg3::BC_CLOSED;
		break;
	case 3:
		m_Page3.m_bcLast = CKPTransportPg3::BC_FLUX;
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	m_Page3.m_bCorrectDisp = correct_disp;

	// Flow conditions

	switch (ishift)
	{
	case -1:
		m_Page3.m_fdFD = CKPTransportPg3::FD_BACK;
		break;
	case 0:
		m_Page3.m_fdFD = CKPTransportPg3::FD_DIFF_ONLY;
		break;
	case 1:
		m_Page3.m_fdFD = CKPTransportPg3::FD_FORWARD;
		break;
	default:
		ASSERT(FALSE);
		break;
	}
	m_Page3.m_dDiffCoef   = diffc;
	m_Page3.m_bUseThermal = (tempr != 2.0 || heat_diffc != heat_diffc);
	m_Page3.m_dTRF        = tempr;
	m_Page3.m_dTDC        = heat_diffc;


	// Page 4
	CRepeat rLength(cell_data[0].length);
	CRepeat rDisp(cell_data[0].disp);
	for (i = 1; i < count_cells; ++i)
	{
		// lengths
		if (cell_data[i].length == rLength.GetDValue())
		{
			rLength.Increment();
		}
		else
		{
			m_Page4.m_lrLengths.push_back(rLength);
			rLength = CRepeat(cell_data[i].length);
		}

		// disps
		if (cell_data[i].disp == rDisp.GetDValue())
		{
			rDisp.Increment();
		}
		else
		{
			m_Page4.m_lrDisps.push_back(rDisp);
			rDisp = CRepeat(cell_data[i].disp);
		}
	}
	if (!(rLength.GetCount() == (size_t)count_cells && rLength.GetDValue() == 1.0))
	{
		m_Page4.m_lrLengths.push_back(rLength);
	}
	if (!(rDisp.GetCount() == (size_t)count_cells && rDisp.GetDValue() == 0.0))
	{
		m_Page4.m_lrDisps.push_back(rDisp);
	}

	// Page 5 Stagnant/Dump options
	m_Page5.m_nStagCells   = stag_data->count_stag;
	m_Page5.m_dExchFactor  = stag_data->exch_f;
	m_Page5.m_dThetaM      = stag_data->th_m;
	m_Page5.m_dThetaIM     = stag_data->th_im;
	m_Page5.m_bUseStagnant = (stag_data->count_stag != 0);

	m_Page5.m_bDumpToFile     = (dump_in == TRUE);
	m_Page5.m_strDumpFileName = dump_file_name;
	m_Page5.m_nDumpModulus    = dump_modulus;
	m_Page5.m_nDumpRestart    = transport_start;
}

