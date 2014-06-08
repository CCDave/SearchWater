#ifndef _HTTP_DOCUMENT_PROXY_OBJECT_
#define _HTTP_DOCUMENT_PROXY_OBJECT_
#include "SpiderInterface.h"

#include "../../common/base/SingleObject.h"
#include "../../Common/base/ObjectInterface.h"

#include "Browser/Browser.h"

class CHttpDocumentProxy : 
			public IHttpDocumentProxy,
			public Singleton<CHttpDocumentProxy>,
			public _object
{
	//单例模式防止拷贝，最好在进程启动早些时候调用GetInstance实例化。
public:
	CHttpDocumentProxy(void);
	CHttpDocumentProxy(const CHttpDocumentProxy &);
	CHttpDocumentProxy& operator = (const CHttpDocumentProxy &);

public:
	virtual ~CHttpDocumentProxy(void);
	STDMETHOD(GetHttpDocument)(LPCWSTR lpszUrl, UINT uTimeOut = 5000 /*超时时间*/, IDispatch** ppDoc = NULL);
	STDMETHOD(SetHostWindow)(HWND hWnd);
	STDMETHOD(SetFinishEvent)(HANDLE hEvent);
	STDMETHOD(IsFinish)();
	STDMETHOD(SetCallBack)(LPVOID);
protected:
	STDMETHOD (init)();
	STDMETHOD (destory)();
	
private:
	HWND _parent_wnd_handle;
	CBrowser _browser;
	IDispatch** _doc;
	HANDLE _finish_handle;
	IBrowserCallBack* _call_back;
};
#endif