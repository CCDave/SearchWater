#pragma once

/*******************************************************************************************

	name:小说查找类
	use:在htmldocument中找到小说正文Element
	
	Way:小说结构特点: 1.有多个子节点.
					  2.子节点大部分有相同的结构（ps:这里的结构指的事有相同个数的子节点）
					  3.字符串长度大于一定比例。（ps:这里的比例指父节点字符串长度除以子节点个数）

********************************************************************************************/


#include <list>
#include <vector>
#include <comdef.h>
#include "IFindElement.h"
#define  ELEMENT_CONTENT_LENGTH_PERCENT  10    ///字符串限制，占document字符串长度的百分比
#define  SAVE_ELEMENT_COUNT              2	   ///二次删选的个数
#define NOVEL_STRING_LIMIT_COUNT         3	   ///小说每句话的子节点限制
#define OUT_LIMIT                        100   ///小说每句话的子节点筛选标记，超出筛选范围
#define TAG_CHECK_TIMES                  2     ///标记，找到有TAG_CHECK_TIMES个tag符合<p>或<br>结构的节点为类似小说节点。

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
	///节点字符串产度
	DWORD dwContentLength;
	///子节点个数
	DWORD dwChildrenCount;
	///子节点普遍有多少个子节点
	DWORD dwGrandSonCount;
};
#endif

typedef std::list<CNovelElementInfo*> ListNode;   ///小说节点信息链表
typedef ListNode::iterator IteratorNode;		  ///小说节点信息链表迭代器
typedef std::vector<DWORD>  VectorNovelPos;		  ///小说节点位置

class CFindNovel : public IFindElement
{
public:
	CFindNovel(void);
	virtual ~CFindNovel(void);

private:

	///类似小说对象链表
	ListNode m_listNode;
	///筛选后对象
	CNovelElementInfo* m_pArryElement[SAVE_ELEMENT_COUNT];
	///get_innerText 临时字符串
	CComBSTR m_strContentTemp;
	///记录子节点的孩子节点个数数组，用于归纳小说特性
	DWORD  m_arryChildren[NOVEL_STRING_LIMIT_COUNT];

protected:

	///初始化
	void Initialize();
	///清理链表
	void ClearList(ListNode* listNode);
	///是否符合百分比
	BOOL IsPercentOk(const DWORD percent,const DWORD Deno, const DWORD Member);

protected:

	///找到小说对象
	HRESULT FindNovel(IHTMLDocument2* pDoc, CNovelElementInfo* out_ElementInfo);

	///枚举Element
	void EnumElement(IHTMLDOMNode* spNode,const DWORD FatherNodeStringLength, VectorNovelPos FatherPos ,const DWORD DocumentStringLength);

	///从链表中搜索小说
	HRESULT FindNovelFromList(ListNode* listElementInfo, CNovelElementInfo* out_ElementInfo);

	///处理html中的保护字符串
	void  DeleteExString(CNovelElementInfo& in_out_ElementInfo);

protected:
	///枚举IHTMLDOMNode 获得 IHTMLDOMChildrenCollection
	CComQIPtr<IHTMLDOMChildrenCollection> EnumIHTMLDOMNode(IHTMLDOMNode* pNode);

	///判断子节点是否符合 <p>或者<br> 结构 
	BOOL IsStringTagOk(IHTMLDOMNode* pNode, const DWORD dwNumOfStringChile);

	///是否是单一模式
	DWORD IsSingleModel(IHTMLDOMNode* pNode);

	///获得一个Element的字符串大小
	BOOL GetElementTextSize(IHTMLElement* pElement, DWORD* pElementLength);

	///插入到链表中
	void InsertToTheList(IHTMLElement* pElement, const DWORD FatherNodeStringLength, VectorNovelPos Fatherlevel ,const DWORD DocumentStringLength, const DWORD dwChildrenCount, const DWORD dwGrandSonCount);
	
	///获取节点字符串
	CString GetElementText(IHTMLElement* pElement);

	///删除指定节点
	void DeleteNodes(IHTMLDOMNode* pNode, const DWORD dwChildrenCount);

public:
	///找小说element和其位置
	HRESULT FindElement(IHTMLDocument2* pDoc, CElementInfo* out_ElementInfo, CCondition* in_Condition = NULL);
};
