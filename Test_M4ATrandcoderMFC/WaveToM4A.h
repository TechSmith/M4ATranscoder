#pragma once

#define WM_USER_NOTIFY_FINISH    WM_USER+10

class WaveToM4A
{
public:
   WaveToM4A(const CString& strInput, const CString& strOutput);
   ~WaveToM4A();
   static bool PerformCheck(const CString& strInput, const CString& strOutput);
   bool Transcode(HWND hwnd = 0);

   void WaitThreadFinish();

   int GetProgress() const;
   void Cancel();

protected:
   static DWORD WINAPI TranscoderThread(LPVOID lpParam);
   bool TranscoderWorker();

protected:
   CString m_strInput;
   CString m_strOutput;
   HANDLE m_hTranscoderThread;
   bool m_bCancel;
   HWND m_hwnd;
};
