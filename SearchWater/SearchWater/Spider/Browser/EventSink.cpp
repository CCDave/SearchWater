#include "StdAfx.h"
#include "EventSink.h"
#include <exdispid.h>


STDMETHODIMP CEventSink::Invoke(DISPID dispIdMember,REFIID riid,LCID lcid,WORD wFlags,DISPPARAMS *pDispParams,VARIANT *pVarResult,EXCEPINFO *pExcepInfo,UINT *puArgErr)
{
    UNREFERENCED_PARAMETER(lcid);
    UNREFERENCED_PARAMETER(wFlags);
    UNREFERENCED_PARAMETER(pVarResult);
    UNREFERENCED_PARAMETER(pExcepInfo);
    UNREFERENCED_PARAMETER(puArgErr);

    bool b;

    if(!IsEqualIID(riid,IID_NULL)) return DISP_E_UNKNOWNINTERFACE; // riid should always be IID_NULL
    if(dispIdMember==DISPID_DOCUMENTCOMPLETE) { // Handle the DocumentComplete event

        b=Event_DocumentComplete();
    }
    return S_OK;
}

// Return true to prevent the url from being opened
bool CEventSink::Event_DocumentComplete()
{
    //MessageBox(NULL,_T("succeed!!!"),_T("CodeProject BHO Example - BeforeNavigate2 event fired!"),MB_OK|MB_ICONINFORMATION);
    OutputDebugString(_T("HOOKCOM::Get DocumentComplete Event."));
    ::MessageBox(NULL,_T("xx"),_T("xx"),MB_OK);
    return true;
}
