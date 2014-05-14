// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CMainDlg : public CAxDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
	public CMessageFilter, public CIdleHandler,
	public CWinDataExchange<CMainDlg>,
	public CDialogResize<CMainDlg>
{
public:

	CMainDlg();
	enum { IDD = IDD_MAINDLG };
	
	CEdit edt;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();
	// UI 刷新宏
	BEGIN_UPDATE_UI_MAP(CMainDlg)
		UPDATE_ELEMENT(IDC_BUTTON4, UPDUI_CHILDWINDOW)
        UPDATE_ELEMENT(IDC_BUTTON5, UPDUI_CHILDWINDOW)
	END_UPDATE_UI_MAP()
	
	// resize 宏
	BEGIN_DLGRESIZE_MAP(CMainDlg)
        DLGRESIZE_CONTROL(IDC_EDIT1, DLSZ_SIZE_X)
        DLGRESIZE_CONTROL(IDC_BUTTON1, DLSZ_MOVE_X)
     
      
        DLGRESIZE_CONTROL(IDC_IE, DLSZ_SIZE_X|DLSZ_SIZE_Y)
        DLGRESIZE_CONTROL(IDC_REORT, DLSZ_SIZE_X|DLSZ_MOVE_Y)
        DLGRESIZE_CONTROL(IDC_EDIT_TEXT, DLSZ_SIZE_X|DLSZ_MOVE_Y)
        DLGRESIZE_CONTROL(IDC_LOG_BEFORENAV, DLSZ_MOVE_Y)
        DLGRESIZE_CONTROL(IDC_LOG_NAVCOMPLETE, DLSZ_MOVE_Y)
        DLGRESIZE_CONTROL(IDC_LOG_STATUSCHG, DLSZ_MOVE_Y)
        DLGRESIZE_CONTROL(IDC_LOG_DOWNLOADBEGIN, DLSZ_MOVE_Y)
        DLGRESIZE_CONTROL(IDC_LOG_DOWNLOADEND, DLSZ_MOVE_Y)
        DLGRESIZE_CONTROL(IDC_LOG_CMDSTATECHG, DLSZ_MOVE_Y)
    END_DLGRESIZE_MAP()
	// 消息宏
	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_HANDLER(IDC_BUTTON1, BN_CLICKED, OnBnClickedButton1)
		COMMAND_HANDLER(IDC_BUTTON5, BN_CLICKED, OnBnClickedButton5)
		COMMAND_HANDLER(IDC_BUTTON4, BN_CLICKED, OnBnClickedButton4)
		COMMAND_HANDLER(IDC_BUTTON6, BN_CLICKED, OnBnClickedButton6)
		COMMAND_HANDLER(IDC_BUTTON7, BN_CLICKED, OnBnClickedButton7)
		CHAIN_MSG_MAP(CUpdateUI<CMainDlg>)
		CHAIN_MSG_MAP(CDialogResize<CMainDlg>)
	END_MSG_MAP()

	//DDX 控制宏
	 BEGIN_DDX_MAP(CMainDlg)
	   DDX_CONTROL(IDC_REORT, m_wndIEStatus)
	   DDX_CONTROL(IDC_WAITING, m_wndWait)
       DDX_CONTROL(IDC_EDIT1, m_wndEdit)
	   DDX_CONTROL(IDC_BUTTON4, m_btnBack)
	   DDX_CONTROL(IDC_BUTTON5, m_btnFwd)
	   DDX_CONTROL(IDC_BUTTON6, m_btnStop)
	   DDX_CONTROL(IDC_BUTTON7, m_btnReload)
	   DDX_TEXT(IDC_EDIT1, m_strAddress)
		DDX_CHECK(IDC_LOG_BEFORENAV, m_nLogBeforeNav)
        DDX_CHECK(IDC_LOG_NAVCOMPLETE, m_nLogNavComplete)
        DDX_CHECK(IDC_LOG_STATUSCHG, m_nLogStatusTextChange)
        DDX_CHECK(IDC_LOG_DOWNLOADBEGIN, m_nLogDownloadBegin)
        DDX_CHECK(IDC_LOG_DOWNLOADEND, m_nLogDownloadComplete)
        DDX_CHECK(IDC_LOG_CMDSTATECHG, m_nLogCmdStateChange)
    END_DDX_MAP()

//  Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	void CloseDialog(int nVal);



protected:
	//web接口
	CComPtr<IWebBrowser2> m_pWB2;
	CComPtr<IOleObject> m_spOleObjectPreloadBrowser;
	//地址
	CString m_strAddress;
	////各种显示控件
	CStaticImpl m_wndIEStatus;
	CStaticImpl m_wndWait;
	CEditImpl m_wndEdit;

	CBitmapButton m_btnBack, m_btnFwd, m_btnStop, m_btnReload;
	
	int m_uDownloadEvents;

	int m_nLogBeforeNav, m_nLogNavComplete, m_nLogStatusTextChange,
        m_nLogDownloadBegin, m_nLogDownloadComplete, m_nLogCmdStateChange;
public:
	LRESULT OnBnClickedButton1(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButton5(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButton4(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButton6(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButton7(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButton2(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButton3(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
