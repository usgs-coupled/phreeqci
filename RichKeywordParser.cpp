// RichKeywordParser.cpp: implementation of the CRichKeywordParser class.
//
// $Id$
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phreeqci2.h"
#include "RichKeywordParser.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "RichLogicalLineParser.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRichKeywordParser::CRichKeywordParser(CRichLogicalLineParser& rRichLogicalLineParser)
: CKeywordParser(rRichLogicalLineParser),
  cr(rRichLogicalLineParser.m_crLine)
{
}

CRichKeywordParser::~CRichKeywordParser()
{

}

IMPLEMENT_DYNAMIC(CRichKeywordParser, CKeywordParser)
