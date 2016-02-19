// KPSelectedOutputPg1.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "KPSelectedOutputPg1.h"

#include "Util.h"
#include "DelayUpdate.h"
#include "SaveCurrentDirectory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CKPSelectedOutputPg1, baseCKPSelectedOutputPg1)


/////////////////////////////////////////////////////////////////////////////
// CKPSelectedOutputPg1 property page

CKPSelectedOutputPg1::CKPSelectedOutputPg1() : baseCKPSelectedOutputPg1(CKPSelectedOutputPg1::IDD)
{
	//{{AFX_DATA_INIT(CKPSelectedOutputPg1)
	m_strFileName = _T("selected_1.out");
	//}}AFX_DATA_INIT
	for (int i = 0; i < sizeof(m_arrValue) / sizeof(m_arrValue[0]); ++i)
	{
		m_arrValue[i] = AS_IS;
	}
}

CKPSelectedOutputPg1::~CKPSelectedOutputPg1()
{
}

void CKPSelectedOutputPg1::DoDataExchange(CDataExchange* pDX)
{
	baseCKPSelectedOutputPg1::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKPSelectedOutputPg1)
	if (m_bFirstSetActive)
	{
		//
		// Init NumDesc SELECTED_OUTPUT doesn't support a range
		//
		m_egNumDesc.SetColWidth(0, 0, 1100);
		m_egNumDesc.SetTextMatrix(0, 0, _T("Number"));
		m_egNumDesc.SetRowHeight(1, 0);
	}

	DDX_Text(pDX, IDC_FILE_NAME, m_strFileName);
	//}}AFX_DATA_MAP
	if (pDX->m_bSaveAndValidate)
	{
		m_strFileName.TrimLeft();
		m_strFileName.TrimRight();
	}

	CWaitCursor wait;

	for (int i = 0; i < sizeof(m_arrValue) / sizeof(m_arrValue[0]); ++i)
	{
		ASSERT(m_arrValue[i] == AS_IS || m_arrValue[i] == AS_TRUE || m_arrValue[i] == AS_FALSE);
		int nValue = static_cast<int>(m_arrValue[i]);
		DDX_CBIndex(pDX, IDC_CBO_SE_00 + i, nValue);
		if (pDX->m_bSaveAndValidate)
		{
			m_arrValue[i] = static_cast<ValueType>(nValue);
			ASSERT(m_arrValue[i] == AS_IS || m_arrValue[i] == AS_TRUE || m_arrValue[i] == AS_FALSE);
		}
	}
}


BEGIN_MESSAGE_MAP(CKPSelectedOutputPg1, baseCKPSelectedOutputPg1)
	//{{AFX_MSG_MAP(CKPSelectedOutputPg1)
	ON_BN_CLICKED(IDC_B_AS_IS, OnBAsIs)
	ON_BN_CLICKED(IDC_B_TRUE, OnBTrue)
	ON_BN_CLICKED(IDC_B_FALSE, OnBFalse)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_EN_SETFOCUS(IDC_FILE_NAME, OnSetfocusFileName)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
	// custom setfocus notifications
	ON_BN_SETFOCUS(IDC_BROWSE, OnSetfocusBrowse)
	ON_CONTROL_RANGE(CBN_SETFOCUS, IDC_CBO_SE_00, IDC_CBO_SE_18, OnSetfocusCbo)
	ON_CONTROL_RANGE(BN_SETFOCUS, IDC_B_AS_IS, IDC_B_FALSE, OnSetfocusBtn)
	//ON_CBN_SELCHANGE(IDC_CBO_SE_00, &CKPSelectedOutputPg1::OnCbnSelchangeCboSe00)
	//CBN_SELCHANGE
	ON_CONTROL_RANGE(CBN_SELCHANGE, IDC_CBO_SE_00, IDC_CBO_SE_18, OnSelChangeCbo)
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
	ON_MESSAGE(EGN_SETFOCUS, OnSetfocusGrid)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CKPSelectedOutputPg1, baseCKPSelectedOutputPg1)
    //{{AFX_EVENTSINK_MAP(CKPSelectedOutputPg2)
	ON_EVENT(CKPSelectedOutputPg1, IDC_MSHFG_NUM_DESC, 71 /* EnterCell */, OnEnterCellMshfgNumDesc, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CKPSelectedOutputPg1::OnBAsIs() 
{
	for (int i = 3; i <= 17; ++i)
	{
		m_arrValue[i] = AS_IS;
	}
	UpdateData(FALSE);
}

void CKPSelectedOutputPg1::OnBTrue() 
{
	for (int i = 3; i <= 17; ++i)
	{
		m_arrValue[i] = AS_TRUE;
	}
	UpdateData(FALSE);
}

void CKPSelectedOutputPg1::OnBFalse() 
{
	for (int i = 3; i <= 17; ++i)
	{
		m_arrValue[i] = AS_FALSE;
	}
	UpdateData(FALSE);
}

void CKPSelectedOutputPg1::OnSelChangeCbo(UINT nID)
{
	if (CComboBox *pCbo = (CComboBox*)this->GetDlgItem(nID))
	{
#if defined(_DEBUG)
		TCHAR szClassName[ MAX_PATH ];
		::GetClassName( pCbo->GetSafeHwnd(), szClassName, MAX_PATH );
		ASSERT(_tcscmp(szClassName, _T("ComboBox")) == 0);
#endif
		m_arrValue[nID - IDC_CBO_SE_00] = static_cast<ValueType>(pCbo->GetCurSel());
	}
}

LRESULT CKPSelectedOutputPg1::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
{
	UINT nID = wParam;
	UNUSED_ALWAYS(lParam);

	switch (nID)
	{
	case IDC_MSHFG_NUM_DESC :
		OnEnterCellMshfgNumDesc();
		return 0;
		break;
	}
	return 0;
}

LRESULT CKPSelectedOutputPg1::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bMakeChange = TRUE;

	// ignore if edit is cancelled
	if ( pInfo->item.pszText == NULL ) return bMakeChange;

	switch ( nID )
	{
	case IDC_MSHFG_NUM_DESC :
		break;
	}
	return bMakeChange;
}


/////////////////////////////////////////////////////////////////////////////
// CKPSelectedOutputPg3 property page

IMPLEMENT_DYNCREATE(CKPSelectedOutputPg3, baseCKPSelectedOutputPg3)

CKPSelectedOutputPg3::CKPSelectedOutputPg3() : baseCKPSelectedOutputPg3(CKPSelectedOutputPg3::IDD)
, m_glDoc(&m_egMolalities, &m_clcMolalities, 1)
{
	//{{AFX_DATA_INIT(CKPSelectedOutputPg3)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CKPSelectedOutputPg3::~CKPSelectedOutputPg3()
{
}

void CKPSelectedOutputPg3::DoDataExchange(CDataExchange* pDX)
{
	baseCKPSelectedOutputPg3::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKPSelectedOutputPg3)
	DDX_Control(pDX, IDC_DESCRIPTION, m_eInputDesc);
	DDX_Control(pDX, IDC_CL_MOLALITIES, m_clcMolalities);
	DDX_Control(pDX, IDC_MSHFG_MOLALITIES, m_egMolalities);
	//}}AFX_DATA_MAP

	CWaitCursor wait;

	// reduce grid flicker
	CDelayUpdate update(this, IDC_MSHFG_MOLALITIES);

	if (m_bFirstSetActive)
	{
		//
		// Init NumDesc SELECTED_OUTPUT doesn't support a range
		//
		m_egNumDesc.SetColWidth(0, 0, 1100);
		m_egNumDesc.SetTextMatrix(0, 0, _T("Number"));
		m_egNumDesc.SetRowHeight(1, 0);

		CUtil::InsertAqSpecies(&m_clcMolalities, GetDatabase());
		m_egMolalities.AddRowHeaders();
		m_egMolalities.SetTextMatrix(0, 1, _T("Name"));
		m_egMolalities.SetColWidth(1, 0, 1400);

		// center align the column headers
		m_egMolalities.SetFillStyle(flexFillRepeat);
		m_egMolalities.SetRow(0);
		m_egMolalities.SetCol(0);
		m_egMolalities.SetColSel(m_egMolalities.GetCols(0) - 1);
		m_egMolalities.SetCellAlignment(flexAlignCenterCenter);
		m_egMolalities.SetFillStyle(flexFillSingle);


		// set bold titles
		m_egMolalities.SetRow(0), m_egMolalities.SetCol(1);
		m_egMolalities.SetCellFontBold(TRUE);

		// set initial cell
		m_egMolalities.SetRow(1), m_egMolalities.SetCol(1);

	}

	if (pDX->m_bSaveAndValidate)
	{
		std::list<CString> listTotals;
		for (long nRow = m_egMolalities.GetFixedRows(); nRow < m_egMolalities.GetRows(); ++nRow)
		{
			CString strMolalities;

			// phase name
			DDX_GridText(pDX, IDC_MSHFG_MOLALITIES, nRow, 1, strMolalities);
			if (strMolalities.IsEmpty())
				continue;

			// validate strElement
			if (!::_istupper(strMolalities.GetAt(0)) && strMolalities.GetAt(0) != _T('(')) // TODO DAVID
			{
				DDX_GridFail(pDX, _T("Expected species name to begin with upper case letter."),
					_T("Error"));
			}

			listTotals.push_back(strMolalities);
		}


		// if here list is valid and can be assigned to the member variable
		m_listMolalities.assign(listTotals.begin(), listTotals.end());

	}
	else
	{
		// see OnKillActive
		ASSERT(m_glDoc.GetCount() == 0);

		std::list<CString>::iterator iter = m_listMolalities.begin();
		for (; iter != m_listMolalities.end(); ++iter)
		{
			m_glDoc.AddItem(*iter);
		}
		ASSERT(m_listMolalities.size() == m_glDoc.GetCount());
	}
}


BEGIN_MESSAGE_MAP(CKPSelectedOutputPg3, baseCKPSelectedOutputPg3)
	//{{AFX_MSG_MAP(CKPSelectedOutputPg3)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CL_MOLALITIES, OnItemchangedClMolalities)
	ON_NOTIFY(NM_SETFOCUS, IDC_CL_MOLALITIES, OnSetfocusClMolalities)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
	// custom edit grid notifications
	ON_MESSAGE(EGN_BEGINCELLEDIT, OnBeginCellEdit)
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
	ON_MESSAGE(EGN_SETFOCUS, OnSetfocusGrid)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKPSelectedOutputPg3 message handlers

void CKPSelectedOutputPg3::OnSetfocusClMolalities(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNUSED(pNMHDR);

	CString strRes;
	strRes.LoadString(IDS_STRING390);
	m_eInputDesc.SetWindowText(strRes);

	// set default selection
	// this gives user feedback when the list recieves the focus
	if (m_clcMolalities.GetItemCount() && m_clcMolalities.GetFirstSelectedItemPosition() == NULL)
	{
		m_clcMolalities.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}
	
	*pResult = 0;
}

void CKPSelectedOutputPg3::OnEnterCellMshfgMolalities() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING514);
	m_eInputDesc.SetWindowText(strRes);	
}


/////////////////////////////////////////////////////////////////////////////
// CKPSelectedOutputPg4 property page

IMPLEMENT_DYNCREATE(CKPSelectedOutputPg4, baseCKPSelectedOutputPg4)

CKPSelectedOutputPg4::CKPSelectedOutputPg4() : baseCKPSelectedOutputPg4(CKPSelectedOutputPg4::IDD)
, m_glDoc(&m_egActivities, &m_clcActivities, 1)
{
	//{{AFX_DATA_INIT(CKPSelectedOutputPg4)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CKPSelectedOutputPg4::~CKPSelectedOutputPg4()
{
}

void CKPSelectedOutputPg4::DoDataExchange(CDataExchange* pDX)
{
	baseCKPSelectedOutputPg4::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKPSelectedOutputPg4)
	DDX_Control(pDX, IDC_DESCRIPTION, m_eInputDesc);
	DDX_Control(pDX, IDC_CL_ACTIVITIES, m_clcActivities);
	DDX_Control(pDX, IDC_MSHFG_ACTIVITIES, m_egActivities);
	//}}AFX_DATA_MAP

	CWaitCursor wait;

	// reduce grid flicker
	CDelayUpdate update(this, IDC_MSHFG_ACTIVITIES);

	if (m_bFirstSetActive)
	{
		//
		// Init NumDesc SELECTED_OUTPUT doesn't support a range
		//
		m_egNumDesc.SetColWidth(0, 0, 1100);
		m_egNumDesc.SetTextMatrix(0, 0, _T("Number"));
		m_egNumDesc.SetRowHeight(1, 0);

		CUtil::InsertAqSpecies(&m_clcActivities, GetDatabase());
		m_egActivities.AddRowHeaders();
		m_egActivities.SetTextMatrix(0, 1, _T("Name"));
		m_egActivities.SetColWidth(1, 0, 1400);

		// center align the column headers
		m_egActivities.SetFillStyle(flexFillRepeat);
		m_egActivities.SetRow(0);
		m_egActivities.SetCol(0);
		m_egActivities.SetColSel(m_egActivities.GetCols(0) - 1);
		m_egActivities.SetCellAlignment(flexAlignCenterCenter);
		m_egActivities.SetFillStyle(flexFillSingle);


		// set bold titles
		m_egActivities.SetRow(0), m_egActivities.SetCol(1);
		m_egActivities.SetCellFontBold(TRUE);

		// set initial cell
		m_egActivities.SetRow(1), m_egActivities.SetCol(1);
	}
	if (pDX->m_bSaveAndValidate)
	{
		std::list<CString> listActivities;
		for (long nRow = m_egActivities.GetFixedRows(); nRow < m_egActivities.GetRows(); ++nRow)
		{
			CString strActivities;

			DDX_GridText(pDX, IDC_MSHFG_ACTIVITIES, nRow, 1, strActivities);
			if (strActivities.IsEmpty())
				continue;

			// validate strElement
			if (!::_istupper(strActivities.GetAt(0)) && strActivities.GetAt(0) != _T('(')) // TODO DAVID
			{
				DDX_GridFail(pDX, _T("Expected species name to begin with upper case letter."),
					_T("Error"));
			}

			listActivities.push_back(strActivities);
		}


		// if here list is valid and can be assigned to the member variable
		m_listActivities.assign(listActivities.begin(), listActivities.end());

	}
	else
	{
		// see OnKillActive
		ASSERT(m_glDoc.GetCount() == 0);

		std::list<CString>::iterator iter = m_listActivities.begin();
		for (; iter != m_listActivities.end(); ++iter)
		{
			m_glDoc.AddItem(*iter);
		}
		ASSERT(m_listActivities.size() == m_glDoc.GetCount());
	}
}


BEGIN_MESSAGE_MAP(CKPSelectedOutputPg4, baseCKPSelectedOutputPg4)
	//{{AFX_MSG_MAP(CKPSelectedOutputPg4)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CL_ACTIVITIES, OnItemchangedClActivities)
	ON_NOTIFY(NM_SETFOCUS, IDC_CL_ACTIVITIES, OnSetfocusClActivities)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
	ON_MESSAGE(EGN_BEGINCELLEDIT, OnBeginCellEdit)
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
	ON_MESSAGE(EGN_SETFOCUS, OnSetfocusGrid)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKPSelectedOutputPg4 message handlers

void CKPSelectedOutputPg4::OnSetfocusClActivities(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNUSED(pNMHDR);

	CString strRes;
	strRes.LoadString(IDS_STRING391);
	m_eInputDesc.SetWindowText(strRes);

	// set default selection
	// this gives user feedback when the list recieves the focus
	if (m_clcActivities.GetItemCount() && m_clcActivities.GetFirstSelectedItemPosition() == NULL)
	{
		m_clcActivities.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}
	
	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////
// CKPSelectedOutputPg2 property page

IMPLEMENT_DYNCREATE(CKPSelectedOutputPg2, baseCKPSelectedOutputPg2)

CKPSelectedOutputPg2::CKPSelectedOutputPg2() : baseCKPSelectedOutputPg2(CKPSelectedOutputPg2::IDD)
, m_glDoc(&m_egTotals, &m_clcTotals, 1)
{
	//{{AFX_DATA_INIT(CKPSelectedOutputPg2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CKPSelectedOutputPg2::~CKPSelectedOutputPg2()
{
}

void CKPSelectedOutputPg2::DoDataExchange(CDataExchange* pDX)
{
	baseCKPSelectedOutputPg2::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKPSelectedOutputPg2)
	DDX_Control(pDX, IDC_CL_TOTALS, m_clcTotals);
	DDX_Control(pDX, IDC_MSHFG_TOTALS, m_egTotals);
	//}}AFX_DATA_MAP

	CWaitCursor wait;

	// reduce grid flicker
	CDelayUpdate update(this, IDC_MSHFG_TOTALS);

	if (m_bFirstSetActive)
	{
		//
		// Init NumDesc SELECTED_OUTPUT doesn't support a range
		//
		m_egNumDesc.SetColWidth(0, 0, 1100);
		m_egNumDesc.SetTextMatrix(0, 0, _T("Number"));
		m_egNumDesc.SetRowHeight(1, 0);

		CUtil::InsertTotals(&m_clcTotals, GetDatabase());
		m_egTotals.AddRowHeaders();
		m_egTotals.SetTextMatrix(0, 1, _T("Name"));
		m_egTotals.SetColWidth(1, 0, 1400);

		// center align the column headers
		m_egTotals.SetFillStyle(flexFillRepeat);
		m_egTotals.SetRow(0);
		m_egTotals.SetCol(0);
		m_egTotals.SetColSel(m_egTotals.GetCols(0) - 1);
		m_egTotals.SetCellAlignment(flexAlignCenterCenter);
		m_egTotals.SetFillStyle(flexFillSingle);


		// set bold titles
		m_egTotals.SetRow(0), m_egTotals.SetCol(1);
		m_egTotals.SetCellFontBold(TRUE);

		// set initial cell
		m_egTotals.SetRow(1), m_egTotals.SetCol(1);

	}

	if (pDX->m_bSaveAndValidate)
	{
		std::list<CString> listTotals;
		for (long nRow = m_egTotals.GetFixedRows(); nRow < m_egTotals.GetRows(); ++nRow)
		{
			CString strTotal;

			// phase name
			DDX_GridText(pDX, IDC_MSHFG_TOTALS, nRow, 1, strTotal);
			if (strTotal.IsEmpty())
				continue;

			listTotals.push_back(strTotal);
		}


		// if here list is valid and can be assigned to the member variable
		m_listTotals.assign(listTotals.begin(), listTotals.end());

	}
	else
	{
		std::list<CString>::iterator iter = m_listTotals.begin();
		for (; iter != m_listTotals.end(); ++iter)
		{
			m_glDoc.AddItem(*iter);
		}
	}

}


BEGIN_MESSAGE_MAP(CKPSelectedOutputPg2, baseCKPSelectedOutputPg2)
	//{{AFX_MSG_MAP(CKPSelectedOutputPg2)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CL_TOTALS, OnItemchangedClTotals)
	ON_NOTIFY(NM_SETFOCUS, IDC_CL_TOTALS, OnSetfocusClTotals)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
	// custom edit grid notifications
	ON_MESSAGE(EGN_BEGINCELLEDIT, OnBeginCellEdit)
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
	ON_MESSAGE(EGN_SETFOCUS, OnSetfocusGrid)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKPSelectedOutputPg2 message handlers

void CKPSelectedOutputPg2::OnItemchangedClTotals(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// Add your control notification handler code here

	m_glDoc.OnItemChanged(pNMHDR, pResult);
	
	*pResult = 0;
}

BEGIN_EVENTSINK_MAP(CKPSelectedOutputPg2, baseCKPSelectedOutputPg2)
    //{{AFX_EVENTSINK_MAP(CKPSelectedOutputPg2)
	ON_EVENT(CKPSelectedOutputPg2, IDC_MSHFG_TOTALS, -602 /* KeyDown */, OnKeyDownMshfgTotals, VTS_PI2 VTS_I2)
	ON_EVENT(CKPSelectedOutputPg2, IDC_MSHFG_NUM_DESC, 71 /* EnterCell */, OnEnterCellMshfgNumDesc, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CKPSelectedOutputPg2::OnKeyDownMshfgTotals(short FAR* KeyCode, short Shift) 
{
	m_glDoc.OnKeyDown(KeyCode, Shift);	
}

LRESULT CKPSelectedOutputPg2::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bDenyEdit = FALSE;	// allow edit	

	switch (nID)
	{
	case IDC_MSHFG_TOTALS :
		switch ( pInfo->item.iCol )
		{
		case 1 :
			pInfo->item.bUseCombo = (CUtil::InsertTotals(pInfo->item.hWndCombo, GetDatabase()) > 0);
			break;
		default :
			break;
		}
		break;
	}

	return bDenyEdit;
}

LRESULT CKPSelectedOutputPg2::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bMakeChange = TRUE;

	// ignore if edit is cancelled
	if ( pInfo->item.pszText == NULL ) return bMakeChange;

	switch ( nID )
	{
	case IDC_MSHFG_NUM_DESC :
		break;
	}
	return bMakeChange;
}

LRESULT CKPSelectedOutputPg2::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
{
	UINT nID = wParam;
	UNUSED_ALWAYS(lParam);

	CString strRes;
	switch (nID)
	{
	case IDC_MSHFG_NUM_DESC :
		OnEnterCellMshfgNumDesc();
		return 0;
		break;
	case IDC_MSHFG_TOTALS:
		strRes.LoadString(IDS_STRING513);
		break;
	}
	m_eInputDesc.SetWindowText(strRes);

	return 0;
}

LRESULT CKPSelectedOutputPg3::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bDenyEdit = FALSE;	// allow edit	

	switch (nID)
	{
	case IDC_MSHFG_MOLALITIES :
		switch ( pInfo->item.iCol )
		{
		case 1 :
			pInfo->item.bUseCombo = (CUtil::InsertAqSpecies(pInfo->item.hWndCombo, GetDatabase()) > 0);
			break;
		default :
			break;
		}
		break;
	}

	return bDenyEdit;
}

LRESULT CKPSelectedOutputPg3::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bMakeChange = TRUE;

	// ignore if edit is cancelled
	if ( pInfo->item.pszText == NULL ) return bMakeChange;

	switch ( nID )
	{
	case IDC_MSHFG_NUM_DESC :
		break;
	}
	return bMakeChange;
}

LRESULT CKPSelectedOutputPg3::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
{
	UINT nID = wParam;
	UNUSED_ALWAYS(lParam);
	switch (nID)
	{
	case IDC_MSHFG_NUM_DESC :
		OnEnterCellMshfgNumDesc();
		break;
	case IDC_MSHFG_MOLALITIES:
		OnEnterCellMshfgMolalities();
		break;
	}
	return 0;
}

void CKPSelectedOutputPg3::OnItemchangedClMolalities(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_glDoc.OnItemChanged(pNMHDR, pResult);
}

BEGIN_EVENTSINK_MAP(CKPSelectedOutputPg3, baseCKPSelectedOutputPg3)
    //{{AFX_EVENTSINK_MAP(CKPSelectedOutputPg3)
	ON_EVENT(CKPSelectedOutputPg3, IDC_MSHFG_MOLALITIES, -602 /* KeyDown */, OnKeyDownMshfgMolalities, VTS_PI2 VTS_I2)
	ON_EVENT(CKPSelectedOutputPg3, IDC_MSHFG_MOLALITIES, 71 /* EnterCell */, OnEnterCellMshfgMolalities, VTS_NONE)
	ON_EVENT(CKPSelectedOutputPg3, IDC_MSHFG_NUM_DESC, 71 /* EnterCell */, OnEnterCellMshfgNumDesc, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CKPSelectedOutputPg3::OnKeyDownMshfgMolalities(short FAR* KeyCode, short Shift) 
{
	m_glDoc.OnKeyDown(KeyCode, Shift);	
}

void CKPSelectedOutputPg1::OnBrowse() 
{
	CSaveCurrentDirectory scd;

	// Show file Dialog box
	CFileDialog dlg(
		FALSE,					// bOpenFileDialog
		_T("sel"),				// lpszDefExt
		m_strFileName,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST,
		_T("Selected Output Files (*.sel)|*.sel|All Files (*.*)|*.*||")
		);
	dlg.m_ofn.lpstrTitle = _T("Save Selected Output As");
	if (dlg.DoModal() == IDOK)
	{
		m_strFileName = dlg.GetPathName();
		UpdateData(FALSE);
	}	
}

void CKPSelectedOutputPg1::OnSetfocusFileName() 
{
	CString strRes;
	strRes.LoadString(IDS_SEL_OUT_358);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPSelectedOutputPg1::OnSetfocusCbo(UINT nID) 
{
	UINT nResID = nID - IDC_CBO_SE_00 + IDS_STRING359;

	CString strRes;
	strRes.LoadString(nResID);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPSelectedOutputPg1::OnSetfocusBtn(UINT nID) 
{
	UINT nResID = nID - IDC_B_AS_IS + IDS_STRING385;

	CString strRes;
	strRes.LoadString(nResID);
	m_eInputDesc.SetWindowText(strRes);	
}

void CKPSelectedOutputPg1::OnSetfocusBrowse()
{
	// in order for a check box to recieve this notification
	// it must have the notify style set
	CString strRes;
	strRes.LoadString(IDS_EXCHANGE_165);
	m_eInputDesc.SetWindowText(strRes);
}

BOOL CKPSelectedOutputPg1::OnInitDialog() 
{
	baseCKPSelectedOutputPg1::OnInitDialog();
	
	// Add extra initialization here

	// set layout
	CreateRoot(VERTICAL, 5, 6) 
		<< (pane(HORIZONTAL, ABSOLUTE_VERT)
			<< item(IDC_MSHFG_NUM_DESC, ABSOLUTE_VERT)
			)
		<< itemFixed(VERTICAL, 1)
		<< (pane(HORIZONTAL, ABSOLUTE_VERT)
			<< itemFixed(HORIZONTAL, 4)
			<< item(IDC_S_FILENAME, NORESIZE | ALIGN_CENTER)
			<< item(IDC_FILE_NAME, ABSOLUTE_VERT | ALIGN_CENTER)
			<< item(IDC_BROWSE, NORESIZE)
			)
		<< (paneCtrl(IDC_S_OPTS, HORIZONTAL, ABSOLUTE_VERT, nDefaultBorder, 10, 10)
			<< (pane(VERTICAL, GREEDY)
				<< (pane(HORIZONTAL, GREEDY)
					<< itemGrowing(HORIZONTAL)
					<< item(IDC_S_0, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 1)
					<< item(IDC_CBO_SE_00, NORESIZE | ALIGN_CENTER)
					<< itemGrowing(HORIZONTAL)
					<< item(IDC_S_2, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 1)
					<< item(IDC_CBO_SE_02, NORESIZE | ALIGN_CENTER)
					<< itemGrowing(HORIZONTAL)
					)
				<< (pane(HORIZONTAL, GREEDY)
					<< itemGrowing(HORIZONTAL)
					<< item(IDC_S_1, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 1)
					<< item(IDC_CBO_SE_01, NORESIZE | ALIGN_CENTER)
					<< itemGrowing(HORIZONTAL)
					<< item(IDC_S_18, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 1)
					<< item(IDC_CBO_SE_18, NORESIZE | ALIGN_CENTER)
					<< itemGrowing(HORIZONTAL)
					)
				)
			)

		/**
		 * paneCtrl( UINT nID, layOrientation orientation, layResizeMode modeResize = GREEDY,
		 *          int sizeBorder = nDefaultBorder, int sizeExtraBorder = 0, int sizeTopExtra = 0,
		 *          int sizeSecondary = 0);
		 */
		/**
		 * pane( layOrientation orientation, layResizeMode modeResize = GREEDY,
		 *       int sizeBorder = nDefaultBorder, int sizeExtraBorder = 0, int sizeSecondary = 0);
		 */
		/**
		 * item( UINT nID, layResizeMode modeResize = GREEDY, int sizeX =0, int sizeY =0,
		 *       int sizeXMin =-1, int sizeYMin =-1);
		 */
		<< (paneCtrl(IDC_S_ADD_OPTS, HORIZONTAL, ABSOLUTE_VERT, 0, 3, 16, 0)
			<< (pane(VERTICAL, GREEDY, 0, 0, 0)
				<< (pane(HORIZONTAL, GREEDY, 0, 0, 0)
					<< item(IDC_S_3, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_CBO_SE_03, NORESIZE | ALIGN_CENTER)
					<< itemGrowing(HORIZONTAL)
					<< item(IDC_S_8, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_CBO_SE_08, NORESIZE | ALIGN_CENTER)
					<< itemGrowing(HORIZONTAL)
					<< item(IDC_S_13, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_CBO_SE_13, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					)
				<< itemFixed(VERTICAL, 7)
				<< (pane(HORIZONTAL, GREEDY, 0, 0, 0)
					<< item(IDC_S_4, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_CBO_SE_04, NORESIZE | ALIGN_CENTER)
					<< itemGrowing(HORIZONTAL)
					<< item(IDC_S_9, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_CBO_SE_09, NORESIZE | ALIGN_CENTER)
					<< itemGrowing(HORIZONTAL)
					<< item(IDC_S_14, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_CBO_SE_14, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					)
				<< itemFixed(VERTICAL, 7)
				<< (pane(HORIZONTAL, GREEDY, 0, 0, 0)
					<< item(IDC_S_5, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_CBO_SE_05, NORESIZE | ALIGN_CENTER)
					<< itemGrowing(HORIZONTAL)
					<< item(IDC_S_10, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_CBO_SE_10, NORESIZE | ALIGN_CENTER)
					<< itemGrowing(HORIZONTAL)
					<< item(IDC_S_15, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_CBO_SE_15, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					)
				<< itemFixed(VERTICAL, 7)
				<< (pane(HORIZONTAL, GREEDY, 0, 0, 0)
					<< item(IDC_S_6, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_CBO_SE_06, NORESIZE | ALIGN_CENTER)
					<< itemGrowing(HORIZONTAL)
					<< item(IDC_S_11, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_CBO_SE_11, NORESIZE | ALIGN_CENTER)
					<< itemGrowing(HORIZONTAL)
					<< item(IDC_S_16, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_CBO_SE_16, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					)
				<< itemFixed(VERTICAL, 7)
				<< (pane(HORIZONTAL, GREEDY, 0, 0, 0)
					<< item(IDC_S_7, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_CBO_SE_07, NORESIZE | ALIGN_CENTER)
					<< itemGrowing(HORIZONTAL)
					<< item(IDC_S_12, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_CBO_SE_12, NORESIZE | ALIGN_CENTER)
					<< itemGrowing(HORIZONTAL)
					<< item(IDC_S_17, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_CBO_SE_17, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					)
				<< itemFixed(VERTICAL, 11)
				<< (pane(HORIZONTAL, GREEDY, 0, 0, 0)
					<< itemGrowing(HORIZONTAL)
					<< item(IDC_B_AS_IS, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_B_TRUE, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					<< item(IDC_B_FALSE, NORESIZE | ALIGN_CENTER)
					<< itemFixed(HORIZONTAL, 10)
					)
				<< itemFixed(VERTICAL, 7)
				)
			)
			//}}

		<< (paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, GREEDY, nDefaultBorder, 10, 10)
			<< item(IDC_E_DESC_INPUT, GREEDY)
			)
	;
	UpdateLayout();

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CKPSelectedOutputPg2::OnInitDialog() 
{
	baseCKPSelectedOutputPg2::OnInitDialog();
	
	// set layout
	CreateRoot(VERTICAL, 5, 6) 
		<< (pane(HORIZONTAL, ABSOLUTE_VERT)
			<< item(IDC_MSHFG_NUM_DESC, ABSOLUTE_VERT)
			)
		<< itemFixed(VERTICAL, 1)
		<< item(IDC_S_DEFINED, NORESIZE)
		<< (pane(VERTICAL, GREEDY, 0, 0, 0)
			<< (pane(HORIZONTAL, RELATIVE_VERT, 20, 0, 70)
				<< item(IDC_CL_TOTALS, GREEDY)
				<< item(IDC_MSHFG_TOTALS, ABSOLUTE_HORZ)
				)
			<< itemFixed(VERTICAL, 7)
			<< (paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, RELATIVE_VERT, nDefaultBorder, 10, 10, 30)
				<< item(IDC_E_DESC_INPUT, GREEDY)
				)
			)
	;
	UpdateLayout();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CKPSelectedOutputPg3::OnInitDialog() 
{
	baseCKPSelectedOutputPg3::OnInitDialog();
	
	// set layout
	CreateRoot(VERTICAL, 5, 6) 
		<< (pane(HORIZONTAL, ABSOLUTE_VERT)
			<< item(IDC_MSHFG_NUM_DESC, ABSOLUTE_VERT)
			)
		<< itemFixed(VERTICAL, 1)
		<< item(IDC_S_DEFINED, NORESIZE)
		<< (pane(VERTICAL, GREEDY, 0, 0, 0)
			<< (pane(HORIZONTAL, RELATIVE_VERT, 20, 0, 80)
				<< item(IDC_CL_MOLALITIES, GREEDY)
				<< item(IDC_MSHFG_MOLALITIES, ABSOLUTE_HORZ)
				)
			<< itemFixed(VERTICAL, 7)
			<< (paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, RELATIVE_VERT, nDefaultBorder, 10, 10, 20)
				<< item(IDC_E_DESC_INPUT, GREEDY)
				)
			)
	;
	UpdateLayout();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CKPSelectedOutputPg4::OnInitDialog() 
{
	baseCKPSelectedOutputPg4::OnInitDialog();
	
	// set layout
	CreateRoot(VERTICAL, 5, 6) 
		<< (pane(HORIZONTAL, ABSOLUTE_VERT)
			<< item(IDC_MSHFG_NUM_DESC, ABSOLUTE_VERT)
			)
		<< itemFixed(VERTICAL, 1)
		<< item(IDC_S_DEFINED, NORESIZE)
		<< (pane(VERTICAL, GREEDY, 0, 0, 0)
			<< (pane(HORIZONTAL, RELATIVE_VERT, 20, 0, 80)
				<< item(IDC_CL_ACTIVITIES, GREEDY)
				<< item(IDC_MSHFG_ACTIVITIES, ABSOLUTE_HORZ)
				)
			<< itemFixed(VERTICAL, 7)
			<< (paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, RELATIVE_VERT, nDefaultBorder, 10, 10, 20)
				<< item(IDC_E_DESC_INPUT, GREEDY)
				)
			)
	;
	UpdateLayout();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKPSelectedOutputPg4::OnItemchangedClActivities(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_glDoc.OnItemChanged(pNMHDR, pResult);
}

LRESULT CKPSelectedOutputPg4::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bDenyEdit = FALSE;	// allow edit	

	switch (nID)
	{
	case IDC_MSHFG_ACTIVITIES :
		switch ( pInfo->item.iCol )
		{
		case 1 :
			pInfo->item.bUseCombo = (CUtil::InsertAqSpecies(pInfo->item.hWndCombo, GetDatabase()) > 0);
			break;

		default :
			break;
		}
		break;
	}

	return bDenyEdit;
}

LRESULT CKPSelectedOutputPg4::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bMakeChange = TRUE;

	// ignore if edit is cancelled
	if ( pInfo->item.pszText == NULL ) return bMakeChange;

	switch ( nID )
	{
	case IDC_MSHFG_NUM_DESC :
		break;
	}
	return bMakeChange;
}

LRESULT CKPSelectedOutputPg4::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
{
	UINT nID = wParam;
	UNUSED_ALWAYS(lParam);

	switch (nID)
	{
	case IDC_MSHFG_NUM_DESC :
		OnEnterCellMshfgNumDesc();
		break;
	case IDC_MSHFG_ACTIVITIES:
		OnEnterCellMshfgActivities();
		break;
	}
	return 0;
}

BEGIN_EVENTSINK_MAP(CKPSelectedOutputPg4, baseCKPSelectedOutputPg4)
    //{{AFX_EVENTSINK_MAP(CKPSelectedOutputPg4)
	ON_EVENT(CKPSelectedOutputPg4, IDC_MSHFG_ACTIVITIES, -602 /* KeyDown */, OnKeyDownMshfgActivities, VTS_PI2 VTS_I2)
	ON_EVENT(CKPSelectedOutputPg4, IDC_MSHFG_ACTIVITIES, 71 /* EnterCell */, OnEnterCellMshfgActivities, VTS_NONE)
	ON_EVENT(CKPSelectedOutputPg4, IDC_MSHFG_NUM_DESC, 71 /* EnterCell */, OnEnterCellMshfgNumDesc, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CKPSelectedOutputPg4::OnKeyDownMshfgActivities(short FAR* KeyCode, short Shift) 
{
	m_glDoc.OnKeyDown(KeyCode, Shift);	
}

BOOL CKPSelectedOutputPg3::OnKillActive() 
{
	if (baseCKPSelectedOutputPg3::OnKillActive())
	{
		// as last step remove all entries from m_glDoc;
		m_glDoc.DeleteAllRows();
		return TRUE;
	}
	return FALSE;
}

BOOL CKPSelectedOutputPg4::OnKillActive() 
{
	if (baseCKPSelectedOutputPg4::OnKillActive())
	{
		// as last step remove all entries from m_glDoc;
		m_glDoc.DeleteAllRows();
		return TRUE;
	}
	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// CKPSelectedOutputPg5 property page

IMPLEMENT_DYNCREATE(CKPSelectedOutputPg5, baseCKPSelectedOutputPg5)

CKPSelectedOutputPg5::CKPSelectedOutputPg5() : baseCKPSelectedOutputPg5(CKPSelectedOutputPg5::IDD)
, m_glDoc(&m_egPhases, &m_clcPhases, 1)
{
	//{{AFX_DATA_INIT(CKPSelectedOutputPg5)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CKPSelectedOutputPg5::~CKPSelectedOutputPg5()
{
}

void CKPSelectedOutputPg5::DoDataExchange(CDataExchange* pDX)
{
	baseCKPSelectedOutputPg5::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKPSelectedOutputPg5)
	DDX_Control(pDX, IDC_DESCRIPTION, m_eInputDesc);
	DDX_Control(pDX, IDC_CL_PHASES, m_clcPhases);
	DDX_Control(pDX, IDC_MSHFG_PHASES, m_egPhases);
	//}}AFX_DATA_MAP

	CWaitCursor wait;

	// reduce grid flicker
	CDelayUpdate update(this, IDC_MSHFG_PHASES);

	if (m_bFirstSetActive)
	{
		//
		// Init NumDesc SELECTED_OUTPUT doesn't support a range
		//
		m_egNumDesc.SetColWidth(0, 0, 1100);
		m_egNumDesc.SetTextMatrix(0, 0, _T("Number"));
		m_egNumDesc.SetRowHeight(1, 0);

		CUtil::InsertPhases(&m_clcPhases, GetDatabase());
		m_egPhases.AddRowHeaders();
		m_egPhases.SetTextMatrix(0, 1, _T("Name"));
		m_egPhases.SetColWidth(1, 0, 1400);

		// center align the column headers
		m_egPhases.SetFillStyle(flexFillRepeat);
		m_egPhases.SetRow(0);
		m_egPhases.SetCol(0);
		m_egPhases.SetColSel(m_egPhases.GetCols(0) - 1);
		m_egPhases.SetCellAlignment(flexAlignCenterCenter);
		m_egPhases.SetFillStyle(flexFillSingle);


		// set bold titles
		m_egPhases.SetRow(0), m_egPhases.SetCol(1);
		m_egPhases.SetCellFontBold(TRUE);

		// set initial cell
		m_egPhases.SetRow(1), m_egPhases.SetCol(1);
	}
	if (pDX->m_bSaveAndValidate)
	{
		std::list<CString> listPhases;
		for (long nRow = m_egPhases.GetFixedRows(); nRow < m_egPhases.GetRows(); ++nRow)
		{
			CString strPhase;

			DDX_GridText(pDX, IDC_MSHFG_PHASES, nRow, 1, strPhase);
			if (strPhase.IsEmpty())
				continue;

			listPhases.push_back(strPhase);
		}


		// if here list is valid and can be assigned to the member variable
		m_listPhases.assign(listPhases.begin(), listPhases.end());

	}
	else
	{
		// see OnKillActive
		ASSERT(m_glDoc.GetCount() == 0);

		std::list<CString>::iterator iter = m_listPhases.begin();
		for (; iter != m_listPhases.end(); ++iter)
		{
			m_glDoc.AddItem(*iter);
		}
		ASSERT(m_listPhases.size() == m_glDoc.GetCount());
	}
}


BEGIN_MESSAGE_MAP(CKPSelectedOutputPg5, baseCKPSelectedOutputPg5)
	//{{AFX_MSG_MAP(CKPSelectedOutputPg5)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CL_PHASES, OnItemchangedClPhases)
	ON_NOTIFY(NM_SETFOCUS, IDC_CL_PHASES, OnSetfocusClPhases)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
	ON_MESSAGE(EGN_BEGINCELLEDIT, OnBeginCellEdit)
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
	ON_MESSAGE(EGN_SETFOCUS, OnSetfocusGrid)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKPSelectedOutputPg5 message handlers

BOOL CKPSelectedOutputPg5::OnKillActive() 
{
	if (baseCKPSelectedOutputPg5::OnKillActive())
	{
		// as last step remove all entries from m_glDoc;
		m_glDoc.DeleteAllRows();
		return TRUE;
	}
	return FALSE;
}

LRESULT CKPSelectedOutputPg5::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bDenyEdit = FALSE;	// allow edit	

	switch (nID)
	{
	case IDC_MSHFG_ACTIVITIES :
		switch ( pInfo->item.iCol )
		{
		case 1 :
			pInfo->item.bUseCombo = (CUtil::InsertPhases(pInfo->item.hWndCombo, GetDatabase()) > 0);
			break;

		default :
			break;
		}
		break;
	}

	return bDenyEdit;
}

LRESULT CKPSelectedOutputPg5::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bMakeChange = TRUE;

	// ignore if edit is cancelled
	if ( pInfo->item.pszText == NULL ) return bMakeChange;

	switch ( nID )
	{
	case IDC_MSHFG_NUM_DESC :
		break;
	}
	return bMakeChange;
}

LRESULT CKPSelectedOutputPg5::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
{
	UINT nID = wParam;
	UNUSED_ALWAYS(lParam);
	switch (nID)
	{
	case IDC_MSHFG_NUM_DESC :
		OnEnterCellMshfgNumDesc();
		break;
	case IDC_MSHFG_PHASES:
		OnEnterCellMshfgPhases();
		break;
	}
	return 0;
}

void CKPSelectedOutputPg5::OnItemchangedClPhases(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_glDoc.OnItemChanged(pNMHDR, pResult);
}

BEGIN_EVENTSINK_MAP(CKPSelectedOutputPg5, baseCKPSelectedOutputPg5)
    //{{AFX_EVENTSINK_MAP(CKPSelectedOutputPg5)
	ON_EVENT(CKPSelectedOutputPg5, IDC_MSHFG_PHASES, -602 /* KeyDown */, OnKeyDownMshfgPhases, VTS_PI2 VTS_I2)
	ON_EVENT(CKPSelectedOutputPg5, IDC_MSHFG_PHASES, 71 /* EnterCell */, OnEnterCellMshfgPhases, VTS_NONE)
	ON_EVENT(CKPSelectedOutputPg5, IDC_MSHFG_NUM_DESC, 71 /* EnterCell */, OnEnterCellMshfgNumDesc, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CKPSelectedOutputPg5::OnKeyDownMshfgPhases(short FAR* KeyCode, short Shift) 
{
	m_glDoc.OnKeyDown(KeyCode, Shift);	
}

BOOL CKPSelectedOutputPg5::OnInitDialog() 
{
	baseCKPSelectedOutputPg5::OnInitDialog();
	
	// set layout
	CreateRoot(VERTICAL, 5, 6) 
		<< (pane(HORIZONTAL, ABSOLUTE_VERT)
			<< item(IDC_MSHFG_NUM_DESC, ABSOLUTE_VERT)
			)
		<< itemFixed(VERTICAL, 1)
		<< item(IDC_S_DEFINED, NORESIZE)
		<< (pane(VERTICAL, GREEDY, 0, 0, 0)
			<< (pane(HORIZONTAL, RELATIVE_VERT, 20, 0, 80)
				<< item(IDC_CL_PHASES, GREEDY)
				<< item(IDC_MSHFG_PHASES, ABSOLUTE_HORZ)
				)
			<< itemFixed(VERTICAL, 7)
			<< (paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, RELATIVE_VERT, nDefaultBorder, 10, 10, 20)
				<< item(IDC_E_DESC_INPUT, GREEDY)
				)
			)
	;
	UpdateLayout();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


/////////////////////////////////////////////////////////////////////////////
// CKPSelectedOutputPg6 property page

IMPLEMENT_DYNCREATE(CKPSelectedOutputPg6, baseCKPSelectedOutputPg6)

CKPSelectedOutputPg6::CKPSelectedOutputPg6() : baseCKPSelectedOutputPg6(CKPSelectedOutputPg6::IDD)
, m_glDoc(&m_egPhases, &m_clcPhases, 1)
{
	//{{AFX_DATA_INIT(CKPSelectedOutputPg6)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CKPSelectedOutputPg6::~CKPSelectedOutputPg6()
{
}

void CKPSelectedOutputPg6::DoDataExchange(CDataExchange* pDX)
{
	baseCKPSelectedOutputPg6::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKPSelectedOutputPg6)
	DDX_Control(pDX, IDC_DESCRIPTION, m_eInputDesc);
	DDX_Control(pDX, IDC_CL_PHASES, m_clcPhases);
	DDX_Control(pDX, IDC_MSHFG_PHASES, m_egPhases);
	//}}AFX_DATA_MAP

	CWaitCursor wait;

	// reduce grid flicker
	CDelayUpdate update(this, IDC_MSHFG_PHASES);

	if (m_bFirstSetActive)
	{
		//
		// Init NumDesc SELECTED_OUTPUT doesn't support a range
		//
		m_egNumDesc.SetColWidth(0, 0, 1100);
		m_egNumDesc.SetTextMatrix(0, 0, _T("Number"));
		m_egNumDesc.SetRowHeight(1, 0);

		CUtil::InsertPhases(&m_clcPhases, GetDatabase());
		m_egPhases.AddRowHeaders();
		m_egPhases.SetTextMatrix(0, 1, _T("Name"));
		m_egPhases.SetColWidth(1, 0, 1400);

		// center align the column headers
		m_egPhases.SetFillStyle(flexFillRepeat);
		m_egPhases.SetRow(0);
		m_egPhases.SetCol(0);
		m_egPhases.SetColSel(m_egPhases.GetCols(0) - 1);
		m_egPhases.SetCellAlignment(flexAlignCenterCenter);
		m_egPhases.SetFillStyle(flexFillSingle);


		// set bold titles
		m_egPhases.SetRow(0), m_egPhases.SetCol(1);
		m_egPhases.SetCellFontBold(TRUE);

		// set initial cell
		m_egPhases.SetRow(1), m_egPhases.SetCol(1);
	}
	if (pDX->m_bSaveAndValidate)
	{
		std::list<CString> listPhases;
		for (long nRow = m_egPhases.GetFixedRows(); nRow < m_egPhases.GetRows(); ++nRow)
		{
			CString strPhase;

			DDX_GridText(pDX, IDC_MSHFG_PHASES, nRow, 1, strPhase);
			if (strPhase.IsEmpty())
				continue;

			listPhases.push_back(strPhase);
		}


		// if here list is valid and can be assigned to the member variable
		m_listPhases.assign(listPhases.begin(), listPhases.end());

	}
	else
	{
		// see OnKillActive
		ASSERT(m_glDoc.GetCount() == 0);

		std::list<CString>::iterator iter = m_listPhases.begin();
		for (; iter != m_listPhases.end(); ++iter)
		{
			m_glDoc.AddItem(*iter);
		}
		ASSERT(m_listPhases.size() == m_glDoc.GetCount());
	}
}


BEGIN_MESSAGE_MAP(CKPSelectedOutputPg6, baseCKPSelectedOutputPg6)
	//{{AFX_MSG_MAP(CKPSelectedOutputPg6)
	ON_NOTIFY(NM_SETFOCUS, IDC_CL_PHASES, OnSetfocusClPhases)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CL_PHASES, OnItemchangedClPhases)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
	ON_MESSAGE(EGN_BEGINCELLEDIT, OnBeginCellEdit)
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
	ON_MESSAGE(EGN_SETFOCUS, OnSetfocusGrid)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKPSelectedOutputPg6 message handlers

BOOL CKPSelectedOutputPg6::OnKillActive() 
{
	if (baseCKPSelectedOutputPg6::OnKillActive())
	{
		// as last step remove all entries from m_glDoc;
		m_glDoc.DeleteAllRows();
		return TRUE;
	}
	return FALSE;
}

LRESULT CKPSelectedOutputPg6::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bDenyEdit = FALSE;	// allow edit	

	switch (nID)
	{
	case IDC_MSHFG_PHASES :
		switch ( pInfo->item.iCol )
		{
		case 1 :
			pInfo->item.bUseCombo = (CUtil::InsertPhases(pInfo->item.hWndCombo, GetDatabase()) > 0);
			break;
		default :
			break;
		}
		break;
	}

	return bDenyEdit;
}

LRESULT CKPSelectedOutputPg6::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bMakeChange = TRUE;

	// ignore if edit is cancelled
	if ( pInfo->item.pszText == NULL ) return bMakeChange;

	switch ( nID )
	{
	case IDC_MSHFG_NUM_DESC :
		break;
	}
	return bMakeChange;
}

LRESULT CKPSelectedOutputPg6::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
{
	UINT nID = wParam;
	UNUSED_ALWAYS(lParam);
	switch (nID)
	{
	case IDC_MSHFG_NUM_DESC :
		OnEnterCellMshfgNumDesc();
		break;
	case IDC_MSHFG_PHASES:
		OnEnterCellMshfgPhases();
		break;
	}
	return 0;
}

void CKPSelectedOutputPg6::OnItemchangedClPhases(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_glDoc.OnItemChanged(pNMHDR, pResult);
}

BEGIN_EVENTSINK_MAP(CKPSelectedOutputPg6, baseCKPSelectedOutputPg6)
    //{{AFX_EVENTSINK_MAP(CKPSelectedOutputPg6)
	ON_EVENT(CKPSelectedOutputPg6, IDC_MSHFG_PHASES, -602 /* KeyDown */, OnKeyDownMshfgPhases, VTS_PI2 VTS_I2)
	ON_EVENT(CKPSelectedOutputPg6, IDC_MSHFG_PHASES, 71 /* EnterCell */, OnEnterCellMshfgPhases, VTS_NONE)
	ON_EVENT(CKPSelectedOutputPg6, IDC_MSHFG_NUM_DESC, 71 /* EnterCell */, OnEnterCellMshfgNumDesc, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CKPSelectedOutputPg6::OnKeyDownMshfgPhases(short FAR* KeyCode, short Shift) 
{
	m_glDoc.OnKeyDown(KeyCode, Shift);	
}

BOOL CKPSelectedOutputPg6::OnInitDialog() 
{
	baseCKPSelectedOutputPg6::OnInitDialog();
	
	// set layout
	CreateRoot(VERTICAL, 5, 6) 
		<< (pane(HORIZONTAL, ABSOLUTE_VERT)
			<< item(IDC_MSHFG_NUM_DESC, ABSOLUTE_VERT)
			)
		<< itemFixed(VERTICAL, 1)
		<< item(IDC_S_DEFINED, NORESIZE)
		<< (pane(VERTICAL, GREEDY, 0, 0, 0)
			<< (pane(HORIZONTAL, RELATIVE_VERT, 20, 0, 80)
				<< item(IDC_CL_PHASES, GREEDY)
				<< item(IDC_MSHFG_PHASES, ABSOLUTE_HORZ)
				)
			<< itemFixed(VERTICAL, 7)
			<< (paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, RELATIVE_VERT, nDefaultBorder, 10, 10, 20)
				<< item(IDC_E_DESC_INPUT, GREEDY)
				)
			)
	;
	UpdateLayout();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// CKPSelectedOutputPg7 property page

IMPLEMENT_DYNCREATE(CKPSelectedOutputPg7, baseCKPSelectedOutputPg7)

CKPSelectedOutputPg7::CKPSelectedOutputPg7() : baseCKPSelectedOutputPg7(CKPSelectedOutputPg7::IDD)
, m_glDoc(&m_egGases, &m_clcGases, 1)
{
	//{{AFX_DATA_INIT(CKPSelectedOutputPg7)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CKPSelectedOutputPg7::~CKPSelectedOutputPg7()
{
}

void CKPSelectedOutputPg7::DoDataExchange(CDataExchange* pDX)
{
	baseCKPSelectedOutputPg7::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKPSelectedOutputPg7)
	DDX_Control(pDX, IDC_DESCRIPTION, m_eInputDesc);
	DDX_Control(pDX, IDC_CL_GASES, m_clcGases);
	DDX_Control(pDX, IDC_MSHFG_GASES, m_egGases);
	//}}AFX_DATA_MAP

	CWaitCursor wait;

	// reduce grid flicker
	CDelayUpdate update(this, IDC_MSHFG_GASES);

	if (m_bFirstSetActive)
	{
		//
		// Init NumDesc SELECTED_OUTPUT doesn't support a range
		//
		m_egNumDesc.SetColWidth(0, 0, 1100);
		m_egNumDesc.SetTextMatrix(0, 0, _T("Number"));
		m_egNumDesc.SetRowHeight(1, 0);

		CUtil::InsertGases(&m_clcGases, GetDatabase());
		m_egGases.AddRowHeaders();
		m_egGases.SetTextMatrix(0, 1, _T("Name"));
		m_egGases.SetColWidth(1, 0, 1400);

		// center align the column headers
		m_egGases.SetFillStyle(flexFillRepeat);
		m_egGases.SetRow(0);
		m_egGases.SetCol(0);
		m_egGases.SetColSel(m_egGases.GetCols(0) - 1);
		m_egGases.SetCellAlignment(flexAlignCenterCenter);
		m_egGases.SetFillStyle(flexFillSingle);


		// set bold titles
		m_egGases.SetRow(0), m_egGases.SetCol(1);
		m_egGases.SetCellFontBold(TRUE);

		// set initial cell
		m_egGases.SetRow(1), m_egGases.SetCol(1);
	}
	if (pDX->m_bSaveAndValidate)
	{
		std::list<CString> listGases;
		for (long nRow = m_egGases.GetFixedRows(); nRow < m_egGases.GetRows(); ++nRow)
		{
			CString strGas;

			DDX_GridText(pDX, IDC_MSHFG_GASES, nRow, 1, strGas);
			if (strGas.IsEmpty())
				continue;

// COMMENT: {3/8/2001 1:14:10 PM}			// validate strGas
// COMMENT: {3/8/2001 1:14:10 PM}			if (strGas.GetLength() < 4)
// COMMENT: {3/8/2001 1:14:10 PM}			{
// COMMENT: {3/8/2001 1:14:10 PM}				DDX_GridFail(pDX, _T("Expected gas name to end with the substring \"(g)\"."),
// COMMENT: {3/8/2001 1:14:10 PM}					_T("Error"));
// COMMENT: {3/8/2001 1:14:10 PM}			}
// COMMENT: {3/8/2001 1:14:10 PM}                  l
// COMMENT: {3/8/2001 1:14:10 PM}			CString strSuffix = strGas.Right(3);
// COMMENT: {3/8/2001 1:14:10 PM}
// COMMENT: {3/8/2001 1:14:10 PM}			// validate strGas
// COMMENT: {3/8/2001 1:14:10 PM}			if (strSuffix != _T("(g)")
// COMMENT: {3/8/2001 1:14:10 PM}			{
// COMMENT: {3/8/2001 1:14:10 PM}				DDX_GridFail(pDX, _T("Expected gas name to end with the substring \"(g)\"."),
// COMMENT: {3/8/2001 1:14:10 PM}					_T("Error"));
// COMMENT: {3/8/2001 1:14:10 PM}			}

			listGases.push_back(strGas);
		}


		// if here list is valid and can be assigned to the member variable
		m_listGases.assign(listGases.begin(), listGases.end());

	}
	else
	{
		// see OnKillActive
		ASSERT(m_glDoc.GetCount() == 0);

		std::list<CString>::iterator iter = m_listGases.begin();
		for (; iter != m_listGases.end(); ++iter)
		{
			m_glDoc.AddItem(*iter);
		}
		ASSERT(m_listGases.size() == m_glDoc.GetCount());
	}
}


BEGIN_MESSAGE_MAP(CKPSelectedOutputPg7, baseCKPSelectedOutputPg7)
	//{{AFX_MSG_MAP(CKPSelectedOutputPg7)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CL_GASES, OnItemchangedClGases)
	ON_NOTIFY(NM_SETFOCUS, IDC_CL_GASES, OnSetfocusClGases)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
	ON_MESSAGE(EGN_BEGINCELLEDIT, OnBeginCellEdit)
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
	ON_MESSAGE(EGN_SETFOCUS, OnSetfocusGrid)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKPSelectedOutputPg7 message handlers

BOOL CKPSelectedOutputPg7::OnKillActive() 
{
	if (baseCKPSelectedOutputPg7::OnKillActive())
	{
		// as last step remove all entries from m_glDoc;
		m_glDoc.DeleteAllRows();
		return TRUE;
	}
	return FALSE;
}

LRESULT CKPSelectedOutputPg7::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bDenyEdit = FALSE;	// allow edit	

	switch (nID)
	{
	case IDC_MSHFG_GASES :
		switch ( pInfo->item.iCol )
		{
		case 1 :
			pInfo->item.bUseCombo = (CUtil::InsertGases(pInfo->item.hWndCombo, GetDatabase()) > 0);
			break;
		default :
			break;
		}
		break;
	}

	return bDenyEdit;
}

LRESULT CKPSelectedOutputPg7::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bMakeChange = TRUE;

	// ignore if edit is cancelled
	if ( pInfo->item.pszText == NULL ) return bMakeChange;

	switch ( nID )
	{
	case IDC_MSHFG_NUM_DESC :
		break;
	}
	return bMakeChange;
}

LRESULT CKPSelectedOutputPg7::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
{
	UINT nID = wParam;
	UNUSED_ALWAYS(lParam);
	switch (nID)
	{
	case IDC_MSHFG_NUM_DESC :
		OnEnterCellMshfgNumDesc();
		break;
	case IDC_MSHFG_GASES:
		OnEnterCellMshfgGases();
		break;
	}
	return 0;
}

void CKPSelectedOutputPg7::OnItemchangedClGases(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_glDoc.OnItemChanged(pNMHDR, pResult);
}

BEGIN_EVENTSINK_MAP(CKPSelectedOutputPg7, baseCKPSelectedOutputPg7)
    //{{AFX_EVENTSINK_MAP(CKPSelectedOutputPg7)
	ON_EVENT(CKPSelectedOutputPg7, IDC_MSHFG_GASES, -602 /* KeyDown */, OnKeyDownMshfgGases, VTS_PI2 VTS_I2)
	ON_EVENT(CKPSelectedOutputPg7, IDC_MSHFG_GASES, 71 /* EnterCell */, OnEnterCellMshfgGases, VTS_NONE)
	ON_EVENT(CKPSelectedOutputPg7, IDC_MSHFG_NUM_DESC, 71 /* EnterCell */, OnEnterCellMshfgNumDesc, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CKPSelectedOutputPg7::OnKeyDownMshfgGases(short FAR* KeyCode, short Shift) 
{
	m_glDoc.OnKeyDown(KeyCode, Shift);	
}

BOOL CKPSelectedOutputPg7::OnInitDialog() 
{
	baseCKPSelectedOutputPg7::OnInitDialog();
	
	// set layout
	CreateRoot(VERTICAL, 5, 6) 
		<< (pane(HORIZONTAL, ABSOLUTE_VERT)
			<< item(IDC_MSHFG_NUM_DESC, ABSOLUTE_VERT)
			)
		<< itemFixed(VERTICAL, 1)
		<< item(IDC_S_DEFINED, NORESIZE)
		<< (pane(VERTICAL, GREEDY, 0, 0, 0)
			<< (pane(HORIZONTAL, RELATIVE_VERT, 20, 0, 80)
				<< item(IDC_CL_GASES, GREEDY)
				<< item(IDC_MSHFG_GASES, ABSOLUTE_HORZ)
				)
			<< itemFixed(VERTICAL, 7)
			<< (paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, RELATIVE_VERT, nDefaultBorder, 10, 10, 20)
				<< item(IDC_E_DESC_INPUT, GREEDY)
				)
			)
	;
	UpdateLayout();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CKPSelectedOutputPg2::OnKillActive() 
{
	if (baseCKPSelectedOutputPg2::OnKillActive())
	{
		// as last step remove all entries from m_glDoc;
		m_glDoc.DeleteAllRows();
		return TRUE;
	}
	return FALSE;
}

void CKPSelectedOutputPg2::OnSetfocusClTotals(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNUSED(pNMHDR);

	CString strRes;
	strRes.LoadString(IDS_STRING389);
	m_eInputDesc.SetWindowText(strRes);

	// set default selection
	// this gives user feedback when the list recieves the focus
	if (m_clcTotals.GetItemCount() && m_clcTotals.GetFirstSelectedItemPosition() == NULL)
	{
		m_clcTotals.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}
	
	*pResult = 0;
}

void CKPSelectedOutputPg5::OnSetfocusClPhases(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNUSED(pNMHDR);

	CString strRes;
	strRes.LoadString(IDS_STRING392);
	m_eInputDesc.SetWindowText(strRes);

	// set default selection
	// this gives user feedback when the list recieves the focus
	if (m_clcPhases.GetItemCount() && m_clcPhases.GetFirstSelectedItemPosition() == NULL)
	{
		m_clcPhases.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}
	
	*pResult = 0;
}

void CKPSelectedOutputPg6::OnSetfocusClPhases(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNUSED(pNMHDR);

	CString strRes;
	strRes.LoadString(IDS_STRING393);
	m_eInputDesc.SetWindowText(strRes);

	// set default selection
	// this gives user feedback when the list recieves the focus
	if (m_clcPhases.GetItemCount() && m_clcPhases.GetFirstSelectedItemPosition() == NULL)
	{
		m_clcPhases.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}
	
	*pResult = 0;
}

void CKPSelectedOutputPg7::OnSetfocusClGases(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNUSED(pNMHDR);

	CString strRes;
	strRes.LoadString(IDS_STRING394);
	m_eInputDesc.SetWindowText(strRes);

	// set default selection
	// this gives user feedback when the list recieves the focus
	if (m_clcGases.GetItemCount() && m_clcGases.GetFirstSelectedItemPosition() == NULL)
	{
		m_clcGases.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}
	
	*pResult = 0;
}

BOOL CKPSelectedOutputPg1::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// Declare the popup structure and initialize it.
	HH_POPUP        myPopup;
	memset(&myPopup, 0, sizeof(HH_POPUP));
	
	// Fill in the popup structure
	myPopup.cbStruct         = sizeof(HH_POPUP);
	myPopup.rcMargins.top    = 5;
	myPopup.rcMargins.bottom = 5;
	myPopup.rcMargins.left   = 5;
	myPopup.rcMargins.right  = 5;
	myPopup.clrForeground    = (COLORREF)-1;  // use default
	myPopup.clrBackground    = (COLORREF)-1;  // use default
	myPopup.idString         = 0;             // set the ID string to 0

	// Set placement
	if (IsContextHelp())
	{
		myPopup.pt.x = pHelpInfo->MousePos.x;
		myPopup.pt.y = pHelpInfo->MousePos.y;
	}
	else
	{
		if (pHelpInfo->hItemHandle)
		{
			// Center pt of window w/ offset
			CRect rc;
			::GetWindowRect((HWND)pHelpInfo->hItemHandle, &rc);
			myPopup.pt = rc.CenterPoint();
			myPopup.pt.y += 10;
		}
	}

	CString strRes;

	// hack since IDC_S_XX are non-contiguous
	switch (pHelpInfo->iCtrlId)
	{
	case IDC_S_0:
		pHelpInfo->iCtrlId = IDC_CBO_SE_00;
		break;
	case IDC_S_1:
		pHelpInfo->iCtrlId = IDC_CBO_SE_01;
		break;
	case IDC_S_2:
		pHelpInfo->iCtrlId = IDC_CBO_SE_02;
		break;
	case IDC_S_3:
		pHelpInfo->iCtrlId = IDC_CBO_SE_03;
		break;
	case IDC_S_4:
		pHelpInfo->iCtrlId = IDC_CBO_SE_04;
		break;
	case IDC_S_5:
		pHelpInfo->iCtrlId = IDC_CBO_SE_05;
		break;
	case IDC_S_6:
		pHelpInfo->iCtrlId = IDC_CBO_SE_06;
		break;
	case IDC_S_7:
		pHelpInfo->iCtrlId = IDC_CBO_SE_07;
		break;
	case IDC_S_8:
		pHelpInfo->iCtrlId = IDC_CBO_SE_08;
		break;
	case IDC_S_9:
		pHelpInfo->iCtrlId = IDC_CBO_SE_09;
		break;
	case IDC_S_10:
		pHelpInfo->iCtrlId = IDC_CBO_SE_10;
		break;
	case IDC_S_11:
		pHelpInfo->iCtrlId = IDC_CBO_SE_11;
		break;
	case IDC_S_12:
		pHelpInfo->iCtrlId = IDC_CBO_SE_12;
		break;
	case IDC_S_13:
		pHelpInfo->iCtrlId = IDC_CBO_SE_13;
		break;
	case IDC_S_14:
		pHelpInfo->iCtrlId = IDC_CBO_SE_14;
		break;
	case IDC_S_15:
		pHelpInfo->iCtrlId = IDC_CBO_SE_15;
		break;
	case IDC_S_16:
		pHelpInfo->iCtrlId = IDC_CBO_SE_16;
		break;
	case IDC_S_17:
		pHelpInfo->iCtrlId = IDC_CBO_SE_17;
		break;
	case IDC_S_18:
		pHelpInfo->iCtrlId = IDC_CBO_SE_18;
		break;
	default:
		// do nothing
		break;
	}

	if (IDC_CBO_SE_00 <= pHelpInfo->iCtrlId && pHelpInfo->iCtrlId <= IDC_CBO_SE_18)
	{
		strRes.LoadString(pHelpInfo->iCtrlId - IDC_CBO_SE_00 + IDS_STRING359);
	}
	else if(IDC_B_AS_IS <= pHelpInfo->iCtrlId && pHelpInfo->iCtrlId <= IDC_B_FALSE)
	{
		strRes.LoadString(pHelpInfo->iCtrlId - IDC_B_AS_IS + IDS_STRING385);
	}
	else
	{
		switch (pHelpInfo->iCtrlId)
		{
		case IDC_BROWSE:
			strRes.LoadString(IDS_STRING447);
			break;
		case IDC_FILE_NAME: case IDC_S_FILENAME:
			strRes.LoadString(IDS_SEL_OUT_358);
			break;
		case IDC_MSHFG_NUM_DESC:
			if (!IsContextHelp())
			{
				if (!(m_egNumDesc.GetRowIsVisible(m_egNumDesc.GetRow()) && m_egNumDesc.GetColIsVisible(m_egNumDesc.GetCol())))
				{
					::MessageBeep((UINT)-1);
					return TRUE;
				}

				// modify placement
				CDC* pDC = m_egNumDesc.GetDC();
				int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
				int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

				long nLeft = ::MulDiv(m_egNumDesc.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
				long nTop  = ::MulDiv(m_egNumDesc.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

				CRect rc;
				m_egNumDesc.GetWindowRect(rc);

				myPopup.pt.x = rc.left + nLeft;
				myPopup.pt.y = rc.top + nTop + 10;
			}
			switch (IsContextHelp() ? m_egNumDesc.GetMouseRow() : m_egNumDesc.GetRow())
			{
			case 0 :
				strRes.LoadString(IDS_STRING751);
				break;
			case 2 :
				strRes.LoadString(IDS_STRING752);
				break;
			default:
				// No help topic is associated with this item. 
				strRes.LoadString(IDS_STRING441);
				break;
			}
			break;
		default:
			// No help topic is associated with this item. 
			strRes.LoadString(IDS_STRING441);
			break;
		}
	}
	myPopup.pszText = strRes;
	return ::HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}

void CKPSelectedOutputPg1::OnEnterCellMshfgNumDesc() 
{
	CString strRes;
	switch (m_egNumDesc.GetRow())
	{
	case 0 :
		strRes.LoadString(IDS_STRING751);
		break;
	case 2 :
		strRes.LoadString(IDS_STRING752);
		break;
	}
	m_eInputDesc.SetWindowText(strRes);
}

BOOL CKPSelectedOutputPg2::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// Declare the popup structure and initialize it.
	HH_POPUP        myPopup;
	memset(&myPopup, 0, sizeof(HH_POPUP));
	
	// Fill in the popup structure
	myPopup.cbStruct         = sizeof(HH_POPUP);
	myPopup.rcMargins.top    = 5;
	myPopup.rcMargins.bottom = 5;
	myPopup.rcMargins.left   = 5;
	myPopup.rcMargins.right  = 5;
	myPopup.clrForeground    = (COLORREF)-1;  // use default
	myPopup.clrBackground    = (COLORREF)-1;  // use default
	myPopup.idString         = 0;             // set the ID string to 0

	// Set placement
	if (IsContextHelp())
	{
		myPopup.pt.x = pHelpInfo->MousePos.x;
		myPopup.pt.y = pHelpInfo->MousePos.y;
	}
	else
	{
		if (pHelpInfo->hItemHandle)
		{
			// Center pt of window w/ offset
			CRect rc;
			::GetWindowRect((HWND)pHelpInfo->hItemHandle, &rc);
			myPopup.pt = rc.CenterPoint();
			myPopup.pt.y += 10;
		}
	}

	CString strRes;

	switch (pHelpInfo->iCtrlId)
	{
	case IDC_MSHFG_TOTALS:
		strRes.LoadString(IDS_STRING513);
		break;
	case IDC_CL_TOTALS: case IDC_S_DEFINED:
		strRes.LoadString(IDS_STRING389);
		break;
	case IDC_MSHFG_NUM_DESC:
		if (!IsContextHelp())
		{
			if (!(m_egNumDesc.GetRowIsVisible(m_egNumDesc.GetRow()) && m_egNumDesc.GetColIsVisible(m_egNumDesc.GetCol())))
			{
				::MessageBeep((UINT)-1);
				return TRUE;
			}

			// modify placement
			CDC* pDC = m_egNumDesc.GetDC();
			int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
			int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

			long nLeft = ::MulDiv(m_egNumDesc.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
			long nTop  = ::MulDiv(m_egNumDesc.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

			CRect rc;
			m_egNumDesc.GetWindowRect(rc);

			myPopup.pt.x = rc.left + nLeft;
			myPopup.pt.y = rc.top + nTop + 10;
		}
		switch (IsContextHelp() ? m_egNumDesc.GetMouseRow() : m_egNumDesc.GetRow())
		{
		case 0 :
			strRes.LoadString(IDS_STRING751);
			break;
		case 2 :
			strRes.LoadString(IDS_STRING752);
			break;
		default:
			// No help topic is associated with this item. 
			strRes.LoadString(IDS_STRING441);
			break;
		}
		break;
	default:
		// No help topic is associated with this item. 
		strRes.LoadString(IDS_STRING441);
		break;
	}

	myPopup.pszText = strRes;
	return ::HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}

void CKPSelectedOutputPg2::OnEnterCellMshfgNumDesc() 
{
	CString strRes;
	switch (m_egNumDesc.GetRow())
	{
	case 0 :
		strRes.LoadString(IDS_STRING751);
		break;
	case 2 :
		strRes.LoadString(IDS_STRING752);
		break;
	}
	m_eInputDesc.SetWindowText(strRes);
}

BOOL CKPSelectedOutputPg3::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// Declare the popup structure and initialize it.
	HH_POPUP        myPopup;
	memset(&myPopup, 0, sizeof(HH_POPUP));
	
	// Fill in the popup structure
	myPopup.cbStruct         = sizeof(HH_POPUP);
	myPopup.rcMargins.top    = 5;
	myPopup.rcMargins.bottom = 5;
	myPopup.rcMargins.left   = 5;
	myPopup.rcMargins.right  = 5;
	myPopup.clrForeground    = (COLORREF)-1;  // use default
	myPopup.clrBackground    = (COLORREF)-1;  // use default
	myPopup.idString         = 0;             // set the ID string to 0

	// Set placement
	if (IsContextHelp())
	{
		myPopup.pt.x = pHelpInfo->MousePos.x;
		myPopup.pt.y = pHelpInfo->MousePos.y;
	}
	else
	{
		if (pHelpInfo->hItemHandle)
		{
			// Center pt of window w/ offset
			CRect rc;
			::GetWindowRect((HWND)pHelpInfo->hItemHandle, &rc);
			myPopup.pt = rc.CenterPoint();
			myPopup.pt.y += 10;
		}
	}

	CString strRes;

	switch (pHelpInfo->iCtrlId)
	{
	case IDC_CL_MOLALITIES: case IDC_S_DEFINED:
		strRes.LoadString(IDS_STRING390);
		break;
	case IDC_MSHFG_MOLALITIES:
		strRes.LoadString(IDS_STRING514);
		break;
	case IDC_MSHFG_NUM_DESC:
		if (!IsContextHelp())
		{
			if (!(m_egNumDesc.GetRowIsVisible(m_egNumDesc.GetRow()) && m_egNumDesc.GetColIsVisible(m_egNumDesc.GetCol())))
			{
				::MessageBeep((UINT)-1);
				return TRUE;
			}

			// modify placement
			CDC* pDC = m_egNumDesc.GetDC();
			int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
			int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

			long nLeft = ::MulDiv(m_egNumDesc.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
			long nTop  = ::MulDiv(m_egNumDesc.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

			CRect rc;
			m_egNumDesc.GetWindowRect(rc);

			myPopup.pt.x = rc.left + nLeft;
			myPopup.pt.y = rc.top + nTop + 10;
		}
		switch (IsContextHelp() ? m_egNumDesc.GetMouseRow() : m_egNumDesc.GetRow())
		{
		case 0 :
			strRes.LoadString(IDS_STRING751);
			break;
		case 2 :
			strRes.LoadString(IDS_STRING752);
			break;
		default:
			// No help topic is associated with this item. 
			strRes.LoadString(IDS_STRING441);
			break;
		}
		break;
	default:
		// No help topic is associated with this item. 
		strRes.LoadString(IDS_STRING441);
		break;
	}
	myPopup.pszText = strRes;
	return ::HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}

void CKPSelectedOutputPg3::OnEnterCellMshfgNumDesc() 
{
	CString strRes;
	switch (m_egNumDesc.GetRow())
	{
	case 0 :
		strRes.LoadString(IDS_STRING751);
		break;
	case 2 :
		strRes.LoadString(IDS_STRING752);
		break;
	}
	m_eInputDesc.SetWindowText(strRes);
}

void CKPSelectedOutputPg4::OnEnterCellMshfgActivities() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING515);
	m_eInputDesc.SetWindowText(strRes);	
}

BOOL CKPSelectedOutputPg4::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// Declare the popup structure and initialize it.
	HH_POPUP        myPopup;
	memset(&myPopup, 0, sizeof(HH_POPUP));
	
	// Fill in the popup structure
	myPopup.cbStruct         = sizeof(HH_POPUP);
	myPopup.rcMargins.top    = 5;
	myPopup.rcMargins.bottom = 5;
	myPopup.rcMargins.left   = 5;
	myPopup.rcMargins.right  = 5;
	myPopup.clrForeground    = (COLORREF)-1;  // use default
	myPopup.clrBackground    = (COLORREF)-1;  // use default
	myPopup.idString         = 0;             // set the ID string to 0

	// Set placement
	if (IsContextHelp())
	{
		myPopup.pt.x = pHelpInfo->MousePos.x;
		myPopup.pt.y = pHelpInfo->MousePos.y;
	}
	else
	{
		if (pHelpInfo->hItemHandle)
		{
			// Center pt of window w/ offset
			CRect rc;
			::GetWindowRect((HWND)pHelpInfo->hItemHandle, &rc);
			myPopup.pt = rc.CenterPoint();
			myPopup.pt.y += 10;
		}
	}

	CString strRes;
	switch (pHelpInfo->iCtrlId)
	{
	case IDC_CL_ACTIVITIES: case IDC_S_DEFINED:
		strRes.LoadString(IDS_STRING391);
		break;
	case IDC_MSHFG_ACTIVITIES:
		strRes.LoadString(IDS_STRING515);
		break;
	case IDC_MSHFG_NUM_DESC:
		if (!IsContextHelp())
		{
			if (!(m_egNumDesc.GetRowIsVisible(m_egNumDesc.GetRow()) && m_egNumDesc.GetColIsVisible(m_egNumDesc.GetCol())))
			{
				::MessageBeep((UINT)-1);
				return TRUE;
			}

			// modify placement
			CDC* pDC = m_egNumDesc.GetDC();
			int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
			int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

			long nLeft = ::MulDiv(m_egNumDesc.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
			long nTop  = ::MulDiv(m_egNumDesc.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

			CRect rc;
			m_egNumDesc.GetWindowRect(rc);

			myPopup.pt.x = rc.left + nLeft;
			myPopup.pt.y = rc.top + nTop + 10;
		}
		switch (IsContextHelp() ? m_egNumDesc.GetMouseRow() : m_egNumDesc.GetRow())
		{
		case 0 :
			strRes.LoadString(IDS_STRING751);
			break;
		case 2 :
			strRes.LoadString(IDS_STRING752);
			break;
		default:
			// No help topic is associated with this item. 
			strRes.LoadString(IDS_STRING441);
			break;
		}
		break;
	default:
		// No help topic is associated with this item. 
		strRes.LoadString(IDS_STRING441);
		break;
	}
	myPopup.pszText = strRes;
	return ::HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}

void CKPSelectedOutputPg4::OnEnterCellMshfgNumDesc() 
{
	CString strRes;
	switch (m_egNumDesc.GetRow())
	{
	case 0 :
		strRes.LoadString(IDS_STRING751);
		break;
	case 2 :
		strRes.LoadString(IDS_STRING752);
		break;
	}
	m_eInputDesc.SetWindowText(strRes);
}

void CKPSelectedOutputPg5::OnEnterCellMshfgPhases() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING516);
	m_eInputDesc.SetWindowText(strRes);	
}

BOOL CKPSelectedOutputPg5::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// Declare the popup structure and initialize it.
	HH_POPUP        myPopup;
	memset(&myPopup, 0, sizeof(HH_POPUP));
	
	// Fill in the popup structure
	myPopup.cbStruct         = sizeof(HH_POPUP);
	myPopup.rcMargins.top    = 5;
	myPopup.rcMargins.bottom = 5;
	myPopup.rcMargins.left   = 5;
	myPopup.rcMargins.right  = 5;
	myPopup.clrForeground    = (COLORREF)-1;  // use default
	myPopup.clrBackground    = (COLORREF)-1;  // use default
	myPopup.idString         = 0;             // set the ID string to 0

	// Set placement
	if (IsContextHelp())
	{
		myPopup.pt.x = pHelpInfo->MousePos.x;
		myPopup.pt.y = pHelpInfo->MousePos.y;
	}
	else
	{
		if (pHelpInfo->hItemHandle)
		{
			// Center pt of window w/ offset
			CRect rc;
			::GetWindowRect((HWND)pHelpInfo->hItemHandle, &rc);
			myPopup.pt = rc.CenterPoint();
			myPopup.pt.y += 10;
		}
	}

	CString strRes;
	switch (pHelpInfo->iCtrlId)
	{
	case IDC_CL_PHASES: case IDC_S_DEFINED:
		strRes.LoadString(IDS_STRING392);
		break;
	case IDC_MSHFG_PHASES:
		strRes.LoadString(IDS_STRING516);
		break;
	case IDC_MSHFG_NUM_DESC:
		if (!IsContextHelp())
		{
			if (!(m_egNumDesc.GetRowIsVisible(m_egNumDesc.GetRow()) && m_egNumDesc.GetColIsVisible(m_egNumDesc.GetCol())))
			{
				::MessageBeep((UINT)-1);
				return TRUE;
			}

			// modify placement
			CDC* pDC = m_egNumDesc.GetDC();
			int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
			int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

			long nLeft = ::MulDiv(m_egNumDesc.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
			long nTop  = ::MulDiv(m_egNumDesc.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

			CRect rc;
			m_egNumDesc.GetWindowRect(rc);

			myPopup.pt.x = rc.left + nLeft;
			myPopup.pt.y = rc.top + nTop + 10;
		}
		switch (IsContextHelp() ? m_egNumDesc.GetMouseRow() : m_egNumDesc.GetRow())
		{
		case 0 :
			strRes.LoadString(IDS_STRING751);
			break;
		case 2 :
			strRes.LoadString(IDS_STRING752);
			break;
		default:
			// No help topic is associated with this item. 
			strRes.LoadString(IDS_STRING441);
			break;
		}
		break;
	default:
		// No help topic is associated with this item. 
		strRes.LoadString(IDS_STRING441);
		break;
	}
	myPopup.pszText = strRes;
	return ::HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}

void CKPSelectedOutputPg5::OnEnterCellMshfgNumDesc() 
{
	CString strRes;
	switch (m_egNumDesc.GetRow())
	{
	case 0 :
		strRes.LoadString(IDS_STRING751);
		break;
	case 2 :
		strRes.LoadString(IDS_STRING752);
		break;
	}
	m_eInputDesc.SetWindowText(strRes);
}

void CKPSelectedOutputPg6::OnEnterCellMshfgPhases() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING518);
	m_eInputDesc.SetWindowText(strRes);	
}

BOOL CKPSelectedOutputPg6::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// Declare the popup structure and initialize it.
	HH_POPUP        myPopup;
	memset(&myPopup, 0, sizeof(HH_POPUP));
	
	// Fill in the popup structure
	myPopup.cbStruct         = sizeof(HH_POPUP);
	myPopup.rcMargins.top    = 5;
	myPopup.rcMargins.bottom = 5;
	myPopup.rcMargins.left   = 5;
	myPopup.rcMargins.right  = 5;
	myPopup.clrForeground    = (COLORREF)-1;  // use default
	myPopup.clrBackground    = (COLORREF)-1;  // use default
	myPopup.idString         = 0;             // set the ID string to 0

	// Set placement
	if (IsContextHelp())
	{
		myPopup.pt.x = pHelpInfo->MousePos.x;
		myPopup.pt.y = pHelpInfo->MousePos.y;
	}
	else
	{
		if (pHelpInfo->hItemHandle)
		{
			// Center pt of window w/ offset
			CRect rc;
			::GetWindowRect((HWND)pHelpInfo->hItemHandle, &rc);
			myPopup.pt = rc.CenterPoint();
			myPopup.pt.y += 10;
		}
	}

	CString strRes;
	switch (pHelpInfo->iCtrlId)
	{
	case IDC_CL_PHASES: case IDC_S_DEFINED:
		strRes.LoadString(IDS_STRING393);
		break;
	case IDC_MSHFG_PHASES:
		strRes.LoadString(IDS_STRING518);
		break;
	case IDC_MSHFG_NUM_DESC:
		if (!IsContextHelp())
		{
			if (!(m_egNumDesc.GetRowIsVisible(m_egNumDesc.GetRow()) && m_egNumDesc.GetColIsVisible(m_egNumDesc.GetCol())))
			{
				::MessageBeep((UINT)-1);
				return TRUE;
			}

			// modify placement
			CDC* pDC = m_egNumDesc.GetDC();
			int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
			int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

			long nLeft = ::MulDiv(m_egNumDesc.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
			long nTop  = ::MulDiv(m_egNumDesc.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

			CRect rc;
			m_egNumDesc.GetWindowRect(rc);

			myPopup.pt.x = rc.left + nLeft;
			myPopup.pt.y = rc.top + nTop + 10;
		}
		switch (IsContextHelp() ? m_egNumDesc.GetMouseRow() : m_egNumDesc.GetRow())
		{
		case 0 :
			strRes.LoadString(IDS_STRING751);
			break;
		case 2 :
			strRes.LoadString(IDS_STRING752);
			break;
		default:
			// No help topic is associated with this item. 
			strRes.LoadString(IDS_STRING441);
			break;
		}
		break;
	default:
		// No help topic is associated with this item. 
		strRes.LoadString(IDS_STRING441);
		break;
	}
	myPopup.pszText = strRes;
	return ::HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}

void CKPSelectedOutputPg6::OnEnterCellMshfgNumDesc() 
{
	CString strRes;
	switch (m_egNumDesc.GetRow())
	{
	case 0 :
		strRes.LoadString(IDS_STRING751);
		break;
	case 2 :
		strRes.LoadString(IDS_STRING752);
		break;
	}
	m_eInputDesc.SetWindowText(strRes);
}

void CKPSelectedOutputPg7::OnEnterCellMshfgGases() 
{
	CString strRes;
	strRes.LoadString(IDS_STRING517);
	m_eInputDesc.SetWindowText(strRes);	
}

BOOL CKPSelectedOutputPg7::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// Declare the popup structure and initialize it.
	HH_POPUP        myPopup;
	memset(&myPopup, 0, sizeof(HH_POPUP));
	
	// Fill in the popup structure
	myPopup.cbStruct         = sizeof(HH_POPUP);
	myPopup.rcMargins.top    = 5;
	myPopup.rcMargins.bottom = 5;
	myPopup.rcMargins.left   = 5;
	myPopup.rcMargins.right  = 5;
	myPopup.clrForeground    = (COLORREF)-1;  // use default
	myPopup.clrBackground    = (COLORREF)-1;  // use default
	myPopup.idString         = 0;             // set the ID string to 0

	// Set placement
	if (IsContextHelp())
	{
		myPopup.pt.x = pHelpInfo->MousePos.x;
		myPopup.pt.y = pHelpInfo->MousePos.y;
	}
	else
	{
		if (pHelpInfo->hItemHandle)
		{
			// Center pt of window w/ offset
			CRect rc;
			::GetWindowRect((HWND)pHelpInfo->hItemHandle, &rc);
			myPopup.pt = rc.CenterPoint();
			myPopup.pt.y += 10;
		}
	}

	CString strRes;
	switch (pHelpInfo->iCtrlId)
	{
	case IDC_CL_GASES: case IDC_S_DEFINED:
		strRes.LoadString(IDS_STRING394);
		break;
	case IDC_MSHFG_GASES:
		strRes.LoadString(IDS_STRING517);
		break;
	case IDC_MSHFG_NUM_DESC:
		if (!IsContextHelp())
		{
			if (!(m_egNumDesc.GetRowIsVisible(m_egNumDesc.GetRow()) && m_egNumDesc.GetColIsVisible(m_egNumDesc.GetCol())))
			{
				::MessageBeep((UINT)-1);
				return TRUE;
			}

			// modify placement
			CDC* pDC = m_egNumDesc.GetDC();
			int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
			int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

			long nLeft = ::MulDiv(m_egNumDesc.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
			long nTop  = ::MulDiv(m_egNumDesc.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

			CRect rc;
			m_egNumDesc.GetWindowRect(rc);

			myPopup.pt.x = rc.left + nLeft;
			myPopup.pt.y = rc.top + nTop + 10;
		}
		switch (IsContextHelp() ? m_egNumDesc.GetMouseRow() : m_egNumDesc.GetRow())
		{
		case 0 :
			strRes.LoadString(IDS_STRING751);
			break;
		case 2 :
			strRes.LoadString(IDS_STRING752);
			break;
		default:
			// No help topic is associated with this item. 
			strRes.LoadString(IDS_STRING441);
			break;
		}
		break;
	default:
		// No help topic is associated with this item. 
		strRes.LoadString(IDS_STRING441);
		break;
	}
	myPopup.pszText = strRes;
	return ::HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}

void CKPSelectedOutputPg7::OnEnterCellMshfgNumDesc() 
{
	CString strRes;
	switch (m_egNumDesc.GetRow())
	{
	case 0 :
		strRes.LoadString(IDS_STRING751);
		break;
	case 2 :
		strRes.LoadString(IDS_STRING752);
		break;
	}
	m_eInputDesc.SetWindowText(strRes);
}

