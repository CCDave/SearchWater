#pragma once
#include "IFindElement.h"

class CFindElementByPos : 
				public IFindElement

{
public:
	CFindElementByPos(void);
	virtual ~CFindElementByPos(void);

	virtual HRESULT FindElement(IHTMLDocument2* pDoc, CElementInfo* out_ElementInfo, CCondition* in_Condition);
	virtual HRESULT FindElementByPos(IHTMLDocument2* pDoc, CElementInfo* out_ElementInfo, CCondition* in_Condition);
protected:

	CComQIPtr<IHTMLDOMChildrenCollection> CFindElementByPos::EnumIHTMLDOMNode(IHTMLDOMNode* pNode);
};
