// RichDocInPages.cpp : implementation file
//
// $Date: 2001/09/14 20:12:04 $
// $Revision: 1.1.1.27 $
// $Name: R_2_10 $
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "RichDocInPages.h"

#include "phreeqci2.h"

#include "Util.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CRichDocInPage1, baseRichDocInPage1)


/////////////////////////////////////////////////////////////////////////////
// CRichDocInPage1 property page

CRichDocInPage1::CRichDocInPage1() : baseRichDocInPage1(CRichDocInPage1::IDD)
{
	//{{AFX_DATA_INIT(CRichDocInPage1)
	m_bSetAsDefault = FALSE;
	//}}AFX_DATA_INIT

	// hide Help button
	m_psp.dwFlags &= ~PSP_HASHELP;
}

CRichDocInPage1::~CRichDocInPage1()
{
}

void CRichDocInPage1::DoDataExchange(CDataExchange* pDX)
{
	baseRichDocInPage1::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CRichDocInPage1)
	DDX_Control(pDX, IDC_SET_AS_DEFAULT, m_btnSetAsDefault);
	DDX_Control(pDX, IDC_CB_DB, m_cboDatabase);
	DDX_Check(pDX, IDC_SET_AS_DEFAULT, m_bSetAsDefault);
	//}}AFX_DATA_MAP


	//
	// File name:
	//
	if (!pDX->m_bSaveAndValidate && m_bFirstSetActive)
	{
		//{{
		DDX_Text(pDX, IDC_E_TITLE, m_strTitle);
		//}}
		if (m_strPathName.IsEmpty())
		{
// COMMENT: {12/14/2000 10:12:35 PM}			DDX_Text(pDX, IDC_E_FILENAME, m_strTitle);
		}
		else
		{
			//{{
			TCHAR szOutput[_MAX_PATH];
			TCHAR drive[_MAX_DRIVE];
			TCHAR dir[_MAX_DIR];
			TCHAR fname[_MAX_FNAME];
			TCHAR ext[_MAX_EXT];
			TCHAR szBuff[_MAX_PATH];

			_tcscpy(szBuff, m_strPathName);

			_tsplitpath(szBuff, drive, dir, fname, ext);
			_tmakepath(szOutput, drive, dir, NULL, NULL);
			CString strLocation = szOutput;
			strLocation.TrimRight(_T('\\'));
			DDX_Text(pDX, IDC_E_FILENAME, strLocation);
			//}}
// COMMENT: {12/14/2000 10:05:31 PM}			DDX_Text(pDX, IDC_E_FILENAME, m_strPathName);
		}
	}

	//
	// Output file name:
	//
	DDX_Text(pDX, IDC_E_OUTPUT, m_props.m_strOutPathName);

	//
	// Database file name:
	//
	if (pDX->m_bSaveAndValidate)
	{
		// save database
		ASSERT(m_cboDatabase.GetCurSel() != CB_ERR);
		m_cboDatabase.GetLBText(m_cboDatabase.GetCurSel(), m_props.m_strDBPathName);
		ASSERT(!m_props.m_strDBPathName.IsEmpty());
	}
	else
	{
		if (m_bFirstSetActive)
		{
			// fill database combo
			m_cboDatabase.ResetContent();
			CRecentFileList* pList = ((CPhreeqciApp*)AfxGetApp())->m_pRecentDBFileList;
			CString strPathName;
			for (int i = 0; i < pList->GetSize(); ++i)
			{
				strPathName = (*pList)[i];
				if ( !(strPathName.IsEmpty()) && CUtil::FileExists(strPathName) )
					m_cboDatabase.AddString(strPathName);
			}

			// set Set as default checkbox
			UpdateSetAsDefault();
		}

		// set database combo
		ASSERT( !m_props.m_strDBPathName.IsEmpty() );
		int nFind = m_cboDatabase.FindString(-1, m_props.m_strDBPathName);
		if (nFind == CB_ERR)
		{
			// database not in combo so add to list
			CRecentFileList* pList = ((CPhreeqciApp*)AfxGetApp())->m_pRecentDBFileList;
			nFind = m_cboDatabase.AddString(m_props.m_strDBPathName);
			pList->Add(m_props.m_strDBPathName);
		}
		m_cboDatabase.SetCurSel(nFind);
	}

	//
	// Size:
	//
	if (!pDX->m_bSaveAndValidate && m_bFirstSetActive)
	{
		// get size
		CString strSize;
		ASSERT(m_pRichView != NULL);
		if (m_pRichView)
		{
			long bytes = m_pRichView->GetRichEditCtrl().GetTextLength();
			int lines = m_pRichView->GetRichEditCtrl().GetLineCount();
			strSize.Format(_T("%ld Bytes (%d Lines)"), bytes, lines);
		}
		DDX_Text(pDX, IDC_E_SIZE, strSize);
	}

	//
	// Last saved:
	//
	if (!pDX->m_bSaveAndValidate && m_bFirstSetActive)
	{
		// get last modification time
		CString strSaved;
		if (!m_strPathName.IsEmpty())
		{
			CFileStatus fileStatus;
			CFile::GetStatus(m_strPathName, fileStatus);
			time_t time = fileStatus.m_mtime.GetTime();
			strSaved = _tctime(&time);
			strSaved.TrimRight();
		}
		DDX_Text(pDX, IDC_E_SAVED, strSaved);
	}
}


BEGIN_MESSAGE_MAP(CRichDocInPage1, baseRichDocInPage1)
	//{{AFX_MSG_MAP(CRichDocInPage1)
	ON_BN_CLICKED(IDC_B_OUTPUT, OnBOutput)
	ON_BN_CLICKED(IDC_B_DB, OnBDb)
	ON_CBN_SELCHANGE(IDC_CB_DB, OnSelchangeCbDb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



BOOL CRichDocInPage1::OnInitDialog() 
{
	baseRichDocInPage1::OnInitDialog();
	
	//{{Set Layout
	CreateRoot(VERTICAL, 5, 6) 

		<< ( pane(HORIZONTAL, ABSOLUTE_VERT)
			<< item(IDC_STATIC_ICON, NORESIZE)
			<< itemFixed(HORIZONTAL, 31)
			<< item(IDC_E_TITLE, ABSOLUTE_VERT|ALIGN_VCENTER)
		)

		<< itemFixed(VERTICAL, 2)

		<< item(IDC_HORZ_LINE2, ABSOLUTE_VERT)

		<< itemFixed(VERTICAL, 2)


		<< ( pane(HORIZONTAL, ABSOLUTE_VERT)
			<< item(IDC_ST_1, NORESIZE)
			<< item(IDC_E_FILENAME, ABSOLUTE_VERT)
			// << itemSpaceLike(HORIZONTAL, IDC_B_OUTPUT)
		)

		<< ( pane(HORIZONTAL, ABSOLUTE_VERT)
			<< item(IDC_ST_2, NORESIZE | ALIGN_VCENTER)
			<< item(IDC_E_OUTPUT, ABSOLUTE_VERT)
			<< item(IDC_B_OUTPUT, NORESIZE)
		)

		<< itemFixed(VERTICAL, 1)

		<< ( pane(HORIZONTAL, ABSOLUTE_VERT)
			<< item(IDC_ST_3, NORESIZE | ALIGN_VCENTER)
			<< item(IDC_CB_DB, ABSOLUTE_VERT)
			<< item(IDC_B_DB, NORESIZE)
		)

		//{{
		// << itemFixed(VERTICAL, 6)

		<< ( pane(HORIZONTAL, ABSOLUTE_VERT)
			<< itemSpaceLike(HORIZONTAL, IDC_ST_3)
			<< itemGrowing(HORIZONTAL)
			<< item(IDC_SET_AS_DEFAULT, NORESIZE)
			// << itemGrowing(HORIZONTAL)
			<< itemSpaceLike(HORIZONTAL, IDC_B_DB)

		)
		<< itemFixed(VERTICAL, 2)
		<< item(IDC_HORZ_LINE, ABSOLUTE_VERT)
		<< itemFixed(VERTICAL, 2)
		//}}

// COMMENT: {12/14/2000 8:25:25 PM}		<< itemFixed(VERTICAL, 6)

		<< ( pane(HORIZONTAL, ABSOLUTE_VERT)
			<< item(IDC_ST_4, NORESIZE, 0, 0, 0, 0)
			<< item(IDC_E_SIZE, ABSOLUTE_VERT, 0, 0, 0, 0)
			<< itemSpaceLike(HORIZONTAL, IDC_B_OUTPUT)
		)

		<< itemFixed(VERTICAL, 0)

		<< ( pane(HORIZONTAL, ABSOLUTE_VERT)
			<< item(IDC_ST_5, NORESIZE, 0, 0, 0, 0)
			<< item(IDC_E_SAVED, ABSOLUTE_VERT, 0, 0, 0, 0)
			<< itemSpaceLike(HORIZONTAL, IDC_B_OUTPUT)
		);
	UpdateLayout();
	//}}Set Layout

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRichDocInPage1::OnBOutput() 
{
	// Show file Dialog box
	CFileDialog dlg(
		FALSE,					// bOpenFileDialog
		_T("pqo"),				// lpszDefExt
		m_props.m_strOutPathName,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST,
		_T("Phreeqci Output Files (*.pqo)|*.pqo|All Files (*.*)|*.*||")
		);

	// set dialog caption
	dlg.m_ofn.lpstrTitle = _T("Save Output As");

	if (dlg.DoModal() == IDOK)
	{
		m_props.m_strOutPathName = dlg.GetPathName();
		UpdateData(FALSE);		// set window text
	}	
}

void CRichDocInPage1::OnBDb() 
{
	// Show file Dialog box
	CFileDialog dlg(
		TRUE,					// bOpenFileDialog
		NULL,					// lpszDefExt TODO: should this be _T("dat")
		m_props.m_strDBPathName,
		OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, 
		_T("Database Files (*.dat)|*.dat|All Files (*.*)|*.*||")
		);

	// set dialog caption
	dlg.m_ofn.lpstrTitle = _T("Select a Database");
	
	if (dlg.DoModal() == IDOK)
	{
		m_props.m_strDBPathName = dlg.GetPathName();
		UpdateData(FALSE);		// set window
		UpdateSetAsDefault();
	}	
}

void CRichDocInPage1::OnSelchangeCbDb() 
{
	// update m_props.m_strDBPathName before calling UpdateSetAsDefault
	UpdateData(TRUE);
	UpdateSetAsDefault();
}

void CRichDocInPage1::UpdateSetAsDefault()
{
	if (m_props.m_strDBPathName.CompareNoCase(((CPhreeqciApp*)AfxGetApp())->m_settings.m_strDefDBPathName) == 0)
	{
		m_btnSetAsDefault.EnableWindow(FALSE);
		m_btnSetAsDefault.SetCheck(BST_CHECKED);
	}
	else
	{
		m_btnSetAsDefault.EnableWindow(TRUE);
		m_btnSetAsDefault.SetCheck(BST_UNCHECKED);
	}
}
