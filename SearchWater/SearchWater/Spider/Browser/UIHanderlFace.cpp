#include "StdAfx.h"
#include "UIHanderlFace.h"


#include <mshtmcid.h>

HRESULT ModifyContextMenu(DWORD dwID, POINT *ppt, IUnknown *pcmdtReserved);
HRESULT CustomContextMenu(POINT *ppt, IUnknown *pcmdtReserved);

CUIHanderlFace::CUIHanderlFace()
{
    m_contextMenuMode			= kDefaultMenuSupport;
    m_cRef						= 0;
}

CUIHanderlFace::~CUIHanderlFace()
{
    //We release our default interfaces
    this->SetDefaultClientSite(NULL);
}

VOID CUIHanderlFace::SetDefaultClientSite(IOleClientSite *pClientSite)
{
    if (pClientSite != NULL)
    {
        m_defaultClientSite = pClientSite;
        m_defaultClientSite->QueryInterface(IID_IDocHostUIHandler, (VOID **)&m_defaultDocHostUIHandler);
    }
}

VOID CUIHanderlFace::SetContextMenuMode(ULONG inMode)
{
    ATLASSERT(inMode < kWebContextMenuModeLimit);

    if (inMode < kWebContextMenuModeLimit)
        m_contextMenuMode = inMode;
}

// *** IUnknown ***

HRESULT STDMETHODCALLTYPE CUIHanderlFace::QueryInterface(REFIID riid, LPVOID *ppv) 
{
    HRESULT result = S_OK;

    // Always set out parameter to NULL, validating it first 
    if (IsBadWritePtr(ppv, sizeof(LPVOID))) 
        result = E_INVALIDARG;  

    if (result == S_OK)
    {
        *ppv = NULL; 

        if ( IsEqualIID( riid, IID_IUnknown ) )
            *ppv = this;
        else if ( IsEqualIID( riid, IID_IOleClientSite ) )
            *ppv = (IOleClientSite *) this;
        else if ( IsEqualIID( riid, IID_IDocHostUIHandler ) )
            *ppv = (IDocHostUIHandler *) this;
        else
            result = E_NOINTERFACE;
    }

    if (result == S_OK)
        this->AddRef(); 

    return result; 
}

ULONG STDMETHODCALLTYPE CUIHanderlFace::AddRef() 
{    
    InterlockedIncrement(&m_cRef); 
    return m_cRef; 
} 

ULONG STDMETHODCALLTYPE CUIHanderlFace::Release() 
{ 
    // Decrement the object's internal counter 
    ULONG ulRefCount = InterlockedDecrement(&m_cRef); 

    if (0 == m_cRef) 
    {
        delete this; 
    }

    return ulRefCount; 
} 

// *** IOleClientSite ***

HRESULT STDMETHODCALLTYPE CUIHanderlFace::SaveObject()
{
    if (m_defaultClientSite != NULL)
        return m_defaultClientSite->SaveObject();
    else
        return E_FAIL;
}

HRESULT STDMETHODCALLTYPE CUIHanderlFace::GetMoniker(DWORD dwAssign, 
    DWORD dwWhichMoniker, LPMONIKER *ppmk)
{
    if (m_defaultClientSite != NULL)
        return m_defaultClientSite->GetMoniker(dwAssign, dwWhichMoniker, ppmk);
    else
    {
        if (! IsBadWritePtr(ppmk, sizeof(*ppmk)))
            *ppmk = NULL;

        return E_NOTIMPL;
    }
}

HRESULT STDMETHODCALLTYPE CUIHanderlFace::GetContainer(LPOLECONTAINER *ppContainer)
{	
    if (m_defaultClientSite != NULL)
        return m_defaultClientSite->GetContainer(ppContainer);
    else
    {
        if (! IsBadWritePtr(ppContainer, sizeof(*ppContainer)))
            *ppContainer = NULL;

        return E_NOINTERFACE;
    }
}

HRESULT STDMETHODCALLTYPE CUIHanderlFace::ShowObject()
{
    if (m_defaultClientSite != NULL)
        return m_defaultClientSite->ShowObject();
    else
        return S_OK;
}

HRESULT STDMETHODCALLTYPE CUIHanderlFace::OnShowWindow(BOOL fShow)
{
    if (m_defaultClientSite != NULL)
        return m_defaultClientSite->OnShowWindow(fShow);
    else
        return S_OK;
}

HRESULT STDMETHODCALLTYPE CUIHanderlFace::RequestNewObjectLayout()
{
    if (m_defaultClientSite != NULL)
        return m_defaultClientSite->RequestNewObjectLayout();
    else
        return E_NOTIMPL;
}

// *** IDocHostUIHandler ***

HRESULT STDMETHODCALLTYPE CUIHanderlFace::ShowContextMenu(DWORD dwID, POINT *ppt, 
    IUnknown *pcmdtReserved, IDispatch *pdispReserved)
{

    ::MessageBox(NULL,_T("ShowContextMenu"),_T("ShowContextMenu"),MB_OK);
    return S_OK;

    HRESULT result	= S_FALSE; //Dont Interfere
    BOOL	handled	= FALSE;

    switch ( m_contextMenuMode )
    {
    case kDefaultMenuSupport:
        break;

    case kNoContextMenu:
        result	= S_OK;
        handled	= TRUE;
        break;

    case kTextSelectionOnly:
        if (dwID != CONTEXT_MENU_TEXTSELECT)
        {
            result	= S_OK;
            handled	= TRUE;
        }
        break;

    case kAllowAllButViewSource:
        if (dwID == CONTEXT_MENU_DEFAULT)
        {
            result	= ModifyContextMenu(dwID, ppt, pcmdtReserved);
            handled	= TRUE;
        }
        break;

    case kCustomMenuSupport:
        if (dwID == CONTEXT_MENU_DEFAULT)
        {
            result = CustomContextMenu(ppt, pcmdtReserved);
            handled	= TRUE;
        }
        break;
    }

    if (! handled)
    {
        if (m_defaultDocHostUIHandler != NULL)
            result = m_defaultDocHostUIHandler->ShowContextMenu(dwID, ppt, 
            pcmdtReserved, pdispReserved);
        else
            result = S_FALSE;
    }

    return result;
}

HRESULT STDMETHODCALLTYPE CUIHanderlFace::GetHostInfo(DOCHOSTUIINFO *pInfo)
{
//     if (m_defaultDocHostUIHandler != NULL)
//         return m_defaultDocHostUIHandler->GetHostInfo(pInfo);
//     else
//         return E_NOTIMPL;

	if (pInfo)
	{
		if (m_defaultDocHostUIHandler)
		{
			m_defaultDocHostUIHandler->GetHostInfo(pInfo);
//			pInfo->dwFlags = pInfo->dwFlags | DOCHOSTUIFLAG_SCROLL_NO;

// 			LPTSTR css =_T("\
// 						   body {color:red;word-break:break-all;}\
// 						   ");
			LPTSTR css =_T("\
						   BODY {\
						   SCROLLBAR-FACE-COLOR: #daeef3; MARGIN: 5px; SCROLLBAR-HIGHLIGHT-=\
COLOR: #96bfd3; SCROLLBAR-SHADOW-COLOR: #96bfd3; SCROLLBAR-3DLIGH=\
	   T-COLOR: #ffffff; SCROLLBAR-ARROW-COLOR: #96bfd3; SCROLLBAR-TRACK=\
	   -COLOR: #ffffff; SCROLLBAR-DARKSHADOW-COLOR: #ffffff; BACKGROUND-=\
COLOR: #ffffff=0D=0A}\
						   ");
			pInfo->pchHostCss = (LPTSTR)::CoTaskMemAlloc( (_tcslen(css)+1)*sizeof(TCHAR) );
			_tcscpy(pInfo->pchHostCss,css);
		}
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CUIHanderlFace::ShowUI(DWORD dwID, 
    IOleInPlaceActiveObject *pActiveObject, IOleCommandTarget *pCommandTarget,
    IOleInPlaceFrame *pFrame, IOleInPlaceUIWindow *pDoc)
{
    if (m_defaultDocHostUIHandler != NULL)
        return m_defaultDocHostUIHandler->ShowUI(dwID, pActiveObject, 
        pCommandTarget, pFrame, pDoc);
    else
        return S_FALSE;
}

HRESULT STDMETHODCALLTYPE CUIHanderlFace::HideUI()
{
    if (m_defaultDocHostUIHandler != NULL)
        return m_defaultDocHostUIHandler->HideUI();
    else
        return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CUIHanderlFace::UpdateUI()
{
    if (m_defaultDocHostUIHandler != NULL)
        return m_defaultDocHostUIHandler->UpdateUI();
    else
        return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CUIHanderlFace::EnableModeless(BOOL fEnable)
{
    if (m_defaultDocHostUIHandler != NULL)
        return m_defaultDocHostUIHandler->EnableModeless(fEnable);
    else
        return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CUIHanderlFace::OnDocWindowActivate(BOOL fActivate)
{
    if (m_defaultDocHostUIHandler != NULL)
        return m_defaultDocHostUIHandler->OnDocWindowActivate(fActivate);
    else
        return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CUIHanderlFace::OnFrameWindowActivate(BOOL fActivate)
{
    if (m_defaultDocHostUIHandler != NULL)
        return m_defaultDocHostUIHandler->OnFrameWindowActivate(fActivate);
    else
        return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CUIHanderlFace::ResizeBorder(LPCRECT prcBorder,
    IOleInPlaceUIWindow __RPC_FAR *pUIWindow, BOOL fRameWindow)
{
    if (m_defaultDocHostUIHandler != NULL)
        return m_defaultDocHostUIHandler->ResizeBorder(prcBorder, pUIWindow, fRameWindow);
    else
        return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CUIHanderlFace::TranslateAccelerator(LPMSG lpMsg,
    const GUID *pguidCmdGroup, DWORD nCmdID)
{
    if (m_defaultDocHostUIHandler != NULL)
        return m_defaultDocHostUIHandler->TranslateAccelerator(lpMsg, pguidCmdGroup, nCmdID);
    else
        return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CUIHanderlFace::GetOptionKeyPath( 
    LPOLESTR *pchKey, DWORD dw)
{
    if (m_defaultDocHostUIHandler != NULL)
        return m_defaultDocHostUIHandler->GetOptionKeyPath(pchKey, dw);
    else
    {
        if (! IsBadWritePtr(pchKey, sizeof(*pchKey))) 
            *pchKey = NULL;

        return E_NOTIMPL;
    }
}

HRESULT STDMETHODCALLTYPE CUIHanderlFace::GetDropTarget( 
    IDropTarget *pDropTarget, IDropTarget **ppDropTarget)
{
    if (m_defaultDocHostUIHandler != NULL)
    {
        HRESULT result = m_defaultDocHostUIHandler->GetDropTarget(pDropTarget, ppDropTarget);

        //Returning S_FALSE seems to disable DragNDrop, while DragNDrop is by default on.
        //Changing return code to E_FAIL seems to fix things. 
        if (result == S_FALSE)
            result = E_FAIL;

        return result;
    }
    else
    {
        if (! IsBadWritePtr(ppDropTarget, sizeof(*ppDropTarget))) 
            *ppDropTarget = NULL;

        return E_NOTIMPL;
    }
}

HRESULT STDMETHODCALLTYPE CUIHanderlFace::GetExternal(IDispatch **ppDispatch)
{
    if (m_defaultDocHostUIHandler != NULL)
        return m_defaultDocHostUIHandler->GetExternal(ppDispatch);
    else
    {
        if (! IsBadWritePtr(ppDispatch, sizeof(*ppDispatch))) 
            *ppDispatch = NULL;

        return E_NOTIMPL;
    }
}

HRESULT STDMETHODCALLTYPE CUIHanderlFace::TranslateUrl( 
    DWORD dwTranslate, OLECHAR *pchURLIn, OLECHAR **ppchURLOut)
{
    if (m_defaultDocHostUIHandler != NULL)
        return m_defaultDocHostUIHandler->TranslateUrl(dwTranslate, pchURLIn, ppchURLOut);
    else
    {
        if (! IsBadWritePtr(ppchURLOut, sizeof(*ppchURLOut))) 
            *ppchURLOut = NULL;

        return S_FALSE;
    }
}

HRESULT STDMETHODCALLTYPE CUIHanderlFace::FilterDataObject( 
    IDataObject *pDO, IDataObject  **ppDORet)
{
    if (m_defaultDocHostUIHandler != NULL)
        return m_defaultDocHostUIHandler->FilterDataObject(pDO, ppDORet);
    else
    {
        if (! IsBadWritePtr(ppDORet, sizeof(*ppDORet))) 
            *ppDORet = NULL;

        return S_FALSE;
    }
}

HRESULT ModifyContextMenu(DWORD dwID, POINT *ppt, IUnknown *pcmdtReserved) 
{
    //#define IDR_BROWSE_CONTEXT_MENU  24641
    //#define IDR_FORM_CONTEXT_MENU    24640
    // #define SHDVID_GETMIMECSETMENU   27
    // #define SHDVID_ADDMENUEXTENSIONS 53
    // 
    //     HRESULT hr;
    //     HINSTANCE hinstSHDOCLC;
    //     HWND hwnd;
    //     HMENU hMenu;
    //     IOleCommandTarget *spCT;
    //     IOleWindow *spWnd;
    //     MENUITEMINFO mii = {0};
    //     VARIANT var, var1, var2;
    // 
    //     hr = pcmdtReserved->QueryInterface(IID_IOleCommandTarget, (void**)&spCT);
    //     hr = pcmdtReserved->QueryInterface(IID_IOleWindow, (void**)&spWnd);
    //     hr = spWnd->GetWindow(&hwnd);
    // 
    //     hinstSHDOCLC = LoadLibrary(TEXT("SHDOCLC.DLL"));
    // 
    //     hMenu = LoadMenu(hinstSHDOCLC,
    //         MAKEINTRESOURCE(IDR_BROWSE_CONTEXT_MENU));
    // 
    //     hMenu = GetSubMenu(hMenu, dwID);
    // 
    //     // Get the language submenu
    //     hr = spCT->Exec(&CGID_ShellDocView, SHDVID_GETMIMECSETMENU, 0, NULL, &var);
    // 
    //     mii.cbSize = sizeof(mii);
    //     mii.fMask  = MIIM_SUBMENU;
    //     mii.hSubMenu = (HMENU) var.byref;
    // 
    //     // Add language submenu to Encoding context item
    //     SetMenuItemInfo(hMenu, IDM_LANGUAGE, FALSE, &mii);
    // 
    //     // Insert Shortcut Menu Extensions from registry
    //     V_VT(&var1) = VT_PTR;
    //     V_BYREF(&var1) = hMenu;
    // 
    //     V_VT(&var2) = VT_I4;
    //     V_I4(&var2) = dwID;
    // 
    //     hr = spCT->Exec(&CGID_ShellDocView, SHDVID_ADDMENUEXTENSIONS, 0, &var1, &var2);
    // 
    //     // Remove View Source
    //     ::DeleteMenu(hMenu, IDM_VIEWSOURCE, MF_BYCOMMAND);
    //     // Remove the item that produces the exta separator
    //     ::DeleteMenu(hMenu, IDM_EXTRA_ITEM, MF_BYCOMMAND);
    // 
    //     // Show shortcut menu
    //     int iSelection = ::TrackPopupMenu(hMenu,
    //         TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD,
    //         ppt->x,
    //         ppt->y,
    //         0,
    //         hwnd,
    //         (RECT*)NULL);
    // 
    //     // Send selected shortcut menu item command to shell
    //     if (iSelection != 0)
    //         LRESULT lr = ::SendMessage(hwnd, WM_COMMAND, iSelection, NULL);
    // 
    //     FreeLibrary(hinstSHDOCLC);
    return S_OK;
}

HRESULT CustomContextMenu(POINT *ppt, IUnknown *pcmdtReserved) 
{
    //     IOleWindow	*oleWnd			= NULL;
    //     HWND		hwnd			= NULL;
    //     HMENU		hMainMenu		= NULL;
    //     HMENU		hPopupMenu		= NULL;
    //     HRESULT		hr				= 0;
    //     INT			iSelection		= 0;
    // 
    //     if ((ppt == NULL) || (pcmdtReserved == NULL))
    //         goto error;
    // 
    //     hr = pcmdtReserved->QueryInterface(IID_IOleWindow, (void**)&oleWnd);
    //     if ( (hr != S_OK) || (oleWnd == NULL))
    //         goto error;
    // 
    //     hr = oleWnd->GetWindow(&hwnd);
    //     if ( (hr != S_OK) || (hwnd == NULL))
    //         goto error;
    // 
    //     hMainMenu = LoadMenu(AfxGetInstanceHandle(),
    //         MAKEINTRESOURCE(IDR_CUSTOM_POPUP));
    //     if (hMainMenu == NULL)
    //         goto error;
    // 
    //     hPopupMenu = GetSubMenu(hMainMenu, 0);
    //     if (hPopupMenu == NULL)
    //         goto error;
    // 
    //     // Show shortcut menu
    //     iSelection = ::TrackPopupMenu(hPopupMenu,
    //         TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD,
    //         ppt->x,
    //         ppt->y,
    //         0,
    //         hwnd,
    //         (RECT*)NULL);
    // 
    //     // Send selected shortcut menu item command to shell
    //     if (iSelection != 0)
    //         (void) ::SendMessage(hwnd, WM_COMMAND, iSelection, NULL);

    // error:
    // 
    //     if (hMainMenu != NULL)
    //         ::DestroyMenu(hMainMenu);

    return S_OK;
}