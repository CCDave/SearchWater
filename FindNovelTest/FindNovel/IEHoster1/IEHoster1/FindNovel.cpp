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
						///�˴�Ĭ�ϱ�����0���ڵ�,ʣ�µ�ȫ��ɾ��
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
		///�ַ���������֤���ٷֱ�
		if (IsPercentOk(ELEMENT_CONTENT_LENGTH_PERCENT, DocumentStringLength, FatherNodeStringLength))
		{
			///�����Ӷ��󣬼���ö��
			CComQIPtr<IHTMLDOMChildrenCollection> pChildNodes = EnumIHTMLDOMNode(spNode);			
			if (pChildNodes)
			{
				long ChildrenCount = 0;
				pChildNodes->get_length(&ChildrenCount);
				if (ChildrenCount)
				{	
					///ö��ÿ���Ӷ���
					DWORD dwCount_Children = 0;
					ZeroMemory(m_arryChildren, sizeof(DWORD) * NOVEL_STRING_LIMIT_COUNT);
					for (int index = 0; index < ChildrenCount; index++)
					{
						IDispatchPtr IDispChild = NULL;
						pChildNodes->item(index, &IDispChild);

						CComQIPtr<IHTMLDOMNode> pNodeChild = IDispChild;
						if (pNodeChild)
						{
							///�������Ӷ����ж��ٸ��Ӷ���
							dwCount_Children = IsSingleModel(pNodeChild);
							
							if (dwCount_Children < NOVEL_STRING_LIMIT_COUNT)
							{
								///���Ӷ���������������������
								m_arryChildren[dwCount_Children]++;
								///û���ӽڵ�ʱ����ö��
								if (!dwCount_Children)
								   continue;
							}
							CComQIPtr<IHTMLElement> pElementChild = pNodeChild;
							if (pElementChild)
							{
								DWORD ChildTextLength = 0;
								///����ַ�������
								GetElementTextSize(pElementChild, &ChildTextLength);
								///�ϴ�Ľڵ����ö��
								if (ChildTextLength >= (FatherNodeStringLength / ChildrenCount))
								{
									VectorNovelPos ChileLevel = FatherPos;
									ChileLevel.push_back(index);
									EnumElement(pNodeChild, ChildTextLength, ChileLevel, DocumentStringLength);
								}	
							}
						}
					}///ö�ٽ�����ͨ��m_arryChildre�����ȡ�ýڵ��ÿ�����ӽڵ㣬�ĺ��ӽڵ�ĸ�����

					DWORD dwMaxCountChildren = 0;
					DWORD dwNumOfStringChild = 0;
					
					///�ҵ����������ֵ��Ϊ������������
					for (int index = 0; index < NOVEL_STRING_LIMIT_COUNT; index++)
					{
						if (m_arryChildren[index] > dwMaxCountChildren)
						{
							dwMaxCountChildren = m_arryChildren[index];
							///С˵��һ�仰�ڵ㣬�м����ӽڵ�
							dwNumOfStringChild = index;
						}
					}
						
					if (
						((dwMaxCountChildren*2) > ChildrenCount) &&
						(ChildrenCount > 2)
						)
					{
						///���� dwNumOfStringChild �ж�һ���Ƿ��б�������   dwNumOfStringChild > 1 Ϊ�б�������
						///�ж��ӽڵ��Ƿ����<p>����<br>�ṹ 
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
				///��ʼ��
				Initialize();
				///��ȡ�����󳤶�
				DWORD dwDocumentTextLength;
				if (GetElementTextSize(pElement, &dwDocumentTextLength))
				{
					CComQIPtr<IHTMLDOMNode> pRoot = pElement;
					if (pRoot)
					{
						VectorNovelPos level;
						///ö�ٶ���,�ҵ����Ϲ��Ķ���
						EnumElement(pRoot, dwDocumentTextLength, level, dwDocumentTextLength);
						if (!m_listNode.empty())
						{
							///�ҵ�С˵///�����ҵ��Ķ����и��ݲ㼶��ϵ�ҵ�С˵����
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
			///�����������
			(*out_ElementInfo) = listElementInfo->front();
			hRet = S_OK;
		}
		else
		{		
			///�ҵ�����������ڵ�
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
					///������ڵ������ȣ����ص�һ��
					(*out_ElementInfo) = m_pArryElement[0]->dwChildrenCount > m_pArryElement[1]->dwChildrenCount ? m_pArryElement[0] : m_pArryElement[1];
					hRet = S_OK;
				}
				else
				{
					///������ȣ��ҵ�С˵ 
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
///��ȡ�ڵ��ַ���
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

	///�ҵ��Ӷ���
	CComQIPtr<IHTMLDOMChildrenCollection> pNodeChildren = EnumIHTMLDOMNode(pNode);
	if (pNodeChildren)
	{
		pNodeChildren->get_length(&Rlength);
		if (Rlength >= NOVEL_STRING_LIMIT_COUNT)    ///�Ӷ��������������,ֱ�ӷ����Ӷ�������
			Rlength =  OUT_LIMIT;  ///��������������һ��Ϊ����ӽڵ㡣
				 
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
	
	///���ڵ㳤��
	Info->dwContentLength = FatherNodeStringLength;
	///���ٸ��ӽڵ�
	Info->dwChildrenCount = dwChildrenCount;
	///λ��
	Info->vElementPos = Fatherlevel;
	///���ӽڵ����
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
	///��ȡ�ӽڵ�
	CComQIPtr<IHTMLDOMChildrenCollection> pChildrenNodes = EnumIHTMLDOMNode(pNode);
	if (pChildrenNodes)
	{
		long length =0;
		pChildrenNodes->get_length(&length);
		///��¼Tag�������͵�����
		DWORD dwCountTagOK = 0;
		///���ӽڵ㳤��
		long lengthChildren = 0;
		///�����ٸ�Ԫ��
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
					lengthChildren = 0; ///û���ӽڵ�,�ýڵ�Ϊ0���ӽڵ����͡�

				///�ڵ��Ӷ�������Ŀ���ӽڵ�����ͬʱ���ҵ�Ŀ�����ͽڵ�
				if (lengthChildren == dwNumOfStringChile)
				{
					CComQIPtr<IHTMLElement> pElementChild = pChildNode;
					if (pElementChild)
					{
						///��ȡ����tagname
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
								///��ͬ�����ҵ�TAG_CHECK_TIMES���ڵ����<p> <br>������Ϊ�ø��ڵ�Ϊ����С˵�ڵ㡣
								if (dwCountTagOK == TAG_CHECK_TIMES)
								{
									bRet = TRUE;
									break;
								}
							}
						}
					}
				}
				///����ҵ�������û���ҹ�TAG_CHECK_TIMES���ڵ㣬���������
				if (index == dwCountCheck)
					break;				
			}
		}
	}
	return bRet;
}
