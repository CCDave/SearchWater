#pragma once
#include "HttpDocmentParse.h"
#include "../../common/base/ObjectInterface.h"
class CParseHomezjcq : 
					public CHttpDocmentParse
{
public:
	CParseHomezjcq(void);
	virtual ~CParseHomezjcq(void);
	
	STDMETHOD(ParseElement)(IHTMLElement* pElement, LPVOID);
	virtual HRESULT FindElement(IHTMLDocument2* pDoc, CElementInfo* out_ElementInfo, CCondition* in_Condition);
protected:
	BOOL ParseText(LPCWSTR lpszText, LPVOID lPvoid);
	BOOL ParseHtml(LPCWSTR lpszHtml, LPVOID lPvoid);

	STDMETHOD (init)();
	STDMETHOD (destory)();

};
