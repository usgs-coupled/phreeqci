// DumpPg1.cpp : implementation file
//

#include "stdafx.h"
#include "phreeqci2.h"
#include "DumpPg1.h"
#include "Util.h"

#include "phrq_io.h"
#include "parser.h"
#include "StorageBinList.h"
#include "Font.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


const CString CDumpPg1::s_arrStrKeys[] =
{
	_T("cells"),                // 1
	_T("equilibrium_phases"),   // 2
	_T("exchange"),             // 3
	_T("gas_phase"),            // 4
	_T("kinetics"),             // 5
	_T("mix"),                  // 6
	_T("reaction"),             // 7
	_T("reaction_pressure"),    // 8
	_T("reaction_temperature"), // 9
	_T("solid_solutions"),      // 10
	_T("solution"),             // 11
	_T("surface"),              // 12
};

const TCHAR THROUGH[] = _T("-");

/////////////////////////////////////////////////////////////////////////////
// CDumpPg1 property page

IMPLEMENT_DYNCREATE(CDumpPg1, baseDumpPg1)


CDumpPg1::CDumpPg1(CTreeCtrlNode simNode) : baseDumpPg1(CDumpPg1::IDD)
, m_ranges(simNode, true, true)
, bAll(false)
{
	for (int i = 0; i < sizeof(s_arrStrKeys)/sizeof(s_arrStrKeys[0]); ++i)
	{
		CKeyword::type n = CKeyword::GetKeywordType2(s_arrStrKeys[i]);
		CUtil::CreateRange(m_setNums[n], m_ranges[n]);
	}

#ifdef _DEBUG
	std::set<int> nums;
	ASSERT(GetRanges(nums).Compare("") == 0);
	nums.insert(1);
	ASSERT(GetRanges(nums).Compare("1") == 0);
	nums.insert(2);
	ASSERT(GetRanges(nums).Compare("1-2") == 0);
	nums.insert(3);
	ASSERT(GetRanges(nums).Compare("1-3") == 0);
	nums.insert(4);
	ASSERT(GetRanges(nums).Compare("1-4") == 0);
	nums.insert(6);
	ASSERT(GetRanges(nums).Compare("1-4 6") == 0);
	nums.insert(7);
	ASSERT(GetRanges(nums).Compare("1-4 6-7") == 0);
	nums.insert(8);
	ASSERT(GetRanges(nums).Compare("1-4 6-8") == 0);
#endif
}


CDumpPg1::~CDumpPg1()
{
}

void CDumpPg1::DoDataExchange(CDataExchange* pDX)
{
	baseDumpPg1::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDumpPg1)
	DDX_Control(pDX, IDC_E_DESC_INPUT, this->m_eDesc);
	DDX_GridControl(pDX, IDC_GRID_DELETE, this->gridDelete);
	//}}AFX_DATA_MAP
	DDX_Grid(pDX);
}


BEGIN_MESSAGE_MAP(CDumpPg1, baseDumpPg1)
	//{{AFX_MSG_MAP(CDumpPg1)
	//}}AFX_MSG_MAP
	// specialized grid notifications
	// ON_MESSAGE(EGN_KILLFOCUS, OnKillfocusEG)
	ON_WM_SIZE()
	ON_WM_HELPINFO()

	ON_BN_CLICKED(IDC_ALL_CHECK, &CDumpPg1::OnBnClickedAllCheck)
	ON_BN_SETFOCUS(IDC_ALL_CHECK, &CDumpPg1::OnBnSetfocusAllCheck)

	ON_NOTIFY(GVN_SELCHANGED, IDC_GRID_DELETE, &CDumpPg1::OnSelChangedDelete)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDumpPg1 message handlers

BEGIN_EVENTSINK_MAP(CDumpPg1, baseDumpPg1)
    //{{AFX_EVENTSINK_MAP(CDumpPg1)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CDumpPg1::DDX_Grid(CDataExchange *pDX)
{
	 // do one time initialize
	if (m_bFirstSetActive && !pDX->m_bSaveAndValidate)
	{
		InitGrid();
	}

	// get and format data from control
	if (pDX->m_bSaveAndValidate)
	{
		if (this->IsDlgButtonChecked(IDC_ALL_CHECK) == BST_CHECKED)
		{
			this->bAll = true;
		}
		else
		{
			this->bAll = false;
			ValidateGrid(pDX);
		}
	}
	// load data to control
	else
	{
		ExchangeGrid(pDX);
		if (this->bAll)
		{
			this->CheckDlgButton(IDC_ALL_CHECK, BST_CHECKED);
		}
		else
		{
			this->CheckDlgButton(IDC_ALL_CHECK, BST_UNCHECKED);
		}
		this->OnBnClickedAllCheck();
	}
}

void CDumpPg1::ExchangeGrid(CDataExchange* pDX)
{
	UNUSED(pDX);
	CString str;

	if (this->bAll) return;

	std::set<int> cells = CDumpPg1::GetCells(this->delete_info);

	// cell
	if (cells.size())
	{
		str = CDumpPg1::GetRanges(cells);
		this->gridDelete.SetItemText(1, 2, str);
	}

	// equilibrium_phases
	this->ExchangeItem(this->delete_info.Get_pp_assemblage(), 2);

	// exchange
	this->ExchangeItem(this->delete_info.Get_exchange(), 3);

	// gas_phase
	this->ExchangeItem(this->delete_info.Get_gas_phase(), 4);

	// kinetics
	this->ExchangeItem(this->delete_info.Get_kinetics(), 5);

	// mix
	this->ExchangeItem(this->delete_info.Get_mix(), 6);

	// reaction
	this->ExchangeItem(this->delete_info.Get_reaction(), 7);

	// reaction_pressure
	this->ExchangeItem(this->delete_info.Get_pressure(), 8);

	// reaction_temperature
	this->ExchangeItem(this->delete_info.Get_temperature(), 9);

	// ss_assemblage
	this->ExchangeItem(this->delete_info.Get_ss_assemblage(), 10);

	// solution
	this->ExchangeItem(this->delete_info.Get_solution(), 11);

	// surface
	this->ExchangeItem(this->delete_info.Get_surface(), 12);
}

void CDumpPg1::ExchangeItem(StorageBinListItem &item, int row)
{
	if (item.Get_defined())
	{
		CString str = CDumpPg1::GetRanges(item.Get_numbers());
		if (!str.IsEmpty())
		{
			this->gridDelete.SetItemText(row, 2, str);
		}
		else
		{
			this->gridDelete.SetCheck(row, 1, BST_CHECKED);
		}
	}
}

void CDumpPg1::ValidateGrid(CDataExchange* pDX)
{
	std::list< std::vector<int> > listCopies;
	CString str;
	CString nums;
	int index = -1;
	int index_start = -1;
	int index_end = -1;

	StorageBinList storageBin;

	bool b_all_cells = false;
	for (long row = this->gridDelete.GetFixedRowCount(); row < this->gridDelete.GetRowCount(); ++row)
	{
		::DDX_TextGridControl(pDX, IDC_GRID_DELETE, row, 2, nums);
		bool all = (this->gridDelete.GetCheck(row, 1) == BST_CHECKED);
		if (row == 1 && all)
		{
			b_all_cells = true;
			break;
		}

		if (all || !nums.IsEmpty())
		{
			CString str(s_arrStrKeys[row - this->gridDelete.GetFixedRowCount()]);
			str.Insert(0, "-");

			if (!all)
			{
				str += _T(" ");
				str += nums;
			}

			std::string str_in(str);
			std::istringstream iss_in;
			iss_in.str(str_in);
			std::ostringstream oss;

			PHRQ_io phrq_io;
			phrq_io.Set_error_ostream(&oss);
			CParser parser(iss_in, &phrq_io);
			//StorageBinList bin(parser, &phrq_io);
			storageBin.Read(parser);

			if (phrq_io.Get_io_error_count())
			{
				CString str(oss.str().c_str());
				int n;
				if ((n = str.Find("\nERROR: ")) == 0)
				{
					str = str.Mid(8);
				}
				::DDX_GridControlFail(pDX, IDC_GRID_DELETE, row, 2, str);
			}
		}
	}
	// if here storageBin is valid and can be assigned to the member variable
	if (!b_all_cells)
	{
		this->bAll = this->GetAll(storageBin);
	}
	else
	{
		this->bAll = true;
	}
	this->delete_info = storageBin;
}

void CDumpPg1::InitGrid()
{
	try
	{
		CFont *pDefFont = this->GetFont();
		this->gridDelete.SetFont(pDefFont);

		this->gridDelete.SetRowCount(sizeof(s_arrStrKeys)/sizeof(s_arrStrKeys[0]) + 1);
		this->gridDelete.SetColumnCount(3);
		this->gridDelete.SetFixedRowCount(1);
		this->gridDelete.SetFixedColumnCount(1);
		this->gridDelete.EnableTitleTips(FALSE);
		this->gridDelete.SetRowResize(FALSE);
		this->gridDelete.SetCurrentFocusCell(1, 0);

		this->gridDelete.SetGridColor(RGB(0, 0, 0));

		this->gridDelete.SetItemText(0, 0, _T("Reactant"));
		this->gridDelete.SetItemText(0, 1, _T("All")); this->gridDelete.SetItemFormat(0, 1, DT_CENTER);
		this->gridDelete.SetItemText(0, 2, _T("List of ranges"));

		int n = this->gridDelete.GetRowHeight(0);
		n -= 3;
		for(int i = 0; i < this->gridDelete.GetRowCount(); ++i)
		{
			this->gridDelete.SetRowHeight(i, n);
		}
		for(int r = 1; r < this->gridDelete.GetRowCount(); ++r)
		{
			this->gridDelete.SetCheck(r, 1, BST_UNCHECKED);
		}
		for (int i = 0; i < sizeof(s_arrStrKeys)/sizeof(s_arrStrKeys[0]); ++i)
		{
			this->gridDelete.SetItemText(i + 1, 0, s_arrStrKeys[i]);
		}
		this->gridDelete.AutoSizeColumn(0);
		this->gridDelete.AutoSizeColumn(1);
	}
	catch (CMemoryException *e)
	{
		e->ReportError();
		e->Delete();
	}
}

int CDumpPg1::FillKeywords(HWND hWndCombo)
{
	CComboBox* pCombo = CUtil::PrepareCombo(hWndCombo);
	CDC* pDC = CUtil::PrepareDC(pCombo);
	CSize size(0, 0);
	long nWidest = 0;

	if (pCombo)
	{
		for (int i = 0; i < sizeof(s_arrStrKeys)/sizeof(s_arrStrKeys[0]); ++i)
		{
			pCombo->AddString(s_arrStrKeys[i]);

			size = pDC->GetTextExtent(s_arrStrKeys[i]);
			if (nWidest < size.cx )
			{
				nWidest = size.cx;
			}
		}
	}
	if (pCombo)
	{
		pCombo->SetDroppedWidth(nWidest);
		return pCombo->GetCount();
	}
	return 0;
}

BOOL CDumpPg1::OnInitDialog()
{
	baseDumpPg1::OnInitDialog();

	// set layout
	CreateRoot(VERTICAL, 5, 6)
		<< item(&this->gridDelete, ABSOLUTE_VERT)
		<< item(IDC_ALL_CHECK, ABSOLUTE_VERT)
		<< ( paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, GREEDY, nDefaultBorder, 10, 10)
			<< item(IDC_E_DESC_INPUT, GREEDY)
		);
	UpdateLayout();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDumpPg1::OnSize(UINT nType, int cx, int cy)
{
	CKeywordPage::OnSize(nType, cx, cy);

	// Add your message handler code here

	if (this->gridDelete.GetSafeHwnd())
	{
		this->gridDelete.SetRedraw(false, false);

		long nCol0 = this->gridDelete.GetColumnWidth(0);
		long nCol1 = this->gridDelete.GetColumnWidth(1);

		CRect rect;
		CDC* pDC = GetDC();
		this->gridDelete.GetClientRect(&rect);
		this->gridDelete.SetColumnWidth(2, rect.right - nCol0 - nCol1);
		this->gridDelete.SetColumnWidth(2, rect.right - nCol0 - nCol1 - 1);
		this->ModifyStyle(WS_VSCROLL | WS_HSCROLL, 0, SWP_NOSIZE);
		this->gridDelete.SetRedraw(true, true);
	}
}

#ifdef SAVE_IMPERATIVE
CString CDumpPg1::GetRanges(std::set<int>& nums)
{
	CString str;
	std::set<int>::iterator it = nums.begin();
	while (it != nums.end())
	{
		if (str.GetLength()) str += " ";
		int start = *(it++);
		int end   = start;
		while (it != nums.end() && (*(it) == end + 1))
		{
			end = *(it++);
		}
		if (start != end)
		{
			str.Format("%s%d-%d", str, start, end);
		}
		else
		{
			str.Format("%s%d", str, end);
		}
	}
	return str;
}
#endif

CString CDumpPg1::GetRanges(std::set<int>& nums)
{
	CString str;
	return GetRanges0(nums, str);
}

CString CDumpPg1::GetRanges0(std::set<int> nums, CString acc)
{
	if (nums.empty())
	{
		return acc;
	}
	std::set<int>::iterator it = nums.begin();
	while (it != nums.end())
	{
		if (acc.GetLength()) acc += _T(" ");
		int start = *(it++);
		int end   = start;
		while (it != nums.end() && *(it) == end + 1)
		{
			end = *(it++);
		}
		if (start != end)
		{
			acc.Format(_T("%s%d-%d"), acc, start, end);
		}
		else
		{
			acc.Format(_T("%s%d"), acc, end);
		}
	}
	return GetRanges0(std::set<int>(it, nums.end()), acc);
}

bool CDumpPg1::Add(StorageBinListItem& item)
{
	return item.Get_defined() && !item.Get_numbers().empty();
}

std::set<int> CDumpPg1::GetCells(StorageBinList& bin)
{
	std::set<int> intersection;
	std::list< std::set<int>* > sets;

	if (Add(bin.Get_exchange()))      sets.push_back(&bin.Get_exchange().Get_numbers());
	if (Add(bin.Get_gas_phase()))     sets.push_back(&bin.Get_gas_phase().Get_numbers());
	if (Add(bin.Get_exchange()))      sets.push_back(&bin.Get_kinetics().Get_numbers());
	if (Add(bin.Get_mix()))           sets.push_back(&bin.Get_mix().Get_numbers());
	if (Add(bin.Get_pp_assemblage())) sets.push_back(&bin.Get_pp_assemblage().Get_numbers());
	if (Add(bin.Get_pressure()))      sets.push_back(&bin.Get_pressure().Get_numbers());
	if (Add(bin.Get_reaction()))      sets.push_back(&bin.Get_reaction().Get_numbers());
	if (Add(bin.Get_solution()))      sets.push_back(&bin.Get_solution().Get_numbers());
	if (Add(bin.Get_ss_assemblage())) sets.push_back(&bin.Get_ss_assemblage().Get_numbers());
	if (Add(bin.Get_surface()))       sets.push_back(&bin.Get_surface().Get_numbers());
	if (Add(bin.Get_temperature()))   sets.push_back(&bin.Get_temperature().Get_numbers());

	std::list< std::set<int>* >::iterator list_iter = sets.begin();
	for (; list_iter != sets.end(); ++list_iter)
	{
		std::set<int>::iterator set_iter = (*list_iter)->begin();
		for (; set_iter != (*list_iter)->end(); ++set_iter)
		{
			// inefficient for now
			bool in_all = true;
			std::list< std::set<int>* >::iterator rest_iter = sets.begin();
			for (; rest_iter != sets.end(); ++rest_iter)
			{
				if ((*rest_iter)->find(*set_iter) == (*rest_iter)->end())
				{
					in_all = false;
					break;
				}
			}
			if (in_all)
			{
				intersection.insert(*set_iter);
			}
		}
	}

	// remove cells
	list_iter = sets.begin();
	for (; list_iter != sets.end(); ++list_iter)
	{
		std::set<int>::iterator int_iter = intersection.begin();
		for (; int_iter != intersection.end(); ++int_iter)
		{
			std::set<int>::iterator it = (*list_iter)->find(*int_iter);
			(*list_iter)->erase((*list_iter)->find(*int_iter));
		}
	}
	return intersection;
}

void CDumpPg1::OnBnClickedAllCheck()
{
	if (this->IsDlgButtonChecked(IDC_ALL_CHECK) == BST_CHECKED)
	{
		for (int r = 1; r < this->gridDelete.GetRowCount(); ++r)
		{
			this->gridDelete.DisableCell(r, 1);
			this->gridDelete.DisableCell(r, 2);
		}
		this->RedrawWindow();
		this->gridDelete.EnableWindow(FALSE);
	}
	else
	{
		for (int r = 1; r < this->gridDelete.GetRowCount(); ++r)
		{
			this->gridDelete.EnableCell(r, 1);
			this->gridDelete.EnableCell(r, 2);
		}
		this->RedrawWindow();
		this->gridDelete.EnableWindow(TRUE);
	}
}

void CDumpPg1::OnBnSetfocusAllCheck()
{
	// TODO: Add your control notification handler code here
}

bool CDumpPg1::GetAll(StorageBinList bin)
{
	return
		bin.Get_solution().Get_defined()      &&  bin.Get_solution().Get_numbers().empty()      &&
		bin.Get_pp_assemblage().Get_defined() &&  bin.Get_pp_assemblage().Get_numbers().empty() &&
		bin.Get_exchange().Get_defined()      &&  bin.Get_exchange().Get_numbers().empty()      &&
		bin.Get_surface().Get_defined()       &&  bin.Get_surface().Get_numbers().empty()       &&
		bin.Get_ss_assemblage().Get_defined() &&  bin.Get_ss_assemblage().Get_numbers().empty() &&
		bin.Get_gas_phase().Get_defined()     &&  bin.Get_gas_phase().Get_numbers().empty()     &&
		bin.Get_kinetics().Get_defined()      &&  bin.Get_kinetics().Get_numbers().empty()      &&
		bin.Get_mix().Get_defined()           &&  bin.Get_mix().Get_numbers().empty()           &&
		bin.Get_reaction().Get_defined()      &&  bin.Get_reaction().Get_numbers().empty()      &&
		bin.Get_temperature().Get_defined()   &&  bin.Get_temperature().Get_numbers().empty()   &&
		bin.Get_pressure().Get_defined()      &&  bin.Get_pressure().Get_numbers().empty()      ;
}

void CDumpPg1::OnSelChangedDelete(NMHDR *pNotifyStruct, LRESULT *result)
{
	UNUSED(pNotifyStruct);
	UNUSED(result);

	CCellID focus = this->gridDelete.GetFocusCell();

	if (!this->gridDelete.IsValid(focus)) return;

	CString strRes;
	switch (focus.col)
	{
	case 0:
		ASSERT(FALSE);
		break;
	case 1:
		// Select this option to delete all %1 definitions.
		strRes = CDumpPg1::GetHelpString(focus.row, focus.col);
		break;
	case 2:
		// List of number ranges. The number ranges may be a single integer or a range defined by an integer, a hyphen, and an integer, without intervening spaces.  %1 identified by any of the numbers in the list will be deleted.
		strRes = CDumpPg1::GetHelpString(focus.row, focus.col);
		break;
	default:
		ASSERT(FALSE);
		break;
	}
	this->m_eDesc.SetWindowText(strRes);
}

BOOL CDumpPg1::OnHelpInfo(HELPINFO* pHelpInfo)
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
	case IDC_GRID_DELETE:
		if (!IsContextHelp())
		{
			CCellID id = this->gridDelete.GetFocusCell();
			this->gridDelete.GetCellOrigin(id, &myPopup.pt);
			this->gridDelete.ClientToScreen(&myPopup.pt);

			if (id.row > 0 && id.col > 0)
			{
				strRes = CDumpPg1::GetHelpString(id.row, id.col);
			}
		}

		if (IsContextHelp())
		{
			CPoint pt(pHelpInfo->MousePos);
			this->gridDelete.ScreenToClient(&pt);
			int sum = 0;
			int col = -1;
			for (int c = 0; c < this->gridDelete.GetColumnCount(); ++c)
			{
				if (pt.x <= (sum += this->gridDelete.GetColumnWidth(c)))
				{
					col = c;
					break;
				}
			}
			sum = 0;
			int row = -1;
			for (int r = 0; r < this->gridDelete.GetRowCount(); ++r)
			{
				if (pt.y <= (sum += this->gridDelete.GetRowHeight(r)))
				{
					row = r;
					break;
				}
			}
			if (row > 0 && col > 0)
			{
				strRes = CDumpPg1::GetHelpString(row, col);
			}
		}
		break;

	case IDC_ALL_CHECK:
		strRes = CDumpPg1::GetHelpString(1, 1);
		break;

	default:
		// No help topic is associated with this item.
		strRes.LoadString(IDS_STRING441);
		break;
	}
	myPopup.pszText = strRes;
	return ::HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}

CString CDumpPg1::GetHelpString(int row, int col)
{
	CString strRes;
	switch (col)
	{
	case 0:
		ASSERT(FALSE);
		break;
	case 1:
		// Select this option to delete all %1 definitions.
		switch (row)
		{
		case 1:
			strRes = _T("Select this option to delete all numbered reactants.");
			break;
		case 2:
			AfxFormatString1(strRes, IDS_STRING741, _T("equilibrium-phase-assemblage"));
			break;
		case 3:
			AfxFormatString1(strRes, IDS_STRING741, _T("exchange-assemblage"));
			break;
		case 4:
			AfxFormatString1(strRes, IDS_STRING741, _T("gas-phase"));
			break;
		case 5:
			AfxFormatString1(strRes, IDS_STRING741, _T("kinetic-reaction"));
			break;
		case 6:
			AfxFormatString1(strRes, IDS_STRING741, _T("mix"));
			break;
		case 7:
			AfxFormatString1(strRes, IDS_STRING741, _T("reaction"));
			break;
		case 8:
			AfxFormatString1(strRes, IDS_STRING741, _T("reaction-pressure"));
			break;
		case 9:
			AfxFormatString1(strRes, IDS_STRING741, _T("reaction-temperature"));
			break;
		case 10:
			AfxFormatString1(strRes, IDS_STRING741, _T("solid-solution assemblages"));
			break;
		case 11:
			AfxFormatString1(strRes, IDS_STRING741, _T("solution"));
			break;
		case 12:
			AfxFormatString1(strRes, IDS_STRING741, _T("surface-assemblage"));
			break;
		default:
			ASSERT(FALSE);
			break;
		}
		break;
	case 2:
		// List of number ranges. The number ranges may be a single integer or a range defined by an integer, a hyphen, and an integer, without intervening spaces.  %1 identified by any of the numbers in the list will be deleted.
		switch (row)
		{
		case 1:
			AfxFormatString1(strRes, IDS_STRING740, _T("Reactants of any type that are"));
			break;
		case 2:
			AfxFormatString1(strRes, IDS_STRING740, _T("Equilibrium-phase assemblages"));
			break;
		case 3:
			AfxFormatString1(strRes, IDS_STRING740, _T("Exchange assemblages"));
			break;
		case 4:
			AfxFormatString1(strRes, IDS_STRING740, _T("Gas phases"));
			break;
		case 5:
			AfxFormatString1(strRes, IDS_STRING740, _T("Kinetics reactants"));
			break;
		case 6:
			AfxFormatString1(strRes, IDS_STRING740, _T("Mix definitions"));
			break;
		case 7:
			AfxFormatString1(strRes, IDS_STRING740, _T("Reactions"));
			break;
		case 8:
			AfxFormatString1(strRes, IDS_STRING740, _T("Reaction-pressure definitions"));
			break;
		case 9:
			AfxFormatString1(strRes, IDS_STRING740, _T("Reaction-temperature definitions"));
			break;
		case 10:
			AfxFormatString1(strRes, IDS_STRING740, _T("Solid-solution assemblages"));
			break;
		case 11:
			AfxFormatString1(strRes, IDS_STRING740, _T("Solutions"));
			break;
		case 12:
			AfxFormatString1(strRes, IDS_STRING740, _T("Surface assemblages"));
			break;
		default:
			ASSERT(FALSE);
			break;
		}
		break;
	default:
		ASSERT(FALSE);
		break;
	}
	return strRes;
}