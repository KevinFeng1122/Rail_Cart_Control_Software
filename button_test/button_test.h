
// button_test.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Cbutton_testApp:
// �йش����ʵ�֣������ button_test.cpp
//

class Cbutton_testApp : public CWinAppEx
{
public:
	Cbutton_testApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Cbutton_testApp theApp;