#pragma once

// CSuspendUndo command target
#include <Tom.h>    // ITextDocument Interface

class CSuspendUndo : public CObject
{
public:
	CSuspendUndo(CRichEditCtrl &richEditCtrl);
	virtual ~CSuspendUndo();

public:
	ITextDocument *iTextDocument;
};


