// SelectElementsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "phreeqci2.h"
#include "SelectElementsDlg.h"
#include "Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectElementsDlg dialog


CSelectElementsDlg::CSelectElementsDlg(const CDatabase& database, CWnd* pParent /*= NULL*/)
: CDialog(CSelectElementsDlg::IDD, pParent) , m_database(database)
{
	//{{AFX_DATA_INIT(CSelectElementsDlg)
	m_strDatabase = database.GetPathName();
	//}}AFX_DATA_INIT
}


void CSelectElementsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectElementsDlg)
	DDX_Control(pDX, IDC_LIST_ENTRIES, m_ctrlListBox);
	DDX_Control(pDX, IDC_ELEMENT_LIST, m_ctrlElements);
	DDX_Text(pDX, IDC_EDIT_DATABASE, m_strDatabase);
	//}}AFX_DATA_MAP
	if (pDX->m_bSaveAndValidate)
	{
		CWaitCursor wait;
		std::list<CString> listElements;
		CString strElement;
		for (int n = 0; n < m_ctrlListBox.GetCount(); ++n)
		{
			m_ctrlListBox.GetText(n, strElement);
			listElements.push_back(strElement);
		}
		// if here list is valid and can be assigned to the member variable
		m_listElements.assign(listElements.begin(), listElements.end());
	}
}


BEGIN_MESSAGE_MAP(CSelectElementsDlg, CDialog)
	//{{AFX_MSG_MAP(CSelectElementsDlg)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_ELEMENT_LIST, OnItemchangedElementList)
	ON_WM_CONTEXTMENU()
	ON_LBN_SETFOCUS(IDC_LIST_ENTRIES, OnSetfocusListEntries)
	//}}AFX_MSG_MAP
	// context menu check list commands
	ON_COMMAND_RANGE(ID_VIEW_AS_LIST, ID_LIST_CLEAR_ALL, OnViewChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectElementsDlg message handlers

BOOL CSelectElementsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Add extra initialization here

	CUtil::InsertAqElements(&m_ctrlElements, m_database);

	LVFINDINFO info;
	info.flags = LVFI_STRING;
	std::list<CString>::iterator iter = m_listElements.begin();
	for (; iter != m_listElements.end(); ++iter)
	{
		info.psz = iter->GetBuffer(iter->GetLength());
		int nItem = m_ctrlElements.FindItem(&info);

		if (nItem != -1)
		{
			if (m_ctrlElements.GetCheckBox(nItem) == BST_UNCHECKED)
			{
				m_ctrlElements.SetCheckBox(nItem, BST_CHECKED);
			}
		}
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectElementsDlg::OnItemchangedElementList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// Note:
	// This function IS called when items are checked from the 
	// OnInitDialog function
	//
	NMLISTVIEW* pNMListView = (NMLISTVIEW*)pNMHDR;


	if ((pNMListView->uNewState ^ pNMListView->uOldState) & LVIS_STATEIMAGEMASK)
	{
		UpdateElementList(pNMListView->iItem);
	}
	*pResult = 0;
}

void CSelectElementsDlg::UpdateElementList(int nItem) 
{
	if (nItem == -1) return;

	CString str0 = m_ctrlElements.GetItemText(nItem, 0); 
	CString str1 = m_ctrlElements.GetItemText(nItem, 1);
	ASSERT( !str1.IsEmpty() );

	LVFINDINFO info;

	switch (m_ctrlElements.GetCheckBox(nItem))
	{
	case BST_UNCHECKED:
		//{{
		m_ctrlListBox.DeleteString(m_ctrlListBox.FindStringExact(0, str0));
		//}}

		// check if redox state
		if (str1.Find(_T("("), 0) != -1)
		{
			CString strTotalElem = str1.SpanExcluding(_T("("));

			// Find total element state (nTotalElem)
			info.flags = LVFI_STRING;
			info.psz   = strTotalElem.GetBuffer(strTotalElem.GetLength());
			int nTotalElem = m_ctrlElements.FindItem(&info, -1);

			if (nTotalElem != -1)
			{
				// Note: This assumes that col 1 is sorted
				CString strRedox;
				for (int i = nTotalElem + 1; ; ++i)
				{
					strRedox = m_ctrlElements.GetItemText(i, 1);
					if (strRedox.Find(strTotalElem + _T("("), 0) != 0)
					{
						m_ctrlElements.SetCheckBox(nTotalElem, BST_UNCHECKED);
						break;
					}
					if (m_ctrlElements.GetCheckBox(i) == BST_CHECKED)
						break;
				}
			}
		}
		else
		{
			// Note: This assumes that col 1 is sorted
			CString strRedox;
			for (int i = nItem + 1; ; ++i)
			{
				strRedox = m_ctrlElements.GetItemText(i, 1);
				if (strRedox.Find(str1 + _T("("), 0) != 0)
					break;
				m_ctrlElements.SetCheckBox(i, BST_UNCHECKED);
			}
		}
		break;

	case BST_CHECKED:

		//{{
		m_ctrlListBox.AddString(str0);
		//}}

		// check if redox state
		if (str1.Find(_T("("), 0) != -1)
		{
			// turn off total element
			CString strElem = str1.SpanExcluding(_T("("));

			info.flags = LVFI_STRING;
			info.psz = strElem.GetBuffer(strElem.GetLength());
			nItem = m_ctrlElements.FindItem(&info);

			if (nItem != -1)
			{
				m_ctrlElements.SetCheckBox(nItem, BST_INDETERMINATE);
			}
		}
		else
		{
			// Note: This assumes that col 1 is sorted
			CString strRedox;
			for (int i = nItem + 1; ; ++i)
			{
				strRedox = m_ctrlElements.GetItemText(i, 1);
				if (strRedox.Find(str1 + _T("("), 0) != 0)
					break;
				m_ctrlElements.SetCheckBox(i, BST_INDETERMINATE);
			}
		}
		break;
	}
}

void CSelectElementsDlg::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	if (pWnd == &m_ctrlElements) 
	{
		if (point.x == -1 && point.y == -1)
		{
			//keystroke invocation
			CRect rect;

			pWnd->GetClientRect(rect);
			pWnd->ClientToScreen(rect);

			point = rect.TopLeft();
			point.Offset(5, 5);
		}

		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_CHECKLIST));

		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);

		pPopup->CheckMenuRadioItem(ID_VIEW_AS_LIST, ID_VIEW_AS_REPORT,
			(m_ctrlElements.GetStyle() & LVS_TYPEMASK) == LVS_LIST ? ID_VIEW_AS_LIST : ID_VIEW_AS_REPORT,
			MF_BYCOMMAND );
		
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
			this/*pWndPopupOwner*/);
	}
}

void CSelectElementsDlg::OnViewChange(UINT uID)
{
	int i, nElements;
	
	switch (uID)
	{
	case ID_VIEW_AS_LIST :
		m_ctrlElements.ModifyStyle(LVS_REPORT, LVS_LIST);
		break;

	case ID_VIEW_AS_REPORT :
		m_ctrlElements.ModifyStyle(LVS_LIST, LVS_REPORT);
		m_ctrlElements.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
		m_ctrlElements.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
		m_ctrlElements.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);
		m_ctrlElements.SetColumnWidth(3, LVSCW_AUTOSIZE_USEHEADER);
		m_ctrlElements.RedrawWindow();
		break;

	case ID_LIST_CLEAR_ALL :
		nElements = m_ctrlElements.GetItemCount();
		for (i = 0; i < nElements; ++i)
		{
			if (m_ctrlElements.GetCheckBox(i) == BST_CHECKED)
			{
				m_ctrlElements.SetCheckBox(i, BST_UNCHECKED);
			}
		}
		break;

	default :
		ASSERT(FALSE);

	}
}

void CSelectElementsDlg::OnSetfocusListEntries() 
{
	// Add your control notification handler code here
	m_ctrlElements.SetFocus();
}
