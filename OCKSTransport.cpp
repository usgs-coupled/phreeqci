// OCKSTransport.cpp : implementation file
//
// $Id$
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
	 , m_pTransport(0)
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
	AddPage(&m_Page6);
}

COCKSTransport::~COCKSTransport()
{
	if (this->m_pTransport)
	{
		delete this->m_pTransport;
	}
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

	// Line 1:     -cells                 5
	if ((m_Page1.m_nCells > 0) && (this->m_pTransport->count_cells != (int)m_Page1.m_nCells))
	{
		strFormat.Format(_T("%s%4c-cells                 %d"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			m_Page1.m_nCells
			);
		strLines += strFormat;
	}

	// Line 2:     -shifts                25
	if ((m_Page1.m_nShifts > 0) && (this->m_pTransport->count_shifts != (int)m_Page1.m_nShifts))
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
		double dTime = m_Page1.m_dTimeStep;
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
		if (this->m_pTransport->timest != dTime)
		{
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
	}

	// Line 4:     -flow_direction        forward
	if (this->m_pTransport->shift != m_Page3.m_fdFD)
	{
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
	}

	// Line 5:     -boundary_conditions   flux constant
	if ((this->m_pTransport->bc_first != m_Page3.m_bcFirst) || (this->m_pTransport->bc_last != m_Page3.m_bcLast))
	{
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
	}

	// Line 6:     -lengths               4*1.0 2.0
	m_Page4.SetCells(m_Page1.m_nCells);
	std::list<CRepeat>::const_iterator clrIterPrev = this->m_pTransport->lengths_list.begin();
	std::list<CRepeat>::const_iterator clrIter     = m_Page4.m_lrLengths.begin();
	if (clrIter != m_Page4.m_lrLengths.end())
	{
		bool changed = false;
		if (m_Page4.m_lrLengths.size() != this->m_pTransport->lengths_list.size())
		{
			changed = true;
		}
		for (; !changed && clrIter != m_Page4.m_lrLengths.end(); ++clrIter, ++clrIterPrev)
		{
			if (clrIter->toCString().Compare(clrIterPrev->toCString()) != 0)
			{
				changed = true;
			}
		}
		if (changed)
		{
			strFormat.Format(_T("%s%4c-lengths              "),
				(LPCTSTR)s_strNewLine,
				_T(' ')
				);
			strLines += strFormat;
			clrIter = m_Page4.m_lrLengths.begin();
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
		}
	}

	// Line 7:     -dispersivities        4*0.1 0.2
	clrIterPrev = this->m_pTransport->disp_list.begin();
	clrIter     = m_Page4.m_lrDisps.begin();
	if (clrIter != m_Page4.m_lrDisps.end())
	{
		bool changed = false;
		if (m_Page4.m_lrDisps.size() != this->m_pTransport->disp_list.size())
		{
			changed = true;
		}
		for (; !changed && clrIter != m_Page4.m_lrDisps.end(); ++clrIter, ++clrIterPrev)
		{
			if (clrIter->toCString().Compare(clrIterPrev->toCString()) != 0)
			{
				changed = true;
			}
		}
		if (changed)
		{
			strFormat.Format(_T("%s%4c-dispersivities       "),
				(LPCTSTR)s_strNewLine,
				_T(' ')
				);
			strLines += strFormat;
			clrIter = m_Page4.m_lrDisps.begin();
			for (int i = 0; clrIter != m_Page4.m_lrDisps.end(); ++clrIter, ++i)
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
		}
	}

	// Line 8:     -correct_disp          true
	if (m_Page3.m_bCorrectDisp != this->m_pTransport->correct_disp)
	{
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
			strFormat.Format(_T("%s%4c-correct_disp          false"),
				(LPCTSTR)s_strNewLine,
				_T(' ')
				);
			strLines += strFormat;
		}
	}

	// Line 9:     -diffusion_coefficient 1.0e-9
	if (m_Page3.m_dDiffCoef != this->m_pTransport->diffc)
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
	if ((int)m_Page5.m_nStagCells != this->m_pTransport->count_stag)
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
	if (m_Page3.m_dTRF != this->m_pTransport->tempr || m_Page3.m_dTDC != this->m_pTransport->heat_diffc)
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

	// Line 12:    -initial_time          1000
	if (m_Page1.m_dInitTime > 0)
	{
		double dTime = m_Page1.m_dInitTime;
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
		if (this->m_pTransport->initial_total_time != dTime)
		{
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
	}

	// Line 13:    -print_cells           1-3 5
	if (m_Page5.m_bUseStagnant)
	{
		m_Page2.SetCells(m_Page1.m_nCells, m_Page5.m_nStagCells);
	}
	else
	{
		m_Page2.SetCells(m_Page1.m_nCells, 0);
	}
	std::list<CRange>::const_iterator cIterPrev = this->m_pTransport->print_range_list.begin();
	std::list<CRange>::const_iterator cIter     = m_Page2.m_listPrintRange.begin();
	if (cIter != m_Page2.m_listPrintRange.end())
	{
		bool changed = false;
		if (m_Page2.m_listPrintRange.size() != this->m_pTransport->print_range_list.size())
		{
			changed = true;
		}
		for (; !changed && cIter != m_Page2.m_listPrintRange.end(); ++cIter, ++cIterPrev)
		{
			if (cIter->toCString().Compare(cIterPrev->toCString()) != 0)
			{
				changed = true;
			}
		}
		if (changed)
		{
			std::list<CRange>::const_iterator cIter = m_Page2.m_listPrintRange.begin();
			if (cIter != m_Page2.m_listPrintRange.end())
			{
				strFormat.Format(_T("%s%4c-print_cells          "),
					(LPCTSTR)s_strNewLine,
					_T(' ')
					);
				strLines += strFormat;
			}
			for (int i = 0; cIter != m_Page2.m_listPrintRange.end(); ++cIter, ++i)
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
		}
	}
	else
	{
		// no range so print all cells if more cells
		if ((int)m_Page1.m_nCells > this->m_pTransport->count_cells)
		{
			strFormat.Format(_T("%s%4c-print_cells          "),
				(LPCTSTR)s_strNewLine,
				_T(' ')
				);
			strLines += strFormat;

			CRange r(1, m_Page1.m_nCells);

			strFormat.Format(_T(" %s"), (LPCTSTR)r.toCString());
			strLines += strFormat;
		}
	}

	// Line 14:      -print_frequency 5
	if ((int)m_Page2.m_nPrintModulus != this->m_pTransport->print_modulus)
	{
		strFormat.Format(_T("%s%4c-print_frequency       %d"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			m_Page2.m_nPrintModulus
			);
		strLines += strFormat;
	}

	// Line 15:      -punch_cells 2-5
	cIterPrev = this->m_pTransport->punch_range_list.begin();
	cIter     = m_Page2.m_listPunchRange.begin();
	if (cIter != m_Page2.m_listPunchRange.end())
	{
		bool changed = false;
		if (m_Page2.m_listPunchRange.size() != this->m_pTransport->punch_range_list.size())
		{
			changed = true;
		}
		for (; !changed && cIter != m_Page2.m_listPunchRange.end(); ++cIter, ++cIterPrev)
		{
			if (cIter->toCString().Compare(cIterPrev->toCString()) != 0)
			{
				changed = true;
			}
		}
		if (changed)
		{
			if (cIter != m_Page2.m_listPunchRange.end())
			{
				strFormat.Format(_T("%s%4c-punch_cells          "),
					(LPCTSTR)s_strNewLine,
					_T(' ')
					);
				strLines += strFormat;
			}
			for (int i = 0; cIter != m_Page2.m_listPunchRange.end(); ++cIter, ++i)
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
		}
	}
	else
	{
		// no range so print all cells if more cells
		if ((int)m_Page1.m_nCells > this->m_pTransport->count_cells)
		{
			strFormat.Format(_T("%s%4c-punch_cells          "),
				(LPCTSTR)s_strNewLine,
				_T(' ')
				);
			strLines += strFormat;

			CRange r(1, m_Page1.m_nCells);

			strFormat.Format(_T(" %s"), (LPCTSTR)r.toCString());
			strLines += strFormat;
		}
	}

	// Line 16:      -punch_frequency 5
	if ((int)m_Page2.m_nPunchModulus != this->m_pTransport->punch_modulus)
	{
		strFormat.Format(_T("%s%4c-punch_frequency       %d"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			m_Page2.m_nPunchModulus
			);
		strLines += strFormat;
	}

	if (m_Page5.m_bDumpToFile != (this->m_pTransport->dump_in != 0))
	{
		if (m_Page5.m_bDumpToFile)
		{
			// Line 17:    -dump                  dump.file
			if (m_Page5.m_strDumpFileName != this->m_pTransport->dump_file_name)
			{
				strFormat.Format(_T("%s%4c-dump                  %s"),
					(LPCTSTR)s_strNewLine,
					_T(' '),
					(LPCTSTR)m_Page5.m_strDumpFileName
					);
				strLines += strFormat;
			}

			// Line 18:    -dump_frequency        10
			if ((int)m_Page5.m_nDumpModulus != this->m_pTransport->dump_modulus)
			{
				strFormat.Format(_T("%s%4c-dump_frequency        %d"),
					(LPCTSTR)s_strNewLine,
					_T(' '),
					m_Page5.m_nDumpModulus
					);
				strLines += strFormat;
			}

			// Line 19:    -dump_restart          20
			if ((int)m_Page5.m_nDumpRestart != this->m_pTransport->transport_start)
			{
				strFormat.Format(_T("%s%4c-dump_restart          %d"),
					(LPCTSTR)s_strNewLine,
					_T(' '),
					m_Page5.m_nDumpRestart
					);
				strLines += strFormat;
			}
		}
	}

	// Line 20:    -warnings              false
	if (m_Page1.m_bPrintWarnings != this->m_pTransport->transport_warnings)
	{
		if (m_Page1.m_bPrintWarnings)
		{
			strFormat.Format(_T("%s%4c-warnings              true"),
				(LPCTSTR)s_strNewLine,
				_T(' ')
				);
		}
		else
		{
			strFormat.Format(_T("%s%4c-warnings              false"),
				(LPCTSTR)s_strNewLine,
				_T(' ')
				);
		}
		strLines += strFormat;
	}

	// Multicomponent diffusion
	// Line 21: -multi_d true 1e-9 0.3 0.05 1.0
	if (m_Page6.m_bUseMCD != (this->m_pTransport->multi_Dflag != 0)
		||
		m_Page6.m_default_Dw != this->m_pTransport->default_Dw
		||
		m_Page6.m_multi_Dpor != this->m_pTransport->multi_Dpor
		||
		m_Page6.m_multi_Dpor_lim != this->m_pTransport->multi_Dpor_lim
		||
		m_Page6.m_multi_Dn != this->m_pTransport->multi_Dn
		)
	{
		if (m_Page6.m_bUseMCD)
		{
			strFormat.Format(_T("%s%4c-multi_d               true %.*g %.*g %.*g %.*g"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				DBL_DIG,
				m_Page6.m_default_Dw,
				DBL_DIG,
				m_Page6.m_multi_Dpor,
				DBL_DIG,
				m_Page6.m_multi_Dpor_lim,
				DBL_DIG,
				m_Page6.m_multi_Dn
				);
			strLines += strFormat;

			// Interlayer diffusion
			// Line 22: -interlayer_D true 0.09 0.01 150
			if (m_Page6.m_bUseID != (this->m_pTransport->interlayer_Dflag != 0)
				||
				m_Page6.m_interlayer_Dpor != this->m_pTransport->interlayer_Dpor
				||
				m_Page6.m_interlayer_Dpor_lim != this->m_pTransport->interlayer_Dpor_lim
				||
				m_Page6.m_interlayer_tortf != this->m_pTransport->interlayer_tortf
				)
			{

				if (m_Page6.m_bUseID)
				{
					strFormat.Format(_T("%s%4c-interlayer_D          true %.*g %.*g %.*g"),
						(LPCTSTR)s_strNewLine,
						_T(' '),
						DBL_DIG,
						m_Page6.m_interlayer_Dpor,
						DBL_DIG,
						m_Page6.m_interlayer_Dpor_lim,
						DBL_DIG,
						m_Page6.m_interlayer_tortf
						);
				}
				else
				{
					strFormat.Format(_T("%s%4c-interlayer_D          false"),
						(LPCTSTR)s_strNewLine,
						_T(' ')
						);
				}
				strLines += strFormat;
			}
		}
		else
		{
			if (this->m_pTransport->simul_tr > 1)
			{
				strFormat.Format(_T("%s%4c-multi_d               false"),
					(LPCTSTR)s_strNewLine,
					_T(' ')
					);
				strLines += strFormat;
			}
		}
	}

	return strLines + s_strNewLine;
}

void COCKSTransport::Edit2(CString& rStr, CString &rPrev)
{
	CString cat;
	if (rPrev.IsEmpty() || (rPrev.MakeUpper().Find(_T("TRANSPORT")) == -1))
	{
		if (rStr.MakeUpper().Find(_T("TRANSPORT")) != -1)
		{
			cat = rStr;
		}
		rPrev += s_strNewLine;
		rPrev += _T("TRANSPORT");
		rPrev += s_strNewLine;

		// store previous transport vars
		CKeywordLoader2 keywordLoader2(rPrev, true);
		ASSERT(m_pTransport == 0);
		m_pTransport = new CTransport();

		if (cat.IsEmpty())
		{
			cat = rPrev;
		}
	}
	else
	{
		// store previous transport vars
		CKeywordLoader2 keywordLoader2(rPrev, true);
		ASSERT(m_pTransport == 0);
		m_pTransport = new CTransport();

		cat = rPrev;
		cat += rStr;
	}
	CKeywordLoader2 keywordLoader2(cat, true);

	// Line 1:     -cells                 5
	m_Page1.m_nCells         = ::count_cells;

	// Line 2:     -shifts                25
	m_Page1.m_nShifts        = ::count_shifts;

	// Line 3:      -time_step 3.15e7 # seconds = 1 yr.
	m_Page1.m_dTimeStep      = ::timest;

	// Line 4:     -flow_direction        forward
	switch (::ishift)
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
		m_Page3.m_fdFD = CKPTransportPg3::FD_FORWARD;
		break;
	}

	// Line 5:     -boundary_conditions   flux constant
	switch (::bcon_first)
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
		m_Page3.m_bcFirst = CKPTransportPg3::BC_FLUX;
		break;
	}

	switch (::bcon_last)
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
		m_Page3.m_bcLast = CKPTransportPg3::BC_FLUX;
		break;
	}

	// Line 6:     -lengths               4*1.0 2.0
	// Line 7:     -dispersivities        4*0.1 0.2
	CRepeat rLength(::cell_data[0].length);
	CRepeat rDisp(::cell_data[0].disp);
	for (int i = 1; i < ::count_cells; ++i)
	{
		// lengths
		if (::cell_data[i].length == rLength.GetDValue())
		{
			rLength.Increment();
		}
		else
		{
			m_Page4.m_lrLengths.push_back(rLength);
			rLength = CRepeat(::cell_data[i].length);
		}

		// disps
		if (::cell_data[i].disp == rDisp.GetDValue())
		{
			rDisp.Increment();
		}
		else
		{
			m_Page4.m_lrDisps.push_back(rDisp);
			rDisp = CRepeat(::cell_data[i].disp);
		}
	}
	if (!(rLength.GetCount() == (size_t)::count_cells && rLength.GetDValue() == 1.0))
	{
		m_Page4.m_lrLengths.push_back(rLength);
	}
	if (!(rDisp.GetCount() == (size_t)::count_cells && rDisp.GetDValue() == 0.0))
	{
		m_Page4.m_lrDisps.push_back(rDisp);
	}

	// Line 8:     -correct_disp          true
	m_Page3.m_bCorrectDisp = ::correct_disp;

	// Line 9:     -diffusion_coefficient 1.0e-9
	m_Page3.m_dDiffCoef   = ::diffc;

	// Line 10:    -stagnant              1  6.8e-6   0.3   0.1
	m_Page5.m_bUseStagnant = (::stag_data->count_stag != 0);
	m_Page5.m_nStagCells   = ::stag_data->count_stag;
	m_Page5.m_dExchFactor  = ::stag_data->exch_f;
	m_Page5.m_dThetaM      = ::stag_data->th_m;
	m_Page5.m_dThetaIM     = ::stag_data->th_im;

	// Line 11:    -thermal_diffusion     3.0   0.5e-6
	m_Page3.m_bUseThermal = TRUE;        // (::tempr != 2.0 || ::heat_diffc != ::diffc);
	m_Page3.m_dTRF        = ::tempr;
	m_Page3.m_dTDC        = ::heat_diffc;

	// Line 12:    -initial_time          1000
	m_Page1.m_dInitTime      = ::initial_total_time;

	// determine max_cell (used for print_cells & punch_cells)
	int max_cell = ::count_cells;
	if (::stag_data->count_stag)
	{
		max_cell = (::stag_data->count_stag  + 1) * ::count_cells + 1;
	}

	// Line 13:    -print_cells           1-3 5
	{
		CRange rPrint;
		rPrint.nMin = -1;
		for (int i = 0; i < max_cell; ++i)
		{
			if (::cell_data[i].print == TRUE)
			{
				if (rPrint.nMin == -1)
				{
					rPrint.nMin = i + 1;
				}
				rPrint.nMax = i + 1;
			}
			else
			{
				if (rPrint.nMin != -1)
				{
					m_Page2.m_listPrintRange.push_back(rPrint);
					rPrint.nMin = -1;
				}
			}
		}
		if (rPrint.nMin != -1)
		{
			// No ranges if all cells are selected
			if (rPrint.nMin != 1 && rPrint.nMax != ::count_cells)
			{
				m_Page2.m_listPrintRange.push_back(rPrint);
			}
		}
	}

	// Line 14:    -print_frequency       5
	m_Page2.m_nPrintModulus = ::print_modulus;

	// Line 15:    -punch_cells           2-5
	{
		CRange rPunch;
		rPunch.nMin = -1;
		for (int i = 0; i < max_cell; ++i)
		{
			if (::cell_data[i].punch == TRUE)
			{
				if (rPunch.nMin == -1)
				{
					rPunch.nMin = i + 1;
				}
				rPunch.nMax = i + 1;
			}
			else
			{
				if (rPunch.nMin != -1)
				{
					m_Page2.m_listPunchRange.push_back(rPunch);
					rPunch.nMin = -1;
				}
			}
		}
		if (rPunch.nMin != -1)
		{
			// No ranges if all cells are selected
			if (rPunch.nMin != 1 && rPunch.nMax != ::count_cells)
			{
				m_Page2.m_listPunchRange.push_back(rPunch);
			}
		}
	}

	// Line 16:    -punch_frequency       5
	m_Page2.m_nPunchModulus = ::punch_modulus;

	// Line 17:    -dump                  dump.file
	m_Page5.m_bDumpToFile     = (::dump_in == TRUE);
	m_Page5.m_strDumpFileName = ::dump_file_name;

	// Line 18:    -dump_frequency        10
	m_Page5.m_nDumpModulus    = ::dump_modulus;

	// Line 19:    -dump_restart          20
	m_Page5.m_nDumpRestart    = ::transport_start;

	// Line 20:    -warnings              false
	m_Page1.m_bPrintWarnings = ::transport_warnings;

	// Multicomponent diffusion
	// Line 21: -multi_d true 1e-9 0.3 0.05 1.0
	m_Page6.m_bUseMCD = (::multi_Dflag != 0);
	//if (m_Page6.m_bUseMCD)
	{
		m_Page6.m_default_Dw     = ::default_Dw;
		m_Page6.m_multi_Dpor     = ::multi_Dpor;
		m_Page6.m_multi_Dpor_lim = ::multi_Dpor_lim;
		m_Page6.m_multi_Dn       = ::multi_Dn;
	}

	// Interlayer diffusion
	// Line 22: -interlayer_D true 0.09 0.01 150
	m_Page6.m_bUseID = (::interlayer_Dflag != 0);
	if (m_Page6.m_bUseID)
	{
		m_Page6.m_interlayer_Dpor     = ::interlayer_Dpor;
		m_Page6.m_interlayer_Dpor_lim = ::interlayer_Dpor_lim;
		m_Page6.m_interlayer_tortf    = ::interlayer_tortf;
	}
}

