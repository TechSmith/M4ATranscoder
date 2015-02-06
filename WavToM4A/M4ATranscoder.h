#pragma once

#include <vector>

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
   ~M4ATranscoder();

   void Init(WCHAR* pstrInput, WCHAR* pstrOutput);
   bool Transcode(IM4AProgress* pProgress);
   std::vector<WAVEFORMATEX>* GetOutputFormats() { return m_pOutputFormats; }
   void SetOutputFormatIndex(int index) { m_FormatIndex = index; }
   double GetEncodingProgress();

protected:
   HRESULT ConfigureOutput();
   void BuildSourceDuration();
   void BuildPresentationClock();
   void BuildOutputFormats();

protected:
   WCHAR* m_pstrOutput;
   CComPtr<IMFTopology> m_Topology;
   CComPtr<IMFMediaSession> m_MediaSession;
   CComQIPtr<IMFMediaSource> m_Source;
   CComPtr<IMFPresentationClock> m_Clock;
   CComPtr<IMFStreamDescriptor> m_StreamDesc;
   CComPtr<IMFTranscodeProfile> m_TranscodeProfile;
   CComPtr<IMFCollection> m_AvailableOutputTypes;
   std::vector<WAVEFORMATEX>* m_pOutputFormats;
   int m_FormatIndex;
   MFTIME m_SourceDuration;
   IM4AProgress* m_pProgress;
};
