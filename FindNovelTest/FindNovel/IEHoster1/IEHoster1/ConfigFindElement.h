#pragma once
#include <comdef.h>
#include "IFindElement.h"
/*******************************************************************************************

	name:�ڵ������
	use:��htmldocument���ҵ���Ӧλ�õĽڵ�
	
	Way:ͨ������Ľڵ�λ�ã���htmldocument���ҵ���Ӧ�Ľڵ�

********************************************************************************************/
class CConfigFindElement : public IFindElement
{
public:
	CConfigFindElement(void);
	virtual ~CConfigFindElement(void);
protected:
	///���ҵ�pElement�ĵ�dwPos���ӽڵ㲢����
	HRESULT SearchElement(CComPtr<IHTMLElement> pElement, DWORD dwPos, CComQIPtr<IHTMLElement>& out_pElement);
public:
	///���ݽڵ�λ����Document���ҵ��ڵ�
	HRESULT FindElement(IHTMLDocument2* pDoc, CElementInfo* out_ElementInfo, CCondition* in_Condition);
};
