#include "StdAfx.h"
#include "HttpDocumentProxy.h"
#include "Browser/Browser.h"

CHttpDocumentProxy::CHttpDocumentProxy(void):_finish_handle(NULL),_doc(NULL),_parent_wnd_handle(NULL),_call_back(NULL)
{
	//init();
}

CHttpDocumentProxy::~CHttpDocumentProxy(void)
{
	destory();
}

HRESULT CHttpDocumentProxy::GetHttpDocument(LPCWSTR lpszUrl, UINT uTimeOut /*³¬Ê±Ê±¼ä*/, IDispatch** ppDoc)
{
	if (!lpszUrl || !ppDoc)
		return E_FAIL;
	_doc = ppDoc;
	return _browser.Visit(0, lpszUrl) ? S_OK : E_FAIL;
}



HRESULT CHttpDocumentProxy::SetHostWindow(HWND hWnd)
{
	_parent_wnd_handle = hWnd;
	init();
	return S_OK;
}

HRESULT CHttpDocumentProxy::SetFinishEvent(HANDLE hEvent)
{
	_finish_handle = hEvent;
	return S_OK;
}

HRESULT CHttpDocumentProxy::SetCallBack(LPVOID p)
{
	_call_back = (IBrowserCallBack*)p;
	return S_OK;
}

HRESULT CHttpDocumentProxy::IsFinish()
{	
	return S_OK;
}

HRESULT CHttpDocumentProxy::init()
{
	if (_parent_wnd_handle && !is_init_ok())
	{
		RECT rec = {};
		if (_browser.Init(_call_back, _parent_wnd_handle, rec))
		{
			init_done();
			return S_OK;
		}
	}	
		
	return E_FAIL;
}

HRESULT CHttpDocumentProxy::destory()
{
	return S_OK;
}