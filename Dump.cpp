// Dump.cpp : implementation file
//

#include "stdafx.h"
#include "phreeqci2.h"
#include "Dump.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDump

IMPLEMENT_DYNAMIC(CDump, baseDump)

CDump::CDump(CWnd* pWndParent, CTreeCtrlNode simNode)
	 : baseDump(IDS_PROPSHT_CAPTION_163, pWndParent), m_Page1(simNode)
{
	m_bNeedDatabase = false;
	// Add all of the property pages here.  Note that
	// the order that they appear in here will be
	// the order they appear in on screen.  By default,
	// the first page of the set is the active one.
	// One way to make a different property page the
	// active one is to call SetActivePage().

	AddPage(&m_Page1);
}


CDump::~CDump()
{
}


BEGIN_MESSAGE_MAP(CDump, baseDump)
	//{{AFX_MSG_MAP(CDump)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDump message handlers

CString CDump::GetString()
{
	// COPY keyword index index_start[-index_end]
	//
	CString strLines = CKeyword::GetString(CKeyword::K_DUMP);

	CString strLine;
	CString strKey;
	CString strRange;

	// file
	strLine.Format(_T("%s%4c-%-20s %s"),
		(LPCTSTR)s_strNewLine,
		_T(' '),
		_T("file"),
		this->m_Page1.dump_info.Get_file_name().c_str()
		);
	strLines += strLine;

	// append
	strLine.Format(_T("%s%4c-%-20s %s"),
		(LPCTSTR)s_strNewLine,
		_T(' '),
		_T("append"),
		this->m_Page1.dump_info.Get_append() ? _T("true") : _T("false")
		);
	strLines += strLine;

	if (this->m_Page1.bAll)
	{
		// all
		strLine.Format(_T("%s%4c-%s"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			_T("all")
			);
		strLines += strLine;
		return strLines + s_strNewLine;
	}

	StorageBinList dinfo = this->m_Page1.dump_info.Get_StorageBinList();
	std::set<int> cells = CDumpPg1::GetCells(dinfo);

	// equilibrium_phases
	strLines += this->GetLine(CKeyword::K_EQUILIBRIUM_PHASES, dinfo.Get_pp_assemblage());

	// exchange
	strLines += this->GetLine(CKeyword::K_EXCHANGE, dinfo.Get_exchange());

	// gas_phase
	strLines += this->GetLine(CKeyword::K_GAS_PHASE, dinfo.Get_gas_phase());

	// kinetics
	strLines += this->GetLine(CKeyword::K_KINETICS, dinfo.Get_kinetics());

	// mix
	strLines += this->GetLine(CKeyword::K_MIX, dinfo.Get_mix());

	// reaction
	strLines += this->GetLine(CKeyword::K_REACTION, dinfo.Get_reaction());

	// reaction_pressure
	strLines += this->GetLine(CKeyword::K_REACTION_PRESSURE, dinfo.Get_pressure());

	// reaction_temperature
	strLines += this->GetLine(CKeyword::K_REACTION_TEMPERATURE, dinfo.Get_temperature());

	// ss_assemblage
	strLines += this->GetLine(CKeyword::K_SOLID_SOLUTIONS, dinfo.Get_ss_assemblage());

	// solution
	strLines += this->GetLine(CKeyword::K_SOLUTION, dinfo.Get_solution());

	// surface
	strLines += this->GetLine(CKeyword::K_SURFACE, dinfo.Get_surface());

	// cell
	if (cells.size())
	{
		strKey = CKeyword::GetString2(CKeyword::K_CELL).MakeLower();
		strRange = CDumpPg1::GetRanges(cells);
		strLine.Format(_T("%s%4c-%-20s %s"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			(LPCTSTR)strKey,
			(LPCTSTR)strRange
			);
		strLines += strLine;
	}

	return strLines + s_strNewLine;
}

CString CDump::GetLine(CKeyword::type kt, StorageBinListItem& item)
{
	CString strLine;
	if (item.Get_defined())
	{
		CString strKey = CKeyword::GetString2(kt).MakeLower();
		CString strRange = CDumpPg1::GetRanges(item.Get_numbers());
		if (strRange.IsEmpty())
		{
			strLine.Format(_T("%s%4c-%s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				(LPCTSTR)strKey
				);
		}
		else
		{
			strLine.Format(_T("%s%4c-%-20s %s"),
				(LPCTSTR)s_strNewLine,
				_T(' '),
				(LPCTSTR)strKey,
				(LPCTSTR)strRange
				);
		}
	}
	return strLine;
}

void CDump::Edit(CString& rStr)
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

void CDump::Edit(const CTreeCtrlNode& rKeyNode)
{
	ASSERT(m_editNode == 0);
	m_editNode = rKeyNode;
}
