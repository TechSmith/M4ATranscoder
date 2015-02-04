// WavToM4A.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "M4ATranscoder.h"
#include <M4ATranscoder/M4ATranscoderAPI.h>

M4ATranscoder* _Encoder = NULL;

WAVETOM4A_EXTERN void WaveToM4A(WCHAR* pstrInput, WCHAR* pstrOutput, IM4AProgress* pProgress)
{
   HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

   // Initialize the COM library.
   HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

   if (SUCCEEDED(hr))
   {
      hr = MFStartup(MF_VERSION);
   }

   {
      M4ATranscoder encoder;
      encoder.Transcode(pstrInput, pstrOutput, pProgress);
   }

   MFShutdown();
   CoUninitialize();
}

//extern "C" __declspec(dllexport) void CancelWaveToM4A()
//{
//   if (_Encoder)
//      _Encoder->CancelTranscode();
//}

