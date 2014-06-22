#include "StdAfx.h"
#include "FindElementByPos.h"

CFindElementByPos::CFindElementByPos(void)
{

}

CFindElementByPos::~CFindElementByPos(void)
{
	//TODO:此处清理数据
	m_vHtmlNodeInfo.clear();
}

HRESULT CFindElementByPos::FindElement(IHTMLDocument2* pDoc, CElementInfo* out_ElementInfo, CCondition* in_Condition)
{
	return S_OK;
}

HRESULT CFindElementByPos::FindElementByPos(IHTMLDocument2* pDoc, CElementInfo* out_ElementInfo, CCondition* in_Condition)
{
	HRESULT hRet = E_FAIL;
	
 	if (!pDoc || !out_ElementInfo || !in_Condition)
 		return hRet;
 	do 
 	{
 		if (!in_Condition->vElementPos.size())
 			break;
 		
 		CComPtr<IHTMLElement> pElement;
 		hRet = pDoc->get_body(&pElement);
 		if (FAILED(hRet) || !pElement)
 			break;
 		
 		CComQIPtr<IHTMLDOMNode> pRoot = pElement;
 		if (!pRoot)
 			break;		

		m_FindpElement = NULL;
		FindByPos(pRoot, NULL, in_Condition->vElementPos);
		
		if (m_FindpElement)
		{
			CComBSTR str;
			m_FindpElement->get_innerText(&str);
			CString strin = str;
			out_ElementInfo->pElement = m_FindpElement;
			out_ElementInfo->vElementPos = in_Condition->vElementPos;
			hRet = S_OK;
			MessageBox(NULL, strin, NULL, MB_OK);
		}


// 		VectorHtmlInfo vHtmlInfo;
// 		EnumDocumentInfo(pRoot, &vHtmlInfo, m_vInfPos);

	}while(FALSE);
	
	return hRet;
}

CComQIPtr<IHTMLDOMChildrenCollection> CFindElementByPos::EnumIHTMLDOMNode(IHTMLDOMNode* pNode)
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

void CFindElementByPos::EnumDocumentInfo(IHTMLDOMNode* pNode, LPVOID lPvoid, std::vector<WORD> vPos)
{
	do 
	{
		if (!pNode || !lPvoid)
			break;
		PVectorHtmlInfo pVNodeInfo = (PVectorHtmlInfo)lPvoid;
		if (!pVNodeInfo)
			break;

		CComQIPtr<IHTMLElement> pElement = pNode;
		if (pElement)
		{
			CComBSTR strClassName;
			HRESULT hr = pElement->get_className(&strClassName);
			if (SUCCEEDED(hr))
			{
				CString str = strClassName;
				if (str.CompareNoCase(L"jc-table-box jczq-table-box"))
				{
					CComBSTR strInerText;
					pElement->get_innerText(&strInerText);
					CComBSTR strInnerHtml;
					pElement->get_innerHTML(&strInnerHtml);
					CComBSTR strOuterHtml;
					pElement->get_innerHTML(&strOuterHtml);
				}
			}
// 			CComBSTR cstrText;
// 			HRESULT hr = pElement->get_innerText(&cstrText);
// 			if (SUCCEEDED(hr))
// 			{
// 				CString strTemp = cstrText;
// 				OutputDebugStr(strTemp);
// 				PSNode pInfo = new SNode;
// 				pInfo->strInfo = cstrText;
// 				pInfo->vPos = vPos;
// 				pVNodeInfo->push_back(pInfo);
// 			}
		}
		
		CComQIPtr<IHTMLDOMChildrenCollection> pChildNodes = EnumIHTMLDOMNode(pNode);
		if (!pChildNodes)
			break;

		long ChildrenCount = 0;
		pChildNodes->get_length(&ChildrenCount);
		if (!ChildrenCount)
			break;
		for (size_t index = 0; index < ChildrenCount; index ++)
		{
			IDispatchPtr IDispChild = NULL;
			pChildNodes->item(index, &IDispChild);
			
			if (!IDispChild)
				continue;
			CComQIPtr<IHTMLDOMNode> pChildNode = IDispChild;
			if (!pChildNode)
				continue;
			std::vector<WORD> vPosThisFlord = vPos;
			vPosThisFlord.push_back(index);
			EnumDocumentInfo(pChildNode, lPvoid, vPosThisFlord);
		}		

	} while (FALSE);
}

void CFindElementByPos::FindByPos(IHTMLDOMNode* pNode, LPVOID lPvoid, std::vector<DWORD> vPos)
{	
	do 
	{
		if (!vPos.size())
		{
			MessageBox(NULL,NULL,NULL,MB_OK);
			CComQIPtr<IHTMLElement> pElement = pNode;
			if (pElement)
			{
				CComBSTR str;
				pElement->get_innerText(&str);
				m_FindpElement = pElement;
				CString strint = str;
				OutputDebugStr(strint);
			}
			break;
		}
		CComQIPtr<IHTMLDOMChildrenCollection> pChildNodes = EnumIHTMLDOMNode(pNode);
		if (!pChildNodes)
			break;
		long ChildrenCount = 0;
		pChildNodes->get_length(&ChildrenCount);

		if (!ChildrenCount)
			break;

		size_t index = vPos.front();

		IDispatchPtr IDispChild = NULL;
		pChildNodes->item(index, &IDispChild);

		if (!IDispChild)
			break;

		CComQIPtr<IHTMLDOMNode> pChildNode = IDispChild;

		if (!pChildNode)
			break;
		std::vector<DWORD>::iterator ite = vPos.begin();
		vPos.erase(ite);

		FindByPos(pChildNode, lPvoid, vPos);

	} while (FALSE);
}