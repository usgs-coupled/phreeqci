// SRCDBPGPpg.cpp : Implementation of the CSRCDBPagerPropPage property page class.
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SRCDBPG.h"
#include "SRCDBPGPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CSRCDBPagerPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CSRCDBPagerPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CSRCDBPagerPropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CSRCDBPagerPropPage, "SRCDBPager.SRCDBPGPropPage.1",
	0xd48e3786, 0x6364, 0x4f71, 0x9c, 0x7b, 0x9, 0x4d, 0xb9, 0xcc, 0x73, 0x3a)


/////////////////////////////////////////////////////////////////////////////
// CSRCDBPagerPropPage::CSRCDBPagerPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CSRCDBPagerPropPage

BOOL CSRCDBPagerPropPage::CSRCDBPagerPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_SRCDBPG_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CSRCDBPagerPropPage::CSRCDBPagerPropPage - Constructor

CSRCDBPagerPropPage::CSRCDBPagerPropPage() :
	COlePropertyPage(IDD, IDS_SRCDBPG_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CSRCDBPagerPropPage)
	m_strCaption = _T("");
	m_bGotoFirstEnabled = FALSE;
	m_bGotoLastEnabled = FALSE;
	m_bGotoNewEnabled = FALSE;
	m_bGotoNextEnabled = FALSE;
	m_bGotoPrevEnabled = FALSE;
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CSRCDBPagerPropPage::DoDataExchange - Moves data between page and properties

void CSRCDBPagerPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CSRCDBPagerPropPage)
	DDP_Text(pDX, IDC_CAPTION, m_strCaption, _T("Caption") );
	DDX_Text(pDX, IDC_CAPTION, m_strCaption);
	DDP_Check(pDX, IDC_GOTO_FIRST_ENABLED, m_bGotoFirstEnabled, _T("GotoFirstEnabled") );
	DDX_Check(pDX, IDC_GOTO_FIRST_ENABLED, m_bGotoFirstEnabled);
	DDP_Check(pDX, IDC_GOTO_LAST_ENABLED, m_bGotoLastEnabled, _T("GotoLastEnabled") );
	DDX_Check(pDX, IDC_GOTO_LAST_ENABLED, m_bGotoLastEnabled);
	DDP_Check(pDX, IDC_GOTO_NEW_ENABLED, m_bGotoNewEnabled, _T("GotoNewEnabled") );
	DDX_Check(pDX, IDC_GOTO_NEW_ENABLED, m_bGotoNewEnabled);
	DDP_Check(pDX, IDC_GOTO_NEXT_ENABLED, m_bGotoNextEnabled, _T("GotoNextEnabled") );
	DDX_Check(pDX, IDC_GOTO_NEXT_ENABLED, m_bGotoNextEnabled);
	DDP_Check(pDX, IDC_GOTO_PREV_ENABLED, m_bGotoPrevEnabled, _T("GotoPrevEnabled") );
	DDX_Check(pDX, IDC_GOTO_PREV_ENABLED, m_bGotoPrevEnabled);
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CSRCDBPagerPropPage message handlers
