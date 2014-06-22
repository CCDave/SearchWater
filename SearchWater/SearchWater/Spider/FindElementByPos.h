#pragma once
#include "IFindElement.h"

class CFindElementByPos : 
				public IFindElement

{
public:
	CFindElementByPos(void);
	virtual ~CFindElementByPos(void);

	virtual HRESULT FindElement(IHTMLDocument2* pDoc, CElementInfo* out_ElementInfo, CCondition* in_Condition);
	virtual HRESULT FindElementByPos(IHTMLDocument2* pDoc, CElementInfo* out_ElementInfo, CCondition* in_Condition);

	static void EnumDocumentInfo(IHTMLDOMNode* pNode, LPVOID lPvoid, std::vector<WORD> vPos);
	static CComQIPtr<IHTMLDOMChildrenCollection> CFindElementByPos::EnumIHTMLDOMNode(IHTMLDOMNode* pNode);
protected:
	void FindByPos(IHTMLDOMNode* pNode, LPVOID lPvoid, std::vector<DWORD> vPos);
	
	std::vector<PSNode> m_vHtmlNodeInfo;
	std::vector<WORD> m_vInfPos;
	CComPtr<IHTMLElement> m_FindpElement;
};
