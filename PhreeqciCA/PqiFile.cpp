#include "stdafx.h"

#define LOGMSG_VERBOSE   (LOGLEVEL)2
#define LOGMSG_TRACEONLY (LOGLEVEL)2

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
};

enum eXmlPreserveDate
{
    xdDontPreserve = 0,
    xdPreserve
};

LPCWSTR vcsXmlFileQuery =
    L"SELECT `PqiFile`.`Id`, `PqiFile`.`File`, `PqiFile`.`Header`, "
    L"`PqiFile`.`Component_`, `Component`.`Attributes` "
    L"FROM `PqiFile`,`Component` WHERE `PqiFile`.`Component_`=`Component`.`Component` ORDER BY `File`";
enum eXmlFileQuery { xfqXmlFile = 1, xfqFile, xfqHeader, xfqComponent, xfqCompAttributes  };

struct XML_FILE_CHANGE
{
    WCHAR wzId[MAX_DARWIN_KEY];

    INSTALLSTATE isInstalled;
    INSTALLSTATE isAction;

    WCHAR wzFile[MAX_PATH];
    LPWSTR pwzHeader;

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
    if (S_FALSE == WcaTableExists(L"PqiFile"))
        ExitFunction1(hr = S_FALSE);

    // loop through all the xml configurations
    hr = WcaOpenExecuteView(vcsXmlFileQuery, &hView);
    ExitOnFailure(hr, "failed to open view on PqiFile table");

    while (S_OK == (hr = WcaFetchRecord(hView, &hRec)))
    {
        hr = AddXmlFileChangeToList(ppxfcHead, ppxfcTail);
        ExitOnFailure(hr, "failed to add xml file change to list");

        // Get record Id
        hr = WcaGetRecordString(hRec, xfqXmlFile, &pwzData);
        ExitOnFailure(hr, "failed to get PqiFile record Id");
        hr = StringCchCopyW((*ppxfcTail)->wzId, countof((*ppxfcTail)->wzId), pwzData);
        ExitOnFailure(hr, "failed to copy PqiFile record Id");

        // Get component name
        hr = WcaGetRecordString(hRec, xfqComponent, &pwzData);
        ExitOnFailure1(hr, "failed to get component name for PqiFile: %S", (*ppxfcTail)->wzId);

        // Get the component's state
        er = ::MsiGetComponentStateW(WcaGetInstallHandle(), pwzData, &(*ppxfcTail)->isInstalled, &(*ppxfcTail)->isAction);
        ExitOnFailure1(hr = HRESULT_FROM_WIN32(er), "failed to get install state for Component: %S", pwzData);

        // Get the xml file
        hr = WcaGetRecordFormattedString(hRec, xfqFile, &pwzData);
        ExitOnFailure1(hr, "failed to get xml file for PqiFile: %S", (*ppxfcTail)->wzId);
        hr = StringCchCopyW((*ppxfcTail)->wzFile, countof((*ppxfcTail)->wzFile), pwzData);
        ExitOnFailure1(hr, "failed to copy xml file: %S", (*ppxfcTail)->wzFile);

        // Get the header
        hr = WcaGetRecordFormattedString(hRec, xfqHeader, &pwzData);
        ExitOnFailure1(hr, "failed to get Header for PqiFile: %S", (*ppxfcTail)->wzId);
        hr = StrAllocString(&(*ppxfcTail)->pwzHeader, pwzData, 0);
        ExitOnFailure1(hr, "failed to allocate buffer for Header: %S", (*ppxfcTail)->pwzHeader);

        // Get the component attributes
        hr = WcaGetRecordInteger(hRec, xfqCompAttributes, &(*ppxfcTail)->iCompAttributes);
        ExitOnFailure1(hr, "failed to get component attributes for PqiFile: %S", (*ppxfcTail)->wzId);
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
    LPBYTE pbData = NULL;
    DWORD cbData = 0;

    LPWSTR pwzRollbackCustomActionData = NULL;

    hr = WcaWriteStringToCaData(pwzFile, ppwzCustomActionData);
    ExitOnFailure1(hr, "failed to write file to custom action data: %S", pwzFile);
	WcaLog(LOGMSG_STANDARD, "WcaWriteStringToCaData(%S) - OK", pwzFile);

    // If the file already exits, then we have to put it back the way it was on failure
    if (FileExistsEx(pwzFile, NULL))
    {
		WcaLog(LOGMSG_STANDARD, "FileExistsEx(%S) - TRUE", pwzFile);

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
	else
	{
		WcaLog(LOGMSG_STANDARD, "FileExistsEx(%S) - FALSE", pwzFile);
	}
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

    hr = WcaWriteStringToCaData(pxfc->pwzHeader, ppwzCustomActionData);
    ExitOnFailure1(hr, "failed to write Header to custom action data: %S", pxfc->pwzHeader);
	WcaLog(LOGMSG_STANDARD, "WriteChangeData:WcaWriteStringToCaData(pwzHeader = %S)", pxfc->pwzHeader);
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
        WcaLog(LOGMSG_VERBOSE, "Skipping SchedXmlFile because PqiFile table not present");
        ExitFunction1(hr = S_OK);
    }

    MessageExitOnFailure(hr, msierrXmlFileFailedRead, "failed to read PqiFile table");

    // loop through all the xml configurations
    for (pxfc = pxfcHead; pxfc; pxfc = pxfc->pxfcNext)
    {
        // If this is the first file, a different file, the last file, or the SelectionLanguage property changes...
        if (NULL == pwzCurrentFile || 0 != lstrcmpW(pwzCurrentFile, pxfc->wzFile) || NULL == pxfc->pxfcNext /* || fCurrentUseXPath != ((XMLFILE_USE_XPATH & pxfc->iXmlFlags)) */)
        {
            // If this isn't the first file
            if (NULL != pwzCurrentFile)
            {
                // Do the uninstall work for the current file by walking backwards through the list (so the sequence is reversed)
                for (pxfcUninstall = ((NULL != pxfc->pxfcNext) ? pxfc->pxfcPrev : pxfc); pxfcUninstall && 0 == lstrcmpW(pwzCurrentFile, pxfcUninstall->wzFile) /* &&  fCurrentUseXPath == ((XMLFILE_USE_XPATH & pxfcUninstall->iXmlFlags)) */; pxfcUninstall = pxfcUninstall->pxfcPrev)
                {
                    // If it's being uninstalled
                    if (WcaIsUninstalling(pxfcUninstall->isInstalled, pxfcUninstall->isAction))
                    {
                    }
                }
            }

            // Remember the file we're currently working on
            hr = StrAllocString(&pwzCurrentFile, pxfc->wzFile, 0);
            ExitOnFailure1(hr, "failed to copy file name: %S", pxfc->wzFile);

            // We haven't changed the current file yet
            fCurrentFileChanged = FALSE;
        }

        // If it's being installed
        if (WcaIsInstalling(pxfc->isInstalled, pxfc->isAction))
        {
            if (!fCurrentFileChanged)
            {
				WcaLog(LOGMSG_STANDARD, "Calling BeginChangeFile");
                hr = BeginChangeFile(pwzCurrentFile, pxfc, &pwzCustomActionData);
                ExitOnFailure1(hr, "failed to begin file change for file: %S", pwzCurrentFile);
                fCurrentFileChanged = TRUE;
                cFiles++;
            }

            {
				// Note: Always preserve the original file date/time so that
				// windows installer will overwrite the file on repair.
                hr = WcaWriteIntegerToCaData((int)xdPreserve, &pwzCustomActionData);
                ExitOnFailure(hr, "failed to write Preserve Date indicator to custom action data");
				WcaLog(LOGMSG_STANDARD, "WcaWriteIntegerToCaData(xdPreserve) = %d SUCCESS ", xdPreserve);
            }

            hr = WriteChangeData(pxfc, &pwzCustomActionData);
            ExitOnFailure(hr, "failed to write change data");
			WcaLog(LOGMSG_STANDARD, "WriteChangeData SUCCESS ");
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
    LPWSTR pwzHeader = NULL;
    LPWSTR pwz = NULL;

    FILETIME ft;

    eXmlPreserveDate xd;

    // initialize
    hr = WcaInitialize(hInstall, "ExecXmlFile");
    ExitOnFailure(hr, "failed to initialize");

    hr = WcaGetProperty( L"CustomActionData", &pwzCustomActionData);
    ExitOnFailure(hr, "failed to get CustomActionData");

    WcaLog(LOGMSG_TRACEONLY, "CustomActionData: %S", pwzCustomActionData);

    pwz = pwzCustomActionData;

    // Initialize the Wow64 API - store the result in fWow64APIPresent
    // If it fails, this doesn't warrant an error yet, because we only need the Wow64 API in some cases
    WcaInitializeWow64();
    fWow64APIPresent = WcaIsWow64Initialized();
	if (fWow64APIPresent)
	{
		WcaLog(LOGMSG_STANDARD, "fWow64APIPresent=TRUE");
	}
	else
	{
		WcaLog(LOGMSG_STANDARD, "fWow64APIPresent=FALSE");
	}

// COMMENT: {10/30/2009 9:18:24 PM}    if (xaOpenFile != xa && xaOpenFilex64 != xa)
// COMMENT: {10/30/2009 9:18:24 PM}        ExitOnFailure(hr = E_INVALIDARG, "invalid custom action data");

    // loop through all the passed in data
    while (pwz && *pwz)
    {
        hr = WcaReadStringFromCaData(&pwz, &pwzFile);
        ExitOnFailure(hr, "failed to read file name from custom action data");
		WcaLog(LOGMSG_STANDARD, "WcaReadStringFromCaData successful pwzFile=%S", pwzFile);

        // Default to not preserve the modified date
        fPreserveDate = FALSE;

        if (pwz && *pwz)
        {
            hr = WcaReadIntegerFromCaData(&pwz, (int*) &xd);
            ExitOnFailure(hr, "failed to process CustomActionData");
			WcaLog(LOGMSG_STANDARD, "WcaReadIntegerFromCaData successful xd=%d", xd);

            if (xdPreserve == xd)
            {
                fPreserveDate = TRUE;
				WcaLog(LOGMSG_STANDARD, "xdPreserve == xd");
            }
			else
			{
				WcaLog(LOGMSG_STANDARD, "xdPreserve != xd");
			}
            hr = WcaReadStringFromCaData(&pwz, &pwzHeader);
            ExitOnFailure(hr, "failed to process CustomActionData");
			WcaLog(LOGMSG_STANDARD, "pwzHeader = %S", pwzHeader);

        }

        // Now that we've made all of the changes to this file, save it and move on to the next
        if (S_OK == hrOpenFailure)
        {
			WcaLog(LOGMSG_STANDARD, "S_OK == hrOpenFailure");
            if (fPreserveDate)
            {
                hr = FileGetTime(pwzFile, NULL, NULL, &ft);
                ExitOnFailure1(hr, "failed to get modified time of file : %S", pwzFile);
				WcaLog(LOGMSG_STANDARD, "FileGetTime(%S) SUCCESS ", pwzFile);
            }

            do
            {
				{
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
					hr = StrAnsiAllocString(&pzHeader, pwzHeader, 0, CP_ACP);
					ExitOnFailure(hr, "failed to convert output to ANSI");
					WcaLog(LOGMSG_STANDARD, pzHeader);

					LPSTR pzLine = NULL;
					hr = StrAnsiAllocFormatted(&pzLine, "%s\r\n", pzHeader);
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
		else
		{
			WcaLog(LOGMSG_STANDARD, "S_OK != hrOpenFailure");
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
    ReleaseStr(pwzHeader);

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

