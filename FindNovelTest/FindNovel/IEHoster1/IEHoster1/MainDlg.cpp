// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "MainDlg.h"
#include "BrowserEvent.h"



CMainDlg::CMainDlg() : 
    m_uDownloadEvents(0), m_nLogBeforeNav(1), m_nLogNavComplete(1),
    m_nLogStatusTextChange(1), m_nLogDownloadBegin(1),
    m_nLogDownloadComplete(1), m_nLogCmdStateChange(1)
{
}

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->hwnd == GetDlgItem(IDC_EDIT1))
	{
		if(pMsg->message == WM_KEYDOWN)
		{
			if(pMsg->wParam  == VK_RETURN)
			{
				CComVariant v;  
				if ( !DoDataExchange(true) )
					return 0;
					 m_pWB2->Navigate ( CComBSTR(m_strAddress), &v, &v, &v, &v );
			}
		}
	}
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
	UIUpdateChildWindows();
	return FALSE;
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();
		//resize、初始化
	DlgResize_Init();
	//刷新DDX 
	DoDataExchange(false);
	// set icons
	HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);
   
	UIAddChildWindowContainer(m_hWnd);
	
//	OleInitialize(NULL);

	//HRESULT hr = S_FALSE;
	/*CoCreateInstance(CLSID_InternetExplorer, NULL, CLSCTX_LOCAL_SERVER ,  
		IID_IWebBrowser2, (void**)&m_pWB2);*/
	
	/* hr = CoCreateInstance(CLSID_WebBrowser, NULL, CLSCTX_INPROC,
		IID_IOleObject, (void**)&m_spOleObjectPreloadBrowser);
	m_spOleObjectPreloadBrowser->QueryInterface(IID_IWebBrowser2, (void**)&m_pWB2);*/

	//为了明确起见，省略错误检查
	//CComPtr<IOleObject> spOleObj;
	//CActiveXCtrl* pact = new CActiveXCtrl;
	//创建 WebBrowser--在类成员变量 m_spWebBrowser中保存指针
	//CoCreateInstance(CLSID_WebBrowser, NULL, CLSCTX_INPROC, IID_IWebBrowser2, (void**)&m_pWB2);
	// 查询WebBrowser的IOleObject接口
	//m_pWB2->QueryInterface(IID_IOleObject, (void**)&spOleObj);
	//设置用户站点
	//spOleObj->SetClientSite(this);
	//本地激活浏览器控件
	//RECT rcClient;
	//	GetClientRect(&rcClient);
	//spOleObj->DoVerb(OLEIVERB_INPLACEACTIVATE, NULL, NULL, 0, GetTopLevelWindow(), &rcClient);
	//容器拦截浏览器事件的注册
	//DWORD  m_dwCookie = 0;
	//AtlAdvise(m_pWB2,GetUnknown(), DIID_DWebBrowserEvents2,&m_dwCookie);
	//导航到启动页
	//m_pWB2->Navigate(L"res://webhost.exe/startpage.htm", NULL, NULL, NULL, NULL);	


	//设置web 地址
	CAxWindow wndIE = GetDlgItem(IDC_IE);
	HRESULT hr;
	hr = wndIE.QueryControl ( &m_pWB2 );

	//容器拦截浏览器事件的注册'
	////////连接点挂接事件
	/////////////////////////////
	LPVOID lpvoid;
	//
	/////找到IConnectionPointContainer
	hr = m_pWB2->QueryInterface(IID_IConnectionPointContainer, (void**)&lpvoid);

	IConnectionPointContainer* pConnect = (IConnectionPointContainer*)lpvoid;

	IEnumConnectionPoints* ppEnum = NULL;

	IConnectionPoint* ppConncetPt = NULL;
	edt = GetDlgItem(IDC_EDIT_TEXT);
	CBrowserEvent* pevent = new CBrowserEvent;
	pevent->SetEdit(&edt);
	pevent->SetSite((LPVOID)m_pWB2);
	pevent->SetUrl(&m_strAddress);

	///获取事件连接点
	hr = pConnect->FindConnectionPoint(DIID_DWebBrowserEvents2, &ppConncetPt);

	DWORD dwCookie = 0; 

	///挂接连接器 CSetEvent
	hr = ppConncetPt->Advise((IUnknown*)(void*)pevent,&dwCookie);

	//DispEventAdvise( m_pWB2);
	if ( m_pWB2 )
    {
		CComVariant v(VT_I4);
		CComVariant v1;
		CComVariant v2;
		CComVariant v3;
		/*hr = m_pWB2->Navigate ( CComBSTR("www.baidu.com"),
						&v, &v1, &v2, &v3 );*/
    }
	//m_pWB2->put_Visible(VARIANT_TRUE);
	m_strAddress = _T("about:mozilla");


	m_wndEdit.SetWindowTextW(m_strAddress);

	//按钮图片加载
	m_btnBack, m_btnFwd, m_btnStop, m_btnReload;
	CImageList iml;

    iml.CreateFromImage ( IDB_BACK, 17, 1, CLR_NONE, IMAGE_BITMAP, 
                          LR_LOADTRANSPARENT | LR_LOADMAP3DCOLORS | LR_CREATEDIBSECTION );
    m_btnBack.SetImageList ( iml.Detach() );
    m_btnBack.SetImages ( 0, 3, 1, 2 );
    m_btnBack.SetBitmapButtonExtendedStyle ( BMPBTN_HOVER, BMPBTN_HOVER );

    iml.CreateFromImage ( IDB_FORWORD, 17, 1, CLR_NONE, IMAGE_BITMAP, 
                          LR_LOADTRANSPARENT | LR_LOADMAP3DCOLORS | LR_CREATEDIBSECTION );
    m_btnFwd.SetImageList ( iml.Detach() );
    m_btnFwd.SetImages ( 0, 3, 1, 2 );
    m_btnFwd.SetBitmapButtonExtendedStyle ( BMPBTN_HOVER, BMPBTN_HOVER );

    iml.CreateFromImage ( IDB_STOP, 17, 1, CLR_NONE, IMAGE_BITMAP, 
                          LR_LOADTRANSPARENT | LR_LOADMAP3DCOLORS | LR_CREATEDIBSECTION );
    m_btnStop.SetImageList ( iml.Detach() );
    m_btnStop.SetImages ( 0, 3, 1, 2 );
    m_btnStop.SetBitmapButtonExtendedStyle ( BMPBTN_HOVER, BMPBTN_HOVER );

    iml.CreateFromImage ( IDB_RELOAD, 17, 1, CLR_NONE, IMAGE_BITMAP, 
                          LR_LOADTRANSPARENT | LR_LOADMAP3DCOLORS | LR_CREATEDIBSECTION );
    m_btnReload.SetImageList ( iml.Detach() );
    m_btnReload.SetImages ( 0, 3, 1, 2 );
    m_btnReload.SetBitmapButtonExtendedStyle ( BMPBTN_HOVER, BMPBTN_HOVER );


	return TRUE;
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	  if ( m_pWB2 )
        {
        m_pWB2.Release();
        }

    SetMsgHandled(false);

	return 0;
}

LRESULT CMainDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}

LRESULT CMainDlg::OnBnClickedButton1(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{	
	
	CComVariant v;
	HRESULT hr = S_FALSE;
    if ( !DoDataExchange(true) )
        return 0;

    hr = m_pWB2->Navigate ( CComBSTR(m_strAddress), &v, &v, &v, &v );

	return 0;
}

LRESULT CMainDlg::OnBnClickedButton5(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	m_pWB2->GoForward();
	return 0;
}

LRESULT CMainDlg::OnBnClickedButton4(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	m_pWB2->GoBack();
	return 0;
}

LRESULT CMainDlg::OnBnClickedButton6(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	m_pWB2->Stop();
	return 0;
}

LRESULT CMainDlg::OnBnClickedButton7(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	CComVariant vLevel;

    if ( GetKeyState(VK_SHIFT) & 0x8000 )
        vLevel = REFRESH_COMPLETELY;
    else
        vLevel = REFRESH_IFEXPIRED;
    m_pWB2->Refresh2 (&vLevel);
	return 0;
}



