#include "StdAfx.h"
#include "BrowserEvent.h"
#include "resource.h"
CBrowserEvent::CBrowserEvent(void) : _pWeb(NULL),_edt(NULL),_Url(NULL),_pFindNovel(NULL),_pConfigFind(NULL)
{
}

CBrowserEvent::~CBrowserEvent(void)
{
}


STDMETHODIMP CBrowserEvent::QueryInterface(REFIID iid,void**ppvObject)
{
	*ppvObject = 0; 
	if ( iid == IID_IOleClientSite ) 
		*ppvObject = (IOleClientSite*)this; 

	if ( iid == IID_IUnknown ) 
		*ppvObject = this; 

	if ( iid == IID_IDispatch ) 
		*ppvObject = (IDispatch*)this; 

	if ( iid == DIID_DWebBrowserEvents2 ) 
		*ppvObject = (DWebBrowserEvents2*)this; 

	if ( iid == IID_IDocHostUIHandler) 
		*ppvObject = (IDocHostUIHandler*)this; 

	if (iid == IID_IHTMLDocument2)
	{
		*ppvObject = (IHTMLDocument2*)this; 
	}  
	if ( *ppvObject ) 
	{ 
		AddRef(); 
		return S_OK; 
	} 
	return E_NOINTERFACE; 
}
STDMETHODIMP_(DWORD) CBrowserEvent::AddRef(void)
{
	return NULL;
}
STDMETHODIMP_(DWORD) CBrowserEvent::Release(void)
{
	return NULL;
}

HRESULT STDMETHODCALLTYPE CBrowserEvent::GetTypeInfoCount( 
	unsigned int FAR* pctinfo 
	)
{
	return E_NOTIMPL; 
}

HRESULT STDMETHODCALLTYPE CBrowserEvent::GetTypeInfo( 
	unsigned int iTInfo, 
	LCID lcid, 
	ITypeInfo FAR* FAR* ppTInfo 
	)
{
	return E_NOTIMPL; 
}

HRESULT STDMETHODCALLTYPE CBrowserEvent::GetIDsOfNames( 
	REFIID riid, 
	OLECHAR FAR* FAR* rgszNames, 
	unsigned int cNames, 
	LCID lcid, 
	DISPID FAR* rgDispId 
	)
{
	return E_NOTIMPL; 
}

HRESULT STDMETHODCALLTYPE CBrowserEvent::Invoke( 
	DISPID dispIdMember, 
	REFIID riid, 
	LCID lcid, 
	WORD wFlags, 
	DISPPARAMS FAR* pDispParams, 
	VARIANT FAR* pVarResult, 
	EXCEPINFO FAR* pExcepInfo, 
	unsigned int FAR* puArgErr 
	)
{
	if( dispIdMember == DISPID_DOCUMENTCOMPLETE) 
	{ 
		DocumentComplete(pDispParams->rgvarg[1].pdispVal,pDispParams->rgvarg[0].pvarVal); 
		return S_OK; 
	} 
	if (dispIdMember == DISPID_NEWWINDOW2)
	{
		NewWindow2(NULL, pDispParams->rgvarg[0].pboolVal);
	}
	return E_NOTIMPL; 

}


void CBrowserEvent::NewWindow2(LPDISPATCH* ppDisp, VARIANT_BOOL * Cancel)
{
	CComPtr<IHTMLDocument2> pHTMLDocument2;

	IWebBrowser2* pWb = (IWebBrowser2*)_pWeb;

	pWb->get_Document((IDispatch **)&pHTMLDocument2);
	if (pHTMLDocument2!=NULL)
	{
		CComPtr<IHTMLElement> pIHTMLElement;
		pHTMLDocument2->get_activeElement(&pIHTMLElement);

		if (pIHTMLElement!=NULL)
		{
			VARIANT url;
			HRESULT hr=pIHTMLElement->getAttribute(_T("href"), 0, &url);
			if (!hr)
			{
				hr=pWb->Navigate2(&url, NULL, NULL, NULL, NULL);

				if (!hr)
				{
					*Cancel=TRUE;
				}
			}
		}
	}

	//CHtmlView::OnNewWindow2(ppDisp, Cancel);
}

void CBrowserEvent::DocumentComplete( IDispatch *pDisp,VARIANT *URL) 
{ 

	DWORD dwDisp = (DWORD)pDisp;
	DWORD dwWeb = (DWORD)_pWeb;
	CString url(_T("http://"));
	url += _Url->GetBuffer(NULL);
	url += _T("/");
	if ((!url.Compare((LPCTSTR)URL->pcVal))||
		(dwDisp == dwWeb))
	{
		HRESULT hr = S_FALSE;
		IWebBrowser2* pWb = (IWebBrowser2*)_pWeb;
		CComPtr< IDispatch >    pDisp;
		hr = pWb->get_Document( &pDisp );
		if (hr == S_OK)
		{
			CComQIPtr< IHTMLDocument2 > pDoc = pDisp;
			if( pDoc )
			{
				//if (!_INfo.vElementPos.size())   ///此段代码开放根据位置查找信息功能
				{
					if (!_pFindNovel)
					{
						_pFindNovel = new CFindNovel;
					}
					_FindElement = _pFindNovel;
					hr = _FindElement->FindElement(pDoc, &_INfo, NULL);
					if (!hr)
					{
						CComBSTR strContent(_T(""));
						_INfo.pElement->get_innerText(&strContent);
						CString strNovel(strContent);
						if (!strNovel.IsEmpty())
						{
							_edt->SetWindowText(_T(""));
							///放入控件
							int index = _edt->GetLimitText();
							int Length = _edt->GetWindowTextLength();

							_edt->SetSel(Length, Length);
							_edt->ReplaceSel(strNovel);
							_edt->LineScroll( _edt->GetLineCount());
						}
					}
				}
				/*else
				{
					if (!_pConfigFind)
					{
						_pConfigFind = new CConfigFindElement;
					}
					_FindElement = _pConfigFind;

					CCondition condition;
					condition.vElementPos = _INfo.vElementPos;

					hr = _FindElement->FindElement(pDoc, &_INfo, &condition);
					if (!hr)
					{
						CComBSTR strContent(_T(""));
						_INfo.pElement->get_innerText(&strContent);
						CString strNovel(strContent);
						if (!strNovel.IsEmpty())
						{
							_edt->SetWindowText(_T(""));
							///放入控件
							int index = _edt->GetLimitText();
							int Length = _edt->GetWindowTextLength();

							_edt->SetSel(Length, Length);
							_edt->ReplaceSel(strNovel);
							_edt->LineScroll( _edt->GetLineCount());
						}
					}
				}*/
			}
		}
	}
	return ; 
} 