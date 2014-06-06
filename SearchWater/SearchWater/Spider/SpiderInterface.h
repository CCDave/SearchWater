#ifndef INTERFACE_GET_DOCUMENT_
#define INTERFACE_GET_DOCUMENT_

/*
* Http�ĵ���������URL��ȡ��Ӧ���ĵ���
*/
struct IHttpDocumentProxy
{
	STDMETHOD(GetHttpDocument)(LPCWSTR lpszUrl, UINT uTimeOut /*��ʱʱ��*/, IDispatch** ppDoc) PURE;
};
#endif

/* 
* http �ĵ�������������ݲ�ͬ��http ����������
*/
#ifndef INTERFACE_HTTP_DOCUMENT_PARSE_
#define INTERFACE_HTTP_DOCUMENT_PARSE_

template <class T>
struct IHttpDocumentParse
{
	STDMETHOD(ParseHttpDocument)(IHTMLDocument2* pDoc, T& container) PURE;
};
#endif