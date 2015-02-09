#pragma once

#include <vector>

#include <mfapi.h>
#include <mfidl.h>
#include <mferror.h>

#include <windows.h>
#include <atlbase.h>//For CComPtr
#include <atlstr.h>//For CString

#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mf.lib")
#pragma comment(lib, "mfuuid.lib")

class IM4AProgress;
class M4ATranscoder
{
public:
   void Init(WCHAR* pstrInput, WCHAR* pstrOutput);
   bool Transcode(IM4AProgress* pProgress);
   const std::vector<WAVEFORMATEX>& GetOutputFormats();
   void SetOutputFormatIndex(int index);
   double GetEncodingProgress();

protected:
   HRESULT ConfigureOutput();
   void BuildSourceDuration();
   void BuildPresentationClock();
   void BuildOutputFormats();

protected:
   ATL::CString m_strOutput;
   CComPtr<IMFTopology> m_Topology;
   CComPtr<IMFMediaSession> m_MediaSession;
   CComQIPtr<IMFMediaSource> m_Source;
   CComPtr<IMFPresentationClock> m_Clock;
   CComPtr<IMFStreamDescriptor> m_StreamDesc;
   CComPtr<IMFTranscodeProfile> m_TranscodeProfile;
   CComPtr<IMFCollection> m_AvailableOutputTypes;

   std::vector<WAVEFORMATEX> m_aOutputFormats;
   int m_FormatIndex = 0;
   MFTIME m_SourceDuration = 0LL;
   IM4AProgress* m_pProgress = NULL;
};
