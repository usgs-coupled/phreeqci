// DisableUndo.cpp : implementation file
//

#include "stdafx.h"
#include "phreeqci2.h"
#include "SuspendUndo.h"


// CSuspendUndo

CSuspendUndo::CSuspendUndo(CRichEditCtrl &richEditCtrl)
: iTextDocument(0)
{
	IRichEditOle *iRichEditOle = richEditCtrl.GetIRichEditOle();
	if (iRichEditOle)
	{
		HRESULT hr = iRichEditOle->QueryInterface(__uuidof(ITextDocument), (void **)&this->iTextDocument);
		if(hr != S_OK)
		{
			this->iTextDocument = 0;
		}
		iRichEditOle->Release();
	}

	if (this->iTextDocument)
	{
		this->iTextDocument->Undo(tomSuspend, NULL);  //suspend undo
	}
}

CSuspendUndo::~CSuspendUndo()
{
	if(this->iTextDocument)
	{
		this->iTextDocument->Undo(tomResume, NULL);  //resume normal undo/redo
		this->iTextDocument->Release();
	}
}


// CSuspendUndo member functions
