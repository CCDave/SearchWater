#include "StdAfx.h"
#include "HttpDocmentParse.h"

CHttpDocmentParse::CHttpDocmentParse(void) 
{
}

CHttpDocmentParse::~CHttpDocmentParse(void)
{
}

HRESULT CHttpDocmentParse::ParseHttpDocument(IHTMLDocument2* pDoc, LPVOID lPvoid /*解析后的数据*/)
{
	HRESULT hRet = FALSE;
	if (!pDoc || !lPvoid)
		return hRet;

	hRet = FindElementByPos(pDoc, &_element_info, &_condition);
	if (SUCCEEDED(hRet))
	{
		//找到element 直接进入解析element阶段
	}
	else
	{
		//没找到调用自己找element
		hRet = FindElement(pDoc, &_element_info, &_condition);
	}

	if (SUCCEEDED(hRet))
	{
		hRet = ParseElement(_element_info.pElement, lPvoid);
	}
	
	return hRet;
}