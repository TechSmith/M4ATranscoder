#ifndef M4ATRANSCODER_API_H
#define M4ATRANSCODER_API_H

#ifdef WIN32
#define WAVETOM4A_EXTERN extern "C" __declspec(dllexport)
#else
#define WAVETOM4A_EXTERN extern "C"
#endif

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
typedef int(*WAVETOM4A_FUNC)(WaveToM4AHandle pHandle, WCHAR* pstrInput, WCHAR* pstrOutput, IM4AProgress* pProgress);

WAVETOM4A_EXTERN int WaveToM4ACreate(WaveToM4AHandle* ppHandle);
WAVETOM4A_EXTERN int WaveToM4AFree(WaveToM4AHandle* ppHandle);

WAVETOM4A_EXTERN int WaveToM4A(WaveToM4AHandle pHandle, WCHAR* pstrInput, WCHAR* pstrOutput, IM4AProgress* pProgress);

#endif

