// Spider.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Browser/Browser.h"
CAppModule _Module;


CBrowser m_oBrowser;
// ��ȡ֩��Ķ���
HRESULT WINAPI CreateSpider(ULONG /*�ӿ�ID*/ Id, HWND hWnd, LPVOID* ppvObj)
{
	RECT rc = {};
    if (FALSE==m_oBrowser.Init(hWnd,rc))
        return FALSE;

    m_oBrowser.Visit(_T("http://cp.360.cn/jczq"));
    return NULL;
}