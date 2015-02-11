
// Test_M4ATrandcoderMFC.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CTest_M4ATrandcoderMFCApp:
// See Test_M4ATrandcoderMFC.cpp for the implementation of this class
//

class CTest_M4ATrandcoderMFCApp : public CWinApp
{
public:
	CTest_M4ATrandcoderMFCApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CTest_M4ATrandcoderMFCApp theApp;