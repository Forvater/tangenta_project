
// TestComPort.h : ������� ���� ��������� ��� ���������� PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�������� stdafx.h �� ��������� ����� ����� � PCH"
#endif

#include "resource.h"		// �������� �������


// CTestComPortApp:
// � ���������� ������� ������ ��. TestComPort.cpp
//

class CTestComPortApp : public CWinApp
{
public:
	CTestComPortApp();

// ���������������
public:
	virtual BOOL InitInstance();

// ����������

	DECLARE_MESSAGE_MAP()
};

extern CTestComPortApp theApp;