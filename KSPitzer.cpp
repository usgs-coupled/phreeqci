// KSPitzer.cpp : implementation file
//
// $Id: KSPitzer.cpp 3706 2009-10-09 06:17:41Z charlton $
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "KSPitzer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKSPitzer

IMPLEMENT_DYNAMIC(CKSPitzer, baseCKSPitzer)

CKSPitzer::CKSPitzer(CWnd* pWndParent)
	 : baseCKSPitzer(IDS_PROPSHT_CAPTION34, pWndParent)
	 , m_PageGen(CKPPitzerGeneral::IDD, IDS_STRING650)
	 , m_PageB0(CKPPitzerCA::IDD, IDS_STRING651)
	 , m_PageB1(CKPPitzerCA::IDD, IDS_STRING652)
	 , m_PageB2(CKPPitzerCA::IDD, IDS_STRING653)
	 , m_PageC0(CKPPitzerCA::IDD, IDS_STRING654)
	 , m_PagePsi(CKPPitzerXXY::IDD, IDS_STRING655)
	 , m_PageTheta(CKPPitzerXX::IDD, IDS_STRING656)
	 , m_PageLamda(CKPPitzerXN::IDD, IDS_STRING657)
	 , m_PageZeta(CKPPitzerCAN::IDD, IDS_STRING658)
	 , m_PageMu(CKPPitzerNNV::IDD, IDS_STRING659)
	 , m_PageEta(CKPPitzerNXX::IDD, IDS_STRING660)
	 , m_PageAlphas(CKPPitzerAlphas::IDD, IDS_STRING661)
{
	m_bNeedDatabase = true;
	// Add all of the property pages here.  Note that
	// the order that they appear in here will be
	// the order they appear in on screen.  By default,
	// the first page of the set is the active one.
	// One way to make a different property page the 
	// active one is to call SetActivePage().

	AddPage(&m_PageGen);
	AddPage(&m_PageB0);
	AddPage(&m_PageB1);
	AddPage(&m_PageB2);
	AddPage(&m_PageC0);
	AddPage(&m_PagePsi);
	AddPage(&m_PageTheta);
	AddPage(&m_PageLamda);
	AddPage(&m_PageZeta);
	AddPage(&m_PageMu);
	AddPage(&m_PageEta);
	AddPage(&m_PageAlphas);
}

CKSPitzer::~CKSPitzer()
{
}


BEGIN_MESSAGE_MAP(CKSPitzer, baseCKSPitzer)
	//{{AFX_MSG_MAP(CKSPitzer)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CKSPitzer message handlers

CString CKSPitzer::GetString()
{
	// Line 0
	CString strLines = _T("PITZER");

	CString strFormat;
	CString strAppend;

	// -MacInnes
	strFormat.Format(_T("%s%s"),
		(LPCTSTR)s_strNewLine,
		m_PageGen.m_bMacInnes  ? _T("-MacInnes   true") : _T("-MacInnes   false")
		);
	strLines += strFormat;

	// -use_etheta
	strFormat.Format(_T("%s%s"),
		(LPCTSTR)s_strNewLine,
		m_PageGen.m_bUseEtheta ? _T("-use_etheta true") : _T("-use_etheta false")
		);
	strLines += strFormat;

	// -redox
	strFormat.Format(_T("%s%s"),
		(LPCTSTR)s_strNewLine,
		m_PageGen.m_bRedox     ? _T("-redox      true") : _T("-redox      false")
		);
	strLines += strFormat;

	// -B0
	std::list<CPitzParam>::const_iterator cIter = m_PageB0.m_listParams.begin();
	if (cIter != m_PageB0.m_listParams.end())
	{
		strFormat.Format(_T("%s%s"),
			(LPCTSTR)s_strNewLine,
			_T("-B0")
			);
		strLines += strFormat;
	}
	for (; cIter != m_PageB0.m_listParams.end(); ++cIter)
	{
		// species
		strFormat.Format(_T("%s  %-11s  %-11s"),
			(LPCTSTR)s_strNewLine,
			cIter->species[0],
			cIter->species[1]
		);
		strLines += strFormat;

		// analytical_expression
		int nnums = 6;
		for (; nnums; --nnums)
		{
			if (cIter->a[nnums - 1] != 0 && (cIter->a[nnums - 1] == cIter->a[nnums - 1]))
			{
				break;
			}
		}
		// need at least one
		if (nnums == 0) nnums = 1;
		for (int i = 0; i < nnums; ++i)
		{
			if (cIter->a[i] != cIter->a[i])
			{
				strFormat.Format(_T("   %.*g"),
					DBL_DIG,
					0.0
				);
			}
			else
			{
				strFormat.Format(_T("   %.*g"),
					DBL_DIG,
					cIter->a[i]
				);
			}
			strAppend.Format(_T("%-10s"), strFormat);
			strLines += strAppend;
		}
	}


	// -B1
	cIter = m_PageB1.m_listParams.begin();
	if (cIter != m_PageB1.m_listParams.end())
	{
		strFormat.Format(_T("%s%s"),
			(LPCTSTR)s_strNewLine,
			_T("-B1")
			);
		strLines += strFormat;
	}
	for (; cIter != m_PageB1.m_listParams.end(); ++cIter)
	{
		// species
		strFormat.Format(_T("%s  %-11s  %-11s"),
			(LPCTSTR)s_strNewLine,
			cIter->species[0],
			cIter->species[1]
		);
		strLines += strFormat;

		// analytical_expression
		int nnums = 6;
		for (; nnums; --nnums)
		{
			if (cIter->a[nnums - 1] != 0 && (cIter->a[nnums - 1] == cIter->a[nnums - 1]))
			{
				break;
			}
		}
		// need at least one
		if (nnums == 0) nnums = 1;
		for (int i = 0; i < nnums; ++i)
		{
			if (cIter->a[i] != cIter->a[i])
			{
				strFormat.Format(_T("   %.*g"),
					DBL_DIG,
					0.0
				);
			}
			else
			{
				strFormat.Format(_T("   %.*g"),
					DBL_DIG,
					cIter->a[i]
				);
			}
			strAppend.Format(_T("%-10s"), strFormat);
			strLines += strAppend;
		}
	}

	// -B2
	cIter = m_PageB2.m_listParams.begin();
	if (cIter != m_PageB2.m_listParams.end())
	{
		strFormat.Format(_T("%s%s"),
			(LPCTSTR)s_strNewLine,
			_T("-B2")
			);
		strLines += strFormat;
	}
	for (; cIter != m_PageB2.m_listParams.end(); ++cIter)
	{
		// species
		strFormat.Format(_T("%s  %-11s  %-11s"),
			(LPCTSTR)s_strNewLine,
			cIter->species[0],
			cIter->species[1]
		);
		strLines += strFormat;

		// analytical_expression
		int nnums = 6;
		for (; nnums; --nnums)
		{
			if (cIter->a[nnums - 1] != 0 && (cIter->a[nnums - 1] == cIter->a[nnums - 1]))
			{
				break;
			}
		}
		// need at least one
		if (nnums == 0) nnums = 1;
		for (int i = 0; i < nnums; ++i)
		{
			if (cIter->a[i] != cIter->a[i])
			{
				strFormat.Format(_T("   %.*g"),
					DBL_DIG,
					0.0
				);
			}
			else
			{
				strFormat.Format(_T("   %.*g"),
					DBL_DIG,
					cIter->a[i]
				);
			}
			strAppend.Format(_T("%-10s"), strFormat);
			strLines += strAppend;
		}
	}

	// -C0
	cIter = m_PageC0.m_listParams.begin();
	if (cIter != m_PageC0.m_listParams.end())
	{
		strFormat.Format(_T("%s%s"),
			(LPCTSTR)s_strNewLine,
			_T("-C0")
			);
		strLines += strFormat;
	}
	for (; cIter != m_PageC0.m_listParams.end(); ++cIter)
	{
		// species
		strFormat.Format(_T("%s  %-11s  %-11s"),
			(LPCTSTR)s_strNewLine,
			cIter->species[0],
			cIter->species[1]
		);
		strLines += strFormat;

		// analytical_expression
		int nnums = 6;
		for (; nnums; --nnums)
		{
			if (cIter->a[nnums - 1] != 0 && (cIter->a[nnums - 1] == cIter->a[nnums - 1]))
			{
				break;
			}
		}
		// need at least one
		if (nnums == 0) nnums = 1;
		for (int i = 0; i < nnums; ++i)
		{
			if (cIter->a[i] != cIter->a[i])
			{
				strFormat.Format(_T("   %.*g"),
					DBL_DIG,
					0.0
				);
			}
			else
			{
				strFormat.Format(_T("   %.*g"),
					DBL_DIG,
					cIter->a[i]
				);
			}
			strAppend.Format(_T("%-10s"), strFormat);
			strLines += strAppend;
		}
	}

	// -PSI
	cIter = m_PagePsi.m_listParams.begin();
	if (cIter != m_PagePsi.m_listParams.end())
	{
		strFormat.Format(_T("%s%s"),
			(LPCTSTR)s_strNewLine,
			_T("-PSI")
			);
		strLines += strFormat;
	}
	for (; cIter != m_PagePsi.m_listParams.end(); ++cIter)
	{
		// species
		strFormat.Format(_T("%s  %-11s  %-11s  %-11s"),
			(LPCTSTR)s_strNewLine,
			cIter->species[0],
			cIter->species[1],
			cIter->species[2]
		);
		strLines += strFormat;

		// analytical_expression
		int nnums = 6;
		for (; nnums; --nnums)
		{
			if (cIter->a[nnums - 1] != 0 && (cIter->a[nnums - 1] == cIter->a[nnums - 1]))
			{
				break;
			}
		}
		// need at least one
		if (nnums == 0) nnums = 1;
		for (int i = 0; i < nnums; ++i)
		{
			if (cIter->a[i] != cIter->a[i])
			{
				strFormat.Format(_T("   %.*g"),
					DBL_DIG,
					0.0
				);
			}
			else
			{
				strFormat.Format(_T("   %.*g"),
					DBL_DIG,
					cIter->a[i]
				);
			}
			strAppend.Format(_T("%-10s"), strFormat);
			strLines += strAppend;
		}
	}

	// -THETA
	cIter = m_PageTheta.m_listParams.begin();
	if (cIter != m_PageTheta.m_listParams.end())
	{
		strFormat.Format(_T("%s%s"),
			(LPCTSTR)s_strNewLine,
			_T("-THETA")
			);
		strLines += strFormat;
	}
	for (; cIter != m_PageTheta.m_listParams.end(); ++cIter)
	{
		// species
		strFormat.Format(_T("%s  %-11s  %-11s"),
			(LPCTSTR)s_strNewLine,
			cIter->species[0],
			cIter->species[1]
		);
		strLines += strFormat;

		// analytical_expression
		int nnums = 6;
		for (; nnums; --nnums)
		{
			if (cIter->a[nnums - 1] != 0 && (cIter->a[nnums - 1] == cIter->a[nnums - 1]))
			{
				break;
			}
		}
		// need at least one
		if (nnums == 0) nnums = 1;
		for (int i = 0; i < nnums; ++i)
		{
			if (cIter->a[i] != cIter->a[i])
			{
				strFormat.Format(_T("   %.*g"),
					DBL_DIG,
					0.0
				);
			}
			else
			{
				strFormat.Format(_T("   %.*g"),
					DBL_DIG,
					cIter->a[i]
				);
			}
			strAppend.Format(_T("%-10s"), strFormat);
			strLines += strAppend;
		}
	}

	// -LAMDA
	cIter = m_PageLamda.m_listParams.begin();
	if (cIter != m_PageLamda.m_listParams.end())
	{
		strFormat.Format(_T("%s%s"),
			(LPCTSTR)s_strNewLine,
			_T("-LAMDA")
			);
		strLines += strFormat;
	}
	for (; cIter != m_PageLamda.m_listParams.end(); ++cIter)
	{
		// species
		strFormat.Format(_T("%s  %-11s  %-11s"),
			(LPCTSTR)s_strNewLine,
			cIter->species[0],
			cIter->species[1]
		);
		strLines += strFormat;

		// analytical_expression
		int nnums = 6;
		for (; nnums; --nnums)
		{
			if (cIter->a[nnums - 1] != 0 && (cIter->a[nnums - 1] == cIter->a[nnums - 1]))
			{
				break;
			}
		}
		// need at least one
		if (nnums == 0) nnums = 1;
		for (int i = 0; i < nnums; ++i)
		{
			if (cIter->a[i] != cIter->a[i])
			{
				strFormat.Format(_T("   %.*g"),
					DBL_DIG,
					0.0
				);
			}
			else
			{
				strFormat.Format(_T("   %.*g"),
					DBL_DIG,
					cIter->a[i]
				);
			}
			strAppend.Format(_T("%-10s"), strFormat);
			strLines += strAppend;
		}
	}

	// -ZETA
	cIter = m_PageZeta.m_listParams.begin();
	if (cIter != m_PageZeta.m_listParams.end())
	{
		strFormat.Format(_T("%s%s"),
			(LPCTSTR)s_strNewLine,
			_T("-ZETA")
			);
		strLines += strFormat;
	}
	for (; cIter != m_PageZeta.m_listParams.end(); ++cIter)
	{
		// species
		strFormat.Format(_T("%s  %-11s  %-11s  %-11s"),
			(LPCTSTR)s_strNewLine,
			cIter->species[0],
			cIter->species[1],
			cIter->species[2]
		);
		strLines += strFormat;

		// analytical_expression
		int nnums = 6;
		for (; nnums; --nnums)
		{
			if (cIter->a[nnums - 1] != 0 && (cIter->a[nnums - 1] == cIter->a[nnums - 1]))
			{
				break;
			}
		}
		// need at least one
		if (nnums == 0) nnums = 1;
		for (int i = 0; i < nnums; ++i)
		{
			if (cIter->a[i] != cIter->a[i])
			{
				strFormat.Format(_T("   %.*g"),
					DBL_DIG,
					0.0
				);
			}
			else
			{
				strFormat.Format(_T("   %.*g"),
					DBL_DIG,
					cIter->a[i]
				);
			}
			strAppend.Format(_T("%-10s"), strFormat);
			strLines += strAppend;
		}
	}

	// -MU
	cIter = m_PageMu.m_listParams.begin();
	if (cIter != m_PageMu.m_listParams.end())
	{
		strFormat.Format(_T("%s%s"),
			(LPCTSTR)s_strNewLine,
			_T("-MU")
			);
		strLines += strFormat;
	}
	for (; cIter != m_PageMu.m_listParams.end(); ++cIter)
	{
		// species
		strFormat.Format(_T("%s  %-11s  %-11s  %-11s"),
			(LPCTSTR)s_strNewLine,
			cIter->species[0],
			cIter->species[1],
			cIter->species[2]
		);
		strLines += strFormat;

		// analytical_expression
		int nnums = 6;
		for (; nnums; --nnums)
		{
			if (cIter->a[nnums - 1] != 0 && (cIter->a[nnums - 1] == cIter->a[nnums - 1]))
			{
				break;
			}
		}
		// need at least one
		if (nnums == 0) nnums = 1;
		for (int i = 0; i < nnums; ++i)
		{
			if (cIter->a[i] != cIter->a[i])
			{
				strFormat.Format(_T("   %.*g"),
					DBL_DIG,
					0.0
				);
			}
			else
			{
				strFormat.Format(_T("   %.*g"),
					DBL_DIG,
					cIter->a[i]
				);
			}
			strAppend.Format(_T("%-10s"), strFormat);
			strLines += strAppend;
		}
	}

	// -ETA
	cIter = m_PageEta.m_listParams.begin();
	if (cIter != m_PageEta.m_listParams.end())
	{
		strFormat.Format(_T("%s%s"),
			(LPCTSTR)s_strNewLine,
			_T("-ETA")
			);
		strLines += strFormat;
	}
	for (; cIter != m_PageEta.m_listParams.end(); ++cIter)
	{
		// species
		strFormat.Format(_T("%s  %-11s  %-11s  %-11s"),
			(LPCTSTR)s_strNewLine,
			cIter->species[0],
			cIter->species[1],
			cIter->species[2]
		);
		strLines += strFormat;

		// analytical_expression
		int nnums = 6;
		for (; nnums; --nnums)
		{
			if (cIter->a[nnums - 1] != 0 && (cIter->a[nnums - 1] == cIter->a[nnums - 1]))
			{
				break;
			}
		}
		// need at least one
		if (nnums == 0) nnums = 1;
		for (int i = 0; i < nnums; ++i)
		{
			if (cIter->a[i] != cIter->a[i])
			{
				strFormat.Format(_T("   %.*g"),
					DBL_DIG,
					0.0
				);
			}
			else
			{
				strFormat.Format(_T("   %.*g"),
					DBL_DIG,
					cIter->a[i]
				);
			}
			strAppend.Format(_T("%-10s"), strFormat);
			strLines += strAppend;
		}
	}

	// -ALPHAS
	cIter = m_PageAlphas.m_listParams.begin();
	if (cIter != m_PageAlphas.m_listParams.end())
	{
		strFormat.Format(_T("%s%s"),
			(LPCTSTR)s_strNewLine,
			_T("-ALPHAS")
			);
		strLines += strFormat;
	}
	for (; cIter != m_PageAlphas.m_listParams.end(); ++cIter)
	{
		// species
		strFormat.Format(_T("%s  %-11s  %-11s"),
			(LPCTSTR)s_strNewLine,
			cIter->species[0],
			cIter->species[1]
		);
		strLines += strFormat;

		// analytical_expression
		int nnums = 2;
		for (int i = 0; i < nnums; ++i)
		{
			strFormat.Format(_T("   %.*g"),
				DBL_DIG,
				cIter->a[i]
			);
			strAppend.Format(_T("%-10s"), strFormat);
			strLines += strAppend;
		}
	}

	return strLines + s_strNewLine;
}

void CKSPitzer::Edit(CString& rStr)
{
	PhreeqcI p(rStr);
	p.GetData(this);
}
