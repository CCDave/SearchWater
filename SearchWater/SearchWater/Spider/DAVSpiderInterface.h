#ifndef _DAV_SPIDER_INTERFACE_
#define _DAV_SPIDER_INTERFACE_

struct IDAVSpiderInterface
{
	STDMETHOD(SpiderWork)(UINT uMsg, LPCWSTR lpszUrl, LPVOID lPvoid, UINT uTimeOut = 5000) PURE;
	STDMETHOD(SetHostWindow)(HWND hWnd) PURE;
	virtual HANDLE GetFinishHandle() PURE;
};

const UINT GET_HOME_PAGE_ZJCQ_DADA = 10000;

typedef struct _HTML_NODE_ENUM_INFO_
{
	CString strInfo;
	std::vector<WORD> vPos;
} SNode, *PSNode;

typedef std::vector<PSNode>* PVectorHtmlInfo;
typedef std::vector<PSNode> VectorHtmlInfo;

typedef struct _MAIN_MATCH_INFO_ : public _HTML_NODE_ENUM_INFO_
{
	CString strNum;
	CString strType;
	CString strTime;
	CString strAllInfo;
	CString	strTeamInfo;
	CString strMainOdds;
	CString strUUrl;
} SMatchInfo, *PSMatchInfo;

typedef std::vector<PSMatchInfo>* PVMatchInfo;
typedef std::vector<PSMatchInfo> VMatchInfo;
#endif

