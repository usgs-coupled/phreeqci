#pragma once
#include "Phreeqc.h"
#include "PHRQ_io.h"

class COCKSTransport;
class CTransport;
class COCKSAdvection;
class CCKSMix;
class CCKSExchange;
class CCKSEquilibrium_Phases;
class CCKSGas_Phase;
class CCKSReaction;
class CCKSReaction_Temperature;
class CCKSSolution;
class CCKSSolid_Solutions;
class COCKSRates;
class CCKSInverse;
class COCKSUserPrint;
class COCKSUserPunch;
class CSurfaceSheet;
class CKSPrint;
class CCKSKinetics;
class CKSIncrement;
class CKSSelectedOutput;
class COCKSSolution_Spread;
class CKSSolutionSpecies;
class CKSSurfaceSpecies;
class CKSExchangeSpecies;
class CKSPhases;
class CKSSolutionMasterSpecies;
class CKSExchangeMasterSpecies;
class CKSSurfaceMasterSpecies;
class CKSKnobs;
class CKSPitzer;
class CKSSIT;
class CRange;

class PhreeqcI : public Phreeqc, public PHRQ_io
{
public:
	PhreeqcI(void);
	PhreeqcI(PHRQ_io* io);
	PhreeqcI(int argc, char *argv[], PHRQ_io* io);
	PhreeqcI(CString& rString, bool bMoreThanOneKeyword=false);
	/*virtual*/ ~PhreeqcI(void);

	// opening files
	virtual bool output_open(const char *file_name, std::ios_base::openmode mode = std::ios_base::out);
	virtual bool log_open(const char *file_name, std::ios_base::openmode mode = std::ios_base::out);
	virtual bool punch_open(const char *file_name, std::ios_base::openmode mode = std::ios_base::out);
	virtual bool error_open(const char *file_name, const char * mode = "w");
	virtual bool dump_open(const char *file_name, std::ios_base::openmode mode = std::ios_base::out);

public:
	DWORD GetReturnValue(void) { return dwReturn; };
public:
	void GetData(COCKSTransport* sheet)const;
	void GetData(COCKSAdvection* sheet)const;
	void GetData(CCKSMix* sheet)const;
	void GetData(CCKSExchange* sheet)const;
	void GetData(CCKSEquilibrium_Phases* sheet)const;
	void GetData(CCKSGas_Phase* sheet)const;
	void GetData(CCKSReaction* sheet)const;
	void GetData(CCKSReaction_Temperature* sheet)const;
	void GetData(CCKSSolution* sheet)const;
	void GetData(CCKSSolid_Solutions* sheet)const;
	void GetData(COCKSRates* sheet)const;
	void GetData(CCKSInverse* sheet)const;
	void GetData(COCKSUserPrint* sheet)const;
	void GetData(COCKSUserPunch* sheet)const;
	void GetData(CSurfaceSheet* sheet)const;
	void GetData(CKSPrint* sheet)const;
	void GetData(CCKSKinetics* sheet)const;
	void GetData(CKSIncrement* sheet)const;
	void GetData(CKSSelectedOutput* sheet)const;
	void GetData(COCKSSolution_Spread* sheet)const;
	void GetData(CKSSolutionSpecies* sheet)const;
	void GetData(CKSSurfaceSpecies* sheet)const;
	void GetData(CKSExchangeSpecies* sheet)const;
	void GetData(CKSPhases* sheet)const;
	void GetData(CKSSolutionMasterSpecies* sheet)const;
	void GetData(CKSExchangeMasterSpecies* sheet)const;
	void GetData(CKSSurfaceMasterSpecies* sheet)const;
	void GetData(CKSKnobs* sheet)const;
	void GetData(CKSPitzer* sheet)const;
	void GetData(CKSSIT* sheet)const;

	void Update(CTransport* transport)const;
	void UpdatePrintRange(std::list<CRange> &list)const;
	void UpdatePunchRange(std::list<CRange> &list)const;

public:
	int Get_max_line(void)const  {return this->max_line;};
	void Set_max_line(int ml)    {this->max_line = ml;};

protected:
	void local_init(void);

protected:
	std::string        m_s;
	std::istringstream m_iss;
	DWORD              dwReturn;
};
