#pragma once
/*******************************************************************************************************************

	抽象接口类，此类的派生类用于完成FindElement方法，设计算法，用于在htmldocument中找到各种元素。

********************************************************************************************************************/
#include <vector>

#ifndef _ELEMENTINFOEX
#define _ELEMENTINFOEX
class CElementInfo 
{
public:
	///找到的对象
	CComQIPtr<IHTMLElement> pElement;
	///具体位置
	std::vector<DWORD> vElementPos;

	CElementInfo& operator=(CElementInfo* pElementInfo)
	{
		pElement = pElementInfo->pElement;
		vElementPos = pElementInfo->vElementPos;
		return *this;
	}
	CElementInfo& operator=(CElementInfo pElementInfo)
	{
		pElement = pElementInfo.pElement;
		vElementPos = pElementInfo.vElementPos;
		return *this;
	}

	CElementInfo(){}
	virtual~CElementInfo(){vElementPos.clear();}
};
#endif

#ifndef _ELEMENTCONDITIONEX
#define _ELEMENTCONDITIONEX
///条件类：在查找htmldocument时，有时需要一些条件约束。
class CCondition
{
public:
	CCondition(){}
	virtual~CCondition(){vElementPos.clear();}
public:
	std::vector<DWORD> vElementPos;

	CCondition& operator=(CCondition* pCondition)
	{
		vElementPos = pCondition->vElementPos;
		return *this;
	}
	CCondition& operator=(CCondition pCondition)
	{
		vElementPos = pCondition.vElementPos;
		return *this;
	}
};
#endif

class IFindElement
{
public:
	virtual HRESULT FindElement(IHTMLDocument2* pDoc, CElementInfo* out_ElementInfo, CCondition* in_Condition) = 0;
	virtual HRESULT FindElementByPos(IHTMLDocument2* pDoc, CElementInfo* out_ElementInfo, CCondition* in_Condition) = 0;
};
