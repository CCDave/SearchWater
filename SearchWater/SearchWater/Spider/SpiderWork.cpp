#include "StdAfx.h"
#include "SpiderWork.h"
#include "HttpDocumentProxy.h"
#include "ParseHomezjcq.h"

CSpiderWork::CSpiderWork(HWND hWnd) : _DocProxy(NULL)
								,_hFinishEvent(NULL)
								,_hHostWnd(hWnd)
							    
{
	init();
}

CSpiderWork::~CSpiderWork(void)
{
}

HRESULT CSpiderWork::BrowserCallBack(IDispatch* pObject, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandle)
{
	HRESULT hRet = E_FAIL;
	DocParseIte ite = _DocParse.find(_uMsg);
	IHttpDocumentParse* pItem = ite->second;
	if (pItem && pObject)
	{
		hRet = pItem->ParseHttpDocument((IHTMLDocument2 *)pObject, _lPvoid);
	}
	SetEvent(_hFinishEvent);
	return hRet;
}
HRESULT CSpiderWork::SpiderWork(UINT uMsg, LPCWSTR lpszUrl, LPVOID lPvoid, UINT uTimeOut)
{
	HRESULT hRet = E_FAIL;
	if (!uMsg || !lpszUrl || !lPvoid || !_hFinishEvent)
		return hRet;
	_uMsg = uMsg;
	_strUrl = lpszUrl;
	_lPvoid = lPvoid;
	_uTimeOut = uTimeOut;

	if (WaitForSingleObject(_hFinishEvent, 0) == WAIT_OBJECT_0)
	{
		ResetEvent(_hFinishEvent);
	}
	if (!_DocProxy)
		return hRet;
	_DocProxy->GetHttpDocument(_strUrl, _uTimeOut, (IDispatch **)&_pDoc);

	return hRet;
}

HRESULT CSpiderWork::init()
{
	if (!_hFinishEvent)
	{
		_hFinishEvent = CreateEvent(NULL, 0, 0, NULL);
	}
	if (!_DocProxy)
	{
		_DocProxy = CHttpDocumentProxy::GetInstance();
		_DocProxy->SetCallBack(this);
		_DocProxy->SetFinishEvent(_hFinishEvent);
		_DocProxy->SetHostWindow(_hHostWnd);
	}
	
	//此处根据ID创建全部对应的对象
	_DocParse.clear();
	_DocParse.insert(std::pair<UINT,IHttpDocumentParse*>(GET_HOME_PAGE_ZJCQ_DADA, (IHttpDocumentParse*)new CParseHomezjcq));

	return S_OK;
}

HRESULT CSpiderWork::destory()
{
	return S_OK;
}

int	CSpiderWork::Run()
{
	HRESULT hRet = E_FAIL;

	do 
	{
		
	} while (FALSE);

	return 0;
}

HRESULT CSpiderWork::SetHostWindow(HWND hWnd)
{
	_hHostWnd = hWnd;
	return S_OK;
}
