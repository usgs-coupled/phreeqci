// UserGraph.cpp : implementation file
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
///#include "phrqtype.h"          // LDBLE
///#include "phreeqc.h"           // user_punch user_punch_count_headings user_punch_headings
#include "UserGraph.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserGraph

IMPLEMENT_DYNAMIC(CUserGraph, baseUserGraph)

CUserGraph::CUserGraph(CWnd* pWndParent)
	 : baseUserGraph(IDS_STRING710, pWndParent)
{
	// Add all of the property pages here.  Note that
	// the order that they appear in here will be
	// the order they appear in on screen.  By default,
	// the first page of the set is the active one.
	// One way to make a different property page the
	// active one is to call SetActivePage().

	AddPage(&m_Page1);
// COMMENT: {5/2/2012 10:10:59 PM}	AddPage(&m_Page2);
	m_pPage2 = new CUserGraphPg2;
	AddPage(m_pPage2);
}

CUserGraph::~CUserGraph()
{
	ASSERT(m_pPage2);
	if (m_pPage2)
	{
		delete m_pPage2;
		m_pPage2 = NULL;
	}
}


BEGIN_MESSAGE_MAP(CUserGraph, baseUserGraph)
	//{{AFX_MSG_MAP(CUserGraph)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CUserGraph message handlers
/////////////////////////////////////////////////////////////////////////////
// COCKSRates message handlers

CString CUserGraph::GetString()
{
	/*
	Line 0: USER_GRAPH 2 Plots F and pH against Ca concentration
	Line 1:     -headings                F pH
	Line 2:     -axis_titles             "Ca /(mg/L)" "F /(mg/L)" "pH"
	Line 3:     -chart_title             "Fluorite Equilibrium"
	Line 4:     -axis_scale x_axis       0 350 50 25
	Line 4a:    -axis_scale y_axis       0 7 1
	Line 4b:    -axis_scale sy_axis      7 14 0 0
	Line 5:     -initial_solutions       true
	Line 6:     -connect_simulations     true
	Line 7:     -plot_concentration_vs   x
	Line 8:     -plot_csv_file           filename
	Line 9:     -start
	Line 10:  10 PLOT_XYTOT("Ca") * 40.08e3, TOT("F") * 19e3, color = Red, symbol = Square, symbol_size = 6, y-axis = 1
	Line 10a: 20 PLOT_XYTOT("Ca") * 40.08e3, -LA("H+"), color = Green, symbol = Diamond, symbol_size = 7, y-axis = 2, line_width 1
	Line 11:    -end

	Line 0a: USER_GRAPH 4
	Line 12:     -detach

	Line 0b: USER_GRAPH 1
	Line 13:     -active  false
	*/

	const int width = 23;

	CString strLines;
	CString strFormat;

	// Line 0
	this->m_n_user_end = CCommonKeywordSheet::N_EMPTY;
	strLines = this->GetLineZero(CKeyword::K_USER_GRAPH);

	if (this->m_Page1.m_detach)
	{
		// Line 12
		// -detach
		strFormat.Format(_T("%s%4c-detach"),
			(LPCTSTR)s_strNewLine,
			_T(' ')
			);
		strLines += strFormat;
	}
	else
	{

		// Line 1
		// -headings
		if (this->m_Page1.m_strHeadings.GetLength())
		{
			strFormat.Format(_T("%s%4c%-*s %s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				width,
				_T("-headings"),
				(LPCTSTR)this->m_Page1.m_strHeadings
				);
			strLines += strFormat;
		}


		// Line 2
		// -axis_titles
		if (this->m_Page1.axis_title_x.GetLength() || this->m_Page1.axis_title_y.GetLength() || this->m_Page1.axis_title_y2.GetLength())
		{
			strFormat.Format(_T("%s%4c%-*s \"%s\" \"%s\" \"%s\""),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				width,
				_T("-axis_titles"),
				(LPCTSTR)this->m_Page1.axis_title_x,
				(LPCTSTR)this->m_Page1.axis_title_y,
				(LPCTSTR)this->m_Page1.axis_title_y2
				);
			strLines += strFormat;
		}

		// Line 3
		// -chart_title
		if (this->m_Page1.chart_title.GetLength())
		{
			strFormat.Format(_T("%s%4c%-*s \"%s\""),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				width,
				_T("-chart_title"),
				(LPCTSTR)m_Page1.chart_title
				);
			strLines += strFormat;
		}

		// Line 4
		// -axis_scale x_axis
		if (!this->m_Page1.auto_min_x || !this->m_Page1.auto_max_x || !this->m_Page1.auto_maj_x || !this->m_Page1.auto_minor_x || this->m_Page1.log_x)
		{
			strFormat.Format(_T("%s%4c%-*s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				width,
				_T("-axis_scale x_axis")
				);
			strLines += strFormat;
			if (m_Page1.auto_min_x)
			{
				strFormat = " auto";
			}
			else
			{
				strFormat.Format(_T(" %g"), m_Page1.min_x);
			}
			strLines += strFormat;
			if (m_Page1.auto_max_x)
			{
				strFormat = " auto";
			}
			else
			{
				strFormat.Format(_T(" %g"), m_Page1.max_x);
			}
			strLines += strFormat;
			if (m_Page1.auto_maj_x)
			{
				strFormat = " auto";
			}
			else
			{
				strFormat.Format(_T(" %g"), m_Page1.maj_x);
			}
			strLines += strFormat;
			if (m_Page1.auto_minor_x)
			{
				strFormat = _T(" auto");
			}
			else
			{
				strFormat.Format(_T(" %g"), m_Page1.minor_x);
			}
			strLines += strFormat;

			if (m_Page1.log_x)
			{
				strLines += _T(" log");
			}
		}

		// Line 4a
		// -axis_scale y_axis
		if (!this->m_Page1.auto_min_y || !this->m_Page1.auto_max_y || !this->m_Page1.auto_maj_y || !this->m_Page1.auto_minor_y || this->m_Page1.log_y)
		{
			strFormat.Format(_T("%s%4c%-*s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				width,
				_T("-axis_scale y_axis")
				);
			strLines += strFormat;
			if (m_Page1.auto_min_y)
			{
				strFormat = " auto";
			}
			else
			{
				strFormat.Format(_T(" %g"), m_Page1.min_y);
			}
			strLines += strFormat;
			if (m_Page1.auto_max_y)
			{
				strFormat = " auto";
			}
			else
			{
				strFormat.Format(_T(" %g"), m_Page1.max_y);
			}
			strLines += strFormat;
			if (m_Page1.auto_maj_y)
			{
				strFormat = " auto";
			}
			else
			{
				strFormat.Format(_T(" %g"), m_Page1.maj_y);
			}
			strLines += strFormat;
			if (m_Page1.auto_minor_y)
			{
				strFormat = " auto";
			}
			else
			{
				strFormat.Format(_T(" %g"), m_Page1.minor_y);
			}
			strLines += strFormat;

			if (m_Page1.log_y)
			{
				strLines += _T(" log");
			}
		}

		// Line 4b
		// -axis_scale sy_axis
		if (!this->m_Page1.auto_min_y2 || !this->m_Page1.auto_max_y2 || !this->m_Page1.auto_maj_y2 || !this->m_Page1.auto_minor_y2 || this->m_Page1.log_y2)
		{
			strFormat.Format(_T("%s%4c%-*s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				width,
				_T("-axis_scale sy_axis")
				);
			strLines += strFormat;
			if (m_Page1.auto_min_y2)
			{
				strFormat = " auto";
			}
			else
			{
				strFormat.Format(_T(" %g"), m_Page1.min_y2);
			}
			strLines += strFormat;
			if (m_Page1.auto_max_y2)
			{
				strFormat = " auto";
			}
			else
			{
				strFormat.Format(_T(" %g"), m_Page1.max_y2);
			}
			strLines += strFormat;
			if (m_Page1.auto_maj_y2)
			{
				strFormat = " auto";
			}
			else
			{
				strFormat.Format(_T(" %g"), m_Page1.maj_y2);
			}
			strLines += strFormat;
			if (m_Page1.auto_minor_y2)
			{
				strFormat = " auto";
			}
			else
			{
				strFormat.Format(_T(" %g"), m_Page1.minor_y2);
			}
			strLines += strFormat;

			if (m_Page1.log_y2)
			{
				strLines += _T(" log");
			}
		}

		// Line 5
		// -initial_solutions
		//if (this->m_Page1.m_initial_solutions)
		{
			strFormat.Format(_T("%s%4c%-*s %s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				width,
				_T("-initial_solutions"),
				this->m_Page1.m_initial_solutions ? _T("true") : _T("false")
				);
			strLines += strFormat;
		}

		// Line 6
		// -connect_simulations
		//if (!this->m_Page1.m_connect_simulations)
		{
			strFormat.Format(_T("%s%4c%-*s %s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				width,
				_T("-connect_simulations"),
				this->m_Page1.m_connect_simulations ? _T("true") : _T("false")
				);
			strLines += strFormat;
		}

		// Line 7
		//if (this->m_Page1.m_chart_type != CUserGraphPg1::CT_CONC_VS_X)
		{
			strFormat.Format(_T("%s%4c%-*s %s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				width,
				_T("-plot_concentration_vs"),
				this->m_Page1.m_chart_type == CUserGraphPg1::CT_CONC_VS_T ? _T("t") : _T("x")
				);
			strLines += strFormat;
		}

		// Line 8
		ASSERT(this->m_pPage2);
		if (this->m_pPage2->csv_file_names.size() > 0)
		{
			for (size_t j = 0; j < this->m_pPage2->csv_file_names.size(); ++j)
			{
				strFormat.Format(_T("%s%4c%-*s %s"),
					(LPCTSTR)s_strNewLine,
					_T(' '),
					width,
					_T("-plot_csv_file"),
					this->m_pPage2->csv_file_names[j].c_str()
					);
				strLines += strFormat;
			}
		}

		// Line 9
		ASSERT(m_pPage2);
		if (m_pPage2->m_listCommands.size() > 0)
		{
			strFormat.Format(_T("%s%2c-start"),
				(LPCTSTR)s_strNewLine,
				_T(' ')
				);
			strLines += strFormat;
		}

		// Line 10
		// Basic
		ASSERT(m_pPage2);
		std::list<basic_command>::const_iterator command_iter = m_pPage2->m_listCommands.begin();
		for ( ; command_iter != m_pPage2->m_listCommands.end(); ++command_iter)
		{
			strFormat.Format(_T("%s%d %s"),
				(LPCTSTR)s_strNewLine,
				(*command_iter).nLine,
				(LPCTSTR)(*command_iter).strCommand
				);
			strLines += strFormat;
		}

		// Line 11
		ASSERT(m_pPage2);
		if (m_pPage2->m_listCommands.size() > 0)
		{
			strFormat.Format(_T("%s%2c-end"),
				(LPCTSTR)s_strNewLine,
				_T(' ')
				);
			strLines += strFormat;
		}

		// Line 13
		// -active
		//if (!this->m_Page1.m_active)
		{
			strFormat.Format(_T("%s%4c%-*s %s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				width,
				_T("-active"),
				this->m_Page1.m_active ? _T("true") : _T("false")
				);
			strLines += strFormat;
		}
	}

	return strLines + s_strNewLine;
}

void CUserGraph::Edit(CString& rStr)
{
	try
	{
		PhreeqcI p(rStr);
		p.GetData(this);
	}
	catch (...)
	{
		CString strResource;
		strResource.LoadString(IDS_EXCEPTION_ACCESS_VIOLATION);
		::MessageBox(NULL, strResource, _T("Unhandled Exception"), MB_OK|MB_ICONERROR);
	}
}
