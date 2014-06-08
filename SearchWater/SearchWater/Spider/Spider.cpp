// Spider.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "SpiderWork.h"
IDAVSpiderInterface* pInterface = NULL;
CAppModule _Module;
HWND _hostWnd = NULL;

// 获取蜘蛛的对象
HRESULT WINAPI CreateSpider(ULONG /*接口ID*/ Id, HWND hWnd, LPVOID* ppvObj)
{
	HRESULT hRet = E_FAIL;
	_hostWnd = hWnd;
	if (!pInterface)
	{
		pInterface = new CSpiderWork(hWnd);
		pInterface->SetHostWindow(hWnd);
	}

	if (pInterface)
	{
		*ppvObj = pInterface;
	}

    return hRet;
}