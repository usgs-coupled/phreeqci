//-------------------------------------------------------------------------------------------------
// <copyright file="XmlFile.cpp" company="Microsoft">
//    Copyright (c) Microsoft Corporation.  All rights reserved.
//    
//    The use and distribution terms for this software are covered by the
//    Common Public License 1.0 (http://opensource.org/licenses/cpl.php)
//    which can be found in the file CPL.TXT at the root of this distribution.
//    By using this software in any fashion, you are agreeing to be bound by
//    the terms of this license.
//    
//    You must not remove this notice, or any other, from this software.
// </copyright>
// 
// <summary>
//    Code to configure XML files.
// </summary>
//-------------------------------------------------------------------------------------------------

// COMMENT: {10/26/2009 9:01:32 PM}#include "precomp.h"
#include "stdafx.h"
//{{
#define LOGMSG_VERBOSE   (LOGLEVEL)2
#define LOGMSG_TRACEONLY (LOGLEVEL)2
//}}

#define XMLFILE_CREATE_ELEMENT 0x00000001
#define XMLFILE_DELETE_VALUE 0x00000002
#define XMLFILE_BULKWRITE_VALUE 0x00000004

#define XMLFILE_DONT_UNINSTALL 0x00010000
#define XMLFILE_PRESERVE_MODIFIED 0x00001000
#define XMLFILE_USE_XPATH 0x00000100

extern BOOL vfMsxml30;

enum eXmlAction
{
    xaOpenFile = 1,
    xaOpenFilex64,
    xaWriteValue,
    xaDeleteValue,
    xaCreateElement,
    xaDeleteElement,
    xaBulkWriteValue,
};

enum eXmlPreserveDate
{
    xdDontPreserve = 0,
    xdPreserve
};

enum eXmlSelectionLanguage
{
    xsXSLPattern = 0,
    xsXPath = 1,
};

LPCWSTR vcsXmlFileQuery =
    L"SELECT `XmlFile`.`XmlFile`, `XmlFile`.`File`, `XmlFile`.`ElementPath`, `XmlFile`.`Name`, `XmlFile`.`Header`, "
    L"`XmlFile`.`Flags`, `XmlFile`.`Component_`, `Component`.`Attributes` "
    L"FROM `XmlFile`,`Component` WHERE `XmlFile`.`Component_`=`Component`.`Component` ORDER BY `File`, `Sequence`";
enum eXmlFileQuery { xfqXmlFile = 1, xfqFile, xfqXPath, xfqName, xfqHeader, xfqXmlFlags, xfqComponent, xfqCompAttributes  };

struct XML_FILE_CHANGE
{
    WCHAR wzId[MAX_DARWIN_KEY];

    INSTALLSTATE isInstalled;
    INSTALLSTATE isAction;

    WCHAR wzFile[MAX_PATH];
    LPWSTR pwzElementPath;
    WCHAR wzName[MAX_DARWIN_COLUMN];
    LPWSTR pwzHeader;

    int iXmlFlags;
    int iCompAttributes;

    XML_FILE_CHANGE* pxfcPrev;
    XML_FILE_CHANGE* pxfcNext;
};

static HRESULT FreeXmlFileChangeList(
    __in XML_FILE_CHANGE* pxfcList
    )
{
    HRESULT hr = S_OK;

    XML_FILE_CHANGE* pxfcDelete;
    while(pxfcList)
    {
        pxfcDelete = pxfcList;
        pxfcList = pxfcList->pxfcNext;

        ReleaseStr(pxfcDelete->pwzElementPath);
        ReleaseStr(pxfcDelete->pwzHeader);

        hr = MemFree(pxfcDelete);
        ExitOnFailure(hr, "failed to free xml file change list item");
    }

LExit:
    return hr;
}

static HRESULT AddXmlFileChangeToList(
    __inout XML_FILE_CHANGE** ppxfcHead,
    __inout XML_FILE_CHANGE** ppxfcTail
    )
{
    Assert(ppxfcHead && ppxfcTail);

    HRESULT hr = S_OK;

    XML_FILE_CHANGE* pxfc = static_cast<XML_FILE_CHANGE*>(MemAlloc(sizeof(XML_FILE_CHANGE), TRUE));
    ExitOnNull(pxfc, hr, E_OUTOFMEMORY, "failed to allocate memory for new xml file change list element");

    // Add it to the end of the list
    if (NULL == *ppxfcHead)
    {
        *ppxfcHead = pxfc;
        *ppxfcTail = pxfc;
    }
    else
    {
        Assert(*ppxfcTail && (*ppxfcTail)->pxfcNext == NULL);
        (*ppxfcTail)->pxfcNext = pxfc;
        pxfc->pxfcPrev = *ppxfcTail;
        *ppxfcTail = pxfc;
    }

LExit:
    return hr;
}


static HRESULT ReadXmlFileTable(
    __inout XML_FILE_CHANGE** ppxfcHead,
    __inout XML_FILE_CHANGE** ppxfcTail
    )
{
    Assert(ppxfcHead && ppxfcTail);

    HRESULT hr = S_OK;
    UINT er = ERROR_SUCCESS;

    PMSIHANDLE hView = NULL;
    PMSIHANDLE hRec = NULL;

    LPWSTR pwzData = NULL;

    // check to see if necessary tables are specified
    if (S_FALSE == WcaTableExists(L"XmlFile"))
        ExitFunction1(hr = S_FALSE);

    // loop through all the xml configurations
    hr = WcaOpenExecuteView(vcsXmlFileQuery, &hView);
    ExitOnFailure(hr, "failed to open view on XmlFile table");

    while (S_OK == (hr = WcaFetchRecord(hView, &hRec)))
    {
        hr = AddXmlFileChangeToList(ppxfcHead, ppxfcTail);
        ExitOnFailure(hr, "failed to add xml file change to list");

        // Get record Id
        hr = WcaGetRecordString(hRec, xfqXmlFile, &pwzData);
        ExitOnFailure(hr, "failed to get XmlFile record Id");
        hr = StringCchCopyW((*ppxfcTail)->wzId, countof((*ppxfcTail)->wzId), pwzData);
        ExitOnFailure(hr, "failed to copy XmlFile record Id");

        // Get component name
        hr = WcaGetRecordString(hRec, xfqComponent, &pwzData);
        ExitOnFailure1(hr, "failed to get component name for XmlFile: %S", (*ppxfcTail)->wzId);

        // Get the component's state
        er = ::MsiGetComponentStateW(WcaGetInstallHandle(), pwzData, &(*ppxfcTail)->isInstalled, &(*ppxfcTail)->isAction);
        ExitOnFailure1(hr = HRESULT_FROM_WIN32(er), "failed to get install state for Component: %S", pwzData);

        // Get the xml file
        hr = WcaGetRecordFormattedString(hRec, xfqFile, &pwzData);
        ExitOnFailure1(hr, "failed to get xml file for XmlFile: %S", (*ppxfcTail)->wzId);
        hr = StringCchCopyW((*ppxfcTail)->wzFile, countof((*ppxfcTail)->wzFile), pwzData);
        ExitOnFailure1(hr, "failed to copy xml file: %S", (*ppxfcTail)->wzFile);

        // Get the XmlFile table flags
        hr = WcaGetRecordInteger(hRec, xfqXmlFlags, &(*ppxfcTail)->iXmlFlags);
        ExitOnFailure1(hr, "failed to get XmlFile flags for XmlFile: %S", (*ppxfcTail)->wzId);

        // Get the XPath
        hr = WcaGetRecordFormattedString(hRec, xfqXPath, &(*ppxfcTail)->pwzElementPath);
        ExitOnFailure1(hr, "failed to get XPath for XmlFile: %S", (*ppxfcTail)->wzId);

        // Get the name
        hr = WcaGetRecordFormattedString(hRec, xfqName, &pwzData);
        ExitOnFailure1(hr, "failed to get Name for XmlFile: %S", (*ppxfcTail)->wzId);
        hr = StringCchCopyW((*ppxfcTail)->wzName, countof((*ppxfcTail)->wzName), pwzData);
        ExitOnFailure1(hr, "failed to copy name: %S", (*ppxfcTail)->wzName);

        // Get the value
        hr = WcaGetRecordFormattedString(hRec, xfqHeader, &pwzData);
        ExitOnFailure1(hr, "failed to get Header for XmlFile: %S", (*ppxfcTail)->wzId);
        hr = StrAllocString(&(*ppxfcTail)->pwzHeader, pwzData, 0);
        ExitOnFailure1(hr, "failed to allocate buffer for Header: %S", (*ppxfcTail)->pwzHeader);

        // Get the component attributes
        hr = WcaGetRecordInteger(hRec, xfqCompAttributes, &(*ppxfcTail)->iCompAttributes);
        ExitOnFailure1(hr, "failed to get component attributes for XmlFile: %S", (*ppxfcTail)->wzId);
    }

    // if we looped through all records all is well
    if (E_NOMOREITEMS == hr)
        hr = S_OK;
    ExitOnFailure(hr, "failed while looping through all objects to secure");

LExit:
    ReleaseStr(pwzData);

    return hr;
}


static HRESULT BeginChangeFile(
    __in LPCWSTR pwzFile,
    __in XML_FILE_CHANGE* pxfc,
    __inout LPWSTR* ppwzCustomActionData
    )
{
    Assert(pwzFile && *pwzFile && ppwzCustomActionData);

    HRESULT hr = S_OK;
    BOOL fIs64Bit = pxfc->iCompAttributes & msidbComponentAttributes64bit;
    BOOL fUseXPath = pxfc->iXmlFlags & XMLFILE_USE_XPATH;
    LPBYTE pbData = NULL;
    DWORD cbData = 0;

    LPWSTR pwzRollbackCustomActionData = NULL;

    if (fIs64Bit)
    {
        hr = WcaWriteIntegerToCaData((int)xaOpenFilex64, ppwzCustomActionData);
        ExitOnFailure(hr, "failed to write 64-bit file indicator to custom action data");
    }
    else
    {
        hr = WcaWriteIntegerToCaData((int)xaOpenFile, ppwzCustomActionData);
        ExitOnFailure(hr, "failed to write file indicator to custom action data");
    }
    if (fUseXPath)
    {
        hr = WcaWriteIntegerToCaData((int)xsXPath, ppwzCustomActionData);
        ExitOnFailure(hr, "failed to write XPath selectionlanguage indicator to custom action data");
    }
    else
    {
        hr = WcaWriteIntegerToCaData((int)xsXSLPattern, ppwzCustomActionData);
        ExitOnFailure(hr, "failed to write XSLPattern selectionlanguage indicator to custom action data");
    }
    hr = WcaWriteStringToCaData(pwzFile, ppwzCustomActionData);
    ExitOnFailure1(hr, "failed to write file to custom action data: %S", pwzFile);

    // If the file already exits, then we have to put it back the way it was on failure
    if (FileExistsEx(pwzFile, NULL))
    {
		//{{
		WcaLog(LOGMSG_STANDARD, "FileExistsEx(%S) - TRUE", pwzFile);
		//}}

        hr = FileRead(&pbData, &cbData, pwzFile);
        ExitOnFailure1(hr, "failed to read file: %S", pwzFile);

        // Set up the rollback for this file
        hr = WcaWriteIntegerToCaData((int)fIs64Bit, &pwzRollbackCustomActionData);
        ExitOnFailure(hr, "failed to write component bitness to rollback custom action data");

        hr = WcaWriteStringToCaData(pwzFile, &pwzRollbackCustomActionData);
        ExitOnFailure1(hr, "failed to write file name to rollback custom action data: %S", pwzFile);

        hr = WcaWriteStreamToCaData(pbData, cbData, &pwzRollbackCustomActionData);
        ExitOnFailure(hr, "failed to write file contents to rollback custom action data.");

        hr = WcaDoDeferredAction(L"ExecXmlFileRollbackSRC", pwzRollbackCustomActionData, COST_XMLFILE);
        ExitOnFailure1(hr, "failed to schedule ExecXmlFileRollbackSRC for file: %S", pwzFile);

        ReleaseStr(pwzRollbackCustomActionData);
    }
	//{{
	else
	{
		WcaLog(LOGMSG_STANDARD, "FileExistsEx(%S) - FALSE", pwzFile);
	}
	//}}
LExit:
    ReleaseMem(pbData);

    return hr;
}


static HRESULT WriteChangeData(
    __in XML_FILE_CHANGE* pxfc,
    __inout LPWSTR* ppwzCustomActionData
    )
{
    Assert(pxfc && ppwzCustomActionData);

    HRESULT hr = S_OK;

    hr = WcaWriteStringToCaData(pxfc->pwzElementPath, ppwzCustomActionData);
    ExitOnFailure1(hr, "failed to write ElementPath to custom action data: %S", pxfc->pwzElementPath);

    hr = WcaWriteStringToCaData(pxfc->wzName, ppwzCustomActionData);
    ExitOnFailure1(hr, "failed to write Name to custom action data: %S", pxfc->wzName);

    hr = WcaWriteStringToCaData(pxfc->pwzHeader, ppwzCustomActionData);
    ExitOnFailure1(hr, "failed to write Header to custom action data: %S", pxfc->pwzHeader);

LExit:
    return hr;
}


/******************************************************************
 SchedXmlFile - entry point for XmlFile Custom Action

********************************************************************/
extern "C" UINT __stdcall SchedXmlFileSRC(
    __in MSIHANDLE hInstall
    )
{
    AssertSz(FALSE, "debug SchedXmlFile");

    HRESULT hr = S_OK;
    UINT er = ERROR_SUCCESS;

    LPWSTR pwzCurrentFile = NULL;
    BOOL fCurrentFileChanged = FALSE;
    BOOL fCurrentUseXPath = FALSE;

    PMSIHANDLE hView = NULL;
    PMSIHANDLE hRec = NULL;

    XML_FILE_CHANGE* pxfcHead = NULL;
    XML_FILE_CHANGE* pxfcTail = NULL;
    XML_FILE_CHANGE* pxfc = NULL;
    XML_FILE_CHANGE* pxfcUninstall = NULL;

    LPWSTR pwzCustomActionData = NULL;

    DWORD cFiles = 0;

    // initialize
    hr = WcaInitialize(hInstall, "SchedXmlFile");
    ExitOnFailure(hr, "failed to initialize");

    hr = ReadXmlFileTable(&pxfcHead, &pxfcTail);
    if (S_FALSE == hr)
    {
        WcaLog(LOGMSG_VERBOSE, "Skipping SchedXmlFile because XmlFile table not present");
        ExitFunction1(hr = S_OK);
    }

    MessageExitOnFailure(hr, msierrXmlFileFailedRead, "failed to read XmlFile table");

    // loop through all the xml configurations
    for (pxfc = pxfcHead; pxfc; pxfc = pxfc->pxfcNext)
    {
        // If this is the first file, a different file, the last file, or the SelectionLanguage property changes...
        if (NULL == pwzCurrentFile || 0 != lstrcmpW(pwzCurrentFile, pxfc->wzFile) || NULL == pxfc->pxfcNext || fCurrentUseXPath != ((XMLFILE_USE_XPATH & pxfc->iXmlFlags)))
        {
            // If this isn't the first file
            if (NULL != pwzCurrentFile)
            {
                // Do the uninstall work for the current file by walking backwards through the list (so the sequence is reversed)
                for (pxfcUninstall = ((NULL != pxfc->pxfcNext) ? pxfc->pxfcPrev : pxfc); pxfcUninstall && 0 == lstrcmpW(pwzCurrentFile, pxfcUninstall->wzFile) &&  fCurrentUseXPath == ((XMLFILE_USE_XPATH & pxfcUninstall->iXmlFlags)); pxfcUninstall = pxfcUninstall->pxfcPrev)
                {
                    // If it's being uninstalled
                    if (WcaIsUninstalling(pxfcUninstall->isInstalled, pxfcUninstall->isAction))
                    {
                        // Uninstall the change
                        if (!(XMLFILE_DONT_UNINSTALL & pxfcUninstall->iXmlFlags))
                        {
                            if (!fCurrentFileChanged)
                            {
                                hr = BeginChangeFile(pwzCurrentFile, pxfcUninstall, &pwzCustomActionData);
                                ExitOnFailure1(hr, "failed to begin file change for file: %S", pwzCurrentFile);

                                fCurrentFileChanged = TRUE;
                                cFiles++;
                            }
                            if (XMLFILE_CREATE_ELEMENT & pxfcUninstall->iXmlFlags)
                            {
                                hr = WcaWriteIntegerToCaData((int)xaDeleteElement, &pwzCustomActionData);
                                ExitOnFailure(hr, "failed to write delete element action indicator to custom action data");
                            }
                            else
                            {
                                hr = WcaWriteIntegerToCaData((int)xaDeleteValue, &pwzCustomActionData);
                                ExitOnFailure(hr, "failed to write delete value action indicator to custom action data");
                            }

                            if (XMLFILE_PRESERVE_MODIFIED & pxfc->iXmlFlags)
                            {
                                hr = WcaWriteIntegerToCaData((int)xdPreserve, &pwzCustomActionData);
                                ExitOnFailure(hr, "failed to write Preserve Date indicator to custom action data");
                            }
                            else
                            {
                                hr = WcaWriteIntegerToCaData((int)xdDontPreserve, &pwzCustomActionData);
                                ExitOnFailure(hr, "failed to write Don't Preserve Date indicator to custom action data");
                            }

                            hr = WriteChangeData(pxfcUninstall, &pwzCustomActionData);
                            ExitOnFailure(hr, "failed to write uninstall change data");
                        }
                    }
                }
            }

            // Remember the file we're currently working on
            hr = StrAllocString(&pwzCurrentFile, pxfc->wzFile, 0);
            ExitOnFailure1(hr, "failed to copy file name: %S", pxfc->wzFile);
            fCurrentUseXPath = (XMLFILE_USE_XPATH & pxfc->iXmlFlags);

            // We haven't changed the current file yet
            fCurrentFileChanged = FALSE;
        }

        // If it's being installed
        if (WcaIsInstalling(pxfc->isInstalled, pxfc->isAction))
        {
            if (!fCurrentFileChanged)
            {
                hr = BeginChangeFile(pwzCurrentFile, pxfc, &pwzCustomActionData);
                ExitOnFailure1(hr, "failed to begin file change for file: %S", pwzCurrentFile);
                fCurrentFileChanged = TRUE;
                cFiles++;
            }

            // Install the change
            if (XMLFILE_CREATE_ELEMENT & pxfc->iXmlFlags)
            {
                hr = WcaWriteIntegerToCaData((int)xaCreateElement, &pwzCustomActionData);
                ExitOnFailure(hr, "failed to write create element action indicator to custom action data");
            }
            else if (XMLFILE_DELETE_VALUE & pxfc->iXmlFlags)
            {
                hr = WcaWriteIntegerToCaData((int)xaDeleteValue, &pwzCustomActionData);
                ExitOnFailure(hr, "failed to write delete value action indicator to custom action data");
            }
            else if (XMLFILE_BULKWRITE_VALUE & pxfc->iXmlFlags)
            {
                hr = WcaWriteIntegerToCaData((int)xaBulkWriteValue, &pwzCustomActionData);
                ExitOnFailure(hr, "failed to write builkwrite value action indicator to custom action data");
            }
            else
            {
                hr = WcaWriteIntegerToCaData((int)xaWriteValue, &pwzCustomActionData);
                ExitOnFailure(hr, "failed to write file indicator to custom action data");
            }

            if (XMLFILE_PRESERVE_MODIFIED & pxfc->iXmlFlags)
            {
                hr = WcaWriteIntegerToCaData((int)xdPreserve, &pwzCustomActionData);
                ExitOnFailure(hr, "failed to write Preserve Date indicator to custom action data");
            }
            else
            {
                hr = WcaWriteIntegerToCaData((int)xdDontPreserve, &pwzCustomActionData);
                ExitOnFailure(hr, "failed to write Don't Preserve Date indicator to custom action data");
            }

            hr = WriteChangeData(pxfc, &pwzCustomActionData);
            ExitOnFailure(hr, "failed to write change data");
        }
    }

    // If we looped through all records all is well
    if (E_NOMOREITEMS == hr)
        hr = S_OK;
    ExitOnFailure(hr, "failed while looping through all objects to secure");

    // Schedule the custom action and add to progress bar
    if (pwzCustomActionData && *pwzCustomActionData)
    {
        Assert(0 < cFiles);

// COMMENT: {10/26/2009 10:41:20 PM}        hr = WcaDoDeferredAction(L"ExecXmlFile", pwzCustomActionData, cFiles * COST_XMLFILE);
// COMMENT: {10/26/2009 10:41:20 PM}        ExitOnFailure(hr, "failed to schedule ExecXmlFile action");
        hr = WcaDoDeferredAction(L"ExecXmlFileSRC", pwzCustomActionData, cFiles * COST_XMLFILE);
        ExitOnFailure(hr, "failed to schedule ExecXmlFileSRC action");
    }

LExit:
    ReleaseStr(pwzCurrentFile);
    ReleaseStr(pwzCustomActionData);

    if (FAILED(hr))
        er = ERROR_INSTALL_FAILURE;
    return WcaFinalize(er);
}

/////////{{{{
/********************************************************************
StrAnsiAllocString - allocates or reuses dynamic ANSI string memory and copies in an existing string

NOTE: caller is responsible for freeing ppsz even if function fails
NOTE: cchSource must equal the length of wzSource (not including the NULL terminator)
NOTE: if cchSource == 0, length of wzSource is used instead
********************************************************************/
extern "C" HRESULT DAPI StrAnsiAllocStringSRC(
    __deref_out_ecount_z(cchSource+1) LPSTR* ppsz,
    __in LPCWSTR wzSource,
    __in DWORD_PTR cchSource,
    __in UINT uiCodepage
    )
{
	WcaLog(LOGMSG_STANDARD, "WideCharToMultiByte IN");
    Assert(ppsz && wzSource);

	WcaLog(LOGMSG_STANDARD, "WideCharToMultiByte 1");
    HRESULT hr = S_OK;
    LPSTR psz = NULL;
    DWORD_PTR cch = 0;
    DWORD_PTR cchDest = cchSource; // at least enough

	WcaLog(LOGMSG_STANDARD, "WideCharToMultiByte 2");
    if (*ppsz)
    {
		WcaLog(LOGMSG_STANDARD, "WideCharToMultiByte 3");
        cch = MemSize(*ppsz);  // get the count in bytes so we can check if it failed (returns -1)
        if (-1 == cch)
            ExitOnFailure(hr = E_INVALIDARG, "failed to get size of destination string");
        cch /= sizeof(CHAR);  //convert the count in bytes to count in characters
    }

	WcaLog(LOGMSG_STANDARD, "WideCharToMultiByte 4");
    if (0 == cchSource)
    {
		WcaLog(LOGMSG_STANDARD, "WideCharToMultiByte 5");
        cchDest = ::WideCharToMultiByte(uiCodepage, 0, wzSource, -1, NULL, 0, NULL, NULL);
        if (0 == cchDest)
        {
			WcaLog(LOGMSG_STANDARD, "WideCharToMultiByte cchDest==0");
            ExitWithLastError1(hr, "failed to get required size for conversion to ANSI: %S", wzSource);
        }

        --cchDest; // subtract one because WideChageToMultiByte includes space for the NULL terminator that we track below
		WcaLog(LOGMSG_STANDARD, "WideCharToMultiByte 6");
    }
    else if (L'\0' == wzSource[cchSource]) // if the source already had a null terminator, don't count that in the character count because we track it below
    {
        cchDest = cchSource - 1;
		WcaLog(LOGMSG_STANDARD, "WideCharToMultiByte 7");
    }
	WcaLog(LOGMSG_STANDARD, "WideCharToMultiByte 8");
    if (cch < cchDest + 1)
    {
		WcaLog(LOGMSG_STANDARD, "WideCharToMultiByte 9");
        cch = cchDest + 1;   // add one for the NULL terminator
        if (cch >= MAXDWORD / sizeof(WCHAR))
        {
            ExitOnFailure1(hr = E_OUTOFMEMORY, "Not enough memory to allocate string of size: %d", cch);
        }
		WcaLog(LOGMSG_STANDARD, "WideCharToMultiByte 10");

        if (*ppsz)
        {
            psz = static_cast<LPSTR>(MemReAlloc(*ppsz, sizeof(CHAR) * cch, TRUE));
        }
        else
        {
            psz = static_cast<LPSTR>(MemAlloc(sizeof(CHAR) * cch, TRUE));
        }
		WcaLog(LOGMSG_STANDARD, "WideCharToMultiByte 11");
        ExitOnNull1(psz, hr, E_OUTOFMEMORY, "failed to allocate string, len: %d", cch);

        *ppsz = psz;
    }
	WcaLog(LOGMSG_STANDARD, "WideCharToMultiByte 12");

    if (0 == ::WideCharToMultiByte(uiCodepage, 0, wzSource, 0 == cchSource ? -1 : (int)cchSource, *ppsz, (int)cch, NULL, NULL))
    {
		WcaLog(LOGMSG_STANDARD, "WideCharToMultiByte==0");
        ExitWithLastError1(hr, "failed to convert to ansi: %S", wzSource);
    }
    (*ppsz)[cchDest] = L'\0';
	WcaLog(LOGMSG_STANDARD, "WideCharToMultiByte 13");

LExit:
	WcaLog(LOGMSG_STANDARD, "WideCharToMultiByte OUT");
    return hr;
}
/////////}}}}


/******************************************************************
 ExecXmlFile - entry point for XmlFile Custom Action

*******************************************************************/
extern "C" UINT __stdcall ExecXmlFileSRC(
    __in MSIHANDLE hInstall
    )
{
    AssertSz(FALSE, "debug ExecXmlFile");
    HRESULT hr = S_OK;
    HRESULT hrOpenFailure = S_OK;
    UINT er = ERROR_SUCCESS;

    BOOL fWow64APIPresent = FALSE;
    BOOL fIsFSRedirectDisabled = FALSE;
    BOOL fPreserveDate = FALSE;

    int id = IDRETRY;

    LPWSTR pwzCustomActionData = NULL;
    LPWSTR pwzData = NULL;
    LPWSTR pwzFile = NULL;
    LPWSTR pwzXPath = NULL;
    LPWSTR pwzName = NULL;
    LPWSTR pwzHeader = NULL;
    LPWSTR pwz = NULL;

    IXMLDOMDocument* pixd = NULL;
    IXMLDOMNode* pixn = NULL;
    IXMLDOMNode* pixnNewNode = NULL;
    IXMLDOMNodeList* pixNodes = NULL;
    IXMLDOMDocument2 *pixdDocument2 = NULL;

    FILETIME ft;

    BSTR bstrProperty = ::SysAllocString(L"SelectionLanguage");
    ExitOnNull(bstrProperty, hr, E_OUTOFMEMORY, "failed SysAllocString");
    VARIANT varValue;
    ::VariantInit(&varValue);
    varValue.vt = VT_BSTR;
    varValue.bstrVal = ::SysAllocString(L"XPath");
    ExitOnNull(varValue.bstrVal, hr, E_OUTOFMEMORY, "failed SysAllocString");
    eXmlAction xa;
    eXmlPreserveDate xd;
    eXmlSelectionLanguage xl;

    // initialize
    hr = WcaInitialize(hInstall, "ExecXmlFile");
    ExitOnFailure(hr, "failed to initialize");

    hr = XmlInitialize();
    ExitOnFailure(hr, "failed to initialize xml utilities");

    hr = WcaGetProperty( L"CustomActionData", &pwzCustomActionData);
    ExitOnFailure(hr, "failed to get CustomActionData");

    WcaLog(LOGMSG_TRACEONLY, "CustomActionData: %S", pwzCustomActionData);

    pwz = pwzCustomActionData;

    hr = WcaReadIntegerFromCaData(&pwz, (int*) &xa);
    ExitOnFailure(hr, "failed to process CustomActionData");

    // Initialize the Wow64 API - store the result in fWow64APIPresent
    // If it fails, this doesn't warrant an error yet, because we only need the Wow64 API in some cases
    WcaInitializeWow64();
    fWow64APIPresent = WcaIsWow64Initialized();

    if (xaOpenFile != xa && xaOpenFilex64 != xa)
        ExitOnFailure(hr = E_INVALIDARG, "invalid custom action data");

    // loop through all the passed in data
    while (pwz && *pwz)
    {
        hr = WcaReadIntegerFromCaData(&pwz, (int*) &xl);
        ExitOnFailure(hr, "failed to process CustomActionData");

        hr = WcaReadStringFromCaData(&pwz, &pwzFile);
        ExitOnFailure(hr, "failed to read file name from custom action data");

        // Default to not preserve the modified date
        fPreserveDate = FALSE;

        // Open the file
        ReleaseNullObject(pixd);

        if (xaOpenFilex64 == xa)
        {
            if (!fWow64APIPresent)
            {
                hr = E_NOTIMPL;
                ExitOnFailure(hr, "Custom action was told to act on a 64-bit component, but the Wow64 API is unavailable.");
            }

            hr = WcaDisableWow64FSRedirection();
            ExitOnFailure(hr, "Custom action was told to act on a 64-bit component, but was unable to disable filesystem redirection through the Wow64 API.");

            fIsFSRedirectDisabled = TRUE;
        }

        hr = XmlLoadDocumentFromFileEx(pwzFile, XML_LOAD_PRESERVE_WHITESPACE, &pixd);
        if (FAILED(hr))
        {
            // Ignore the return code for now.  If they try to add something, we'll fail the install.  If all they do is remove stuff then it doesn't matter.
            hrOpenFailure = hr;
            hr = S_OK;
        }
        else
        {
            hrOpenFailure = S_OK;
        }
        WcaLog(LOGMSG_VERBOSE, "Configuring Xml File: %S", pwzFile);
		WcaLog(LOGMSG_STANDARD, "Configuring Xml File: %S", pwzFile);

        if (xsXPath == xl)
        {
            if (vfMsxml30)
            {
                // If we failed to open the file, don't fail immediately; just skip setting the selection language, and we'll fail later if appropriate
                if (SUCCEEDED(hrOpenFailure))
                {
                    hr = pixd->QueryInterface(XmlUtil_IID_IXMLDOMDocument2, (void**)&pixdDocument2);
                    ExitOnFailure(hr, "failed in querying IXMLDOMDocument2 interface");
                    hr = pixdDocument2->setProperty(bstrProperty, varValue);
                    ExitOnFailure(hr, "failed in setting SelectionLanguage");
                }
            }
            else
            {
                ExitOnFailure(E_NOTIMPL, "current MSXML version does not support xpath query");
            }
        }

        while (pwz && *pwz)
        {
            hr = WcaReadIntegerFromCaData(&pwz, (int*) &xa);
            ExitOnFailure(hr, "failed to process CustomActionData");

            // Break if we need to move on to a different file
            if (xaOpenFile == xa || xaOpenFilex64 == xa)
                break;

            hr = WcaReadIntegerFromCaData(&pwz, (int*) &xd);
            ExitOnFailure(hr, "failed to process CustomActionData");

            if (xdPreserve == xd)
            {
                fPreserveDate = TRUE;
            }

            // Get path, name, and value to be written
            hr = WcaReadStringFromCaData(&pwz, &pwzXPath);
            ExitOnFailure(hr, "failed to process CustomActionData");
            hr = WcaReadStringFromCaData(&pwz, &pwzName);
            ExitOnFailure(hr, "failed to process CustomActionData");
            hr = WcaReadStringFromCaData(&pwz, &pwzHeader);
            ExitOnFailure(hr, "failed to process CustomActionData");

            // If we failed to open the file and we're adding something to the file, we've got a problem.  Otherwise, just continue on since the file's already gone.
            if (FAILED(hrOpenFailure))
            {
                if (xaCreateElement == xa || xaWriteValue == xa || xaBulkWriteValue == xa)
                {
                    MessageExitOnFailure1(hr = hrOpenFailure, msierrXmlFileFailedOpen, "failed to load XML file: %S", pwzFile);
                }
                else
                {
                    continue;
                }
            }

            // Select the node we're about to modify
            ReleaseNullObject(pixn);

            if (xaBulkWriteValue == xa)
            {
                hr = XmlSelectNodes(pixd, pwzXPath, &pixNodes);
                if (S_FALSE == hr)
                {
                    hr = HRESULT_FROM_WIN32(ERROR_OBJECT_NOT_FOUND);
                }

                MessageExitOnFailure2(hr, msierrXmlFileFailedSelect, "failed to find any nodes: %S in XML file: %S", pwzXPath, pwzFile);
                for(;;)
                {
                    pixNodes->nextNode(&pixn);
                    if (NULL == pixn)
                        break;

                    if (pwzName && *pwzName)
                    {
                        // We're setting an attribute
                        hr = XmlSetAttribute(pixn, pwzName, pwzHeader);
                        ExitOnFailure2(hr, "failed to set attribute: %S to value %S", pwzName, pwzHeader);
                    }
                    else
                    {
                        // We're setting the text of the node
                        hr = XmlSetText(pixn, pwzHeader);
                        ExitOnFailure2(hr, "failed to set text to: %S for element %S.  Make sure that XPath points to an element.", pwzHeader, pwzXPath);
                    }
                    ReleaseNullObject(pixn);
                }
            }
            else 
            {
                hr = XmlSelectSingleNode(pixd, pwzXPath, &pixn);
                if (S_FALSE == hr)
                    hr = HRESULT_FROM_WIN32(ERROR_OBJECT_NOT_FOUND);
                MessageExitOnFailure2(hr, msierrXmlFileFailedSelect, "failed to find node: %S in XML file: %S", pwzXPath, pwzFile);

                // Make the modification
                if (xaWriteValue == xa)
                {
                    if (pwzName && *pwzName)
                    {
                        // We're setting an attribute
                        hr = XmlSetAttribute(pixn, pwzName, pwzHeader);
                        ExitOnFailure2(hr, "failed to set attribute: %S to value %S", pwzName, pwzHeader);
                    }
                    else
                    {
                        // We're setting the text of the node
                        hr = XmlSetText(pixn, pwzHeader);
                        ExitOnFailure2(hr, "failed to set text to: %S for element %S.  Make sure that XPath points to an element.", pwzHeader, pwzXPath);
                    }
                }
                else if (xaCreateElement == xa)
                {
                    hr = XmlCreateChild(pixn, pwzName, &pixnNewNode);
                    ExitOnFailure1(hr, "failed to create child element: %S", pwzName);

                    if (pwzHeader && *pwzHeader)
                    {
                        hr = XmlSetText(pixnNewNode, pwzHeader);
                        ExitOnFailure2(hr, "failed to set text to: %S for node: %S", pwzHeader, pwzName);
                    }

                    ReleaseNullObject(pixnNewNode);
                }
                else if (xaDeleteValue == xa)
                {
                    if (pwzName && *pwzName)
                    {
                        // Delete the attribute
                        hr = XmlRemoveAttribute(pixn, pwzName);
                        ExitOnFailure1(hr, "failed to remove attribute: %S", pwzName);
                    }
                    else
                    {
                        // Clear the text value for the node
                        hr = XmlSetText(pixn, L"");
                        ExitOnFailure(hr, "failed to clear text value");
                    }
                }
                else if (xaDeleteElement == xa)
                {
                    // TODO: This may be a little heavy handed
                    hr = XmlRemoveChildren(pixn, pwzName);
                    ExitOnFailure1(hr, "failed to delete child node: %S", pwzName);
                }
                else
                {
                    ExitOnFailure(hr = E_UNEXPECTED, "Invalid modification specified in custom action data");
                }
            }
        }

        // Now that we've made all of the changes to this file, save it and move on to the next
        if (S_OK == hrOpenFailure)
        {
            if (fPreserveDate)
            {
                hr = FileGetTime(pwzFile, NULL, NULL, &ft);
                ExitOnFailure1(hr, "failed to get modified time of file : %S", pwzFile);
            }

            do
            {
// COMMENT: {10/27/2009 7:13:35 PM}                hr = XmlSaveDocument(pixd, pwzFile);
				//{{
// COMMENT: {10/27/2009 7:13:35 PM}				if (FAILED(hr))
// COMMENT: {10/27/2009 7:13:35 PM}				{
// COMMENT: {10/27/2009 7:13:35 PM}					WcaLog(LOGMSG_STANDARD, "XmlSaveDocument FAILED", pwzFile);
// COMMENT: {10/27/2009 7:13:35 PM}				}
// COMMENT: {10/27/2009 7:13:35 PM}				else
				{
// COMMENT: {10/27/2009 7:13:51 PM}					WcaLog(LOGMSG_STANDARD, "XmlSaveDocument SUCCEEDED", pwzFile);

					LPBYTE pbData = NULL;
					DWORD cbData = 0;

					hr = FileRead(&pbData, &cbData, pwzFile);
					ExitOnFailure1(hr, "failed to read file: %S", pwzFile);
					WcaLog(LOGMSG_STANDARD, "FileRead(%S) - OK cbData=%d", pwzFile, cbData);

					// Open the file
				    HANDLE hFile = INVALID_HANDLE_VALUE;
					hFile = ::CreateFileW(pwzFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
					ExitOnInvalidHandleWithLastError1(hFile, hr, "Failed to open file: %S", pwzFile);
					WcaLog(LOGMSG_STANDARD, "CreateFileW(%S) - OK", pwzFile);

					// pwzHeader
					WcaLog(LOGMSG_STANDARD, "Attempting to convert = \"%S\"", pwzHeader);

					LPSTR pzHeader = NULL;
					/*
					hr = StrAnsiAllocString(&pzValue, pwzHeader, 0, CP_ACP);
					ExitOnFailure(hr, "failed to convert to ANSI code page");
					*/

					hr = StrAnsiAllocString(&pzHeader, pwzHeader, 0, CP_ACP);
					ExitOnFailure(hr, "failed to convert output to ANSI");
					WcaLog(LOGMSG_STANDARD, pzHeader);

					LPSTR pzLine = NULL;
					hr = StrAnsiAllocFormatted(&pzLine, "%s\n", pzHeader);
					ExitOnFailure(hr, "StrAnsiAllocFormatted failed");
					WcaLog(LOGMSG_STANDARD, "StrAnsiAllocFormatted OK = \"%s\"", pzHeader);

				    DWORD cbTotal = 0;
					DWORD cbDataWritten = 0;
					size_t len;
					StringCchLengthA(pzLine, STRSAFE_MAX_CCH, &len);
					cbTotal = len;
					if (!::WriteFile(hFile, pzLine, cbTotal, &cbDataWritten, NULL))
					{
						ExitOnLastError1(hr, "Failed to write to file: %S", pwzFile);
					}
					WcaLog(LOGMSG_STANDARD, "WriteFile(\"%s\") - OK", pzLine);

					// Write out the old data
					cbTotal = 0;
					while (cbTotal < cbData)
					{
						if (!::WriteFile(hFile, pbData + cbTotal, cbData - cbTotal, &cbDataWritten, NULL))
						{
							ExitOnLastError1(hr, "Failed to write to file: %S", pwzFile);
						}
						cbTotal += cbDataWritten;
					}
					ReleaseFile(hFile);
					ReleaseMem(pbData);
				}
				//}}
                if (FAILED(hr))
                {
                    id = WcaErrorMessage(msierrXmlConfigFailedSave, hr, INSTALLMESSAGE_ERROR | MB_ABORTRETRYIGNORE, 1, pwzFile);
                    switch (id)
                    {
                    case IDABORT:
                        ExitOnFailure1(hr, "Failed to save changes to XML file: %S", pwzFile);
                    case IDRETRY:
                        hr = S_FALSE;   // hit me, baby, one more time
                        break;
                    case IDIGNORE:
                        hr = S_OK;  // pretend everything is okay and bail
                        break;
                    }
                }
            } while (S_FALSE == hr);

            if (fPreserveDate)
            {
                hr = FileSetTime(pwzFile, NULL, NULL, &ft);
                ExitOnFailure1(hr, "failed to set modified time of file : %S", pwzFile);
            }

            if (fIsFSRedirectDisabled)
            {
                fIsFSRedirectDisabled = FALSE;
                WcaRevertWow64FSRedirection();
            }
        }
    }

LExit:
    // Make sure we revert FS Redirection if necessary before exiting
    if (fIsFSRedirectDisabled)
    {
        fIsFSRedirectDisabled = FALSE;
        WcaRevertWow64FSRedirection();
    }
    WcaFinalizeWow64();

    ReleaseStr(pwzCustomActionData);
    ReleaseStr(pwzData);
    ReleaseStr(pwzFile);
    ReleaseStr(pwzXPath);
    ReleaseStr(pwzName);
    ReleaseStr(pwzHeader);
    ReleaseBSTR(bstrProperty);
    ReleaseVariant(varValue);

    ReleaseObject(pixdDocument2);
    ReleaseObject(pixn);
    ReleaseObject(pixd);
    ReleaseObject(pixnNewNode);
    ReleaseObject(pixNodes);

    XmlUninitialize();

    if (FAILED(hr))
        er = ERROR_INSTALL_FAILURE;
    return WcaFinalize(er);
}


/******************************************************************
 ExecXmlFileRollback - entry point for XmlFile rollback Custom Action

*******************************************************************/
extern "C" UINT __stdcall ExecXmlFileRollbackSRC(
    __in MSIHANDLE hInstall
    )
{
    AssertSz(FALSE, "debug ExecXmlFileRollback");
    HRESULT hr = S_OK;
    UINT er = ERROR_SUCCESS;

    int iIs64Bit;
    BOOL fIs64Bit = FALSE;

    LPWSTR pwzCustomActionData = NULL;
    LPWSTR pwz = NULL;
    LPWSTR pwzFileName = NULL;
    LPBYTE pbData = NULL;
    DWORD_PTR cbData = 0;
    DWORD cbDataWritten = 0;

    FILETIME ft;

    HANDLE hFile = INVALID_HANDLE_VALUE;

    // initialize
    hr = WcaInitialize(hInstall, "ExecXmlFileRollback");
    ExitOnFailure(hr, "failed to initialize");


    hr = WcaGetProperty( L"CustomActionData", &pwzCustomActionData);
    ExitOnFailure(hr, "failed to get CustomActionData");

    WcaLog(LOGMSG_TRACEONLY, "CustomActionData: %S", pwzCustomActionData);

    pwz = pwzCustomActionData;

    hr = WcaReadIntegerFromCaData(&pwz, &iIs64Bit);
    ExitOnFailure(hr, "failed to read component bitness from custom action data");

    hr = WcaReadStringFromCaData(&pwz, &pwzFileName);
    ExitOnFailure(hr, "failed to read file name from custom action data");

    hr = WcaReadStreamFromCaData(&pwz, &pbData, &cbData);
    ExitOnFailure(hr, "failed to read file contents from custom action data");

    fIs64Bit = (BOOL)iIs64Bit;

    if (fIs64Bit)
    {
        WcaInitializeWow64();
        ExitOnFailure(hr, "failed to initialize Wow64 API");

        if (!WcaIsWow64Initialized())
        {
            hr = E_NOTIMPL;
            ExitOnFailure(hr, "Custom action was told to rollback a 64-bit component, but the Wow64 API is unavailable.");
        }

        hr = WcaDisableWow64FSRedirection();
        ExitOnFailure(hr, "Custom action was told to rollback a 64-bit component, but was unable to Disable Filesystem Redirection through the Wow64 API.");
    }

    // Always preserve the modified date on rollback
    hr = FileGetTime(pwzFileName, NULL, NULL, &ft);
    ExitOnFailure1(hr, "Failed to get modified date of file %S.", pwzFileName);

    // Open the file
    hFile = ::CreateFileW(pwzFileName, GENERIC_WRITE, NULL, NULL, TRUNCATE_EXISTING, NULL, NULL);
    ExitOnInvalidHandleWithLastError1(hFile, hr, "failed to open file: %S", pwzFileName);

    // Write out the old data
    if (!::WriteFile(hFile, pbData, (DWORD)cbData, &cbDataWritten, NULL))
        ExitOnLastError1(hr, "failed to write to file: %S", pwzFileName);

    Assert(cbData == cbDataWritten);

    ReleaseFile(hFile);

    // Always preserve the modified date on rollback
    hr = FileSetTime(pwzFileName, NULL, NULL, &ft);
    ExitOnFailure1(hr, "Failed to set modified date of file %S.", pwzFileName);

LExit:
    ReleaseStr(pwzCustomActionData);
    ReleaseStr(pwzFileName);

    ReleaseFile(hFile);

    if (fIs64Bit)
    {
        WcaRevertWow64FSRedirection();
        WcaFinalizeWow64();
    }

    ReleaseMem(pbData);

    if (FAILED(hr))
        er = ERROR_INSTALL_FAILURE;
    return WcaFinalize(er);
}

