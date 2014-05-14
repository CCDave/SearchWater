#pragma once
#include <comdef.h>
#include "IFindElement.h"
/*******************************************************************************************

	name:节点查找类
	use:在htmldocument中找到相应位置的节点
	
	Way:通过传入的节点位置，在htmldocument中找到相应的节点

********************************************************************************************/
class CConfigFindElement : public IFindElement
{
public:
	CConfigFindElement(void);
	virtual ~CConfigFindElement(void);
protected:
	///在找到pElement的第dwPos个子节点并返回
	HRESULT SearchElement(CComPtr<IHTMLElement> pElement, DWORD dwPos, CComQIPtr<IHTMLElement>& out_pElement);
public:
	///根据节点位置在Document中找到节点
	HRESULT FindElement(IHTMLDocument2* pDoc, CElementInfo* out_ElementInfo, CCondition* in_Condition);
};
