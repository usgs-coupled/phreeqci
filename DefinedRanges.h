// DefinedRanges.h: interface for the CDefinedRanges class.
//
// $Date: 2004/10/23 01:35:35 $
// $Revision: 1.4 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEFINEDRANGES_H__77824312_546E_48CE_81FC_D04F6171897A__INCLUDED_)
#define AFX_DEFINEDRANGES_H__77824312_546E_48CE_81FC_D04F6171897A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Keyword.h"
#include "Tree.h"

class CRange
{
public:
	CRange();
	CRange(int nStart, int nEnd);
	CRange(LPCTSTR lpsz);
	virtual ~CRange();
	int nMin;
	int nMax;

	bool operator<(const CRange& rDBObject) const;
	CString toCString() const;
	std::string toString() const;
	bool Contains(int nVal) const;
	bool IsValid() const;
	bool IsEmpty() const;
	CRange Remove(int nVal);
};

class CRepeat
{
public:
	CRepeat(): stCount(0), dValue(std::numeric_limits<double>::signaling_NaN())
	{
		strValue.Format("%.*g", DBL_DIG, dValue);
	}

	CRepeat(double dValue): stCount(1), dValue(dValue)
	{
		strValue.Format("%.*g", DBL_DIG, dValue);
	}

	~CRepeat(){} // won't be subclassed so doesn't need to be virtual

	void Increment() {stCount++;}
	void Decrement() {stCount--;}

	size_t GetCount() const {return stCount;}

	double GetDValue() const {return dValue;}
	
	double SetDValue(double dValue)	{
		this->dValue = dValue;
		strValue.Format("%.*g", DBL_DIG, this->dValue);
	}

	CString GetStrValue() const {return strValue;}

	CString toCString() const {
		CString strFormat;
		if (stCount == 1)
		{
			strFormat.Format("%.*g", DBL_DIG, dValue);
		}
		else if (stCount > 1)
		{
			strFormat.Format("%d*%.*g", stCount, DBL_DIG, dValue);
		}
		return strFormat;
	}
private:
	size_t stCount;
	double dValue;
	CString strValue;
};

class CDefinedRanges : public std::map< CKeyword::type, std::set<CRange> >
{
public:
	CDefinedRanges(const CTreeCtrlNode& rSimNode, bool bSkipSaves = false, bool bSkipCopys = false);
	virtual ~CDefinedRanges();
private:
	int ReadSolutionSpread(CString& rStr);
};

#endif // !defined(AFX_DEFINEDRANGES_H__77824312_546E_48CE_81FC_D04F6171897A__INCLUDED_)
