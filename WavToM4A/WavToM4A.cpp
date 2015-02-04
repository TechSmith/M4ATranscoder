// WavToM4A.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "M4ATranscoder.h"

M4ATranscoder* _Encoder = NULL;

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
      _Encoder = new M4ATranscoder();
      _Encoder->Transcode(pstrInput, pstrOutput);
   }

   delete _Encoder;
   _Encoder = NULL;

   MFShutdown();
   CoUninitialize();
}

extern "C" __declspec(dllexport) void CancelWaveToM4A()
{
   if (_Encoder)
      _Encoder->CancelTranscode();
}