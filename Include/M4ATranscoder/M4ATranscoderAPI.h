#ifndef M4ATRANSCODER_API_H
#define M4ATRANSCODER_API_H

#ifdef WIN32
#define WAVETOM4A_EXTERN extern "C" __declspec(dllexport)
#else
#define WAVETOM4A_EXTERN extern "C"
#endif

class IM4AProgress
{
public:
   virtual void SetProgress(double dPercent) = 0;
   virtual bool GetCanceled()                = 0;
};

typedef void(*WAVETOM4A_FUNC)(WCHAR* pstrInput, WCHAR* pstrOutput, IM4AProgress* pProgress);

WAVETOM4A_EXTERN void WaveToM4A(WCHAR* pstrInput, WCHAR* pstrOutput, IM4AProgress* pProgress);

#endif

