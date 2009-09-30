// DBObject.cpp: implementation of the CDBObject class.
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "DBObject.h"

extern "C" {
int compute_gfw(const char *string, double *gfw);
}


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// COMMENT: {5/23/2000 9:29:00 PM}IMPLEMENT_SERIAL(CDBObject, CObject, 0)

CDBObject::CDBObject()
{
}

CDBObject::CDBObject(const CString& rStr)
: m_strName(rStr)
{
}

CDBObject::CDBObject(LPCTSTR psz)
: m_strName(psz)
{
}


CDBObject::CDBObject(const CDBObject& rDBObject)	// copy ctor
{
	m_strName = rDBObject.m_strName;
}

CDBObject::~CDBObject()
{

}


// COMMENT: {5/23/2000 9:29:13 PM}//
// COMMENT: {5/23/2000 9:29:13 PM}// throw( CMemoryException );
// COMMENT: {5/23/2000 9:29:13 PM}// throw( CArchiveException );
// COMMENT: {5/23/2000 9:29:13 PM}// throw( CFileException );
// COMMENT: {5/23/2000 9:29:13 PM}void CDBObject::Serialize(CArchive &ar)
// COMMENT: {5/23/2000 9:29:13 PM}{
// COMMENT: {5/23/2000 9:29:13 PM}	CObject::Serialize(ar);
// COMMENT: {5/23/2000 9:29:13 PM}	if (ar.IsStoring())
// COMMENT: {5/23/2000 9:29:13 PM}	{
// COMMENT: {5/23/2000 9:29:13 PM}		ar << m_strName;
// COMMENT: {5/23/2000 9:29:13 PM}	}
// COMMENT: {5/23/2000 9:29:13 PM}	else
// COMMENT: {5/23/2000 9:29:13 PM}	{
// COMMENT: {5/23/2000 9:29:13 PM}		ar >> m_strName;
// COMMENT: {5/23/2000 9:29:13 PM}	}
// COMMENT: {5/23/2000 9:29:13 PM}}

bool CDBObject::operator<(const CDBObject& rDBObject) const
{
	return (m_strName < rDBObject.m_strName);
}

//////////////////////////////////////////////////////////////////////
// CDBElement Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// COMMENT: {5/23/2000 9:29:21 PM}IMPLEMENT_SERIAL(CDBElement, CDBObject, 0)

CDBElement::CDBElement()
: m_type(typeUnknown), m_dGFW(0.0)
{
}

CDBElement::CDBElement(const struct element* p_element)
{
	_ASSERTE( p_element         != NULL );
	_ASSERTE( p_element->master != NULL );
	_ASSERTE( p_element->name   != NULL );

	m_strName   = p_element->name;
	m_type      = static_cast<enum CDBElement::type>(p_element->master->type);
	m_strMaster = p_element->master->s->name;
	m_dGFW      = p_element->gfw;

	if (m_dGFW == 0.0) {
		if (p_element->master->gfw > 0.0)
		{
			m_dGFW = p_element->master->gfw;
		}
		else
		{
			if (p_element->master->gfw_formula != NULL) {
				compute_gfw(p_element->master->gfw_formula, &m_dGFW);
			}
		}
	}
}

CDBElement::~CDBElement()
{

}

// COMMENT: {5/23/2000 9:29:30 PM}//
// COMMENT: {5/23/2000 9:29:30 PM}// throw( CMemoryException );
// COMMENT: {5/23/2000 9:29:30 PM}// throw( CArchiveException );
// COMMENT: {5/23/2000 9:29:30 PM}// throw( CFileException );
// COMMENT: {5/23/2000 9:29:30 PM}void CDBElement::Serialize(CArchive &ar)
// COMMENT: {5/23/2000 9:29:30 PM}{
// COMMENT: {5/23/2000 9:29:30 PM}	CDBObject::Serialize(ar);
// COMMENT: {5/23/2000 9:29:30 PM}
// COMMENT: {5/23/2000 9:29:30 PM}	int type;
// COMMENT: {5/23/2000 9:29:30 PM}	if (ar.IsStoring())
// COMMENT: {5/23/2000 9:29:30 PM}	{
// COMMENT: {5/23/2000 9:29:30 PM}		type = m_type;
// COMMENT: {5/23/2000 9:29:30 PM}		ar << type;
// COMMENT: {5/23/2000 9:29:30 PM}
// COMMENT: {5/23/2000 9:29:30 PM}		ar << m_strMaster;
// COMMENT: {5/23/2000 9:29:30 PM}		ar << m_dGFW;
// COMMENT: {5/23/2000 9:29:30 PM}	}
// COMMENT: {5/23/2000 9:29:30 PM}	else
// COMMENT: {5/23/2000 9:29:30 PM}	{
// COMMENT: {5/23/2000 9:29:30 PM}		ar >> type;
// COMMENT: {5/23/2000 9:29:30 PM}		m_type = static_cast<enum CDBElement::type>(type);
// COMMENT: {5/23/2000 9:29:30 PM}
// COMMENT: {5/23/2000 9:29:30 PM}		ar >> m_strMaster;
// COMMENT: {5/23/2000 9:29:30 PM}		ar >> m_dGFW;
// COMMENT: {5/23/2000 9:29:30 PM}	}
// COMMENT: {5/23/2000 9:29:30 PM}}

bool CDBElement::operator<(const CDBElement& rDBElement)const
{
	// "e" is always last in list
	// Note: The first test must return false
	if (m_strName == _T("e"))
		return false;
	if (rDBElement.m_strName == _T("e"))
		return true;
	return (m_strName < rDBElement.m_strName);
}

//////////////////////////////////////////////////////////////////////
// CDBSpecies Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDBSpecies::CDBSpecies()
: m_type(CDBElement::typeUnknown)
{
}

CDBSpecies::CDBSpecies(const struct species* p_species)
{
	ASSERT(p_species != NULL);
	m_strName = p_species->name;
	m_type = static_cast<enum CDBElement::type>(p_species->type);
	m_z = p_species->z;
}

CDBSpecies::~CDBSpecies()
{
}

//////////////////////////////////////////////////////////////////////
// CDBPhase Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDBPhase::CDBPhase()
{
}

CDBPhase::CDBPhase(const struct phase* p_phase)
{
	ASSERT(p_phase != NULL);
	ASSERT(p_phase->type == SOLID);
	m_strName    = p_phase->name;
	m_strFormula = p_phase->formula;
}

CDBPhase::~CDBPhase()
{

}

//////////////////////////////////////////////////////////////////////
// CDBRate Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDBRate::CDBRate()
{
}

CDBRate::CDBRate(const struct rate* p_rate)
{
	ASSERT(p_rate != NULL);
	m_strName = p_rate->name;
}

CDBRate::~CDBRate()
{
}

//////////////////////////////////////////////////////////////////////
// CDBRedox Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDBRedox::CDBRedox()
{
}

CDBRedox::CDBRedox(const char* psz_redox)
{
	ASSERT(psz_redox != NULL);
	m_strName = psz_redox;
}

CDBRedox::~CDBRedox()
{
}

bool CDBRedox::operator<(const CDBRedox& rDBRedox)const
{
	// force pe to top
	// Note: The first test must return false
	if (rDBRedox.m_strName == _T("pe"))
		return false;
	if (m_strName == _T("pe"))
		return true;
	return (m_strName < rDBRedox.m_strName);
}

//////////////////////////////////////////////////////////////////////
// CDBRange Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDBRange::CDBRange(LPCTSTR psz)
: CDBObject(psz)
{
}

CDBRange::CDBRange(const CString& rStr)
: CDBObject(rStr)
{
}

bool CDBRange::operator<(const CDBRange& rDBRange)const
{
	// force none to top
	// Note: The first test must return false
	if (rDBRange.m_strName == _T("none"))
		return false;
	if (m_strName == _T("none"))
		return true;
	return (m_strName < rDBRange.m_strName);
}


//////////////////////////////////////////////////////////////////////
// CDBSS Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDBSS::CDBSS(const struct s_s* s_s_ptr)
{
	m_strName = s_s_ptr->name;

	for (int i = 0; i < s_s_ptr->count_comps; ++i)
	{
		CDBSSComp comp(s_s_ptr->comps[i].name);
		m_listSSComp.push_back(comp);
	}
}

//////////////////////////////////////////////////////////////////////
// CDBNamedExp Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDBNamedExp::CDBNamedExp(const struct logk* logk_ptr)
{
	m_strName = logk_ptr->name;
}

CDBNamedExp::~CDBNamedExp()
{
}

//////////////////////////////////////////////////////////////////////
// CDBCalcVal Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDBCalcVal::CDBCalcVal(const struct calculate_value* calculate_value_ptr)
{
	m_strName = calculate_value_ptr->name;
}

CDBCalcVal::~CDBCalcVal()
{
}
