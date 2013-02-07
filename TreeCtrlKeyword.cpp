// TreeCtrlKeyword.cpp : implementation file
//

#include "stdafx.h"
#include "phreeqci2.h"
#include "TreeCtrlKeyword.h"

#include <shlwapi.h>

#include "SaveCurrentDirectory.h"
#include "Util.h"

#include "RichEditLineParser.h"
#include "RichLogicalLineParser.h"
#include "RichKeywordParser.h"
#include "CStringLineParser.h"

#include "RichDocIn.h"

// CTreeCtrlKeyword

IMPLEMENT_DYNAMIC(CTreeCtrlKeyword, baseCTreeCtrlKeyword)

CTreeCtrlKeyword::CTreeCtrlKeyword()
{

}

CTreeCtrlKeyword::~CTreeCtrlKeyword()
{
}


BEGIN_MESSAGE_MAP(CTreeCtrlKeyword, baseCTreeCtrlKeyword)
END_MESSAGE_MAP()



// CTreeCtrlKeyword message handlers

CTreeCtrlNode CTreeCtrlKeyword::ParseTree(CRichEditDoc *pDoc, int nErrors /*= 0*/)
{
	CSaveCurrentDirectory scd(pDoc->GetPathName());

	// set hourglass
	CWaitCursor wait;

	// update status bar
	CString strStatus;
	strStatus.Format(_T("Loading %s"), pDoc->GetTitle());
	CUtil::GetStatusBar().SetWindowText(strStatus);

	// check validity
	ASSERT(pDoc != NULL);
	ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(CRichEditDoc)));
	ASSERT(pDoc->GetView() != NULL);
	ASSERT(pDoc->GetView()->IsKindOf(RUNTIME_CLASS(CRichEditView)));
	ASSERT(::IsWindow(pDoc->GetView()->m_hWnd));

	// create and set file node
	CTreeCtrlNode fileNode = GetFileNode(pDoc);
	if (fileNode.HasChildren())
	{
		CTreeCtrlNode child = fileNode.GetChild();
		for (; child != NULL; child = fileNode.GetChild())
		{
			VERIFY(child.Delete());
		}
		ASSERT(!fileNode.HasChildren());
	}
	ASSERT((CRichEditDoc*)fileNode.GetData() == pDoc);

	if (nErrors == 0)
	{
		RemoveDatabaseKeyword(pDoc);
	}

	// create keyword parser
	CRichEditLineParser lineParser(pDoc->GetView()->m_hWnd);
	CRichLogicalLineParser logicalLineParser(lineParser);
	CRichKeywordParser keyParser(logicalLineParser);


	CString str;
	int nSim = 0;

	// add first sim node
	str.Format(IDS_SIM_FORMAT_135, ++nSim);
	CTreeCtrlNode simNode = fileNode.AddTail(str, simImage);

	if (nErrors != 0)
	{
		// file contains errors do not load
		fileNode.SetState(INDEXTOOVERLAYMASK(ErrorState), TVIS_OVERLAYMASK);
		fileNode.Expand(TVE_COLLAPSE);
		return fileNode;
	}
	//{{
	// This fixes a bug which occurred when opening an existing file
	// containing an error; fixing it and expanding the file node.
	// This caused a cascading event which, for some reason, doesn't
	// occur when the file has not been saved or when the file was 
	// opened successfully once
	else
	{
		fileNode.SetState(INDEXTOOVERLAYMASK(SyncState), TVIS_OVERLAYMASK);
	}
	//}}

	CTreeCtrlNode keyNode;

	while (keyParser.GetLine() != CKeywordParser::typeEof)
	{
		switch (keyParser.type)
		{
		case CKeywordParser::typeKeyword :
			ASSERT( !keyParser.tree_line.IsEmpty() );
			keyNode = simNode.AddTail(keyParser.tree_line, GetImageIndex(keyParser.last_keyword));
			if (keyParser.last_keyword == CKeyword::K_END)
			{
				str.Format(IDS_SIM_FORMAT_135, ++nSim);
				simNode = fileNode.AddTail(str, simImage);
				break;
			}
			//{{
			// break on single line keywords
			if (keyParser.last_keyword == CKeyword::K_SAVE) break;
			if (keyParser.last_keyword == CKeyword::K_USE) break;
			if (keyParser.last_keyword == CKeyword::K_INCREMENTAL_REACTIONS) break;
			//}}

#ifdef SUB_PARSE_SOLUTION_SPECIES
			ASSERT( FALSE ); // Need to fix OnDebugSelect
			if (keyParser.last_keyword == CKeyword::K_SOLUTION_SPECIES)
			{
				while (keyParser.GetLine() != CKeywordParser::typeEof)
				{
					if (keyParser.type == CKeywordParser::typeKeyword)
					{
						keyParser.PushBack();
						break;
					}
					if (keyParser.type != CKeywordParser::typeEmpty)
					{
						ASSERT( keyParser.type == CKeywordParser::typeOk || keyParser.type == CKeywordParser::typeOption);
						CTreeCtrlNode optionNode = keyNode.AddTail(keyParser.tree_line, CTreeCtrlIn::optionImage);
						if (!CKeywordParser::IsSolutionSpeciesOption(keyParser.line))
						{
							while (keyParser.GetLine() != CKeywordParser::typeEof)
							{
								if (keyParser.type == CKeywordParser::typeKeyword)
								{
									break;
								}
								if (keyParser.type != CKeywordParser::typeEmpty)
								{
									if (CKeywordParser::IsSolutionSpeciesOption(keyParser.line))
									{
										optionNode.AddTail(keyParser.tree_line, CTreeCtrlIn::optionImage);
									}
									else
									{
										break;
									}
								}
							}
							keyParser.PushBack();
						}
					}
				}
				break;
			}
#endif // SUB_PARSE_SOLUTION_SPECIES

			while (keyParser.GetLine() != CKeywordParser::typeEof)
			{
				if (keyParser.type == CKeywordParser::typeKeyword)
				{
					keyParser.PushBack();
					break;
				}
				if (keyParser.type != CKeywordParser::typeEmpty)
				{
					keyNode.AddTail(keyParser.tree_line, optionImage);
				}
			}
			break;

		case CKeywordParser::typeEmpty :
			break;

		default :
			ASSERT(!keyParser.tree_line.IsEmpty());
			simNode.AddTail(keyParser.tree_line, optionImage);
			break;
		}
	}

// SEE CTreeCtrlIn::UpdateAllViews
// COMMENT: {6/9/2000 5:16:48 PM}	fileNode.Select();

	fileNode.Expand();
	fileNode.EnsureVisible();

	// reset status bar to idle message
	::AfxGetMainWnd()->PostMessage(WM_SETMESSAGESTRING, AFX_IDS_IDLEMESSAGE);

	return fileNode;
}

/**
Removes the DATABASE keyword line from the input file and sets
the database appropriately if it exists.

  Assumes that the DATABASE keyword must be the first keyword
  in the file.

  Assumes that the file is error free.
*/
void CTreeCtrlKeyword::RemoveDatabaseKeyword(CRichEditDoc *pDoc)
{
	CRichEditLineParser lineParser(pDoc->GetView()->m_hWnd);
	CRichLogicalLineParser logicalLineParser(lineParser);
	CRichKeywordParser keyParser(logicalLineParser);

	// skip non-keyword lines
	while (keyParser.GetLine() != CKeywordParser::typeKeyword)
	{
		if (keyParser.type == CKeywordParser::typeEof)
		{
			break;
		}
	}
	if (keyParser.last_keyword == CKeyword::K_DATABASE)
	{
		BOOL bModified = pDoc->IsModified();

		CString strDBPathName = keyParser.line.Mid(8);
		strDBPathName.TrimLeft();
		strDBPathName.TrimRight();

		// convert unix paths to dos paths
		strDBPathName.Replace(_T('/'), _T('\\'));

		if (::PathIsRelative(strDBPathName))
		{
			// convert strDBPathName to absolute path
			TCHAR szPath[MAX_PATH];
			TCHAR szOut[MAX_PATH];
			VERIFY(::GetCurrentDirectory(MAX_PATH, szPath));
			VERIFY(::PathAddBackslash(szPath));
			VERIFY(::PathAppend(szPath, strDBPathName));
			VERIFY(::PathCanonicalize(szOut, szPath));
			strDBPathName = szOut;
		}

#ifdef SAVE_EXPAND_ENVIR
		TCHAR infoBuf[32767];
		DWORD bufCharCount = 32767;
		bufCharCount = ExpandEnvironmentStrings(strDBPathName, infoBuf, 32767); 
		strDBPathName = infoBuf;
#endif
		// check if database exists
		CPhreeqciApp* pApp = (CPhreeqciApp*)AfxGetApp();
		if (!CUtil::FileExists(strDBPathName))
		{
			ASSERT(CUtil::FileExists(pApp->m_settings.m_strDefDBPathName));
			CString warning;
			AfxFormatString2(warning, IDS_MISSING_DB_171, strDBPathName, pApp->m_settings.m_strDefDBPathName);

			// Note: MessageBox is now modal.  This fixes
			// a bug that occured if user attempted to 
			// exit program without responding to the
			// MessageBox,
			//
			// If opening more than one file this will not
			// allow any remaining files to be opened
			HWND hWndTop;
			HWND hWnd = CWnd::GetSafeOwner_(NULL, &hWndTop);
			::MessageBox(hWnd, warning, _T("Missing database"), MB_OK | MB_ICONWARNING);
			bModified = TRUE;

			strDBPathName = pApp->m_settings.m_strDefDBPathName;
		}
		else
		{
			try
			{
				pApp->PreLoadDatabase(strDBPathName);
			}
			catch (CException* p)
			{
				// TODO
				throw p;
			}
		}

		ASSERT_KINDOF(CRichDocIn, pDoc);
		((CRichDocIn*)pDoc)->m_props.m_strDBPathName = strDBPathName;

		CHARRANGE cr = keyParser.cr;
		pDoc->GetView()->GetRichEditCtrl().SetSel(cr);
		pDoc->GetView()->GetRichEditCtrl().Clear();
		if (!bModified)
		{
			pDoc->GetView()->GetRichEditCtrl().EmptyUndoBuffer();
			pDoc->SetModifiedFlag(FALSE);
		}
	}

	// open database
	if (CPhreeqciApp *app = dynamic_cast<CPhreeqciApp*>(::AfxGetApp()))
	{
		if (CRichDocIn *in = dynamic_cast<CRichDocIn*>(pDoc))
		{
			app->OpenAssocDB(in);
		}
	}
}

// static
//
//
enum CTreeCtrlIn::ImageIndex CTreeCtrlKeyword::GetImageIndex(enum CKeyword::type nType)
{
	ImageIndex index = genericKeyImage;

	switch (nType)
	{
	case CKeyword::K_END :
		index = endImage;
		break;

	case CKeyword::K_SOLUTION_SPECIES :
		index = solution_speciesImage;
		break;

	case CKeyword::K_SOLUTION_MASTER_SPECIES :
		index = solution_master_speciesImage;
		break;

	case CKeyword::K_SOLUTION :
		index = solutionImage;
		break;

	case CKeyword::K_PHASES :
		index = phasesImage;
		break;

	case CKeyword::K_REACTION :
		index = reactionImage;
		break;

	case CKeyword::K_MIX :
		index = mixImage;
		break;

	case CKeyword::K_USE :
		index = useImage;
		break;

	case CKeyword::K_SAVE :
		index = saveImage;
		break;

	case CKeyword::K_EXCHANGE_SPECIES :
		index = exchange_speciesImage;
		break;

	case CKeyword::K_EXCHANGE_MASTER_SPECIES :
		index = exchange_master_speciesImage;
		break;

	case CKeyword::K_EXCHANGE :
		index = exchangeImage;
		break;

	case CKeyword::K_SURFACE_SPECIES :
		index = surface_speciesImage;
		break;

	case CKeyword::K_SURFACE_MASTER_SPECIES :
		index = surface_master_speciesImage;
		break;

	case CKeyword::K_SURFACE :
		index = surfaceImage;
		break;

	case CKeyword::K_REACTION_TEMPERATURE :
		index = reaction_temperatureImage;
		break;

	case CKeyword::K_INVERSE_MODELING :
		index = inverse_modelingImage;
		break;

	case CKeyword::K_GAS_PHASE :
		index = gas_phaseImage;
		break;

	case CKeyword::K_TRANSPORT :
		index = transportImage;
		break;

	case CKeyword::K_SELECTED_OUTPUT :
		index = selected_outputImage;
		break;

	case CKeyword::K_KNOBS :
		index = knobsImage;
		break;

	case CKeyword::K_PRINT :
		index = printImage;
		break;

	case CKeyword::K_EQUILIBRIUM_PHASES :
		index = equilibrium_phasesImage;
		break;

	case CKeyword::K_TITLE :
		index = titleImage;
		break;

	//
	// New keywords added by version 2
	//

	case CKeyword::K_ADVECTION :
		index = advectionImage;
		break;

	case CKeyword::K_KINETICS :
		index = kineticsImage;
		break;

	case CKeyword::K_INCREMENTAL_REACTIONS :
		index = incremental_reactionsImage;
		break;

	case CKeyword::K_RATES :
		index = ratesImage;
		break;

	case CKeyword::K_USER_PRINT :
		index = user_printImage;
		break;

	case CKeyword::K_USER_PUNCH :
		index = user_punchImage;
		break;

	case CKeyword::K_SOLID_SOLUTIONS :
		index = solid_solutionsImage;
		break;

	case CKeyword::K_SOLUTION_SPREAD :
		index = solution_spreadImage;
		break;

	case CKeyword::K_LLNL_AQUEOUS_MODEL_PARAMETERS :
		index = llnl_aqueous_model_parametersImage;
		break;

	case CKeyword::K_NAMED_EXPRESSIONS :
		index = genericKeyImage;
		break;

	case CKeyword::K_ISOTOPES :
		index = genericKeyImage;
		break;

	case CKeyword::K_CALCULATE_VALUES :
		index = genericKeyImage;
		break;

	case CKeyword::K_ISOTOPE_RATIOS :
		index = genericKeyImage;
		break;

	case CKeyword::K_ISOTOPE_ALPHAS :
		index = genericKeyImage;
		break;

	case CKeyword::K_COPY :
		index = copyImage;
		break;

	case CKeyword::K_PITZER :
		index = pitzerImage;
		break;

	case CKeyword::K_SIT :
		index = sitImage;
		break;

	//
	// phreeqc3
	//

	case CKeyword::K_SOLUTION_RAW :
		index = genericKeyImage;
		break;

	case CKeyword::K_EXCHANGE_RAW :
		index = genericKeyImage;
		break;

	case CKeyword::K_SURFACE_RAW :
		index = genericKeyImage;
		break;

	case CKeyword::K_EQUILIBRIUM_PHASES_RAW :
		index = genericKeyImage;
		break;

	case CKeyword::K_KINETICS_RAW :
		index = genericKeyImage;
		break;

	case CKeyword::K_SOLID_SOLUTIONS_RAW :
		index = genericKeyImage;
		break;

	case CKeyword::K_GAS_PHASE_RAW :
		index = genericKeyImage;
		break;

	case CKeyword::K_REACTION_RAW :
		index = genericKeyImage;
		break;

	case CKeyword::K_MIX_RAW :
		index = genericKeyImage;
		break;

	case CKeyword::K_REACTION_TEMPERATURE_RAW :
		index = genericKeyImage;
		break;

	case CKeyword::K_DUMP :
		index = genericKeyImage;
		break;

	case CKeyword::K_SOLUTION_MODIFY :
		index = genericKeyImage;
		break;

	case CKeyword::K_EQUILIBRIUM_PHASES_MODIFY :
		index = genericKeyImage;
		break;

	case CKeyword::K_EXCHANGE_MODIFY :
		index = genericKeyImage;
		break;

	case CKeyword::K_SURFACE_MODIFY :
		index = genericKeyImage;
		break;

	case CKeyword::K_SOLID_SOLUTIONS_MODIFY :
		index = genericKeyImage;
		break;

	case CKeyword::K_GAS_PHASE_MODIFY :
		index = genericKeyImage;
		break;

	case CKeyword::K_KINETICS_MODIFY :
		index = genericKeyImage;
		break;

	case CKeyword::K_DELETE :
		index = deleteImage;
		break;

	case CKeyword::K_RUN_CELLS :
		index = genericKeyImage;
		break;

	case CKeyword::K_REACTION_MODIFY :
		index = genericKeyImage;
		break;

	case CKeyword::K_REACTION_TEMPERATURE_MODIFY :
		index = genericKeyImage;
		break;

	case CKeyword::K_REACTION_PRESSURE :
		index = reaction_pressureImage;
		break;

	case CKeyword::K_REACTION_PRESSURE_RAW :
		index = genericKeyImage;
		break;

	case CKeyword::K_REACTION_PRESSURE_MODIFY :
		index = genericKeyImage;
		break;

	case CKeyword::K_USER_GRAPH :
		index = user_graphImage;
		break;

	case CKeyword::K_INCLUDE :
		index = includeImage;
		break;

	//{{NEW KEYWORD HERE}}

	default :
		ASSERT(FALSE);	// undefined keyword
		break;
	}

	//// ASSERT(index != genericKeyImage);

	return index;
}

// static
//
//
enum CKeyword::type CTreeCtrlKeyword::GetKeywordType(enum CTreeCtrlIn::ImageIndex nImageID)
{
	CKeyword::type nType = CKeyword::K_NOT_KEYWORD;

	switch (nImageID)
	{
	case endImage :
		nType = CKeyword::K_END;
		break;

	case solution_speciesImage :
		nType = CKeyword::K_SOLUTION_SPECIES;
		break;

	case solution_master_speciesImage :
		nType = CKeyword::K_SOLUTION_MASTER_SPECIES;
		break;

	case solutionImage :
		nType = CKeyword::K_SOLUTION;
		break;

	case phasesImage :
		nType = CKeyword::K_PHASES;
		break;

	case reactionImage :
		nType = CKeyword::K_REACTION;
		break;

	case mixImage :
		nType = CKeyword::K_MIX;
		break;

	case useImage :
		nType = CKeyword::K_USE;
		break;

	case saveImage :
		nType = CKeyword::K_SAVE;
		break;

	case exchange_speciesImage :
		nType = CKeyword::K_EXCHANGE_SPECIES;
		break;

	case exchange_master_speciesImage :
		nType = CKeyword::K_EXCHANGE_MASTER_SPECIES;
		break;

	case exchangeImage :
		nType = CKeyword::K_EXCHANGE;
		break;

	case surface_speciesImage :
		nType = CKeyword::K_SURFACE_SPECIES;
		break;

	case surface_master_speciesImage :
		nType = CKeyword::K_SURFACE_MASTER_SPECIES;
		break;

	case surfaceImage :
		nType = CKeyword::K_SURFACE;
		break;

	case reaction_temperatureImage :
		nType = CKeyword::K_REACTION_TEMPERATURE;
		break;

	case inverse_modelingImage :
		nType = CKeyword::K_INVERSE_MODELING;
		break;

	case gas_phaseImage :
		nType = CKeyword::K_GAS_PHASE;
		break;

	case transportImage :
		nType = CKeyword::K_TRANSPORT;
		break;

	case selected_outputImage :
		nType = CKeyword::K_SELECTED_OUTPUT;
		break;

	case knobsImage :
		nType = CKeyword::K_KNOBS;
		break;

	case printImage :
		nType = CKeyword::K_PRINT;
		break;

	case equilibrium_phasesImage :
		nType = CKeyword::K_EQUILIBRIUM_PHASES;
		break;

	case titleImage :
		nType = CKeyword::K_TITLE;
		break;

	//
	// New keywords added by version 2
	//

	case advectionImage :
		nType = CKeyword::K_ADVECTION;
		break;

	case kineticsImage :
		nType = CKeyword::K_KINETICS;
		break;

	case incremental_reactionsImage :
		nType = CKeyword::K_INCREMENTAL_REACTIONS;
		break;

	case ratesImage :
		nType = CKeyword::K_RATES;
		break;

	case user_printImage :
		nType = CKeyword::K_USER_PRINT;
		break;

	case user_punchImage :
		nType = CKeyword::K_USER_PUNCH;
		break;

	case solid_solutionsImage :
		nType = CKeyword::K_SOLID_SOLUTIONS;
		break;

	case solution_spreadImage :
		nType = CKeyword::K_SOLUTION_SPREAD;
		break;

	case llnl_aqueous_model_parametersImage :
		nType = CKeyword::K_LLNL_AQUEOUS_MODEL_PARAMETERS;
		break;

	case copyImage :
		nType = CKeyword::K_COPY;
		break;

	case pitzerImage :
		nType = CKeyword::K_PITZER;
		break;

	case sitImage :
		nType = CKeyword::K_SIT;
		break;

	case reaction_pressureImage :
		nType = CKeyword::K_REACTION_PRESSURE;
		break;

	//
	// New keywords added by version 3
	//

	case deleteImage :
		nType = CKeyword::K_DELETE;
		break;

	case user_graphImage :
		nType = CKeyword::K_USER_GRAPH;
		break;

	case includeImage :
		nType = CKeyword::K_INCLUDE;
		break;

	//{{NEW KEYWORD HERE}}

	default :
		ASSERT(FALSE);
		break;
	}

	ASSERT(nType != CKeyword::K_NOT_KEYWORD);
	return nType;
}

//
// returns the file node associated with pDoc
// if pDoc is not associated with any file nodes a file node
// is created
//
CTreeCtrlNode CTreeCtrlKeyword::GetFileNode(CRichEditDoc *pDoc)
{
	ASSERT_KINDOF(CRichEditDoc, pDoc);
	CTreeCtrlNode fileNode;
	bool bFound = false;
	for (fileNode = this->GetRootItem().GetChild(); fileNode != NULL; fileNode = fileNode.GetNextSibling())
	{
		if (pDoc == (CRichEditDoc*)fileNode.GetData())
		{
			bFound = true;
			break;
		}
	}
	if (!bFound)
	{
		// node not found so add it
		fileNode = InsertItem(pDoc->GetTitle(), fileImage, fileImage, this->GetRootItem());
		fileNode.SetData((DWORD)pDoc);
		ASSERT((CRichEditDoc*)fileNode.GetData() == pDoc);
	}
	ASSERT(fileNode && fileNode.GetLevel() == FileLevel);
	return fileNode;
}

//
// returns the file node associated with childNode
//
CTreeCtrlNode CTreeCtrlKeyword::GetFileNode(const CTreeCtrlNode& childNode)const
{
	CTreeCtrlNode fileNode(childNode);

	if (fileNode == 0) return fileNode;

	if (fileNode.GetLevel() == WorkLevel)
	{
		return CTreeCtrlNode();
	}

	while (fileNode.GetLevel() > FileLevel)
	{
		fileNode = fileNode.GetParent();
	}

	ASSERT(fileNode && fileNode.GetLevel() == FileLevel);
	return fileNode;
}

