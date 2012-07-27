// KPCopyPg1.cpp : implementation file
//

#include "stdafx.h"
#include "phreeqci2.h"
#include "KPCopyPg1.h"
#include "Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


const CString CKPCopyPg1::s_arrStrKeys[] =
{
	_T("cell"),
	_T("equilibrium_phases"),
	_T("exchange"),
	_T("gas_phase"),
	_T("kinetics"),
	_T("mix"),
	_T("reaction"),
	_T("reaction_temperature"),
	_T("solid_solutions"),
	_T("solution"),
	_T("surface"),
};

const TCHAR THROUGH[] = _T("-");

/////////////////////////////////////////////////////////////////////////////
// CKPCopyPg1 property page

IMPLEMENT_DYNCREATE(CKPCopyPg1, baseKPCopyPg1)


CKPCopyPg1::CKPCopyPg1(CTreeCtrlNode simNode) : baseKPCopyPg1(CKPCopyPg1::IDD)
, m_ranges(simNode, true, true)
{
	for (int i = 0; i < sizeof(s_arrStrKeys)/sizeof(s_arrStrKeys[0]); ++i)
	{
		CKeyword::type n = CKeyword::GetKeywordType2(s_arrStrKeys[i]);
		CUtil::CreateRange(m_setNums[n], m_ranges[n]);
	}
}


CKPCopyPg1::~CKPCopyPg1()
{
}

void CKPCopyPg1::DoDataExchange(CDataExchange* pDX)
{
	baseKPCopyPg1::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKPCopyPg1)
	DDX_Control(pDX, IDC_E_DESC_INPUT, m_eDesc);
	DDX_Control(pDX, IDC_MSHFG_COPY, m_egCopy);
	//}}AFX_DATA_MAP
	DDX_Grid(pDX);
}


BEGIN_MESSAGE_MAP(CKPCopyPg1, baseKPCopyPg1)
	//{{AFX_MSG_MAP(CKPCopyPg1)
	//}}AFX_MSG_MAP
	// specialized grid notifications
	ON_MESSAGE(EGN_BEGINCELLEDIT, OnBeginCellEdit)
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
	// ON_MESSAGE(EGN_SETFOCUS, OnSetfocusEG)
	// ON_MESSAGE(EGN_KILLFOCUS, OnKillfocusEG)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKPCopyPg1 message handlers

BEGIN_EVENTSINK_MAP(CKPCopyPg1, baseKPCopyPg1)
    //{{AFX_EVENTSINK_MAP(CKPCopyPg1)
	ON_EVENT(CKPCopyPg1, IDC_MSHFG_COPY, 71 /* EnterCell */, OnEnterCellMshfgCopy, VTS_NONE)
	ON_EVENT(CKPCopyPg1, IDC_MSHFG_COPY, -602 /* KeyDown */, OnKeyDownMshfgCopy, VTS_PI2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CKPCopyPg1::DDX_Grid(CDataExchange *pDX)
{
	 // do one time initialize
	if (m_bFirstSetActive && !pDX->m_bSaveAndValidate)
	{
		InitGrid();
	}

	// get and format data from control
	if (pDX->m_bSaveAndValidate)
	{
		ValidateGrid(pDX);
	}
	// load data to control
	else		
	{
		ExchangeGrid(pDX);
	}
}

void CKPCopyPg1::ExchangeGrid(CDataExchange* pDX)
{
	UNUSED(pDX);
	CString str;

	this->m_egCopy.SetRows(this->m_egCopy.GetRows() + (long)this->m_listCopies.size());

	std::list< std::vector<int> >::iterator iter = this->m_listCopies.begin();
	for (long nRow = 1; iter != this->m_listCopies.end(); ++iter, ++nRow)
	{
		// keyword
		//
		str = CKeyword::GetString2((CKeyword::type)(*iter)[0]);
		str.MakeLower();
		this->m_egCopy.SetTextMatrix(nRow, 0, str);

		// index
		//
		str.Format(_T("%d"), (*iter)[1]);
		this->m_egCopy.SetTextMatrix(nRow, 1, str);

		// index_start
		//
		str.Format(_T("%d"), (*iter)[2]);
		this->m_egCopy.SetTextMatrix(nRow, 2, str);

		// index_end
		//
		if ((*iter)[2] != (*iter)[3])
		{
			str.Format(_T("%d"), (*iter)[3]);
			this->m_egCopy.SetTextMatrix(nRow, 4, str);

		}
	}
}

void CKPCopyPg1::ValidateGrid(CDataExchange* pDX)
{
	std::list< std::vector<int> > listCopies;
	CString str;
	int index = -1;
	int index_start = -1;
	int index_end = -1;

	for (long row = m_egCopy.GetFixedRows(); row < m_egCopy.GetRows(); ++row)
	{
		::DDX_GridText(pDX, IDC_MSHFG_COPY, row, 0, str);
		if (!str.IsEmpty())
		{
			std::vector<int> copy;

			// keyword
			//
			CKeyword::type nType = CKeyword::GetKeywordType2(str);
			switch (nType)				
			{
			case CKeyword::K_SOLUTION:
			case CKeyword::K_MIX:
			case CKeyword::K_KINETICS:
			case CKeyword::K_REACTION:
			case CKeyword::K_REACTION_TEMPERATURE:
			case CKeyword::K_REACTION_PRESSURE:
			case CKeyword::K_EQUILIBRIUM_PHASES:
			case CKeyword::K_EXCHANGE:
			case CKeyword::K_SURFACE:
			case CKeyword::K_GAS_PHASE:
			case CKeyword::K_SOLID_SOLUTIONS:
			case CKeyword::K_CELL:
				break;
			default:
				::DDX_GridFail(pDX, _T("Expecting keyword solution, mix, kinetics, reaction, reaction_pressure, reaction_temperature,")
					_T(" equilibrium_phases, exchange, surface, gas_phase, solid_solutions, or cell."));
			}

			// index
			//
			DDX_GridText(pDX, IDC_MSHFG_COPY, row, 1, str);
			// Note: grid trims white space on entry
			if (!str.IsEmpty() && ::isdigit(str[0]))
			{
				if (::_stscanf(str, _T("%d"), &index) < 1)
				{
					::DDX_GridFail(pDX, _T("Source index number must be a positive integer."));
				}
			}
			else
			{
				::DDX_GridFail(pDX, _T("Source index number must be a positive integer."));
			}

			// index_start
			//
			DDX_GridText(pDX, IDC_MSHFG_COPY, row, 2, str);
			// Note: grid trims white space on entry
			if (!str.IsEmpty() && ::isdigit(str[0]))
			{
				if (::_stscanf(str, _T("%d"), &index_start) < 1)
				{
					::DDX_GridFail(pDX, _T("Target index number must be a positive integer."));
				}
				index_end = index_start;
			}
			else
			{
				::DDX_GridFail(pDX, _T("Target index number must be a positive integer."));
			}

			// index_end
			//
			DDX_GridText(pDX, IDC_MSHFG_COPY, row, 4, str);
			// Note: grid trims white space on entry
			if (!str.IsEmpty() && ::isdigit(str[0]))
			{
				if (::_stscanf(str, _T("%d"), &index_end) < 1)
				{
					::DDX_GridFail(pDX, _T("Target index number must be a positive integer."));
				}
				if (index_start > index_end)
				{
					::DDX_GridFail(pDX, _T("Index end must be greater than index start."));
				}
			}

			std::vector<int> intvec;
			intvec.resize(4);
			intvec[0] = nType;
			intvec[1] = index;
			intvec[2] = index_start;
			intvec[3] = index_end;
			listCopies.push_back(intvec);
		}
	}
	// if here listCopies is valid and can be assigned to the member variable
	this->m_listCopies.empty();
	this->m_listCopies.assign(listCopies.begin(), listCopies.end());
}

void CKPCopyPg1::OnEnterCellMshfgCopy() 
{
	CString strRes;
	switch (m_egCopy.GetCol())
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

void CKPCopyPg1::OnKeyDownMshfgCopy(short FAR* KeyCode, short Shift) 
{
	UNUSED(Shift);

	m_egCopy.SetRedraw(FALSE);

	// save selection
	long nSaveRow    = m_egCopy.GetRow();
	long nSaveCol    = m_egCopy.GetCol();
	long nSaveRowSel = m_egCopy.GetRowSel();
	long nSaveColSel = m_egCopy.GetColSel();

	long i, j;

	switch (*KeyCode)
	{
	case VK_DELETE :
		// just fill with empty strings
		for (i = nSaveRow; i <= nSaveRowSel; ++i)
		{
			for (j = nSaveCol; j <= nSaveColSel; ++j)
			{
				if (m_egCopy.GetCellEnabled(i, j))
					m_egCopy.SetTextMatrix(i, j, _T(""));
			}
		}
		break;
	}

	// reset selection
	m_egCopy.SetRow(nSaveRow);
	m_egCopy.SetCol(nSaveCol);
	m_egCopy.SetRowSel(nSaveRowSel);
	m_egCopy.SetColSel(nSaveColSel);	

	m_egCopy.SetRedraw(TRUE);
}

void CKPCopyPg1::InitGrid()
{
	m_egCopy.SetTextMatrix(0, 0, _T("Reactant"));
	m_egCopy.SetTextMatrix(0, 1, _T("Source index"));
	m_egCopy.SetTextMatrix(0, 2, _T("Index start"));
	m_egCopy.SetTextMatrix(0, 4, _T("Index end"));

	// bold and centered
	m_egCopy.SetRow(0);
	for (long nCol = 0; nCol < m_egCopy.GetCols(0); ++nCol)
	{
		m_egCopy.SetCol(nCol);
		m_egCopy.SetCellFontBold(TRUE);
	}


	for (long nRow = 0; nRow < m_egCopy.GetRows(); ++nRow)
	{
		// add through column
		m_egCopy.SetTextMatrix(nRow, 3, THROUGH);
		m_egCopy.SetCol(3);
		m_egCopy.SetRow(nRow);
		m_egCopy.SetCellFontBold(TRUE);
		if (nRow >= m_egCopy.GetFixedRows()) {
			m_egCopy.DisableCell(nRow, 3);
		}
	}

	// set "through" alignment
	m_egCopy.SetColAlignment(3, flexAlignCenterCenter);
	m_egCopy.SetColAlignmentFixed(3, flexAlignCenterCenter);

	// set col widths
	const long col_0 = 1750;
	//const long col_3 = 960; // (THROUGH == "through")
	const long col_3 = 250; // (THROUGH == "-")
	m_egCopy.SetColWidth(0, 0, col_0);
	m_egCopy.SetColWidth(3, 0, col_3);

	int nScroll = ::GetSystemMetrics(SM_CXVSCROLL);

	// determine edit widths
	CRect rect;
	CDC* pDC = m_egCopy.GetDC();
	int nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
	m_egCopy.GetClientRect(&rect);
	long nWidth = MulDiv(rect.right, TWIPS_PER_INCH, nLogX);
	long nScrollTwips = MulDiv(nScroll, TWIPS_PER_INCH, nLogX);
	// long col124 = ((nWidth - col_0 - col_3 - nScrollTwips) / 3) - (3 * m_egCopy.GetCols(0));
	long col124 = ((nWidth - col_0 - col_3 - nScrollTwips) / 3) - (MulDiv(m_egCopy.GetCols(0), TWIPS_PER_INCH, nLogX));
	m_egCopy.SetColWidth(1, 0, col124);
	m_egCopy.SetColWidth(2, 0, col124);
	m_egCopy.SetColWidth(4, 0, col124);

	// set initial position
	m_egCopy.SetRow(1);
	m_egCopy.SetCol(0);

	// update m_eDesc
	OnEnterCellMshfgCopy();
}

LRESULT CKPCopyPg1::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
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
			CString str = this->m_egCopy.GetTextMatrix(pInfo->item.iRow, 0);
			if (!str.IsEmpty())
			{
				pInfo->item.bUseCombo = (CUtil::InsertRange(pInfo->item.hWndCombo, m_setNums[CKeyword::GetKeywordType(str)]) > 0);
			}
		}
	}
	return bDenyEdit;
}

LRESULT CKPCopyPg1::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
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
			m_egCopy.SetRedraw(FALSE);

			// add a row
			//
			this->m_egCopy.SetRows(this->m_egCopy.GetRows() + 1);


			// save selection
			long nSaveRow    = m_egCopy.GetRow();
			long nSaveCol    = m_egCopy.GetCol();
			long nSaveRowSel = m_egCopy.GetRowSel();
			long nSaveColSel = m_egCopy.GetColSel();

			// add through column
			//
			m_egCopy.SetTextMatrix(this->m_egCopy.GetRows() - 1, 3, THROUGH);
			m_egCopy.SetCol(3);
			m_egCopy.SetRow(this->m_egCopy.GetRows() - 1);
			m_egCopy.SetCellFontBold(TRUE);
			m_egCopy.DisableCell(this->m_egCopy.GetRows() - 1, 3);

			// reset selection
			m_egCopy.SetRow(nSaveRow);
			m_egCopy.SetCol(nSaveCol);
			m_egCopy.SetRowSel(nSaveRowSel);
			m_egCopy.SetColSel(nSaveColSel);	

			m_egCopy.SetRedraw(TRUE);
		}
		break;
	}

	return bMakeChange;
}

int CKPCopyPg1::FillKeywords(HWND hWndCombo)
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

BOOL CKPCopyPg1::OnInitDialog() 
{
	baseKPCopyPg1::OnInitDialog();
	
	
	// set layout
	CreateRoot(VERTICAL, 5, 6)
		<< item(IDC_MSHFG_COPY, ABSOLUTE_VERT)

		<< ( paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, GREEDY, nDefaultBorder, 10, 10)
			<< item(IDC_E_DESC_INPUT, GREEDY)
		);
	UpdateLayout();

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
