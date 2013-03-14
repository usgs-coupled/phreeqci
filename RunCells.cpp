// RunCells.cpp : implementation file
//

#include "stdafx.h"
#include "phreeqci2.h"
#include "RunCells.h"

#include "DeletePg1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CRunCells

IMPLEMENT_DYNAMIC(CRunCells, baseRunCells)

CRunCells::CRunCells(CWnd* pParentWnd, UINT iSelectPage)
	:baseRunCells(IDS_PROPSHT_CAPTION_111, pParentWnd, iSelectPage)
{
	this->AddPage(&m_Page1);
}

CRunCells::~CRunCells()
{
}


BEGIN_MESSAGE_MAP(CRunCells, baseRunCells)
END_MESSAGE_MAP()


// CRunCells message handlers

CString CRunCells::GetString()
{
	char *units[] = {
		"seconds",
		"minutes",
		"hours",
		"days",
		"years"
	};

	// Line 0: RUN_CELLS
	// Line 1:     -cells 1 2
	// Line 2:            5-6
	// Line 2a:           7
	// Line 3:     -start_time 100 day
	// Line 4:     -time_step 10 day

	// Line 0
	CString strLines = CKeyword::GetString(CKeyword::K_RUN_CELLS);

	// Line 1-2a
	strLines += this->GetLine(this->m_Page1.Runner.Get_cells());

	// Line 3
	double st = this->m_Page1.Runner.Get_start_time();
	if (st != 0)
	{
		CString strLine;
		strLine.Format(_T("%s%4c-start_time   %g %s"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			st,
			units[this->m_Page1.m_tuInitTime]
			);
		strLines += strLine;
	}

	// Line 4
	double ts = this->m_Page1.Runner.Get_time_step();
	if (ts != 0)
	{
		CString strLine;
		strLine.Format(_T("%s%4c-time_step    %g %s"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			ts,
			units[this->m_Page1.m_tuTimeStep]
			);
		strLines += strLine;
	}
	return strLines + s_strNewLine;
}

CString CRunCells::GetLine(StorageBinListItem& item)
{
	CString strLine;
	if (item.Get_defined())
	{
		CString strRange = CDeletePg1::GetRanges(item.Get_numbers());
		if (strRange.IsEmpty())
		{
			strLine.Format(_T("%s%4c-cells"),
				(LPCTSTR)s_strNewLine,
				_T(' ')
				);
		}
		else
		{
			strLine.Format(_T("%s%4c-cells %s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				(LPCTSTR)strRange
				);
		}
	}
	return strLine;
}

void CRunCells::Edit(CString& rStr)
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
