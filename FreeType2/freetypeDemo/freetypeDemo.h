
// freetypeDemo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CfreetypeDemoApp: 
// �йش����ʵ�֣������ freetypeDemo.cpp
//

class CfreetypeDemoApp : public CWinApp
{
public:
	CfreetypeDemoApp();

// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
// ʵ��

	DECLARE_MESSAGE_MAP()
	ULONG_PTR m_gdiplusToken;
	GdiplusStartupInput m_gdiplusStartupInput;
};

extern CfreetypeDemoApp theApp;