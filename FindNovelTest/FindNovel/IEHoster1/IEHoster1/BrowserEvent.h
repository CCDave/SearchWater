#pragma once
#include <ExDispid.h>
#include "FindNovel.h"
#include "ConfigFindElement.h"

///事件类用于挂接 DIID_DWebBrowserEvents2 事件
class CBrowserEvent: public IDispatch 
{
public:
	CBrowserEvent(void);
	~CBrowserEvent(void);

private:

	void* _pWeb; 	
	CFindNovel* _pFindNovel;
	CConfigFindElement* _pConfigFind;
	IFindElement* _FindElement;
	CElementInfo _INfo;
	CEdit* _edt;
	CString* _Url;

public:

	STDMETHODIMP QueryInterface(REFIID iid,void**ppvObject);
	STDMETHODIMP_(DWORD) AddRef(void);

	STDMETHODIMP_(DWORD) Release(void);

	HRESULT STDMETHODCALLTYPE GetTypeInfoCount( 
		unsigned int FAR* pctinfo 
		);

	HRESULT STDMETHODCALLTYPE GetTypeInfo( 
		unsigned int iTInfo, 
		LCID lcid, 
		ITypeInfo FAR* FAR* ppTInfo 
		);

	HRESULT STDMETHODCALLTYPE GetIDsOfNames( 
		REFIID riid, 
		OLECHAR FAR* FAR* rgszNames, 
		unsigned int cNames, 
		LCID lcid, 
		DISPID FAR* rgDispId 
		);

	HRESULT STDMETHODCALLTYPE Invoke( 
		DISPID dispIdMember, 
		REFIID riid, 
		LCID lcid, 
		WORD wFlags, 
		DISPPARAMS FAR* pDispParams, 
		VARIANT FAR* pVarResult, 
		EXCEPINFO FAR* pExcepInfo, 
		unsigned int FAR* puArgErr 
		);

public:
	void DocumentComplete( IDispatch *pDisp,VARIANT *URL);
	void NewWindow2(LPDISPATCH* ppDisp, VARIANT_BOOL * Cancel);

public:
	void SetSite(LPVOID lPvoid){ _pWeb = lPvoid;}
	void SetEdit(CEdit* pEdit){_edt = pEdit;}
	void SetUrl(CString*  pUrl){_Url = pUrl;}

};