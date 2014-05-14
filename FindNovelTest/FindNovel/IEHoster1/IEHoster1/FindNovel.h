#pragma once

/*******************************************************************************************

	name:С˵������
	use:��htmldocument���ҵ�С˵����Element
	
	Way:С˵�ṹ�ص�: 1.�ж���ӽڵ�.
					  2.�ӽڵ�󲿷�����ͬ�Ľṹ��ps:����Ľṹָ��������ͬ�������ӽڵ㣩
					  3.�ַ������ȴ���һ����������ps:����ı���ָ���ڵ��ַ������ȳ����ӽڵ������

********************************************************************************************/


#include <list>
#include <vector>
#include <comdef.h>
#include "IFindElement.h"
#define  ELEMENT_CONTENT_LENGTH_PERCENT  10    ///�ַ������ƣ�ռdocument�ַ������ȵİٷֱ�
#define  SAVE_ELEMENT_COUNT              2	   ///����ɾѡ�ĸ���
#define NOVEL_STRING_LIMIT_COUNT         3	   ///С˵ÿ�仰���ӽڵ�����
#define OUT_LIMIT                        100   ///С˵ÿ�仰���ӽڵ�ɸѡ��ǣ�����ɸѡ��Χ
#define TAG_CHECK_TIMES                  2     ///��ǣ��ҵ���TAG_CHECK_TIMES��tag����<p>��<br>�ṹ�Ľڵ�Ϊ����С˵�ڵ㡣

#ifndef _NOVELELEMENTINFO
#define _NOVELELEMENTINFO
class CNovelElementInfo : public CElementInfo
{
public:
	CNovelElementInfo() : dwContentLength(0),dwChildrenCount(0),dwGrandSonCount(0) {}
	~CNovelElementInfo(){}

CNovelElementInfo& operator=(CNovelElementInfo* pElementInfo)
{
	pElement = pElementInfo->pElement;
	vElementPos = pElementInfo->vElementPos;
	dwContentLength = pElementInfo->dwContentLength;
	dwChildrenCount = pElementInfo->dwChildrenCount;
	dwGrandSonCount = pElementInfo->dwGrandSonCount;
	return *this;
}
CNovelElementInfo& operator=(CNovelElementInfo pElementInfo)
{
	pElement = pElementInfo.pElement;
	vElementPos = pElementInfo.vElementPos;
	dwContentLength = pElementInfo.dwContentLength;
	dwChildrenCount = pElementInfo.dwChildrenCount;
	dwGrandSonCount = pElementInfo.dwGrandSonCount;
	return *this;
}


public:
	///�ڵ��ַ�������
	DWORD dwContentLength;
	///�ӽڵ����
	DWORD dwChildrenCount;
	///�ӽڵ��ձ��ж��ٸ��ӽڵ�
	DWORD dwGrandSonCount;
};
#endif

typedef std::list<CNovelElementInfo*> ListNode;   ///С˵�ڵ���Ϣ����
typedef ListNode::iterator IteratorNode;		  ///С˵�ڵ���Ϣ���������
typedef std::vector<DWORD>  VectorNovelPos;		  ///С˵�ڵ�λ��

class CFindNovel : public IFindElement
{
public:
	CFindNovel(void);
	virtual ~CFindNovel(void);

private:

	///����С˵��������
	ListNode m_listNode;
	///ɸѡ�����
	CNovelElementInfo* m_pArryElement[SAVE_ELEMENT_COUNT];
	///get_innerText ��ʱ�ַ���
	CComBSTR m_strContentTemp;
	///��¼�ӽڵ�ĺ��ӽڵ�������飬���ڹ���С˵����
	DWORD  m_arryChildren[NOVEL_STRING_LIMIT_COUNT];

protected:

	///��ʼ��
	void Initialize();
	///��������
	void ClearList(ListNode* listNode);
	///�Ƿ���ϰٷֱ�
	BOOL IsPercentOk(const DWORD percent,const DWORD Deno, const DWORD Member);

protected:

	///�ҵ�С˵����
	HRESULT FindNovel(IHTMLDocument2* pDoc, CNovelElementInfo* out_ElementInfo);

	///ö��Element
	void EnumElement(IHTMLDOMNode* spNode,const DWORD FatherNodeStringLength, VectorNovelPos FatherPos ,const DWORD DocumentStringLength);

	///������������С˵
	HRESULT FindNovelFromList(ListNode* listElementInfo, CNovelElementInfo* out_ElementInfo);

	///����html�еı����ַ���
	void  DeleteExString(CNovelElementInfo& in_out_ElementInfo);

protected:
	///ö��IHTMLDOMNode ��� IHTMLDOMChildrenCollection
	CComQIPtr<IHTMLDOMChildrenCollection> EnumIHTMLDOMNode(IHTMLDOMNode* pNode);

	///�ж��ӽڵ��Ƿ���� <p>����<br> �ṹ 
	BOOL IsStringTagOk(IHTMLDOMNode* pNode, const DWORD dwNumOfStringChile);

	///�Ƿ��ǵ�һģʽ
	DWORD IsSingleModel(IHTMLDOMNode* pNode);

	///���һ��Element���ַ�����С
	BOOL GetElementTextSize(IHTMLElement* pElement, DWORD* pElementLength);

	///���뵽������
	void InsertToTheList(IHTMLElement* pElement, const DWORD FatherNodeStringLength, VectorNovelPos Fatherlevel ,const DWORD DocumentStringLength, const DWORD dwChildrenCount, const DWORD dwGrandSonCount);
	
	///��ȡ�ڵ��ַ���
	CString GetElementText(IHTMLElement* pElement);

	///ɾ��ָ���ڵ�
	void DeleteNodes(IHTMLDOMNode* pNode, const DWORD dwChildrenCount);

public:
	///��С˵element����λ��
	HRESULT FindElement(IHTMLDocument2* pDoc, CElementInfo* out_ElementInfo, CCondition* in_Condition = NULL);
};
