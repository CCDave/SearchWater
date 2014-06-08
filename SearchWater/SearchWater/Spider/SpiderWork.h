#pragma once
#include "SpiderInterface.h"
#include "../../common/base/ObjectInterface.h"
#include "../../common/base/ThreadBase.h"
#include "BrowserCallback.h"

typedef std::map<UINT,IHttpDocumentParse*> DocumentParseMap;
typedef std::map<UINT,IHttpDocumentParse*>::iterator DocParseIte;
typedef std::pair<UINT,IHttpDocumentParse*> PairDocParse;

class CSpiderWork :
				public IBrowserCallBack,
				public IDAVSpiderInterface,
				public _object,
				public CThreadBase
				
{
public:
	CSpiderWork(HWND hWnd);
	virtual ~CSpiderWork(void);
	STDMETHOD(BrowserCallBack)(IDispatch* pObject, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandle);
	STDMETHOD(SpiderWork)(UINT uMsg, LPCWSTR lpszUrl, LPVOID lPvoid, UINT uTimeOut = 5000);
	STDMETHOD(SetHostWindow)(HWND hWnd);
	virtual HANDLE GetFinishHandle(){return _hFinishEvent;}
	
protected:
	STDMETHOD (init)();
	STDMETHOD (destory)();
	int	Run();
private:
	 IHttpDocumentProxy* _DocProxy;
	 DocumentParseMap _DocParse;

	 HANDLE _hFinishEvent;
	 HWND _hHostWnd;
	 
	 UINT _uMsg;
	 CString _strUrl;
	 LPVOID _lPvoid;
	 UINT _uTimeOut;
	 CComPtr<IHTMLDocument2> _pDoc;
};
