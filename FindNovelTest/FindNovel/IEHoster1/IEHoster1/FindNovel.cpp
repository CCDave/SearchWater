#include "StdAfx.h"
#include "FindNovel.h"

CFindNovel::CFindNovel(void)
{ 
	Initialize();
}
CFindNovel::~CFindNovel(void)
{
	Initialize();
}
void CFindNovel::ClearList(ListNode* listNode)
{
	if (!listNode->empty())
	{
		IteratorNode ite = listNode->begin();
		for (; ite != listNode->end(); ite++)
		{
			if (*ite)
			{
				delete (*ite);
				(*ite) = NULL;
			}
		}
		listNode->clear();
	}
}
void CFindNovel::DeleteNodes(IHTMLDOMNode* pNode, const DWORD dwChildrenCount)
{
	if (pNode &&  
		dwChildrenCount > 0 &&
		dwChildrenCount < NOVEL_STRING_LIMIT_COUNT)
	{
		VARIANT_BOOL vBool = VARIANT_FALSE;
		pNode->hasChildNodes(&vBool);
		if (vBool == VARIANT_TRUE)
		{
			IDispatchPtr IDisp = NULL;
			pNode->get_childNodes(&IDisp);
			if (IDisp)
			{
				CComQIPtr<IHTMLDOMChildrenCollection> pChildrenNode = IDisp;
				if (pChildrenNode)
				{
					long lengthChild = 0;
					pChildrenNode->get_length(&lengthChild);
					if (lengthChild == dwChildrenCount)
					{
						///此处默认保留第0个节点,剩下的全部删除
						for (int indexChild = 1; indexChild < lengthChild; indexChild++)
						{
							IDispatchPtr IDispChild = NULL;
							pChildrenNode->item(indexChild, &IDispChild);
							if (IDispChild)
							{
								CComQIPtr<IHTMLDOMNode> pChildNode = IDispChild;
								if (pChildNode)
								{
									CComPtr<IHTMLDOMNode> pNodeRemove = NULL;
									pChildNode->removeNode(VARIANT_TRUE, &pNodeRemove);
								}
							}
						}
					}
				}
			}
		}
	}
}
void  CFindNovel::DeleteExString(CNovelElementInfo& in_out_ElementInfo)
{
	if (in_out_ElementInfo.pElement)
	{
		CComQIPtr<IHTMLDOMNode> pNodeRoot = in_out_ElementInfo.pElement;
		if (pNodeRoot)
		{
			VARIANT_BOOL vBool(VARIANT_FALSE);
			pNodeRoot->hasChildNodes(&vBool);
			if (vBool == VARIANT_TRUE)
			{
				IDispatchPtr IDisp = NULL;
				pNodeRoot->get_childNodes(&IDisp);
				if (IDisp)
				{
					CComQIPtr<IHTMLDOMChildrenCollection> pChildrenNode = IDisp;
					if (pChildrenNode)
					{
						long length = 0;
						pChildrenNode->get_length(&length);
						for (int index = 0; index < length; index++)
						{
							IDispatchPtr IDispChild = NULL;
							pChildrenNode->item(index, &IDispChild);
							if (IDispChild)
							{
								CComQIPtr<IHTMLDOMNode> pChildNode = IDispChild;
								if (pChildNode)
								{
									DeleteNodes(pChildNode, in_out_ElementInfo.dwGrandSonCount);
								}
							}
						}
					}
				}
			}
			in_out_ElementInfo.pElement = pNodeRoot;
		}
	}
}

void CFindNovel::EnumElement(IHTMLDOMNode* spNode, const DWORD FatherNodeStringLength, VectorNovelPos FatherPos ,const DWORD DocumentStringLength)
{
	if (spNode)
	{	
		HRESULT hRet = S_FALSE;
		///字符串长度验证，百分比
		if (IsPercentOk(ELEMENT_CONTENT_LENGTH_PERCENT, DocumentStringLength, FatherNodeStringLength))
		{
			///存在子对象，继续枚举
			CComQIPtr<IHTMLDOMChildrenCollection> pChildNodes = EnumIHTMLDOMNode(spNode);			
			if (pChildNodes)
			{
				long ChildrenCount = 0;
				pChildNodes->get_length(&ChildrenCount);
				if (ChildrenCount)
				{	
					///枚举每个子对象
					DWORD dwCount_Children = 0;
					ZeroMemory(m_arryChildren, sizeof(DWORD) * NOVEL_STRING_LIMIT_COUNT);
					for (int index = 0; index < ChildrenCount; index++)
					{
						IDispatchPtr IDispChild = NULL;
						pChildNodes->item(index, &IDispChild);

						CComQIPtr<IHTMLDOMNode> pNodeChild = IDispChild;
						if (pNodeChild)
						{
							///检查这个子对象有多少个子对象
							dwCount_Children = IsSingleModel(pNodeChild);
							
							if (dwCount_Children < NOVEL_STRING_LIMIT_COUNT)
							{
								///孙子对象数量符合限制条件。
								m_arryChildren[dwCount_Children]++;
								///没有子节点时跳过枚举
								if (!dwCount_Children)
								   continue;
							}
							CComQIPtr<IHTMLElement> pElementChild = pNodeChild;
							if (pElementChild)
							{
								DWORD ChildTextLength = 0;
								///获得字符串长度
								GetElementTextSize(pElementChild, &ChildTextLength);
								///较大的节点继续枚举
								if (ChildTextLength >= (FatherNodeStringLength / ChildrenCount))
								{
									VectorNovelPos ChileLevel = FatherPos;
									ChileLevel.push_back(index);
									EnumElement(pNodeChild, ChildTextLength, ChileLevel, DocumentStringLength);
								}	
							}
						}
					}///枚举结束，通过m_arryChildre数组获取该节点的每个孩子节点，的孩子节点的个数、

					DWORD dwMaxCountChildren = 0;
					DWORD dwNumOfStringChild = 0;
					
					///找到数组中最大值，为特殊类型数量
					for (int index = 0; index < NOVEL_STRING_LIMIT_COUNT; index++)
					{
						if (m_arryChildren[index] > dwMaxCountChildren)
						{
							dwMaxCountChildren = m_arryChildren[index];
							///小说中一句话节点，有几个子节点
							dwNumOfStringChild = index;
						}
					}
						
					if (
						((dwMaxCountChildren*2) > ChildrenCount) &&
						(ChildrenCount > 2)
						)
					{
						///根据 dwNumOfStringChild 判断一下是否有保护数据   dwNumOfStringChild > 1 为有保护数据
						///判断子节点是否符合<p>或者<br>结构 
						if(IsStringTagOk(spNode, dwNumOfStringChild))
						{
							CComQIPtr<IHTMLElement> pElement = spNode;
							InsertToTheList(pElement, FatherNodeStringLength, FatherPos, DocumentStringLength, dwMaxCountChildren, dwNumOfStringChild);
						}	
					}
				}
			}
		}
	}
}
CComQIPtr<IHTMLDOMChildrenCollection> CFindNovel::EnumIHTMLDOMNode(IHTMLDOMNode* pNode)
{
	IDispatchPtr DispChildrenCollection = NULL;
	CComQIPtr<IHTMLDOMChildrenCollection> RetChildrenCollection;
	HRESULT hr = S_FALSE;

	if (pNode)
	{
		VARIANT_BOOL vBool(VARIANT_FALSE);
		hr = pNode->hasChildNodes(&vBool);
		if (!hr)
			if (vBool == VARIANT_TRUE)
				pNode->get_childNodes(&DispChildrenCollection);
		RetChildrenCollection = DispChildrenCollection;
	}
	return RetChildrenCollection;
}
HRESULT CFindNovel::FindElement(IHTMLDocument2* pDoc, CElementInfo* out_ElementInfo, CCondition* in_Condition)
{
	HRESULT hRet = S_FALSE;
	CNovelElementInfo Info;
	if (pDoc && out_ElementInfo)
	{
		hRet = FindNovel(pDoc, &Info);
		if (!hRet)
		{
			(*out_ElementInfo) = (CElementInfo*)&Info;
			hRet = S_OK;
		}
	}
	return hRet;
}
HRESULT CFindNovel::FindNovel(IHTMLDocument2* pDoc, CNovelElementInfo* out_ElementInfo)
{
	HRESULT hRet = S_FALSE; 
	if (pDoc)
	{
		CComPtr<IHTMLElement> pElement;
		hRet = pDoc->get_body(&pElement);
		if (!hRet)
		{
			hRet = S_FALSE;
			if (pElement)
			{
				DWORD dwTime = GetTickCount();
				///初始化
				Initialize();
				///获取主对象长度
				DWORD dwDocumentTextLength;
				if (GetElementTextSize(pElement, &dwDocumentTextLength))
				{
					CComQIPtr<IHTMLDOMNode> pRoot = pElement;
					if (pRoot)
					{
						VectorNovelPos level;
						///枚举对象,找到符合规格的对象
						EnumElement(pRoot, dwDocumentTextLength, level, dwDocumentTextLength);
						if (!m_listNode.empty())
						{
							///找到小说///在已找到的对象中根据层级关系找到小说对象，
							hRet = FindNovelFromList(&m_listNode, out_ElementInfo);

							if (out_ElementInfo->dwGrandSonCount > 1)
								DeleteExString((*out_ElementInfo));

							DWORD dwUseTime = GetTickCount() - dwTime;
							CString str;
							str.Format(_T("%d"), dwUseTime);
							MessageBox(NULL, str, _T(""), MB_OK);
						}	
					}
				}
			}
		}
	}
	return hRet;
}

HRESULT CFindNovel::FindNovelFromList(ListNode* listElementInfo, CNovelElementInfo* out_ElementInfo)
{
	HRESULT hRet = S_FALSE;
	if (!listElementInfo->empty())
	{
		DWORD dwSize = 0;
		dwSize = listElementInfo->size();
		if (dwSize == 1)
		{
			///最大的这个就是
			(*out_ElementInfo) = listElementInfo->front();
			hRet = S_OK;
		}
		else
		{		
			///找到最深的两个节点
			IteratorNode ite = listElementInfo->begin();
			for ( int index = 0; index < SAVE_ELEMENT_COUNT; index++)
			{
				if (*ite)
					m_pArryElement[index] = *ite;
				ite++;
			}	
			if (m_pArryElement[0] && m_pArryElement[1])
			{
				if(m_pArryElement[0]->vElementPos.size() == m_pArryElement[1]->vElementPos.size())
				{
					///如果两节点层数相等，返回第一个
					(*out_ElementInfo) = m_pArryElement[0]->dwChildrenCount > m_pArryElement[1]->dwChildrenCount ? m_pArryElement[0] : m_pArryElement[1];
					hRet = S_OK;
				}
				else
				{
					///如果不等，找到小说 
					if ((m_pArryElement[0]->vElementPos.size() - m_pArryElement[1]->vElementPos.size()) == 1)
					{
						(*out_ElementInfo) = (m_pArryElement[1]->dwContentLength <= (m_pArryElement[0]->dwContentLength*2)) ? m_pArryElement[0] : m_pArryElement[1];
						hRet = S_OK;
					}
					else
					{
						(*out_ElementInfo) = m_pArryElement[0]->dwChildrenCount > m_pArryElement[1]->dwChildrenCount ? m_pArryElement[0] : m_pArryElement[1];
						hRet = S_OK;
					}
				}
			}
		}
	}
	return hRet;
}
///获取节点字符串
CString CFindNovel::GetElementText(IHTMLElement* pElement)
{
	CString strRet(_T(""));
	if (pElement)
	{
		HRESULT hr = S_FALSE;
		m_strContentTemp.Empty();
		hr = pElement->get_innerText(&m_strContentTemp);
		if (!hr)
			strRet = m_strContentTemp;
	}
	return strRet;
	
}
BOOL CFindNovel::GetElementTextSize(IHTMLElement* pElement, DWORD* pElementInfo)
{
	BOOL bRet = FALSE;
	HRESULT hr = S_FALSE;
	if (pElement)
	{
		m_strContentTemp.Empty();
		hr = pElement->get_innerText(&m_strContentTemp);
		if (S_OK == hr)
		{ 
			*pElementInfo = m_strContentTemp.Length();
			bRet = TRUE;
		}
	}
	return bRet;
}
DWORD CFindNovel::IsSingleModel(IHTMLDOMNode* pNode)
{
	long Rlength = 0;

	///找到子对象
	CComQIPtr<IHTMLDOMChildrenCollection> pNodeChildren = EnumIHTMLDOMNode(pNode);
	if (pNodeChildren)
	{
		pNodeChildren->get_length(&Rlength);
		if (Rlength >= NOVEL_STRING_LIMIT_COUNT)    ///子对象符合条件数量,直接返回子对象数量
			Rlength =  OUT_LIMIT;  ///超出条件数量，一定为多个子节点。
				 
	}
	return Rlength;
}
void CFindNovel::Initialize()
{
	ClearList(&m_listNode);
	m_strContentTemp.Empty();
	ZeroMemory(m_arryChildren, sizeof(DWORD)*NOVEL_STRING_LIMIT_COUNT);
	ZeroMemory(m_pArryElement, sizeof(CNovelElementInfo*)*SAVE_ELEMENT_COUNT);
}

BOOL CFindNovel::IsPercentOk(const DWORD percent,const DWORD Deno, const DWORD Member)
{
	BOOL bRet = FALSE;
	float fDeno = (float)Deno;
	float fMember = (float)Member;
	float fPercent = fMember / fDeno * 100;
	if (((DWORD)fPercent) > percent)
		bRet = TRUE;
	return bRet;
}
void CFindNovel::InsertToTheList(IHTMLElement* pElement, const DWORD FatherNodeStringLength, VectorNovelPos Fatherlevel, const DWORD DocumentStringLength, const DWORD dwChildrenCount, const DWORD dwGrandSonCount)
{
	HRESULT hr = S_FALSE;
	CNovelElementInfo* Info = new CNovelElementInfo;
	
	///父节点长度
	Info->dwContentLength = FatherNodeStringLength;
	///多少个子节点
	Info->dwChildrenCount = dwChildrenCount;
	///位置
	Info->vElementPos = Fatherlevel;
	///孙子节点个数
	Info->dwGrandSonCount = dwGrandSonCount;

	if ((Info->dwContentLength > 10) &&
		(Info->dwContentLength < DocumentStringLength))
	{
		Info->pElement = pElement;
		if (m_listNode.empty())
			m_listNode.push_back(Info);	
		else
			m_listNode.push_back(Info);
	}
	else
		delete Info;
}
BOOL CFindNovel::IsStringTagOk(IHTMLDOMNode* pNode, const DWORD dwNumOfStringChile)
{
	BOOL bRet = FALSE;
	CComBSTR bstrTag(_T(""));
	CString strTag(_T(""));
	///获取子节点
	CComQIPtr<IHTMLDOMChildrenCollection> pChildrenNodes = EnumIHTMLDOMNode(pNode);
	if (pChildrenNodes)
	{
		long length =0;
		pChildrenNodes->get_length(&length);
		///记录Tag符合类型的数量
		DWORD dwCountTagOK = 0;
		///孩子节点长度
		long lengthChildren = 0;
		///检查多少个元素
		DWORD dwCountCheck = length / 2; 
		for (int index = 0; index < length; index++)
		{
			lengthChildren = 0;
			IDispatchPtr IDispChild = NULL;
			pChildrenNodes->item(index, &IDispChild);
			if (IDispChild)
			{
				CComQIPtr<IHTMLDOMNode> pChildNode= IDispChild;
				CComQIPtr<IHTMLDOMChildrenCollection> pNodeGrandSonNodes = EnumIHTMLDOMNode(pChildNode);
				
				if (pNodeGrandSonNodes)
					pNodeGrandSonNodes->get_length(&lengthChildren);
				else
					lengthChildren = 0; ///没有子节点,该节点为0个子节点类型。

				///节点子对象数与目标子节点数相同时，找到目标类型节点
				if (lengthChildren == dwNumOfStringChile)
				{
					CComQIPtr<IHTMLElement> pElementChild = pChildNode;
					if (pElementChild)
					{
						///获取类型tagname
						pElementChild->get_tagName(&bstrTag);
						strTag = bstrTag;
						if (!strTag.IsEmpty())
						{
							if (
								(!strTag.Compare(_T("p")))    ||
								(!strTag.Compare(_T("br")))   ||
								(!strTag.Compare(_T("P")))    ||
								(!strTag.Compare(_T("BR"))) 
								)
							{
								dwCountTagOK++;
								///相同类型找到TAG_CHECK_TIMES个节点符合<p> <br>类型认为该父节点为类似小说节点。
								if (dwCountTagOK == TAG_CHECK_TIMES)
								{
									bRet = TRUE;
									break;
								}
							}
						}
					}
				}
				///如果找到半数还没有找够TAG_CHECK_TIMES个节点，则放弃遍历
				if (index == dwCountCheck)
					break;				
			}
		}
	}
	return bRet;
}
