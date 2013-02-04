#pragma once


// CTreeCtrlKeyword
#include "Tree.h"

#include "Keyword.h"

#ifndef baseCTreeCtrlKeyword
#define baseCTreeCtrlKeyword CTreeCtrlEx
#endif

class CTreeCtrlKeyword : public baseCTreeCtrlKeyword
{
	DECLARE_DYNAMIC(CTreeCtrlKeyword)

public:
	CTreeCtrlKeyword();
	virtual ~CTreeCtrlKeyword();

public:
	enum ImageIndex
	{
		simImage,
		unusedImage,
		workImageIn,
		workImageDB,
		fileImage,
		endImage,                     // K_END
		solutionImage,                // K_SOLUTION
		exchangeImage,                // K_EXCHANGE
		gas_phaseImage,               // K_GAS_PHASE
		equilibrium_phasesImage,      // K_EQUILIBRIUM_PHASES
		solid_solutionsImage,         // K_SOLID_SOLUTIONS
		surfaceImage,                 // K_SURFACE
		useImage,                     // K_USE
		mixImage,                     // K_MIX
		reactionImage,                // K_REACTION
		reaction_temperatureImage,    // K_REACTION_TEMPERATURE
		saveImage,                    // K_SAVE
		titleImage,                   // K_TITLE
		genericKeyImage,
		optionImage,
		UnSyncOverlayImage,
		ErrorOverlayImage,
		solution_speciesImage,        // K_SOLUTION_SPECIES
		solution_master_speciesImage, // K_SOLUTION_MASTER_SPECIES
		phasesImage,                  // K_PHASES
		exchange_speciesImage,        // K_EXCHANGE_SPECIES
		exchange_master_speciesImage, // K_EXCHANGE_MASTER_SPECIES
		surface_speciesImage,         // K_SURFACE_SPECIES
		surface_master_speciesImage,  // K_SURFACE_MASTER_SPECIES
		inverse_modelingImage,        // K_INVERSE_MODELING
		transportImage,               // K_TRANSPORT
		selected_outputImage,         // K_SELECTED_OUTPUT
		knobsImage,                   // K_KNOBS
		printImage,                   // K_PRINT
		advectionImage,               // K_ADVECTION
		kineticsImage,                // K_KINETICS
		incremental_reactionsImage,   // K_INCREMENTAL_REACTIONS
		ratesImage,                   // K_RATES
		user_printImage,              // K_USER_PRINT
		user_punchImage,              // K_USER_PUNCH
		solution_spreadImage,         // K_SOLUTION_SPREAD
		user_graphImage,              // K_USER_GRAPH
		llnl_aqueous_model_parametersImage,  // K_LLNL_AQUEOUS_MODEL_PARAMETERS
		copyImage,                    // K_COPY
		pitzerImage,                  // K_PITZER
		sitImage,                     // K_SIT
		reaction_pressureImage,       // K_REACTION_PRESSURE
		deleteImage,                  // K_DELETE
		//{{NEW KEYWORD HERE}}
	};
	enum NodeLevel
	{
		WorkLevel          = 0,
		FileLevel          = 1,
		SimLevel           = 2,
		KeywordLevel       = 3,
		KeywordOptionLevel = 4,
	};
	enum OverlayState
	{
		SyncState   = 0,
		UnSyncState = 1,
		ErrorState  = 2,
	};

public:
	CTreeCtrlNode ParseTree(CRichEditDoc *pDoc, int nErrors /*= 0*/);
	CTreeCtrlNode GetFileNode(CRichEditDoc *pDoc);
	CTreeCtrlNode GetFileNode(const CTreeCtrlNode& childNode)const;

	static enum CTreeCtrlKeyword::ImageIndex GetImageIndex(enum CKeyword::type nType);
	static enum CKeyword::type GetKeywordType(enum CTreeCtrlKeyword::ImageIndex nImageID);

protected:
	virtual void RemoveDatabaseKeyword(CRichEditDoc *pDoc);

	DECLARE_MESSAGE_MAP()

public:
	CImageList m_ctrlImage;
};


