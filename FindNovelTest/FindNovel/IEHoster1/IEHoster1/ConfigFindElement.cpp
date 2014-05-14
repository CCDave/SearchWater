#include "StdAfx.h"
#include "ConfigFindElement.h"

CConfigFindElement::CConfigFindElement(void)
{
}

CConfigFindElement::~CConfigFindElement(void)
{
}
HRESULT CConfigFindElement::FindElement(IHTMLDocument2* pDoc, CElementInfo* out_ElementInfo, CCondition* in_Condition)
{
	HRESULT hRet = S_FALSE;
	HRESULT hr = S_FALSE;
	if (pDoc &&
		out_ElementInfo &&
		in_Condition)
	{
		CComQIPtr<IHTMLElement> pElement;
		CComQIPtr<IHTMLElement> pElementTemp;
		hr = pDoc->get_body(&pElement);
		if (!hr)
		{
			DWORD dwSize = in_Condition->vElementPos.size();
			if (dwSize > 0)
			{
				std::vector<DWORD> vNewPos;
				for (int index = 0; index < dwSize; index++)
				{
					hr = SearchElement(pElement, in_Condition->vElementPos[index], pElementTemp);
					if (!hr)
					{
						if (!pElementTemp)
						{
							hRet = S_OK;
							break;
						}
							
					}
					vNewPos.push_back(in_Condition->vElementPos[index]);
					pElement = pElementTemp;
				}
				out_ElementInfo->vElementPos = vNewPos;
				out_ElementInfo->pElement = pElement;
				if (S_OK == hRet)
					hRet = S_FALSE;
				else
					hRet = S_OK;
			}
		}
	}

	return hRet;
}

HRESULT CConfigFindElement::SearchElement(CComPtr<IHTMLElement> pElement, DWORD dwPos, CComQIPtr<IHTMLElement>& out_pElement)
{
	HRESULT hr = S_FALSE;

	CComQIPtr<IHTMLDOMNode> pNode = pElement;
	if (pNode)
	{
		VARIANT_BOOL vBool(VARIANT_FALSE);
		hr = pNode->hasChildNodes(&vBool);
		if (!hr)
		{
			if (VARIANT_TRUE == vBool)
			{
				IDispatchPtr pDisp = NULL;
				hr = pNode->get_childNodes(&pDisp);
				if (!hr)
				{
					CComQIPtr<IHTMLDOMChildrenCollection> pNodeChildren = pDisp;
					if (pNodeChildren)
					{
						long Length = 0;
						hr = pNodeChildren->get_length(&Length);
						if (!hr)
						{
							if (Length > dwPos)
							{
								IDispatchPtr pDispChild = NULL;
								hr = pNodeChildren->item(dwPos, &pDispChild);
								if (!hr)
								{
									out_pElement = pDispChild;
									hr = S_OK;
								}
							}
						}
					}
				}
			}
		}
	}
	return hr;
}