#ifndef _DAV_SPIDER_INTERFACE_
#define _DAV_SPIDER_INTERFACE_

struct IDAVSpiderInterface
{
	STDMETHOD(SpiderWork)(UINT uMsg, LPCWSTR lpszUrl, LPVOID lPvoid, UINT uTimeOut = 5000) PURE;
	STDMETHOD(SetHostWindow)(HWND hWnd) PURE;
	virtual HANDLE GetFinishHandle() PURE;
};
#endif

const UINT GET_HOME_PAGE_ZJCQ_DADA = 10000;