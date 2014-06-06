#ifndef _BROWSER_OBJECT_
#define _BROWSER_OBJECT_

#include <string>
#include <atlcom.h>
#include <atlstr.h>
#include <ExDispid.h>

#include "UIHanderlFace.h"

// define tstring.
#ifdef tstring
#undef tstring
#endif

#ifdef _UNICODE
#define tstring std::wstring
#else
#define tstring std::string
#endif

class CBrowser : public DWebBrowserEvents2
{
public: // IDispatch methods
	STDMETHOD(QueryInterface)( REFIID riid, void **ppvObject);
	STDMETHOD_(ULONG,AddRef)();
	STDMETHOD_(ULONG,Release)();
	STDMETHOD(GetTypeInfoCount)(UINT*) ;
	STDMETHOD(GetTypeInfo)(UINT, LCID, ITypeInfo**) ;
	STDMETHOD(GetIDsOfNames)(REFIID, LPOLESTR *rgszNames, UINT, LCID, DISPID *rgDispId) ;
	STDMETHOD(Invoke)(DISPID dispIdMember,REFIID riid,LCID lcid,WORD wFlags,DISPPARAMS FAR* pDispParams,VARIANT FAR* pVarResult,EXCEPINFO FAR* pExcepInfo,unsigned int FAR* puArgErr);  

public:
    CBrowser();
    ~CBrowser(void);

    BOOL Init(const HWND hParentWnd,RECT & rc);
    BOOL Visit(tstring strURL);

protected:
    HRESULT RegisterBrowserEventSink();
    HRESULT UnRegisterBrowserEventSink();

	HRESULT Report();
	void DocumentComplete( IDispatch *pDisp,VARIANT *URL);
    CAxWindow m_axWindow;
    CComPtr<IWebBrowser2> m_spWebBrowser2;
    CUIHanderlFace	* m_iUIFace;

    IConnectionPoint *m_pCP;
    DWORD m_dwEventCookie;
	tstring m_strCurrentUrl;
};

#endif