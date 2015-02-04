#include "stdafx.h"
#include "CWaveToM4A.h"

CWaveToM4A::CWaveToM4A(const CString& strInput, const CString& strOutput)
   : m_strInput(strInput), m_strOutput(strOutput), m_hTranscoderThread(0), 
   m_dProgress(0.), m_bCancel(false), m_hwnd(0)
{

}

CWaveToM4A::~CWaveToM4A()
{
   if (m_hTranscoderThread != 0)
      WaitThreadFinish();
}

/*static*/
bool CWaveToM4A::PerformCheck(const CString& strInput, const CString& strOutput)
{
   if (strInput.IsEmpty())
      return AfxMessageBox(NEVER_TRANSLATE("Input is empty")), false;
   if (strOutput.IsEmpty())
      return AfxMessageBox(NEVER_TRANSLATE("Output is empty")), false;

   if (!PathFileExists(strInput))
      return AfxMessageBox(NEVER_TRANSLATE("Input file doesn't exist")), false;

   return true;
}

bool CWaveToM4A::Transcode(HWND hwnd /*= 0*/)
{
   m_hwnd = hwnd;

   DWORD transcoderThreadId;
   m_hTranscoderThread = CreateThread(
      NULL,
      0,
      TranscoderThread,
      this,
      0,
      &transcoderThreadId);

   return true;
}

void CWaveToM4A::WaitThreadFinish()
{
   ASSERT(m_hTranscoderThread != 0);

   // wait for transcoding to finish
   WaitForSingleObject(m_hTranscoderThread, INFINITE);
   CloseHandle(m_hTranscoderThread);
}

double CWaveToM4A::GetProgress() const
{
   return m_dProgress;
}

void CWaveToM4A::Cancel()
{
   m_bCancel = true;
   WaitThreadFinish();
}

void CWaveToM4A::SetProgress(double dPercent)
{
   m_dProgress = dPercent;
}

bool CWaveToM4A::GetCanceled()
{
   return m_bCancel;
}

/*static*/
DWORD WINAPI CWaveToM4A::TranscoderThread(LPVOID lpParam)
{
   CWaveToM4A* pThiz = (CWaveToM4A*)lpParam;
   DWORD dwRet = pThiz->TranscoderWorker() ? 1 : 0;
   pThiz->m_hTranscoderThread = 0;
   return dwRet;
}

bool CWaveToM4A::TranscoderWorker()
{
   bool bOK = false;
   HMODULE hMod = NULL;

   CString strDLLPath = NEVER_TRANSLATE("WavToM4A.dll");
   hMod = LoadLibrary(strDLLPath);
   if (!hMod)
   {
      //AfxMessageBox is ok to call in a thread.  Still normally you'd want to return an enum
      //and message elsewhere :)
      AfxMessageBox(NEVER_TRANSLATE("Unable to open transcode library"));
      goto Done;
   }

   WAVETOM4A_FUNC convertfunc = (WAVETOM4A_FUNC)GetProcAddress(hMod, "WaveToM4A");
   if (convertfunc == NULL)
   {
      //AfxMessageBox is ok to call in a thread.  Still normally you'd want to return an enum
      //and message elsewhere :)
      AfxMessageBox(NEVER_TRANSLATE("Transcode method not found"));
      goto Done;
   }

   convertfunc(m_strInput.LockBuffer(), m_strOutput.LockBuffer(), (IM4AProgress*)this);
   m_strInput.UnlockBuffer();
   m_strOutput.UnlockBuffer();

   FreeLibrary(hMod);

Done:
   if (m_hwnd != 0)
      ::PostMessage(m_hwnd, WM_USER_NOTIFY_FINISH, MAKEWPARAM(0, 0), MAKELPARAM(0, 0));

   return bOK;
}

