
#include <windows.h>
#include <strsafe.h>
#include "StdAfx.h"
#include "Browser.h"

CBrowser::CBrowser()
{
    m_pCP = NULL;
    m_dwEventCookie =0;
//	m_iUIFace = NULL;
}

CBrowser::~CBrowser(void)
{
	UnRegisterBrowserEventSink();
	if (m_iUIFace)
	{
		m_iUIFace->Release();

		// 不确定调用Reales()和手动delete效果是否一样。如果同时保留的话会运行出错。
// 		delete m_iUIFace;
// 		m_iUIFace = NULL;
	}
}

STDMETHODIMP CBrowser::QueryInterface( REFIID riid, void **ppvObject)
{
	HRESULT hr = E_NOINTERFACE;
	if (riid == __uuidof(IDispatch))
	{
		*ppvObject = (IDispatch*)this;
		AddRef();
		hr = S_OK;
	} 
	else if (riid == __uuidof(DWebBrowserEvents2))
	{
		*ppvObject = (DWebBrowserEvents2*)this;
		AddRef();
		hr = S_OK;            
	}

	return hr;
}
STDMETHODIMP_ (ULONG) CBrowser::AddRef(void)
{
	return 1;
}
STDMETHODIMP_ (ULONG) CBrowser::Release(void)
{
	return 1;
}
STDMETHODIMP CBrowser::GetTypeInfoCount(UINT*) 
{
	return E_NOTIMPL;
}
STDMETHODIMP CBrowser::GetTypeInfo(UINT, LCID, ITypeInfo**) 
{
	return E_NOTIMPL; 
}

STDMETHODIMP CBrowser::GetIDsOfNames(REFIID, LPOLESTR *rgszNames, UINT, LCID, DISPID *rgDispId) 
{ 
	return E_NOTIMPL;
}

STDMETHODIMP CBrowser::Invoke(DISPID dispIdMember,REFIID riid,LCID lcid,WORD wFlags,
	DISPPARAMS FAR* pDispParams,VARIANT FAR* pVarResult,
	EXCEPINFO FAR* pExcepInfo,unsigned int FAR* puArgErr)
{
	HRESULT hr = S_OK;
	
	if(dispIdMember == DISPID_DOCUMENTCOMPLETE) 
	{
		
		DocumentComplete(pDispParams->rgvarg[1].pdispVal,pDispParams->rgvarg[0].pvarVal); 
		return S_OK; 	
		//文档接手完成通过比对URL判断加载的页面
		// OnNavigateComplete 
		//::MessageBox(NULL,_T("DISPID_DOCUMENTCOMPLETE"),_T("文档加载完毕提示"),MB_OK);

// 		CComQIPtr<IWebBrowser2> pDisp1 = pDispParams->rgvarg[1].pdispVal;
// 		if (m_spWebBrowser2 && pDisp1==m_spWebBrowser2)
// 		{
// 			// 获取Document
// 			CComQIPtr<IDispatch> pDisp;
// 			hr = m_spWebBrowser2->get_Document(&pDisp);
// 			if (FAILED(hr))
// 				return S_FALSE;
// 
// 			// 获取IHTMLDocument2指针
// 			CComQIPtr<IHTMLDocument2> pDocument;
// 			hr = pDisp->QueryInterface(IID_IHTMLDocument2,(void**)&pDocument);
// 			if (FAILED(hr))
// 				return S_FALSE;
// 		}
	}

	return hr;
}

BOOL CBrowser::Init(const HWND hParentWnd,RECT & rc)
{
    m_axWindow.Create(hParentWnd, rc, 0,WS_CHILD |WS_VISIBLE);
    m_axWindow.CreateControl(OLESTR("shell.Explorer.2"));
    m_axWindow.QueryControl(&m_spWebBrowser2);  

	if(NULL==m_spWebBrowser2)
		return FALSE;

	//禁止javascript警告。
	m_spWebBrowser2->put_Silent(VARIANT_TRUE);

    m_iUIFace = new CUIHanderlFace;	
    if (m_iUIFace != NULL)
        m_iUIFace->AddRef();


	IOleObject *pIOleObj = NULL;

	if (m_iUIFace != NULL)
	{
		if (m_spWebBrowser2 != NULL)
			m_spWebBrowser2->QueryInterface(IID_IOleObject, (void**)&pIOleObj);

		if (pIOleObj != NULL)
		{
			IOleClientSite *oldClientSite = NULL;

			if (pIOleObj->GetClientSite(&oldClientSite) == S_OK)
			{
				m_iUIFace->SetDefaultClientSite(oldClientSite);
				oldClientSite->Release();
			}

			pIOleObj->SetClientSite(m_iUIFace);
		}
	}
	RegisterBrowserEventSink();

    return TRUE;
}

BOOL CBrowser::Visit(tstring strURL)
{

    CComVariant varUrl(strURL.c_str());
	m_strCurrentUrl = strURL;
    m_spWebBrowser2->Navigate2(&varUrl,0,0,0,0);

    return TRUE;
}

HRESULT CBrowser::RegisterBrowserEventSink()
{
    HRESULT     hr = S_FALSE;
    IConnectionPointContainer  *pCPCont = NULL;

    if (!m_spWebBrowser2)
        goto Cleanup;
    // Get the connection point container for the browser object.
    hr = m_spWebBrowser2->QueryInterface(IID_IConnectionPointContainer, (LPVOID *)&pCPCont);
    if (FAILED(hr))
    {
        hr = S_FALSE;
        goto Cleanup;
    }
    // Look for DWebBrowserEvents2 connection point.
    hr = pCPCont->FindConnectionPoint(DIID_DWebBrowserEvents2, &m_pCP);
    if (FAILED(hr))
    {
        m_pCP = NULL;
        goto Cleanup;
	}

    hr = m_pCP->Advise( static_cast<IUnknown*>(static_cast<DWebBrowserEvents2*>(this)), &(m_dwEventCookie));
    if (FAILED(hr))
        goto Cleanup;
Cleanup:
    if (pCPCont)
        pCPCont->Release();
    return hr;
}

HRESULT CBrowser::Report()
{
	// 			// 获取表单数据并提交
	// 			CComQIPtr<IHTMLElementCollection> spElementCollection;
	// 			hr = pDocument->get_forms(&spElementCollection);
	// 			if (FAILED(hr))
	// 				return S_FALSE;
	// 
	// 			long nFormCount =0;
	// 			hr = spElementCollection->get_length(&nFormCount);
	// 			if (FAILED(hr)||nFormCount<=0)
	// 				return S_FALSE;
	// 
	// 			for (long k=0;k<nFormCount;k++)
	// 			{
	// 				CComQIPtr<IDispatch> pDisp1;
	// 				hr = spElementCollection->item(CComVariant(k),CComVariant(),&pDisp1);
	// 				CComQIPtr<IHTMLFormElement> spFormElement =pDisp1;
	// 
	// 				long nElementCount =0;
	// 				hr = spFormElement->get_length(&nElementCount);
	// 				if (FAILED(hr))
	// 					return S_FALSE;
	// 
	// 				for (long c=0;c<nElementCount;c++)
	// 				{
	// 					CComDispatchDriver spInputElement1;
	// 					CComQIPtr<IHTMLElement> pHElement;
	// 					hr = spFormElement->item(CComVariant(c),CComVariant(),&spInputElement1);
	// 					if (FAILED(hr))
	// 						continue;
	// 
	// 					spInputElement1->QueryInterface(IID_IHTMLElement,(void**)&pHElement);
	// 
	// 					CComVariant vType;
	// 					CComVariant vName;
	// 					CComVariant	vValue;
	// 					if (pHElement==NULL)
	// 						continue;
	// 
	// 					hr = spInputElement1.GetPropertyByName(_T("type"),&vType);
	// 					if (FAILED(hr))
	// 						continue;
	// 					hr = spInputElement1.GetPropertyByName(_T("name"),&vName);
	// 					if (FAILED(hr))
	// 						continue;
	// 					hr = spInputElement1.GetPropertyByName(_T("value"),&vValue);
	// 					if (FAILED(hr))
	// 						continue;
	// 
	// 					LPCTSTR lpType = vType.bstrVal ? OLE2CT(vType.bstrVal) : _T("NULL");
	// 					LPCTSTR lpName = vType.bstrVal ? OLE2CT(vType.bstrVal) : _T("NULL");
	// 					LPCTSTR lpVal = vType.bstrVal ? OLE2CT(vType.bstrVal) : _T("NULL");
	// 
	// 					if ( _tcscmp(lpType,_T("submit"))==0 && _tcscmp(lpVal,_T("submit"))==0 )
	// 					{
	// 						//::MessageBox(NULL,lpType,_T("type="),MB_OK);
	// 						pHElement->click();
	// 					}
	// 				}
	// 			}

	// 			// 从body获得IHTMLElement2指针，用以访问滚动条
	// 			CComQIPtr<IHTMLElement> pBody;
	// 			hr = pDocument->get_body(&pBody);
	// 			if (FAILED(hr))
	// 				return S_FALSE;
	// 
	// 			CComQIPtr<IHTMLElement2> pElement;
	// 			hr = pBody->QueryInterface(IID_IHTMLElement2,(void**)&pElement);
	// 			if (FAILED(hr))
	// 				return S_FALSE;
	// 
	// 			//获得文档高度，不是可见区域的高度。
	// 			long scroll_height;
	// 			pElement->get_scrollHeight(&scroll_height);
	// 
	// 			//获得文档宽度，不是可见区域的宽度。
	// 			long scroll_width;
	// 			pElement->get_scrollWidth(&scroll_width);
	// 
	// 			//获得滚动条位置，从顶端开始。
	// 			long scroll_top;
	// 			pElement->get_scrollTop(&scroll_top);
	// 
	// 			//获得文档高度，including padding,but not including margin,border,or scroll bar.
	// 			long height;
	// 			pElement->get_clientHeight(&height);
	// 
	// 			//滚动到最后
	// 			hr = pElement->put_scrollTop(500);
	// 			if (FAILED(hr))
	// 			{
	// 				::MessageBox(NULL,_T("Failed to put_scrollTop"),_T("xx"),MB_OK);
	// 			}
	return S_OK;
}
HRESULT CBrowser::UnRegisterBrowserEventSink()
{
    HRESULT     hr = S_FALSE;
    m_pCP->Unadvise(m_dwEventCookie);
    m_pCP->Release();
    return hr;
}

void CBrowser::DocumentComplete( IDispatch *pDisp,VARIANT *URL) 
{ 

	DWORD dwDisp = (DWORD)pDisp;
	DWORD dwWeb = (DWORD)((IWebBrowser2*)m_spWebBrowser2);
	tstring url(_T("http://"));
	url += m_strCurrentUrl.c_str();
	url += _T("/");
	if ((!url.compare((LPCTSTR)URL->pcVal))||
		(dwDisp == dwWeb))
	{
		HRESULT hr = S_FALSE;
		IWebBrowser2* pWb = (IWebBrowser2*)m_spWebBrowser2;
		CComPtr< IDispatch >    pDisp;
		hr = pWb->get_Document( &pDisp );
		if (hr == S_OK)
		{
			CComQIPtr< IHTMLDocument2 > pDoc = pDisp;
			if( pDoc )
			{
				//TODO:此处已经接手到 DOC 发送去解析。
				MessageBox(NULL, L"Doc Done", L"Doc Done", MB_OK);
			}
		}
	}
	return ; 
} 