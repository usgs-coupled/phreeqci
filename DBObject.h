// DBObject.h: interface for the CDBObject class.
//
// $Id$
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBOBJECT_H__1FD7C0EF_F3AF_4574_81C8_21B43ACA3ED4__INCLUDED_)
#define AFX_DBOBJECT_H__1FD7C0EF_F3AF_4574_81C8_21B43ACA3ED4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

extern "C"
{
#define EXTERNAL extern
#include "phreeqc/src/global.h"
}

class CDBObject// : public CObject  
{
public:
	CDBObject();
	CDBObject(const CString& rStr);
	CDBObject(LPCTSTR psz);
	CDBObject(const CDBObject& rDBObject);	// copy ctor
	virtual ~CDBObject();

	bool operator<(const CDBObject& rDBObject) const;

	CString m_strName;

// COMMENT: {5/23/2000 9:28:06 PM}	DECLARE_SERIAL(CDBObject)
// COMMENT: {5/23/2000 9:28:21 PM}	virtual void Serialize(CArchive& ar);
};


class CDBElement : public CDBObject  
{
public:
	CDBElement();
	CDBElement(const struct element* p_element);
	virtual ~CDBElement();
	bool operator<(const CDBElement& rDBElement)const;

	enum type {
		typeUnknown  = -1,
		typeAQ       =  0,
		typeHPLUS    =  1,
		typeH2O      =  2,
		typeEMINUS   =  3,
		typeSOLID    =  4,
		typeEX       =  5,
		typeSURF     =  6,
		typeSURF_PSI =  7,
	} m_type;
	CString m_strMaster;
	double m_dGFW;

// COMMENT: {5/23/2000 9:28:36 PM}	DECLARE_SERIAL(CDBElement)
// COMMENT: {5/23/2000 9:28:36 PM}	virtual void Serialize(CArchive& ar);
};


class CDBSpecies : public CDBObject  
{
public:
	CDBSpecies();
	CDBSpecies(const struct species* p_species);
	virtual ~CDBSpecies();

	enum CDBElement::type m_type;

};

class CDBPhase : public CDBObject  
{
public:
	CDBPhase();
	CDBPhase(const struct phase* p_phase);
	virtual ~CDBPhase();

	CString m_strFormula;
};


class CDBRate : public CDBObject  
{
public:
	CDBRate();
	CDBRate(const struct rate* p_rate);
	virtual ~CDBRate();

};


class CDBRedox : public CDBObject  
{
public:
	CDBRedox();
	CDBRedox(const char* psz_redox);
	virtual ~CDBRedox();

	bool operator<(const CDBRedox& rDBRedox)const;
};

class CDBRange : public CDBObject  
{
public:
	CDBRange(const CString& rStr);
	CDBRange(LPCTSTR psz);

	bool operator<(const CDBRange& rDBRange)const;
};

class CDBSSComp : public CDBObject  
{
public:
	CDBSSComp() : CDBObject() {;}
	CDBSSComp(const CString& rStr) : CDBObject(rStr) {;}
	CDBSSComp(LPCTSTR psz) : CDBObject(psz) {;}
	virtual ~CDBSSComp() {;}
};

class CDBSS : public CDBObject  
{
public:
	CDBSS() : CDBObject() {;}
	CDBSS(const struct s_s* s_s_ptr);
	CDBSS(const CString& rStr) : CDBObject(rStr) {;}
	CDBSS(LPCTSTR psz) : CDBObject(psz) {;}
	virtual ~CDBSS() {;}
public:
	std::list<CDBSSComp> m_listSSComp;
};

class CDBNamedExp : public CDBObject  
{
public:
	CDBNamedExp();
	CDBNamedExp(const struct logk* logk_ptr);
	virtual ~CDBNamedExp();

};

class CDBCalcVal : public CDBObject  
{
public:
	CDBCalcVal();
	CDBCalcVal(const struct calculate_value* calculate_value_ptr);
	virtual ~CDBCalcVal();

};



#endif // !defined(AFX_DBOBJECT_H__1FD7C0EF_F3AF_4574_81C8_21B43ACA3ED4__INCLUDED_)
