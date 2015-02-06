#pragma once

#include <M4ATranscoder/M4ATranscoderAPI.h>

#define WM_USER_NOTIFY_FINISH    WM_USER+10

class CWaveToM4A : public IM4AProgress
{
public:
   CWaveToM4A(const CString& strInput, const CString& strOutput);
   ~CWaveToM4A();
   static bool PerformCheck(const CString& strInput, const CString& strOutput);
   bool Transcode(HWND hwnd = 0);

   void WaitThreadFinish();

   double GetProgress() const;
   void Cancel();

   //IM4AProgress
   virtual void SetProgress(double dPercent);
   virtual bool GetCanceled();

protected:
   static DWORD WINAPI TranscoderThread(LPVOID lpParam);
   bool TranscoderWorker();

protected:
   HMODULE m_hModule;
   WaveToM4AHandle m_W2M4AHandle;
   CString m_strInput;
   CString m_strOutput;
   HANDLE m_hTranscoderThread;
   double m_dProgress;
   bool m_bCancel;
   HWND m_hwnd;
};
