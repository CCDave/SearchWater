#ifndef _HTTP_DOCUMENT_PROXY_OBJECT_
#define _HTTP_DOCUMENT_PROXY_OBJECT_
#include "SpiderInterface.h"

#include "../../common/base/SingleObject.h"
#include "../../Common/base/ObjectInterface.h"

#include "Browser/Browser.h"
#include "CWindowContainer.h"

class CHttpDocumentProxy : 
			public IHttpDocumentProxy,
			public Singleton<CHttpDocumentProxy>,
			public _object
{
	//����ģʽ��ֹ����������ڽ���������Щʱ�����GetInstanceʵ������
private:
	CHttpDocumentProxy(void);
	CHttpDocumentProxy(const CHttpDocumentProxy &);
	CHttpDocumentProxy& operator = (const CHttpDocumentProxy &);

public:
	virtual ~CHttpDocumentProxy(void);
	STDMETHOD(GetHttpDocument)(LPCWSTR lpszUrl, UINT uTimeOut /*��ʱʱ��*/, IDispatch** ppDoc);
	
protected:
	STDMETHOD (init)();
	STDMETHOD (destory)();
	
private:
	HWND _parent_wnd_handle;
	CBrowser _browser;
};
#endif