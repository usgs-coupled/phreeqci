// Database.h: interface for the CDatabase class.
//
// $Id$
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATABASE_H__ABCAA3BA_D92C_44C6_AFC2_5D644C53FAE7__INCLUDED_)
#define AFX_DATABASE_H__ABCAA3BA_D92C_44C6_AFC2_5D644C53FAE7__INCLUDED_


#include "DBObject.h"

// forward declaration
class CLineParser;
class CLogicalLineParser;
class CPhreeqcCallback;

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 
// CDatabase holds data specified within a phreeqc database file
//
class CDatabase : public CObject  
{
// inner class
	class CLoader  
	{
	public:
	// inner class
		class CCookie  
		{
		public:
			CLogicalLineParser& m_rLogicalLineParser;
			CCookie(CLogicalLineParser& rLogicalLineParser);
			virtual ~CCookie();

		};
	public:
		static BOOL CALLBACK LineCallBack(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);
		CLoader(LPCTSTR lpszFileName);
		CLoader(CRichEditCtrl* pRichEditCtrl, int nSimulation);
		virtual ~CLoader();

	protected:
		CLineParser* m_pLineParser;
		CLogicalLineParser* m_pLogicalLineParser;
		CPhreeqcCallback* m_pPhreeqcCallback;
	};
	class CLoader2  
	{
	public:
		CLoader2(LPCTSTR lpszFileName);
		CLoader2(CRichEditCtrl* pRichEditCtrl, int nSimulation);
		virtual ~CLoader2();
		static int ReadCallBack(void *cookie);
		static int WriteCallBack(const int action, const int type, const char *err_str, const int stop, void *cookie, const char *format, va_list args);
	protected:
		CStdioFile     m_inputFile;
		CString        m_strErr;
		CString        m_strOther;
		CString        m_strAll;
		CRichEditCtrl *m_pRichEditCtrl;
		TEXTRANGE      m_tr;
		long           m_nTextLength;
	};

public:
	CString GetPathName()const;
	BOOL DenyWrite();
	void Merge(const CDatabase& rDatabase);
	BOOL Load(LPCTSTR lpszPathName);
	BOOL Load(CRichEditCtrl* pRichEditCtrl, int nSimulation);

	// sets
	std::set<CDBElement>  m_elementSet;
	std::set<CDBPhase>    m_phaseSet;
	std::set<CDBRate>     m_rateSet;
	std::set<CDBRedox>    m_redoxSet;
	std::set<CDBSS>       m_solidSolutionSet;
	std::set<CDBSpecies>  m_speciesSet;
	std::set<CDBSpecies>  m_speciesExSet;
	std::set<CDBSpecies>  m_speciesSurfSet;
	std::set<CDBNamedExp> m_namedExpSet;
	std::set<CDBCalcVal>  m_calcValSet;	

	CDatabase();
	CDatabase(const CDatabase& rDatabase);	// copy ctor
	virtual ~CDatabase();
protected:
	CString m_strPathName;
	void CopyPhreeqcStructs();
private:
	CFile* m_pFile;
};

template<class T>inline 
void merge_list(std::list<T>& L1, const std::list<T>& L2)
{
	if (L2.size() == 0) return;
	if (L1.size() == 0)
	{
		L1.insert(L1.begin(), L2.begin(), L2.end());
		return;
	}
	std::list<T>::const_iterator it = L2.begin();
	for (; it != L2.end(); ++it)
	{
		std::list<T>::iterator pos = 
			std::lower_bound(L1.begin(), L1.end(), *it);
		if ( pos != L1.end() && !( *it < *pos ) ) {
			ASSERT( std::binary_search(L1.begin(), L1.end(), *it) == true );
			// item found don't add it
		} else {
			ASSERT( std::binary_search(L1.begin(), L1.end(), *it) == false );
			// item not found insert it
			L1.insert(pos, *it);
		}
	}
}

template<class T>inline 
void merge_set(std::set<T>& S1, const std::set<T>& S2)
{
	std::set<T>::const_iterator it = S2.begin();
	for (; it != S2.end(); ++it)
	{
		S1.insert(*it);
	}
}

#endif // !defined(AFX_DATABASE_H__ABCAA3BA_D92C_44C6_AFC2_5D644C53FAE7__INCLUDED_)
