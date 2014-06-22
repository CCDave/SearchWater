#include "StdAfx.h"
#include "ParseHomezjcq.h"
#include "AdvFunction.h"

CParseHomezjcq::CParseHomezjcq(void)
{
	init();
}

CParseHomezjcq::~CParseHomezjcq(void)
{

}

HRESULT CParseHomezjcq::ParseElement(IHTMLElement* pElement, LPVOID lPVoid)
{
	HRESULT hRet = E_FAIL;
	do 
	{
		if (!pElement || !lPVoid)
			break;
		CComQIPtr<IHTMLDOMNode> pNode = pElement;
		if (!pNode)
			break;
		CComBSTR strText;
		pElement->get_innerText(&strText);
		CString strInnerText = strText;

		CComBSTR strHtml;
		pElement->get_innerHTML(&strHtml);
		CString strInnerHtml = strHtml;

		if (!ParseText(strInnerText, lPVoid))
			break;

		if (!ParseHtml(strInnerHtml, lPVoid))
			break;
		
		hRet = S_OK;

// 		std::vector<WORD> vPosThisFlord;
// 		CFindElementByPos::EnumDocumentInfo(pNode, lPVoid, vPosThisFlord);

	} while (FALSE);
	return hRet;
}

HRESULT CParseHomezjcq::FindElement(IHTMLDocument2* pDoc, CElementInfo* out_ElementInfo, CCondition* in_Condition)
{
	return S_OK;
}


HRESULT CParseHomezjcq::init()
{
	DWORD pos[] = {1,1,1,1,0,0,0,1,0,0,0};
	std::vector<DWORD> vet(pos, pos + 10);
	_condition.vElementPos = vet;
	return S_OK;
}

HRESULT CParseHomezjcq::destory()
{
	return S_OK;
}

BOOL CParseHomezjcq::ParseText(LPCWSTR lpszText, LPVOID lPvoid)
{
	BOOL bRet = FALSE;
	do 
	{
		std::string s = (char*)lpszText;
// 		while (*pChar != '0')
// 		{
// 			char Temp = *pChar;
// 			pChar ++;
// 			OutputDebugStr(lpszText);
// 		};
		if (!lpszText || !lPvoid)
			break;
		std::vector<CString> vString;
		AdvFunction::SplitString(lpszText, L"   ", &vString);


	} while (FALSE);
	return bRet;
}

BOOL CParseHomezjcq::ParseHtml(LPCWSTR lpszHtml, LPVOID lPvoid)
{
	BOOL bRet = FALSE;
	
	do 
	{
		if (!lpszHtml || !lPvoid)
			break;
		std::vector<CString> vString;
		AdvFunction::SplitString(lpszHtml, L"\n", &vString);


	} while (FALSE);

	return bRet;
}