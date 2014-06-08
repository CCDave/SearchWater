#pragma once
/*******************************************************************************************************************

	����ӿ��࣬������������������FindElement����������㷨��������htmldocument���ҵ�����Ԫ�ء�

********************************************************************************************************************/
#include <vector>

#ifndef _ELEMENTINFOEX
#define _ELEMENTINFOEX
class CElementInfo 
{
public:
	///�ҵ��Ķ���
	CComQIPtr<IHTMLElement> pElement;
	///����λ��
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
///�����ࣺ�ڲ���htmldocumentʱ����ʱ��ҪһЩ����Լ����
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
