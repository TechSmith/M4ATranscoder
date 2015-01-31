// WavToM4A.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "M4ATranscoder.h"

//class M4ATranscoderImpl : public CAtlDllModuleT < M4ATranscoder >
//{
//   HRESULT PreMessageLoop(int nShowCmd) throw()
//   {
//      HRESULT hr = MFStartup(MF_VERSION);
//      if (FAILED(hr)) {
//         return hr;
//      }
//
//      return S_OK;
//   }
//
//   HRESULT PostMessageLoop() throw()
//   {
//      HRESULT hr = MFShutdown();
//      return S_OK;
//   }
//};

extern "C" __declspec(dllexport) void SomeFunction()
{
   M4ATranscoder encoder;
}


