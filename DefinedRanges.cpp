// DefinedRanges.cpp: implementation of the CDefinedRanges class.
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "DefinedRanges.h"

#include "TreeCtrlIn.h"
#include "KeywordParser.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDefinedRanges::CDefinedRanges(const CTreeCtrlNode& rSimNode, bool bSkipSaves/* = false */, bool bSkipCopys/* = false */)
{
	CTreeCtrlNode simNode(rSimNode);
	ASSERT(simNode && simNode.GetLevel() == CTreeCtrlIn::SimLevel);
	if (simNode == 0)
	{
		// Avoid crash in Release builds
		TRACE0("WARNING: NULL simNode passed to CDefinedRanges\n");
		return;
	}

	CTreeCtrlNode fileNode = simNode.GetParent();
	ASSERT(fileNode && fileNode.GetLevel() == CTreeCtrlIn::FileLevel);

	// foreach sim inclusive
	Phreeqc p;
	p.do_initialize();
	for (CTreeCtrlNode sim = fileNode.GetChild(); sim != 0; sim = sim.GetNextSibling())
	{
		int no_soln_num_count = 0;
		// foreach key
		for (CTreeCtrlNode key = sim.GetChild(); key != 0; key = key.GetNextSibling())
		{
			int imageID = key.GetImageID();
			CString strSolnSpread;
			CTreeCtrlNode sib;
			switch (imageID)
			{
			case CTreeCtrlIn::equilibrium_phasesImage :
				break;
			case CTreeCtrlIn::exchangeImage :
				break;
			case CTreeCtrlIn::gas_phaseImage :
				break;
			case CTreeCtrlIn::kineticsImage :
				break;
			case CTreeCtrlIn::mixImage :
				break;
			case CTreeCtrlIn::reactionImage :
				break;
			case CTreeCtrlIn::reaction_temperatureImage :
				break;
			case CTreeCtrlIn::solid_solutionsImage :
				break;
			case CTreeCtrlIn::solutionImage :
				break;
			case CTreeCtrlIn::surfaceImage :
				break;
			case CTreeCtrlIn::saveImage :
				if (bSkipSaves) continue;
				break;
			case CTreeCtrlIn::solution_spreadImage :
				break;
			default:
				// ignore all other keywords
				continue;
				break;
			}
			int n_user, n_user_end;
			char *description = 0;
			CString strKey = key.GetText();
			
			// special case for SAVE
			if (imageID == CTreeCtrlIn::saveImage)
			{		
				ASSERT(strKey.GetLength() > (int)::strlen("SAVE"));
				strKey = strKey.Mid(::strlen("SAVE"));
				strKey.TrimLeft();
			}

			p.read_number_description(strKey.GetBuffer(strKey.GetLength() + 1),
				&n_user, &n_user_end, &description);
			p.PHRQ_free(description);

			CTreeCtrlIn::ImageIndex imageIndex = static_cast<CTreeCtrlIn::ImageIndex>(imageID);

			CKeyword::type nType = CTreeCtrlIn::GetKeywordType(imageIndex);
			
			// special case for SAVE
			if (imageID == CTreeCtrlIn::saveImage)
			{
				CString strSaved = strKey.SpanExcluding(_T(" \t"));
				nType = CKeyword::GetKeywordType(strSaved);
				ASSERT(nType != CKeyword::K_NOT_KEYWORD);
			}
			else if (imageID == CTreeCtrlIn::solution_spreadImage)
			{
				strSolnSpread = CKeywordParser::GetInverseTreeString(key.GetText()) + " \n";
				for (CTreeCtrlNode sib = key.GetChild(); sib != 0; sib = sib.GetNextSibling())
				{
					strSolnSpread += CKeywordParser::GetInverseTreeString(sib.GetText()) + " \n";
				}
				no_soln_num_count += ReadSolutionSpread(strSolnSpread);
				continue; // skip insert
			}
			(*this)[nType].insert(CRange(n_user, n_user_end));
		}
		if (no_soln_num_count > 0)
		{
			int max_soln = 0;
			std::map< CKeyword::type, std::set<CRange> >::iterator iter = this->find(CKeyword::K_SOLUTION);
			for (; iter != this->end(); ++iter)
			{
				std::set<CRange>::iterator si = iter->second.begin();
				for (; si != iter->second.end(); ++si)
				{
					max_soln = max(max_soln, si->nMax);
				}
			}
			(*this)[CKeyword::K_SOLUTION].insert(CRange(max_soln + 1, max_soln + no_soln_num_count));
		}
		//
		// process previous sim for copies
		//
		CTreeCtrlNode prevSim = sim.GetPrevSibling();
		if (!bSkipCopys && prevSim)
		{
			// foreach key
			for (CTreeCtrlNode key = prevSim.GetChild(); key != 0; key = key.GetNextSibling())
			{
				if (key.GetImageID() != CTreeCtrlIn::copyImage) continue;

				int n_user, n_user_end;
				char *description;
				CString strKey = key.GetText();			

				// strip off "COPY" keyword
				ASSERT(strKey.GetLength() > (int)::strlen("COPY"));
				strKey = strKey.Mid(::strlen("COPY"));
				strKey.TrimLeft();

				// read keyword
				p.read_number_description(strKey.GetBuffer(strKey.GetLength() + 1),
					&n_user, &n_user_end, &description);
				ASSERT(n_user == n_user_end); // COPY does not accept a range of numbers for source index

				ASSERT(description);
				if (description)
				{
					CString strRange(_T("ignored_token "));
					strRange += description;
					p.PHRQ_free(description);
					p.read_number_description(strRange.GetBuffer(strRange.GetLength() + 1),
						&n_user, &n_user_end, &description);
					p.PHRQ_free(description);

					CKeyword::type nType = CKeyword::GetKeywordType(strKey.SpanExcluding(_T(" \t")));
					ASSERT(nType != CKeyword::K_NOT_KEYWORD || strKey.SpanExcluding(_T(" \t")).CompareNoCase("cell") == 0);
					if (nType != CKeyword::K_NOT_KEYWORD)
					{
						(*this)[nType].insert(CRange(n_user, n_user_end));
					}
				}
			}
		}
		if (sim == simNode) break;
	}
}

CDefinedRanges::~CDefinedRanges()
{
}

int CDefinedRanges::ReadSolutionSpread(CString& rStr)
{
	PhreeqcI phreeqci(rStr);
	return phreeqci.GetDefinedRanges(this);
}


//////////////////////////////////////////////////////////////////////
// CRange Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRange::CRange()
: nMin(-1), nMax(-1)
{
	ASSERT(nMin <= nMax);
}

CRange::CRange(int nStart, int nEnd)
: nMin(nStart), nMax(nEnd)
{
	ASSERT(nMin <= nMax);
}

CRange::CRange(LPCTSTR lpsz)
: nMin(-1), nMax(-1)
{
	if (lpsz == 0 || _tcslen(lpsz) == 0)
		return;

	CString strRange(lpsz);
	switch (strRange.Replace(_T("-"), _T(" ")))
	{
	case 0:
		{
			int min;
			if (_stscanf(strRange, "%d", &min) == 1)
			{
				nMin = min;
				nMax = min;
			}
		}
		break;

	case 1:
		{
			int min, max;
			if (::_stscanf(strRange, "%d %d", &min, &max) == 2)
			{
				nMin = min;
				nMax = max;
			}

		}
		break;

	default:
		// Invalid range
		break;
	}
}

CRange::~CRange()
{
}

bool CRange::operator<(const CRange& rRange) const
{
	if (this->nMin == rRange.nMin)
	{
		return (this->nMax < rRange.nMax);
	}
	else
	{
		return (this->nMin < rRange.nMin);
	}
}

CRange CRange::Remove(int nVal)
{
	if (IsEmpty() || !IsValid())
	{
		return CRange();
	}

	if (nMin == nMax)
	{
		if (nMin == nVal)
		{
			nMin = nMax = -1;
		}
		return CRange();
	}
	else
	{
		if (!Contains(nVal))
		{
			// nothing to do
			return CRange();
		}
		else
		{
			if (nVal == nMax) // check endpoint
			{
				nMax--;
				ASSERT(nMin <= nMax);
				return CRange(); // return empty range
			}
			else
			{
				CRange range(*this);
				if (nVal == nMin) // check beginpoint
				{
					range.nMin++;
					nMin = nMax = -1;  // set (*this) to empty
					ASSERT(range.nMin <= range.nMax);
				}
				else
				{
					nMax = nVal - 1;
					range.nMin = nVal + 1;
				}
				return range;
			}
		}
	}
}


bool CRange::IsEmpty() const
{
	return (nMin == -1 && nMax == -1);
}

bool CRange::IsValid() const
{
	return (nMin != -1 && nMax != -1 && nMin <= nMax);
}

bool CRange::Contains(int nVal) const
{
	return (IsValid() && (nMin <= nVal && nVal <= nMax));
}


CString CRange::toCString() const
{
	CString strRange;
	if (IsEmpty())
	{
		strRange = _T("Empty Range");
	}
	if (!IsValid())
	{
		strRange = _T("Invalid Range");
	}
	if (nMin != nMax)
	{
		strRange.Format(_T("%d-%d"), nMin, nMax);
	}
	else
	{
		strRange.Format(_T("%d"), nMin);
	}
	return strRange;
}

std::string CRange::toString() const
{
	std::string sRange = (LPCTSTR)toCString();
	return sRange;
}

