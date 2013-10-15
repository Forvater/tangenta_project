
// TestComPort.h : главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CTestComPortApp:
// О реализации данного класса см. TestComPort.cpp
//

class CTestComPortApp : public CWinApp
{
public:
	CTestComPortApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CTestComPortApp theApp;