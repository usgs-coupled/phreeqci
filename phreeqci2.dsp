# Microsoft Developer Studio Project File - Name="phreeqci2" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=phreeqci2 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "phreeqci2.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "phreeqci2.mak" CFG="phreeqci2 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "phreeqci2 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "phreeqci2 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "phreeqci2"
# PROP Scc_LocalPath "."
CPP=cl.exe
F90=df.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE F90 /compile_only /include:"Release/" /nologo /warn:nofileopt /winapp
# ADD F90 /browser /compile_only /include:"Release/" /nologo /warn:nofileopt /winapp
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W4 /WX /GX /Zi /O2 /I "$(DEV_HTMLHELP_INC)" /I "$(DEV_GMP_INC)" /D "USE_PHRQ_ALLOC" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "PHREEQCI_GUI" /D "INVERSE_CL1MP" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 gmp.lib Htmlhelp.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"LIBC.lib" /out:"Release/phreeqci.exe" /libpath:"$(DEV_HTMLHELP_LIB)" /libpath:"$(DEV_GMP_LIB)" /release
# SUBTRACT LINK32 /verbose /pdb:none

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE F90 /check:bounds /compile_only /debug:full /include:"Debug/" /nologo /traceback /warn:argument_checking /warn:nofileopt /winapp
# ADD F90 /browser /check:bounds /compile_only /debug:full /include:"Debug/" /nologo /traceback /warn:argument_checking /warn:nofileopt /winapp
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /ZI /Od /I "$(DEV_HTMLHELP_INC)" /I "$(DEV_GMP_INC)" /D "WIN32_MEMORY_DEBUG" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "PHREEQCI_GUI" /D "INVERSE_CL1MP" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Htmlhelp.lib gmp.lib /nologo /subsystem:windows /profile /map /debug /machine:I386 /nodefaultlib:"LIBC.lib" /out:"Debug/phreeqci.exe" /libpath:"$(DEV_HTMLHELP_LIB)" /libpath:"$(DEV_GMP_LIB)"

!ENDIF 

# Begin Target

# Name "phreeqci2 - Win32 Release"
# Name "phreeqci2 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;f90;for;f;fpp"
# Begin Source File

SOURCE=.\AccelPropertySheet.cpp
# End Source File
# Begin Source File

SOURCE=.\AllVirtualPropertyPage.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\AllVirtualPropertySheet.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\AllVirtualTreeCtrl.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\BasicDesc.cpp
# End Source File
# Begin Source File

SOURCE=.\BasicDesc2.cpp
# End Source File
# Begin Source File

SOURCE=.\BasicEditGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\BasicObj.cpp
# End Source File
# Begin Source File

SOURCE=.\CEPExchangePg1.cpp
# End Source File
# Begin Source File

SOURCE=.\CheckListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\CKPEquilibrium_PhasesPg1.cpp
# End Source File
# Begin Source File

SOURCE=.\CKPGas_PhasePg1.cpp
# End Source File
# Begin Source File

SOURCE=.\CKPKineticsPg1.cpp
# End Source File
# Begin Source File

SOURCE=.\CKPMixPg1.cpp
# End Source File
# Begin Source File

SOURCE=.\CKPReaction_TemperaturePg1.cpp
# End Source File
# Begin Source File

SOURCE=.\CKPReactionPg1.cpp
# End Source File
# Begin Source File

SOURCE=.\CKPSolid_SolutionsPg1.cpp
# End Source File
# Begin Source File

SOURCE=.\CKSEquilibrium_Phases.cpp
# End Source File
# Begin Source File

SOURCE=.\CKSExchange.cpp
# End Source File
# Begin Source File

SOURCE=.\CKSGas_Phase.cpp
# End Source File
# Begin Source File

SOURCE=.\CKSInverse.cpp
# End Source File
# Begin Source File

SOURCE=.\CKSMix.cpp
# End Source File
# Begin Source File

SOURCE=.\CKSReaction.cpp
# End Source File
# Begin Source File

SOURCE=.\CKSReaction_Temperature.cpp
# End Source File
# Begin Source File

SOURCE=.\CKSSolid_Solutions.cpp
# End Source File
# Begin Source File

SOURCE=.\CKSSolution.cpp
# End Source File
# Begin Source File

SOURCE=.\CKSSurface.cpp
# End Source File
# Begin Source File

SOURCE=.\ClipListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\ClipTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\CommonKeywordPage.cpp
# End Source File
# Begin Source File

SOURCE=.\CommonKeywordSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\CSPSurfacePg1.cpp
# End Source File
# Begin Source File

SOURCE=.\CStringLineParser.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomizePage1.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomizeSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\Database.cpp
# End Source File
# Begin Source File

SOURCE=.\DBObject.cpp
# End Source File
# Begin Source File

SOURCE=.\DDX_DDV.cpp
# End Source File
# Begin Source File

SOURCE=.\DefinedRanges.cpp
# End Source File
# Begin Source File

SOURCE=.\DelayUpdate.cpp
# End Source File
# Begin Source File

SOURCE=.\EditGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\EqnChecker.cpp
# End Source File
# Begin Source File

SOURCE=.\EqnChecker2.cpp
# End Source File
# Begin Source File

SOURCE=.\ErrorChecker2.cpp
# End Source File
# Begin Source File

SOURCE=.\Layout\ETSLayout.cpp

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# ADD CPP /W4
# SUBTRACT CPP /WX

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# ADD CPP /W3

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FixedTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\font.cpp
# End Source File
# Begin Source File

SOURCE=.\GridListDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\InputTreeOleDropTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\Keyword.cpp
# End Source File
# Begin Source File

SOURCE=.\KeywordLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\KeywordLoader2.cpp
# End Source File
# Begin Source File

SOURCE=.\KeywordPage.cpp
# End Source File
# Begin Source File

SOURCE=.\KeywordPageListItems.cpp
# End Source File
# Begin Source File

SOURCE=.\KeywordParser.cpp
# End Source File
# Begin Source File

SOURCE=.\KeywordSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\KPAdvectionPg1.cpp
# End Source File
# Begin Source File

SOURCE=.\KPCopyPg1.cpp
# End Source File
# Begin Source File

SOURCE=.\KPExchangeMasterSpeciesPg1.cpp
# End Source File
# Begin Source File

SOURCE=.\KPExchangeSpeciesPg1.cpp
# End Source File
# Begin Source File

SOURCE=.\KPIncrementPg1.CPP
# End Source File
# Begin Source File

SOURCE=.\KPInversePg1.cpp
# End Source File
# Begin Source File

SOURCE=.\KPKnobsPg1.cpp
# End Source File
# Begin Source File

SOURCE=.\KPPhasesPg1.cpp
# End Source File
# Begin Source File

SOURCE=.\KPPrintPg1.cpp
# End Source File
# Begin Source File

SOURCE=.\KPRatesPg1.cpp
# End Source File
# Begin Source File

SOURCE=.\KPSavePg1.cpp
# End Source File
# Begin Source File

SOURCE=.\KPSelectedOutputPg1.cpp
# End Source File
# Begin Source File

SOURCE=.\KPSelectedOutputPg8.cpp
# End Source File
# Begin Source File

SOURCE=.\KPSelectedOutputPg9.cpp
# End Source File
# Begin Source File

SOURCE=.\KPSolution_SpreadPg1.cpp
# End Source File
# Begin Source File

SOURCE=.\KPSolutionMasterSpeciesPg1.cpp
# End Source File
# Begin Source File

SOURCE=.\KPSolutionPg1.cpp
# End Source File
# Begin Source File

SOURCE=.\KPSolutionSpeciesPg1.cpp
# End Source File
# Begin Source File

SOURCE=.\KPSurfaceMasterSpeciesPg1.cpp
# End Source File
# Begin Source File

SOURCE=.\KPSurfaceSpeciesPg1.cpp
# End Source File
# Begin Source File

SOURCE=.\KPTitlePg1.cpp
# End Source File
# Begin Source File

SOURCE=.\KPTransportPg1.cpp
# End Source File
# Begin Source File

SOURCE=.\KPUsePg1.cpp
# End Source File
# Begin Source File

SOURCE=.\KPUserPrintPg1.cpp
# End Source File
# Begin Source File

SOURCE=.\KPUserPunchPg1.cpp
# End Source File
# Begin Source File

SOURCE=.\KSExchangeMasterSpecies.cpp
# End Source File
# Begin Source File

SOURCE=.\KSExchangeSpecies.cpp
# End Source File
# Begin Source File

SOURCE=.\KSIncrement.cpp
# End Source File
# Begin Source File

SOURCE=.\KSKinetics.cpp
# End Source File
# Begin Source File

SOURCE=.\KSKnobs.cpp
# End Source File
# Begin Source File

SOURCE=.\KSPhases.cpp
# End Source File
# Begin Source File

SOURCE=.\KSPrint.cpp
# End Source File
# Begin Source File

SOURCE=.\KSSelectedOutput.cpp
# End Source File
# Begin Source File

SOURCE=.\KSSolutionMasterSpecies.cpp
# End Source File
# Begin Source File

SOURCE=.\KSSolutionSpecies.cpp
# End Source File
# Begin Source File

SOURCE=.\KSSurfaceMasterSpecies.cpp
# End Source File
# Begin Source File

SOURCE=.\KSSurfaceSpecies.cpp
# End Source File
# Begin Source File

SOURCE=.\KSTitle.cpp
# End Source File
# Begin Source File

SOURCE=.\LineParser.cpp
# End Source File
# Begin Source File

SOURCE=.\LocalException.cpp
# End Source File
# Begin Source File

SOURCE=.\LogicalLineParser.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\mshflexgrid.cpp
# End Source File
# Begin Source File

SOURCE=.\MultiDocTemplateIn.cpp
# End Source File
# Begin Source File

SOURCE=.\MultiDocTemplateOut.cpp
# End Source File
# Begin Source File

SOURCE=.\MyPropertyPage1.cpp
# End Source File
# Begin Source File

SOURCE=.\MyPropertySheet.cpp
# End Source File
# Begin Source File

SOURCE=.\MyRecentFileList.cpp
# End Source File
# Begin Source File

SOURCE=.\NodeObject.cpp
# End Source File
# Begin Source File

SOURCE=.\OCKSAdvection.cpp
# End Source File
# Begin Source File

SOURCE=.\OCKSCopy.cpp
# End Source File
# Begin Source File

SOURCE=.\OCKSRates.cpp
# End Source File
# Begin Source File

SOURCE=.\OCKSSave.cpp
# End Source File
# Begin Source File

SOURCE=.\OCKSSolution_Spread.cpp
# End Source File
# Begin Source File

SOURCE=.\OCKSTransport.cpp
# End Source File
# Begin Source File

SOURCE=.\OCKSUse.cpp
# End Source File
# Begin Source File

SOURCE=.\OCKSUserPrint.cpp
# End Source File
# Begin Source File

SOURCE=.\OCKSUserPunch.cpp
# End Source File
# Begin Source File

SOURCE=.\OleCtlKeywordSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\PhreeqcCallback.cpp
# End Source File
# Begin Source File

SOURCE=.\phreeqci2.cpp
# End Source File
# Begin Source File

SOURCE=.\phreeqci2.rc
# End Source File
# Begin Source File

SOURCE=.\picture.cpp
# End Source File
# Begin Source File

SOURCE=.\recordset.cpp
# End Source File
# Begin Source File

SOURCE=.\RichDocIn.cpp
# End Source File
# Begin Source File

SOURCE=.\RichDocInPages.cpp
# End Source File
# Begin Source File

SOURCE=.\RichDocInSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\RichDocOut.cpp
# End Source File
# Begin Source File

SOURCE=.\RichEditLineParser.cpp
# End Source File
# Begin Source File

SOURCE=.\RichEditLineParserSafe.cpp
# End Source File
# Begin Source File

SOURCE=.\RichInCntrItem.cpp
# End Source File
# Begin Source File

SOURCE=.\RichKeywordParser.cpp
# End Source File
# Begin Source File

SOURCE=.\RichLogicalLineParser.cpp
# End Source File
# Begin Source File

SOURCE=.\RichOutCntrItem.cpp
# End Source File
# Begin Source File

SOURCE=.\RichViewIn.cpp
# End Source File
# Begin Source File

SOURCE=.\RichViewOut.cpp
# End Source File
# Begin Source File

SOURCE=.\RunDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectElementsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Posea\sizecbar.cpp

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# ADD CPP /W4
# SUBTRACT CPP /WX

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# ADD CPP /W3

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Splash.cpp
# End Source File
# Begin Source File

SOURCE=.\srcdbpg.cpp
# End Source File
# Begin Source File

SOURCE=.\SRCDBPGCH.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StdioFileLineParser.cpp
# End Source File
# Begin Source File

SOURCE=.\TipDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TreeCtrlErr.cpp
# End Source File
# Begin Source File

SOURCE=.\TreeCtrlEx.cpp
# End Source File
# Begin Source File

SOURCE=.\TreeCtrlIn.cpp
# End Source File
# Begin Source File

SOURCE=.\TreeCtrlNode.cpp
# End Source File
# Begin Source File

SOURCE=.\TreeCtrlOut.cpp
# End Source File
# Begin Source File

SOURCE=.\UpdateObject.cpp
# End Source File
# Begin Source File

SOURCE=.\Util.cpp
# End Source File
# Begin Source File

SOURCE=.\WorkspaceBar.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\AccelPropertySheet.h
# End Source File
# Begin Source File

SOURCE=.\AllVirtualPropertyPage.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\AllVirtualPropertySheet.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\AllVirtualTreeCtrl.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\BasicDesc.h
# End Source File
# Begin Source File

SOURCE=.\BasicDesc2.h
# End Source File
# Begin Source File

SOURCE=.\BasicEditGrid.h
# End Source File
# Begin Source File

SOURCE=.\BasicObj.h
# End Source File
# Begin Source File

SOURCE=.\CEPExchangePg1.h
# End Source File
# Begin Source File

SOURCE=.\CheckListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrame.h
# End Source File
# Begin Source File

SOURCE=.\CKPEquilibrium_PhasesPg1.h
# End Source File
# Begin Source File

SOURCE=.\CKPGas_PhasePg1.h
# End Source File
# Begin Source File

SOURCE=.\CKPKineticsPg1.h
# End Source File
# Begin Source File

SOURCE=.\CKPMixPg1.h
# End Source File
# Begin Source File

SOURCE=.\CKPReaction_TemperaturePg1.h
# End Source File
# Begin Source File

SOURCE=.\CKPReactionPg1.h
# End Source File
# Begin Source File

SOURCE=.\CKPSolid_SolutionsPg1.h
# End Source File
# Begin Source File

SOURCE=.\CKSEquilibrium_Phases.h
# End Source File
# Begin Source File

SOURCE=.\CKSExchange.h
# End Source File
# Begin Source File

SOURCE=.\CKSGas_Phase.h
# End Source File
# Begin Source File

SOURCE=.\CKSInverse.h
# End Source File
# Begin Source File

SOURCE=.\CKSMix.h
# End Source File
# Begin Source File

SOURCE=.\CKSReaction.h
# End Source File
# Begin Source File

SOURCE=.\CKSReaction_Temperature.h
# End Source File
# Begin Source File

SOURCE=.\CKSSolid_Solutions.h
# End Source File
# Begin Source File

SOURCE=.\CKSSolution.h
# End Source File
# Begin Source File

SOURCE=.\CKSSurface.h
# End Source File
# Begin Source File

SOURCE=.\ClipListBox.h
# End Source File
# Begin Source File

SOURCE=.\ClipTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\CommonKeywordPage.h
# End Source File
# Begin Source File

SOURCE=.\CommonKeywordSheet.h
# End Source File
# Begin Source File

SOURCE=.\CSPSurfacePg1.h
# End Source File
# Begin Source File

SOURCE=.\CStringLineParser.h
# End Source File
# Begin Source File

SOURCE=.\CustomizePage1.h
# End Source File
# Begin Source File

SOURCE=.\CustomizeSheet.h
# End Source File
# Begin Source File

SOURCE=.\Database.h
# End Source File
# Begin Source File

SOURCE=.\DBObject.h
# End Source File
# Begin Source File

SOURCE=.\DDX_DDV.h
# End Source File
# Begin Source File

SOURCE=.\DefinedRanges.h
# End Source File
# Begin Source File

SOURCE=.\DelayUpdate.h
# End Source File
# Begin Source File

SOURCE=.\EditGrid.h
# End Source File
# Begin Source File

SOURCE=.\EqnChecker.h
# End Source File
# Begin Source File

SOURCE=.\EqnChecker2.h
# End Source File
# Begin Source File

SOURCE=.\ErrorChecker2.h
# End Source File
# Begin Source File

SOURCE=.\Layout\ETSLayout.h
# End Source File
# Begin Source File

SOURCE=.\FixedTabCtrl.h
# End Source File
# Begin Source File

SOURCE=.\font.h
# End Source File
# Begin Source File

SOURCE=.\phreeqc\src\global.h
# End Source File
# Begin Source File

SOURCE=.\GridListDoc.h
# End Source File
# Begin Source File

SOURCE=.\InputTreeOleDropTarget.h
# End Source File
# Begin Source File

SOURCE=.\Keyword.h
# End Source File
# Begin Source File

SOURCE=.\KeywordLoader.h
# End Source File
# Begin Source File

SOURCE=.\KeywordLoader2.h
# End Source File
# Begin Source File

SOURCE=.\KeywordPage.h
# End Source File
# Begin Source File

SOURCE=.\KeywordPageListItems.h
# End Source File
# Begin Source File

SOURCE=.\KeywordParser.h
# End Source File
# Begin Source File

SOURCE=.\KeywordSheet.h
# End Source File
# Begin Source File

SOURCE=.\KPAdvectionPg1.h
# End Source File
# Begin Source File

SOURCE=.\KPCopyPg1.h
# End Source File
# Begin Source File

SOURCE=.\KPExchangeMasterSpeciesPg1.h
# End Source File
# Begin Source File

SOURCE=.\KPExchangeSpeciesPg1.h
# End Source File
# Begin Source File

SOURCE=.\KPIncrementPg1.h
# End Source File
# Begin Source File

SOURCE=.\KPInversePg1.h
# End Source File
# Begin Source File

SOURCE=.\KPKnobsPg1.h
# End Source File
# Begin Source File

SOURCE=.\KPPhasesPg1.h
# End Source File
# Begin Source File

SOURCE=.\KPPrintPg1.h
# End Source File
# Begin Source File

SOURCE=.\KPRatesPg1.h
# End Source File
# Begin Source File

SOURCE=.\KPSavePg1.h
# End Source File
# Begin Source File

SOURCE=.\KPSelectedOutputPg1.h
# End Source File
# Begin Source File

SOURCE=.\KPSelectedOutputPg8.h
# End Source File
# Begin Source File

SOURCE=.\KPSelectedOutputPg9.h
# End Source File
# Begin Source File

SOURCE=.\KPSolution_SpreadPg1.h
# End Source File
# Begin Source File

SOURCE=.\KPSolutionMasterSpeciesPg1.h
# End Source File
# Begin Source File

SOURCE=.\KPSolutionPg1.h
# End Source File
# Begin Source File

SOURCE=.\KPSolutionSpeciesPg1.h
# End Source File
# Begin Source File

SOURCE=.\KPSurfaceMasterSpeciesPg1.h
# End Source File
# Begin Source File

SOURCE=.\KPSurfaceSpeciesPg1.h
# End Source File
# Begin Source File

SOURCE=.\KPTitlePg1.h
# End Source File
# Begin Source File

SOURCE=.\KPTransportPg1.h
# End Source File
# Begin Source File

SOURCE=.\KPUsePg1.h
# End Source File
# Begin Source File

SOURCE=.\KPUserPrintPg1.h
# End Source File
# Begin Source File

SOURCE=.\KPUserPunchPg1.h
# End Source File
# Begin Source File

SOURCE=.\KSExchangeMasterSpecies.h
# End Source File
# Begin Source File

SOURCE=.\KSExchangeSpecies.h
# End Source File
# Begin Source File

SOURCE=.\KSIncrement.h
# End Source File
# Begin Source File

SOURCE=.\KSKinetics.h
# End Source File
# Begin Source File

SOURCE=.\KSKnobs.h
# End Source File
# Begin Source File

SOURCE=.\KSPhases.h
# End Source File
# Begin Source File

SOURCE=.\KSPrint.h
# End Source File
# Begin Source File

SOURCE=.\KSSelectedOutput.h
# End Source File
# Begin Source File

SOURCE=.\KSSolutionMasterSpecies.h
# End Source File
# Begin Source File

SOURCE=.\KSSolutionSpecies.h
# End Source File
# Begin Source File

SOURCE=.\KSSurfaceMasterSpecies.h
# End Source File
# Begin Source File

SOURCE=.\KSSurfaceSpecies.h
# End Source File
# Begin Source File

SOURCE=.\KSTitle.h
# End Source File
# Begin Source File

SOURCE=.\LineParser.h
# End Source File
# Begin Source File

SOURCE=.\LocalException.h
# End Source File
# Begin Source File

SOURCE=.\LogicalLineParser.h
# End Source File
# Begin Source File

SOURCE=.\MainFrame.h
# End Source File
# Begin Source File

SOURCE=.\mshflexgrid.h
# End Source File
# Begin Source File

SOURCE=.\MultiDocTemplateIn.h
# End Source File
# Begin Source File

SOURCE=.\MultiDocTemplateOut.h
# End Source File
# Begin Source File

SOURCE=.\MyPropertyPage1.h
# End Source File
# Begin Source File

SOURCE=.\MyPropertySheet.h
# End Source File
# Begin Source File

SOURCE=.\MyRecentFileList.h
# End Source File
# Begin Source File

SOURCE=.\NodeObject.h
# End Source File
# Begin Source File

SOURCE=.\OCKSAdvection.h
# End Source File
# Begin Source File

SOURCE=.\OCKSCopy.h
# End Source File
# Begin Source File

SOURCE=.\OCKSRates.h
# End Source File
# Begin Source File

SOURCE=.\OCKSSave.h
# End Source File
# Begin Source File

SOURCE=.\OCKSSolution_Spread.h
# End Source File
# Begin Source File

SOURCE=.\OCKSTransport.h
# End Source File
# Begin Source File

SOURCE=.\OCKSUse.h
# End Source File
# Begin Source File

SOURCE=.\OCKSUserPrint.h
# End Source File
# Begin Source File

SOURCE=.\OCKSUserPunch.h
# End Source File
# Begin Source File

SOURCE=.\OleCtlKeywordSheet.h
# End Source File
# Begin Source File

SOURCE=.\PhreeqcCallback.h
# End Source File
# Begin Source File

SOURCE=.\phreeqci2.h
# End Source File
# Begin Source File

SOURCE=.\phreeqci_gui.h
# End Source File
# Begin Source File

SOURCE=.\picture.h
# End Source File
# Begin Source File

SOURCE=.\recordset.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# PROP Ignore_Default_Tool 1

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Making help include file...
TargetName=phreeqci
InputPath=.\Resource.h

"hlp\$(TargetName).hm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo. >"hlp\$(TargetName).hm" 
	echo // Commands (ID_* and IDM_*) >>"hlp\$(TargetName).hm" 
	makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Prompts (IDP_*) >>"hlp\$(TargetName).hm" 
	makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Resources (IDR_*) >>"hlp\$(TargetName).hm" 
	makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Dialogs (IDD_*) >>"hlp\$(TargetName).hm" 
	makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Frame Controls (IDW_*) >>"hlp\$(TargetName).hm" 
	makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\$(TargetName).hm" 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RichDocIn.h
# End Source File
# Begin Source File

SOURCE=.\RichDocInPages.h
# End Source File
# Begin Source File

SOURCE=.\RichDocInSheet.h
# End Source File
# Begin Source File

SOURCE=.\RichDocOut.h
# End Source File
# Begin Source File

SOURCE=.\RichEditLineParser.h
# End Source File
# Begin Source File

SOURCE=.\RichEditLineParserSafe.h
# End Source File
# Begin Source File

SOURCE=.\RichInCntrItem.h
# End Source File
# Begin Source File

SOURCE=.\RichKeywordParser.h
# End Source File
# Begin Source File

SOURCE=.\RichLogicalLineParser.h
# End Source File
# Begin Source File

SOURCE=.\RichOutCntrItem.h
# End Source File
# Begin Source File

SOURCE=.\RichViewIn.h
# End Source File
# Begin Source File

SOURCE=.\RichViewOut.h
# End Source File
# Begin Source File

SOURCE=.\RunDlg.h
# End Source File
# Begin Source File

SOURCE=.\SelectElementsDlg.h
# End Source File
# Begin Source File

SOURCE=.\Posea\sizecbar.h
# End Source File
# Begin Source File

SOURCE=.\Splash.h
# End Source File
# Begin Source File

SOURCE=.\srcdbpg.h
# End Source File
# Begin Source File

SOURCE=.\SRCDBPGCH.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StdioFileLineParser.h
# End Source File
# Begin Source File

SOURCE=.\TipDlg.h
# End Source File
# Begin Source File

SOURCE=.\Tree.h
# End Source File
# Begin Source File

SOURCE=.\Tree.inl
# End Source File
# Begin Source File

SOURCE=.\TreeCtrlErr.h
# End Source File
# Begin Source File

SOURCE=.\TreeCtrlIn.h
# End Source File
# Begin Source File

SOURCE=.\TreeCtrlOut.h
# End Source File
# Begin Source File

SOURCE=.\UpdateObject.h
# End Source File
# Begin Source File

SOURCE=.\Util.h
# End Source File
# Begin Source File

SOURCE=.\WorkspaceBar.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\checks.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ErrorItems.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\input.ico
# End Source File
# Begin Source File

SOURCE=.\res\InputItems.bmp
# End Source File
# Begin Source File

SOURCE=.\res\litebulb.bmp
# End Source File
# Begin Source File

SOURCE=.\res\litebulb2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\output.ico
# End Source File
# Begin Source File

SOURCE=.\res\OutputItems.bmp
# End Source File
# Begin Source File

SOURCE=.\res\phreeqci2.ico
# End Source File
# Begin Source File

SOURCE=.\res\phreeqci2.rc2
# End Source File
# Begin Source File

SOURCE=.\res\RichDocIn.ico
# End Source File
# Begin Source File

SOURCE=.\res\Splsh16.bmp
# End Source File
# Begin Source File

SOURCE=.\res\TabItems.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolbarForward.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolbarInitCond.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolbarPrint.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolbarRun.bmp
# End Source File
# End Group
# Begin Group "PHREEQC Source Files"

# PROP Default_Filter "c"
# Begin Source File

SOURCE=.\phreeqc\src\advection.c

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# ADD CPP /W3 /O2 /Op-
# SUBTRACT CPP /WX /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# ADD CPP /W3
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\src\basic.c

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# ADD CPP /W3 /O2 /Op-
# SUBTRACT CPP /WX /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# ADD CPP /W3
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\src\basicsubs.c

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# ADD CPP /W3 /O2 /Op-
# SUBTRACT CPP /WX /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# ADD CPP /W3
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\src\cl1.c

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# ADD CPP /W3 /O2 /Op-
# SUBTRACT CPP /WX /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# ADD CPP /W3
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\src\cl1mp.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\phreeqc\src\cvdense.c

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# ADD CPP /W3
# SUBTRACT CPP /WX /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\src\cvode.c

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# ADD CPP /W3
# SUBTRACT CPP /WX /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\src\dense.c

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# ADD CPP /W3
# SUBTRACT CPP /WX /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\src\dw.c

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# ADD CPP /W3
# SUBTRACT CPP /WX /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\src\input.c

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# ADD CPP /W3
# SUBTRACT CPP /WX /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\src\integrate.c

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# ADD CPP /W3 /O2 /Op-
# SUBTRACT CPP /WX /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# ADD CPP /W3
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\src\inverse.c

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# ADD CPP /W3 /O2 /Op-
# SUBTRACT CPP /WX /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# ADD CPP /W3
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\src\isotopes.c

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# ADD CPP /W3 /O2 /Op-
# SUBTRACT CPP /WX /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# ADD CPP /W3
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\src\kinetics.c

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# ADD CPP /W3 /O2 /Op-
# SUBTRACT CPP /WX /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# ADD CPP /W3
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\src\main.c

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# ADD CPP /W3 /O2 /Op-
# SUBTRACT CPP /WX /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# ADD CPP /W3
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\src\mainsubs.c

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# ADD CPP /W3 /O2 /Op-
# SUBTRACT CPP /WX /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# ADD CPP /W3
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\src\model.c

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# ADD CPP /W3 /O2 /Op-
# SUBTRACT CPP /WX /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# ADD CPP /W3
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\src\nvector.c

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# ADD CPP /W3
# SUBTRACT CPP /WX /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\src\nvector_serial.c

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# ADD CPP /W3
# SUBTRACT CPP /WX /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\src\output.c

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# ADD CPP /W3
# SUBTRACT CPP /WX /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\src\p2clib.c

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# ADD CPP /W3 /O2 /Op-
# SUBTRACT CPP /WX /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# ADD CPP /W3
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\src\parse.c

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# ADD CPP /W3 /O2 /Op-
# SUBTRACT CPP /WX /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# ADD CPP /W3
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\src\phqalloc.c

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# ADD CPP /W3 /O2 /Op-
# SUBTRACT CPP /WX /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# ADD CPP /W3
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\src\phreeqc_files.c

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# ADD CPP /W3
# SUBTRACT CPP /WX /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqci_files.c

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# ADD CPP /W3
# SUBTRACT CPP /WX /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqci_gui.c

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# ADD CPP /W3 /O2 /Op-
# SUBTRACT CPP /WX /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# ADD CPP /W3
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\src\pitzer.c

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# ADD CPP /W3
# SUBTRACT CPP /WX /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\src\pitzer_structures.c

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# ADD CPP /W3
# SUBTRACT CPP /WX /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\src\prep.c

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# ADD CPP /W3 /O2 /Op-
# SUBTRACT CPP /WX /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# ADD CPP /W3
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\src\print.c

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# ADD CPP /W3 /O2 /Op-
# SUBTRACT CPP /WX /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# ADD CPP /W3
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\src\read.c

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# ADD CPP /W3 /O2 /Op-
# SUBTRACT CPP /WX /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# ADD CPP /W3
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\src\readtr.c

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# ADD CPP /W3 /O2 /Op-
# SUBTRACT CPP /WX /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# ADD CPP /W3
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\src\smalldense.c

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# ADD CPP /W3
# SUBTRACT CPP /WX /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\src\spread.c

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# ADD CPP /W3 /O2 /Op-
# SUBTRACT CPP /WX /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# ADD CPP /W3
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\src\step.c

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# ADD CPP /W3 /O2 /Op-
# SUBTRACT CPP /WX /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# ADD CPP /W3
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\src\structures.c

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# ADD CPP /W3 /O2 /Op-
# SUBTRACT CPP /WX /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# ADD CPP /W3
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\src\sundialsmath.c

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# ADD CPP /W3
# SUBTRACT CPP /WX /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\src\tally.c

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# ADD CPP /W3
# SUBTRACT CPP /WX /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\src\tidy.c

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# ADD CPP /W3 /O2 /Op-
# SUBTRACT CPP /WX /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# ADD CPP /W3
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\src\transport.c

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# ADD CPP /W3 /O2 /Op-
# SUBTRACT CPP /WX /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# ADD CPP /W3
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\src\utilities.c

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# ADD CPP /W3 /O2 /Op-
# SUBTRACT CPP /WX /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# ADD CPP /W3
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# End Group
# Begin Group "Databases"

# PROP Default_Filter "dat"
# Begin Source File

SOURCE=.\phreeqc\examples\ex15.dat

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# Begin Custom Build - Copying $(InputPath)
InputPath=.\phreeqc\examples\ex15.dat
InputName=ex15

"examples\$(InputName).dat" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) examples

# End Custom Build

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath)
InputPath=.\phreeqc\examples\ex15.dat
InputName=ex15

"examples\$(InputName).dat" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) examples

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\database\iso.dat

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# Begin Custom Build - Copying $(InputPath)
OutDir=.\Release
InputPath=.\phreeqc\database\iso.dat
InputName=iso

"$(OutDir)\$(InputName).dat" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) $(OutDir)

# End Custom Build

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath)
OutDir=.\Debug
InputPath=.\phreeqc\database\iso.dat
InputName=iso

"$(OutDir)\$(InputName).dat" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) $(OutDir)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\database\llnl.dat

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# Begin Custom Build - Copying $(InputPath)
OutDir=.\Release
InputPath=.\phreeqc\database\llnl.dat
InputName=llnl

"$(OutDir)\$(InputName).dat" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) $(OutDir)

# End Custom Build

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath)
OutDir=.\Debug
InputPath=.\phreeqc\database\llnl.dat
InputName=llnl

"$(OutDir)\$(InputName).dat" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) $(OutDir)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\database\minteq.dat

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# Begin Custom Build - Copying $(InputPath)
OutDir=.\Release
InputPath=.\phreeqc\database\minteq.dat
InputName=minteq

"$(OutDir)\$(InputName).dat" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) $(OutDir)

# End Custom Build

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath)
OutDir=.\Debug
InputPath=.\phreeqc\database\minteq.dat
InputName=minteq

"$(OutDir)\$(InputName).dat" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) $(OutDir)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\database\minteq.v4.dat

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# Begin Custom Build - Copying $(InputPath)
OutDir=.\Release
InputPath=.\phreeqc\database\minteq.v4.dat
InputName=minteq.v4

"$(OutDir)\$(InputName).dat" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) $(OutDir)

# End Custom Build

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\database\phreeqc.dat

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# Begin Custom Build - Copying $(InputPath)
OutDir=.\Release
InputPath=.\phreeqc\database\phreeqc.dat
InputName=phreeqc

"$(OutDir)\$(InputName).dat" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) $(OutDir)

# End Custom Build

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath)
OutDir=.\Debug
InputPath=.\phreeqc\database\phreeqc.dat
InputName=phreeqc

"$(OutDir)\$(InputName).dat" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) $(OutDir)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\database\phreeqd.dat

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# Begin Custom Build - Copying $(InputPath)
OutDir=.\Release
InputPath=.\phreeqc\database\phreeqd.dat
InputName=phreeqd

"$(OutDir)\$(InputName).dat" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) $(OutDir)

# End Custom Build

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\database\pitzer.dat

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# Begin Custom Build - Copying $(InputPath)
OutDir=.\Release
InputPath=.\phreeqc\database\pitzer.dat
InputName=pitzer

"$(OutDir)\$(InputName).dat" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) $(OutDir)

# End Custom Build

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath)
OutDir=.\Debug
InputPath=.\phreeqc\database\pitzer.dat
InputName=pitzer

"$(OutDir)\$(InputName).dat" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) $(OutDir)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\database\wateq4f.dat

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# Begin Custom Build - Copying $(InputPath)
OutDir=.\Release
InputPath=.\phreeqc\database\wateq4f.dat
InputName=wateq4f

"$(OutDir)\$(InputName).dat" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) $(OutDir)

# End Custom Build

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath)
OutDir=.\Debug
InputPath=.\phreeqc\database\wateq4f.dat
InputName=wateq4f

"$(OutDir)\$(InputName).dat" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) $(OutDir)

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Examples"

# PROP Default_Filter ""
# Begin Group "Input"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\phreeqc\examples\ex1

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# Begin Custom Build - Copying $(InputPath)
InputPath=.\phreeqc\examples\ex1
InputName=ex1

"examples\$(InputName).pqi" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	AddDB.pl $(InputPath) examples\$(InputName).pqi

# End Custom Build

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath)
InputPath=.\phreeqc\examples\ex1
InputName=ex1

"examples\$(InputName).pqi" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	AddDB.pl $(InputPath) examples\$(InputName).pqi

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\examples\ex10

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# Begin Custom Build - Copying $(InputPath)
InputPath=.\phreeqc\examples\ex10
InputName=ex10

"examples\$(InputName).pqi" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	AddDB.pl $(InputPath) examples\$(InputName).pqi

# End Custom Build

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath)
InputPath=.\phreeqc\examples\ex10
InputName=ex10

"examples\$(InputName).pqi" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	AddDB.pl $(InputPath) examples\$(InputName).pqi

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\examples\ex11

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# Begin Custom Build - Copying $(InputPath)
InputPath=.\phreeqc\examples\ex11
InputName=ex11

"examples\$(InputName).pqi" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	AddDB.pl $(InputPath) examples\$(InputName).pqi

# End Custom Build

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath)
InputPath=.\phreeqc\examples\ex11
InputName=ex11

"examples\$(InputName).pqi" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	AddDB.pl $(InputPath) examples\$(InputName).pqi

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\examples\ex12

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# Begin Custom Build - Copying $(InputPath)
InputPath=.\phreeqc\examples\ex12
InputName=ex12

"examples\$(InputName).pqi" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	AddDB.pl $(InputPath) examples\$(InputName).pqi

# End Custom Build

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath)
InputPath=.\phreeqc\examples\ex12
InputName=ex12

"examples\$(InputName).pqi" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	AddDB.pl $(InputPath) examples\$(InputName).pqi

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\examples\ex12a

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# Begin Custom Build - Copying $(InputPath)
InputPath=.\phreeqc\examples\ex12a
InputName=ex12a

"examples\$(InputName).pqi" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	AddDB.pl $(InputPath) examples\$(InputName).pqi

# End Custom Build

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath)
InputPath=.\phreeqc\examples\ex12a
InputName=ex12a

"examples\$(InputName).pqi" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	AddDB.pl $(InputPath) examples\$(InputName).pqi

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\examples\ex13a

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# Begin Custom Build - Copying $(InputPath)
InputPath=.\phreeqc\examples\ex13a
InputName=ex13a

"examples\$(InputName).pqi" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	AddDB.pl $(InputPath) examples\$(InputName).pqi

# End Custom Build

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath)
InputPath=.\phreeqc\examples\ex13a
InputName=ex13a

"examples\$(InputName).pqi" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	AddDB.pl $(InputPath) examples\$(InputName).pqi

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\examples\ex13b

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# Begin Custom Build - Copying $(InputPath)
InputPath=.\phreeqc\examples\ex13b
InputName=ex13b

"examples\$(InputName).pqi" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	AddDB.pl $(InputPath) examples\$(InputName).pqi

# End Custom Build

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath)
InputPath=.\phreeqc\examples\ex13b
InputName=ex13b

"examples\$(InputName).pqi" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	AddDB.pl $(InputPath) examples\$(InputName).pqi

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\examples\ex13c

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# Begin Custom Build - Copying $(InputPath)
InputPath=.\phreeqc\examples\ex13c
InputName=ex13c

"examples\$(InputName).pqi" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	AddDB.pl $(InputPath) examples\$(InputName).pqi

# End Custom Build

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath)
InputPath=.\phreeqc\examples\ex13c
InputName=ex13c

"examples\$(InputName).pqi" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	AddDB.pl $(InputPath) examples\$(InputName).pqi

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\examples\ex14

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# Begin Custom Build - Copying $(InputPath)
InputPath=.\phreeqc\examples\ex14
InputName=ex14

"examples\$(InputName).pqi" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	AddDB.pl $(InputPath) examples\$(InputName).pqi

# End Custom Build

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath)
InputPath=.\phreeqc\examples\ex14
InputName=ex14

"examples\$(InputName).pqi" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	AddDB.pl $(InputPath) examples\$(InputName).pqi

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\examples\ex15

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# Begin Custom Build - Copying $(InputPath)
InputPath=.\phreeqc\examples\ex15
InputName=ex15

"examples\$(InputName).pqi" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	AddDB.pl $(InputPath) examples\$(InputName).pqi

# End Custom Build

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath)
InputPath=.\phreeqc\examples\ex15
InputName=ex15

"examples\$(InputName).pqi" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	AddDB.pl $(InputPath) examples\$(InputName).pqi

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\examples\ex16

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# Begin Custom Build - Copying $(InputPath)
InputPath=.\phreeqc\examples\ex16
InputName=ex16

"examples\$(InputName).pqi" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	AddDB.pl $(InputPath) examples\$(InputName).pqi

# End Custom Build

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath)
InputPath=.\phreeqc\examples\ex16
InputName=ex16

"examples\$(InputName).pqi" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	AddDB.pl $(InputPath) examples\$(InputName).pqi

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\examples\ex17

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# Begin Custom Build - Copying $(InputPath)
InputPath=.\phreeqc\examples\ex17
InputName=ex17

"examples\$(InputName).pqi" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	AddDB.pl $(InputPath) examples\$(InputName).pqi

# End Custom Build

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath)
InputPath=.\phreeqc\examples\ex17
InputName=ex17

"examples\$(InputName).pqi" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	AddDB.pl $(InputPath) examples\$(InputName).pqi

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\examples\ex18

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# Begin Custom Build - Copying $(InputPath)
InputPath=.\phreeqc\examples\ex18
InputName=ex18

"examples\$(InputName).pqi" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	AddDB.pl $(InputPath) examples\$(InputName).pqi

# End Custom Build

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath)
InputPath=.\phreeqc\examples\ex18
InputName=ex18

"examples\$(InputName).pqi" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	AddDB.pl $(InputPath) examples\$(InputName).pqi

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\examples\ex2

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# Begin Custom Build - Copying $(InputPath)
InputPath=.\phreeqc\examples\ex2
InputName=ex2

"examples\$(InputName).pqi" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	AddDB.pl $(InputPath) examples\$(InputName).pqi

# End Custom Build

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath)
InputPath=.\phreeqc\examples\ex2
InputName=ex2

"examples\$(InputName).pqi" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	AddDB.pl $(InputPath) examples\$(InputName).pqi

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\examples\ex3

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# Begin Custom Build - Copying $(InputPath)
InputPath=.\phreeqc\examples\ex3
InputName=ex3

"examples\$(InputName).pqi" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	AddDB.pl $(InputPath) examples\$(InputName).pqi

# End Custom Build

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath)
InputPath=.\phreeqc\examples\ex3
InputName=ex3

"examples\$(InputName).pqi" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	AddDB.pl $(InputPath) examples\$(InputName).pqi

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\examples\ex4

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# Begin Custom Build - Copying $(InputPath)
InputPath=.\phreeqc\examples\ex4
InputName=ex4

"examples\$(InputName).pqi" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	AddDB.pl $(InputPath) examples\$(InputName).pqi

# End Custom Build

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath)
InputPath=.\phreeqc\examples\ex4
InputName=ex4

"examples\$(InputName).pqi" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	AddDB.pl $(InputPath) examples\$(InputName).pqi

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\examples\ex5

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# Begin Custom Build - Copying $(InputPath)
InputPath=.\phreeqc\examples\ex5
InputName=ex5

"examples\$(InputName).pqi" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	AddDB.pl $(InputPath) examples\$(InputName).pqi

# End Custom Build

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath)
InputPath=.\phreeqc\examples\ex5
InputName=ex5

"examples\$(InputName).pqi" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	AddDB.pl $(InputPath) examples\$(InputName).pqi

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\examples\ex6

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# Begin Custom Build - Copying $(InputPath)
InputPath=.\phreeqc\examples\ex6
InputName=ex6

"examples\$(InputName).pqi" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	AddDB.pl $(InputPath) examples\$(InputName).pqi

# End Custom Build

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath)
InputPath=.\phreeqc\examples\ex6
InputName=ex6

"examples\$(InputName).pqi" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	AddDB.pl $(InputPath) examples\$(InputName).pqi

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\examples\ex7

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# Begin Custom Build - Copying $(InputPath)
InputPath=.\phreeqc\examples\ex7
InputName=ex7

"examples\$(InputName).pqi" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	AddDB.pl $(InputPath) examples\$(InputName).pqi

# End Custom Build

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath)
InputPath=.\phreeqc\examples\ex7
InputName=ex7

"examples\$(InputName).pqi" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	AddDB.pl $(InputPath) examples\$(InputName).pqi

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\examples\ex8

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# Begin Custom Build - Copying $(InputPath)
InputPath=.\phreeqc\examples\ex8
InputName=ex8

"examples\$(InputName).pqi" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	AddDB.pl $(InputPath) examples\$(InputName).pqi

# End Custom Build

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath)
InputPath=.\phreeqc\examples\ex8
InputName=ex8

"examples\$(InputName).pqi" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	AddDB.pl $(InputPath) examples\$(InputName).pqi

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqc\examples\ex9

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# Begin Custom Build - Copying $(InputPath)
InputPath=.\phreeqc\examples\ex9
InputName=ex9

"examples\$(InputName).pqi" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	AddDB.pl $(InputPath) examples\$(InputName).pqi

# End Custom Build

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# Begin Custom Build - Copying $(InputPath)
InputPath=.\phreeqc\examples\ex9
InputName=ex9

"examples\$(InputName).pqi" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	AddDB.pl $(InputPath) examples\$(InputName).pqi

# End Custom Build

!ENDIF 

# End Source File
# End Group
# End Group
# Begin Group "Html Help"

# PROP Default_Filter "*.chm"
# Begin Source File

SOURCE=.\Help\phreeqci.chm
# End Source File
# End Group
# Begin Group "Fact Sheet"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\fs-031-02\fs-031-02.chm"
# End Source File
# End Group
# Begin Group "PHREEQC Extra"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\phreeqc\extra\basic1.c
# ADD CPP /W1
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# End Group
# Begin Group "PHREEQC Header Files"

# PROP Default_Filter ""
# End Group
# Begin Source File

SOURCE=.\Bugs.txt
# End Source File
# Begin Source File

SOURCE=".\Class Layout.txt"
# End Source File
# Begin Source File

SOURCE=".\Command Routing.txt"
# End Source File
# Begin Source File

SOURCE=".\EditGrid Combo Handling.txt"
# End Source File
# Begin Source File

SOURCE=.\Layout\doc\layoutmgr.html
# End Source File
# Begin Source File

SOURCE=.\Notes.txt
# End Source File
# Begin Source File

SOURCE=.\phreeqci.eng

!IF  "$(CFG)" == "phreeqci2 - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=.\phreeqci.eng
InputName=phreeqci

"$(OutDir)\$(InputName).eng" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputName).eng $(OutDir)

# End Custom Build

!ELSEIF  "$(CFG)" == "phreeqci2 - Win32 Debug"

# Begin Custom Build
OutDir=.\Debug
InputPath=.\phreeqci.eng
InputName=phreeqci

"$(OutDir)\$(InputName).eng" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputName).eng $(OutDir)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\phreeqci2.reg
# End Source File
# Begin Source File

SOURCE=".\Docs\Q241209 - HOWTO Convert a New MFC Application to HTML Help Using WinHelp.htm"
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section phreeqci2 : {F0D2F21A-CCB0-11D0-A316-00AA00688B10}
# 	2:5:Class:CDataCombo
# 	2:10:HeaderFile:datacombo.h
# 	2:8:ImplFile:datacombo.cpp
# End Section
# Section phreeqci2 : {2540D29A-5FB9-494D-A2A1-7AD80599E582}
# 	2:5:Class:CSRCDBPG
# 	2:10:HeaderFile:srcdbpg.h
# 	2:8:ImplFile:srcdbpg.cpp
# End Section
# Section phreeqci2 : {72ADFD54-2C39-11D0-9903-00A0C91BC942}
# 	1:17:CG_IDS_DIDYOUKNOW:126
# 	1:22:CG_IDS_TIPOFTHEDAYMENU:125
# 	1:18:CG_IDS_TIPOFTHEDAY:124
# 	1:22:CG_IDS_TIPOFTHEDAYHELP:161
# 	1:19:CG_IDP_FILE_CORRUPT:160
# 	1:7:IDD_TIP:123
# 	1:13:IDB_LIGHTBULB:122
# 	1:18:CG_IDS_FILE_ABSENT:127
# 	2:17:CG_IDS_DIDYOUKNOW:CG_IDS_DIDYOUKNOW
# 	2:7:CTipDlg:CTipDlg
# 	2:22:CG_IDS_TIPOFTHEDAYMENU:CG_IDS_TIPOFTHEDAYMENU
# 	2:18:CG_IDS_TIPOFTHEDAY:CG_IDS_TIPOFTHEDAY
# 	2:12:CTIP_Written:OK
# 	2:22:CG_IDS_TIPOFTHEDAYHELP:CG_IDS_TIPOFTHEDAYHELP
# 	2:2:BH:
# 	2:19:CG_IDP_FILE_CORRUPT:CG_IDP_FILE_CORRUPT
# 	2:7:IDD_TIP:IDD_TIP
# 	2:8:TipDlg.h:TipDlg.h
# 	2:13:IDB_LIGHTBULB:IDB_LIGHTBULB
# 	2:18:CG_IDS_FILE_ABSENT:CG_IDS_FILE_ABSENT
# 	2:10:TipDlg.cpp:TipDlg.cpp
# End Section
# Section phreeqci2 : {0ECD9B64-23AA-11D0-B351-00A0C9055D8E}
# 	2:21:DefaultSinkHeaderFile:mshflexgrid.h
# 	2:16:DefaultSinkClass:CMSHFlexGrid
# End Section
# Section phreeqci2 : {BEF6E003-A874-101A-8BBA-00AA00300CAB}
# 	2:5:Class:COleFont
# 	2:10:HeaderFile:font.h
# 	2:8:ImplFile:font.cpp
# End Section
# Section phreeqci2 : {F0D2F21C-CCB0-11D0-A316-00AA00688B10}
# 	2:21:DefaultSinkHeaderFile:datacombo.h
# 	2:16:DefaultSinkClass:CDataCombo
# End Section
# Section phreeqci2 : {72ADFD78-2C39-11D0-9903-00A0C91BC942}
# 	1:10:IDB_SPLASH:162
# 	2:21:SplashScreenInsertKey:4.0
# End Section
# Section phreeqci2 : {7BF80981-BF32-101A-8BBB-00AA00300CAB}
# 	2:5:Class:CPicture
# 	2:10:HeaderFile:picture.h
# 	2:8:ImplFile:picture.cpp
# End Section
# Section phreeqci2 : {0ECD9B62-23AA-11D0-B351-00A0C9055D8E}
# 	2:5:Class:CMSHFlexGrid
# 	2:10:HeaderFile:mshflexgrid.h
# 	2:8:ImplFile:mshflexgrid.cpp
# End Section
# Section phreeqci2 : {5EE2FCD7-F527-4900-AF7C-33BCC60627FB}
# 	2:21:DefaultSinkHeaderFile:srcdbpg.h
# 	2:16:DefaultSinkClass:CSRCDBPG
# End Section
# Section phreeqci2 : {0000050E-0000-0010-8000-00AA006D2EA4}
# 	2:5:Class:CRecordset1
# 	2:10:HeaderFile:recordset.h
# 	2:8:ImplFile:recordset.cpp
# End Section
