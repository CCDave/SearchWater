#include "StdAfx.h"
#include "ParseHomezjcq.h"

CParseHomezjcq::CParseHomezjcq(void)
{
	init();
}

CParseHomezjcq::~CParseHomezjcq(void)
{

}

HRESULT CParseHomezjcq::ParseElement(IHTMLElement* pElement, LPVOID)
{
	return S_OK;
}

HRESULT CParseHomezjcq::FindElement(IHTMLDocument2* pDoc, CElementInfo* out_ElementInfo, CCondition* in_Condition)
{
	return S_OK;
}


HRESULT CParseHomezjcq::init()
{
	DWORD pos[6] = {2, 2, 2, 2, 2, 1};
	std::vector<DWORD> vet(pos, pos + 5);
	_condition.vElementPos = vet;
	return S_OK;
}

HRESULT CParseHomezjcq::destory()
{
	return S_OK;
}