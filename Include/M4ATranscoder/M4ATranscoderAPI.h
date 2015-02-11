#ifndef M4ATRANSCODER_API_H
#define M4ATRANSCODER_API_H

#ifdef WIN32
#define WAVETOM4A_EXTERN extern "C" __declspec(dllexport)
#else
#define WAVETOM4A_EXTERN extern "C"
#endif

#include <vector>
#include "Mmreg.h"

typedef void* WaveToM4AHandle;

#define WAVETOM4A_SUCCESS               0
#define WAVETOM4A_FAILURE_GENERIC       -1

class IM4AProgress
{
public:
   virtual void SetProgress(double dPercent) = 0;
   virtual bool GetCanceled()                = 0;
};

typedef int(*WAVETOM4ACREATE_FUNC)(WaveToM4AHandle* ppHandle);
typedef int(*WAVETOM4AFREE_FUNC)(WaveToM4AHandle* ppHandle);
typedef int(*WAVETOM4AINIT_FUNC)(WaveToM4AHandle pHandle, WCHAR* pstrInput, WCHAR* pstrOutput);
typedef int(*WAVETOM4AGETFORMATS_FUNC)(WaveToM4AHandle pHandle, WAVEFORMATEX** ppFormats, int& nCount);
typedef int(*WAVETOM4ASETFORMAT_FUNC)(WaveToM4AHandle pHandle, int formatIndex);
typedef int(*WAVETOM4A_FUNC)(WaveToM4AHandle pHandle, IM4AProgress* pProgress);

WAVETOM4A_EXTERN int WaveToM4ACreate(WaveToM4AHandle* ppHandle);
WAVETOM4A_EXTERN int WaveToM4AFree(WaveToM4AHandle* ppHandle);
WAVETOM4A_EXTERN int WaveToM4AInit(WaveToM4AHandle pHandle, WCHAR* pstrInput, WCHAR* pstrOutput);
WAVETOM4A_EXTERN int WaveToM4AGetFormats(WaveToM4AHandle pHandle, WAVEFORMATEX** ppFormats, int& nCount);
WAVETOM4A_EXTERN int WaveToM4ASetFormat(WaveToM4AHandle pHandle, int formatIndex);
WAVETOM4A_EXTERN int WaveToM4A(WaveToM4AHandle pHandle, IM4AProgress* pProgress);

#endif

