#pragma once

#include <mfapi.h>
#include <mfidl.h>
#include <mferror.h>

#include <windows.h>
#include <atlbase.h>//For CComPtr

#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mf.lib")
#pragma comment(lib, "mfuuid.lib")

class M4ATranscoder
{
public:
   M4ATranscoder();
   ~M4ATranscoder();

   bool Transcode(WCHAR* pstrInput, WCHAR* pstrOutput);
   int  GetEncodingProgress();
   void CancelTranscode();

protected:
   HRESULT ConfigureOutput(WCHAR* pstrOutput, CComPtr<IMFStreamDescriptor> stream_desc,
      CComPtr<IMFTopology>& topology);
   void SetSourceDuration();
   void SetPresentationClock();

protected:
   CComPtr<IMFMediaSession> m_MediaSession;
   CComQIPtr<IMFMediaSource> m_Source;
   CComPtr<IMFPresentationClock> m_Clock;
   MFTIME m_SourceDuration;
   bool m_Canceling;
};
