// KeywordPageListItems.h: interface for the CPurePhase class.
//
// $Date: 2001/08/30 18:49:12 $
// $Revision: 1.1.1.32 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KEYWORDPAGELISTITEMS_H__13CB5247_EF41_48EA_86AA_A788A4219F30__INCLUDED_)
#define AFX_KEYWORDPAGELISTITEMS_H__13CB5247_EF41_48EA_86AA_A788A4219F30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

extern "C"
{
#define EXTERNAL extern
#include "phreeqc/src/global.h"
}

class CPurePhase  
{
public:
	CPurePhase();
	CPurePhase(const struct pure_phase *pure_phase_ptr);
	virtual ~CPurePhase();
public:
	CString m_strName;
	double m_dSI;
	double m_dAmount;
	CString m_strAlt;
	bool m_bDissolveOnly;
};


class CGasComp  
{
public:
	CGasComp();
	CGasComp(const struct gas_comp* gas_comp_ptr);
	virtual ~CGasComp();
public:
	double m_dP_Read;
	CString m_strName;
};

class CExchComp
{
public:
	CExchComp();
	CExchComp(const struct exch_comp* exch_comp_ptr);
	virtual ~CExchComp();
public:
	double m_dPhase_proportion;
	long double m_ldMoles;
	CString m_strFormula;
	CString m_strPhase_name;
	CString m_strRate_name;
};

class CNameCoef
{
public:
	CNameCoef();
	CNameCoef(const struct name_coef* name_coef_ptr);
	// use implicit copy ctor
	~CNameCoef();
public:
	CString m_strName;
	double m_dCoef;
};

class CConc
{
public:
	CString m_strDesc;
	double m_dInputConc;
	CString m_strUnits;
	CString m_strAs;
	double m_dGFW;
	CString m_strGFW;
	CString m_strRedox;
	CString m_strPhase;
	double m_dPhaseSI;
	CString m_strPhaseSI;
public:
	static CString GetUnits(CString& rStr);
	static CString GetAs(CString& rStr);
	static CString GetGFW(CString& rStr);
	static CString GetRedox(CString& rStr);
	static void GetPhase(CString &rStr, CConc& rConc);

	static CConc Create(LPCTSTR psz);

	CConc();
	CConc(const struct solution* solution_ptr, const struct conc* conc_ptr);
	virtual ~CConc();
	CString GetSubHeading()const;
};

class CIsotope
{
public:
	double m_dIsotopeNumber;
	CString m_strEltName;
	CString m_strName;
	double m_dRatio;
	double m_dRatioUncertainty;
public:
	CIsotope();
	CIsotope(const struct isotope* isotope_ptr);
	CIsotope(const struct iso* iso_ptr);
};

struct InvSol
{
	int nSol;
	double dUncertainty;
	bool bForce;
};

class CInvIsotope : public CIsotope
{
public:
	std::map<int, double> m_mapSol2Unc;
public:
	CInvIsotope();
	CString GetString(std::list<InvSol>& rlistInvSol, InvSol& rFinalInvSol, std::map<CString, double>& rDefaults);
	CInvIsotope(const struct isotope* isotope_ptr);
	CInvIsotope(const struct inv_isotope* inv_isotope_ptr);
};

class CS_S_Comp
{
public:
	long double m_ldMoles;
	CString m_strName;
public:	
	CS_S_Comp();
	CS_S_Comp(const struct s_s_comp* s_s_comp_ptr);
	virtual ~CS_S_Comp();
};

class CS_S
{
public:
	enum InputCase
	{
		IC_GUGG_NONDIMENSIONAL       = 0,
		IC_ACTIVITY_COEFFICIENTS     = 1,
		IC_DISTRIBUTION_COEFFICIENTS = 2,
		IC_MISCIBILITY_GAP           = 3,
		IC_SPINODAL_GAP              = 4,
		IC_CRITICAL_POINT            = 5,
		IC_ALYOTROPIC_POINT          = 6,
		IC_GUGG_KJ                   = 7,
		IC_THOMPSON                  = 8,
		IC_MARGULES                  = 9,
	} m_nInputCase;

	std::list<CS_S_Comp> m_listComp;
	CString m_strName;
	long double m_arrldP[4];
	double m_dTk;

public:	
	CS_S();
	CS_S(const struct s_s* s_s_ptr);
	virtual ~CS_S();
};

class basic_command
{
public:
	long nLine;
	CString strCommand;
};

class CRate  
{
public:
	CRate();
	CRate(const struct rate *rate_ptr);
	CString GetString();
	virtual ~CRate();
public:
	CString m_strName;
	std::list<basic_command> m_listCommands;
};


class CInvPhase
{
public:
	CInvPhase();
	CInvPhase(const struct inv_phases *inv_phases_ptr);
	CString GetString();
	CString GetString(int nWidth);
	virtual ~CInvPhase();
public:
	CString m_strName;
	enum Constraint
	{
		IPC_EITHER,
		IPC_PRECIPITATE,
		IPC_DISSOLVE,
	} m_nConstraint;
	bool m_bForce;
	std::map<CString, CIsotope> m_mapIsotopes;
};

class CInvElem
{
public:
	CInvElem();
	CString GetString(std::list<InvSol>& rlistInvSol, InvSol& rFinalInvSol);
	virtual ~CInvElem();
public:
	CString m_strName;
	std::map<int, double> m_mapSol2Unc;
};

class CSurfComp
{
public:
	CSurfComp();
	CSurfComp(const struct surface* surface_ptr, const struct surface_comp* surface_comp_ptr);
	virtual ~CSurfComp();

	// use implicit copy ctor

public:
	double m_dGrams;
	double m_dSpecific_area;
	double m_dPhase_proportion;
	double m_dMoles;
	CString m_strFormula;
	CString m_strPhase_name;
	CString m_strRate_name;
};

class CKineticComp  
{
public:
	CKineticComp();
	CKineticComp(const struct kinetics_comp *kinetics_comp_ptr);
	virtual ~CKineticComp();

public:
	CString m_strRateName;
	std::list<CNameCoef> m_listNameCoef;
	double m_dTol;
	double m_dM;
	double m_dM0;
	std::list<double> m_listDParams;
private:
	std::list<CString> m_listCParams;
	double m_dInitialMoles;
	double m_dMoles;
};

class CSpecies
{
public:
	CSpecies(const struct species *species_ptr);
	CSpecies();
	virtual ~CSpecies();
public:
	CString m_strEqn;
	CString m_strMoleBalance;
	double m_dLogK;
	double m_dDeltaH;
	bool m_bHasAnalExp;
	bool m_bCheckEqn;
	double m_dA1;
	double m_dA2;
	double m_dA3;
	double m_dA4;
	double m_dA5;
	//{{
	enum ActType
	{
		AT_UNKNOWN      = 0,
		AT_DAVIES       = 1,
		AT_DEBYE_HUCKEL = 2,
		AT_LLNL_DH      = 3,
		AT_LLNL_DH_CO2  = 4,
	} m_nActType;
	//}}
	double m_dDHa;
	double m_dDHb;
	DELTA_H_UNIT m_nDeltaHUnits;
protected:
	static CString WriteEqn(const struct species *species_ptr);
};

class CPhase
{
public:
	CPhase(const struct phase *phase_ptr);
	CPhase();
	virtual ~CPhase();
public:
	CString m_strName;
	CString m_strEqn;
	CString m_strMoleBalance;
	double m_dLogK;
	double m_dDeltaH;
	bool m_bHasAnalExp;
	bool m_bCheckEqn;
	double m_dA1;
	double m_dA2;
	double m_dA3;
	double m_dA4;
	double m_dA5;
	DELTA_H_UNIT m_nDeltaHUnits;
protected:
	static CString WriteEqn(const struct phase *phase_ptr);
};

class CMaster
{
public:
	CMaster(const struct master *master_ptr);
	CMaster();
	virtual ~CMaster();
public:
	CString m_strName;
	bool m_bPrimary;
	CString m_strMasterSpecies;
	double m_dAlk;
	double m_dGFW;
	CString m_strFormula;
};

#endif // !defined(AFX_KEYWORDPAGELISTITEMS_H__13CB5247_EF41_48EA_86AA_A788A4219F30__INCLUDED_)
