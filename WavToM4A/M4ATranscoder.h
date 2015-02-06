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
   void Init(WCHAR* pstrInput, WCHAR* pstrOutput);
   bool Transcode(IM4AProgress* pProgress);
   double GetEncodingProgress();

protected:
   HRESULT ConfigureOutput(CComPtr<IMFStreamDescriptor> stream_desc);
   void SetSourceDuration();
   void SetPresentationClock();
   void SetOutputFormats();

protected:
   WCHAR* m_pstrOutput;
   CComPtr<IMFTopology> m_Topology;
   CComPtr<IMFMediaSession> m_MediaSession;
   CComQIPtr<IMFMediaSource> m_Source;
   CComPtr<IMFPresentationClock> m_Clock;
   MFTIME m_SourceDuration;
   IM4AProgress* m_pProgress;
};
