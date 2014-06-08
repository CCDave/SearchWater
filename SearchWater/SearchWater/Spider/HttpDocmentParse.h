#ifndef _HTTP_CODUMENT_PARSE_OBJECT_
#define _HTTP_CODUMENT_PARSE_OBJECT_
#include "SpiderInterface.h"
#include "FindElementByPos.h"
#include "../../common/base/ObjectInterface.h"

class CHttpDocmentParse :
				public IHttpDocumentParse,
				public CFindElementByPos,
				public _object
{
public:
	CHttpDocmentParse(void);
	virtual ~CHttpDocmentParse(void);

	//先根据位置遭到element
	//再解析element拿到数据	
	STDMETHOD(ParseHttpDocument)(IHTMLDocument2* pDoc, LPVOID lPvoid /*解析后的数据*/);
	STDMETHOD(ParseElement)(IHTMLElement* pElement, LPVOID) PURE;
	virtual HRESULT FindElement(IHTMLDocument2* pDoc, CElementInfo* out_ElementInfo, CCondition* in_Condition){ATLASSERT(NULL); return S_OK;}

	STDMETHOD (init)(){ATLASSERT(NULL); return S_OK;}
	STDMETHOD (destory)(){ATLASSERT(NULL); return S_OK;}
protected:
	CCondition _condition;
	CElementInfo _element_info;
};
#endif