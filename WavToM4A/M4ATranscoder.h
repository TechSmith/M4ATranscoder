#pragma once

#include <mfapi.h>
#include <mfidl.h>
#include <mferror.h>

#include <windows.h>
#include <atlbase.h>//For CComPtr

#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mf.lib")
#pragma comment(lib, "mfuuid.lib")

class IM4AProgress;
class M4ATranscoder
{
public:
   bool Transcode(WCHAR* pstrInput, WCHAR* pstrOutput, IM4AProgress* pProgress);

protected:
   HRESULT ConfigureOutput(WCHAR* pstrOutput, CComPtr<IMFStreamDescriptor> stream_desc,
      CComPtr<IMFTopology>& topology);

protected:
   CComPtr<IMFMediaSession> m_MediaSession;
   CComQIPtr<IMFMediaSource> m_Source;
   IM4AProgress* m_pProgress;
};
