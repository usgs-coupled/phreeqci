// KPSolution_SpreadPg1.cpp : implementation file
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "KPSolution_SpreadPg1.h"

#include "Util.h"
#include "DelayUpdate.h"

#include "OCKSSolution_Spread.h"

#include "DefinedRanges.h"

#define WM_DISPLAY_ERRORS     WM_USER + 202

//{{
#pragma warning(disable : 4097)   // disable truncation warning (Only used by debugger)
#include <strstream>
//}}

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CKPSolution_SpreadPg1, baseKPSolution_SpreadPg1)
IMPLEMENT_DYNCREATE(CKPSolution_SpreadPg2, baseKPSolution_SpreadPg2)
IMPLEMENT_DYNCREATE(CKPSolution_SpreadPg3, baseKPSolution_SpreadPg3)
IMPLEMENT_DYNCREATE(CKPSolution_SpreadPg4, baseKPSolution_SpreadPg4)


/////////////////////////////////////////////////////////////////////////////
// CKPSolution_SpreadPg1 property page

CKPSolution_SpreadPg1::CKPSolution_SpreadPg1() : baseKPSolution_SpreadPg1(CKPSolution_SpreadPg1::IDD)
{
	//{{AFX_DATA_INIT(CKPSolution_SpreadPg1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bFreeze = TRUE;
	m_bIgnoreItemchanged = false;

	m_mapExtraHeadings[_T("Temperature")] = IDS_STRING461;
	m_mapExtraHeadings[_T("Density")]     = IDS_STRING463;
	m_mapExtraHeadings[_T("Redox")]       = IDS_STRING462;
	m_mapExtraHeadings[_T("Description")] = IDS_STRING454;
	m_mapExtraHeadings[_T("Number")]      = IDS_STRING532;
	m_mapExtraHeadings[_T("pH")]          = IDS_STRING533;
	m_mapExtraHeadings[_T("pe")]          = IDS_STRING534;
	m_mapExtraHeadings[_T("Water")]       = IDS_STRING542;
}

CKPSolution_SpreadPg1::~CKPSolution_SpreadPg1()
{
}

//static bool _SimpleFloatParse(LPCTSTR lpszText, double& d)
inline bool _SimpleFloatParse(LPCTSTR lpszText, double& d)
{
	ASSERT(lpszText != NULL);

	TCHAR chFirst = lpszText[0];
	d = _tcstod(lpszText, (LPTSTR*)&lpszText);
	if (d == 0.0 && chFirst != _T('0'))
		return FALSE;   // could not convert

	if (*lpszText != _T('\0'))
		return FALSE;   // not terminated properly

	return TRUE;
}

void CKPSolution_SpreadPg1::DoDataExchange(CDataExchange* pDX)
{
	baseKPSolution_SpreadPg1::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKPSolution_SpreadPg1)
	DDX_Control(pDX, IDC_DESCRIPTION, m_ctrlDescription);
	DDX_Control(pDX, IDC_CL_TOTALS, m_ctrlCheckList);
	DDX_Control(pDX, IDC_MSHFG_TOTALS, m_ctrlGrid);
	DDX_Check(pDX, IDC_B_FREEZE, m_bFreeze);
	//}}AFX_DATA_MAP

	// avoid flashing grid
	CDelayUpdate update(this, IDC_MSHFG_TOTALS);


	if (m_bFirstSetActive)
	{
		m_ctrlGrid.SetRows(__max(51, m_vvsCells.size() + 10));
		m_ctrlGrid.SetCols(0, __max(30, m_vsHeading.size() + 10));
		m_ctrlGrid.AddRowHeaders();
		m_ctrlGrid.SetTextMatrix(0, 0, "Heading");

		m_ctrlGrid.SetColWidth(0, 0, 800);

		std::map< CString, UINT >::const_iterator mapIter = m_mapExtraHeadings.begin();
		for (; mapIter != m_mapExtraHeadings.end(); ++mapIter)
		{
			m_ctrlCheckList.InsertItem(0, mapIter->first);
		}


		CUtil::InsertAqElements(&m_ctrlCheckList, GetDatabase());

		for (int i = 0; i < m_ctrlCheckList.GetItemCount(); ++i)
		{
			CString item = m_ctrlCheckList.GetItemText(i, 0);
			CString sub_item = m_ctrlCheckList.GetItemText(i, 1);
			if (!sub_item.IsEmpty())
			{
				m_mapSubItem2Item[sub_item] = item;
				m_mapItem2SubItem[item] = sub_item;
			}
		}
	}

	if (pDX->m_bSaveAndValidate)
	{
		CWaitCursor wait;

		//
		// headings
		//

		std::vector<std::string> vsHeading;
		vsHeading.reserve(m_ctrlGrid.GetCols(0));
		long nCol = 1;
		for (; nCol < m_ctrlGrid.GetCols(0); ++nCol)
		{
			CString strItem = m_ctrlGrid.GetTextMatrix(0, nCol);
			strItem.Replace(_T("(+"), _T("("));
			std::map<CString, CString>::const_iterator mapIter = m_mapItem2SubItem.find(strItem);
			if (mapIter != m_mapItem2SubItem.end())
			{
				strItem = mapIter->second;
			}
			std::string sHead = strItem;
			vsHeading.push_back(sHead);
		}

		// remove empty cols
		std::vector<std::string>::reverse_iterator colRevIter = vsHeading.rbegin();
		for (; colRevIter != vsHeading.rend(); colRevIter = vsHeading.rbegin())
		{
			if (!colRevIter->empty()) break;
			vsHeading.pop_back();
		}


		//
		// rows and cols
		//

		std::vector<std::string>::size_type stCol;
		std::vector< std::vector<std::string> > vvsCells;		
		std::vector<std::string>::size_type nCols = vsHeading.size();
		if (nCols > 0)
		{
			std::vector<std::string>::size_type nRows = m_ctrlGrid.GetRows();

			vvsCells.resize(nRows);

			// Note using GetClip() with a pair of istrstreams is slightly 
			// faster than using GetTextMatrix() for each cell

			m_ctrlGrid.SetRow(1);
			m_ctrlGrid.SetCol(1);
			m_ctrlGrid.SetRowSel(nRows - 1);
			m_ctrlGrid.SetColSel(nCols);

			CString strClip = m_ctrlGrid.GetClip();
			std::istrstream issRows((LPCTSTR)strClip);

			std::string sRow;
			std::string sCell;
			std::vector<std::string>::size_type nRow;
			std::vector<std::string>::size_type nCol;

			for (nRow = 0; nRow < nRows - 1; ++nRow)
			{
				ASSERT(issRows.good());
				std::getline(issRows, sRow, _T('\r'));
				std::istrstream issCols(sRow.c_str());

				vvsCells[nRow].resize(nCols);
				for (nCol = 0; nCol < nCols; ++nCol)
				{
					ASSERT(issCols.good());
					std::getline(issCols, sCell, _T('\t'));
					vvsCells[nRow][nCol] = sCell;
				}
				ASSERT(issCols.eof());
			}
			ASSERT(issRows.eof());


			// remove empty rows
			std::vector< std::vector<std::string> >::reverse_iterator rowRevIter = vvsCells.rbegin();
			for (; rowRevIter != vvsCells.rend(); rowRevIter = vvsCells.rbegin())
			{
				std::vector<std::string>::iterator colIter = rowRevIter->begin();
				for (; colIter != rowRevIter->end(); ++colIter)
				{
					if (!colIter->empty()) break;
				}
				if (colIter == rowRevIter->end())
					vvsCells.pop_back();
				else
					break;
			}


			//
			// validate known headings
			//

			std::vector<std::string>::size_type stCols = vsHeading.size();
			std::vector<std::string>::size_type stRows = vvsCells.size();
			std::vector<std::string>::size_type stRow;
			double dVal;
			const char* pszCell;
			for (stCol = 0; stCol < stCols; ++stCol)
			{
				std::string sHeading = vsHeading[stCol];
				if (sHeading.compare("pH") == 0)
				{
					for (stRow = 0; stRow < stRows; ++stRow)
					{
						pszCell = vvsCells[stRow][stCol].c_str();
						if (pszCell && strlen(pszCell) > 0)
						{
							if (!(_SimpleFloatParse(pszCell, dVal) /* && (2.0 <= dVal && dVal <= 15.)*/ ))
							{
								CString strValue;
								DDX_GridText(pDX, IDC_MSHFG_TOTALS, stRow + 1, stCol + 1, strValue);
								DDX_GridFail(pDX, _T("pH must be a number."));
							}
						}
					}
				}
				else if (sHeading.compare("pe") == 0)
				{
					for (stRow = 0; stRow < stRows; ++stRow)
					{
						pszCell = vvsCells[stRow][stCol].c_str();
						if (pszCell && strlen(pszCell) > 0)
						{
							if (!(_SimpleFloatParse(pszCell, dVal) && (-20.0 < dVal && dVal < 20.0)))
							{
								CString strValue;
								DDX_GridText(pDX, IDC_MSHFG_TOTALS, stRow + 1, stCol + 1, strValue);
								DDX_GridFail(pDX, _T("pe must be between -20 and 20"));
							}
						}
					}
				}
				else if (sHeading.compare(_T("Temperature")) == 0)
				{
					for (stRow = 0; stRow < stRows; ++stRow)
					{
						pszCell = vvsCells[stRow][stCol].c_str();
						if (pszCell && strlen(pszCell) > 0)
						{
							if (!(_SimpleFloatParse(pszCell, dVal)))
							{
								DDX_GridText(pDX, IDC_MSHFG_TOTALS, stRow + 1, stCol + 1, dVal);
							}
						}
					}
				}
				else if (sHeading.compare(_T("Density")) == 0)
				{
					for (stRow = 0; stRow < stRows; ++stRow)
					{
						pszCell = vvsCells[stRow][stCol].c_str();
						if (pszCell && strlen(pszCell) > 0)
						{
							if (!(_SimpleFloatParse(pszCell, dVal) && (dVal > 0.0)))
							{
								CString strValue;
								DDX_GridText(pDX, IDC_MSHFG_TOTALS, stRow + 1, stCol + 1, strValue);
								DDX_GridFail(pDX, _T("Density must be a positive number."));
							}
						}
					}
				}
				else if (sHeading.compare(_T("Number")) == 0)
				{
					for (stRow = 0; stRow < stRows; ++stRow)
					{
						pszCell = vvsCells[stRow][stCol].c_str();
						if (pszCell && strlen(pszCell) > 0)
						{
							CRange range(pszCell);
							if (!range.IsEmpty())
							{
								if (!range.IsValid())
								{
									CString strValue;
									DDX_GridText(pDX, IDC_MSHFG_TOTALS, stRow + 1, stCol + 1, strValue);
									DDX_GridFail(pDX, _T("Invalid solution number."));
								}
							}
							else
							{
								CString strValue;
								DDX_GridText(pDX, IDC_MSHFG_TOTALS, stRow + 1, stCol + 1, strValue);
								DDX_GridFail(pDX, _T("Invalid solution number."));
							}
						}
					}
				}
				else if (sHeading.compare(_T("Water")) == 0)
				{
					for (stRow = 0; stRow < stRows; ++stRow)
					{
						pszCell = vvsCells[stRow][stCol].c_str();
						if (pszCell && strlen(pszCell) > 0)
						{
							if (!(_SimpleFloatParse(pszCell, dVal) && (dVal > 0.0)))
							{
								CString strValue;
								DDX_GridText(pDX, IDC_MSHFG_TOTALS, stRow + 1, stCol + 1, strValue);
								DDX_GridFail(pDX, _T("Mass must be a positive number."));
							}
						}
					}
				}
				else if (sHeading.compare(_T("Redox")) == 0)
				{
					CString strValue;
					for (stRow = 0; stRow < stRows; ++stRow)
					{
						strValue = vvsCells[stRow][stCol].c_str();
						if (!strValue.IsEmpty())
						{
							if (!CUtil::IsValidRedoxCouple(strValue))
							{
								DDX_GridText(pDX, IDC_MSHFG_TOTALS, stRow + 1, stCol + 1, strValue);
								DDX_GridFail(pDX, CUtil::GetLastErrorString());
							}
						}
					}
				}
				else
				{
					// check if this is a concentration
					std::map<CString, CString>::const_iterator mapIter;
					mapIter = m_mapItem2SubItem.find(sHeading.c_str());
					if (mapIter != m_mapItem2SubItem.end())
					{
						for (stRow = 0; stRow < stRows; ++stRow)
						{
							pszCell = vvsCells[stRow][stCol].c_str();
							if (pszCell && strlen(pszCell) > 0)
							{
								if (!(_SimpleFloatParse(pszCell, dVal) && (dVal >= 0.0)))
								{
									CString strValue;
									DDX_GridText(pDX, IDC_MSHFG_TOTALS, stRow + 1, stCol + 1, strValue);
									// DDX_GridFail(pDX, _T("Concentration must be greater than or equal to 0."));
								}
							}
						}
					}
				}
			}
		}
		// all ok set members
		m_vsHeading = vsHeading;
		m_vvsCells = vvsCells;
	}
	else
	{
		// clear all checkboxes
		m_ctrlCheckList.SetCheckBox(-1, BST_UNCHECKED);

		LVFINDINFO info = {0};
		info.flags = LVFI_STRING;

		// headings
		std::vector<std::string>::size_type stCol = 0;
		for (; stCol < m_vsHeading.size(); ++stCol)
		{
			// check for isotope
			if (!m_vsHeading[stCol].empty() && ::_istdigit(m_vsHeading[stCol].at(0)))
			{
				CButton* pBtn = (CButton*)GetDlgItem(IDC_CHK_USE_INVERSE);
				if (pBtn->GetCheck() == BST_UNCHECKED)
				{
					pBtn->SetCheck(BST_CHECKED);
					OnChkUseInverse();
				}
			}
			CString strItem = m_vsHeading[stCol].c_str();
			strItem.Replace(_T("(+"), _T("("));
			std::map<CString, CString>::const_iterator cIter;
			cIter = m_mapSubItem2Item.find(strItem);
			if (cIter != m_mapSubItem2Item.end())
			{
				info.psz = cIter->second;
			}
			else
			{
				info.psz = m_vsHeading[stCol].c_str();
			}
			int nFind = m_ctrlCheckList.FindItem(&info, -1);
			if (nFind != -1)
			{
				m_ctrlCheckList.SetCheckBox(nFind, BST_CHECKED);
				UpdateElementList(nFind);
				m_ctrlGrid.SetTextMatrix(0, long(stCol + 1), m_ctrlCheckList.GetItemText(nFind, 0));
			}
			else
			{
				m_ctrlGrid.SetTextMatrix(0, long(stCol + 1), m_vsHeading[stCol].c_str());
			}
		}

		// rows
		std::vector< std::vector< std::string > >::const_iterator cvvsIter = m_vvsCells.begin();
		for (long nRow = 1; cvvsIter != m_vvsCells.end(); ++cvvsIter, ++nRow)
		{
			// cols
			std::vector< std::string >::const_iterator cvsIter = (*cvvsIter).begin();
			for (long nCol = 1; cvsIter != (*cvvsIter).end(); ++cvsIter, ++nCol)
			{
				if ((unsigned)nCol > m_vsHeading.size()) break;
				m_ctrlGrid.SetTextMatrix(nRow, nCol, (*cvsIter).c_str());
			}
		}
	}

}

BEGIN_MESSAGE_MAP(CKPSolution_SpreadPg1, baseKPSolution_SpreadPg1)
	//{{AFX_MSG_MAP(CKPSolution_SpreadPg1)
	ON_WM_HELPINFO()
	ON_BN_CLICKED(IDC_B_FREEZE, OnBFreeze)
	ON_BN_DOUBLECLICKED(IDC_B_FREEZE, OnDoubleclickedBFreeze)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CL_TOTALS, OnItemchanged)
	ON_BN_CLICKED(IDC_CHK_USE_INVERSE, OnChkUseInverse)
	ON_NOTIFY(NM_SETFOCUS, IDC_CL_TOTALS, OnSetfocusClTotals)
	//}}AFX_MSG_MAP
	// custom BN_SETFOCUS notifications
	ON_BN_SETFOCUS(IDC_B_FREEZE, OnSetfocusBFreeze)
	ON_BN_SETFOCUS(IDC_CHK_USE_INVERSE, OnSetfocusBUseInverse)

	// custom grid messages
	// ON_MESSAGE(EGN_BEGINCELLEDIT, OnBeginCellEdit)
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
	ON_MESSAGE(EGN_SETFOCUS, OnSetfocusGrid)

	ON_MESSAGE(EGN_ALLOW_DELETE_ROW, OnAllowDeleteRow)
	ON_MESSAGE(EGN_DELETE_ROW, OnDeleteRow)
	ON_MESSAGE(EGN_ALLOW_DELETE_COL, OnAllowDeleteCol)
	ON_MESSAGE(EGN_DELETE_COL, OnDeleteCol)

	ON_MESSAGE(EGN_ALLOW_INSERT_COL, OnAllowInsertCol)
	ON_MESSAGE(EGN_INSERT_COL, OnInsertCol)

	ON_MESSAGE(EGN_ALLOW_INSERT_ROW, OnAllowInsertRow)
	ON_MESSAGE(EGN_INSERT_ROW, OnInsertRow)
	//{{
	ON_MESSAGE(EGN_RESIZE_ROWS, OnResizeRows)
	ON_MESSAGE(EGN_RESIZE_COLS, OnResizeCols)
	//}}
	ON_MESSAGE(WM_DISPLAY_ERRORS, OnDisplayErrors)
END_MESSAGE_MAP()

LRESULT CKPSolution_SpreadPg1::OnResizeRows(WPARAM wParam, LPARAM lParam)
{
	UNUSED(wParam);
	UNUSED(lParam);
	return TRUE;
}

LRESULT CKPSolution_SpreadPg1::OnResizeCols(WPARAM wParam, LPARAM lParam)
{
	UNUSED(wParam);
	UNUSED(lParam);
	return TRUE;
}

LRESULT CKPSolution_SpreadPg1::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	ASSERT(nID == IDC_MSHFG_TOTALS);

	UNUSED(pInfo);

	BOOL bMakeChange = TRUE;

	// ignore if edit is cancelled
	if (pInfo->item.pszText == NULL) return bMakeChange;

	ASSERT(!m_bIgnoreItemchanged);
	m_bIgnoreItemchanged = true;
	if (pInfo->item.iRow == 0)
	{
		LVFINDINFO info = {0};
		info.flags = LVFI_STRING;

		CString strNewItem = pInfo->item.pszText;
		strNewItem.Replace(_T("(+"), _T("("));

		CString strOldItem = m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, pInfo->item.iCol);
		strOldItem.Replace(_T("(+"), _T("("));
		if (strOldItem == strNewItem)
		{
			// nothing to do
			ASSERT(m_bIgnoreItemchanged);
			m_bIgnoreItemchanged = false;
			return bMakeChange;
		}

		if (!strOldItem.IsEmpty())
		{
			//
			// uncheck old string if checked
			//
			std::map<CString, CString>::const_iterator mapIter = m_mapSubItem2Item.find(strOldItem);
			if (mapIter != m_mapSubItem2Item.end())
			{
				info.psz = mapIter->second;
			}
			else
			{
				info.psz = strOldItem;
			}
			int nFind = m_ctrlCheckList.FindItem(&info, -1); // Not case sensitive
			if (nFind != -1 && m_ctrlCheckList.GetItemText(nFind, 0).Compare(info.psz) == 0)
			{
				strOldItem = m_ctrlCheckList.GetItemText(nFind, 0);
				if (!IsMoreThanOneHeader(strOldItem, pInfo->item.iCol))
				{
					m_ctrlCheckList.SetCheckBox(nFind, BST_UNCHECKED);
					UpdateElementList(nFind);
				}
			}
		}

		if (!strNewItem.IsEmpty())
		{
			//
			// check new item if it exists
			//
			std::map<CString, CString>::const_iterator mapIter = m_mapSubItem2Item.find(strNewItem);
			if (mapIter != m_mapSubItem2Item.end())
			{
				info.psz = mapIter->second;
			}
			else
			{
				info.psz = strNewItem;
			}
			int nFind = m_ctrlCheckList.FindItem(&info, -1); // Not case sensitive
			if (nFind != -1 && m_ctrlCheckList.GetItemText(nFind, 0).Compare(info.psz) == 0)
			{
				if (m_ctrlCheckList.GetCheckBox(nFind) == BST_INDETERMINATE)
				{
					CString strElement = m_ctrlCheckList.GetItemText(nFind, 1);
					if (strElement.Find("(") != -1)
					{
						CString strMsg = CString("Cannot enter both total ") + strElement.SpanExcluding("(") + " and " + strElement + ".";
// COMMENT: {4/20/2009 5:30:14 PM}						::AfxMessageBox(strMsg, MB_ICONEXCLAMATION);
						this->m_strErrMsg = strMsg;
					}
					else
					{
						// Find first checked redox item
						CString strRedox;
						for (int j = nFind + 1; ; ++j)
						{
							if (m_ctrlCheckList.GetCheckBox(j) == BST_CHECKED)
							{
								strRedox = m_ctrlCheckList.GetItemText(j, 1);
								break;
							}
						}
						ASSERT( strRedox.Find("(") != -1 );
						CString strMsg = CString("Cannot enter both total ") + strElement + " and " + strRedox + ".";
// COMMENT: {4/20/2009 5:30:09 PM}						::AfxMessageBox(strMsg, MB_ICONEXCLAMATION);
						this->m_strErrMsg = strMsg;
					}
					ASSERT(m_bIgnoreItemchanged);
					m_bIgnoreItemchanged = false;
					m_ctrlGrid.SetTextMatrix(pInfo->item.iRow, pInfo->item.iCol, _T(""));
					this->PostMessage(WM_DISPLAY_ERRORS, 0, 0);
					return FALSE;
				}
				else
				{
					// set check
					m_ctrlCheckList.SetCheckBox(nFind, BST_CHECKED);
					UpdateElementList(nFind);
				}
			}
		}
	}
	ASSERT(m_bIgnoreItemchanged);
	m_bIgnoreItemchanged = false;

	return bMakeChange;
}


LRESULT CKPSolution_SpreadPg1::OnAllowDeleteRow(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	ASSERT(nID == IDC_MSHFG_TOTALS);

	BOOL bAllowDeleteRow = (pInfo->item.iRow != 0);

	return bAllowDeleteRow;
}

LRESULT CKPSolution_SpreadPg1::OnAllowInsertRow(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	ASSERT(nID == IDC_MSHFG_TOTALS);

	BOOL bAllowInsertRow = (pInfo->item.iRow != 0);

	return bAllowInsertRow;
}

LRESULT CKPSolution_SpreadPg1::OnAllowInsertCol(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	ASSERT(nID == IDC_MSHFG_TOTALS);

	BOOL bAllowInsertCol = (pInfo->item.iCol != 0);

	return bAllowInsertCol;
}

LRESULT CKPSolution_SpreadPg1::OnAllowDeleteCol(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	ASSERT(nID == IDC_MSHFG_TOTALS);

	BOOL bAllowDeleteCol = (pInfo->item.iCol != 0);

	return bAllowDeleteCol;
}

LRESULT CKPSolution_SpreadPg1::OnInsertRow(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	ASSERT(nID == IDC_MSHFG_TOTALS);

	if (m_bFreeze)
	{
		return FALSE; // allow default handler
	}

	m_ctrlGrid.InsertRow(pInfo->item.iRow);

	// fix last row format
	ASSERT(m_ctrlGrid.GetFixedRows() == 0);

	CString strPerCentOne;
	strPerCentOne.Format(_T("%ld"), m_ctrlGrid.GetRows() - 1L);

	CString strLabel;
	AfxFormatString1(strLabel, IDS_GRID_ROW_FORMAT531, strPerCentOne);
	m_ctrlGrid.SetTextMatrix(m_ctrlGrid.GetRows() - 1, 0, strLabel);

	return TRUE; // disable default handler
}

LRESULT CKPSolution_SpreadPg1::OnInsertCol(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	ASSERT(nID == IDC_MSHFG_TOTALS);

	return FALSE; // allow default handler
}

LRESULT CKPSolution_SpreadPg1::OnDeleteCol(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	ASSERT(nID == IDC_MSHFG_TOTALS);

	// Note: selection saved and restored by CEditGrid::OnDelete

	// save state
	long nFillStyle = m_ctrlGrid.GetFillStyle();
	BOOL bRedraw    = m_ctrlGrid.GetRedraw();

	CString strOldItem = m_ctrlGrid.GetTextMatrix(0, pInfo->item.iCol);
	strOldItem.Replace(_T("(+"), _T("("));
	if (!strOldItem.IsEmpty())
	{
		//
		// uncheck old string if checked
		//
		LVFINDINFO info = {0};
		info.flags = LVFI_STRING;
		std::map<CString, CString>::const_iterator mapIter = m_mapSubItem2Item.find(strOldItem);
		if (mapIter != m_mapSubItem2Item.end())
		{
			info.psz = mapIter->second;
		}
		else
		{
			info.psz = strOldItem;
		}
		int nFind = m_ctrlCheckList.FindItem(&info, -1); // Not case sensitive
		if (nFind != -1 && m_ctrlCheckList.GetItemText(nFind, 0).Compare(info.psz) == 0)
		{
			strOldItem = m_ctrlCheckList.GetItemText(nFind, 0);
			if (!IsMoreThanOneHeader(strOldItem, pInfo->item.iCol))
			{
				ASSERT(!m_bIgnoreItemchanged);
				m_bIgnoreItemchanged = true;
				m_ctrlCheckList.SetCheckBox(nFind, BST_UNCHECKED);
				UpdateElementList(nFind);
				ASSERT(m_bIgnoreItemchanged);
				m_bIgnoreItemchanged = false;
			}
		}
	}


// COMMENT: {6/8/2001 5:44:30 PM}	CString strOld = m_ctrlGrid.GetTextMatrix(0, pInfo->item.iCol);
// COMMENT: {6/8/2001 5:44:30 PM}	info.psz = strOld;
// COMMENT: {6/8/2001 5:44:30 PM}	int nFind = m_ctrlCheckList.FindItem(&info, -1);
// COMMENT: {6/8/2001 5:44:30 PM}	if (nFind != -1)
// COMMENT: {6/8/2001 5:44:30 PM}	{
// COMMENT: {6/8/2001 5:44:30 PM}		bool bMoreThanOne = false;
// COMMENT: {6/8/2001 5:44:30 PM}		for (long nCol = 1; nCol < m_ctrlGrid.GetCols(0); ++nCol)
// COMMENT: {6/8/2001 5:44:30 PM}		{
// COMMENT: {6/8/2001 5:44:30 PM}			if (nCol == pInfo->item.iCol) continue;
// COMMENT: {6/8/2001 5:44:30 PM}			if (m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, nCol).Compare(strOld) == 0)
// COMMENT: {6/8/2001 5:44:30 PM}			{
// COMMENT: {6/8/2001 5:44:30 PM}				bMoreThanOne = true;
// COMMENT: {6/8/2001 5:44:30 PM}				break;
// COMMENT: {6/8/2001 5:44:30 PM}			}
// COMMENT: {6/8/2001 5:44:30 PM}		}
// COMMENT: {6/8/2001 5:44:30 PM}		if (!bMoreThanOne)
// COMMENT: {6/8/2001 5:44:30 PM}		{
// COMMENT: {6/8/2001 5:44:30 PM}			ASSERT(!m_bIgnoreItemchanged);
// COMMENT: {6/8/2001 5:44:30 PM}			m_bIgnoreItemchanged = true;
// COMMENT: {6/8/2001 5:44:30 PM}			m_ctrlCheckList.SetCheckBox(nFind, BST_UNCHECKED);
// COMMENT: {6/8/2001 5:44:30 PM}			UpdateElementList(nFind);
// COMMENT: {6/8/2001 5:44:30 PM}			ASSERT(m_bIgnoreItemchanged);
// COMMENT: {6/8/2001 5:44:30 PM}			m_bIgnoreItemchanged = false;
// COMMENT: {6/8/2001 5:44:30 PM}		}
// COMMENT: {6/8/2001 5:44:30 PM}	}

	// move deleted col to end
	m_ctrlGrid.SetColPosition(pInfo->item.iCol, 0, m_ctrlGrid.GetCols(0) - 1);

	// clear deleted col
	m_ctrlGrid.SetFillStyle(flexFillRepeat);
	m_ctrlGrid.SetRow(0);
	m_ctrlGrid.SetCol(m_ctrlGrid.GetCols(0) - 1);
	m_ctrlGrid.SetRowSel(m_ctrlGrid.GetRows() - 1);
	m_ctrlGrid.SetText(_T(""));

	// restore state
	m_ctrlGrid.SetFillStyle(nFillStyle);	
	if (bRedraw) m_ctrlGrid.SetRedraw(TRUE);

	return TRUE; // disable default handler
}

LRESULT CKPSolution_SpreadPg1::OnDeleteRow(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	return FALSE; // allow default handler
}


/////////////////////////////////////////////////////////////////////////////
// CKPSolution_SpreadPg2 property page

CKPSolution_SpreadPg2::CKPSolution_SpreadPg2() : baseKPSolution_SpreadPg2(CKPSolution_SpreadPg2::IDD)
{
	//{{AFX_DATA_INIT(CKPSolution_SpreadPg2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CKPSolution_SpreadPg2::~CKPSolution_SpreadPg2()
{
}

void CKPSolution_SpreadPg2::DoDataExchange(CDataExchange* pDX)
{
	baseKPSolution_SpreadPg2::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKPSolution_SpreadPg2)
	DDX_Control(pDX, IDC_DESCRIPTION, m_ctrlDescription);
	DDX_Control(pDX, IDC_CBO_CHARGE, m_cboChargeBalance);
	DDX_Control(pDX, IDC_MSHFG_TOTALS, m_ctrlGrid);
	//}}AFX_DATA_MAP


	// delay grid updates
	CDelayUpdate update(&m_ctrlGrid);


	if (pDX->m_bSaveAndValidate)
	{
		CString strChargeBalance;
		int nIndex = m_cboChargeBalance.GetCurSel();
		m_cboChargeBalance.GetLBText(nIndex, strChargeBalance);

		std::map< std::string, CConc> mapElement2Conc;
		for (long nRow = m_ctrlGrid.GetFixedRows(); nRow < m_ctrlGrid.GetRows(); ++nRow)
		{
			std::string sElement = m_ctrlGrid.GetTextMatrix(nRow, 0);

			CConc conc;
			conc.m_strDesc = sElement.c_str();

			//
			// Units
			//
			DDX_GridText(pDX, IDC_MSHFG_TOTALS, nRow, 1, conc.m_strUnits);
			if (!conc.m_strUnits.IsEmpty())
			{
				// check for valid units
				conc.m_strUnits = CConc::GetUnits(conc.m_strUnits);
				if (conc.m_strUnits.IsEmpty())
				{
					DDX_GridFail(pDX, _T("Invalid concentration units."));
				}
				if (conc.m_strUnits.Find("eq") != -1)
				{
					// test for alk
					if (conc.m_strDesc.Left(3).CompareNoCase(_T("alk")) != 0)
					{
						DDX_GridFail(pDX, _T("Only alkalinity can be entered in equivalents."));
					}
				}
			}

			//{{NEW
			//
			// Phase
			//
			DDX_GridText(pDX, IDC_MSHFG_TOTALS, nRow, 3, conc.m_strPhase);

			if (strChargeBalance.Compare(conc.m_strDesc) == 0)
			{
				if (!conc.m_strPhase.IsEmpty())
				{
					DDX_GridFail(pDX, _T("Phase name may not be used if charge has been specified for this element"));
				}
				else
				{
					conc.m_strPhase = _T("charge");
					conc.m_strPhaseSI.Empty();
				}
			}
			else if (!conc.m_strPhase.IsEmpty())
			{
				//
				// S.I.
				//
				DDX_GridText(pDX, IDC_MSHFG_TOTALS, nRow, 4, conc.m_strPhaseSI);
				if (!conc.m_strPhaseSI.IsEmpty())
				{
					DDX_GridText(pDX, IDC_MSHFG_TOTALS, nRow, 4, conc.m_dPhaseSI);
				}
			}
			//}}NEW

// COMMENT: {6/7/2001 3:18:16 PM}			//
// COMMENT: {6/7/2001 3:18:16 PM}			// Phase
// COMMENT: {6/7/2001 3:18:16 PM}			//
// COMMENT: {6/7/2001 3:18:16 PM}			DDX_GridText(pDX, IDC_MSHFG_TOTALS, nRow, 3, conc.m_strPhase);
// COMMENT: {6/7/2001 3:18:16 PM}
// COMMENT: {6/7/2001 3:18:16 PM}			if (!conc.m_strPhase.IsEmpty())
// COMMENT: {6/7/2001 3:18:16 PM}			{
// COMMENT: {6/7/2001 3:18:16 PM}				//
// COMMENT: {6/7/2001 3:18:16 PM}				// S.I.
// COMMENT: {6/7/2001 3:18:16 PM}				//
// COMMENT: {6/7/2001 3:18:16 PM}				DDX_GridText(pDX, IDC_MSHFG_TOTALS, nRow, 4, conc.m_strPhaseSI);
// COMMENT: {6/7/2001 3:18:16 PM}				if (!conc.m_strPhaseSI.IsEmpty())
// COMMENT: {6/7/2001 3:18:16 PM}				{
// COMMENT: {6/7/2001 3:18:16 PM}					DDX_GridText(pDX, IDC_MSHFG_TOTALS, nRow, 4, conc.m_dPhaseSI);
// COMMENT: {6/7/2001 3:18:16 PM}				}
// COMMENT: {6/7/2001 3:18:16 PM}			}

			//
			// As / GFW   ([as formula] or [gfw gfw])
			//
			DDX_GridText(pDX, IDC_MSHFG_TOTALS, nRow, 2, conc.m_strAs);
			if (!conc.m_strAs.IsEmpty() && !::_istalpha(conc.m_strAs[0]))
			{
				conc.m_strGFW = conc.m_strAs;
				conc.m_strAs.Empty();
				DDX_GridText(pDX, IDC_MSHFG_TOTALS, nRow, 2, conc.m_dGFW);
			}

			//
			// Redox
			//
			DDX_GridText(pDX, IDC_MSHFG_TOTALS, nRow, 5, conc.m_strRedox);

			// if here than conc is OK

			if (!(conc.m_strUnits.IsEmpty() && conc.m_strPhase.IsEmpty() && conc.m_strAs.IsEmpty() && conc.m_strGFW.IsEmpty() && conc.m_strRedox.IsEmpty()))
			{
				mapElement2Conc[sElement] = conc;
			}
			TRACE("%s\n", conc.GetSubHeading());
		}
		m_mapElement2Conc = mapElement2Conc;
	}
	else
	{
		if (m_bFirstSetActive)
		{
			m_ctrlGrid.SetTextMatrix(0, 0, _T("Element"));
			m_ctrlGrid.SetTextMatrix(0, 1, _T("[Units]"));
			m_ctrlGrid.SetTextMatrix(0, 2, _T("[As / GFW]"));
			m_ctrlGrid.SetTextMatrix(0, 3, _T("[Phase]"));
			m_ctrlGrid.SetTextMatrix(0, 4, _T("[S.I.]"));
			m_ctrlGrid.SetTextMatrix(0, 5, _T("[Redox]"));


			m_ctrlGrid.SetFillStyle(flexFillRepeat);
			m_ctrlGrid.SetRow(0);
			m_ctrlGrid.SetCol(1);
			m_ctrlGrid.SetColSel(m_ctrlGrid.GetCols(0) - 1);
			m_ctrlGrid.SetCellAlignment(flexAlignCenterCenter);
			m_ctrlGrid.SetFillStyle(flexFillSingle);

			m_ctrlGrid.SetColWidth(0, 0, 1300);
			m_ctrlGrid.SetColWidth(1, 0, 1300);
			m_ctrlGrid.SetColWidth(2, 0, 1300);
			m_ctrlGrid.SetColWidth(3, 0, 1300);
			m_ctrlGrid.SetColWidth(4, 0, 1300);
			m_ctrlGrid.SetColWidth(5, 0, 1300);


#ifdef _DEBUG
			CConc conc1 = CConc::Create("mg/kgw as HCO3");
			ASSERT(conc1.m_strUnits.Compare("mg/kgw") == 0);
			ASSERT(conc1.m_strAs.Compare("HCO3") == 0);
			ASSERT(conc1.m_strGFW.Compare("") == 0);
			ASSERT(conc1.m_strRedox.Compare("") == 0);
			ASSERT(conc1.m_strPhase.Compare("") == 0);
			ASSERT(conc1.m_strPhaseSI.Compare("") == 0);

			CConc conc2 = CConc::Create("ppm");
			ASSERT(conc2.m_strUnits.Compare("ppm") == 0);
			ASSERT(conc2.m_strAs.Compare("") == 0);
			ASSERT(conc2.m_strGFW.Compare("") == 0);
			ASSERT(conc2.m_strRedox.Compare("") == 0);
			ASSERT(conc2.m_strPhase.Compare("") == 0);
			ASSERT(conc2.m_strPhaseSI.Compare("") == 0);

			CConc conc3 = CConc::Create("mg/liter");
			ASSERT(conc3.m_strUnits.Compare("mg/l") == 0);
			ASSERT(conc3.m_strAs.Compare("") == 0);
			ASSERT(conc3.m_strGFW.Compare("") == 0);
			ASSERT(conc3.m_strRedox.Compare("") == 0);
			ASSERT(conc3.m_strPhase.Compare("") == 0);
			ASSERT(conc3.m_strPhaseSI.Compare("") == 0);

			CConc conc4 = CConc::Create("mg/L");
			ASSERT(conc4.m_strUnits.Compare("mg/l") == 0);
			ASSERT(conc4.m_strAs.Compare("") == 0);
			ASSERT(conc4.m_strGFW.Compare("") == 0);
			ASSERT(conc4.m_strRedox.Compare("") == 0);
			ASSERT(conc4.m_strPhase.Compare("") == 0);
			ASSERT(conc4.m_strPhaseSI.Compare("") == 0);

			CConc conc5 = CConc::Create("as HCO3");
			ASSERT(conc5.m_strUnits.Compare("") == 0);
			ASSERT(conc5.m_strAs.Compare("HCO3") == 0);
			ASSERT(conc5.m_strGFW.Compare("") == 0);
			ASSERT(conc5.m_strRedox.Compare("") == 0);
			ASSERT(conc5.m_strPhase.Compare("") == 0);
			ASSERT(conc5.m_strPhaseSI.Compare("") == 0);

			CConc conc6 = CConc::Create("     as SO4");
			ASSERT(conc6.m_strUnits.Compare("") == 0);
			ASSERT(conc6.m_strAs.Compare("SO4") == 0);
			ASSERT(conc6.m_strGFW.Compare("") == 0);
			ASSERT(conc6.m_strRedox.Compare("") == 0);
			ASSERT(conc6.m_strPhase.Compare("") == 0);
			ASSERT(conc6.m_strPhaseSI.Compare("") == 0);

			CConc conc7 = CConc::Create("      as S  ");
			ASSERT(conc7.m_strUnits.Compare("") == 0);
			ASSERT(conc7.m_strAs.Compare("S") == 0);
			ASSERT(conc7.m_strGFW.Compare("") == 0);
			ASSERT(conc7.m_strRedox.Compare("") == 0);
			ASSERT(conc7.m_strPhase.Compare("") == 0);
			ASSERT(conc7.m_strPhaseSI.Compare("") == 0);

			CConc conc8 = CConc::Create("    as HCO3      CO2(g)     -3.5");
			ASSERT(conc8.m_strUnits.Compare("") == 0);
			ASSERT(conc8.m_strAs.Compare("HCO3") == 0);
			ASSERT(conc8.m_strGFW.Compare("") == 0);
			ASSERT(conc8.m_strRedox.Compare("") == 0);
			ASSERT(conc8.m_strPhase.Compare("CO2(g)") == 0);
			ASSERT(conc8.m_strPhaseSI.Compare("-3.5") == 0);

			CConc conc9 = CConc::Create("     ug/kgs as Fe S(6)/S(-2) Pyrite");
			ASSERT(conc9.m_strUnits.Compare("ug/kgs") == 0);
			ASSERT(conc9.m_strAs.Compare("Fe") == 0);
			ASSERT(conc9.m_strGFW.Compare("") == 0);
			ASSERT(conc9.m_strRedox.Compare("S(6)/S(-2)") == 0);
			ASSERT(conc9.m_strPhase.Compare("Pyrite") == 0);
			ASSERT(conc9.m_strPhaseSI.Compare("") == 0);

			CConc conc10 = CConc::Create("");
			ASSERT(conc10.m_strUnits.Compare("") == 0);
			ASSERT(conc10.m_strAs.Compare("") == 0);
			ASSERT(conc10.m_strGFW.Compare("") == 0);
			ASSERT(conc10.m_strRedox.Compare("") == 0);
			ASSERT(conc10.m_strPhase.Compare("") == 0);
			ASSERT(conc10.m_strPhaseSI.Compare("") == 0);

			CConc conc11 = CConc::Create("ug/kg solution charge");
			ASSERT(conc11.m_strUnits.Compare("ug/kgs") == 0);
			ASSERT(conc11.m_strAs.Compare("") == 0);
			ASSERT(conc11.m_strGFW.Compare("") == 0);
			ASSERT(conc11.m_strRedox.Compare("") == 0);
			ASSERT(conc11.m_strPhase.Compare("charge") == 0);
			ASSERT(conc11.m_strPhaseSI.Compare("") == 0);
#endif

		}

		// initialize charge balance combo
		m_cboChargeBalance.ResetContent();
		int nIndex = m_cboChargeBalance.AddString(_T("(none)"));
		m_cboChargeBalance.SetCurSel(nIndex);

		// Elements
		long nRow = 1;
		m_ctrlGrid.SetRows(2);
		m_ctrlGrid.SetFixedRows(1);
		std::vector<std::string>::const_iterator cIter = ((COCKSSolution_Spread*)GetParent())->m_Page1.m_vsHeading.begin();
		for (; cIter != ((COCKSSolution_Spread*)GetParent())->m_Page1.m_vsHeading.end(); ++cIter)
		{
			ASSERT(!::_istspace((*cIter)[0]));
			if (::_istupper((*cIter)[0]) || (*cIter).compare(_T("pH")) == 0 || (*cIter).compare(_T("pe")) == 0) // TODO DAVID 
			{
				if (((COCKSSolution_Spread*)GetParent())->m_Page1.m_mapExtraHeadings.find(cIter->c_str()) ==
					((COCKSSolution_Spread*)GetParent())->m_Page1.m_mapExtraHeadings.end()
					|| 
					(*cIter).compare(_T("pH")) == 0 || (*cIter).compare(_T("pe")) == 0
					)
				{
					// add to charge balance combo
					nIndex = m_cboChargeBalance.AddString(cIter->c_str());

					// add to grid
					m_ctrlGrid.SetRows(m_ctrlGrid.GetRows() + 1);
					m_ctrlGrid.SetRow(nRow);
					m_ctrlGrid.SetTextMatrix(nRow, 0, cIter->c_str());
					m_ctrlGrid.SetCol(0);
					m_ctrlGrid.SetCellFontItalic(FALSE);
					std::map< std::string, CConc >::const_iterator cMapIter;
					cMapIter = m_mapElement2Conc.find(*cIter);
					if (cMapIter != m_mapElement2Conc.end())
					{
						CString strUnits = cMapIter->second.m_strUnits;
						strUnits.Replace("Mol", "mol");
						m_ctrlGrid.SetTextMatrix(nRow, 1, strUnits);
						if (!cMapIter->second.m_strAs.IsEmpty())
						{
							m_ctrlGrid.SetTextMatrix(nRow, 2, cMapIter->second.m_strAs);
						}
						else if (!cMapIter->second.m_strGFW.IsEmpty())
						{
							m_ctrlGrid.SetTextMatrix(nRow, 2, cMapIter->second.m_strGFW);
						}
						// check for charge balance
						if (cMapIter->second.m_strPhase.CompareNoCase(_T("charge")) == 0)
						{
							m_cboChargeBalance.SetCurSel(nIndex);
						}
						else
						{
							m_ctrlGrid.SetTextMatrix(nRow, 3, cMapIter->second.m_strPhase);
							m_ctrlGrid.SetTextMatrix(nRow, 4, cMapIter->second.m_strPhaseSI);
						}
						m_ctrlGrid.SetTextMatrix(nRow, 5, cMapIter->second.m_strRedox);
					}
					else
					{
						m_ctrlGrid.SetRow(nRow);
						m_ctrlGrid.SetCol(1);
						m_ctrlGrid.SetRowSel(nRow);
						m_ctrlGrid.SetColSel(5);
						// just fill with empty strings
						m_ctrlGrid.SetFillStyle(flexFillRepeat);
						m_ctrlGrid.SetText(_T(""));
						m_ctrlGrid.SetFillStyle(flexFillSingle);
					}
					nRow++;
				}
			}
		}
		if (m_ctrlGrid.GetRows() > 2)
		{
			m_ctrlGrid.SetRows(m_ctrlGrid.GetRows() - 1);
			m_ctrlGrid.EnableWindow(TRUE);
			if (m_ctrlGrid.GetRows() == 2)
			{
				// don't show selected rows or cols
				m_ctrlGrid.SetShowRowSelection(false);
				m_ctrlGrid.SetShowColSelection(false);
			}
			else
			{
				m_ctrlGrid.SetShowRowSelection(true);
				m_ctrlGrid.SetShowColSelection(true);
			}
		}
		else
		{
			// don't show selected rows or cols
			m_ctrlGrid.SetShowRowSelection(false);
			m_ctrlGrid.SetShowColSelection(false);
			m_ctrlGrid.SetTextMatrix(1, 0, _T("None Selected"));
			m_ctrlGrid.SetRow(1);
			m_ctrlGrid.SetCol(0);
			m_ctrlGrid.SetCellFontItalic(TRUE);
			m_ctrlGrid.EnableWindow(FALSE);
		}
		m_ctrlGrid.SetColAlignmentFixed(0, flexAlignCenterCenter);
		m_ctrlGrid.SetRow(1);
		m_ctrlGrid.SetCol(1);
	}

}


BEGIN_MESSAGE_MAP(CKPSolution_SpreadPg2, baseKPSolution_SpreadPg2)
	//{{AFX_MSG_MAP(CKPSolution_SpreadPg2)
	ON_WM_HELPINFO()
	ON_CBN_SETFOCUS(IDC_CBO_CHARGE, OnSetfocusCboCharge)
	//}}AFX_MSG_MAP
	ON_MESSAGE(EGN_BEGINCELLEDIT, OnBeginCellEdit)
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
	ON_MESSAGE(EGN_SETFOCUS, OnSetfocusGrid)
END_MESSAGE_MAP()

LRESULT CKPSolution_SpreadPg2::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	ASSERT(nID == IDC_MSHFG_TOTALS);

	BOOL bDenyEdit = FALSE;

	switch (pInfo->item.iCol)
	{
	case 1: // units
		{
			CString str = m_ctrlGrid.GetTextMatrix(pInfo->item.iRow, 0);
			bool bAlk = (str.Left(3).CompareNoCase("alk") == 0);
			pInfo->item.bUseCombo = (CUtil::InsertConcUnits(pInfo->item.hWndCombo, bAlk) > 0);
		}
		break;

	case 3:	// phase
		pInfo->item.bUseCombo = (CUtil::InsertPhases(pInfo->item.hWndCombo, GetDatabase()) > 0);
		break;

	case 5:	// redox
		pInfo->item.bUseCombo = (CUtil::InsertRedox(pInfo->item.hWndCombo, GetDatabase()) > 0);
		break;
	}
	return bDenyEdit;	// allow edit

}

LRESULT CKPSolution_SpreadPg2::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	ASSERT(nID == IDC_MSHFG_TOTALS);

	UNUSED(pInfo);

	BOOL bMakeChange = TRUE;

	// ignore if edit is cancelled
	if (pInfo->item.pszText == NULL) return bMakeChange;
	return bMakeChange;
}



/////////////////////////////////////////////////////////////////////////////
// CKPSolution_SpreadPg3 property page

CKPSolution_SpreadPg3::CKPSolution_SpreadPg3() : baseKPSolution_SpreadPg3(CKPSolution_SpreadPg3::IDD)
{
	DefaultValues();
	//{{AFX_DATA_INIT(CKPSolution_SpreadPg3)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CKPSolution_SpreadPg3::DefaultValues()
{
	m_dTemp           = 25.0;
	m_dPH             = 7.0;
	m_dPE             = 4.0;
	m_strRedox        = _T("pe");
	m_strDefaultUnits = _T("mmol/kgw");
	m_dDensity        = 1.0;
	m_dWaterMass      = 1.0;
}

CKPSolution_SpreadPg3::~CKPSolution_SpreadPg3()
{
}

//DEL void CKPSolution_SpreadPg3::RearrangeGrid(bool bForBaseClass)
//DEL {
//DEL 	if (bForBaseClass)
//DEL 	{
//DEL 		if (m_ctrlGrid.GetTextMatrix(4, 1).Compare(_T("  [Equilibrium]")))
//DEL 		{
//DEL 			// nothing to do
//DEL 			ASSERT(m_ctrlGrid.GetTextMatrix(5, 1).Compare(_T("  [Sat. index]")) == 0);
//DEL 			ASSERT(m_ctrlGrid.GetTextMatrix(7, 1).Compare(_T("  [Equilibrium]")) == 0);
//DEL 			ASSERT(m_ctrlGrid.GetTextMatrix(8, 1).Compare(_T("  [Sat. index]")) == 0);
//DEL 		}
//DEL 		else
//DEL 		{
//DEL 			ASSERT(m_ctrlGrid.GetTextMatrix(4, 1).Compare(_T("pe")) == 0);
//DEL 
//DEL 			ASSERT(m_ctrlGrid.GetTextMatrix(m_ctrlGrid.GetRows() - 4, 1).Compare(_T("  [Equilibrium]")) == 0);
//DEL 			m_ctrlGrid.SetRowPosition(m_ctrlGrid.GetRows() - 4, 4);
//DEL 
//DEL 			ASSERT(m_ctrlGrid.GetTextMatrix(m_ctrlGrid.GetRows() - 3, 1).Compare(_T("  [Sat. index]")) == 0);
//DEL 			m_ctrlGrid.SetRowPosition(m_ctrlGrid.GetRows() - 3, 5);
//DEL 
//DEL 			ASSERT(m_ctrlGrid.GetTextMatrix(m_ctrlGrid.GetRows() - 2, 1).Compare(_T("  [Equilibrium]")) == 0);
//DEL 			m_ctrlGrid.SetRowPosition(m_ctrlGrid.GetRows() - 2, 7);
//DEL 
//DEL 			ASSERT(m_ctrlGrid.GetTextMatrix(m_ctrlGrid.GetRows() - 1, 1).Compare(_T("  [Sat. index]")) == 0);
//DEL 			m_ctrlGrid.SetRowPosition(m_ctrlGrid.GetRows() - 1, 8);
//DEL 		}
//DEL 	}
//DEL 	else
//DEL 	{
//DEL 		if (m_ctrlGrid.GetTextMatrix(4, 1).Compare(_T("pe")))
//DEL 		{
//DEL 			// nothing to do
//DEL 			ASSERT(m_ctrlGrid.GetTextMatrix(5, 1).Compare(_T("Temperature (°C)")) == 0);
//DEL 		}
//DEL 		else
//DEL 		{
//DEL 			m_ctrlGrid.SetRowPosition(4, m_ctrlGrid.GetRows() - 1);
//DEL 			m_ctrlGrid.SetRowPosition(4, m_ctrlGrid.GetRows() - 1);
//DEL 
//DEL 			m_ctrlGrid.SetRowPosition(5, m_ctrlGrid.GetRows() - 1);
//DEL 			m_ctrlGrid.SetRowPosition(5, m_ctrlGrid.GetRows() - 1);
//DEL 
//DEL 			ASSERT(m_ctrlGrid.GetTextMatrix(4, 1).Compare(_T("pe")) == 0);
//DEL 			ASSERT(m_ctrlGrid.GetTextMatrix(5, 1).Compare(_T("Temperature (°C)")) == 0);
//DEL 		}
//DEL 	}
//DEL }

void CKPSolution_SpreadPg3::DoDataExchange(CDataExchange* pDX)
{
	baseKPSolution_SpreadPg3::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CKPSolution_SpreadPg3)
	DDX_Control(pDX, IDC_DESCRIPTION, m_ctrlDescription);
	DDX_Control(pDX, IDC_MSHFLEXGRID1, m_ctrlGrid);
	//}}AFX_DATA_MAP

	DDX_Grid(pDX, IDC_MSHFLEXGRID1);

// COMMENT: {5/23/2001 4:00:46 PM}	if (m_bFirstSetActive)
// COMMENT: {5/23/2001 4:00:46 PM}	{
// COMMENT: {5/23/2001 4:00:46 PM}// COMMENT: {5/23/2001 3:54:34 PM}		m_ctrlGrid.SetRowHeight(0, 0);
// COMMENT: {5/23/2001 4:00:46 PM}// COMMENT: {5/23/2001 3:54:34 PM}		m_ctrlGrid.SetRowHeight(1, 0);
// COMMENT: {5/23/2001 4:00:46 PM}		// m_ctrlGrid.SetRowHeight(2, 0);
// COMMENT: {5/23/2001 4:00:46 PM}		m_ctrlGrid.SetTextMatrix(2, 1, _T("Units"));
// COMMENT: {5/23/2001 4:00:46 PM}
// COMMENT: {5/23/2001 4:00:46 PM}		// turn-off bold titles
// COMMENT: {5/23/2001 4:00:46 PM}		m_ctrlGrid.SetRow(0), m_ctrlGrid.SetCol(1);
// COMMENT: {5/23/2001 4:00:46 PM}		m_ctrlGrid.SetCellFontBold(FALSE);
// COMMENT: {5/23/2001 4:00:46 PM}		m_ctrlGrid.SetRow(3), m_ctrlGrid.SetCol(1);
// COMMENT: {5/23/2001 4:00:46 PM}		m_ctrlGrid.SetCellFontBold(FALSE);
// COMMENT: {5/23/2001 4:00:46 PM}		m_ctrlGrid.SetRow(6), m_ctrlGrid.SetCol(1);
// COMMENT: {5/23/2001 4:00:46 PM}		m_ctrlGrid.SetCellFontBold(FALSE);
// COMMENT: {5/23/2001 4:00:46 PM}		m_ctrlGrid.SetRow(9), m_ctrlGrid.SetCol(1);
// COMMENT: {5/23/2001 4:00:46 PM}		m_ctrlGrid.SetCellFontBold(FALSE);
// COMMENT: {5/23/2001 4:00:46 PM}
// COMMENT: {5/23/2001 4:00:46 PM}		// hide column 0
// COMMENT: {5/23/2001 4:00:46 PM}		m_ctrlGrid.SetColWidth(0, 0, 0);
// COMMENT: {5/23/2001 4:00:46 PM}
// COMMENT: {5/23/2001 4:00:46 PM}		//{{ TEMP
// COMMENT: {5/23/2001 4:00:46 PM}#ifdef _DEBUG
// COMMENT: {5/23/2001 4:00:46 PM}		m_ctrlGrid.SetRowHeight(4, m_ctrlGrid.GetRowHeight(3));
// COMMENT: {5/23/2001 4:00:46 PM}		m_ctrlGrid.SetRowHeight(5, m_ctrlGrid.GetRowHeight(3));
// COMMENT: {5/23/2001 4:00:46 PM}		m_ctrlGrid.SetRowHeight(7, m_ctrlGrid.GetRowHeight(3));
// COMMENT: {5/23/2001 4:00:46 PM}		m_ctrlGrid.SetRowHeight(8, m_ctrlGrid.GetRowHeight(3));
// COMMENT: {5/23/2001 4:00:46 PM}#endif
// COMMENT: {5/23/2001 4:00:46 PM}		//}} TEMP
// COMMENT: {5/23/2001 4:00:46 PM}
// COMMENT: {5/23/2001 4:00:46 PM}	}
// COMMENT: {5/23/2001 4:00:46 PM}	if (pDX->m_bSaveAndValidate)
// COMMENT: {5/23/2001 4:00:46 PM}	{
// COMMENT: {5/23/2001 4:00:46 PM}		//
// COMMENT: {5/23/2001 4:00:46 PM}		// Units
// COMMENT: {5/23/2001 4:00:46 PM}		//
// COMMENT: {5/23/2001 4:00:46 PM}		CString strTemp;
// COMMENT: {5/23/2001 4:00:46 PM}		DDX_GridText(pDX, IDC_MSHFLEXGRID1, 2, 2, strTemp);
// COMMENT: {5/23/2001 4:00:46 PM}		if (strTemp.IsEmpty())
// COMMENT: {5/23/2001 4:00:46 PM}		{
// COMMENT: {5/23/2001 4:00:46 PM}			m_strDefaultUnits = _T("mmol/kgw");
// COMMENT: {5/23/2001 4:00:46 PM}		}
// COMMENT: {5/23/2001 4:00:46 PM}		else
// COMMENT: {5/23/2001 4:00:46 PM}		{
// COMMENT: {5/23/2001 4:00:46 PM}// COMMENT: {5/23/2001 2:05:57 PM}			strTemp.MakeLower();
// COMMENT: {5/23/2001 4:00:46 PM}			strTemp.Replace(_T("kgs"), _T("kg solution"));
// COMMENT: {5/23/2001 4:00:46 PM}			strTemp.Replace(_T("kgw"), _T("kg water"));
// COMMENT: {5/23/2001 4:00:46 PM}
// COMMENT: {5/23/2001 4:00:46 PM}			// get long units from string resource
// COMMENT: {5/23/2001 4:00:46 PM}			CString strRes;
// COMMENT: {5/23/2001 4:00:46 PM}			VERIFY(strRes.LoadString(IDS_CONC_UNITS_190));
// COMMENT: {5/23/2001 4:00:46 PM}
// COMMENT: {5/23/2001 4:00:46 PM}			LPTSTR lpsz, lpszUnits;
// COMMENT: {5/23/2001 4:00:46 PM}			lpsz = strRes.LockBuffer();
// COMMENT: {5/23/2001 4:00:46 PM}			lpszUnits = _tcstok(lpsz, _T(";"));
// COMMENT: {5/23/2001 4:00:46 PM}			while (lpszUnits)
// COMMENT: {5/23/2001 4:00:46 PM}			{
// COMMENT: {5/23/2001 4:00:46 PM}				CString strValidLongUnit = lpszUnits;
// COMMENT: {5/23/2001 4:00:46 PM}				if (strValidLongUnit == strTemp)
// COMMENT: {5/23/2001 4:00:46 PM}				{
// COMMENT: {5/23/2001 4:00:46 PM}					m_strDefaultUnits = strTemp;
// COMMENT: {5/23/2001 4:00:46 PM}					break;
// COMMENT: {5/23/2001 4:00:46 PM}				}
// COMMENT: {5/23/2001 4:00:46 PM}				lpszUnits = _tcstok(NULL, _T(";"));
// COMMENT: {5/23/2001 4:00:46 PM}			}
// COMMENT: {5/23/2001 4:00:46 PM}			strRes.UnlockBuffer();
// COMMENT: {5/23/2001 4:00:46 PM}			if (lpszUnits == NULL)
// COMMENT: {5/23/2001 4:00:46 PM}			{
// COMMENT: {5/23/2001 4:00:46 PM}				// valid units not found
// COMMENT: {5/23/2001 4:00:46 PM}				DDX_GridFail(pDX, IDS_STRING523, IDS_STRING522);
// COMMENT: {5/23/2001 4:00:46 PM}			}
// COMMENT: {5/23/2001 4:00:46 PM}		}
// COMMENT: {5/23/2001 4:00:46 PM}
// COMMENT: {5/23/2001 4:00:46 PM}		// convert to standard unit
// COMMENT: {5/23/2001 4:00:46 PM}		m_strDefaultUnits.Replace(_T("kg solution"), _T("kgs"));
// COMMENT: {5/23/2001 4:00:46 PM}		m_strDefaultUnits.Replace(_T("kg water"), _T("kgw"));
// COMMENT: {5/23/2001 4:00:46 PM}	}
// COMMENT: {5/23/2001 4:00:46 PM}	else
// COMMENT: {5/23/2001 4:00:46 PM}	{
// COMMENT: {5/23/2001 4:00:46 PM}		// default units
// COMMENT: {5/23/2001 4:00:46 PM}		CString strTemp = m_strDefaultUnits;
// COMMENT: {5/23/2001 4:00:46 PM}		strTemp.Replace(_T("s"), _T(" solution"));
// COMMENT: {5/23/2001 4:00:46 PM}		strTemp.Replace(_T("w"), _T(" water"));
// COMMENT: {5/23/2001 4:00:46 PM}		m_ctrlGrid.SetTextMatrix(2, 2, strTemp);
// COMMENT: {5/23/2001 4:00:46 PM}	}
}

void CKPSolution_SpreadPg3::DDX_Grid(CDataExchange *pDX, int nIDC)
{
	ASSERT(nIDC == IDC_MSHFLEXGRID1);

	// avoid flashing grid
	CDelayUpdate update(this, nIDC);

	if (m_bFirstSetActive)
	{
		InitGrid(pDX, IDC_MSHFLEXGRID1);
	}

	long nCol = 1;

	if (pDX->m_bSaveAndValidate)
	{
		CString strValue;

		//
		// Temperature (°C)
		//
		DDX_GridText(pDX, nIDC, 0, nCol, strValue);
		if (strValue.IsEmpty())
		{
			m_dTemp = 25.0;
		}
		else
		{
			DDX_GridText(pDX, nIDC, 0, nCol, m_dTemp);
		}

		//
		// pH
		//
		DDX_GridText(pDX, nIDC, 1, nCol, strValue);
		if (strValue.IsEmpty())
		{
			m_dPH = 7.0;
		}
		else
		{
			DDX_GridText(pDX, nIDC, 1, nCol, m_dPH);
			if (!(2.0 <= m_dPH && m_dPH <= 15.))
			{
				DDX_GridFail(pDX, _T("pH must be between 2 and 15"));
			}
		}

		//
		// pe
		//
		DDX_GridText(pDX, nIDC, 2, nCol, strValue);
		if (strValue.IsEmpty())
		{
			m_dPE = 4.0;
		}
		else
		{
			DDX_GridText(pDX, nIDC, 2, nCol, m_dPE);
			if (!(-20.0 < m_dPE && m_dPE < 20.0))
			{
				DDX_GridFail(pDX, _T("pe must be between -20 and 20"));
			}
		}

		//
		// Redox
		//
		DDX_GridText(pDX, nIDC, 3, nCol, m_strRedox);
		if (m_strRedox.IsEmpty())
		{
			m_strRedox = _T("pe");
		}
		else
		{
			if (!CUtil::IsValidRedoxCouple(m_strRedox))
			{
				DDX_GridFail(pDX, CUtil::GetLastErrorString());
			}
		}

		//
		// Units
		//
		DDX_GridText(pDX, IDC_MSHFLEXGRID1, 4, nCol, strValue);
		if (strValue.IsEmpty())
		{
			m_strDefaultUnits = _T("mmol/kgw");
		}
		else
		{
			strValue.Replace(_T("kgs"), _T("kg solution"));
			strValue.Replace(_T("kgw"), _T("kg water"));

			// get long units from string resource
			CString strRes;
			VERIFY(strRes.LoadString(IDS_CONC_UNITS_190));

			LPTSTR lpsz, lpszUnits;
			lpsz = strRes.LockBuffer();
			lpszUnits = _tcstok(lpsz, _T(";"));
			while (lpszUnits)
			{
				if (strValue.CompareNoCase(lpszUnits) == 0)
				{
					m_strDefaultUnits = lpszUnits;
					break;
				}
				lpszUnits = _tcstok(NULL, _T(";"));
			}
			strRes.UnlockBuffer();
			if (lpszUnits == NULL)
			{
				// valid units not found
				DDX_GridFail(pDX, IDS_STRING523, IDS_STRING522);
			}
		}
		// convert to standard unit
		m_strDefaultUnits.Replace(_T("kg solution"), _T("kgs"));
		m_strDefaultUnits.Replace(_T("kg water"), _T("kgw"));


		//
		// Density (g/cm^3)
		//
		DDX_GridText(pDX, nIDC, 5, nCol, strValue);
		if (strValue.IsEmpty())
		{
			m_dDensity = 1.0;
		}
		else
		{
			DDX_GridText(pDX, nIDC, 5, nCol, m_dDensity);
			if (!(m_dDensity > 0.0))
			{
				DDX_GridFail(pDX, _T("Density must be a positive number."));
			}
		}

		//
		// Water mass (kg)
		//
		DDX_GridText(pDX, nIDC, 6, nCol, strValue);
		if (strValue.IsEmpty())
		{
			m_dWaterMass = 1.0;
		}
		else
		{
			DDX_GridText(pDX, nIDC, 6, nCol, m_dWaterMass);
			if (!(m_dWaterMass > 0.0))
			{
				DDX_GridFail(pDX, _T("Water mass must be a positive number."));
			}
		}
	}
	else
	{
		//
		// Temperature (°C)
		//
		DDX_GridText(pDX, nIDC, 0, nCol, m_dTemp);

		//
		// pH
		//
		DDX_GridText(pDX, nIDC, 1, nCol, m_dPH);

		//
		// pe
		//
		DDX_GridText(pDX, nIDC, 2, nCol, m_dPE);

		//
		// Redox
		//
		DDX_GridText(pDX, nIDC, 3, nCol, m_strRedox);

		//
		// Units
		//
		CString strLongUnits(m_strDefaultUnits);
		strLongUnits.Replace(_T("kgs"), _T("kg solution"));
		strLongUnits.Replace(_T("kgw"), _T("kg water"));
		DDX_GridText(pDX, IDC_MSHFLEXGRID1, 4, nCol, strLongUnits);

		//
		// Density (g/cm^3)
		//
		DDX_GridText(pDX, nIDC, 5, nCol, m_dDensity);

		//
		// Water mass (kg)
		//
		DDX_GridText(pDX, nIDC, 6, nCol, m_dWaterMass);
	}

	// set starting position
	m_ctrlGrid.SetRow(0);
	m_ctrlGrid.SetCol(1);
}

void CKPSolution_SpreadPg3::InitGrid(CDataExchange* pDX, int nIDC)
{
	UNUSED(pDX);
	ASSERT(nIDC == m_ctrlGrid.GetDlgCtrlID());
	UNUSED(nIDC);   // unused in release builds

	// set title column widths
	m_ctrlGrid.SetColWidth(0, 0, 1550);

	// set edit column width
	CRect rect;
	CDC* pDC = GetDC();
	int nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
	m_ctrlGrid.GetClientRect(&rect);
	m_ctrlGrid.SetColWidth(1, 0, MulDiv(rect.right, TWIPS_PER_INCH, nLogX) 
		- m_ctrlGrid.GetColWidth(0, 0)  //1550
		);

	// set grid titles
    m_ctrlGrid.SetTextMatrix( 0, 0, _T("Temperature (°C)"));
    m_ctrlGrid.SetTextMatrix( 1, 0, _T("pH"));
    m_ctrlGrid.SetTextMatrix( 2, 0, _T("pe"));
    m_ctrlGrid.SetTextMatrix( 3, 0, _T("Redox"));
    m_ctrlGrid.SetTextMatrix( 4, 0, _T("Units"));
    m_ctrlGrid.SetTextMatrix( 5, 0, _T("Density (g/cm^3)"));
    m_ctrlGrid.SetTextMatrix( 6, 0, _T("Water mass (kg)"));

	// set starting position
	m_ctrlGrid.SetRow(0);
	m_ctrlGrid.SetCol(1);
}

BEGIN_MESSAGE_MAP(CKPSolution_SpreadPg3, baseKPSolution_SpreadPg3)
	//{{AFX_MSG_MAP(CKPSolution_SpreadPg3)
	ON_WM_HELPINFO()
	ON_BN_CLICKED(IDC_RESET, OnReset)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	// custom grid messages
	ON_MESSAGE(EGN_BEGINCELLEDIT, OnBeginCellEdit)
	ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
	ON_MESSAGE(EGN_SETFOCUS, OnSetfocusGrid)
	// custom setfocus handlers
	ON_BN_SETFOCUS(IDC_RESET, OnSetfocusReset)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CKPSolution_SpreadPg3, baseKPSolution_SpreadPg3)
    //{{AFX_EVENTSINK_MAP(CKPSolution_SpreadPg3)
	ON_EVENT(CKPSolution_SpreadPg3, IDC_MSHFLEXGRID1, 71 /* EnterCell */, OnEnterCellGrid, VTS_NONE)
	ON_EVENT(CKPSolution_SpreadPg3, IDC_MSHFLEXGRID1, -602 /* KeyDown */, OnKeyDownGrid, VTS_PI2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CKPSolution_SpreadPg3::OnReset() 
{
	DefaultValues();
	UpdateData(FALSE);	
}

LRESULT CKPSolution_SpreadPg3::OnBeginCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	ASSERT(nID == IDC_MSHFLEXGRID1);

	UNUSED(pInfo);

	BOOL bDenyEdit = FALSE;

	switch (pInfo->item.iRow)
	{
	case 3: // Redox
		pInfo->item.bUseCombo = (CUtil::InsertRedox(pInfo->item.hWndCombo, GetDatabase()) > 0);
		break;
	case 4: // Units
		pInfo->item.bUseCombo = (CUtil::InsertConcUnits(pInfo->item.hWndCombo) > 0);
		break;
	}
	return bDenyEdit;
}

LRESULT CKPSolution_SpreadPg3::OnEndCellEdit(WPARAM wParam, LPARAM lParam)
{
	NMEGINFO* pInfo = (NMEGINFO*)lParam;
	UINT nID = wParam;
	ASSERT(nID == IDC_MSHFLEXGRID1);

	UNUSED(pInfo);

	BOOL bMakeChange = TRUE;

	// ignore if edit is cancelled
	if (pInfo->item.pszText == NULL) return bMakeChange;

	ASSERT(pInfo->item.iCol == 1);

	return bMakeChange;
}

LRESULT CKPSolution_SpreadPg3::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
{
	ASSERT((int)wParam == m_ctrlGrid.GetDlgCtrlID());
	UNUSED(wParam); // unused in release builds
	UNUSED_ALWAYS(lParam);

	OnEnterCellGrid();
	return 0;
}

void CKPSolution_SpreadPg3::OnEnterCellGrid() 
{
	/*
	m_ctrlGrid.SetTextMatrix( 0, 0, _T("Temperature (°C)"));
	m_ctrlGrid.SetTextMatrix( 1, 0, _T("pH"));
	m_ctrlGrid.SetTextMatrix( 2, 0, _T("pe"));
	m_ctrlGrid.SetTextMatrix( 3, 0, _T("Redox"));
	m_ctrlGrid.SetTextMatrix( 4, 0, _T("Units"));
	m_ctrlGrid.SetTextMatrix( 5, 0, _T("Density (g/cm^3)"));
	m_ctrlGrid.SetTextMatrix( 6, 0, _T("Water mass (kg)"));
	*/
	CString strRes;
	switch (m_ctrlGrid.GetRow())
	{
	case 0:
		strRes.LoadString(IDS_STRING524);
		break;
	case 1:
		strRes.LoadString(IDS_STRING525);
		break;
	case 2:
		strRes.LoadString(IDS_STRING526);
		break;
	case 3:
		strRes.LoadString(IDS_STRING527);
		break;
	case 4:
		strRes.LoadString(IDS_STRING528);
		break;
	case 5:
		strRes.LoadString(IDS_STRING529);
		break;
	case 6:
		strRes.LoadString(IDS_STRING530);
		break;
	}
	m_ctrlDescription.SetWindowText(strRes);

}

void CKPSolution_SpreadPg3::OnSetfocusReset()
{
	// in order for a button to recieve this notification
	// it must have the notify style set
	CString strRes;
	strRes.LoadString(IDS_STRING465);
	m_ctrlDescription.SetWindowText(strRes);
}

void CKPSolution_SpreadPg3::OnKeyDownGrid(short FAR* KeyCode, short Shift) 
{
	// Add your control notification handler code here
	UNUSED(Shift);

	switch (*KeyCode)
	{
	case VK_DELETE:
		m_ctrlGrid.SetFillStyle(flexFillRepeat);
		m_ctrlGrid.SetText(_T(""));
		m_ctrlGrid.SetFillStyle(flexFillSingle);
		break;
	}	
}

/////////////////////////////////////////////////////////////////////////////
// CKPSolution_SpreadPg4 property page

CKPSolution_SpreadPg4::CKPSolution_SpreadPg4() : baseKPSolution_SpreadPg4()
{
	//{{AFX_DATA_INIT(CKPSolution_SpreadPg4)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CKPSolution_SpreadPg4::~CKPSolution_SpreadPg4()
{
}

void CKPSolution_SpreadPg4::DoDataExchange(CDataExchange* pDX)
{
	baseKPSolutionPg3::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKPSolutionPg3)
	DDX_Control(pDX, IDC_DESCRIPTION, m_eInputDesc);
	DDX_Control(pDX, IDC_ISOTOPE_LIST, m_ctrlIsotopes);
	DDX_Control(pDX, IDC_ISOTOPE_GRID, m_ctrlGrid);
	//}}AFX_DATA_MAP

	if (m_bFirstSetActive)
	{
		// add isotopes to check list control
		CUtil::InsertIsotopes(&m_ctrlIsotopes);
	}
	DDX_IsotopeList(pDX, IDC_ISOTOPE_GRID);

}

void CKPSolution_SpreadPg4::DDX_IsotopeList(CDataExchange* pDX, int nIDC)
{
	if (m_bFirstSetActive)
	{
		InitGrid(pDX, nIDC);
	}

	// avoid flashing grid
	CDelayUpdate update(this, nIDC);

	if (pDX->m_bSaveAndValidate)
	{
		std::list<CIsotope> listIsotopes;
		for (long nRow = m_ctrlGrid.GetFixedRows(); nRow < m_ctrlGrid.GetRows(); ++nRow)
		{
			CIsotope isotope;

			//
			// Name
			//
			DDX_GridText(pDX, nIDC, nRow, 1, isotope.m_strName);
			if (isotope.m_strName.IsEmpty())
				continue;

			if (!(::_istdigit((int)isotope.m_strName[0]))) // TODO DAVID
			{
				DDX_GridFail(pDX, _T("Expected isotope name to begin with an isotopic number."));
			}

			TCHAR* next_char;
			isotope.m_dIsotopeNumber = strtod(isotope.m_strName, &next_char);

			if (*next_char == _T('\0') || !::_istupper(*next_char)) // TODO DAVID
			{
				DDX_GridFail(pDX, _T("Expected element name to follow isotopic number."));
			}

			//
			// Value
			//
			DDX_GridTextNaN(pDX, nIDC, nRow, 2, isotope.m_dRatio);

			//
			// Uncertainty limit
			// 
			DDX_GridTextNaN(pDX, nIDC, nRow, 3, isotope.m_dRatioUncertainty);

			// OK
			listIsotopes.push_back(isotope);
		}
		// All OK
		((COCKSSolution_Spread*)GetParent())->m_listIsotopes.assign(listIsotopes.begin(), listIsotopes.end());
	}
	else
	{
		// reset each time
		m_glDoc.DeleteAllRows();

		std::list<CIsotope>::const_iterator iter = ((COCKSSolution_Spread*)GetParent())->m_listIsotopes.begin();
		for (; iter != ((COCKSSolution_Spread*)GetParent())->m_listIsotopes.end(); ++iter)
		{
			CIsotope isotope(*iter);

			//
			// Name
			//
			long nRow = m_glDoc.AddItem(isotope.m_strName).m_nRow;

			//
			// Value
			//
			DDX_GridTextNaN(pDX, nIDC, nRow, 2, isotope.m_dRatio);

			//
			// Uncertainty limit
			// 
			DDX_GridTextNaN(pDX, nIDC, nRow, 3, isotope.m_dRatioUncertainty);
		}
	}

	// reset initial pos
	m_ctrlGrid.SetRow(m_ctrlGrid.GetFixedRows());
	m_ctrlGrid.SetCol(m_ctrlGrid.GetFixedCols());

}


BEGIN_MESSAGE_MAP(CKPSolution_SpreadPg4, baseKPSolution_SpreadPg4)
	//{{AFX_MSG_MAP(CKPSolution_SpreadPg4)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
	// custom grid messages
	//ON_MESSAGE(EGN_BEGINCELLEDIT, OnBeginCellEdit)
	//ON_MESSAGE(EGN_ENDCELLEDIT, OnEndCellEdit)
	ON_MESSAGE(EGN_SETFOCUS, OnSetfocusGrid)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CKPSolution_SpreadPg4, baseKPSolution_SpreadPg4)
    //{{AFX_EVENTSINK_MAP(CKPSolution_SpreadPg4)
	ON_EVENT(CKPSolution_SpreadPg4, IDC_ISOTOPE_GRID, 71 /* EnterCell */, OnEnterCellIsotopeGrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CKPSolution_SpreadPg4::OnEnterCellIsotopeGrid() 
{
	CString strRes;

	switch (m_ctrlGrid.GetCol())
	{
	case 1:
		strRes.LoadString(IDS_STRING543);
		break;
	case 2:
		strRes.LoadString(IDS_STRING544);
		break;
	case 3:
		strRes.LoadString(IDS_STRING545);
		break;
	}
	m_eInputDesc.SetWindowText(strRes);
}

LRESULT CKPSolution_SpreadPg4::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
{
	ASSERT((int)wParam == m_ctrlGrid.GetDlgCtrlID());
	UNUSED(wParam);  // unused in release builds
	UNUSED_ALWAYS(lParam);
	OnEnterCellIsotopeGrid();
	return 0;
}


BOOL CKPSolution_SpreadPg1::OnApply() 
{
	// Add your specialized code here and/or call the base class
	
	return baseKPSolution_SpreadPg1::OnApply();
}

BOOL CKPSolution_SpreadPg2::OnApply() 
{
	// Add your specialized code here and/or call the base class
	
	return baseKPSolution_SpreadPg2::OnApply();
}

BOOL CKPSolution_SpreadPg3::OnApply() 
{
	// Add your specialized code here and/or call the base class
	
	return baseKPSolution_SpreadPg3::OnApply();
}

BOOL CKPSolution_SpreadPg4::OnApply() 
{
	// Add your specialized code here and/or call the base class
	
	return baseKPSolution_SpreadPg4::OnApply();
}


BOOL CKPSolution_SpreadPg2::OnHelpInfo(HELPINFO* pHelpInfo) 
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
	CString strResEx;

	switch (pHelpInfo->iCtrlId)
	{
	case IDC_CBO_CHARGE: case IDC_S_CHARGE:
		strRes.LoadString(IDS_SOL_205);
		break;
	case IDC_MSHFG_TOTALS:
		{
			if (!IsContextHelp())
			{
				if (!(m_ctrlGrid.GetRowIsVisible(m_ctrlGrid.GetRow()) && m_ctrlGrid.GetColIsVisible(m_ctrlGrid.GetCol())))
				{
					::MessageBeep((UINT)-1);
					return TRUE;
				}

				// modify placement
				CDC* pDC = m_ctrlGrid.GetDC();
				int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
				int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

				long nLeft = ::MulDiv(m_ctrlGrid.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
				long nTop  = ::MulDiv(m_ctrlGrid.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

				CRect rc;
				m_ctrlGrid.GetWindowRect(rc);

				myPopup.pt.x = rc.left + nLeft;
				myPopup.pt.y = rc.top + nTop + 10;
			}
			switch (IsContextHelp() ? m_ctrlGrid.GetMouseCol() : m_ctrlGrid.GetCol())
			{
			case 1: // Units
				strRes.LoadString(IDS_ELEM_197);
				break;
			case 2: // As / GFW
				strRes.LoadString(IDS_ELEM_200);
				strResEx.LoadString(IDS_ELEM_201);
				strRes += strResEx;
				break;
			case 3: // Phase
				strRes.LoadString(IDS_ELEM_198);
				break;
			case 4: // S.I.
				strRes.LoadString(IDS_ELEM_199);
				break;
			case 5: // Redox
				strRes.LoadString(IDS_ELEM_202);
				strResEx.LoadString(IDS_ELEM_203);
				strRes += strResEx;
				break;
			default:
				// No help topic is associated with this item. 
				strRes.LoadString(IDS_STRING441);
				break;
			}
		}
		break;
	default:
		// No help topic is associated with this item. 
		strRes.LoadString(IDS_STRING441);
	}
	myPopup.pszText = strRes;
	return HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}

BOOL CKPSolution_SpreadPg3::OnHelpInfo(HELPINFO* pHelpInfo) 
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
	case IDC_RESET:
		strRes.LoadString(IDS_STRING465);
		break;
	case IDC_MSHFLEXGRID1:
		{
			if (!IsContextHelp())
			{
				if (!(m_ctrlGrid.GetRowIsVisible(m_ctrlGrid.GetRow()) && m_ctrlGrid.GetColIsVisible(m_ctrlGrid.GetCol())))
				{
					::MessageBeep((UINT)-1);
					return TRUE;
				}

				// modify placement
				CDC* pDC = m_ctrlGrid.GetDC();
				int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
				int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

				long nLeft = ::MulDiv(m_ctrlGrid.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
				long nTop  = ::MulDiv(m_ctrlGrid.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

				CRect rc;
				m_ctrlGrid.GetWindowRect(rc);

				myPopup.pt.x = rc.left + nLeft;
				myPopup.pt.y = rc.top + nTop + 10;
			}
			switch (IsContextHelp() ? m_ctrlGrid.GetMouseRow() : m_ctrlGrid.GetRow())
			{
			case 0:
				strRes.LoadString(IDS_STRING524);
				break;
			case 1:
				strRes.LoadString(IDS_STRING525);
				break;
			case 2:
				strRes.LoadString(IDS_STRING526);
				break;
			case 3:
				strRes.LoadString(IDS_STRING527);
				break;
			case 4:
				strRes.LoadString(IDS_STRING528);
				break;
			case 5:
				strRes.LoadString(IDS_STRING529);
				break;
			case 6:
				strRes.LoadString(IDS_STRING530);
				break;
			default:
				// No help topic is associated with this item. 
				strRes.LoadString(IDS_STRING441);
				break;
			}
		}
		break;
	default:
		// No help topic is associated with this item. 
		strRes.LoadString(IDS_STRING441);
	}
	myPopup.pszText = strRes;
	return HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}

BOOL CKPSolution_SpreadPg4::OnHelpInfo(HELPINFO* pHelpInfo) 
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
	case IDC_ISOTOPE_GRID:
		{
			if (!IsContextHelp())
			{
				if (!(m_ctrlGrid.GetRowIsVisible(m_ctrlGrid.GetRow()) && m_ctrlGrid.GetColIsVisible(m_ctrlGrid.GetCol())))
				{
					::MessageBeep((UINT)-1);
					return TRUE;
				}

				// modify placement
				CDC* pDC = m_ctrlGrid.GetDC();
				int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
				int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

				long nLeft = ::MulDiv(m_ctrlGrid.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
				long nTop  = ::MulDiv(m_ctrlGrid.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

				CRect rc;
				m_ctrlGrid.GetWindowRect(rc);

				myPopup.pt.x = rc.left + nLeft;
				myPopup.pt.y = rc.top + nTop + 10;
			}
			switch (IsContextHelp() ? m_ctrlGrid.GetMouseCol() : m_ctrlGrid.GetCol())
			{
			case 1:
				strRes.LoadString(IDS_STRING543);
				break;
			case 2:
				strRes.LoadString(IDS_STRING544);
				break;
			case 3:
				strRes.LoadString(IDS_STRING545);
				break;
			default:
				// No help topic is associated with this item. 
				strRes.LoadString(IDS_STRING441);
				break;
			}
		}
		break;
	default:
		return baseKPSolution_SpreadPg4::OnHelpInfo(pHelpInfo);
	}
	myPopup.pszText = strRes;
	return HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}


BOOL CKPSolution_SpreadPg3::OnInitDialog() 
{
	baseKPSolution_SpreadPg3::OnInitDialog();
	
	// Add extra initialization here
	// set layout
	CreateRoot(VERTICAL, 5, 6) 
		<< (pane(HORIZONTAL, ABSOLUTE_VERT)
			<< item(IDC_MSHFLEXGRID1, GREEDY)
			<< item(IDC_RESET, NORESIZE)
			)
		<< (paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, GREEDY, nDefaultBorder, 10, 10)
			<< item(IDC_DESCRIPTION, GREEDY)
			)
	;
	UpdateLayout();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKPSolution_SpreadPg3::OnSize(UINT nType, int cx, int cy) 
{
	baseKPSolution_SpreadPg3::OnSize(nType, cx, cy);
	
	// resize the columns within IDC_MSHFLEXGRID1
	if (m_ctrlGrid.GetSafeHwnd())
	{
		long nCol0 = m_ctrlGrid.GetColWidth(0, 0);

		CRect rect;
		CDC* pDC = GetDC();
		int nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
		m_ctrlGrid.GetClientRect(&rect);
		m_ctrlGrid.SetColWidth(1, 0, MulDiv(rect.right, TWIPS_PER_INCH, nLogX) - nCol0);
	}	
}

void CKPSolution_SpreadPg1::OnBFreeze() 
{
	// mimick UpdateData(TRUE) with just the freeze elements checkbox
	CDataExchange dx(this, TRUE);

	bool bOk = false;
	try
	{
		DDX_Check(&dx, IDC_B_FREEZE, m_bFreeze);
		bOk = true;
	}
	catch (CUserException* e)
	{
		ASSERT(!bOk);
		ASSERT(FALSE);
		e->ReportError(MB_OK);
		e->Delete();
	}
	if (bOk)
	{
		// save state
		long nRow       = m_ctrlGrid.GetRow();
		long nCol       = m_ctrlGrid.GetCol();
		long nRowSel    = m_ctrlGrid.GetRowSel();
		long nColSel    = m_ctrlGrid.GetColSel();
		long nFillStyle = m_ctrlGrid.GetFillStyle();
		BOOL bRedraw    = m_ctrlGrid.GetRedraw();

		// avoid flashing
		m_ctrlGrid.SetRedraw(FALSE);

		COLORREF clrCellBackColor;
		if (m_bFreeze)
		{
			m_ctrlGrid.SetFixedRows(1);

			m_ctrlGrid.SetRow(0);
			m_ctrlGrid.SetCol(0);
			clrCellBackColor = m_ctrlGrid.GetCellBackColor();
		}
		else
		{
			m_ctrlGrid.SetFixedRows(0);

			m_ctrlGrid.SetRow(2);
			m_ctrlGrid.SetCol(2);
			clrCellBackColor = m_ctrlGrid.GetCellBackColor();
		}

		m_ctrlGrid.SetFillStyle(flexFillRepeat);

		m_ctrlGrid.SetRow(0);
		m_ctrlGrid.SetCol(m_ctrlGrid.GetFixedCols());
		// m_ctrlGrid.SetRowSel(0);
		m_ctrlGrid.SetColSel(m_ctrlGrid.GetCols(0) - 1);
		m_ctrlGrid.SetCellBackColor(clrCellBackColor);

		// restore state
		m_ctrlGrid.SetRow(nRow);
		m_ctrlGrid.SetCol(nCol);
		m_ctrlGrid.SetRowSel(nRowSel);
		m_ctrlGrid.SetColSel(nColSel);
		m_ctrlGrid.SetFillStyle(nFillStyle);	
		if (bRedraw) m_ctrlGrid.SetRedraw(TRUE);

	}
}

void CKPSolution_SpreadPg1::OnDoubleclickedBFreeze() 
{
	OnBFreeze();	
}

void CKPSolution_SpreadPg1::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// Add your control notification handler code here
	if (m_bIgnoreItemchanged)
	{
		*pResult = 0;
		return;
	}


	if ((pNMListView->uNewState ^ pNMListView->uOldState) & LVIS_STATEIMAGEMASK)
	{
		if (!(
			(pNMListView->uNewState == INDEXTOSTATEIMAGEMASK(BST_INDETERMINATE + 1))
			||
			(pNMListView->uOldState == INDEXTOSTATEIMAGEMASK(BST_INDETERMINATE + 1))
			))
		{
			UpdateElementList(pNMListView->iItem);
		}
		if (pNMListView->uOldState == INDEXTOSTATEIMAGEMASK(BST_CHECKED + 1))
		{
			if (pNMListView->uNewState == INDEXTOSTATEIMAGEMASK(BST_UNCHECKED + 1))
			{
				CString str = m_ctrlCheckList.GetItemText(pNMListView->iItem, 0);
				CString strSub = m_ctrlCheckList.GetItemText(pNMListView->iItem, 1);

				bool bIsIsotope = false;
				for (int l = 0; l < count_iso_defaults; ++l)
				{
					if (str.Compare(iso_defaults[l].name) == 0)
					{
						bIsIsotope = true;
						break;
					}
				}
				for (long nCol = 1; nCol < m_ctrlGrid.GetCols(0);)
				{
					if (m_ctrlGrid.GetTextMatrix(0, nCol).Compare(str) == 0/*NEW{{*/|| (!strSub.IsEmpty() && m_ctrlGrid.GetTextMatrix(0, nCol).Compare(strSub) == 0)/*}}NEW*/)
					{
						if (bIsIsotope && m_ctrlGrid.GetTextMatrix(0, nCol + 1).Compare("uncertainty") == 0)
						{
							m_ctrlGrid.SetColPosition(nCol + 1, 0, m_ctrlGrid.GetCols(0) - 1);
						}
						else
						{
							bIsIsotope = false;
						}

						m_ctrlGrid.SetColPosition(nCol, 0, m_ctrlGrid.GetCols(0) - 1);

						// save state
						long nRow       = m_ctrlGrid.GetRow();
						long nCol       = m_ctrlGrid.GetCol();
						long nRowSel    = m_ctrlGrid.GetRowSel();
						long nColSel    = m_ctrlGrid.GetColSel();
						long nFillStyle = m_ctrlGrid.GetFillStyle();
						BOOL bRedraw    = m_ctrlGrid.GetRedraw();

						// avoid flashing
						m_ctrlGrid.SetRedraw(FALSE);

						m_ctrlGrid.SetFillStyle(flexFillRepeat);
						m_ctrlGrid.SetRow(0);

						if (bIsIsotope)
						{
							m_ctrlGrid.SetCol(m_ctrlGrid.GetCols(0) - 2);
							m_ctrlGrid.SetColSel(m_ctrlGrid.GetCols(0) - 1);
						}
						else
						{
							m_ctrlGrid.SetCol(m_ctrlGrid.GetCols(0) - 1);
						}
						m_ctrlGrid.SetRowSel(m_ctrlGrid.GetRows() - 1);
						m_ctrlGrid.SetText(_T(""));

						// restore state
						m_ctrlGrid.SetRow(nRow);
						m_ctrlGrid.SetCol(nCol);
						m_ctrlGrid.SetRowSel(nRowSel);
						m_ctrlGrid.SetColSel(nColSel);
						m_ctrlGrid.SetFillStyle(nFillStyle);	
						if (bRedraw) m_ctrlGrid.SetRedraw(TRUE);
					}
					else
					{
						nCol++;
					}
				}
			}
		}
		if (pNMListView->uOldState == INDEXTOSTATEIMAGEMASK(BST_UNCHECKED + 1))
		{
			if (pNMListView->uNewState == INDEXTOSTATEIMAGEMASK(BST_CHECKED + 1))
			{
				CString str = m_ctrlCheckList.GetItemText(pNMListView->iItem, 0);

				bool bIsIsotope = false;
				for (int l = 0; l < count_iso_defaults; ++l)
				{
					if (str.Compare(iso_defaults[l].name) == 0)
					{
						bIsIsotope = true;
						break;
					}
				}

				bool bFoundSpace = false;
				for (long nCol = 1; nCol < m_ctrlGrid.GetCols(0); ++nCol)
				{
					if (m_ctrlGrid.GetTextMatrix(0, nCol).IsEmpty())
					{
						bFoundSpace = true;
						m_ctrlGrid.SetTextMatrix(0, nCol, str);
						if (bIsIsotope)
						{
							// insert col
							m_ctrlGrid.SetCols(0, m_ctrlGrid.GetCols(0) + 1);
							m_ctrlGrid.SetColPosition(m_ctrlGrid.GetCols(0) - 1, 0, nCol + 1);
							m_ctrlGrid.SetTextMatrix(0, nCol + 1, _T("uncertainty"));
						}
						break;
					}
				}
				if (!bFoundSpace)
				{
					m_ctrlGrid.SetCols(0, m_ctrlGrid.GetCols(0) + 5);
					m_ctrlGrid.SetTextMatrix(0, nCol, str);
					if (bIsIsotope)
					{
						// insert col
						m_ctrlGrid.SetCols(0, m_ctrlGrid.GetCols(0) + 1);
						m_ctrlGrid.SetColPosition(m_ctrlGrid.GetCols(0) - 1, 0, nCol + 1);
						m_ctrlGrid.SetTextMatrix(0, nCol + 1, _T("uncertainty"));
					}
				}
			}
		}


	}	
	*pResult = 0;
}

void CKPSolution_SpreadPg1::UpdateElementList(int nItem) 
{
	if (nItem == -1) return;

	CString strSubItem = m_ctrlCheckList.GetItemText(nItem, 1);
	if (strSubItem.IsEmpty())
	{
		// this is a non-element heading
		return;
	}

	LVFINDINFO info = {0};

	switch (m_ctrlCheckList.GetCheckBox(nItem))
	{
	case BST_UNCHECKED:

		// check if redox state
		if (strSubItem.Find(_T("("), 0) != -1)
		{
			// this is a redox state of an element
			CString strTotalElem = strSubItem.SpanExcluding(_T("("));

			// Find total element state (nTotalElem)
			info.flags = LVFI_STRING;
			info.psz   = (LPCTSTR)strTotalElem;
			int nTotalElem = m_ctrlCheckList.FindItem(&info, -1);

			if (nTotalElem != -1)
			{
				// std::set<CString>::const_iterator iter = m_setSubItems.find(strTotalElem);
				std::map<CString, CString>::const_iterator mapIter = m_mapSubItem2Item.find(strTotalElem);
				//if (iter != m_setSubItems.end())
				if (mapIter != m_mapSubItem2Item.end())
				{
					CString strRedox;
					// for (iter++; iter != m_setSubItems.end(); ++iter)
					for (++mapIter; mapIter != m_mapSubItem2Item.end(); ++mapIter)
					{
						//CString strTemp = (*iter);
						CString strTemp = mapIter->first;
						//if ((*iter).Find(strTotalElem + _T("("), 0) != 0)
						if (mapIter->first.Find(strTotalElem + _T("("), 0) != 0)
						{
							m_ctrlCheckList.SetCheckBox(nTotalElem, BST_UNCHECKED);
							break;
						}
						//strRedox = m_mapSubItem2Item[(*iter)];
						strRedox = mapIter->second;
						info.flags = LVFI_STRING | LVFI_WRAP;
						info.psz = (LPCTSTR)strRedox;
						int nFind = m_ctrlCheckList.FindItem(&info, nItem);
						if (nFind != -1 && m_ctrlCheckList.GetCheckBox(nFind) == BST_CHECKED)
						{
							break;
						}
					}
				}
			}
		}
		else
		{
			// this is the total element which has redox states
			//{{
			// enable redox states

			// std::set<CString>::const_iterator iter = m_setSubItems.find(strSubItem);
			std::map<CString, CString>::const_iterator mapIter = m_mapSubItem2Item.find(strSubItem);
			//if (iter != m_setSubItems.end())
			if (mapIter != m_mapSubItem2Item.end())
			{
				CString strRedox;
				// for (++iter; iter != m_setSubItems.end(); ++iter)
				for (++mapIter; mapIter != m_mapSubItem2Item.end(); ++mapIter)
				{
					//CString strTemp = (*iter);
					CString strTemp = mapIter->first;
					if (mapIter->first.Find(strSubItem + _T("("), 0) != 0)
						break;
					//strRedox = m_mapSubItem2Item[(*iter)];
					strRedox = mapIter->second;
					info.flags = LVFI_STRING | LVFI_WRAP;
					info.psz = (LPCTSTR)strRedox;
					int nFind = m_ctrlCheckList.FindItem(&info, nItem);
					if (nFind != -1)
					{
						m_ctrlCheckList.SetCheckBox(nFind, BST_UNCHECKED);
						nItem = nFind;
					}
				}
			}
			//}}

// COMMENT: {5/30/2001 2:43:43 PM}			// Note: This assumes that col 1 is sorted
// COMMENT: {5/30/2001 2:43:43 PM}			CString strRedox;
// COMMENT: {5/30/2001 2:43:43 PM}			for (int i = nItem + 1; ; ++i)
// COMMENT: {5/30/2001 2:43:43 PM}			{
// COMMENT: {5/30/2001 2:43:43 PM}				strRedox = m_ctrlCheckList.GetItemText(i, 1);
// COMMENT: {5/30/2001 2:43:43 PM}				if (strRedox.Find(str + _T("("), 0) != 0)
// COMMENT: {5/30/2001 2:43:43 PM}					break;
// COMMENT: {5/30/2001 2:43:43 PM}				m_ctrlCheckList.SetCheckBox(i, BST_UNCHECKED);
// COMMENT: {5/30/2001 2:43:43 PM}			}
		}
		break;

	case BST_CHECKED:

		// check if redox state
		if (strSubItem.Find(_T("("), 0) != -1)
		{
			// disable total element
			CString strElem = strSubItem.SpanExcluding(_T("("));

			info.flags = LVFI_STRING;
			info.psz = strElem.GetBuffer(strElem.GetLength());
			nItem = m_ctrlCheckList.FindItem(&info);

			if (nItem != -1)
			{
				m_ctrlCheckList.SetCheckBox(nItem, BST_INDETERMINATE);
			}
		}
		else
		{
			// disable redox states
			//std::set<CString>::const_iterator iter = m_setSubItems.find(strSubItem);
			std::map<CString, CString>::const_iterator mapIter = m_mapSubItem2Item.find(strSubItem);
			// if (iter != m_setSubItems.end())
			if (mapIter != m_mapSubItem2Item.end())
			{
				CString strRedox;
				for (mapIter++; mapIter != m_mapSubItem2Item.end(); ++mapIter)
				{
					CString strTemp = mapIter->first;
					if (mapIter->first.Find(strSubItem + _T("("), 0) != 0)
						break;
					// strRedox = m_mapSubItem2Item[(*iter)];
					strRedox = mapIter->second;
					info.flags = LVFI_STRING | LVFI_WRAP;
					info.psz = (LPCTSTR)strRedox;
					int nFind = m_ctrlCheckList.FindItem(&info, nItem);
					if (nFind != -1)
					{
						m_ctrlCheckList.SetCheckBox(nFind, BST_INDETERMINATE);
						nItem = nFind;
					}
				}

			}
		}
		break;
	}
}

BOOL CKPSolution_SpreadPg1::OnInitDialog() 
{
	baseKPSolution_SpreadPg1::OnInitDialog();
	
	// Add extra initialization here

	// set layout
	CreateRoot(VERTICAL, 5, 6) 
		<< item(IDC_S_DEFINED, NORESIZE|ALIGN_VCENTER)
		<< item(IDC_CL_TOTALS, RELATIVE_VERT, 0, 33)
		<< (pane(HORIZONTAL, ABSOLUTE_VERT)
			<< itemGrowing(HORIZONTAL)
			<< item(IDC_CHK_USE_INVERSE, NORESIZE|ALIGN_VCENTER)
			)
		<< item(IDC_MSHFG_TOTALS, RELATIVE_VERT, 0, 67)
		<< (pane(HORIZONTAL, ABSOLUTE_VERT)
			<< itemGrowing(HORIZONTAL)
			<< item(IDC_B_FREEZE, NORESIZE|ALIGN_VCENTER)
			)
		<< (paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, ABSOLUTE_VERT, nDefaultBorder, 10, 10)
			<< item(IDC_DESCRIPTION, GREEDY)
			)
	;
	UpdateLayout();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CKPSolution_SpreadPg2::OnInitDialog() 
{
	baseKPSolution_SpreadPg2::OnInitDialog();
	
	// Add extra initialization here

	// set layout
	CreateRoot(VERTICAL, 5, 6) 
		<< item(IDC_MSHFG_TOTALS, GREEDY)
		//{{
		<< (pane(HORIZONTAL, ABSOLUTE_VERT)
			<< itemGrowing(HORIZONTAL)
			<< item(IDC_S_CHARGE, NORESIZE|ALIGN_VCENTER)
			<< item(IDC_CBO_CHARGE, NORESIZE|ALIGN_VCENTER)
			)
		//}}
		<< (paneCtrl(IDC_S_DESC_INPUT, HORIZONTAL, GREEDY, nDefaultBorder, 10, 10)
			<< item(IDC_DESCRIPTION, GREEDY)
			)
	;
	UpdateLayout();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKPSolution_SpreadPg1::OnChkUseInverse() 
{
	// Add your control notification handler code here
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHK_USE_INVERSE);
	if (pBtn->GetCheck() == BST_CHECKED)
	{
		// create set of current elements
		CString str;
		std::set<CString> setPossibleIsotopes;
		for (long nCol = 1; nCol < m_ctrlGrid.GetCols(0); ++nCol)
		{
			str = m_ctrlGrid.GetTextMatrix(0, nCol);
			if (!str.IsEmpty() && ::_istdigit(str[0]))
			{
				// save possible isotope heading
				setPossibleIsotopes.insert(str);
			}
		}
		for (int l = 0; l < count_iso_defaults; ++l)
		{
			int nItem = m_ctrlCheckList.InsertItem(0, iso_defaults[l].name);
			if (nItem != -1)
			{
				// set check if already in heading
				std::set<CString>::const_iterator cIter;
				cIter = setPossibleIsotopes.find(iso_defaults[l].name);
				if (cIter != setPossibleIsotopes.end())
				{
					m_bIgnoreItemchanged = true;
					m_ctrlCheckList.SetCheckBox(nItem, BST_CHECKED);
					m_bIgnoreItemchanged = false;
				}
			}
		}
		ASSERT(((COCKSSolution_Spread*)GetParent())->GetPageCount() == 3);
		ASSERT(((COCKSSolution_Spread*)GetParent())->m_pPage4 == NULL);
		((COCKSSolution_Spread*)GetParent())->m_pPage4 = new CKPSolution_SpreadPg4;
		((COCKSSolution_Spread*)GetParent())->AddPage(((COCKSSolution_Spread*)GetParent())->m_pPage4);
	}
	else
	{
		LVFINDINFO info = {0};
		info.flags = LVFI_STRING;
		for (int l = 0; l < count_iso_defaults; ++l)
		{
			info.psz = iso_defaults[l].name;
			int nFind = m_ctrlCheckList.FindItem(&info, -1);
			ASSERT(nFind != -1);
			if (nFind != -1)
			{
				m_ctrlCheckList.DeleteItem(nFind);
			}
		}
		ASSERT(((COCKSSolution_Spread*)GetParent())->GetPageCount() == 4);
		((COCKSSolution_Spread*)GetParent())->RemovePage(((COCKSSolution_Spread*)GetParent())->m_pPage4);
		ASSERT(((COCKSSolution_Spread*)GetParent())->m_pPage4 != NULL);
		delete ((COCKSSolution_Spread*)GetParent())->m_pPage4;
		((COCKSSolution_Spread*)GetParent())->m_pPage4 = NULL;
	}
	
}

BEGIN_EVENTSINK_MAP(CKPSolution_SpreadPg1, baseKPSolution_SpreadPg1)
    //{{AFX_EVENTSINK_MAP(CKPSolution_SpreadPg1)
	ON_EVENT(CKPSolution_SpreadPg1, IDC_MSHFG_TOTALS, -602 /* KeyDown */, OnKeyDownGrid, VTS_PI2 VTS_I2)
	ON_EVENT(CKPSolution_SpreadPg1, IDC_MSHFG_TOTALS, 71 /* EnterCell */, OnEnterCellMshfgTotals, VTS_NONE)
	ON_EVENT(CKPSolution_SpreadPg1, IDC_MSHFG_TOTALS, -601 /* DblClick */, OnDblClickMshfgTotals, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CKPSolution_SpreadPg1::OnKeyDownGrid(short FAR* KeyCode, short Shift) 
{
	// Add your control notification handler code here
	UNUSED(Shift);

	switch (*KeyCode)
	{
	case VK_DELETE:
		m_ctrlGrid.ClearContents();	
		break;
	}	
}

bool CKPSolution_SpreadPg1::IsMoreThanOneHeader(CString strHeader, long nCol)
{
	bool bMoreThanOneHeader = false;
	std::map<CString, CString>::const_iterator mapIter = m_mapItem2SubItem.find(strHeader);
	CString strCell;
	for (long iCol = 1; iCol < m_ctrlGrid.GetCols(0); ++iCol)
	{
		if (iCol == nCol) continue;
		strCell = m_ctrlGrid.GetTextMatrix(0, iCol);
		strCell.Replace(_T("(+"), _T("("));
		if (strCell.Compare(strHeader) == 0)
		{
			bMoreThanOneHeader = true;
			break;
		}
		if (mapIter != m_mapItem2SubItem.end() && (strCell.Compare(mapIter->first) == 0 || strCell.Compare(mapIter->second) == 0))
		{
			bMoreThanOneHeader = true;
			break;
		}
	}
	return bMoreThanOneHeader;
}

void CKPSolution_SpreadPg1::OnSetfocusClTotals(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNREFERENCED_PARAMETER(pNMHDR);

	CString strRes;
	strRes.LoadString(IDS_STRING540);
	m_ctrlDescription.SetWindowText(strRes);

	// set default selection
	// this gives user feedback when the list recieves the focus
	if (m_ctrlCheckList.GetFirstSelectedItemPosition() == NULL)
	{
		m_ctrlCheckList.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}
	*pResult = 0;
}

void CKPSolution_SpreadPg1::OnSetfocusBFreeze()
{
	CString strRes;
	strRes.LoadString(IDS_STRING541);
	m_ctrlDescription.SetWindowText(strRes);
}

void CKPSolution_SpreadPg1::OnSetfocusBUseInverse()
{
	CString strRes;
	strRes.LoadString(IDS_STRING539);
	m_ctrlDescription.SetWindowText(strRes);
}

BOOL CKPSolution_SpreadPg1::OnHelpInfo(HELPINFO* pHelpInfo) 
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
	case IDC_CHK_USE_INVERSE:
		strRes.LoadString(IDS_STRING539);
		break;
	case IDC_S_DEFINED:
		strRes.LoadString(IDS_STRING540);
		break;
	case IDC_CL_TOTALS:
		{
			int nItem = -1;
			if (IsContextHelp())
			{
				LVHITTESTINFO info = {0};
				info.pt = pHelpInfo->MousePos;
				m_ctrlCheckList.ScreenToClient(&info.pt);
				nItem = m_ctrlCheckList.SubItemHitTest(&info);
			}
			else
			{
				POSITION pos = m_ctrlCheckList.GetFirstSelectedItemPosition();
				if (pos != NULL)
				{
					nItem = m_ctrlCheckList.GetNextSelectedItem(pos);
					// reset placement
					CRect rc;
					m_ctrlCheckList.GetWindowRect(&rc);
					myPopup.pt = rc.TopLeft();
					m_ctrlCheckList.GetItemRect(nItem, &rc, LVIR_LABEL);
					myPopup.pt.x += rc.CenterPoint().x;
					myPopup.pt.y += rc.CenterPoint().y + 10;					
				}
			}
			if (nItem != -1)
			{
				CString strHead = m_ctrlCheckList.GetItemText(nItem, 0);
				std::map<CString, UINT>::const_iterator mapIter;
				mapIter = m_mapExtraHeadings.find(strHead);
				if (mapIter != m_mapExtraHeadings.end())
				{
					strRes.LoadString(mapIter->second);
				}
				else
				{
					strRes.LoadString(IDS_STRING540);
				}
			}
			else
			{
				strRes.LoadString(IDS_STRING540);
			}
		}
		break;
	case IDC_B_FREEZE:
		strRes.LoadString(IDS_STRING541);
		break;
	case IDC_MSHFG_TOTALS:
		{
			long nCol = IsContextHelp() ? m_ctrlGrid.GetMouseCol() : m_ctrlGrid.GetCol();

			CString strHead = m_ctrlGrid.GetTextMatrix(0, nCol);
			std::map<CString, UINT>::const_iterator mapIter;
			mapIter = m_mapExtraHeadings.find(strHead);
			if (mapIter != m_mapExtraHeadings.end())
			{
				strRes.LoadString(mapIter->second);
				if (!IsContextHelp())
				{
					if (!(m_ctrlGrid.GetRowIsVisible(m_ctrlGrid.GetRow()) && m_ctrlGrid.GetColIsVisible(m_ctrlGrid.GetCol())))
					{
						::MessageBeep((UINT)-1);
						return TRUE;
					}

					// modify placement
					CDC* pDC = m_ctrlGrid.GetDC();
					int m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
					int m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);

					long nLeft = ::MulDiv(m_ctrlGrid.GetCellLeft(), m_nLogX, TWIPS_PER_INCH);
					long nTop  = ::MulDiv(m_ctrlGrid.GetCellTop(), m_nLogY, TWIPS_PER_INCH);

					CRect rc;
					m_ctrlGrid.GetWindowRect(rc);

					myPopup.pt.x = rc.left + nLeft;
					myPopup.pt.y = rc.top + nTop + 10;
				}
			}
			else
			{
				// No help topic is associated with this item. 
				strRes.LoadString(IDS_STRING441);
			}
		}
		break;
	default:
		// No help topic is associated with this item. 
		strRes.LoadString(IDS_STRING441);
	}
	myPopup.pszText = strRes;
	return HtmlHelp(NULL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&myPopup) != NULL;
}



BEGIN_EVENTSINK_MAP(CKPSolution_SpreadPg2, baseKPSolution_SpreadPg2)
    //{{AFX_EVENTSINK_MAP(CKPSolution_SpreadPg2)
	ON_EVENT(CKPSolution_SpreadPg2, IDC_MSHFG_TOTALS, 71 /* EnterCell */, OnEnterCellMshfgTotals, VTS_NONE)
	ON_EVENT(CKPSolution_SpreadPg2, IDC_MSHFG_TOTALS, -602 /* KeyDown */, OnKeyDownMshfgTotals, VTS_PI2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CKPSolution_SpreadPg2::OnKeyDownMshfgTotals(short FAR* KeyCode, short Shift) 
{
	// Add your control notification handler code here
	UNUSED(Shift);

	switch (*KeyCode)
	{
	case VK_DELETE:
		m_ctrlGrid.SetFillStyle(flexFillRepeat);
		m_ctrlGrid.SetText(_T(""));
		m_ctrlGrid.SetFillStyle(flexFillSingle);
		break;
	}	
	
}

void CKPSolution_SpreadPg2::OnEnterCellMshfgTotals()
{
	CString strRes;
	CString strResEx;
	switch (m_ctrlGrid.GetCol())
	{
	case 1: // Units
		strRes.LoadString(IDS_ELEM_197);
		break;
	case 2: // As / GFW
		strRes.LoadString(IDS_ELEM_200);
		strResEx.LoadString(IDS_ELEM_201);
		strRes += strResEx;
		break;
	case 3: // Phase
		strRes.LoadString(IDS_ELEM_198);
		break;
	case 4: // S.I.
		strRes.LoadString(IDS_ELEM_199);
		break;
	case 5: // Redox
		strRes.LoadString(IDS_ELEM_202);
		strResEx.LoadString(IDS_ELEM_203);
		strRes += strResEx;
		break;
	}
	m_ctrlDescription.SetWindowText(strRes);
}

void CKPSolution_SpreadPg2::OnSetfocusCboCharge() 
{
	CString strRes;
	strRes.LoadString(IDS_SOL_205);
	m_ctrlDescription.SetWindowText(strRes);
}

LRESULT CKPSolution_SpreadPg2::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	OnEnterCellMshfgTotals();
	return 0;
}

LRESULT CKPSolution_SpreadPg1::OnSetfocusGrid(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	OnEnterCellMshfgTotals();
	return 0;
}

void CKPSolution_SpreadPg1::OnEnterCellMshfgTotals() 
{
	CString strRes;
	CString strHead = m_ctrlGrid.GetTextMatrix(0, m_ctrlGrid.GetCol());
	std::map<CString, UINT>::const_iterator mapIter;
	mapIter = m_mapExtraHeadings.find(strHead);
	if (mapIter != m_mapExtraHeadings.end())
	{
		strRes.LoadString(mapIter->second);
	}
	else
	{
		// No help topic is associated with this item. 
		strRes.LoadString(IDS_STRING441);
	}
	m_ctrlDescription.SetWindowText(strRes);
}

void CKPSolution_SpreadPg1::OnDblClickMshfgTotals() 
{
	// Add your control notification handler code here
	if (!m_ctrlGrid.GetMouseRow())
	{
		// resize column to widest or to fit WWW
		long nCol = m_ctrlGrid.GetMouseCol();
		if (nCol == 0) return;

		CDC* pDC = m_ctrlGrid.GetDC();
		int nPixelsPerInchX = pDC->GetDeviceCaps(LOGPIXELSX);

		CSize size;
		long nWidest = pDC->GetTextExtent(_T("WWW"), 3).cx;
		for (long i = 0; i < m_ctrlGrid.GetRows(); ++i)
		{
			size = pDC->GetTextExtent(m_ctrlGrid.GetTextMatrix(i, nCol));
			if (size.cx > nWidest)
			{
				nWidest = size.cx;
			}
		}
		if (nWidest > 5)
		{
			m_ctrlGrid.SetColWidth(nCol, 0, ::MulDiv(nWidest, TWIPS_PER_INCH, nPixelsPerInchX));
		}
	}
	
}

LRESULT CKPSolution_SpreadPg1::OnDisplayErrors(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	::AfxMessageBox(this->m_strErrMsg, MB_ICONEXCLAMATION);
	return 0;
}