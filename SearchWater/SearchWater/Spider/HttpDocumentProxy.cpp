#include "StdAfx.h"
#include "HttpDocumentProxy.h"
#include "Browser/Browser.h"

CHttpDocumentProxy::CHttpDocumentProxy(void)
{
	init();
}

CHttpDocumentProxy::~CHttpDocumentProxy(void)
{
	destory();
}

HRESULT CHttpDocumentProxy::GetHttpDocument(LPCWSTR lpszUrl, UINT uTimeOut /*³¬Ê±Ê±¼ä*/, IDispatch** ppDoc)
{
	return S_OK;
}

HRESULT CHttpDocumentProxy::init()
{
	if (_parent_wnd_handle && SUCCEEDED(is_init_ok()))
	{
		RECT rec = {};
		_browser.Init(_parent_wnd_handle, rec);
		init_done();
		return S_OK;
	}	
		
	return E_FAIL;
}

HRESULT CHttpDocumentProxy::destory()
{
	return S_OK;
}