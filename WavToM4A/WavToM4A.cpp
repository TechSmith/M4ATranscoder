// WavToM4A.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "M4ATranscoder.h"
#include <M4ATranscoder/M4ATranscoderAPI.h>

WAVETOM4A_EXTERN int WaveToM4ACreate(WaveToM4AHandle* ppHandle)
{
   *ppHandle = new M4ATranscoder;
   return WAVETOM4A_SUCCESS;
}

WAVETOM4A_EXTERN int WaveToM4AFree(WaveToM4AHandle* ppHandle)
{
   M4ATranscoder* ptr = (M4ATranscoder*)*ppHandle;
   delete ptr;
   return WAVETOM4A_SUCCESS;
}

WAVETOM4A_EXTERN int WaveToM4A(WaveToM4AHandle pHandle, WCHAR* pstrInput, WCHAR* pstrOutput, IM4AProgress* pProgress)
{
   HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

   HRESULT hr = MFStartup(MF_VERSION);

<<<<<<< HEAD
   M4ATranscoder* pWav2M4A = (M4ATranscoder*)pHandle;
   bool bOK = pWav2M4A->Transcode(pstrInput, pstrOutput, pProgress);
=======
   if (SUCCEEDED(hr))
   {
      hr = MFStartup(MF_VERSION);
   }

   {
      M4ATranscoder* pWav2M4A = (M4ATranscoder*)pHandle;
      pWav2M4A->Init(pstrInput, pstrOutput);
      pWav2M4A->Transcode(pProgress);
   }
>>>>>>> set up init function

   MFShutdown();
   return bOK ? WAVETOM4A_SUCCESS : WAVETOM4A_FAILURE_GENERIC;
}
