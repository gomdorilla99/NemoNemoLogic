
// NoNogram.h : NoNogram ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CNoNogramApp:
// �� Ŭ������ ������ ���ؼ��� NoNogram.cpp�� �����Ͻʽÿ�.
//

class CNoNogramApp : public CWinAppEx
{
public:
	CNoNogramApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CNoNogramApp theApp;