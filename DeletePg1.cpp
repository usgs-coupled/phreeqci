// KPDeletePg1.cpp : implementation file
//

#include "stdafx.h"
#include "phreeqci2.h"
#include "DeletePg1.h"
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


const CString CDeletePg1::s_arrStrKeys[] =
{
	_T("cell"),                 // 1
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
// CDeletePg1 property page

IMPLEMENT_DYNCREATE(CDeletePg1, baseDeletePg1)


CDeletePg1::CDeletePg1(CTreeCtrlNode simNode) : baseDeletePg1(CDeletePg1::IDD)
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


CDeletePg1::~CDeletePg1()
{
}

void CDeletePg1::DoDataExchange(CDataExchange* pDX)
{
	baseDeletePg1::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeletePg1)
	DDX_Control(pDX, IDC_E_DESC_INPUT, m_eDesc);
#ifdef OLD_GRID_CTRL
	DDX_Control(pDX, IDC_MSHFG_DELETE, m_egDelete);
#else
	DDX_GridControl(pDX, IDC_GRID_DELETE, this->gridDelete);
#endif /* OLD_GRID_CTRL */
	DDX_Control(pDX, IDC_ALL_CHECK, btnAll);
	//}}AFX_DATA_MAP
	DDX_Grid(pDX);
}


BEGIN_MESSAGE_MAP(CDeletePg1, baseDeletePg1)
	//{{AFX_MSG_MAP(CDeletePg1)
	//}}AFX_MSG_MAP
	// specialized grid notifications
#ifdef OLD_GRID_CTRL
	ON_MESSAGE(EGN_BEGINCELLEDIT, OnBeginCellEdit)
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
#endif /* OLD_GRID_CTRL */	// ON_MESSAGE(EGN_SETFOCUS, OnSetfocusEG)
	// ON_MESSAGE(EGN_KILLFOCUS, OnKillfocusEG)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_ALL_CHECK, &CDeletePg1::OnBnClickedAllCheck)
	ON_BN_SETFOCUS(IDC_ALL_CHECK, &CDeletePg1::OnBnSetfocusAllCheck)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeletePg1 message handlers

BEGIN_EVENTSINK_MAP(CDeletePg1, baseDeletePg1)
    //{{AFX_EVENTSINK_MAP(CDeletePg1)
#ifdef OLD_GRID_CTRL
	ON_EVENT(CDeletePg1, IDC_MSHFG_DELETE, 71 /* EnterCell */, OnEnterCellMshfgDelete, VTS_NONE)
	ON_EVENT(CDeletePg1, IDC_MSHFG_DELETE, -602 /* KeyDown */, OnKeyDownMshfgDelete, VTS_PI2 VTS_I2)
#endif /* OLD_GRID_CTRL */	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CDeletePg1::DDX_Grid(CDataExchange *pDX)
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
#ifdef OLD_GRID_CTRL
		if (this->bAll)
		{
			this->m_egDelete.EnableWindow(FALSE);
		}
		else
		{
			this->m_egDelete.EnableWindow(TRUE);
		}
#else

		if (this->bAll)
		{
			this->CheckDlgButton(IDC_ALL_CHECK, BST_CHECKED);
		}
		else
		{
			this->CheckDlgButton(IDC_ALL_CHECK, BST_UNCHECKED);
		}
		this->OnBnClickedAllCheck();
#endif /* OLD_GRID_CTRL */
	}
}

void CDeletePg1::ExchangeGrid(CDataExchange* pDX)
{
	UNUSED(pDX);
	CString str;

	if (this->bAll) return;

	std::set<int> cells = CDeletePg1::GetCells(this->delete_info);

	// cell
	if (cells.size())
	{
		str = CDeletePg1::GetRanges(cells);
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

void CDeletePg1::ExchangeItem(StorageBinListItem &item, int row)
{
	if (item.Get_defined())
	{
		CString str = CDeletePg1::GetRanges(item.Get_numbers());
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

void CDeletePg1::ValidateGrid(CDataExchange* pDX)
{
	std::list< std::vector<int> > listCopies;
	CString str;
	CString nums;
	int index = -1;
	int index_start = -1;
	int index_end = -1;

#ifdef OLD_GRID_CTRL
	StorageBinList storageBin;

	for (long row = m_egDelete.GetFixedRows(); row < m_egDelete.GetRows(); ++row)
	{
		::DDX_GridText(pDX, IDC_MSHFG_DELETE, row, 1, nums);

		if (!nums.IsEmpty())
		{
			CString str(s_arrStrKeys[row - m_egDelete.GetFixedRows()]);
			str.Insert(0, "-");
			str += _T(" ");
			str += nums;

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
				::DDX_GridFail(pDX, str);
			}
			//storageBin = bin;
		}
	}
	// if here storageBin is valid and can be assigned to the member variable
	this->delete_info = storageBin;
#else
	StorageBinList storageBin;

	for (long row = this->gridDelete.GetFixedRowCount(); row < this->gridDelete.GetRowCount(); ++row)
	{
		::DDX_TextGridControl(pDX, IDC_GRID_DELETE, row, 2, nums);
		bool all = (this->gridDelete.GetCheck(row, 1) == BST_CHECKED);

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
	this->bAll = this->GetAll(storageBin);
	this->delete_info = storageBin;
#endif /* OLD_GRID_CTRL */
}

#ifdef OLD_GRID_CTRL
void CDeletePg1::OnEnterCellMshfgDelete()
{
	CString strRes;
	switch (m_egDelete.GetCol())
	{
	case 0 :
		strRes.LoadString(IDS_COPY_620); // reactant -- The entity type to be copied which can be any one of the following: CELL, SOLUTION, EQUILIBRIUM_PHASES, EXCHANGE, GAS_PHASE, KINETICS, MIX, REACTION, REACTION_TEMPERATURE, SOLID_SOLUTION, SURFACE.
		break;
	case 1 :
		strRes.LoadString(IDS_COPY_621); // index -- this entity index will be copied to the start_index or the range of indices(index_start-index_end)
		break;
	case 2 :
		strRes.LoadString(IDS_COPY_622); // index_start -- User defined positive integer to be associated with the respective composition.
		break;
	case 4 :
		strRes.LoadString(IDS_COPY_623); // index_end -- (Optional) A range of indices may be copied.  This being the ending index to copy.
		break;
	}
	m_eDesc.SetWindowText(strRes);
}
#endif /* OLD_GRID_CTRL */

#ifdef OLD_GRID_CTRL
void CDeletePg1::OnKeyDownMshfgDelete(short FAR* KeyCode, short Shift)
{
	UNUSED(Shift);

	m_egDelete.SetRedraw(FALSE);

	// save selection
	long nSaveRow    = m_egDelete.GetRow();
	long nSaveCol    = m_egDelete.GetCol();
	long nSaveRowSel = m_egDelete.GetRowSel();
	long nSaveColSel = m_egDelete.GetColSel();

	long i, j;

	switch (*KeyCode)
	{
	case VK_DELETE :
		// just fill with empty strings
		for (i = nSaveRow; i <= nSaveRowSel; ++i)
		{
			for (j = nSaveCol; j <= nSaveColSel; ++j)
			{
				if (m_egDelete.GetCellEnabled(i, j))
					m_egDelete.SetTextMatrix(i, j, _T(""));
			}
		}
		break;
	}

	// reset selection
	m_egDelete.SetRow(nSaveRow);
	m_egDelete.SetCol(nSaveCol);
	m_egDelete.SetRowSel(nSaveRowSel);
	m_egDelete.SetColSel(nSaveColSel);

	m_egDelete.SetRedraw(TRUE);
}
#endif /* OLD_GRID_CTRL */

void CDeletePg1::InitGrid()
{
#ifdef OLD_GRID_CTRL
	m_egDelete.SetRows(sizeof(s_arrStrKeys)/sizeof(s_arrStrKeys[0]) + 1);
	m_egDelete.SetCols(0, 2);
	m_egDelete.SetTextMatrix(0, 0, _T("Reactant"));
	m_egDelete.SetTextMatrix(0, 1, _T("List of ranges"));

	for (int i = 0; i < sizeof(s_arrStrKeys)/sizeof(s_arrStrKeys[0]); ++i)
	{
		this->m_egDelete.SetTextMatrix(i + 1, 0, s_arrStrKeys[i]);
	}


	// bold headings
	m_egDelete.SetRow(0);
	for (long nCol = 0; nCol < m_egDelete.GetCols(0); ++nCol)
	{
		m_egDelete.SetCol(nCol);
		m_egDelete.SetCellFontBold(TRUE);
	}

	// set col widths
	const long col_0 = 1750;
	m_egDelete.SetColWidth(0, 0, col_0);

	int nScroll = ::GetSystemMetrics(SM_CXVSCROLL);

	// determine edit widths
	CRect rect;
	CDC* pDC = m_egDelete.GetDC();
	int nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
	m_egDelete.GetClientRect(&rect);
	long nWidth = MulDiv(rect.right, TWIPS_PER_INCH, nLogX);
	long col1 = (nWidth - col_0) - (MulDiv(m_egDelete.GetCols(0), TWIPS_PER_INCH, nLogX));
	m_egDelete.SetColWidth(1, 0, col1);

	// set initial position
	m_egDelete.SetRow(1);
	m_egDelete.SetCol(1);

	//{{
	COleFont f = m_egDelete.GetFont();
	afxDump << f.GetBold() << "\n";
	afxDump << f.GetName() << "\n";
	afxDump << f.GetSize() << "\n";
	afxDump << f.GetWeight() << "\n";

	LOGFONT lf;

	COleFont fh = m_egDelete.GetFontHeader(0);
	afxDump << fh.GetBold() << "\n";
	afxDump << fh.GetName() << "\n";
	afxDump << fh.GetSize() << "\n";
	afxDump << fh.GetWeight() << "\n";
	//}}

	// update m_eDesc
	OnEnterCellMshfgDelete();
#else
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


// COMMENT: {11/13/2012 5:25:25 PM}		LOGFONT *pFont = this->gridDelete.GetItemFont(0, 0);
// COMMENT: {11/13/2012 5:25:25 PM}		LOGFONT lf;
// COMMENT: {11/13/2012 5:25:25 PM}		CFont f;
// COMMENT: {11/13/2012 5:25:25 PM}		f.CreatePointFont(8.5, "Microsoft Sans Serif");
// COMMENT: {11/13/2012 5:25:25 PM}		f.GetLogFont(&lf);
// COMMENT: {11/13/2012 5:25:25 PM}		lf.lfWeight = FW_BOLD;

		LOGFONT lf;
		pDefFont->GetLogFont(&lf);
		//lf.lfWeight = FW_BOLD;
		//lf.lfWeight = FW_MEDIUM;
		//lf.lfWeight = FW_SEMIBOLD;
		this->gridDelete.SetItemFont(0, 0, &lf);
		this->gridDelete.SetItemFont(0, 1, &lf);
		this->gridDelete.SetItemFont(0, 2, &lf);

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
		//this->gridDelete.ExpandToFit();
	}
	catch (CMemoryException *e)
	{
		e->ReportError();
		e->Delete();
	}
#endif /* OLD_GRID_CTRL */
}

#ifdef OLD_GRID_CTRL
LRESULT CDeletePg1::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bDenyEdit = FALSE;	// allow edit

	if ( nID == IDC_MSHFG_COPY )
	{
		if (pInfo->item.iCol == 0)
		{
			pInfo->item.bUseCombo = (FillKeywords(pInfo->item.hWndCombo) > 0);
		}
		if (pInfo->item.iCol == 1)
		{
			CString str = this->m_egDelete.GetTextMatrix(pInfo->item.iRow, 0);
			if (!str.IsEmpty())
			{
				pInfo->item.bUseCombo = (CUtil::InsertRange(pInfo->item.hWndCombo, m_setNums[CKeyword::GetKeywordType(str)]) > 0);
			}
		}
	}
	return bDenyEdit;
}
#endif /* OLD_GRID_CTRL */

#ifdef OLD_GRID_CTRL
LRESULT CDeletePg1::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;

	BOOL bMakeChange = TRUE;

	// ignore if edit is cancelled
	if ( pInfo->item.pszText == NULL ) return bMakeChange;

	switch ( nID )
	{
	case IDC_MSHFG_COPY :
		if (pInfo->item.iCol == 0)
		{
			m_egDelete.SetRedraw(FALSE);

			// add a row
			//
			this->m_egDelete.SetRows(this->m_egDelete.GetRows() + 1);


			// save selection
			long nSaveRow    = m_egDelete.GetRow();
			long nSaveCol    = m_egDelete.GetCol();
			long nSaveRowSel = m_egDelete.GetRowSel();
			long nSaveColSel = m_egDelete.GetColSel();

			// add through column
			//
			m_egDelete.SetTextMatrix(this->m_egDelete.GetRows() - 1, 3, THROUGH);
			m_egDelete.SetCol(3);
			m_egDelete.SetRow(this->m_egDelete.GetRows() - 1);
			m_egDelete.SetCellFontBold(TRUE);
			m_egDelete.DisableCell(this->m_egDelete.GetRows() - 1, 3);

			// reset selection
			m_egDelete.SetRow(nSaveRow);
			m_egDelete.SetCol(nSaveCol);
			m_egDelete.SetRowSel(nSaveRowSel);
			m_egDelete.SetColSel(nSaveColSel);

			m_egDelete.SetRedraw(TRUE);
		}
		break;
	}

	return bMakeChange;
}
#endif /* OLD_GRID_CTRL */

int CDeletePg1::FillKeywords(HWND hWndCombo)
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

BOOL CDeletePg1::OnInitDialog()
{
	baseDeletePg1::OnInitDialog();


	// set layout
	CreateRoot(VERTICAL, 5, 6)
#ifdef OLD_GRID_CTRL
		<< item(IDC_MSHFG_DELETE, ABSOLUTE_VERT)
#else
		<< item(&this->gridDelete, ABSOLUTE_VERT)
#endif /* OLD_GRID_CTRL */
		<< item(IDC_ALL_CHECK, ABSOLUTE_VERT)
		<< ( paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, GREEDY, nDefaultBorder, 10, 10)
			<< item(IDC_E_DESC_INPUT, GREEDY)
		);
	UpdateLayout();


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDeletePg1::OnSize(UINT nType, int cx, int cy)
{
	CKeywordPage::OnSize(nType, cx, cy);

	// Add your message handler code here

#ifdef OLD_GRID_CTRL
	if (this->m_egDelete.GetSafeHwnd())
	{
		// set col widths
		long col_0 = m_egDelete.GetColWidth(0, 0);

		// determine edit widths
		CRect rect;
		CDC* pDC = m_egDelete.GetDC();
		int nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
		m_egDelete.GetClientRect(&rect);
		m_egDelete.SetColWidth(1, 0, MulDiv(rect.right, TWIPS_PER_INCH, nLogX) - col_0);
	}
#else
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
#endif /* OLD_GRID_CTRL */
}

#ifdef SAVE_IMPERATIVE
CString CDeletePg1::GetRanges(std::set<int>& nums)
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

CString CDeletePg1::GetRanges(std::set<int>& nums)
{
	CString str;
	return GetRanges0(nums, str);
}

CString CDeletePg1::GetRanges0(std::set<int> nums, CString acc)
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

std::set<int> CDeletePg1::GetCells(StorageBinList& bin)
{
	std::set<int> intersection;
	std::list< std::set<int>* > sets;

	sets.push_back(&bin.Get_exchange().Get_numbers());
	sets.push_back(&bin.Get_gas_phase().Get_numbers());
	sets.push_back(&bin.Get_kinetics().Get_numbers());
	sets.push_back(&bin.Get_mix().Get_numbers());
	sets.push_back(&bin.Get_pp_assemblage().Get_numbers());
	sets.push_back(&bin.Get_pressure().Get_numbers());
	sets.push_back(&bin.Get_reaction().Get_numbers());
	sets.push_back(&bin.Get_solution().Get_numbers());
	sets.push_back(&bin.Get_ss_assemblage().Get_numbers());
	sets.push_back(&bin.Get_surface().Get_numbers());
	sets.push_back(&bin.Get_temperature().Get_numbers());

	std::list< std::set<int>* >::iterator list_iter = sets.begin();
	for (; list_iter != sets.end(); ++list_iter)
	{
		std::set<int>::iterator set_iter = (*list_iter)->begin();
		for (; set_iter != (*list_iter)->end(); ++set_iter)
		{
#ifndef _DEBUG
#error
#endif
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

void CDeletePg1::OnBnClickedAllCheck()
{
	if (this->IsDlgButtonChecked(IDC_ALL_CHECK) == BST_CHECKED)
	{
#ifdef OLD_GRID_CTRL
		this->m_egDelete.EnableWindow(FALSE);
#else
		for (int r = 1; r < this->gridDelete.GetRowCount(); ++r)
		{
			this->gridDelete.DisableCell(r, 1);
			this->gridDelete.DisableCell(r, 2);
		}
		this->RedrawWindow();
		this->gridDelete.EnableWindow(FALSE);
#endif /* OLD_GRID_CTRL */
	}
	else
	{
#ifdef OLD_GRID_CTRL
		this->m_egDelete.EnableWindow(TRUE);
#else
		for (int r = 1; r < this->gridDelete.GetRowCount(); ++r)
		{
			this->gridDelete.EnableCell(r, 1);
			this->gridDelete.EnableCell(r, 2);
		}
		this->RedrawWindow();
		this->gridDelete.EnableWindow(TRUE);
#endif /* OLD_GRID_CTRL */
	}
}

void CDeletePg1::OnBnSetfocusAllCheck()
{
	// TODO: Add your control notification handler code here
}

bool CDeletePg1::GetAll(StorageBinList bin)
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
