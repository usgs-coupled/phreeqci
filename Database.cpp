// Database.cpp: implementation of the CDatabase class.
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "Database.h"

#ifdef SAVE
#include "StdioFileLineParser.h"
#include "RichEditLineParser.h"
#include "RichLogicalLineParser.h"
#include "PhreeqcCallback.h"
#endif

extern "C" {
#include "phreeqc/src/input.h"
#include "phreeqc/src/output.h"
#include "phreeqc/src/phrqproto.h"	
DWORD load_database2(void *cookie, PFN_READ_CALLBACK pfnRead, PFN_OUTPUT_CALLBACK pfnWrite);
DWORD load_database_local2(void *cookie, PFN_READ_CALLBACK pfnRead, PFN_OUTPUT_CALLBACK pfnWrite, int last_simulation);
}


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDatabase::CDatabase() : m_pFile(NULL)
{

}

CDatabase::CDatabase(const CDatabase& rDatabase)
{
	m_pFile = NULL;
	m_strPathName = rDatabase.m_strPathName;
	if (rDatabase.m_pFile != NULL)
	{
		DenyWrite();
	}

	// don't load before using a copy ctor
	ASSERT(rDatabase.m_elementSet.empty());
	ASSERT(rDatabase.m_phaseSet.empty());
	ASSERT(rDatabase.m_rateSet.empty());
	ASSERT(rDatabase.m_redoxSet.empty());
	ASSERT(rDatabase.m_speciesSet.empty());
	ASSERT(rDatabase.m_speciesExSet.empty());
	ASSERT(rDatabase.m_speciesSurfSet.empty());

	ASSERT(m_elementSet.empty());
	ASSERT(m_phaseSet.empty());
	ASSERT(m_rateSet.empty());
	ASSERT(m_redoxSet.empty());
	ASSERT(m_speciesSet.empty());
	ASSERT(m_speciesExSet.empty());
	ASSERT(m_speciesSurfSet.empty());
}

CDatabase::~CDatabase()
{
	if (m_pFile)
	{
		ASSERT(!m_strPathName.IsEmpty());
		delete m_pFile;
	}
}

BOOL CDatabase::Load(LPCTSTR lpszPathName)
{
	CLoader2 loader(lpszPathName);

	ASSERT(m_elementSet.empty());
	ASSERT(m_phaseSet.empty());
	ASSERT(m_rateSet.empty());
	ASSERT(m_redoxSet.empty());
	ASSERT(m_solidSolutionSet.empty());
	ASSERT(m_speciesSet.empty());
	ASSERT(m_speciesExSet.empty());
	ASSERT(m_speciesSurfSet.empty());

	CopyPhreeqcStructs();

	ASSERT(m_strPathName.IsEmpty());
	m_strPathName = lpszPathName;
	// VERIFY(DenyWrite());
	
	return TRUE;
}

BOOL CDatabase::Load(CRichEditCtrl* pRichEditCtrl, int nSimulation)
{
	CLoader2 loader2(pRichEditCtrl, nSimulation);

	// clear all sets before CopyPhreeqcStructs
	m_elementSet.clear();
	m_phaseSet.clear();
	m_rateSet.clear();
	m_redoxSet.clear();
	m_solidSolutionSet.clear();
	m_speciesSet.clear();
	m_speciesExSet.clear();
	m_speciesSurfSet.clear();

	CopyPhreeqcStructs();
	
	return TRUE;
}

void CDatabase::CopyPhreeqcStructs()
{
	/* list_all_elements */
	ASSERT(m_elementSet.empty());
	struct element** elemIter;
	for ( elemIter = elements; elemIter < elements + count_elements; ++elemIter )
	{
		if ( (*elemIter)->master != NULL )
		{
			m_elementSet.insert(m_elementSet.end(), *elemIter);
		}
	}

	/* list_phases */
	ASSERT(m_phaseSet.empty());
	struct phase** phaseIter;
	for (phaseIter = phases; phaseIter < phases + count_phases; ++phaseIter)
	{
		if ((*phaseIter)->type != SOLID)
			continue;
		m_phaseSet.insert(m_phaseSet.end(), *phaseIter);

	}

	/* list_rates */
	ASSERT(m_rateSet.empty());
	struct rate* rateIter;
	for (rateIter = rates; rateIter < rates + count_rates; ++rateIter)
	{
		m_rateSet.insert(rateIter);
	}

	// list_redox
	int i, j, k;
	CString str;
	ASSERT(m_redoxSet.empty());
	for (i = 0; i < count_master; ++i)
	{
		if(master[i]->primary == TRUE)
		{
			for (j = i + 1; j < count_master; ++j)
			{
				if ( master[j]->primary == TRUE ) break;
				for ( k = j + 1; k < count_master; ++k)
				{
					if ( master[k]->primary == TRUE ) break;
					if ( master[j]->elt == NULL ) break;
					str.Format(_T("%s/%s"), master[j]->elt->name, master[k]->elt->name);
					m_redoxSet.insert(m_redoxSet.end(), (LPCTSTR)str);
				}
			}
		}
	}
	m_redoxSet.insert(m_redoxSet.begin(), _T("pe"));

	// list_species
	ASSERT(m_speciesSet.empty());
	ASSERT(m_speciesExSet.empty());
	ASSERT(m_speciesSurfSet.empty());
	struct species** specIter;
	for (specIter = s; specIter < s + count_s; ++specIter)
	{
		m_speciesSet.insert(m_speciesSet.end(), *specIter);

		if ((*specIter)->type == EX
			&&
			(*specIter)->primary == NULL)
		{
			m_speciesExSet.insert(m_speciesExSet.end(), *specIter);
		}

		if ((*specIter)->type == SURF
			&&
			(*specIter)->primary == NULL)
		{
			m_speciesSurfSet.insert(m_speciesSurfSet.end(), *specIter);
		}
	}


	ASSERT(m_solidSolutionSet.empty());
	struct s_s_assemblage* s_s_assembIter;
	for (s_s_assembIter = s_s_assemblage; s_s_assembIter < s_s_assemblage + count_s_s_assemblage; ++s_s_assembIter)
	{
		struct s_s *s_sIter = s_s_assembIter->s_s;
		for (; s_sIter < s_s_assembIter->s_s + s_s_assembIter->count_s_s; ++s_sIter)
		{
			m_solidSolutionSet.insert(s_sIter); 
		}
	}

	//{{
	// EXTERNAL struct kinetics *kinetics;
	// EXTERNAL int count_kinetics;
	//}}

	// NAMED_EXPRESSIONS
	ASSERT(m_namedExpSet.empty());
	for (i = 1; i < count_logk; ++i)
	{
		m_namedExpSet.insert(logk[i]); 
	}

	// CALCULATE_VALUES
	ASSERT(m_calcValSet.empty());
	for (i = 0; i < count_calculate_value; ++i)
	{
		m_calcValSet.insert(calculate_value[i]); 
	}


}

void CDatabase::Merge(const CDatabase& rDatabase)
{
	// Note: (*this) should be the database contained within
	// the input file.  std::set doesn't allow duplicates
	// and will therefore not replace an object it already
	// contains
	merge_set(m_elementSet,       rDatabase.m_elementSet);
	merge_set(m_phaseSet,         rDatabase.m_phaseSet);
	merge_set(m_rateSet,          rDatabase.m_rateSet);
	merge_set(m_redoxSet,         rDatabase.m_redoxSet);
	merge_set(m_speciesSet,       rDatabase.m_speciesSet);
	merge_set(m_speciesExSet,     rDatabase.m_speciesExSet);
	merge_set(m_speciesSurfSet,   rDatabase.m_speciesSurfSet);
	merge_set(m_solidSolutionSet, rDatabase.m_solidSolutionSet);
	merge_set(m_namedExpSet,      rDatabase.m_namedExpSet);
	merge_set(m_calcValSet,       rDatabase.m_calcValSet);
}

BOOL CDatabase::DenyWrite()
{
	BOOL bOK = FALSE;

	if (!m_strPathName.IsEmpty())
	{
		try
		{
			if (m_pFile == NULL)
			{
				m_pFile = new CFile(m_strPathName, CFile::modeRead | CFile::shareDenyWrite);
			}
			bOK = TRUE;
		}
		catch (CFileException* pEx)
		{
			ASSERT(FALSE);
			ASSERT(m_pFile == NULL);
			m_pFile = NULL;
			TCHAR   szCause[255];
			CString strFormatted;
			pEx->GetErrorMessage(szCause, 255);
			strFormatted = _T("The data file could not be opened because of this error: ");
			strFormatted += szCause;
			TRACE(strFormatted);
			bOK = FALSE;
		}
	}

	return bOK;
}

#if defined(SAVE)

//////////////////////////////////////////////////////////////////////
// CLoader Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDatabase::CLoader::CLoader(LPCTSTR lpszFileName)
{
	ASSERT(FALSE); // DEPRECATED

	m_pLineParser        = NULL;
	m_pLogicalLineParser = NULL;
	m_pPhreeqcCallback   = NULL;

	// Create parser
	m_pLineParser = new CStdioFileLineParser(lpszFileName);
	m_pLogicalLineParser = new CLogicalLineParser(*m_pLineParser);

	CCookie cookie(*m_pLogicalLineParser);

	GUICALLBACK gui = {(DWORD)&cookie, LineCallBack, NULL/*ErrorCallBack*/, NULL/*WarnCallBack*/};

	m_pPhreeqcCallback = new CPhreeqcCallback(gui);

	// let phreeqc do the work
	m_pPhreeqcCallback->Go(load_database);
}

CDatabase::CLoader::CLoader(CRichEditCtrl* pRichEditCtrl, int nSimulation)
{
	ASSERT(FALSE); // DEPRECATED

	m_pLineParser        = NULL;
	m_pLogicalLineParser = NULL;
	m_pPhreeqcCallback   = NULL;

	// Create parser
	CRichEditLineParser* pRichEditLineParser = new CRichEditLineParser(pRichEditCtrl);
	m_pLineParser = pRichEditLineParser;
	m_pLogicalLineParser = new CRichLogicalLineParser(*pRichEditLineParser);

	CCookie cookie(*m_pLogicalLineParser);
	GUICALLBACK gui = {(DWORD)&cookie, LineCallBack, NULL/*ErrorCallBack*/, NULL/*WarnCallBack*/};

	m_pPhreeqcCallback = new CPhreeqcCallback(gui);

	// let phreeqc do the work
	m_pPhreeqcCallback->Go(load_database_local, nSimulation);
}

CDatabase::CLoader::~CLoader()
{
	// do phreeqc clean_up;
	clean_up();

	// delete pointers created with new
	if (m_pPhreeqcCallback)
	{
		delete m_pPhreeqcCallback;
	}
	if (m_pLogicalLineParser)
	{
		delete m_pLogicalLineParser;
	}
	if (m_pLineParser)
	{
		delete m_pLineParser;
	}
}

//
//
//
// returns		TRUE if line was copied successfully
//				FALSE if a larger buffer is required
//					pcb contains the required buffer size
BOOL CALLBACK CDatabase::CLoader::LineCallBack(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	ASSERT(FALSE); // DEPRECATED

	static CString strLine;

	CDatabase::CLoader::CCookie* pCookie = (CDatabase::CLoader::CCookie*) dwCookie;

	// strLine.IsEmpty() == FALSE signifies that the last call returned FALSE
	// and the strLine has not been copied yet
	if (!strLine.IsEmpty() || pCookie->m_rLogicalLineParser.GetNextLine(strLine))
	{
		*pcb = strLine.GetLength() + 1;			// GetLength does not include NULL terminator
		if (*pcb <= cb)
		{
			strncpy((LPSTR)pbBuff, (LPCSTR)strLine, *pcb);
			strLine = &afxChNil;    // empty string without deallocating
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		strLine.Empty();			// empty string and deallocate
		*pcb = 0;	// EOF
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// CCookie Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDatabase::CLoader::CCookie::CCookie(CLogicalLineParser& rLogicalLineParser)
: m_rLogicalLineParser(rLogicalLineParser)
{
	ASSERT(FALSE); // DEPRECATED
}

CDatabase::CLoader::CCookie::~CCookie()
{

}

#endif // SAVE

CString CDatabase::GetPathName() const
{
	return m_strPathName;
}


//////////////////////////////////////////////////////////////////////
// CLoader2 Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDatabase::CLoader2::CLoader2(LPCTSTR lpszFileName)
: m_pRichEditCtrl(0)
{
	CString strMessage;

	m_inputFile.Open(lpszFileName, CFile::modeRead|CFile::shareDenyWrite);

	if (this->m_inputFile.m_pStream)
	{
		DWORD i = ::load_database2(this, ReadCallBack, WriteCallBack);
		switch (i) {
		case EXCEPTION_ACCESS_VIOLATION:
			strMessage += "Warning: An unexpected error occured while loading the database file:\n";
			strMessage += lpszFileName;
			strMessage += ".";
			::AfxMessageBox(strMessage, MB_OK|MB_ICONEXCLAMATION);
			break;
		case INPUT_CONTAINS_ERRORS:
			strMessage += "Warning: The database file:\n";
			strMessage += lpszFileName;
			strMessage += ",\n contains errors:\n";
			strMessage += this->m_strAll;
			::AfxMessageBox(strMessage, MB_OK|MB_ICONEXCLAMATION);
			break;
		}
	}
	else
	{
		strMessage += "Warning: Unable to open the following database file:\n";
		strMessage += lpszFileName;
		strMessage += ".";
		::AfxMessageBox(strMessage, MB_OK|MB_ICONEXCLAMATION);
	}
}

CDatabase::CLoader2::CLoader2(CRichEditCtrl* pRichEditCtrl, int nSimulation)
{
	CString strMessage;

	if (pRichEditCtrl == NULL) return;

	this->m_pRichEditCtrl = pRichEditCtrl;
	this->m_tr.chrg.cpMin = 0;
	this->m_nTextLength   = this->m_pRichEditCtrl->GetTextLength();

	if (this->m_pRichEditCtrl)
	{
		DWORD i = ::load_database_local2(this, ReadCallBack, WriteCallBack, nSimulation);
		switch (i) {
		case EXCEPTION_ACCESS_VIOLATION:
			ASSERT(FALSE);
			// ignore for now
			break;
		case INPUT_CONTAINS_ERRORS:
			ASSERT(FALSE);
			// ignore for now
			break;
		}
	}
}


CDatabase::CLoader2::~CLoader2()
{
	::clean_up();
}

int CDatabase::CLoader2::ReadCallBack(void *cookie)
{
	ASSERT(cookie);
	CLoader2* pThis = (CLoader2*)cookie;
	if (pThis->m_inputFile.m_pStream)
	{
		return ::getc(pThis->m_inputFile.m_pStream);
	}
	else if (pThis->m_pRichEditCtrl)
	{
		static char buffer[8];
		static long nChars;
		pThis->m_tr.chrg.cpMax = min(pThis->m_tr.chrg.cpMin + 2, pThis->m_nTextLength);
		ASSERT((pThis->m_tr.chrg.cpMax - pThis->m_tr.chrg.cpMin) < 8);
		if (pThis->m_tr.chrg.cpMin < pThis->m_tr.chrg.cpMax)
		{
			pThis->m_tr.lpstrText = buffer;
			nChars = pThis->m_pRichEditCtrl->SendMessage(EM_GETTEXTRANGE, 0, (LPARAM)&pThis->m_tr);
			ASSERT(nChars == 2 || nChars == 1);
			if (nChars == 2 || nChars == 1) 
			{
				++pThis->m_tr.chrg.cpMin;
				if (buffer[0] == '\r' && buffer[1] == '\n') {
					++pThis->m_tr.chrg.cpMin;
					return '\n';
				}
				return buffer[0];
			}
		}
	}
	return EOF;
}

int CDatabase::CLoader2::WriteCallBack(const int action, const int type, const char *err_str, const int stop, void *cookie, const char *format, va_list args)
{
#ifdef _DEBUG
	const size_t length = 500;
	static char buffer[length];
#endif

	CLoader2* pThis = (CLoader2*)cookie;

	if (action == ACTION_OUTPUT)
	{
		switch (type)
		{
		case OUTPUT_ERROR:
		case OUTPUT_WARNING:
			pThis->m_strErr += err_str;
			pThis->m_strErr += "\n";
			pThis->m_strAll += err_str;
			pThis->m_strAll += "\n";
			break;
		default:
#ifdef _DEBUG
			::_vsntprintf(buffer, length, format, args);
			::OutputDebugString(buffer);
#endif
			break;
		}

		if (stop == STOP) {
			::OutputDebugString("An error occured reading the database:\n");
			::RaiseException(INPUT_CONTAINS_ERRORS, 0, 0, NULL);
		}
	}
	return 0;
}
