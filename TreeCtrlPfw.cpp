// TreeCtrlPfw.cpp : implementation file
//

#include "stdafx.h"
#include "phreeqci2.h"
#include "TreeCtrlPfw.h"

#include "RichViewIn.h"

std::pair<CString, CString> CTreeCtrlPfw::COPY("COPY solution 5 1-10 # copies solution 5 to solution 1 to 10",
                                               "Copies a data entity, at the end of the simulation, to a new index number or a range of index numbers. Data entities can be: SOLUTION, EQUILIBRIUM_PHASES, EXCHANGE, GAS_PHASE, KINETICS, MIX, REACTION, REACTION_TEMPERATURE, SOLID_SOLUTION, SURFACE\n"
                                               "\n"
                                               "Example:\n"
                                               "COPY solution 5 1-10\n");
std::pair<CString, CString> CTreeCtrlPfw::DATABASE("DATABASE wateq4f.dat",
                                                   "Specifies the database to be used. It must be the first keyword in the input file.\n"
                                                   "\n"
                                                   "Example:\n"
                                                   "DATABASE wateq4f.dat\n");
std::pair<CString, CString> CTreeCtrlPfw::END("END",
                                              "Ends the data input for a simulation and signals PHREEQC to start calculating.\n"
                                              "\n"
                                              "Example:\n"
                                              "END\n");
std::pair<CString, CString> CTreeCtrlPfw::SAVE("SAVE solution 1-10",
                                               "Saves, at the end of the simulation, the solution, exchange, gas phase, pure-phase, solid-solution, or surface to a new index number or a range of cells.\n"
                                               "\n"
                                               "Example:\n"
                                               "SAVE solution 1-10\n");
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION("SOLUTION 1-10 # a number or a range of numbers. Default: 1.\n"
                                                   "pH 6.05 # Default: 7.\n"
                                                   "pe 14.8 # Default: 4.\n"
                                                   "-units mg/L # Default: mmol/kgw\n"
                                                   "Na 1 mmol/L # Chemical symbol from the 1st column in SOLUTION_MASTER_SPECIES, concentration, concentration is adapted to charge balance\n"
                                                   "Cl 37 charge # concentration is adapted to charge balance\n"
                                                   "C(4) 0.6 as HCO3 # Concentration is in mg of HCO3 = 0.6 / 0.61 = 0.98 mmol/L",
                                                   "Defines initial solutions.\n"
                                                   "\n"
                                                   "Example:\n"
                                                   "SOLUTION 1-10\n"
                                                   "pH 6.05\n"
                                                   "pe 14.8\n"
                                                   "-units mg/L\n"
                                                   "Na 1 mmol/L\n"
                                                   "Cl 37 charge\n"
                                                   "C(4) 0.6 as HCO3\n");
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_temp("temp 10",
                                                        "temperature in Celsius. Default: 25\n"
                                                        "\n"
                                                        "Example:\n"
                                                        "temp 10\n");
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_pH("pH 6.05 Calcite 0.2 # pH is adapted to attain Calcite equilibrium, SI = 0.2",
                                                      "Default: pH 7.\n"
                                                      "\n"
                                                      "Example:\n"
                                                      "pH 6.05 Calcite 0.2\n");
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_pe("pe 12 O2(g) -0.68 # pe is adapted to equilibrium with oxygen gas, partial pressure = 10^-0.68 atm",
                                                      "Default: pe 4.\n"
                                                      "\n"
                                                      "Example:\n"
                                                      "pe 12 O2(g) -0.68\n");
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_redox("redox O(-2)/O(0) # uses activities of H2O and O2 for pe",
                                                         "A redox couple is used to calculate pe.\n"
                                                         "\n"
                                                         "Example:\n"
                                                         "redox O(-2)/O(0)\n");
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_units("units mmol/L",
                                                         "Define units in (mol or g) / (L or kgw or kgs). Prefix m for milli or u for micro. ppm = mg/kgs, ppt = ug/kgs. Default: mmol/kgw.\n"
                                                         "\n"
                                                         "Example:\n"
                                                         "units mmol/L\n");
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_density("density 1.03",
                                                           "Default: 1 kg/L.\n"
                                                           "\n"
                                                           "Example:\n"
                                                           "density 1.03\n");
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_isotope("-isotope 13C -13 1.5 # name, value in permil, uncertainty in permil",
                                                           "Defines isotopic composition for an element.\n"
                                                           "\n"
                                                           "Example:\n"
                                                           "-isotope 13C -13 1.5\n");
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_water("-water 0.1",
                                                         "Defines mass of water (kg). Default: 1 kg.\n"
                                                         "\n"
                                                         "Example:\n"
                                                         "-water 0.1\n");
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPREAD("SOLUTION_SPREAD\n"
                                                          "units mg/l\n"
                                                          "temp 11\n"
                                                          "Alkalinity    Ca    Cl    S(6)    Temp    pH    Li\n"
                                                          "mg/l as HCO3                        ug/l\n"
                                                          "233.4    87.7    22.7    94.4    13    7.3\n"
                                                          "221.7    78.6    35.3    87.4    13.1    7.1    4\n"
                                                          "221.3    75.0    24.9    83.2        6.9\n"
                                                          "# The default temperature of 11oC will be given to the third sample.\n"
                                                          "# Li is in microgram/L",
                                                          "SOLUTION_SPREAD offers a matrix-type input format for SOLUTION.\n"
                                                          "\n"
                                                          "Example:\n"
                                                          "SOLUTION_SPREAD\n"
                                                          "units mg/l\n"
                                                          "temp 11\n"
                                                          "Alkalinity	Ca	Cl	S(6)	Temp	pH	Li\n"
                                                          "mg/l as HCO3						ug/l\n"
                                                          "233.4	87.7	22.7	94.4	13	7.3\n"
                                                          "221.7	78.6	35.3	87.4	13.1	7.1	4\n"
                                                          "221.3	75.0	24.9	83.2		6.9\n"
                                                          "# The default temperature of 11oC will be given to the third sample.\n"
                                                          "# Li is in microgram/L\n");
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPREAD_temp("temp 11",
                                                               "The temperature (Celsius) that is given to samples with temperature not defined. Default: 25 oC.\n"
                                                               "\n"
                                                               "Example:\n"
                                                               "temp 11\n");
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPREAD_pH("pH 6",
                                                             "The pH that is given to samples with pH not defined. Default: 7.\n"
                                                             "\n"
                                                             "Example:\n"
                                                             "pH 6\n");
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPREAD_pe("pe 10",
                                                             "The pe that is given to samples with pe not defined. Default: 4.\n"
                                                             "\n"
                                                             "Example:\n"
                                                             "pe 10\n");
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPREAD_redox("redox O(-2)/O(0) # uses activities of H2O and O2 for pe",
                                                                "A redox couple is used to calculate pe.\n"
                                                                "\n"
                                                                "Example:\n"
                                                                "redox O(-2)/O(0)\n");
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPREAD_units("units mmol/L",
                                                                "Define units in (mol or g) / (L or kgw or kgs). Prefix m for milli, or u for micro. ppm = mg/kgs, ppt = ug/kgs. Default: mmol/kgw.\n"
                                                                "\n"
                                                                "Example:\n"
                                                                "units mmol/L\n");
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPREAD_density("density 1.03",
                                                                  "The density that is given to samples with density undefined. Default: 1 kg/L.\n"
                                                                  "\n"
                                                                  "Example:\n"
                                                                  "density 1.03\n");
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPREAD_isotope("-isotope 13C -13 # name, value in permil",
                                                                  "Sets isotopic composition for an element if undefined.\n"
                                                                  "\n"
                                                                  "Example:\n"
                                                                  "-isotope 13C -13\n");
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPREAD_isotope_uncertainty("-isotope_uncertainty 13C 1.5",
                                                                              "Sets uncertainty (permil) for an isotope in samples if undefined.\n"
                                                                              "\n"
                                                                              "Example:\n"
                                                                              "-isotope_uncertainty 13C 1.5\n");
std::pair<CString, CString> CTreeCtrlPfw::SOLUTION_SPREAD_water("-water 0.1",
                                                                "Sets mass of water in samples if undefined. Default: 1 kg.\n"
                                                                "\n"
                                                                "Example:\n"
                                                                "-water 0.1\n");


std::pair<CString, CString> CTreeCtrlPfw::TITLE("TITLE",
                                                "Adds a title to a simulation.\n"
                                                "\n"
                                                "Example:\n"
                                                "TITLE\n");
std::pair<CString, CString> CTreeCtrlPfw::USE("USE solution 2\n"
                                              "#\n"
                                              "USE solution none # the reaction step is omitted",
                                              "Specifies a solution, exchange, equilibrium_phases, solid_solutions, and surface to be used in the batch-reaction calculation.\n"
                                              "\n"
                                              "Example:\n"
                                              "USE solution 2\n"
                                              "#\n"
                                              "USE solution none\n");

// CTreeCtrlPfw

IMPLEMENT_DYNAMIC(CTreeCtrlPfw, baseCTreeCtrlPfw)

CTreeCtrlPfw::CTreeCtrlPfw()
: m_pWndDesc(0)
{
}

CTreeCtrlPfw::~CTreeCtrlPfw()
{
}


BEGIN_MESSAGE_MAP(CTreeCtrlPfw, CTreeCtrl)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CTreeCtrlPfw::OnNMDblclk)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CTreeCtrlPfw::OnTvnSelchanged)
END_MESSAGE_MAP()



// CTreeCtrlPfw message handlers



int CTreeCtrlPfw::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (baseCTreeCtrlPfw::OnCreate(lpCreateStruct) == -1)
		return -1;

	/***
	// Add your specialized creation code here
	// Create the Image List
	if (!m_ctrlImage.Create(IDB_INPUT_ITEMS, 16, 0, RGB(255,0,255)))
	{
		TRACE0("Failed to create image list\n");
		return -1;
	}

	// Attach image list to Tree
	SetImageList(&m_ctrlImage, TVSIL_NORMAL);
	***/

	CTreeCtrlNode keys = this->InsertItem(_T("PHREEQC Keywords"));

	CTreeCtrlNode start = keys.AddTail(_T("Start"));
	start.AddTail(_T("COPY")).SetData((DWORD)&CTreeCtrlPfw::COPY);
	//start.AddTail(_T("DATABASE")).SetData((DWORD)&CTreeCtrlPfw::DATABASE);
	start.AddTail(_T("END")).SetData((DWORD)&CTreeCtrlPfw::END);
	start.AddTail(_T("SAVE")).SetData((DWORD)&CTreeCtrlPfw::SAVE);
	start.AddTail(_T("SOLUTION")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION);
		start.GetLastChild().AddTail(_T("temp")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_temp);
		start.GetLastChild().AddTail(_T("pH")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_pH);
		start.GetLastChild().AddTail(_T("pe")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_pe);
		start.GetLastChild().AddTail(_T("redox")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_redox);
		start.GetLastChild().AddTail(_T("units")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_units);
		start.GetLastChild().AddTail(_T("density")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_density);
		start.GetLastChild().AddTail(_T("-isotope")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_isotope);
		start.GetLastChild().AddTail(_T("-water")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_water);
	start.AddTail(_T("SOLUTION_SPREAD")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_SPREAD);
		start.GetLastChild().AddTail(_T("temp")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_SPREAD_temp);
		start.GetLastChild().AddTail(_T("pH")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_SPREAD_pH);
		start.GetLastChild().AddTail(_T("pe")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_SPREAD_pe);
		start.GetLastChild().AddTail(_T("redox")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_SPREAD_redox);
		start.GetLastChild().AddTail(_T("units")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_SPREAD_units);
		start.GetLastChild().AddTail(_T("density")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_SPREAD_density);
		start.GetLastChild().AddTail(_T("-isotope")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_SPREAD_isotope);
		start.GetLastChild().AddTail(_T("-isotope_uncertainty")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_SPREAD_isotope_uncertainty);
		start.GetLastChild().AddTail(_T("-water")).SetData((DWORD)&CTreeCtrlPfw::SOLUTION_SPREAD_water);
	start.AddTail(_T("TITLE")).SetData((DWORD)&CTreeCtrlPfw::TITLE);
	start.AddTail(_T("USE")).SetData((DWORD)&CTreeCtrlPfw::USE);
	start.GetLastChild();
	start.GetChildAt(0).EnsureVisible();

	CTreeCtrlNode chem = keys.AddTail(_T("Chemical reaction"));
	chem.AddTail(_T("EQUILIBRIUM_PHASES"));
		chem.GetLastChild().AddTail(_T("-force_equality"));
	chem.AddTail(_T("EXCHANGE"));
		chem.GetLastChild().AddTail(_T("-equilibrate"));
		chem.GetLastChild().AddTail(_T("-pitzer_exchange_gammas"));
	chem.AddTail(_T("GAS_PHASE"));
		chem.GetLastChild().AddTail(_T("-fixed_pressure"));
		chem.GetLastChild().AddTail(_T("-pressure"));
		chem.GetLastChild().AddTail(_T("-volume"));
		chem.GetLastChild().AddTail(_T("-temperature"));
		chem.GetLastChild().AddTail(_T("-fixed_volume"));
		chem.GetLastChild().AddTail(_T("-equilibrate"));
	chem.AddTail(_T("INCREMENTAL_REACTIONS"));
	chem.AddTail(_T("INVERSE_MODELING"));
	chem.AddTail(_T("REACTION"));
	chem.AddTail(_T("REACTION_TEMPERATURE"));
	chem.AddTail(_T("SURFACE"));

	CTreeCtrlNode phys = keys.AddTail(_T("Physical action"));
	phys.AddTail(_T("ADVECTION"));
	phys.AddTail(_T("MIX"));
	phys.AddTail(_T("TRANSPORT"));

	CTreeCtrlNode output = keys.AddTail(_T("Output"));
	output.AddTail(_T("PRINT"));
	output.AddTail(_T("SELECTED_OUTPUT"));
	output.AddTail(_T("USER_GRAPH"));
	output.AddTail(_T("USER_PRINT"));
	output.AddTail(_T("USER_PUNCH"));

	CTreeCtrlNode pbasic = this->InsertItem(_T("PHREEQC BASIC statements"));
	pbasic.AddTail(_T("ACT(\"..\")"));

	CTreeCtrlNode basic = this->InsertItem(_T("GENERAL BASIC statements"));
	basic.AddTail(_T("AND"));

	return 0;
}

void CTreeCtrlPfw::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	UNUSED_ALWAYS(pNMHDR);
	// Add your control notification handler code here

// COMMENT: {8/23/2012 11:14:22 PM}	UINT uFlags;
// COMMENT: {8/23/2012 11:14:22 PM}	CTreeCtrlNode node = HitTest(point, &uFlags);
// COMMENT: {8/23/2012 11:14:22 PM}	if ((node != 0) && (TVHT_ONITEM & uFlags))

	CTreeCtrlNode item = this->GetSelectedItem();
	if ((item.GetLevel() == 2 || item.GetLevel() == 3) && item.GetData())
	{
		if (CWnd* pWnd = ::AfxGetMainWnd())
		{
			if (CMDIFrameWnd *frm = dynamic_cast<CMDIFrameWnd*>(pWnd))
			{
				if (CMDIChildWnd *child = frm->MDIGetActive())
				{
					if (CRichViewIn *in = dynamic_cast<CRichViewIn*>(child->GetActiveView()))
					{
						CString strText((LPCTSTR)((std::pair<CString, CString>*)item.GetData())->first);
						strText += _T('\n');
						in->GetRichEditCtrl().ReplaceSel(strText, TRUE);
					}
					else
					{
						// sound warning message
						::MessageBeep(MB_ICONEXCLAMATION);
					}
					*pResult = TRUE;
					return;
				}
			}
		}
	}
	*pResult = 0;
}

void CTreeCtrlPfw::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	//CTreeCtrlNode item = this->GetSelectedItem();
	if (this->m_pWndDesc)
	{
		CTreeCtrlNode item(pNMTreeView->itemNew.hItem, this);
		if ((item.GetLevel() == 2 || item.GetLevel() == 3) && item.GetData())
		{
			CString strText((LPCTSTR)((std::pair<CString, CString>*)item.GetData())->second);
			strText.Replace(_T("\n"), _T("\r\n"));
			this->m_pWndDesc->SetWindowTextA(strText);
		}
		else
		{
			this->m_pWndDesc->SetWindowTextA(_T(""));
		}
	}
	*pResult = 0;
}
