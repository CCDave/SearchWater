#include "StdAfx.h"
#include "HttpDocmentParse.h"

CHttpDocmentParse::CHttpDocmentParse(void) 
{
}

CHttpDocmentParse::~CHttpDocmentParse(void)
{
}

HRESULT CHttpDocmentParse::ParseHttpDocument(IHTMLDocument2* pDoc, LPVOID lPvoid /*�����������*/)
{
	HRESULT hRet = FALSE;
	if (!pDoc || !lPvoid)
		return hRet;

	hRet = FindElementByPos(pDoc, &_element_info, &_condition);
	if (SUCCEEDED(hRet))
	{
		//�ҵ�element ֱ�ӽ������element�׶�
	}
	else
	{
		//û�ҵ������Լ���element
		hRet = FindElement(pDoc, &_element_info, &_condition);
	}

	if (SUCCEEDED(hRet))
	{
		hRet = ParseElement(_element_info.pElement, lPvoid);
	}
	
	return hRet;
}