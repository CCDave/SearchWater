#ifndef _INTERFACE_BROWSER_CALL_BACK_
#define _INTERFACE_BROWSER_CALL_BACK_
//������첽ִ����ɵĻص�
struct IBrowserCallBack
{
	STDMETHOD(BrowserCallBack)(IDispatch* pObject, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandle) PURE;
};
#endif