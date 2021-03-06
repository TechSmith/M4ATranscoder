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

WAVETOM4A_EXTERN int WaveToM4AInit(WaveToM4AHandle pHandle, WCHAR* pstrInput, WCHAR* pstrOutput)
{
   HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

   HRESULT hr = MFStartup(MF_VERSION);

   M4ATranscoder* pWav2M4A = (M4ATranscoder*)pHandle;
   pWav2M4A->Init(pstrInput, pstrOutput);
   return WAVETOM4A_SUCCESS;
}

WAVETOM4A_EXTERN int WaveToM4AGetFormats(WaveToM4AHandle pHandle, WAVEFORMATEX** ppFormats, int& nCount)
{
   M4ATranscoder* pWav2M4A = (M4ATranscoder*)pHandle;

   const std::vector<WAVEFORMATEX>& formats = pWav2M4A->GetOutputFormats();
   *ppFormats = new WAVEFORMATEX[formats.size()];
   for (std::vector<WAVEFORMATEX>::size_type i = 0; i < formats.size(); i++)
   {
      const WAVEFORMATEX& fmt = formats[i];
      ATLASSERT(fmt.wFormatTag != WAVE_FORMAT_EXTENSIBLE);
      void* pSrc = (void*)&fmt;
      void* pDst = (void*)(*ppFormats + i);
      memcpy(pDst, pSrc, sizeof(WAVEFORMATEX));
   }

   nCount = formats.size();

   return WAVETOM4A_SUCCESS;
}

WAVETOM4A_EXTERN int WaveToM4ASetFormat(WaveToM4AHandle pHandle, int formatIndex)
{
   M4ATranscoder* pWav2M4A = (M4ATranscoder*)pHandle;
   pWav2M4A->SetOutputFormatIndex(formatIndex);
   return WAVETOM4A_SUCCESS;
}

WAVETOM4A_EXTERN int WaveToM4A(WaveToM4AHandle pHandle, IM4AProgress* pProgress)
{
   M4ATranscoder* pWav2M4A = (M4ATranscoder*)pHandle;
   bool bOK = pWav2M4A->Transcode(pProgress);

   MFShutdown();
   return bOK ? WAVETOM4A_SUCCESS : WAVETOM4A_FAILURE_GENERIC;
}
