#ifndef INTERFACE_GET_DOCUMENT_
#define INTERFACE_GET_DOCUMENT_
#include "DAVSpiderInterface.h"
/*
* Http�ĵ���������URL��ȡ��Ӧ���ĵ���
*/
struct IHttpDocumentProxy
{
	STDMETHOD(GetHttpDocument)(LPCWSTR lpszUrl, UINT uTimeOut /*��ʱʱ��*/, IDispatch** ppDoc) PURE;
	STDMETHOD(SetHostWindow)(HWND hWnd) PURE;
	STDMETHOD(SetFinishEvent)(HANDLE hEvent) PURE;
	STDMETHOD(IsFinish)() PURE;
	STDMETHOD(SetCallBack)(LPVOID) PURE;
};
#endif

/* 
* http �ĵ�������������ݲ�ͬ��http ����������
*/
#ifndef INTERFACE_HTTP_DOCUMENT_PARSE_
#define INTERFACE_HTTP_DOCUMENT_PARSE_

struct IHttpDocumentParse
{
	STDMETHOD(ParseHttpDocument)(IHTMLDocument2* pDoc, LPVOID lPvoid /*�����������*/) PURE;
};
#endif


