// OCKSSolution_Spread.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "OCKSSolution_Spread.h"

#include "KeywordLoader2.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COCKSSolution_Spread

IMPLEMENT_DYNAMIC(COCKSSolution_Spread, baseOCKSSolution_Spread)

COCKSSolution_Spread::COCKSSolution_Spread(CWnd* pWndParent)
	 : baseOCKSSolution_Spread(IDS_PROPSHT_CAPTION25, pWndParent)
{
	// Add all of the property pages here.  Note that
	// the order that they appear in here will be
	// the order they appear in on screen.  By default,
	// the first page of the set is the active one.
	// One way to make a different property page the 
	// active one is to call SetActivePage().

	AddPage(&m_Page1);
	AddPage(&m_Page2);
	AddPage(&m_Page3);

	// m_pPage4 is added by CKPSolution_SpreadPg1::OnChkUseInverse()
	for (int i = 0; i < count_iso_defaults; ++i)
	{
		CIsotope isotope(&iso_defaults[i]);
		m_listIsotopes.push_back(isotope);
	}
	m_pPage4 = NULL;
}

COCKSSolution_Spread::~COCKSSolution_Spread()
{
	if (m_pPage4)
	{
		delete m_pPage4;
		m_pPage4 = NULL;
	}
}


BEGIN_MESSAGE_MAP(COCKSSolution_Spread, baseOCKSSolution_Spread)
	//{{AFX_MSG_MAP(COCKSSolution_Spread)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COCKSSolution_Spread message handlers

BOOL COCKSSolution_Spread::OnInitDialog() 
{
	BOOL bResult = baseOCKSSolution_Spread::OnInitDialog();
	
	// Add your specialized code here
	CTabCtrl* pTabCtrl =  GetTabControl();
	ASSERT( pTabCtrl != NULL );

	TCITEM item;
	item.mask = TCIF_TEXT;

// COMMENT: {5/23/2001 3:59:19 PM}	item.pszText = _T("&General defaults");
// COMMENT: {5/23/2001 3:59:19 PM}	pTabCtrl->SetItem(2, &item);
	item.pszText = _T("&Isotope defaults");
	pTabCtrl->SetItem(3, &item);
	
	return bResult;
}

CString COCKSSolution_Spread::GetString()
{
	/*
	Line 0:  SOLUTION_S    # "\t" indicates the tab character
	Line 1:  -temp        25
	Line 2:  -ph          7.1
	Line 3:  -pe          4
	Line 4:  -redox       O(0)/O(-2)
	Line 5:  -units       mmol/kgw
	Line 6:  -density     1
	Line 7:  -water       1.0
	Line 8a: -isotope     34S       15.0   1.0
	Line 8b: -isotope     13C      -12.0
	Line 9:  -isotope_uncertainty   13C    1.0
	Line 10: Number\t    13C\t uncertainty\t  pH\t  Ca\t  Na\t   Cl\t     Alkalinity\t  Description
	Line 11:       \t       \t            \t    \t    \t    \t     \t mg/kgw as HCO3\t
	Line 12a: 10-11\t  -10.2\t        0.05\t 6.9\t  23\t   6\t 10.5\t             61\t  soln 10-11
	Line 12b:     1\t  -12.1\t         0.1\t    \t  17\t   6\t   9.\t             55\t  My well 1
	Line 12c:     5\t  -14.1\t         0.2\t    \t  27\t   9\t  9.5\t             70\t  My well 5
	*/

	CString strLines;
	CString strFormat;
	
	// Line 0
	strLines = _T("SOLUTION_SPREAD");

	// Line 1 temp
	if (m_Page3.m_dTemp != 25.0)
	{
		strFormat.Format(_T("%s%4c%-9s %.*g"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			_T("-temp"),
			DBL_DIG,
			m_Page3.m_dTemp
			);
		strLines += strFormat;
	}

	// Line 2 pH
	if (m_Page3.m_dPH != 7.0)
	{
		strFormat.Format(_T("%s%4c%-9s %.*g"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			_T("-pH"),
			DBL_DIG,
			m_Page3.m_dPH
			);
		strLines += strFormat;
	}

	// Line 3 pe
	if (m_Page3.m_dPE != 4.0)
	{
		strFormat.Format(_T("%s%4c%-9s %.*g"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			_T("-pe"),
			DBL_DIG,
			m_Page3.m_dPE
			);
		strLines += strFormat;
	}

	// Line 4 redox
	if (!m_Page3.m_strRedox.IsEmpty() && m_Page3.m_strRedox.CompareNoCase(_T("pe")) != 0)
	{
		strFormat.Format(_T("%s%4c%-9s %s"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			_T("-redox"),
			(LPCTSTR)m_Page3.m_strRedox
			);
		strLines += strFormat;
	}

	// Line 5 units
	if (m_Page3.m_strDefaultUnits.CompareNoCase(_T("mmol/kgw")) != 0)
	{
		m_Page3.m_strDefaultUnits.MakeLower();
		strFormat.Format(_T("%s%4c%-9s %s"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			_T("-units"), 
			(LPCTSTR)m_Page3.m_strDefaultUnits
			);
		strLines += strFormat;
	}

	// Line 6 density
	if (m_Page3.m_dDensity != 1.0)
	{
		strFormat.Format(_T("%s%4c%-9s %.*g"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			_T("-density"),
			DBL_DIG,
			m_Page3.m_dDensity
			);
		strLines += strFormat;
	}

	// Line 7:  -water       1.0
	if (m_Page3.m_dWaterMass != 1.0)
	{
		strFormat.Format(_T("%s%4c%-9s %.*g # kg"),
			(LPCTSTR)s_strNewLine,
			_T(' '),
			_T("-water"),
			DBL_DIG,
			m_Page3.m_dWaterMass
			);
		strLines += strFormat;
	}

	// Line 8 and 9
	if (m_pPage4 != NULL)
	{
		std::list<CIsotope>::const_iterator isoIter = m_listIsotopes.begin();
		for (; isoIter != m_listIsotopes.end(); ++isoIter)
		{
			CIsotope isotope(*isoIter);
			bool bSkipDefault = false;
			for (int c = 0; c < count_iso_defaults; ++c)
			{
				if (isotope.m_strName == iso_defaults[c].name)
				{
					bSkipDefault = (isotope.m_dRatio == iso_defaults[c].value &&
						isotope.m_dRatioUncertainty == iso_defaults[c].uncertainty);
					break;
				}
			}
			if (bSkipDefault) continue;

			if (isotope.m_dRatio != std::numeric_limits<double>::signaling_NaN())
			{
				// Line 8
				if (isotope.m_dRatioUncertainty != std::numeric_limits<double>::signaling_NaN())
				{
					strFormat.Format(_T("%s%4c%-9s %-7s %.*g %.*g"),
						(LPCTSTR)s_strNewLine,
						_T(' '),
						_T("-isotope"),
						(LPCTSTR)isotope.m_strName,
						DBL_DIG,
						isotope.m_dRatio,
						DBL_DIG,
						isotope.m_dRatioUncertainty
						);
				}
				else
				{
					strFormat.Format(_T("%s%4c%-9s %-7s %.*g"),
						(LPCTSTR)s_strNewLine,
						_T(' '),
						_T("-isotope"),
						(LPCTSTR)isotope.m_strName,
						DBL_DIG,
						isotope.m_dRatio
						);
				}
			}
			else if (isotope.m_dRatioUncertainty != std::numeric_limits<double>::signaling_NaN())
			{
				// Line 9
				strFormat.Format(_T("%s%4c%s  %-7s %.*g"),
					(LPCTSTR)s_strNewLine,
					_T(' '),
					_T("-isotope_uncertainty"),
					(LPCTSTR)isotope.m_strName,
					DBL_DIG,
					isotope.m_dRatioUncertainty
					);
			}
			else
			{
				// do nothing
				continue;
			}
			strLines += strFormat;
		}
	}
	
	// Line 10 and 11
	std::vector< std::string >::size_type stCol = 0;
	CString strLine;
	CString strUnitsLine;
	bool bPrintUnits = false;	
	for (; stCol < m_Page1.m_vsHeading.size(); ++stCol)
	{
		// determine widest item for this column
		std::vector< std::vector<std::string> >::size_type nWidest = m_Page1.m_vsHeading[stCol].size();

		// check units width
		std::map< std::string, CConc >::const_iterator mapIter;
		mapIter = m_Page2.m_mapElement2Conc.find(m_Page1.m_vsHeading[stCol]);
		if (mapIter != m_Page2.m_mapElement2Conc.end())
		{
			nWidest = __max(nWidest, (unsigned)(*mapIter).second.GetSubHeading().GetLength());
		}

		std::vector< std::vector<std::string> >::size_type stRow = 0;
		for (; stRow < m_Page1.m_vvsCells.size(); ++stRow)
		{
			nWidest = __max(nWidest, m_Page1.m_vvsCells[stRow][stCol].size());
		}

		CString strCell;

		// reformat header
		if (stCol == 0)
		{
			strCell.Format(_T("%*s"),
				nWidest,
				m_Page1.m_vsHeading[stCol].c_str()
				);
		}
		else
		{
			strCell.Format(_T(" %*s"),
				nWidest,
				m_Page1.m_vsHeading[stCol].c_str()
				);
		}

		strCell.Format(_T(" %*s"),
			nWidest,
			m_Page1.m_vsHeading[stCol].c_str()
			);
		m_Page1.m_vsHeading[stCol] = strCell;

		if (stCol == 0)
		{
			strLine = m_Page1.m_vsHeading[stCol].c_str();
		}
		else
		{
			strLine += CString("\t") + m_Page1.m_vsHeading[stCol].c_str();
		}

		// format units
		if (mapIter != m_Page2.m_mapElement2Conc.end())
		{
			if (stCol == 0)
			{
				strCell.Format(_T("%*s"),
					nWidest,
					(LPCTSTR)(*mapIter).second.GetSubHeading()
					);
			}
			else
			{
				strCell.Format(_T(" %*s"),
					nWidest,
					(LPCTSTR)(*mapIter).second.GetSubHeading()
					);
			}
			bPrintUnits = true;
		}
		else
		{
			if (stCol == 0)
			{
				strCell.Format(_T("%*c"),
					nWidest,
					_T(' ')
					);
			}
			else
			{
				strCell.Format(_T(" %*c"),
					nWidest,
					_T(' ')
					);
			}
		}
		if (stCol == 0)
		{
			strUnitsLine = strCell;
		}
		else
		{
			strUnitsLine += CString("\t") + strCell;
		}
	}

	// Line 10
	strFormat.Format(_T("%s%s"),
		(LPCTSTR)s_strNewLine,
		(LPCTSTR)strLine
		);
	strLines += strFormat;

	// Line 11
	if (bPrintUnits)
	{
		strFormat.Format(_T("%s%s"),
			(LPCTSTR)s_strNewLine,
			(LPCTSTR)strUnitsLine
			);
		strLines += strFormat;
	}


	// Line 12
	std::vector< std::vector<std::string> >::size_type stRow = 0;
	for (; stRow < m_Page1.m_vvsCells.size(); ++stRow)
	{
		std::vector<std::string>::size_type stCol = 0;

		strLine.Format(_T("%*s"),
			m_Page1.m_vsHeading[stCol].size(),
			m_Page1.m_vvsCells[stRow][stCol].c_str()
			);

		CString strCell;
		for (stCol = 1; stCol < m_Page1.m_vsHeading.size(); ++stCol)
		{
			strCell.Format(_T("\t%*s"),
				m_Page1.m_vsHeading[stCol].size(),
				m_Page1.m_vvsCells[stRow][stCol].c_str()
				);
			strLine += strCell;
		}
		strFormat.Format(_T("%s%s"),
			(LPCTSTR)s_strNewLine,
			(LPCTSTR)strLine
			);
		strLines += strFormat;
	}

	return strLines + s_strNewLine;
}

void COCKSSolution_Spread::Edit(CString& rStr)
{
	ASSERT(std::numeric_limits<double>::has_signaling_NaN == true);

	CKeywordLoader2 keywordLoader2(rStr);

	// spread sheet Page 1
	for (int c = 0; spread_sheet.heading && c < spread_sheet.heading->count; ++c)
	{
		std::string sCell = spread_sheet.heading->char_vector[c];
		m_Page1.m_vsHeading.push_back(sCell);
		if (spread_sheet.units && c < spread_sheet.units->count)
		{
			std::string sUnit = spread_sheet.units->char_vector[c];
			if (!sUnit.empty())
			{
				CConc conc = CConc::Create(sUnit.c_str());
				m_Page2.m_mapElement2Conc[sCell] = conc;
			}
		}
	}

	for (int r = 0; r < spread_sheet.count_rows; ++r)
	{
		std::vector<std::string> vsRow;
		std::string sCell;
		for (int c = 0; c < spread_sheet.rows[r]->count; ++c)
		{
			sCell = spread_sheet.rows[r]->char_vector[c];
			vsRow.push_back(sCell);			
		}
		m_Page1.m_vvsCells.push_back(vsRow);
	}
	

	//
	// &General defaults
	//
	m_Page3.m_dTemp           = spread_sheet.defaults.temp;
	m_Page3.m_dPH             = spread_sheet.defaults.ph;
	m_Page3.m_dPE             = spread_sheet.defaults.pe;
	m_Page3.m_strRedox        = spread_sheet.defaults.redox;
	m_Page3.m_strDefaultUnits = spread_sheet.defaults.units;
	m_Page3.m_dDensity        = spread_sheet.defaults.density;
	m_Page3.m_dWaterMass      = spread_sheet.defaults.water;

	// reformat Units
	m_Page3.m_strDefaultUnits.Replace(_T("Mol"), _T("mol"));
	m_Page3.m_strDefaultUnits.Replace(_T("/l"), _T("/L"));
	m_Page3.m_strDefaultUnits.Replace(_T("mg/kgs"), _T("ppm"));
	m_Page3.m_strDefaultUnits.Replace(_T("ug/kgs"), _T("ppb"));
	m_Page3.m_strDefaultUnits.Replace(_T("g/kgs"), _T("ppt"));	

	//
	// &Isotope defaults
	//

	// default isotopes
	for (int i = 0; i < spread_sheet.defaults.count_iso; ++i)
	{
		CIsotope cisotope(&spread_sheet.defaults.iso[i]);

		std::list<CIsotope>::iterator cIter = m_listIsotopes.begin();
		for (; cIter != m_listIsotopes.end(); ++cIter)
		{
			if (cIter->m_strName == cisotope.m_strName)
			{
				cIter->m_dRatio            = cisotope.m_dRatio;
				cIter->m_dRatioUncertainty = cisotope.m_dRatioUncertainty;
				break;
			}
		}
		if (cIter == m_listIsotopes.end()) m_listIsotopes.push_back(cisotope);
	}
}
