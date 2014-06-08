#ifndef _INTERFACE_BROWSER_CALL_BACK_
#define _INTERFACE_BROWSER_CALL_BACK_
//浏览器异步执行完成的回调
struct IBrowserCallBack
{
	STDMETHOD(BrowserCallBack)(IDispatch* pObject, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandle) PURE;
};
#endif