// WavToM4A.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "M4ATranscoder.h"

extern "C" __declspec(dllexport) void WaveToM4A(WCHAR* pstrInput, WCHAR* pstrOutput)
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
      encoder.Transcode(pstrInput, pstrOutput);
   }

   MFShutdown();
   CoUninitialize();
}


