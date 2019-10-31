#include "stdafx.h"
#include "resource.h"
#include "PhreeqcI.h"
#include "SeException.h"

#include "OCKSTransport.h"
#include "OCKSAdvection.h"
#include "CKSMix.h"
#include "CKSExchange.h"
#include "CKSEquilibrium_Phases.h"
#include "CKSGas_Phase.h"
#include "CKSReaction.h"
#include "CKSReaction_Temperature.h"
#include "CKSSolution.h"
#include "CKSSolid_Solutions.h"
#include "OCKSRates.h"
#include "CKSInverse.h"
#include "OCKSUserPrint.h"
#include "OCKSUserPunch.h"
#include "SurfaceSheet.h"
#include "KSPrint.h"
#include "KSKinetics.h"
#include "KSIncrement.h"
#include "KSSelectedOutput.h"
#include "OCKSSolution_Spread.h"
#include "KSSolutionSpecies.h"
#include "KSSurfaceSpecies.h"
#include "KSExchangeSpecies.h"
#include "KSPhases.h"
#include "KSSolutionMasterSpecies.h"
#include "KSExchangeMasterSpecies.h"
#include "KSSurfaceMasterSpecies.h"
#include "KSKnobs.h"
#include "KSPitzer.h"
#include "KSSIT.h"
#include "DefinedRanges.h"
#include "UserGraph.h"
#include "Reaction_Pressure.h"
#include "Delete.h"
#include "RunCells.h"
#include "CalculateValues.h"
#include "Dump.h"

#include "phreeqc3/src/Exchange.h"
#include "phreeqc3/src/PPassemblage.h"
#include "phreeqc3/src/GasPhase.h"
#include "phreeqc3/src/Reaction.h"
#include "phreeqc3/src/Temperature.h"
#include "phreeqc3/src/Solution.h"
#include "phreeqc3/src/ISolution.h"
#include "phreeqc3/src/SSassemblage.h"
#include "phreeqc3/src/cxxKinetics.h"

static _se_translator_function prev_se_translator_function = 0;

PhreeqcI::PhreeqcI(void)
: start(clock())
{
	ASSERT(prev_se_translator_function == 0);
	prev_se_translator_function = _set_se_translator(CSeException::SeTranslator);

	this->phrq_io = this;
	this->do_initialize();
}

PhreeqcI::PhreeqcI(PHRQ_io* io)
: Phreeqc(io)
, start(clock())
{
	ASSERT(prev_se_translator_function == 0);
	prev_se_translator_function = _set_se_translator(CSeException::SeTranslator);
}

PhreeqcI::PhreeqcI(int argc, char *argv[], PHRQ_io* io)
: Phreeqc(io)
, start(clock())
{
	this->dwReturn = 0;

	ASSERT(prev_se_translator_function == 0);
	prev_se_translator_function = _set_se_translator(CSeException::SeTranslator);
	
	try
	{
		int errors;
		std::istream *db_cookie = NULL;
		std::istream *input_cookie = NULL;

		//
		// Open input/output files
		//
		errors = process_file_names(argc, argv, &db_cookie, &input_cookie, TRUE);
		assert(errors == 0);
		if (errors != 0)
		{
			return;
		}

		//
		// Initialize arrays
		//
		errors = do_initialize();
		assert(errors == 0);
		assert(pr.headings == TRUE);
		if (errors != 0)
		{
			return;
		}

		//
		// Load database into memory
		//
		this->phrq_io->push_istream(db_cookie);
		errors = this->read_database();
		this->phrq_io->clear_istream();
		if (errors != 0)
		{
			return;
		}

		//
		// Read input data for simulation
		//
		this->phrq_io->push_istream(input_cookie);
		errors = this->run_simulations();
		assert(errors == 0);
	}
	catch (DWORD dw)
	{
		ASSERT(dw != 0);
		this->dwReturn = dw;		
	}
	catch (CSeException *e)
	{
		TCHAR trcMsg[1024];
		e->GetErrorMessage(trcMsg, 1024);
		TRACE(trcMsg);
		TRACE(_T("\n"));
		this->dwReturn = e->GetSeCode();
		e->Delete();
	}
#if defined(__cplusplus_cli)
	catch (System::Exception ^exc)
	{
		this->dwReturn = (DWORD)-1;
		const char* chars = (const char*)(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(exc->ToString())).ToPointer();
		TRACE("<<<<<<<\n");
		TRACE(chars);
		TRACE("\n>>>>>>>\n");

		((Phreeqc*)this)->error_msg("Unhandled exception");
		CString str(chars);
		str.Remove('\r');
		((Phreeqc*)this)->error_msg(str);

		System::Runtime::InteropServices::Marshal::FreeHGlobal(System::IntPtr((void*)chars));
	}
#endif
	catch (...)
	{
		ASSERT(FALSE);
	}

	//
	// Close input files
	//
	this->phrq_io->clear_istream();

	//
	// Display status
	//
	int errs = this->do_status();
	assert(errs == 0);

	//
	// Close output files
	//
	this->phrq_io->close_ostreams();
}

PhreeqcI::PhreeqcI(CString& rString, bool bMoreThanOneKeyword)
: m_s(rString)
, m_iss(m_s)
, start(clock())
{
	ASSERT(prev_se_translator_function == 0);
	prev_se_translator_function = _set_se_translator(CSeException::SeTranslator);

	this->phrq_io = this;
	try
	{
		this->do_initialize();
		this->local_init();
		this->push_istream(&this->m_iss, false);
		if (bMoreThanOneKeyword)
		{
			for (this->simulation = 1; true; ++this->simulation)
			{
				if (this->read_input() == EOF) break;
			}
		}
		else
		{
			this->read_input();
		}
		this->input_error = 0;
	}
	catch (DWORD dw)
	{
		ASSERT(FALSE);
		switch (dw)
		{
		case 0:
			break;
		default:
			break;
		}
	}
	catch (CSeException *e)
	{
		TCHAR trcMsg[1024];
		e->GetErrorMessage(trcMsg, 1024);
		TRACE(trcMsg);
		TRACE(_T("\n"));
		e->ReportError(MB_OK | MB_ICONSTOP);
		e->Delete();
	}
	catch (...)
	{
		ASSERT(FALSE);
	}
}


PhreeqcI::~PhreeqcI(void)
{
	this->close_ostreams();
	if (prev_se_translator_function)
	{
		_set_se_translator(prev_se_translator_function);
		prev_se_translator_function = 0;
	}
}

void PhreeqcI::local_init(void)
{
	/* knobs */
	this->pr.logfile = -1;

	this->debug_diffuse_layer = -1;
	this->debug_inverse       = -1;
	this->debug_model         = -1;
	this->debug_prep          = -1;
	this->debug_set           = -1;
	this->delay_mass_water    = -1;
	this->diagonal_scale      = -1;

	/* print */
	this->pr.alkalinity         = -1;
	this->pr.gas_phase          = -1;
	this->pr.ss_assemblage      = -1;
	this->pr.pp_assemblage      = -1;
	this->pr.surface            = -1;
	this->pr.exchange           = -1;
	this->pr.kinetics           = -1;
	this->pr.totals             = -1;
	this->pr.eh                 = -1;
	this->pr.species            = -1;
	this->pr.saturation_indices = -1;
	this->pr.irrev              = -1;
	this->pr.mix                = -1;
	this->pr.use                = -1;
	this->pr.punch              = -1;
	this->pr.status             = -1;
	this->pr.inverse            = -1;
	this->pr.dump               = -1;
	this->pr.user_print         = -1;
	this->pr.headings           = -1;
	this->pr.echo_input         = -1;
}

bool PhreeqcI::output_open(const char *file_name, std::ios_base::openmode mode)
{
	UNUSED_ALWAYS(file_name);
	UNUSED_ALWAYS(mode);
	TRACE("PhreeqcI::output_open\n");
	return true;
}

bool PhreeqcI::log_open(const char *file_name, std::ios_base::openmode mode)
{
	UNUSED_ALWAYS(file_name);
	UNUSED_ALWAYS(mode);
	TRACE("PhreeqcI::log_open\n");
	return true;
}

bool PhreeqcI::punch_open(const char *file_name, std::ios_base::openmode mode, int n_user)
{
	UNUSED_ALWAYS(file_name);
	UNUSED_ALWAYS(mode);
	UNUSED_ALWAYS(n_user);
	TRACE("PhreeqcI::punch_open\n");
	return true;
}

bool PhreeqcI::error_open(const char *file_name, const char *mode)
{
	UNUSED_ALWAYS(file_name);
	UNUSED_ALWAYS(mode);
	TRACE("PhreeqcI::error_open\n");
	return true;
}

bool PhreeqcI::dump_open(const char *file_name, std::ios_base::openmode mode)
{
	UNUSED_ALWAYS(file_name);
	UNUSED_ALWAYS(mode);
	TRACE("PhreeqcI::dump_open\n");
	return true;
}

void PhreeqcI::GetData(COCKSTransport* t)const
{
	// Line 1:     -cells                 5
	t->m_Page1.m_nCells         = this->count_cells;

	// Line 2:     -shifts                25
	t->m_Page1.m_nShifts        = this->count_shifts;

	// Line 3:      -time_step 3.15e7 # seconds = 1 yr.
	t->m_Page1.m_dTimeStep      = this->timest;
	t->m_Page6.m_mcd_substeps   = this->mcd_substeps;


	// Line 4:     -flow_direction        forward
	switch (this->ishift)
	{
	case -1:
		t->m_Page3.m_fdFD = CKPTransportPg3::FD_BACK;
		break;
	case 0:
		t->m_Page3.m_fdFD = CKPTransportPg3::FD_DIFF_ONLY;
		break;
	case 1:
		t->m_Page3.m_fdFD = CKPTransportPg3::FD_FORWARD;
		break;
	default:
		ASSERT(FALSE);
		t->m_Page3.m_fdFD = CKPTransportPg3::FD_FORWARD;
		break;
	}

	// Line 5:     -boundary_conditions   flux constant
	switch (this->bcon_first)
	{
	case 1:
		t->m_Page3.m_bcFirst = CKPTransportPg3::BC_CONSTANT;
		break;
	case 2:
		t->m_Page3.m_bcFirst = CKPTransportPg3::BC_CLOSED;
		break;
	case 3:
		t->m_Page3.m_bcFirst = CKPTransportPg3::BC_FLUX;
		break;
	default:
		ASSERT(FALSE);
		t->m_Page3.m_bcFirst = CKPTransportPg3::BC_FLUX;
		break;
	}

	switch (this->bcon_last)
	{
	case 1:
		t->m_Page3.m_bcLast = CKPTransportPg3::BC_CONSTANT;
		break;
	case 2:
		t->m_Page3.m_bcLast = CKPTransportPg3::BC_CLOSED;
		break;
	case 3:
		t->m_Page3.m_bcLast = CKPTransportPg3::BC_FLUX;
		break;
	default:
		ASSERT(FALSE);
		t->m_Page3.m_bcLast = CKPTransportPg3::BC_FLUX;
		break;
	}

	// Line 6:     -lengths               4*1.0 2.0
	// Line 7:     -dispersivities        4*0.1 0.2
	CRepeat rLength(this->cell_data[1].length);
	CRepeat rDisp(this->cell_data[1].disp);
	for (int i = 2; i <= this->count_cells; ++i)
	{
		// lengths
		if (this->cell_data[i].length == rLength.GetDValue())
		{
			rLength.Increment();
		}
		else
		{
			t->m_Page4.m_lrLengths.push_back(rLength);
			rLength = CRepeat(this->cell_data[i].length);
		}

		// disps
		if (this->cell_data[i].disp == rDisp.GetDValue())
		{
			rDisp.Increment();
		}
		else
		{
			t->m_Page4.m_lrDisps.push_back(rDisp);
			rDisp = CRepeat(this->cell_data[i].disp);
		}
	}
	if (!(rLength.GetCount() == (size_t)this->count_cells && rLength.GetDValue() == 1.0))
	{
		t->m_Page4.m_lrLengths.push_back(rLength);
	}
	if (!(rDisp.GetCount() == (size_t)this->count_cells && rDisp.GetDValue() == 0.0))
	{
		t->m_Page4.m_lrDisps.push_back(rDisp);
	}

	// Line 8:     -correct_disp          true
	t->m_Page3.m_bCorrectDisp = this->correct_disp;

	// Line 9:     -diffusion_coefficient 1.0e-9
	t->m_Page3.m_dDiffCoef   = this->diffc;

	// Line 10:    -stagnant              1  6.8e-6   0.3   0.1
	t->m_Page5.m_bUseStagnant = (this->stag_data->count_stag != 0);
	t->m_Page5.m_nStagCells   = this->stag_data->count_stag;
	t->m_Page5.m_dExchFactor  = this->stag_data->exch_f;
	t->m_Page5.m_dThetaM      = this->stag_data->th_m;
	t->m_Page5.m_dThetaIM     = this->stag_data->th_im;

	// Line 11:    -thermal_diffusion     3.0   0.5e-6
	t->m_Page3.m_bUseThermal = TRUE;        // (this->tempr != 2.0 || this->heat_diffc != this->diffc);
	t->m_Page3.m_dTRF        = this->tempr;
	t->m_Page3.m_dTDC        = this->heat_diffc;

	// Line 12:    -initial_time          1000
	t->m_Page1.m_dInitTime      = this->initial_total_time;

	// determine max_cell (used for print_cells & punch_cells)
	int max_cell = this->count_cells;
	if (this->stag_data->count_stag)
	{
		max_cell = this->count_cells * (1 + this->stag_data->count_stag) + 2;
	}

	// Line 13:    -print_cells           1-3 5
	{
		CRange rPrint;
		rPrint.nMin = -1;
		for (int i = 1; i <= max_cell; ++i)
		{
			if (this->cell_data[i].print == TRUE)
			{
				if (rPrint.nMin == -1)
				{
					rPrint.nMin = i;
				}
				rPrint.nMax = i;
			}
			else
			{
				if (rPrint.nMin != -1)
				{
					t->m_Page2.m_listPrintRange.push_back(rPrint);
					rPrint.nMin = -1;
				}
			}
		}
		if (rPrint.nMin != -1)
		{
			// No ranges if all cells are selected
			if (rPrint.nMin == 1 && rPrint.nMax == this->count_cells)
			{
				ASSERT(t->m_Page2.m_listPrintRange.empty());
			}
			else
			{
				t->m_Page2.m_listPrintRange.push_back(rPrint);
			}
		}
	}

	// Line 14:    -print_frequency       5
	t->m_Page2.m_nPrintModulus = this->print_modulus;

	// Line 15:    -punch_cells           2-5
	{
		CRange rPunch;
		rPunch.nMin = -1;
		for (int i = 1; i <= max_cell; ++i)
		{
			if (this->cell_data[i].punch == TRUE)
			{
				if (rPunch.nMin == -1)
				{
					rPunch.nMin = i;
				}
				rPunch.nMax = i;
			}
			else
			{
				if (rPunch.nMin != -1)
				{
					t->m_Page2.m_listPunchRange.push_back(rPunch);
					rPunch.nMin = -1;
				}
			}
		}
		if (rPunch.nMin != -1)
		{
			if (rPunch.nMin == 1 && rPunch.nMax == this->count_cells)
			{
				ASSERT(t->m_Page2.m_listPunchRange.empty());
			}
			else
			{
				t->m_Page2.m_listPunchRange.push_back(rPunch);
			}

		}
	}

	// Line 16:    -punch_frequency       5
	t->m_Page2.m_nPunchModulus = this->punch_modulus;

	// Line 17:    -dump                  dump.file
	t->m_Page5.m_bDumpToFile     = (this->dump_in == TRUE);
	t->m_Page5.m_strDumpFileName = this->dump_file_name;

	// Line 18:    -dump_frequency        10
	t->m_Page5.m_nDumpModulus    = this->dump_modulus;

	// Line 19:    -dump_restart          20
	t->m_Page5.m_nDumpRestart    = this->transport_start;

	// Line 20:    -warnings              false
	t->m_Page1.m_bPrintWarnings = this->transport_warnings;

	// Multicomponent diffusion
	// Line 21: -multi_d true 1e-9 0.3 0.05 1.0
	t->m_Page6.m_bUseMCD = (this->multi_Dflag != 0);
	//if (t->m_Page6.m_bUseMCD)
	{
		t->m_Page6.m_default_Dw     = this->default_Dw;
		t->m_Page6.m_multi_Dpor     = this->multi_Dpor;
		t->m_Page6.m_multi_Dpor_lim = this->multi_Dpor_lim;
		t->m_Page6.m_multi_Dn       = this->multi_Dn;
	}

	// Interlayer diffusion
	// Line 22: -interlayer_D true 0.09 0.01 150
	t->m_Page6.m_bUseID = (this->interlayer_Dflag != 0);
	if (t->m_Page6.m_bUseID)
	{
		t->m_Page6.m_interlayer_Dpor     = this->interlayer_Dpor;
		t->m_Page6.m_interlayer_Dpor_lim = this->interlayer_Dpor_lim;
		t->m_Page6.m_interlayer_tortf    = this->interlayer_tortf;
	}
}

void PhreeqcI::Update(CTransport* transport)const
{
	// Line 1:     -cells                 5
	transport->count_cells  = this->count_cells;                  // m_Page1.m_nCells

	// Line 2:     -shifts                25
	transport->count_shifts = this->count_shifts;                // m_Page1.m_nShifts

	// Line 3:      -time_step 3.15e7 # seconds = 1 yr.
	transport->timest       = this->timest;                      // m_Page1.m_dTimeStep && m_Page1.m_tuTimeStep
	transport->mcd_substeps = this->mcd_substeps;                // m_Page6.m_mcd_substeps
	
	// Line 4:     -flow_direction        forward
	switch (this->ishift)                                   // m_Page3.m_fdFD
	{
	case -1:
		transport->shift = CKPTransportPg3::FD_BACK;
		break;
	case 0:
		transport->shift = CKPTransportPg3::FD_DIFF_ONLY;
		break;
	case 1:
		transport->shift = CKPTransportPg3::FD_FORWARD;
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	// Line 5:     -boundary_conditions   flux constant
	switch (this->bcon_first)                               // m_Page3.m_bcFirst
	{
	case 1:
		transport->bc_first = CKPTransportPg3::BC_CONSTANT;
		break;
	case 2:
		transport->bc_first = CKPTransportPg3::BC_CLOSED;
		break;
	case 3:
		transport->bc_first = CKPTransportPg3::BC_FLUX;
		break;
	default:
		ASSERT(FALSE);
		transport->bc_first = CKPTransportPg3::BC_FLUX;
		break;
	}

	switch (this->bcon_last)                                // m_Page3.m_bcLast
	{
	case 1:
		transport->bc_last = CKPTransportPg3::BC_CONSTANT;
		break;
	case 2:
		transport->bc_last = CKPTransportPg3::BC_CLOSED;
		break;
	case 3:
		transport->bc_last = CKPTransportPg3::BC_FLUX;
		break;
	default:
		ASSERT(FALSE);
		transport->bc_last = CKPTransportPg3::BC_FLUX;
		break;
	}


	// Line 6:     -lengths               4*1.0 2.0
	// Line 7:     -dispersivities        4*0.1 0.2
	transport->lengths_list.clear();                         // m_Page4.m_lrLengths
	transport->disp_list.clear();                            // m_Page4.m_lrDisps
	ASSERT(this->cell_data);
	CRepeat rLength(this->cell_data[0].length);
	CRepeat rDisp(this->cell_data[0].disp);
	for (int i = 1; i < this->count_cells; ++i)
	{
		// lengths
		if (this->cell_data[i].length == rLength.GetDValue())
		{
			rLength.Increment();
		}
		else
		{
			transport->lengths_list.push_back(rLength);
			rLength = CRepeat(this->cell_data[i].length);
		}

		// disps
		if (this->cell_data[i].disp == rDisp.GetDValue())
		{
			rDisp.Increment();
		}
		else
		{
			transport->disp_list.push_back(rDisp);
			rDisp = CRepeat(this->cell_data[i].disp);
		}
	}
	if (!(rLength.GetCount() == (size_t)this->count_cells && rLength.GetDValue() == 1.0))
	{
		transport->lengths_list.push_back(rLength);
	}
	if (!(rDisp.GetCount() == (size_t)this->count_cells && rDisp.GetDValue() == 0.0))
	{
		transport->disp_list.push_back(rDisp);
	}

	// Line 8:     -correct_disp          true
	transport->correct_disp       = this->correct_disp;          // m_Page3.m_bCorrectDisp

	// Line 9:     -diffusion_coefficient 1.0e-9
	transport->diffc              = this->diffc;                 // m_Page3.m_dDiffCoef

	// Line 10:    -stagnant              1  6.8e-6   0.3   0.1
	transport->count_stag = this->stag_data->count_stag;         // m_Page5.m_nStagCells
	transport->exch_f     = this->stag_data->exch_f;             // m_Page5.m_dExchFactor
	transport->th_m       = this->stag_data->th_m;               // m_Page5.m_dThetaM
	transport->th_im      = this->stag_data->th_im;              // m_Page5.m_dThetaIM

	// Line 11:    -thermal_diffusion     3.0   0.5e-6
	transport->tempr              = this->tempr;                 // m_Page3.m_dTRF
	transport->heat_diffc         = this->heat_diffc;            // m_Page3.m_dTDC

	// Line 12:    -initial_time          1000
	transport->initial_total_time = this->initial_total_time;    // m_Page1.m_dInitTime

	// Line 13:    -print_cells           1-3 5
	this->UpdatePrintRange(transport->print_range_list);     // m_Page2.m_listPrintRange

	// Line 14:    -print_frequency       5
	transport->print_modulus      = this->print_modulus;         // m_Page2.m_nPrintModulus

	// Line 15:    -punch_cells           2-5
	this->UpdatePunchRange(transport->punch_range_list);     // m_Page2.m_listPunchRange

	// Line 16:    -punch_frequency       5
	transport->punch_modulus      = this->punch_modulus;         // m_Page2.m_nPunchModulus

	// Line 17:    -dump                  dump.file
	transport->dump_in        = this->dump_in;                   // m_Page5.m_bDumpToFile
	transport->dump_file_name = this->dump_file_name;            // m_Page5.m_strDumpFileName

	// Line 18:    -dump_frequency        10
	transport->dump_modulus       = this->dump_modulus;          // m_Page5.m_nDumpModulus

	// Line 19:    -dump_restart          20
	transport->transport_start    = this->transport_start;       // m_Page5.m_nDumpRestart

	// Line 20:    -warnings              false
	transport->transport_warnings = this->transport_warnings;    // m_Page1.m_bPrintWarnings

	// Multicomponent diffusion
	// Line 21: -multi_d true 1e-9 0.3 0.05 1.0
	transport->multi_Dflag        = this->multi_Dflag;           // m_Page6.m_bUseMCD
	transport->default_Dw         = this->default_Dw;            // m_Page6.m_default_Dw
	transport->multi_Dpor         = this->multi_Dpor;            // m_Page6.m_multi_Dpor
	transport->multi_Dpor_lim     = this->multi_Dpor_lim;        // m_Page6.m_multi_Dpor_lim
	transport->multi_Dn           = this->multi_Dn;              // m_Page6.m_multi_Dn

	// Interlayer diffusion
	// Line 22: -interlayer_D true 0.09 0.01 150
	transport->interlayer_Dflag    = this->interlayer_Dflag;     // m_Page6.m_bUseID
	transport->interlayer_Dpor     = this->interlayer_Dpor;      // m_Page6.m_interlayer_Dpor
	transport->interlayer_Dpor_lim = this->interlayer_Dpor_lim;  // m_Page6.m_interlayer_Dpor_lim
	transport->interlayer_tortf    = this->interlayer_tortf;     // m_Page6.m_interlayer_tortf

	// count of TRANSPORT keywords
	transport->simul_tr = this->simul_tr;
}

void PhreeqcI::UpdatePrintRange(std::list<CRange> &list)const
{
	list.clear();

	CRange range;
	range.nMin = -1;
	int max_cell;
	if (this->stag_data->count_stag)
	{
		max_cell = (this->stag_data->count_stag  + 1) * this->count_cells + 1;
	}
	else
	{
		max_cell = this->count_cells;
	}
	for (int i = 0; i < max_cell; ++i)
	{
		if (this->cell_data[i].print == TRUE)
		{
			if (range.nMin == -1)
			{
				range.nMin = i + 1;
			}
			range.nMax = i + 1;
		}
		else
		{
			if (range.nMin != -1)
			{
				list.push_back(range);
				range.nMin = -1;
			}
		}
	}
	if (range.nMin != -1)
	{
		// No ranges if all cells are selected
		if (range.nMin == 1 && range.nMax == this->count_cells)
		{
			ASSERT(list.empty());
		}
		else
		{
			list.push_back(range);
		}

	}
}

void PhreeqcI::UpdatePunchRange(std::list<CRange> &list)const
{
	list.clear();

	CRange range;
	range.nMin = -1;
	int max_cell;
	if (this->stag_data->count_stag)
	{
		max_cell = (this->stag_data->count_stag  + 1) * this->count_cells + 1;
	}
	else
	{
		max_cell = this->count_cells;
	}
	for (int i = 0; i < max_cell; ++i)
	{
		if (this->cell_data[i].punch == TRUE)
		{
			if (range.nMin == -1)
			{
				range.nMin = i + 1;
			}
			range.nMax = i + 1;
		}
		else
		{
			if (range.nMin != -1)
			{
				list.push_back(range);
				range.nMin = -1;
			}
		}
	}
	if (range.nMin != -1)
	{
		// No ranges if all cells are selected
		if (range.nMin == 1 && range.nMax == this->count_cells)
		{
			ASSERT(list.empty());
		}
		else
		{
			list.push_back(range);
		}
	}
}

void PhreeqcI::GetData(COCKSAdvection* sheet)const
{
	sheet->m_Page1.m_nCells         = count_ad_cells;
	sheet->m_Page1.m_nShifts        = count_ad_shifts;
	sheet->m_Page1.m_dTimeStep      = advection_kin_time;
	sheet->m_Page1.m_dInitTime      = initial_total_time;
	sheet->m_Page1.m_bPrintWarnings = advection_warnings;

	CRange range;
	range.nMin = -1;
	for (int i = 0; i < count_ad_cells; ++i)
	{
		if (advection_punch[i] == TRUE)
		{
			if (range.nMin == -1)
			{
				range.nMin = i + 1;
			}
			range.nMax = i + 1;
		}
		else
		{
			if (range.nMin != -1)
			{
				sheet->m_Page2.m_listPunchRange.push_back(range);
				range.nMin = -1;
			}
		}
	}
	if (range.nMin != -1)
	{
		// No ranges if all cells are selected
		if (range.nMin != 1 && range.nMax != count_ad_cells)
		{
			sheet->m_Page2.m_listPunchRange.push_back(range);
		}
	}

	sheet->m_Page2.m_nPrintModulus = print_ad_modulus;

	range.nMin = -1;
	for (int i = 0; i < count_ad_cells; ++i)
	{
		if (advection_print[i] == TRUE)
		{
			if (range.nMin == -1)
			{
				range.nMin = i + 1;
			}
			range.nMax = i + 1;
		}
		else
		{
			if (range.nMin != -1)
			{
				sheet->m_Page2.m_listPrintRange.push_back(range);
				range.nMin = -1;
			}
		}
	}
	if (range.nMin != -1)
	{
		// No ranges if all cells are selected
		if (range.nMin != 1 && range.nMax != count_ad_cells)
		{
			sheet->m_Page2.m_listPrintRange.push_back(range);
		}
	}

	sheet->m_Page2.m_nPunchModulus = punch_ad_modulus;
}

void PhreeqcI::GetData(CCKSMix* sheet)const
{
	ASSERT(this->Rxn_mix_map.size() == 1);

	std::map<int, cxxMix>::const_iterator ci = this->Rxn_mix_map.begin();
	sheet->m_n_user         = (*ci).first;
	sheet->m_strDesc        = (*ci).second.Get_description().c_str();	
	sheet->m_Page1.mixComps = (*ci).second.Get_mixComps();
}

void PhreeqcI::GetData(CCKSExchange* sheet)const
{
	ASSERT(std::numeric_limits<double>::has_signaling_NaN == true);

	ASSERT(this->Rxn_exchange_map.size() == 1);
	std::map<int, cxxExchange>::const_iterator ci = this->Rxn_exchange_map.begin();
	if (ci != this->Rxn_exchange_map.end())
	{
		sheet->m_n_user     = (*ci).second.Get_n_user();
		sheet->m_n_user_end = (*ci).second.Get_n_user_end();
		sheet->m_strDesc    = (*ci).second.Get_description().c_str();

		if ((*ci).second.Get_solution_equilibria()) 
		{
			sheet->m_bSolution_equilibria = true;
			sheet->m_nEquilSolutionNum    = (*ci).second.Get_n_solution();
		}
		else
		{
			sheet->m_bSolution_equilibria = false;
			sheet->m_nEquilSolutionNum    = CCKSExchange::N_NONE;
		}


		sheet->m_bPitzer_exchange_gammas = false;
		if ((*ci).second.Get_pitzer_exchange_gammas())
		{
			sheet->m_bPitzer_exchange_gammas = true;
		}

		ASSERT(sheet->m_Page1.m_listExchComp.empty());
		ASSERT(sheet->m_Page2.m_listExchComp.empty());
		ASSERT(sheet->m_Page3.m_listExchComp.empty());

		for (size_t i = 0; i < (*ci).second.Get_exchange_comps().size(); ++i)
		{
			CExchComp exchComp(&(*ci).second.Get_exchange_comps()[i]);

			if (!exchComp.m_strRate_name.IsEmpty())
			{
				ASSERT(exchComp.m_strPhase_name.IsEmpty());
				sheet->m_Page3.m_listExchComp.push_back(exchComp);
			}
			else if (!exchComp.m_strPhase_name.IsEmpty())
			{
				sheet->m_Page2.m_listExchComp.push_back(exchComp);
			}
			else
			{
				sheet->m_Page1.m_listExchComp.push_back(exchComp);
			}
		}
	}
}

void PhreeqcI::GetData(CCKSEquilibrium_Phases* sheet)const
{
	ASSERT(this->Rxn_pp_assemblage_map.size() == 1);
	std::map<int, cxxPPassemblage>::const_iterator ci = this->Rxn_pp_assemblage_map.begin();
	if (ci != this->Rxn_pp_assemblage_map.end())
	{
		sheet->m_n_user     = (*ci).second.Get_n_user();
		sheet->m_n_user_end = (*ci).second.Get_n_user_end();
		sheet->m_strDesc    = (*ci).second.Get_description().c_str();

		std::map <std::string, cxxPPassemblageComp >::const_iterator compIt = (*ci).second.Get_pp_assemblage_comps().begin();
		for (; compIt != (*ci).second.Get_pp_assemblage_comps().end(); ++compIt)
		{
			CPurePhase pure_phase(&(*compIt).second);
			sheet->m_Page1.m_listPurePhase.push_back(pure_phase);
		}
	}
}

void PhreeqcI::GetData(CCKSGas_Phase* sheet)const
{
	ASSERT(this->Rxn_gas_phase_map.size() == 1);
	std::map<int, cxxGasPhase>::const_iterator ci = this->Rxn_gas_phase_map.begin();
	if (ci != this->Rxn_gas_phase_map.end())
	{
		sheet->m_n_user               = (*ci).second.Get_n_user();
		sheet->m_n_user_end           = (*ci).second.Get_n_user_end();
		sheet->m_strDesc              = (*ci).second.Get_description().c_str();
		sheet->m_Page1.m_dTempC       = (*ci).second.Get_temperature() - 273.15;
		sheet->m_Page1.m_dVolumeL     = (*ci).second.Get_volume();
		sheet->m_Page1.m_dPressureAtm = (*ci).second.Get_total_p();
		sheet->m_Page1.m_nType        = (*ci).second.Get_type();

		if ((*ci).second.Get_solution_equilibria())
		{
			sheet->m_Page1.m_nSolution = (*ci).second.Get_n_solution();
		}
		else
		{
			sheet->m_Page1.m_nSolution = CCKPGas_PhasePg1::NONE;
		}
		
		for (size_t i = 0; i < (*ci).second.Get_gas_comps().size(); ++i)
		{
			CGasComp gasComp(&(*ci).second.Get_gas_comps()[i]);
			sheet->m_Page1.m_listGasComp.push_back(gasComp);
		}
	}
}

void PhreeqcI::GetData(CCKSReaction* sheet)const
{
	ASSERT(std::numeric_limits<double>::has_signaling_NaN == true);
	ASSERT(std::numeric_limits<LDBLE>::has_signaling_NaN == true);

	ASSERT(this->Rxn_reaction_map.size() >= 1);

	std::map<int, cxxReaction>::const_iterator ci = this->Rxn_reaction_map.begin();
	if (ci != this->Rxn_reaction_map.end())
	{
		// reaction number
		sheet->m_n_user               = (*ci).second.Get_n_user();
		sheet->m_n_user_end           = (*ci).second.Get_n_user_end();
		sheet->m_strDesc              = (*ci).second.Get_description().c_str();

		// fill page 1
		cxxNameDouble::const_iterator ri = (*ci).second.Get_reactantList().begin();
		for (; ri != (*ci).second.Get_reactantList().end(); ++ri)
		{
			sheet->m_Page1.m_listNameCoef.push_back(ri);
		}

		// fill page 2
		if ((*ci).second.Get_equalIncrements())
		{
			// equal increments
			sheet->m_Page2.m_nType = CCKPReactionPg2::TYPE_LINEAR;

			sheet->m_Page2.m_nLinearSteps = (*ci).second.Get_countSteps();
			sheet->m_Page2.m_dLinearAmt = (*ci).second.Get_steps()[0];
		}
		else
		{
			// list of increments
			sheet->m_Page2.m_nType = CCKPReactionPg2::TYPE_LIST;

			for (size_t i = 0; i < (*ci).second.Get_steps().size(); ++i)
			{
				sheet->m_Page2.m_listSteps.push_back((*ci).second.Get_steps()[i]);
			}
		}

		if (CString(_T("Mol")).CompareNoCase((*ci).second.Get_units().c_str()) == 0)
		{
			sheet->m_Page2.m_nUnits = CCKPReactionPg2::UNITS_MOLES;
		}
		else if (CString(_T("mMol")).CompareNoCase((*ci).second.Get_units().c_str()) == 0)
		{
			sheet->m_Page2.m_nUnits = CCKPReactionPg2::UNITS_MILLIMOLES;
		}
		else if (CString(_T("uMol")).CompareNoCase((*ci).second.Get_units().c_str()) == 0)
		{
			sheet->m_Page2.m_nUnits = CCKPReactionPg2::UNITS_MICROMOLES;
		}
	}
}

void PhreeqcI::GetData(CCKSReaction_Temperature* sheet)const
{
	ASSERT(std::numeric_limits<double>::has_signaling_NaN == true);
	ASSERT(this->Rxn_temperature_map.size() == 1);

	std::map<int, cxxTemperature>::const_iterator ci = this->Rxn_temperature_map.begin();
	if (ci != this->Rxn_temperature_map.end())
	{
		// reaction number
		sheet->m_n_user     = (*ci).second.Get_n_user();
		sheet->m_n_user_end = (*ci).second.Get_n_user_end();
		sheet->m_strDesc    = (*ci).second.Get_description().c_str();

		if ((*ci).second.Get_equalIncrements())
		{
			sheet->m_Page1.m_nType        = CCKPReaction_TemperaturePg1::TYPE_LINEAR;
			sheet->m_Page1.m_nLinearSteps = (*ci).second.Get_countTemps();
			sheet->m_Page1.m_dTemp1       = (*ci).second.Get_temps()[0];
			sheet->m_Page1.m_dTemp2       = (*ci).second.Get_temps()[1];
		}
		else
		{
			// explicit
			sheet->m_Page1.m_nType = CCKPReaction_TemperaturePg1::TYPE_LIST;
			for (int i = 0; i < (*ci).second.Get_countTemps(); ++i)
			{
				sheet->m_Page1.m_listSteps.push_back((*ci).second.Get_temps()[i]);
			}
		}
	}
}

void PhreeqcI::GetData(CCKSSolution* sheet)const
{
	ASSERT(this->Rxn_solution_map.size() == 1);
	std::map<int, cxxSolution>::const_iterator ci = this->Rxn_solution_map.begin();
	if (ci != this->Rxn_solution_map.end())
	{
		// reaction number
		sheet->m_n_user             = (*ci).second.Get_n_user();
		sheet->m_n_user_end         = (*ci).second.Get_n_user_end();
		sheet->m_strDesc            = (*ci).second.Get_description().c_str();

		sheet->m_Page1.m_dPH        = (*ci).second.Get_ph();
		sheet->m_Page1.m_dPE        = (*ci).second.Get_pe();
		sheet->m_Page1.m_dTemp      = (*ci).second.Get_tc();
		sheet->m_Page1.m_strRedox   = (*ci).second.Get_initial_data()->Get_default_pe();
		sheet->m_Page1.m_dDensity   = (*ci).second.Get_density();
		sheet->m_Page1.m_dWaterMass = (*ci).second.Get_mass_water();

		sheet->m_Page2.m_strDefaultUnits = (*ci).second.Get_initial_data()->Get_units().c_str();
		sheet->m_Page2.m_strDefaultUnits.Replace(_T("mg/kgs"), _T("ppm"));
		sheet->m_Page2.m_strDefaultUnits.Replace(_T("ug/kgs"), _T("ppb"));
		sheet->m_Page2.m_strDefaultUnits.Replace(_T("g/kgs"), _T("ppt"));

		const std::map<std::string, cxxISolutionComp>& m = (*ci).second.Get_initial_data()->Get_comps();
		std::map<std::string, cxxISolutionComp>::const_iterator mi = m.begin();
		for (; mi != m.end(); ++mi)
		{
			if (::strcmp((*mi).first.c_str(), "H(1)") == 0)
			{
				if (!(*mi).second.Get_equation_name().empty())
				{
					// check for charge
					if (this->strcmp_nocase_arg1((*mi).second.Get_equation_name().c_str(), "charge") == 0)
					{
						sheet->m_Page2.m_strChargeBalance = _T("(pH)");
					}
					// phase
					else
					{
						sheet->m_Page1.m_strPH_Equil = (*mi).second.Get_equation_name().c_str();
						sheet->m_Page1.m_dPH_SI      = (*mi).second.Get_phase_si();
					}
				}
				continue;
			}
			if (::strcmp((*mi).first.c_str(), "E") == 0)
			{
				if (!(*mi).second.Get_equation_name().empty())
				{
					// check for charge
					if (this->strcmp_nocase_arg1((*mi).second.Get_equation_name().c_str(), "charge") == 0)
					{
						sheet->m_Page2.m_strChargeBalance = _T("(pe)");
					}
					// phase
					else
					{
						sheet->m_Page1.m_strPE_Equil = (*mi).second.Get_equation_name().c_str();
						sheet->m_Page1.m_dPE_SI      = (*mi).second.Get_phase_si();
					}
				}
				continue;
			}
			CConc conc(&((*ci).second), &((*mi).second));
			if (conc.m_strPhase.CompareNoCase(_T("charge")) == 0)
			{
				sheet->m_Page2.m_strChargeBalance = conc.m_strDesc;
			}
			sheet->m_Page2.m_listConc.push_back(conc);
		}

		// isotopes
		std::map<std::string, cxxSolutionIsotope>::const_iterator ii = (*ci).second.Get_isotopes().begin();
		for (; ii != (*ci).second.Get_isotopes().end(); ++ii)
		{
			CIsotope isotope(&(*ii).second);
			sheet->m_Page3.m_listIsotopes.push_back(isotope);
		}
	}
}

void PhreeqcI::GetData(CCKSSolid_Solutions* sheet)const
{
	ASSERT(this->Rxn_ss_assemblage_map.size() == 1);

	std::map<int, cxxSSassemblage>::const_iterator si = this->Rxn_ss_assemblage_map.begin();
	if (si != this->Rxn_ss_assemblage_map.end())
	{
		sheet->m_n_user             = (*si).second.Get_n_user();
		sheet->m_n_user_end         = (*si).second.Get_n_user_end();
		sheet->m_strDesc            = (*si).second.Get_description().c_str();

		std::map<std::string, cxxSS>::const_iterator ss = (*si).second.Get_SSs().begin();
		for (; ss != (*si).second.Get_SSs().end(); ++ss)
		{
			CS_S S_S(&(*ss).second);

			if (S_S.m_nInputCase == CS_S::IC_GUGG_NONDIMENSIONAL && (S_S.m_arrldP[0] == 0.0 && S_S.m_arrldP[1] == 0.0))
			{
				// ideal
				ASSERT( S_S.m_listComp.size() >= 2 );
				sheet->m_Page1.m_listS_S.push_back(S_S);
			}
			else
			{
				// non-ideal
				ASSERT( S_S.m_listComp.size() == 2 );
				sheet->m_Page2.m_listS_S.push_back(S_S);
			}
		}
	}
}

void PhreeqcI::GetData(COCKSRates* sheet)const
{
	// empty keyword will assert
	// ASSERT(count_rates >= 1);

	for (int i = 0; i < this->count_rates; ++i)
	{
		CRate rate(&this->rates[i]);
		sheet->m_Page1.m_listRates.push_back(rate);
	}
}

void PhreeqcI::GetData(CCKSInverse* sheet)const
{
	ASSERT(std::numeric_limits<double>::has_signaling_NaN == true);

	ASSERT(this->count_inverse == 1);
	ASSERT(inverse != NULL);
	
	struct inverse* inverse_ptr = &inverse[0];

	sheet->m_n_user  = inverse_ptr->n_user;
	sheet->m_strDesc = inverse_ptr->description;

	ASSERT(inverse_ptr->count_solns > 0);

	// initial solns
	double dLastUncertainty = 0.05;
	bool bLastForce = false;
	for (int i = 0; i < inverse_ptr->count_solns - 1; ++i)
	{
		InvSol invSol;

		invSol.nSol = inverse_ptr->solns[i];
	
		if (i < inverse_ptr->count_uncertainties)
		{
			dLastUncertainty = inverse_ptr->uncertainties[i];
		}
		invSol.dUncertainty = dLastUncertainty;

		if (i < inverse_ptr->count_force_solns)
		{
			bLastForce = (inverse_ptr->force_solns[i] == TRUE);
		}
		invSol.bForce = bLastForce;

		sheet->m_listInvSol.push_back(invSol);
	}

	// final soln
	if (inverse_ptr->count_solns > 0)
	{
		int j = inverse_ptr->count_solns - 1;
		sheet->m_invSolFinal.nSol = inverse_ptr->solns[j];
		if (j < inverse_ptr->count_uncertainties)
		{
			dLastUncertainty = inverse_ptr->uncertainties[j];
		}
		sheet->m_invSolFinal.dUncertainty = dLastUncertainty;

		if (j < inverse_ptr->count_force_solns)
		{
			bLastForce = (inverse_ptr->force_solns[j] == TRUE);
		}
		sheet->m_invSolFinal.bForce = bLastForce;
	}

	// -phases
	for (int nPhase = 0; nPhase < inverse_ptr->count_phases; ++nPhase)
	{
		CInvPhase invPhase(&inverse_ptr->phases[nPhase]);
		
		for (int nIsotope = 0; nIsotope < inverse_ptr->phases[nPhase].count_isotopes; ++nIsotope)
		{
			CIsotope isotope(&inverse_ptr->phases[nPhase].isotopes[nIsotope]);
			invPhase.m_mapIsotopes[isotope.m_strName] = isotope;
		}
		sheet->m_Page1B.m_listInvPhase.push_back(invPhase);
	}

	// water_uncertainty

	// -balances
	// pH
	CInvElem elem;
	elem.m_strName = _T("pH");
	bool bAddpH = false;

	// foreach soln
	dLastUncertainty = 0.05;
	int i = 0;
	for (; i < inverse_ptr->count_solns; ++i)
	{
		if (i < inverse_ptr->count_ph_uncertainties)
		{
			dLastUncertainty = inverse_ptr->ph_uncertainties[i];
			if (dLastUncertainty != 0.05)
			{
				bAddpH = true;
			}
		}
		elem.m_mapSol2Unc[inverse_ptr->solns[i]] = dLastUncertainty;
	}
	
	// final solution
	if (i < inverse_ptr->count_ph_uncertainties)
	{
		dLastUncertainty = inverse_ptr->ph_uncertainties[i];
		if (dLastUncertainty != 0.05)
		{
			bAddpH = true;
		}
	}
	elem.m_mapSol2Unc[sheet->m_invSolFinal.nSol] = dLastUncertainty;
	if (bAddpH)
	{
		sheet->m_listInvElem.push_back(elem);
	}

	// now foreach elem
	for (int nElt = 0; nElt < inverse_ptr->count_elts; ++nElt)
	{
		// get element name
		CInvElem elem;
		elem.m_strName = inverse_ptr->elts[nElt].name;

		if (inverse_ptr->elts[nElt].count_uncertainties == 0)
		{
			// foreach soln
			std::list<InvSol>::iterator iterInvSol = sheet->m_listInvSol.begin();
			for (; iterInvSol != sheet->m_listInvSol.end(); ++iterInvSol)
			{
				elem.m_mapSol2Unc[iterInvSol->nSol] = iterInvSol->dUncertainty;
			}
			// final solution
			elem.m_mapSol2Unc[sheet->m_invSolFinal.nSol] = sheet->m_invSolFinal.dUncertainty;
		}
		else
		{
			ASSERT(inverse_ptr->elts[nElt].count_uncertainties > 0);

			// foreach soln
			std::list<InvSol>::iterator iterInvSol = sheet->m_listInvSol.begin();
			int nUnc = 0;
			for (; iterInvSol != sheet->m_listInvSol.end(); ++iterInvSol, ++nUnc)
			{
				if (nUnc < inverse_ptr->elts[nElt].count_uncertainties)
				{
					dLastUncertainty = inverse_ptr->elts[nElt].uncertainties[nUnc];
				}
				elem.m_mapSol2Unc[iterInvSol->nSol] = dLastUncertainty;
			}

			// final solution
			if (nUnc < inverse_ptr->elts[nElt].count_uncertainties)
			{
				dLastUncertainty = inverse_ptr->elts[nElt].uncertainties[nUnc];
			}
			elem.m_mapSol2Unc[sheet->m_invSolFinal.nSol] = dLastUncertainty;
		}

		// add to list
		sheet->m_listInvElem.push_back(elem);
	}

	// -isotopes
	// foreach isotope
	// for (int nIso = 0; nIso < inverse_ptr->count_isotopes; ++nIso)
	for (int nIso = 0; nIso < inverse_ptr->count_i_u; ++nIso)
	{
		// CInvIsotope invIsotope(&inverse_ptr->isotopes[nIso]);
		CInvIsotope invIsotope(&inverse_ptr->i_u[nIso]);

		if (inverse_ptr->i_u[nIso].count_uncertainties != 0)
		{
			// foreach initial soln
			std::list<InvSol>::iterator iterInvSol = sheet->m_listInvSol.begin();
			int nIsoUnc = 0;
			for (; iterInvSol != sheet->m_listInvSol.end(); ++iterInvSol, ++nIsoUnc)
			{
				if (nIsoUnc < inverse_ptr->i_u[nIso].count_uncertainties)
				{
					dLastUncertainty = inverse_ptr->i_u[nIso].uncertainties[nIsoUnc];
				}
				invIsotope.m_mapSol2Unc[iterInvSol->nSol] = dLastUncertainty;
			}
			// final solution
			if (nIsoUnc < inverse_ptr->i_u[nIso].count_uncertainties)
			{
				dLastUncertainty = inverse_ptr->i_u[nIso].uncertainties[nIsoUnc];
			}
			invIsotope.m_mapSol2Unc[sheet->m_invSolFinal.nSol] = dLastUncertainty;
		}		
		sheet->m_Page3.m_listInvIsotopes.push_back(invIsotope);
	}


	// -range
	sheet->m_Page1A.m_bRange = inverse_ptr->range;
	sheet->m_Page1A.m_dRange = inverse_ptr->range_max;

	// minimal
	sheet->m_Page1A.m_bMinimal = inverse_ptr->minimal;

	// -tolerance
	sheet->m_Page1A.m_dTol = inverse_ptr->tolerance;

	// -uncertainty_water
	sheet->m_Page2.m_bWaterUncert = (inverse_ptr->water_uncertainty != 0.0);
	sheet->m_Page2.m_dWaterUncert = inverse_ptr->water_uncertainty;

	// -mineral_water
	sheet->m_Page1A.m_bMineralWater = inverse_ptr->mineral_water;

	// -multiple_precision
	sheet->m_Page1A.m_bMPSolve = inverse_ptr->mp;

	// -mp_tolerance
	sheet->m_Page1A.m_dMPTol = inverse_ptr->mp_tolerance;

	// -censor_mp
	sheet->m_Page1A.m_dMPCensor = inverse_ptr->mp_censor;

	// -lon_netpath
	sheet->m_Page1A.m_bLonNetpath = (inverse_ptr->netpath != NULL);
	if (sheet->m_Page1A.m_bLonNetpath)
	{
		sheet->m_Page1A.m_sLonPrefix = inverse_ptr->netpath;
	}

	// -pat_netpath
	sheet->m_Page1A.m_bPatNetpath = (inverse_ptr->pat != NULL);
	if (sheet->m_Page1A.m_bPatNetpath)
	{
		sheet->m_Page1A.m_sPatPrefix = inverse_ptr->pat;
	}
}

void PhreeqcI::GetData(COCKSUserPrint* sheet)const
{
	ASSERT(this->user_print != NULL);

	CRate rate(this->user_print);

	sheet->m_Page1.m_listCommands.assign(rate.m_listCommands.begin(), rate.m_listCommands.end());
}

void PhreeqcI::GetData(COCKSUserPunch* sheet)const
{
	size_t n = this->UserPunch_map.size();

	CString strDelim = _T(" ");
	std::map< int, UserPunch >::const_iterator ci = this->UserPunch_map.begin();
	if (ci != this->UserPunch_map.end())
	{
		// number description
		sheet->m_n_user     = (*ci).second.Get_n_user();
		sheet->m_n_user_end = (*ci).second.Get_n_user_end();
		sheet->m_strDesc    = (*ci).second.Get_description().c_str();

		// -headings
		if ((*ci).second.Get_headings().size())
		{
			sheet->m_Page1.m_strHead = (*ci).second.Get_headings()[0].c_str();
			for (size_t i = 1; i < (*ci).second.Get_headings().size(); ++i)
			{
				sheet->m_Page1.m_strHead += strDelim + (*ci).second.Get_headings()[i].c_str();
			}
		}
		// basic
		CRate rate((*ci).second.Get_rate());
		sheet->m_Page1.m_listCommands.assign(rate.m_listCommands.begin(), rate.m_listCommands.end());
	}
}

void PhreeqcI::GetData(CSurfaceSheet* sheet)const
{
	ASSERT(this->Rxn_surface_map.size() == 1);

	std::map<int, cxxSurface>::const_iterator si = this->Rxn_surface_map.begin();
	if (si != this->Rxn_surface_map.end())
	{
		// Surface number
		sheet->m_n_user             = (*si).second.Get_n_user();
		sheet->m_n_user_end         = (*si).second.Get_n_user_end();
		sheet->m_strDesc            = (*si).second.Get_description().c_str();

		sheet->Page.dl_type         = (*si).second.Get_dl_type();
		sheet->Page.type            = (*si).second.Get_type();
		sheet->Page.thickness       = (*si).second.Get_thickness();
		sheet->Page.sites_units     = (*si).second.Get_sites_units();
		sheet->Page.transport       = FALSE; // set later

		// solution equilibration
		if ((*si).second.Get_solution_equilibria())
		{
			sheet->Page.n_solution = (*si).second.Get_n_solution();
			ASSERT(sheet->Page.n_solution > 0);
		}
		else
		{
			sheet->Page.n_solution = CSurfacePage::N_NONE;
		}

		// counter ions only
		sheet->Page.only_counter_ions = (*si).second.Get_only_counter_ions();

		// viscosity
		sheet->Page.DDL_viscosity     = (*si).second.Get_DDL_viscosity();

		// Donnan (thickness or debye)
		sheet->Page.DT = CSurfacePage::DT_THICKNESS;
		if ((*si).second.Get_debye_lengths() != 0.0)
		{
			sheet->Page.DT            = CSurfacePage::DT_DEBYE_LENGTHS;
			sheet->Page.debye_lengths = (*si).second.Get_debye_lengths();
			sheet->Page.DDL_limit     = (*si).second.Get_DDL_limit();
		}

		const std::vector<cxxSurfaceComp> &vec = (*si).second.Get_surface_comps();
		for (size_t i = 0; i < vec.size(); ++i)
		{
			CSurfComp surfComp(&(*si).second, &vec[i]);
			if (!surfComp.m_strRate_name.IsEmpty())
			{
				ASSERT( surfComp.m_strPhase_name.IsEmpty() == TRUE );
				sheet->Page.ListSurfComp[CSurfacePage::TI_KINETIC_REACTANTS].push_back(surfComp);
			}
			else if (!surfComp.m_strPhase_name.IsEmpty())
			{
				sheet->Page.ListSurfComp[CSurfacePage::TI_EQUILIBRIUM_PHASES].push_back(surfComp);
			}
			else
			{
				sheet->Page.ListSurfComp[CSurfacePage::TI_GENERAL].push_back(surfComp);
			}

			// check for transport
			if ((*si).second.Get_transport())
			{
				sheet->Page.transport = TRUE;
			}
		}
	}
}

void PhreeqcI::GetData(CKSPrint* sheet)const
{
	int val;

	val = pr.alkalinity;
	sheet->m_Page1.m_arrValue[0] = (val > 0) ? CKPPrintPg1::AS_TRUE : (val < 0) ?  CKPPrintPg1::AS_IS :  CKPPrintPg1::AS_FALSE;

	val = pr.echo_input;
	sheet->m_Page1.m_arrValue[1] = (val > 0) ? CKPPrintPg1::AS_TRUE : (val < 0) ?  CKPPrintPg1::AS_IS :  CKPPrintPg1::AS_FALSE;

	val = pr.eh;
	sheet->m_Page1.m_arrValue[2] = (val > 0) ? CKPPrintPg1::AS_TRUE : (val < 0) ?  CKPPrintPg1::AS_IS :  CKPPrintPg1::AS_FALSE;

	val = pr.pp_assemblage;
	sheet->m_Page1.m_arrValue[3] = (val > 0) ? CKPPrintPg1::AS_TRUE : (val < 0) ?  CKPPrintPg1::AS_IS :  CKPPrintPg1::AS_FALSE;

	val = pr.exchange;
	sheet->m_Page1.m_arrValue[4] = (val > 0) ? CKPPrintPg1::AS_TRUE : (val < 0) ?  CKPPrintPg1::AS_IS :  CKPPrintPg1::AS_FALSE;

	val = pr.gas_phase;
	sheet->m_Page1.m_arrValue[5] = (val > 0) ? CKPPrintPg1::AS_TRUE : (val < 0) ?  CKPPrintPg1::AS_IS :  CKPPrintPg1::AS_FALSE;

	val = pr.headings;
	sheet->m_Page1.m_arrValue[6] = (val > 0) ? CKPPrintPg1::AS_TRUE : (val < 0) ?  CKPPrintPg1::AS_IS :  CKPPrintPg1::AS_FALSE;

	val = pr.inverse;
	sheet->m_Page1.m_arrValue[7] = (val > 0) ? CKPPrintPg1::AS_TRUE : (val < 0) ?  CKPPrintPg1::AS_IS :  CKPPrintPg1::AS_FALSE;

	val = pr.kinetics;
	sheet->m_Page1.m_arrValue[8] = (val > 0) ? CKPPrintPg1::AS_TRUE : (val < 0) ?  CKPPrintPg1::AS_IS :  CKPPrintPg1::AS_FALSE;

	val = pr.use;
	sheet->m_Page1.m_arrValue[9] = (val > 0) ? CKPPrintPg1::AS_TRUE : (val < 0) ?  CKPPrintPg1::AS_IS :  CKPPrintPg1::AS_FALSE;

	val = pr.saturation_indices;
	sheet->m_Page1.m_arrValue[10] = (val > 0) ? CKPPrintPg1::AS_TRUE : (val < 0) ?  CKPPrintPg1::AS_IS :  CKPPrintPg1::AS_FALSE;

	val = pr.ss_assemblage;
	sheet->m_Page1.m_arrValue[11] = (val > 0) ? CKPPrintPg1::AS_TRUE : (val < 0) ?  CKPPrintPg1::AS_IS :  CKPPrintPg1::AS_FALSE;

	val = pr.species;
	sheet->m_Page1.m_arrValue[12] = (val > 0) ? CKPPrintPg1::AS_TRUE : (val < 0) ?  CKPPrintPg1::AS_IS :  CKPPrintPg1::AS_FALSE;

	val = pr.surface;
	sheet->m_Page1.m_arrValue[13] = (val > 0) ? CKPPrintPg1::AS_TRUE : (val < 0) ?  CKPPrintPg1::AS_IS :  CKPPrintPg1::AS_FALSE;

	val = pr.totals;
	sheet->m_Page1.m_arrValue[14] = (val > 0) ? CKPPrintPg1::AS_TRUE : (val < 0) ?  CKPPrintPg1::AS_IS :  CKPPrintPg1::AS_FALSE;

	val = pr.user_print;
	sheet->m_Page1.m_arrValue[15] = (val > 0) ? CKPPrintPg1::AS_TRUE : (val < 0) ?  CKPPrintPg1::AS_IS :  CKPPrintPg1::AS_FALSE;	
	
	val = pr.punch;
	sheet->m_Page1.m_arrValue[16] = (val > 0) ? CKPPrintPg1::AS_TRUE : (val < 0) ?  CKPPrintPg1::AS_IS :  CKPPrintPg1::AS_FALSE;

	val = pr.status;
	sheet->m_Page1.m_arrValue[17] = (val > 0) ? CKPPrintPg1::AS_TRUE : (val < 0) ?  CKPPrintPg1::AS_IS :  CKPPrintPg1::AS_FALSE;

	sheet->m_Page1.m_nLimit = pr.warnings;

	sheet->m_Page1.m_censor = censor;
}

void PhreeqcI::GetData(CCKSKinetics* sheet)const
{
	ASSERT(Rxn_kinetics_map.size() > 0); // ASSERT(Rxn_kinetics_map.size() >= 1);

	std::map<int, cxxKinetics>::const_iterator ci = this->Rxn_kinetics_map.begin();
	if (ci != this->Rxn_kinetics_map.end())
	{
		sheet->m_n_user     = (*ci).second.Get_n_user();
		sheet->m_n_user_end = (*ci).second.Get_n_user_end();
		sheet->m_strDesc    = (*ci).second.Get_description().c_str();

		//
		// fill page 1
		//

		// Lines 1-6
		std::vector<cxxKineticsComp>::const_iterator i = (*ci).second.Get_kinetics_comps().begin();
		for (; i != (*ci).second.Get_kinetics_comps().end(); ++i)
		{
			CKineticComp comp(&(*i));
			sheet->m_Page1.m_listComps.push_back(comp);
		}

		//
		// fill page 1A
		//

		// Line 8
		sheet->m_Page1A.m_dStepDivide = (*ci).second.Get_step_divide();

		// Line 9
		switch ((*ci).second.Get_rk())
		{
		case 1 :
			sheet->m_Page1A.m_nRKType = CCKPKineticsPg1A::RK_1;
			break;
		case 2 :
			sheet->m_Page1A.m_nRKType = CCKPKineticsPg1A::RK_2;
			break;
		case 3 :
			sheet->m_Page1A.m_nRKType = CCKPKineticsPg1A::RK_3;
			break;
		case 6 :
			sheet->m_Page1A.m_nRKType = CCKPKineticsPg1A::RK_6;
			break;
		default :
			sheet->m_Page1A.m_nRKType = CCKPKineticsPg1A::RK_3;
			break;
		}


		sheet->m_Page1A.m_nODEMethodType = CCKPKineticsPg1A::ODE_RUNGA_KUTTA;
		if ((*ci).second.Get_use_cvode())
		{
			sheet->m_Page1A.m_nODEMethodType = CCKPKineticsPg1A::ODE_CVODE;
		}
		sheet->m_Page1A.m_nCVMaxBadSteps = (*ci).second.Get_bad_step_max();
		sheet->m_Page1A.m_nRKMaxBadSteps = (*ci).second.Get_bad_step_max();

		sheet->m_Page1A.m_nCVOrder = (*ci).second.Get_cvode_order();
		sheet->m_Page1A.m_nCVSteps = (*ci).second.Get_cvode_steps();

		//
		// fill page 2
		//

		// Line 7
		if ((*ci).second.Get_equalIncrements() /* kinetics_ptr->count_steps < 0 */)
		{
			// equal increments
			sheet->m_Page2.m_nAmountType = CCKPKineticsPg2::TYPE_LINEAR;

			sheet->m_Page2.m_nLinearSteps = (*ci).second.Get_count();
			sheet->m_Page2.m_dLinearAmt   = (*ci).second.Get_steps()[0];
			ASSERT((*ci).second.Get_steps().size() == 1);
		}
		else
		{
			// list of increments
			sheet->m_Page2.m_nAmountType = CCKPKineticsPg2::TYPE_LIST;

			for (size_t i = 0; i < (*ci).second.Get_steps().size(); ++i)
			{
				sheet->m_Page2.m_listSteps.push_back((*ci).second.Get_steps()[i]);
			}
		}
	}
}

void PhreeqcI::GetData(CKSIncrement* sheet)const
{
	sheet->m_Page1.m_bTrueOrFalse = (this->incremental_reactions > 0) ? TRUE : FALSE;
}

void PhreeqcI::GetData(CKSSelectedOutput* sheet)const
{
	size_t n = this->SelectedOutput_map.size();

	std::map< int, SelectedOutput >::const_iterator it = this->SelectedOutput_map.begin();

	// number description
	sheet->m_n_user     = (*it).second.Get_n_user();
	sheet->m_n_user_end = (*it).second.Get_n_user_end();
	sheet->m_strDesc    = (*it).second.Get_description().c_str();

	// totals	
	for (size_t i = 0; i < (*it).second.Get_totals().size(); ++i)
	{
		sheet->m_Page2.m_listTotals.push_back((*it).second.Get_totals()[i].first.c_str());
	}

	// molalities
	for (size_t i = 0; i < (*it).second.Get_molalities().size(); ++i)
	{
		sheet->m_Page3.m_listMolalities.push_back((*it).second.Get_molalities()[i].first.c_str());
	}

	// activities
	for (size_t i = 0; i < (*it).second.Get_activities().size(); ++i)
	{
		sheet->m_Page4.m_listActivities.push_back((*it).second.Get_activities()[i].first.c_str());
	}

	// phases
	for (size_t i = 0; i < (*it).second.Get_pure_phases().size(); ++i)
	{
		sheet->m_Page5.m_listPhases.push_back((*it).second.Get_pure_phases()[i].first.c_str());
	}

	// si
	for (size_t i = 0; i < (*it).second.Get_si().size(); ++i)
	{
		sheet->m_Page6.m_listPhases.push_back((*it).second.Get_si()[i].first.c_str());
	}


	// gases
	for (size_t i = 0; i < (*it).second.Get_gases().size(); ++i)
	{
		sheet->m_Page7.m_listGases.push_back((*it).second.Get_gases()[i].first.c_str());
	}

	// kinetic reactants
	for (size_t i = 0; i < (*it).second.Get_kinetics().size(); ++i)
	{
		sheet->m_Page8.m_listKinetic.push_back((*it).second.Get_kinetics()[i].first.c_str());
	}

	// solid solutions
	for (size_t i = 0; i < (*it).second.Get_s_s().size(); ++i)
	{
		sheet->m_Page9.m_listSolid.push_back((*it).second.Get_s_s()[i].first.c_str());
	}

	// isotopes
	for (size_t i = 0; i < (*it).second.Get_isotopes().size(); ++i)
	{
		sheet->listIsotopes.push_back((*it).second.Get_isotopes()[i].first.c_str());
	}

	// calculate_values
	for (size_t i = 0; i < (*it).second.Get_calculate_values().size(); ++i)
	{
		sheet->m_Page10.listCalcValues.push_back((*it).second.Get_calculate_values()[i].first.c_str());
	}

	sheet->m_Page1.m_strFileName = (*it).second.Get_file_name().c_str();

	bool val;
	bool set;

	/* selected_out */
	//val = pr.punch;
	//sheet->m_Page1.m_arrValue[0] = (val > 0) ? CKPSelectedOutputPg1::AS_TRUE : (val < 0) ?  CKPSelectedOutputPg1::AS_IS :  CKPSelectedOutputPg1::AS_FALSE;

	/* user_punch */
	val = (*it).second.Get_user_punch();
	set = (*it).second.was_set_user_punch();
	sheet->m_Page1.m_arrValue[1] = (set) ? ((val) ? CKPSelectedOutputPg1::AS_TRUE : CKPSelectedOutputPg1::AS_FALSE) : CKPSelectedOutputPg1::AS_IS;

	/* high_precision */
	val = (*it).second.Get_high_precision();
	set = (*it).second.was_set_high_precision();
	sheet->m_Page1.m_arrValue[2] = (set) ? ((val) ? CKPSelectedOutputPg1::AS_TRUE : CKPSelectedOutputPg1::AS_FALSE) : CKPSelectedOutputPg1::AS_IS;

	/* simulation */
	val = (*it).second.Get_sim();
	set = (*it).second.was_set_sim();
	sheet->m_Page1.m_arrValue[3] = (set) ? ((val) ? CKPSelectedOutputPg1::AS_TRUE : CKPSelectedOutputPg1::AS_FALSE) : CKPSelectedOutputPg1::AS_IS;
	
	/* state */
	val = (*it).second.Get_state();
	set = (*it).second.was_set_state();
	sheet->m_Page1.m_arrValue[4] = (set) ? ((val) ? CKPSelectedOutputPg1::AS_TRUE : CKPSelectedOutputPg1::AS_FALSE) : CKPSelectedOutputPg1::AS_IS;

	/* solution */
	val = (*it).second.Get_soln();
	set = (*it).second.was_set_soln();
	sheet->m_Page1.m_arrValue[5] = (set) ? ((val) ? CKPSelectedOutputPg1::AS_TRUE : CKPSelectedOutputPg1::AS_FALSE) : CKPSelectedOutputPg1::AS_IS;

	/* distance */
	val = (*it).second.Get_dist();
	set = (*it).second.was_set_dist();
	sheet->m_Page1.m_arrValue[6] = (set) ? ((val) ? CKPSelectedOutputPg1::AS_TRUE : CKPSelectedOutputPg1::AS_FALSE) : CKPSelectedOutputPg1::AS_IS;

	/* time */
	val = (*it).second.Get_time();
	set = (*it).second.was_set_time();
	sheet->m_Page1.m_arrValue[7] = (set) ? ((val) ? CKPSelectedOutputPg1::AS_TRUE : CKPSelectedOutputPg1::AS_FALSE) : CKPSelectedOutputPg1::AS_IS;

	/* step */
	val = (*it).second.Get_step();
	set = (*it).second.was_set_step();
	sheet->m_Page1.m_arrValue[8] = (set) ? ((val) ? CKPSelectedOutputPg1::AS_TRUE : CKPSelectedOutputPg1::AS_FALSE) : CKPSelectedOutputPg1::AS_IS;

	/* ph */
	val = (*it).second.Get_ph();
	set = (*it).second.was_set_ph();
	sheet->m_Page1.m_arrValue[9] = (set) ? ((val) ? CKPSelectedOutputPg1::AS_TRUE : CKPSelectedOutputPg1::AS_FALSE) : CKPSelectedOutputPg1::AS_IS;

	/* pe */
	val = (*it).second.Get_pe();
	set = (*it).second.was_set_pe();
	sheet->m_Page1.m_arrValue[10] = (set) ? ((val) ? CKPSelectedOutputPg1::AS_TRUE : CKPSelectedOutputPg1::AS_FALSE) : CKPSelectedOutputPg1::AS_IS;

	/* reaction */
	val = (*it).second.Get_rxn();
	set = (*it).second.was_set_rxn();
	sheet->m_Page1.m_arrValue[11] = (set) ? ((val) ? CKPSelectedOutputPg1::AS_TRUE : CKPSelectedOutputPg1::AS_FALSE) : CKPSelectedOutputPg1::AS_IS;

	/* temperature */
	val = (*it).second.Get_temp();
	set = (*it).second.was_set_temp();
	sheet->m_Page1.m_arrValue[12] = (set) ? ((val) ? CKPSelectedOutputPg1::AS_TRUE : CKPSelectedOutputPg1::AS_FALSE) : CKPSelectedOutputPg1::AS_IS;

	/* alkalinity */
	val = (*it).second.Get_alk();
	set = (*it).second.was_set_alk();
	sheet->m_Page1.m_arrValue[13] = (set) ? ((val) ? CKPSelectedOutputPg1::AS_TRUE : CKPSelectedOutputPg1::AS_FALSE) : CKPSelectedOutputPg1::AS_IS;

	/* ionic_strength */
	val = (*it).second.Get_mu();
	set = (*it).second.was_set_mu();
	sheet->m_Page1.m_arrValue[14] = (set) ? ((val) ? CKPSelectedOutputPg1::AS_TRUE : CKPSelectedOutputPg1::AS_FALSE) : CKPSelectedOutputPg1::AS_IS;

	/* water */
	val = (*it).second.Get_water();
	set = (*it).second.was_set_water();
	sheet->m_Page1.m_arrValue[15] = (set) ? ((val) ? CKPSelectedOutputPg1::AS_TRUE : CKPSelectedOutputPg1::AS_FALSE) : CKPSelectedOutputPg1::AS_IS;

	/* charge_balance */
	val = (*it).second.Get_charge_balance();
	set = (*it).second.was_set_charge_balance();
	sheet->m_Page1.m_arrValue[16] = (set) ? ((val) ? CKPSelectedOutputPg1::AS_TRUE : CKPSelectedOutputPg1::AS_FALSE) : CKPSelectedOutputPg1::AS_IS;

	/* percent_error */
	val = (*it).second.Get_percent_error();
	set = (*it).second.was_set_percent_error();
	sheet->m_Page1.m_arrValue[17] = (set) ? ((val) ? CKPSelectedOutputPg1::AS_TRUE : CKPSelectedOutputPg1::AS_FALSE) : CKPSelectedOutputPg1::AS_IS;

	/* inverse_modeling */
	val = (*it).second.Get_inverse();
	set = (*it).second.was_set_inverse();
	sheet->m_Page1.m_arrValue[18] = (set) ? ((val) ? CKPSelectedOutputPg1::AS_TRUE : CKPSelectedOutputPg1::AS_FALSE) : CKPSelectedOutputPg1::AS_IS;
}

void PhreeqcI::GetData(COCKSSolution_Spread* sheet)const
{
	ASSERT(std::numeric_limits<double>::has_signaling_NaN == true);

	// spread sheet Page 1
	for (int c = 0; g_spread_sheet.heading && c < g_spread_sheet.heading->count; ++c)
	{
		std::string sCell = g_spread_sheet.heading->char_vector[c];
		sheet->m_Page1.m_vsHeading.push_back(sCell);
		if (g_spread_sheet.units && c < g_spread_sheet.units->count)
		{
			std::string sUnit = g_spread_sheet.units->char_vector[c];
			if (!sUnit.empty())
			{
				CConc conc = CConc::Create(sUnit.c_str());
				sheet->m_Page2.m_mapElement2Conc[sCell] = conc;
			}
		}
	}

	for (int r = 0; r < g_spread_sheet.count_rows && g_spread_sheet.rows; ++r)
	{
		std::vector<std::string> vsRow;
		std::string sCell;
		for (int c = 0; c < g_spread_sheet.rows[r]->count; ++c)
		{
			sCell = g_spread_sheet.rows[r]->char_vector[c];
			vsRow.push_back(sCell);			
		}
		sheet->m_Page1.m_vvsCells.push_back(vsRow);
	}
	

	//
	// &General defaults
	//
	sheet->m_Page3.m_dTemp           = g_spread_sheet.defaults.temp;
	sheet->m_Page3.m_dPH             = g_spread_sheet.defaults.ph;
	sheet->m_Page3.m_dPE             = g_spread_sheet.defaults.pe;
	sheet->m_Page3.m_strRedox        = g_spread_sheet.defaults.redox;
	sheet->m_Page3.m_strDefaultUnits = g_spread_sheet.defaults.units;
	sheet->m_Page3.m_dDensity        = g_spread_sheet.defaults.density;
	sheet->m_Page3.m_dWaterMass      = g_spread_sheet.defaults.water;

	// reformat Units
	sheet->m_Page3.m_strDefaultUnits.Replace(_T("Mol"),    _T("mol"));
	sheet->m_Page3.m_strDefaultUnits.Replace(_T("/l"),     _T("/L"));
	sheet->m_Page3.m_strDefaultUnits.Replace(_T("mg/kgs"), _T("ppm"));
	sheet->m_Page3.m_strDefaultUnits.Replace(_T("ug/kgs"), _T("ppb"));
	sheet->m_Page3.m_strDefaultUnits.Replace(_T("g/kgs"),  _T("ppt"));	

	//
	// &Isotope defaults
	//

	// default isotopes
	for (int i = 0; i < g_spread_sheet.defaults.count_iso; ++i)
	{
		CIsotope cisotope(&g_spread_sheet.defaults.iso[i]);

		std::list<CIsotope>::iterator cIter = sheet->m_listIsotopes.begin();
		for (; cIter != sheet->m_listIsotopes.end(); ++cIter)
		{
			if (cIter->m_strName == cisotope.m_strName)
			{
				cIter->m_dRatio            = cisotope.m_dRatio;
				cIter->m_dRatioUncertainty = cisotope.m_dRatioUncertainty;
				break;
			}
		}
		if (cIter == sheet->m_listIsotopes.end()) sheet->m_listIsotopes.push_back(cisotope);
	}
}

void PhreeqcI::GetData(CKSSolutionSpecies* sheet)const
{
	bool bEnableLLNL = false;

	for (int i = 0; i < this->count_s; ++i)
	{
		if(this->s[i]->rxn == NULL) continue;
		CSpecies species(this->s[i]);
		sheet->m_Page1.m_listSpecies.push_back(species);
		if (species.m_nActType == CSpecies::AT_LLNL_DH || species.m_nActType == CSpecies::AT_LLNL_DH_CO2)
		{
			bEnableLLNL = true;
		}
	}
	sheet->m_Page1.EnableLLNL(bEnableLLNL);
}

void PhreeqcI::GetData(CKSSurfaceSpecies* sheet)const
{
	for (int i = 0; i < this->count_s; ++i)
	{
		if(this->s[i]->rxn == NULL) continue;
		CSpecies species(s[i]);
		sheet->m_Page1.m_listSpecies.push_back(species);
	}
}

void PhreeqcI::GetData(CKSExchangeSpecies* sheet)const
{
	bool bEnableLLNL = false;

	for (int i = 0; i < this->count_s; ++i)
	{
		if(this->s[i]->rxn == NULL) continue;
		CSpecies species(this->s[i]);
		sheet->m_Page1.m_listSpecies.push_back(species);
		if (species.m_nActType == CSpecies::AT_LLNL_DH || species.m_nActType == CSpecies::AT_LLNL_DH_CO2)
		{
			bEnableLLNL = true;
		}
	}
	sheet->m_Page1.EnableLLNL(bEnableLLNL);
}

void PhreeqcI::GetData(CKSPhases* sheet)const
{
	for (int i = 0; i < this->count_phases; ++i)
	{
		CPhase phase(this->phases[i]);
		sheet->m_Page1.m_listPhase.push_back(phase);
	}
}

void PhreeqcI::GetData(CKSSolutionMasterSpecies* sheet)const
{
	for (int i = 0; i < this->count_master; ++i)
	{
		CMaster mast(this->master[i]);
		sheet->m_Page1.m_listMaster.push_back(mast);
	}
}

void PhreeqcI::GetData(CKSExchangeMasterSpecies* sheet)const
{
	for (int i = 0; i < this->count_master; ++i)
	{
		ASSERT(master[i]->type == EX);
		CMaster mast(this->master[i]);
		sheet->m_Page1.m_listMaster.push_back(mast);
	}
}

void PhreeqcI::GetData(CKSSurfaceMasterSpecies* sheet)const
{
	for (int i = 0; i < this->count_master; ++i)
	{
		if (master[i]->type == SURF)
		{
			CMaster mast(this->master[i]);
			sheet->m_Page1.m_listMaster.push_back(mast);
		}
	}
}

void PhreeqcI::GetData(CKSKnobs* sheet)const
{
	// fill ints
	sheet->m_Page1.m_nItmax = this->itmax;

	// fill doubles
	sheet->m_Page1.m_dIneqTol              = this->ineq_tol;
	sheet->m_Page1.m_dStepSize             = this->step_size;
	sheet->m_Page1.m_dPeStepSize           = this->pe_step_size;
	sheet->m_Page1.m_dConvergenceTolerance = this->convergence_tolerance;
#ifdef ENABLE_SCALE_PURE_PHASES
	sheet->m_Page1.m_dPPScale              = this->pp_scale;
#endif

	// fill TRUE/FALSE/AS IS

	int nVal;
	
	nVal = this->diagonal_scale;
	sheet->m_Page1.m_arrValue[0] = (nVal > 0) ? CKPKnobsPg1::AS_TRUE : (nVal < 0) ?  CKPKnobsPg1::AS_IS :  CKPKnobsPg1::AS_FALSE;

	nVal = this->debug_model;
	sheet->m_Page1.m_arrValue[1] = (nVal > 0) ? CKPKnobsPg1::AS_TRUE : (nVal < 0) ?  CKPKnobsPg1::AS_IS :  CKPKnobsPg1::AS_FALSE;

	nVal = this->debug_prep;
	sheet->m_Page1.m_arrValue[2] = (nVal > 0) ? CKPKnobsPg1::AS_TRUE : (nVal < 0) ?  CKPKnobsPg1::AS_IS :  CKPKnobsPg1::AS_FALSE;

	nVal = this->debug_set;
	sheet->m_Page1.m_arrValue[3] = (nVal > 0) ? CKPKnobsPg1::AS_TRUE : (nVal < 0) ?  CKPKnobsPg1::AS_IS :  CKPKnobsPg1::AS_FALSE;

	nVal = this->debug_inverse;
	sheet->m_Page1.m_arrValue[4] = (nVal > 0) ? CKPKnobsPg1::AS_TRUE : (nVal < 0) ?  CKPKnobsPg1::AS_IS :  CKPKnobsPg1::AS_FALSE;

	nVal = this->pr.logfile;
	sheet->m_Page1.m_arrValue[5] = (nVal > 0) ? CKPKnobsPg1::AS_TRUE : (nVal < 0) ?  CKPKnobsPg1::AS_IS :  CKPKnobsPg1::AS_FALSE;

	nVal = this->debug_diffuse_layer;
	sheet->m_Page1.m_arrValue[6] = (nVal > 0) ? CKPKnobsPg1::AS_TRUE : (nVal < 0) ?  CKPKnobsPg1::AS_IS :  CKPKnobsPg1::AS_FALSE;

	nVal = this->delay_mass_water;
	sheet->m_Page1.m_arrValue[7] = (nVal > 0) ? CKPKnobsPg1::AS_TRUE : (nVal < 0) ?  CKPKnobsPg1::AS_IS :  CKPKnobsPg1::AS_FALSE;
}

void PhreeqcI::GetData(CKSPitzer* sheet)const
{
	// -MacInnes
	sheet->m_PageGen.m_bMacInnes = (this->ICON == 0) ? false : true;

	// -use_etheta
	sheet->m_PageGen.m_bUseEtheta = (this->use_etheta == 0) ? false : true;

	// -redox
	sheet->m_PageGen.m_bRedox = (this->pitzer_pe == 0) ? false : true;

	for (int i = 0; i < this->count_pitz_param; ++i)
	{
		CPitzParam p(this->pitz_params[i]);
		if (p.type == TYPE_B0)
		{
			sheet->m_PageB0.m_listParams.push_back(p);
		}
		else if(p.type == TYPE_B1)
		{
			sheet->m_PageB1.m_listParams.push_back(p);
		}
		else if(p.type == TYPE_B2)
		{
			sheet->m_PageB2.m_listParams.push_back(p);
		}
		else if(p.type == TYPE_C0)
		{
			sheet->m_PageC0.m_listParams.push_back(p);
		}
		else if(p.type == TYPE_PSI)
		{
			sheet->m_PagePsi.m_listParams.push_back(p);
		}
		else if(p.type == TYPE_THETA)
		{
			sheet->m_PageTheta.m_listParams.push_back(p);
		}
		else if(p.type == TYPE_LAMDA)
		{
			sheet->m_PageLamda.m_listParams.push_back(p);
		}
		else if(p.type == TYPE_ZETA)
		{
			sheet->m_PageZeta.m_listParams.push_back(p);
		}
		else if(p.type == TYPE_MU)
		{
			sheet->m_PageMu.m_listParams.push_back(p);
		}
		else if(p.type == TYPE_ETA)
		{
			sheet->m_PageEta.m_listParams.push_back(p);
		}
		else if(p.type == TYPE_ALPHAS)
		{
			sheet->m_PageAlphas.m_listParams.push_back(p);
		}
	}
}

void PhreeqcI::GetData(CKSSIT* sheet)const
{
	for (int i = 0; i < this->count_sit_param; ++i)
	{
		CPitzParam p(this->sit_params[i]);
		if (p.type == TYPE_SIT_EPSILON)
		{
			sheet->m_PageEpsilon.m_listParams.push_back(p);
		}
	}
}

int PhreeqcI::GetDefinedRanges(CDefinedRanges* ranges)const
{
	int no_number_count = 0;

	std::map<int, cxxSolution>::const_iterator ci = this->Rxn_solution_map.begin();
	for (; ci != this->Rxn_solution_map.end(); ++ci)
	{
		if ((*ci).second.Get_n_user() >= 0)
		{
			(*ranges)[CKeyword::K_SOLUTION].insert(CRange((*ci).second.Get_n_user(), (*ci).second.Get_n_user_end()));
		}
		else
		{
			++no_number_count;
		}
	}

	return no_number_count;
}

void PhreeqcI::GetData(CUserGraph* sheet)const
{
#if defined(__cplusplus_cli)
	if (this->chart_handler.Get_chart_count() > 0)
	{
		if (const ChartObject *o = this->chart_handler.Get_current_chart())
		{
			sheet->m_n_user         = o->Get_n_user();
			sheet->m_strDesc        = o->Get_description().c_str();	

			// Line 1
			// -headings
			if (o->Get_new_headings().size() > 0)
			{
				CString strDelim(_T(" ")); // = _T(" ");
				sheet->m_Page1.m_strHeadings = (o->Get_new_headings()[0]).c_str();

				std::vector< std::string >::const_iterator heads = o->Get_new_headings().begin();
				for (size_t i = 1; i < o->Get_new_headings().size(); ++i)
				{
					sheet->m_Page1.m_strHeadings += strDelim + (o->Get_new_headings()[i]).c_str();
				}
			}

			// Line 2
			// -axis_titles
			ASSERT(o->Get_axis_titles().size() == 3 || o->Get_axis_titles().size() == 0);
			if (o->Get_axis_titles().size() == 3)
			{
				sheet->m_Page1.axis_title_x  = o->Get_axis_titles()[0].c_str();
				sheet->m_Page1.axis_title_y  = o->Get_axis_titles()[1].c_str();
				sheet->m_Page1.axis_title_y2 = o->Get_axis_titles()[2].c_str();
			}

			// Line 3
			// -chart_title
			sheet->m_Page1.chart_title           = o->Get_chart_title().c_str();

			// Line 4
			// -axis_scale x_axis
			const double *axis_scale_x           = o->Get_axis_scale_x();

			// x min
			if (axis_scale_x[0] == NA)
			{
				sheet->m_Page1.auto_min_x = true;
			}
			else
			{
				sheet->m_Page1.auto_min_x = false;
				sheet->m_Page1.min_x      = axis_scale_x[0];
			}

			// x max
			if (axis_scale_x[1] == NA)
			{
				sheet->m_Page1.auto_max_x = true;
			}
			else
			{
				sheet->m_Page1.auto_max_x = false;
				sheet->m_Page1.max_x      = axis_scale_x[1];
			}
			
			// x major
			if (axis_scale_x[2] == NA)
			{
				sheet->m_Page1.auto_maj_x = true;
			}
			else
			{
				sheet->m_Page1.auto_maj_x = false;
				sheet->m_Page1.maj_x      = axis_scale_x[2];
			}

			// x minor
			if (axis_scale_x[3] == NA)
			{
				sheet->m_Page1.auto_minor_x = true;
			}
			else
			{
				sheet->m_Page1.auto_minor_x = false;
				sheet->m_Page1.minor_x      = axis_scale_x[3];
			}

			// log x
			if (axis_scale_x[4] == 10.0)
			{
				sheet->m_Page1.log_x = true;
			}
			else
			{
				sheet->m_Page1.log_x = false;
			}

			// Line 4a
			// -axis_scale y_axis
			const double *axis_scale_y = o->Get_axis_scale_y();

			// y min
			if (axis_scale_y[0] == NA)
			{
				sheet->m_Page1.auto_min_y = true;
			}
			else
			{
				sheet->m_Page1.auto_min_y = false;
				sheet->m_Page1.min_y      = axis_scale_y[0];
			}

			// y max
			if (axis_scale_y[1] == NA)
			{
				sheet->m_Page1.auto_max_y = true;
			}
			else
			{
				sheet->m_Page1.auto_max_y = false;
				sheet->m_Page1.max_y      = axis_scale_y[1];
			}
			
			// y major
			if (axis_scale_y[2] == NA)
			{
				sheet->m_Page1.auto_maj_y = true;
			}
			else
			{
				sheet->m_Page1.auto_maj_y = false;
				sheet->m_Page1.maj_y      = axis_scale_y[2];
			}

			// y minor
			if (axis_scale_y[3] == NA)
			{
				sheet->m_Page1.auto_minor_y = true;
			}
			else
			{
				sheet->m_Page1.auto_minor_y = false;
				sheet->m_Page1.minor_y      = axis_scale_y[3];
			}

			// log y
			if (axis_scale_y[4] == 10.0)
			{
				sheet->m_Page1.log_y = true;
			}
			else
			{
				sheet->m_Page1.log_y = false;
			}

			// Line 4b
			// -axis_scale sy_axis
			const double *axis_scale_y2 = o->Get_axis_scale_y2();

			// y2 min
			if (axis_scale_y2[0] == NA)
			{
				sheet->m_Page1.auto_min_y2 = true;
			}
			else
			{
				sheet->m_Page1.auto_min_y2 = false;
				sheet->m_Page1.min_y2      = axis_scale_y2[0];
			}

			// y2 max
			if (axis_scale_y2[1] == NA)
			{
				sheet->m_Page1.auto_max_y2 = true;
			}
			else
			{
				sheet->m_Page1.auto_max_y2 = false;
				sheet->m_Page1.max_y2      = axis_scale_y2[1];
			}
			
			// y2 major
			if (axis_scale_y2[2] == NA)
			{
				sheet->m_Page1.auto_maj_y2 = true;
			}
			else
			{
				sheet->m_Page1.auto_maj_y2 = false;
				sheet->m_Page1.maj_y2      = axis_scale_y2[2];
			}

			// y2 minor
			if (axis_scale_y2[3] == NA)
			{
				sheet->m_Page1.auto_minor_y2 = true;
			}
			else
			{
				sheet->m_Page1.auto_minor_y2 = false;
				sheet->m_Page1.minor_y2      = axis_scale_y2[3];
			}

			// log y
			if (axis_scale_y2[4] == 10.0)
			{
				sheet->m_Page1.log_y2 = true;
			}
			else
			{
				sheet->m_Page1.log_y2 = false;
			}

			// Line 5
			// -initial_solutions
			sheet->m_Page1.m_initial_solutions   = o->Get_graph_initial_solutions();

			// Line 6
			// -connect_simulations
			sheet->m_Page1.m_connect_simulations = o->Get_connect_simulations();;

			// Line 7
			// -plot_concentration_vs (x or t) (0 or 1)
			sheet->m_Page1.m_chart_type = (CUserGraphPg1::ChartType)o->Get_chart_type();

			// Line 8
			// -plot_csv_file  filename
			sheet->m_pPage2->csv_file_names = o->Get_csv_file_names();

			// Line 10
			// BASIC
			CRate rate(o->Get_rate_command_list_original());
			sheet->m_pPage2->m_listCommands.assign(rate.m_listCommands.begin(), rate.m_listCommands.end());

			// Line 12
			// -detach
			sheet->m_Page1.m_detach = o->Get_detach();

			// Line 13
			// -active
			sheet->m_Page1.m_active = o->Get_active();;

		}
	}
#else
	UNUSED_ALWAYS(sheet);
#endif
}

int PhreeqcI::do_status(void)
{
	try
	{
		if (pr.status == TRUE)
		{
			state = -1;
			status(0, "\r\nDone.");
			Phreeqc::screen_msg("\n");
		}
		pr.headings = TRUE;
		clock_t finish = clock();
		
		double duration = (double)(finish - start) / CLOCKS_PER_SEC;

		dup_print(sformatf("End of Run after %g Seconds.", duration), TRUE);
		Phreeqc::screen_msg(sformatf("\nEnd of Run after %g Seconds.\n", duration));
	}
	catch (PhreeqcStop e)
	{
		return get_input_errors();
	}
	return 0;
}

void PhreeqcI::GetData(CReaction_Pressure* sheet)const
{
	ASSERT(std::numeric_limits<double>::has_signaling_NaN == true);
	ASSERT(this->Rxn_pressure_map.size() == 1);

	std::map<int, cxxPressure>::const_iterator ci = this->Rxn_pressure_map.begin();
	if (ci != this->Rxn_pressure_map.end())
	{
		// reaction number
		sheet->m_n_user     = (*ci).second.Get_n_user();
		sheet->m_n_user_end = (*ci).second.Get_n_user_end();
		sheet->m_strDesc    = (*ci).second.Get_description().c_str();

		if ((*ci).second.Get_equalIncrements())
		{
			sheet->m_Page1.m_nType        = CReaction_PressurePg1::TYPE_LINEAR;
			sheet->m_Page1.m_nLinearSteps = (*ci).second.Get_count();
			sheet->m_Page1.m_dPress1      = (*ci).second.Get_pressures()[0];
			sheet->m_Page1.m_dPress2      = (*ci).second.Get_pressures()[1];
		}
		else
		{
			// explicit
			sheet->m_Page1.m_nType = CReaction_PressurePg1::TYPE_LIST;
			for (int i = 0; i < (*ci).second.Get_count(); ++i)
			{
				sheet->m_Page1.m_listSteps.push_back((*ci).second.Get_pressures()[i]);
			}
		}
	}
}

void PhreeqcI::GetData(CDelete* sheet)const
{
	sheet->m_Page1.bAll        = CDeletePg1::GetAll(this->delete_info);
	sheet->m_Page1.delete_info = this->delete_info;
}

void PhreeqcI::GetData(CRunCells* sheet)const
{
	sheet->m_Page1.Runner = this->run_info;
}

void PhreeqcI::GetData(CCalculateValues* sheet)const
{
	// empty keyword will assert
	// ASSERT(count_calculate_value >= 1);
	for (int i = 0; i < this->count_calculate_value; ++i)
	{
		CCalcValue cv(this->calculate_value[i]);
		sheet->m_Page1.m_listCalcValues.push_back(cv);
	}
}

void PhreeqcI::GetData(CDump* sheet)const
{
	sheet->m_Page1.bAll      = CDumpPg1::GetAll(this->dump_info.Get_StorageBinList());
	sheet->m_Page1.dump_info = this->dump_info;
}
