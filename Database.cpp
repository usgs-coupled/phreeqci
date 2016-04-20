// Database.cpp: implementation of the CDatabase class.
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "Database.h"
#include "SeException.h"
#include "phreeqc3/src/SSassemblage.h"


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
	ASSERT(rDatabase.m_speciesAqSet.empty());
	ASSERT(rDatabase.m_speciesAqCationSet.empty());
	ASSERT(rDatabase.m_speciesAqAnionSet.empty());
	ASSERT(rDatabase.m_speciesAqNeutralSet.empty());
	ASSERT(rDatabase.m_speciesExSet.empty());
	ASSERT(rDatabase.m_speciesSurfSet.empty());

	ASSERT(m_elementSet.empty());
	ASSERT(m_phaseSet.empty());
	ASSERT(m_rateSet.empty());
	ASSERT(m_redoxSet.empty());
	ASSERT(m_speciesSet.empty());
	ASSERT(m_speciesAqSet.empty());
	ASSERT(m_speciesAqCationSet.empty());
	ASSERT(m_speciesAqAnionSet.empty());
	ASSERT(m_speciesAqNeutralSet.empty());
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
	CLoader3 loader(lpszPathName);

	ASSERT(m_elementSet.empty());
	ASSERT(m_phaseSet.empty());
	ASSERT(m_rateSet.empty());
	ASSERT(m_redoxSet.empty());
	ASSERT(m_solidSolutionSet.empty());
	ASSERT(m_speciesSet.empty());
	ASSERT(m_speciesAqSet.empty());
	ASSERT(m_speciesAqCationSet.empty());
	ASSERT(m_speciesAqAnionSet.empty());
	ASSERT(m_speciesAqNeutralSet.empty());
	ASSERT(m_speciesExSet.empty());
	ASSERT(m_speciesSurfSet.empty());

#ifdef SAVE_OLD_IO
	CopyPhreeqcStructs();
#else
	loader.CopyPhreeqcStructs(this);
#endif

	ASSERT(m_strPathName.IsEmpty());
	m_strPathName = lpszPathName;
	// VERIFY(DenyWrite());
	
	return TRUE;
}

BOOL CDatabase::Load(CRichEditCtrl* pRichEditCtrl, int nSimulation)
{
	CLoader3 loader(pRichEditCtrl, nSimulation);

	// clear all sets before CopyPhreeqcStructs
	m_elementSet.clear();
	m_phaseSet.clear();
	m_rateSet.clear();
	m_redoxSet.clear();
	m_solidSolutionSet.clear();
	m_speciesSet.clear();
	m_speciesAqSet.clear();
	m_speciesAqCationSet.clear();
	m_speciesAqAnionSet.clear();
	m_speciesAqNeutralSet.clear();
	m_speciesExSet.clear();
	m_speciesSurfSet.clear();
	m_namedExpSet.clear();
	m_calcValSet.clear();

#ifdef SAVE_OLD_IO
	CopyPhreeqcStructs();
#else
	loader.CopyPhreeqcStructs(this);
#endif
	
	return TRUE;
}

void CDatabase::CopyPhreeqcStructs()
{
#ifdef SAVE_OLD_IO
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
	ASSERT(m_speciesAqSet.empty());
	ASSERT(m_speciesExSet.empty());
	ASSERT(m_speciesSurfSet.empty());
	struct species** specIter;
	for (specIter = s; specIter < s + count_s; ++specIter)
	{
		m_speciesSet.insert(m_speciesSet.end(), *specIter);

		if ((*specIter)->type == AQ)
		{
			m_speciesAqSet.insert(m_speciesAqSet.end(), *specIter);
			if ((*specIter)->z > 0)
			{
				m_speciesAqCationSet.insert(m_speciesAqCationSet.end(), *specIter);
			}
			else if ((*specIter)->z < 0)
			{
				m_speciesAqAnionSet.insert(m_speciesAqAnionSet.end(), *specIter);
			}
			else
			{
				m_speciesAqNeutralSet.insert(m_speciesAqNeutralSet.end(), *specIter);
			}
		}

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


#endif  // SAVE_OLD_IO
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
	merge_set(m_speciesAqSet,     rDatabase.m_speciesAqSet);
	merge_set(m_speciesAqCationSet,  rDatabase.m_speciesAqCationSet);
	merge_set(m_speciesAqAnionSet,   rDatabase.m_speciesAqAnionSet);
	merge_set(m_speciesAqNeutralSet, rDatabase.m_speciesAqNeutralSet);
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

#ifdef SAVE_EXPAND_ENVIR
	TCHAR infoBuf[32767];
	DWORD bufCharCount = 32767;
	bufCharCount = ExpandEnvironmentStrings(lpszFileName, infoBuf, 32767); 
#endif

	m_inputFile.Open(lpszFileName, CFile::modeRead|CFile::shareDenyWrite);

	if (this->m_inputFile.m_pStream)
	{
#ifdef SAVE_OLD_IO
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
#endif // SAVE_OLD_IO
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
	UNREFERENCED_PARAMETER(nSimulation);

	CString strMessage;

	if (pRichEditCtrl == NULL) return;

	this->m_pRichEditCtrl = pRichEditCtrl;
	this->m_tr.chrg.cpMin = 0;
	this->m_nTextLength   = this->m_pRichEditCtrl->GetTextLength();

	if (this->m_pRichEditCtrl)
	{
#ifdef SAVE_OLD_IO
		DWORD i = ::load_database_local2(this, ReadCallBack, WriteCallBack, nSimulation);
#else
		DWORD i = 0;
#endif
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
#ifdef SAVE_OLD_IO
	::clean_up();
#endif
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
#if _MSC_VER < 1400
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
#else
			ASSERT(nChars == 2 || nChars == 1 || nChars == 0);
			if (nChars == 2 || nChars == 1) 
			{
				++pThis->m_tr.chrg.cpMin;
				if (buffer[0] == '\r')
				{
					if (buffer[1] == '\n')
					{
						++pThis->m_tr.chrg.cpMin;
					}
					buffer[0] = '\n';
				}
				return buffer[0];
			}
#endif
		}
	}
	return EOF;
}

int CDatabase::CLoader2::WriteCallBack(const int action, const int type, const char *err_str, const int stop, void *cookie, const char *format, va_list args)
{
#ifdef _DEBUG
	const size_t length = 500;
	static char buffer[length];
#else
	UNREFERENCED_PARAMETER(action);
	UNREFERENCED_PARAMETER(type);
	UNREFERENCED_PARAMETER(err_str);
	UNREFERENCED_PARAMETER(stop);
	UNREFERENCED_PARAMETER(format);
	UNREFERENCED_PARAMETER(args);
	UNREFERENCED_PARAMETER(cookie);
#endif

#ifdef SAVE_OLD_IO
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
#endif // SAVE_OLD_IO
	return 0;
}

//////////////////////////////////////////////////////////////////////
// CLoader3 Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDatabase::CLoader3::CLoader3(LPCTSTR lpszFileName)
: RichEditCtrl(0)
{
	CString strMessage;

	//m_inputFile.Open(lpszFileName, CFile::modeRead|CFile::shareDenyWrite);
	this->ifs.open(lpszFileName);	

// COMMENT: {2/23/2012 10:57:44 PM}	if (this->m_inputFile.m_pStream)
	if (this->ifs.is_open())
	{
		try
		{
			//{{
			this->push_istream(&this->ifs, false);
			//}}
			ASSERT(this->input_error == 0);
			ASSERT(this->phast == FALSE);
			///this->do_initialize();
			this->read_database();
		}
		catch(DWORD dw)
		{
			switch (dw)
			{
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
			default:
				ASSERT(FALSE);
				break;
			}
		}
		catch (...)
		{
			ASSERT(FALSE);
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

CDatabase::CLoader3::CLoader3(CRichEditCtrl* pRichEditCtrl, int nSimulation)
{
	CString strMessage;

	if (pRichEditCtrl == NULL) return;

	this->RichEditCtrl = pRichEditCtrl;
	this->m_tr.chrg.cpMin = 0;
	this->m_nTextLength   = this->RichEditCtrl->GetTextLength();

	if (this->RichEditCtrl)
	{
		try
		{
			ASSERT(this->input_error == 0);

			this->push_istream((std::istream*)this->RichEditCtrl, false);

			ASSERT(this->phast == FALSE);

			for (this->simulation = 1; this->simulation <= nSimulation; ++this->simulation)
			{
				if (this->read_input() == EOF) break;
				this->tidy_model_gui();
			}
			this->tidy_model_gui();
			this->input_error = 0;
		}
		catch (DWORD dw)
		{
			switch (dw)
			{
			case EXCEPTION_ACCESS_VIOLATION:
				ASSERT(FALSE);
				// ignore for now
				break;
			case INPUT_CONTAINS_ERRORS:
				ASSERT(FALSE);
				// ignore for now
				break;
			default:
				ASSERT(FALSE);
				break;
			}
		}
		catch (CSeException *e)
		{
			TCHAR trcMsg[1024];
			e->GetErrorMessage(trcMsg, 1024);
			TRACE(trcMsg);
			TRACE(_T("\n"));
			e->Delete();
		}
		catch (...)
		{
			TRACE("Caught unknown exception in CLoader3\n");
		}
	}
}


CDatabase::CLoader3::~CLoader3()
{
}

int CDatabase::CLoader3::getc(void)
{
	if (this->get_istream() == &this->ifs)
	{
		return this->PhreeqcI::getc();
	}
	else if ((CRichEditCtrl*)this->get_istream() == this->RichEditCtrl)
	{
		static char buffer[8];
		static long nChars;
		this->m_tr.chrg.cpMax = min(this->m_tr.chrg.cpMin + 2, this->m_nTextLength);
		ASSERT((this->m_tr.chrg.cpMax - this->m_tr.chrg.cpMin) < 8);
		if (this->m_tr.chrg.cpMin < this->m_tr.chrg.cpMax)
		{
			this->m_tr.lpstrText = buffer;
			nChars = this->RichEditCtrl->SendMessage(EM_GETTEXTRANGE, 0, (LPARAM)&this->m_tr);
			ASSERT(nChars == 2 || nChars == 1 || nChars == 0);
			if (nChars == 2 || nChars == 1) 
			{
				++this->m_tr.chrg.cpMin;
				if (buffer[0] == '\r')
				{
					if (buffer[1] == '\n')
					{
						++this->m_tr.chrg.cpMin;
					}
					buffer[0] = '\n';
				}
				return buffer[0];
			}
		}
	}
	return EOF;
}

void CDatabase::CLoader3::warning_msg(const char *err_str)
{
	this->m_strErr += err_str;
	this->m_strErr += "\n";
	this->m_strAll += err_str;
	this->m_strAll += "\n";
}

void CDatabase::CLoader3::error_msg(const char *err_str, bool stop)
{
	this->m_strErr += err_str;
	this->m_strErr += "\n";
	this->m_strAll += err_str;
	this->m_strAll += "\n";
	if (stop)
	{
		::OutputDebugString("An error occured reading the database:\n");
		throw INPUT_CONTAINS_ERRORS;
	}
}

bool CDatabase::CLoader3::output_open(const char *file_name, std::ios_base::openmode mode)
{
	UNUSED_ALWAYS(file_name);
	UNUSED_ALWAYS(mode);
	TRACE("CDatabase::CLoader3::output_open\n");
	return true;
}

bool CDatabase::CLoader3::log_open(const char *file_name, std::ios_base::openmode mode)
{
	UNUSED_ALWAYS(file_name);
	UNUSED_ALWAYS(mode);
	TRACE("CDatabase::CLoader3::log_open\n");
	return true;
}

bool CDatabase::CLoader3::punch_open(const char *file_name, std::ios_base::openmode mode, int n_user)
{
	UNUSED_ALWAYS(file_name);
	UNUSED_ALWAYS(mode);
	UNUSED_ALWAYS(n_user);
	TRACE("CDatabase::CLoader3::punch_open\n");
	return true;
}

bool CDatabase::CLoader3::error_open(const char *file_name, const char *mode)
{
	UNUSED_ALWAYS(file_name);
	UNUSED_ALWAYS(mode);
	TRACE("CDatabase::CLoader3::error_open\n");
	return true;
}

bool CDatabase::CLoader3::dump_open(const char *file_name, std::ios_base::openmode mode)
{
	UNUSED_ALWAYS(file_name);
	UNUSED_ALWAYS(mode);
	TRACE("CDatabase::CLoader3::dump_open\n");
	return true;
}

void CDatabase::CLoader3::CopyPhreeqcStructs(CDatabase *db)
{
	// list_all_elements
	ASSERT(db->m_elementSet.empty());
	struct element** elemIter;
	for ( elemIter = elements; elemIter < elements + count_elements; ++elemIter )
	{
		if ( (*elemIter)->master != NULL )
		{
			db->m_elementSet.insert(db->m_elementSet.end(), CDBElement(*elemIter, this));
		}
	}


	// list_phases
	ASSERT(db->m_phaseSet.empty());
	struct phase** phaseIter;
	for (phaseIter = phases; phaseIter < phases + count_phases; ++phaseIter)
	{
		if ((*phaseIter)->type != SOLID)
			continue;
		db->m_phaseSet.insert(db->m_phaseSet.end(), *phaseIter);

	}

	// list_rates
	ASSERT(db->m_rateSet.empty());
	struct rate* rateIter;
	for (rateIter = rates; rateIter < rates + count_rates; ++rateIter)
	{
		db->m_rateSet.insert(rateIter);
	}

	// list_redox
	int i, j, k;
	CString str;
	ASSERT(db->m_redoxSet.empty());
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
					db->m_redoxSet.insert(db->m_redoxSet.end(), (LPCTSTR)str);
				}
			}
		}
	}
	db->m_redoxSet.insert(db->m_redoxSet.begin(), _T("pe"));

	// list_species
	ASSERT(db->m_speciesSet.empty());
	ASSERT(db->m_speciesAqSet.empty());
	ASSERT(db->m_speciesExSet.empty());
	ASSERT(db->m_speciesSurfSet.empty());
	struct species** specIter;
	for (specIter = s; specIter < s + count_s; ++specIter)
	{
		db->m_speciesSet.insert(db->m_speciesSet.end(), *specIter);

		if ((*specIter)->type == AQ)
		{
			db->m_speciesAqSet.insert(db->m_speciesAqSet.end(), *specIter);
			if ((*specIter)->z > 0)
			{
				db->m_speciesAqCationSet.insert(db->m_speciesAqCationSet.end(), *specIter);
			}
			else if ((*specIter)->z < 0)
			{
				db->m_speciesAqAnionSet.insert(db->m_speciesAqAnionSet.end(), *specIter);
			}
			else
			{
				db->m_speciesAqNeutralSet.insert(db->m_speciesAqNeutralSet.end(), *specIter);
			}
		}

		if ((*specIter)->type == EX
			&&
			(*specIter)->primary == NULL)
		{
			db->m_speciesExSet.insert(db->m_speciesExSet.end(), *specIter);
		}

		if ((*specIter)->type == SURF
			&&
			(*specIter)->primary == NULL)
		{
			db->m_speciesSurfSet.insert(db->m_speciesSurfSet.end(), *specIter);
		}
	}

	// solid solutions
	ASSERT(db->m_solidSolutionSet.empty());
	std::map<int, cxxSSassemblage>::iterator it = Rxn_ss_assemblage_map.begin();
	for (; it != Rxn_ss_assemblage_map.end(); ++it)
	{
		std::map<std::string, cxxSS>::const_iterator sit = it->second.Get_SSs().begin();
		for (; sit != it->second.Get_SSs().end(); ++sit)
		{
			db->m_solidSolutionSet.insert(&sit->second);
		}
	}

	// NAMED_EXPRESSIONS
	ASSERT(db->m_namedExpSet.empty());
	for (i = 1; i < count_logk; ++i)
	{
		db->m_namedExpSet.insert(logk[i]); 
	}

	// CALCULATE_VALUES
	ASSERT(db->m_calcValSet.empty());
	for (i = 0; i < count_calculate_value; ++i)
	{
		db->m_calcValSet.insert(calculate_value[i]); 
	}
}

int CDatabase::CLoader3::tidy_model_gui(void)
{
	int i;
#ifdef GUI_SKIP
	int n_user, last;
#endif
	int new_named_logk;
	//
	// Determine if any new elements, species, phases have been read
	//
	state = INITIALIZE;
	new_model = FALSE;
	new_pp_assemblage = FALSE;
	new_surface = FALSE;
	new_exchange = FALSE;
	new_reaction = FALSE;
	new_temperature = FALSE;
	new_mix = FALSE;
	new_solution = FALSE;
	new_gas_phase = FALSE;
	new_inverse = FALSE;
	new_punch = FALSE;
	new_surface = FALSE;
	new_ss_assemblage = FALSE;
	new_kinetics = FALSE;
	new_pitzer = FALSE;
	new_named_logk = FALSE;

	if (
		keycount[Keywords::KEY_SOLUTION_SPECIES] > 0				||	// "species"
		keycount[Keywords::KEY_SOLUTION_MASTER_SPECIES] > 0			||	// "master"
		keycount[Keywords::KEY_PHASES] > 0							||	// "phases"
		keycount[Keywords::KEY_EXCHANGE_SPECIES] > 0				||	// "exchange_species"
		keycount[Keywords::KEY_EXCHANGE_MASTER_SPECIES] > 0			||	// "master_exchange_species"
		keycount[Keywords::KEY_SURFACE_SPECIES] > 0					||	// "surface_species"
		keycount[Keywords::KEY_SURFACE_MASTER_SPECIES] > 0			||	// "master_surface_species"
		keycount[Keywords::KEY_RATES] > 0							||	// "rates"
		keycount[Keywords::KEY_LLNL_AQUEOUS_MODEL_PARAMETERS] > 0	||	// "llnl_aqueous_model_parameters"
		(keycount[Keywords::KEY_DATABASE] > 0 && simulation == 0)	||	// "database"
		keycount[Keywords::KEY_NAMED_EXPRESSIONS] > 0				||	// "named_analytical_expressions"
		keycount[Keywords::KEY_ISOTOPES] > 0						||	// "isotopes"
		keycount[Keywords::KEY_CALCULATE_VALUES] > 0				||	// "calculate_values"
		keycount[Keywords::KEY_ISOTOPE_RATIOS] > 0					||	// "isotopes_ratios",
		keycount[Keywords::KEY_ISOTOPE_ALPHAS] > 0					||	// "isotopes_alphas"
		keycount[Keywords::KEY_PITZER] > 0							||	// "pitzer"
		keycount[Keywords::KEY_SIT] > 0								    // "sit"
		)
	{							
		new_model = TRUE;
	}
	if (keycount[Keywords::KEY_EQUILIBRIUM_PHASES] > 0		|| 
		keycount[Keywords::KEY_EQUILIBRIUM_PHASES_RAW] > 0	||
		keycount[Keywords::KEY_EQUILIBRIUM_PHASES_MODIFY])
	{
		new_pp_assemblage = TRUE;					/*"pure_phases" */
	}
	if (keycount[Keywords::KEY_SURFACE] > 0					||
		keycount[Keywords::KEY_SURFACE_RAW] > 0				||
		keycount[Keywords::KEY_SURFACE_MODIFY])
	{
		new_surface = TRUE;							/*"surface" */
	}
	if (keycount[Keywords::KEY_EXCHANGE] > 0				||
		keycount[Keywords::KEY_SURFACE_RAW] > 0				||
		keycount[Keywords::KEY_SURFACE_MODIFY])
	{
		new_exchange = TRUE;						/*"exchange" */
	}
	if (keycount[Keywords::KEY_REACTION] > 0				||
		keycount[Keywords::KEY_REACTION_RAW] > 0			||
		keycount[Keywords::KEY_REACTION_MODIFY])
	{
		new_reaction = TRUE;						/*"reaction" */
	}
	if (keycount[Keywords::KEY_REACTION_TEMPERATURE] > 0		||
		keycount[Keywords::KEY_REACTION_TEMPERATURE_RAW] > 0	||
		keycount[Keywords::KEY_REACTION_TEMPERATURE_MODIFY])
	{
		new_temperature = TRUE;						/*"reacton_temperature" */
	}
	if (keycount[Keywords::KEY_MIX] > 0						||
		keycount[Keywords::KEY_MIX_RAW] > 0)	
	{
		new_mix = TRUE;								/*"mix" */
	}
	if (keycount[Keywords::KEY_SOLUTION] > 0 ||			
		keycount[Keywords::KEY_SOLUTION_SPREAD] > 0			||
		keycount[Keywords::KEY_SOLUTION_RAW] > 0			||
		keycount[Keywords::KEY_SOLUTION_MODIFY])
	{												/*"solution" */
		new_solution = TRUE;
	}
	if (keycount[Keywords::KEY_GAS_PHASE]  > 0				||
		keycount[Keywords::KEY_GAS_PHASE_RAW] > 0			||
		keycount[Keywords::KEY_GAS_PHASE_MODIFY])
	{
		new_gas_phase = TRUE;						/*"gas_phase" */
	}
	if (keycount[Keywords::KEY_SOLID_SOLUTIONS] > 0			||
		keycount[Keywords::KEY_SOLID_SOLUTIONS_RAW] > 0		||
		keycount[Keywords::KEY_SOLID_SOLUTIONS_MODIFY])
	{
		new_ss_assemblage = TRUE;					/*"solid_solutions" */
	}
	if (keycount[Keywords::KEY_KINETICS] > 0				||
		keycount[Keywords::KEY_KINETICS_RAW] > 0			||
		keycount[Keywords::KEY_KINETICS_MODIFY])
	{
		new_kinetics = TRUE;						/*"kinetics" */
	}
	if (keycount[Keywords::KEY_INVERSE_MODELING] > 0)
	{
		new_inverse = TRUE;							/*"inverse_modeling" */
	}
	if (keycount[Keywords::KEY_SELECTED_OUTPUT] > 0			||		/*"selected_output" */
		keycount[Keywords::KEY_USER_PUNCH] > 0)						/*"user_punch" */
	{
		new_punch = TRUE;
	}

	if (keycount[Keywords::KEY_COPY] > 0)
	{
		new_copy = TRUE;							/*"copy" */
	}
	if (keycount[Keywords::KEY_PITZER] > 0)
	{
		new_pitzer = TRUE;							/*"pitzer" */
	}
	if (keycount[Keywords::KEY_NAMED_EXPRESSIONS] > 0)
	{
		new_named_logk = TRUE;						/*"named_log_k" */
	}

/*
 *   Sort arrays
 */

/* species */
	if (new_model == TRUE)
	{
		qsort(s, (size_t) count_s, (size_t) sizeof(struct species *), s_compare);

/* master species */
		qsort(master, (unsigned) count_master, sizeof(struct master *), master_compare);

/* elements */
		qsort(elements, (size_t) count_elements, (size_t) sizeof(struct element *), element_compare);
/* phases */
		qsort(phases, (size_t) count_phases, (size_t) sizeof(struct phase *), phase_compare);

	}

/* reset Peng-Robinson parms... */
	for (i = 0; i < count_phases; i++)
	{
		phases[i]->pr_in = false;
		phases[i]->pr_p = 0.0;
		phases[i]->pr_phi = 1.0;
	}

	/* named_log_k */
	if (new_named_logk)
	{
		tidy_logk();
	}
/*
 *   Check pointers, write reactions for species
 */
	if (new_model)
	{
		tidy_species_gui();

#ifdef GUI_SKIP
		tidy_species();

		tidy_phases();

		tidy_master_isotope();
/*
 *   calculate gfw of water, kg/mole
 */
		compute_gfw("H2O", &gfw_water);
		gfw_water *= 0.001;
#endif // GUI_SKIP
	}
#ifdef GUI_SKIP
/*
 *   tidy surface data
 */
	if (new_model || new_surface)
	{
		tidy_surface();
	}
/*
 *   tidy inverse data
 */
	if (new_inverse)
	{
		tidy_inverse();
	}
/*
 *   tidy gas phase data
 */
	if (new_gas_phase)
	{
		tidy_gas_phase();
	}
/* reset Peng-Robinson parms... */
	for (i = 0; i < count_phases; i++)
	{
		phases[i]->pr_in = false;
		phases[i]->pr_p = 0.0;
		phases[i]->pr_phi = 1.0;
	}
/*
 *   tidy pp_assemblage data
 */
	if (new_model || new_pp_assemblage)
	{
		tidy_pp_assemblage();
	}
/*
 *   tidy ss_assemblage data
 */
	if (new_model || new_ss_assemblage)
	{
		tidy_ss_assemblage();
	}
/*
 *   tidy exchange data, after pp_assemblages
 */
	if (new_exchange)
	{
		tidy_exchange();
	}
	if (new_exchange)
	{
		tidy_min_exchange();
	}
	if (new_exchange)
	{
		tidy_kin_exchange();
	}
/*
 *   tidy surface data
 */
	if (new_surface)
	{
		tidy_min_surface();
	}
	if (new_surface)
	{
		tidy_kin_surface();
	}
/*
 *   tidy solution isotope data
 */
	if (new_solution)
	{
		tidy_isotopes();
	}
	if (new_model)
	{
		tidy_isotope_ratios();
	}
	if (new_model)
	{
		tidy_isotope_alphas();
	}
#endif // GUI_SKIP
/*
 *   Duplicate kinetics
 */
#ifdef GUI_SKIP
	if (new_kinetics)
	{
		std::map<int, cxxKinetics>::iterator it;
		for (it = Rxn_kinetics_map.begin(); it != Rxn_kinetics_map.end(); it++)
		{
			n_user = it->second.Get_n_user();
			last = it->second.Get_n_user_end();
			it->second.Set_n_user_end(n_user);
			Utilities::Rxn_copies(Rxn_kinetics_map, n_user, last);
		}
	}
#endif // GUI_SKIP

#ifdef GUI_SKIP
/*
 *   Tidy pitzer information
 */
	if (pitzer_model && new_model)
	{
		pitzer_tidy();
	}
/*
 *   Tidy SIT information
 */
	if (sit_model && new_model)
	{
		sit_tidy();
	}
/*
 *   Tidy punch information
 */
	if (get_input_errors() == 0 && (new_punch || new_model))
	{
		tidy_punch();
	}
/*
 *   Tidy solution information
 */
	if (new_solution)
	{
		tidy_solutions();
	}

	/*      if (new_model || new_exchange || new_pp_assemblage || new_surface || new_gas_phase || new_kinetics) reset_last_model(); */
	if (new_model)
	{
		reset_last_model();
	}
#endif // GUI_SKIP

/*
 *   make sure essential species are defined
 */
#ifdef GUI_SKIP
	if (s_h2o == NULL)
	{
		input_error++;
		error_msg("H2O not defined.", STOP);
	}
	if (s_h2o->primary == NULL)
	{
		input_error++;
		error_msg("H2O, primary master species for O, not defined.",
				  CONTINUE);
	}
	if (s_h2o->secondary == NULL)
	{
		input_error++;
		error_msg("H2O, secondary master species for O(-2), not defined.",
				  CONTINUE);
	}
	if (s_hplus == NULL && s_h3oplus == NULL)
	{
		input_error++;
		error_msg("Neither H+ nor H3O+ are defined in solution_species.",
				  STOP);
	}
	else if (s_hplus == NULL && s_h3oplus != NULL)
	{
		s_hplus = s_h3oplus;
		s_h3oplus = NULL;
	}
	else if (s_hplus != NULL && s_h3oplus == NULL)
	{
	}
	else if (s_hplus != NULL && s_h3oplus != NULL)
	{
		input_error++;
		error_msg("Can not define both H+ and H3O+ in solution_species.",
				  STOP);
	}
	if (s_hplus->primary == NULL)
	{
		input_error++;
		error_msg("H3O+, primary master species for H, not defined.",
				  CONTINUE);
	}
	if (s_hplus->secondary == NULL)
	{
		input_error++;
		error_msg("H3O+, secondary master species for H(1), not defined.",
				  CONTINUE);
	}
	if (s_eminus == NULL)
	{
		input_error++;
		error_msg("e- not defined in solution_species.", CONTINUE);
	}
	if (s_eminus->primary == NULL)
	{
		input_error++;
		error_msg("e-, primary master species for E-, not defined.",
				  CONTINUE);
	}
	if (pitzer_model == FALSE || pitzer_pe == TRUE)
	{
		if (s_h2 == NULL)
		{
			input_error++;
			error_msg("H2(aq) not defined in solution_species.", CONTINUE);
		}
		if (s_o2 == NULL)
		{
			input_error++;
			error_msg("O2(aq) not defined in solution_species.", CONTINUE);
		}
	}
	element_h_one = element_store("H(1)");
	if (element_h_one == NULL)
	{
		input_error++;
		error_msg("H(1) not defined in solution_master_species.", CONTINUE);
	}
/*
 *   Error check, program termination
 */
	if (get_input_errors() > 0 || parse_error > 0)
	{
		error_msg("Calculations terminating due to input errors.", STOP);
	}
#endif // GUI_SKIP

	return (OK);
}

int CDatabase::CLoader3::tidy_species_gui(void)
{
#ifdef SKIP_GUI
	int i, j;
	struct master *master_ptr;
#else   // SKIP_GUI
	int i;
#endif  // SKIP_GUI
	char c, *ptr;
/*
 *   Make sure species pointers are ok
 */
#ifdef SKIP_GUI
	if (check_species_input() == ERROR)
	{
		error_msg("Calculations terminating due to input errors.", STOP);
	}
#endif  // SKIP_GUI
/*
 *   Set secondary and primary pointers in species structures
 */
	for (i = 0; i < count_s; i++)
	{
		s[i]->number = i;
		s[i]->primary = NULL;
		s[i]->secondary = NULL;
#ifdef SKIP_GUI
		if (s[i]->check_equation == TRUE)
		{
			species_rxn_to_trxn(s[i]);
			if (check_eqn(TRUE) == ERROR)
			{
				input_error++;
				error_string = sformatf(
						"Equation for species %s does not balance.",
						s[i]->name);
				error_msg(error_string, CONTINUE);
			}
		}
#endif
	}
	for (i = 0; i < count_master; i++)
	{
		char * temp_name = string_duplicate(master[i]->elt->name);
		ptr = temp_name;
		if (ptr[0] != '[')
		{
			while ((c = *(++ptr)) != '\0')
			{
				if (isupper((int) c))
				{
					input_error++;
					error_string = sformatf(
							"Element or valence name in SOLUTION_MASTER_SPECIES should include only one element, %s.",
							master[i]->elt->name);
					error_msg(error_string, false);
					break;
				}
			}
		}
		free_check_null(temp_name);
		/* store sequence number in master structure */
		master[i]->number = i;
		if (strcmp(master[i]->elt->name, "Alkalinity") != 0)
		{
			if (master[i]->primary == TRUE)
			{
				master[i]->s->primary = master[i];
			}
			else
			{
				master[i]->s->secondary = master[i];
			}
		}
		if (strcmp(master[i]->elt->name, "C") == 0)
		{
			s_co3 = master[i]->s;
		}
		if (master[i]->gfw_formula != NULL)
		{
#ifdef SKIP_GUI
			if (compute_gfw(master[i]->gfw_formula, &master[i]->gfw) == ERROR)
			{
				input_error++;
				error_string = sformatf(
						"Calculating gfw for master species, %s, formula %s.",
						master[i]->elt->name, master[i]->gfw_formula);
				error_msg(error_string, CONTINUE);
			}
#endif  // SKIP_GUI
		}
	}
/*
 *   Write equations for all master species in terms of primary
 *   master species, set coefficient of element in master species
 */
#ifdef SKIP_GUI
	for (i = 0; i < count_master; i++)
	{
		count_trxn = 0;
		if (master[i]->s->primary != NULL)
		{
			trxn_add(master[i]->s->rxn, 1.0, FALSE);
			trxn_add(master[i]->s->rxn, -1.0, TRUE);
		}
		else
		{
			trxn_add(master[i]->s->rxn, 1.0, FALSE);
			rewrite_eqn_to_primary();
		}
		rxn_free(master[i]->rxn_primary);
		master[i]->rxn_primary = rxn_alloc(count_trxn + 1);
		trxn_copy(master[i]->rxn_primary);
		master[i]->coef = coef_in_master(master[i]);
	}
/*
 *   Rewrite all species to secondary species
 */
	for (i = 0; i < count_s; i++)
	{
		count_trxn = 0;
		if (s[i]->primary != NULL || s[i]->secondary != NULL)
		{
			trxn_add(s[i]->rxn, 1.0, FALSE);
			trxn_add(s[i]->rxn, -1.0, TRUE);
		}
		else
		{
			trxn_add(s[i]->rxn, 1.0, FALSE);
			rewrite_eqn_to_secondary();
		}
		rxn_free(s[i]->rxn_s);
		s[i]->rxn_s = rxn_alloc(count_trxn + 1);
		trxn_copy(s[i]->rxn_s);
		/* calculate alkalinity */
		s[i]->alk = calc_alk(s[i]->rxn_s);
		/* set co2 coefficient */
		s[i]->co2 = 0.0;
		for (j = 1; j < count_trxn; j++)
		{
			if (trxn.token[j].s == s_co3)
			{
				s[i]->co2 = trxn.token[j].coef;
				break;
			}
		}
	}
#endif  // SKIP_GUI
/*
 *   Set pointer in element to master species
 */
	for (i = 0; i < count_elements; i++)
	{
		elements[i]->master = master_bsearch(elements[i]->name);
		if (elements[i]->master == NULL)
		{
#ifdef SKIP_GUI
			input_error++;
#endif  // SKIP_GUI
			error_string = sformatf( "No master species for element %s.",
					elements[i]->name);
			error_msg(error_string, false);
		}
		elements[i]->primary = master_bsearch_primary(elements[i]->name);
		if (elements[i]->primary == NULL)
		{
#ifdef SKIP_GUI
			input_error++;
#endif  // SKIP_GUI
			error_string = sformatf( "No master species for element %s.",
					elements[i]->name);
			error_msg(error_string, false);
		}
	}
/*
 *   Make sure all primary master species for redox elements
 *   are also secondary master species
 */
#ifdef SKIP_GUI
	for (i = 0; i < count_master; i++)
	{
		if (master[i]->primary == FALSE)
		{
			master_ptr = master[i]->s->secondary->elt->primary;
			if (master_ptr == NULL)
			{
				input_error++;
				error_string = sformatf(
						"Every primary master species for a redox element\n"
						"\tmust also be a secondary master species.\n"
						"\tError in definitions related to %s .\n",
						master[i]->s->name);
				error_msg(error_string, CONTINUE);

			}
			else if (master_ptr->s->secondary == NULL)
			{
				input_error++;
				error_string = sformatf(
						"Every primary master species for a redox element\n"
						"\tmust also be a secondary master species.\n"
						"\t%s is the primary master species for element %s.\n"
						"\tAnother entry in SOLUTION_MASTER_SPECIES is needed.\n"
						"\tDefine species %s as a secondary master species for a valence state.\n"
						"\tFor example: \n" "\t%s(0)\t%s alk gfw",
						master_ptr->s->name, master_ptr->elt->name,
						master_ptr->s->name, master_ptr->elt->name,
						master_ptr->s->name);
				error_msg(error_string, CONTINUE);
			}
		}
	}
#endif // SKIP_GUI
/*
 *   Calculate H and O if alternate mass balance is given
 */
#ifdef SKIP_GUI
	for (i = 0; i < count_s; i++)
	{
		if (s[i]->next_secondary != NULL)
		{
			s[i]->h = 0.0;
			s[i]->o = 0.0;
			for (j = 0; s[i]->next_secondary[j].elt != NULL; j++)
			{
				if (s[i]->next_secondary[j].elt->primary == NULL)
					continue;
				if (s[i]->next_secondary[j].elt->primary->s == s_hplus || s[i]->next_secondary[j].elt->primary->s == s_h3oplus)
				{
					s[i]->h += s[i]->next_secondary[j].coef;
				}
				else if (s[i]->next_secondary[j].elt->primary->s == s_h2o)
				{
					s[i]->o += s[i]->next_secondary[j].coef;
				}
				else if (s[i]->mole_balance != NULL)
				{
					master_ptr = s[i]->next_secondary[j].elt->master;
					if (master_ptr != NULL)
					{
						if (master_ptr->primary == TRUE)
						{
							if (master_ptr->s->secondary != NULL)
							{
								master_ptr = master_ptr->s->secondary;
							}
						}
					}
					else
					{
						input_error++;
						error_string = sformatf(
							"Element in -mole_balance %s not defined for species %s.\n", s[i]->mole_balance, s[i]->name);
						error_msg(error_string, CONTINUE);
						continue;
					}
					if (master_ptr->coef != 1)
					{
						s[i]->next_secondary[j].coef /= master_ptr->coef;
					}
				}
			}
			if (s[i]->type == EX)
			{
				for (j = 0; s[i]->next_secondary[j].elt != NULL; j++)
				{
					if (s[i]->next_secondary[j].elt->primary->s->type == EX)
					{
						s[i]->equiv = s[i]->next_secondary[j].coef;
						break;
					}
				}
			}
		}
	}
#endif // SKIP_GUI
	return (OK);
}
