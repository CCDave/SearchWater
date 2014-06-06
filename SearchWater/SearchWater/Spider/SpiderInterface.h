#ifndef INTERFACE_GET_DOCUMENT_
#define INTERFACE_GET_DOCUMENT_

/*
* Http文档代理，根据URL获取相应的文档。
*/
struct IHttpDocumentProxy
{
	STDMETHOD(GetHttpDocument)(LPCWSTR lpszUrl, UINT uTimeOut /*超时时间*/, IDispatch** ppDoc) PURE;
};
#endif

/* 
* http 文档对象解析，根据不同的http 解析出数据
*/
#ifndef INTERFACE_HTTP_DOCUMENT_PARSE_
#define INTERFACE_HTTP_DOCUMENT_PARSE_

template <class T>
struct IHttpDocumentParse
{
	STDMETHOD(ParseHttpDocument)(IHTMLDocument2* pDoc, T& container) PURE;
};
#endif