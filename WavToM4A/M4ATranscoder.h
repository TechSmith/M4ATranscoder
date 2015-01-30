#pragma once

#include <mfapi.h>
#include <mfidl.h>
#include <mferror.h>

#include <windows.h>
#include <atlbase.h>//For CComPtr
//#include <atlstr.h>
#include <atlcom.h>
//#include <atlwin.h>

#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mf.lib")
#pragma comment(lib, "mfuuid.lib")

class M4ATranscoder
   : public CComObjectRootEx<CComMultiThreadModel>, public IMFAsyncCallback
{
public:
   M4ATranscoder();

   BEGIN_COM_MAP(M4ATranscoder)
      COM_INTERFACE_ENTRY(IMFAsyncCallback)
   END_COM_MAP()
   const static unsigned WM_MF_EVENT = WM_APP + 1;

   DECLARE_PROTECT_FINAL_CONSTRUCT();
   //BEGIN_MSG_MAP(M4ATranscoder)
      //MESSAGE_HANDLER(WM_CREATE, OnCreate)
      //MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
      //MESSAGE_HANDLER(WM_MF_EVENT, OnMFEvent)
   //END_MSG_MAP()

   HRESULT CreateMediaSrc();
   HRESULT ConfigureOutput(CComPtr<IMFStreamDescriptor> stream_desc,
      CComPtr<IMFTopology>& topology);

public: // IMFAsyncCallback
   HRESULT STDMETHODCALLTYPE GetParameters(
      /* [out] */ __RPC__out DWORD *pdwFlags,
      /* [out] */ __RPC__out DWORD *pdwQueue);
   HRESULT STDMETHODCALLTYPE Invoke(
      /* [in] */ __RPC__in_opt IMFAsyncResult *pAsyncResult);

   static LPCOLESTR GetAppId() throw()
   {
      return OLESTR("");
   }

   static void InitLibId() throw()
   {
   }

protected:
   CComPtr<IMFMediaSession> m_MediaSession;
   CComQIPtr<IMFMediaSource> m_Source;

   CHandle m_Handle;
};