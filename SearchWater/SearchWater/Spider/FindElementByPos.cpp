#include "StdAfx.h"
#include "FindElementByPos.h"

CFindElementByPos::CFindElementByPos(void)
{

}

CFindElementByPos::~CFindElementByPos(void)
{

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

		BOOL bIsDone = TRUE;

#define BREAK_ bIsDone = FALSE;\
			   break;

		for (size_t index = 0; index < in_Condition->vElementPos.size(); index ++)
		{	
			///存在子对象，继续枚举
			CComQIPtr<IHTMLDOMChildrenCollection> pChildNodes = EnumIHTMLDOMNode(pRoot);
			if (!pChildNodes)
			{
				BREAK_;
			}
			long ChildrenCount = 0;
			pChildNodes->get_length(&ChildrenCount);
			if (!ChildrenCount)
			{
				BREAK_;
			}
			IDispatchPtr IDispChild = NULL;
			pChildNodes->item(in_Condition->vElementPos[index], &IDispChild);
			
			pRoot = IDispChild;
			if (!pRoot)
			{
				BREAK_;
			}
		}
		
		if (bIsDone)
		{
			CComQIPtr<IHTMLElement> pElementChild = pRoot;
			if (pElementChild)
			{
				out_ElementInfo->pElement = pElementChild;
				hRet = S_OK;
			}
		}
	} while (FALSE);
	
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