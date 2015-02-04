#include "stdafx.h"
#include "WaveToM4A.h"

WaveToM4A::WaveToM4A(const CString& strInput, const CString& strOutput)
   : m_strInput(strInput), m_strOutput(strOutput), m_hTranscoderThread(0), m_bCancel(false), m_hwnd(0)
{

}

WaveToM4A::~WaveToM4A()
{
   if (m_hTranscoderThread != 0)
      WaitThreadFinish();
}

/*static*/
bool WaveToM4A::PerformCheck(const CString& strInput, const CString& strOutput)
{
   if (strInput.IsEmpty())
      return AfxMessageBox(NEVER_TRANSLATE("Input is empty")), false;
   if (strOutput.IsEmpty())
      return AfxMessageBox(NEVER_TRANSLATE("Output is empty")), false;

   if (!PathFileExists(strInput))
      return AfxMessageBox(NEVER_TRANSLATE("Input file doesn't exist")), false;

   return true;
}

bool WaveToM4A::Transcode(HWND hwnd /*= 0*/)
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

void WaveToM4A::WaitThreadFinish()
{
   ASSERT(m_hTranscoderThread != 0);

   // wait for transcoding to finish
   WaitForSingleObject(m_hTranscoderThread, INFINITE);
   CloseHandle(m_hTranscoderThread);
}

int WaveToM4A::GetProgress() const
{
   return 15;//TODO: FIX!!!
}

void WaveToM4A::Cancel()
{
   m_bCancel = true;
   WaitThreadFinish();
}

/*static*/
DWORD WINAPI WaveToM4A::TranscoderThread(LPVOID lpParam)
{
   WaveToM4A* pThiz = (WaveToM4A*)lpParam;
   DWORD dwRet = pThiz->TranscoderWorker() ? 1 : 0;
   pThiz->m_hTranscoderThread = 0;
   return dwRet;
}

bool WaveToM4A::TranscoderWorker()
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

   typedef void(*WaveToM4A_FUNC)(WCHAR*, WCHAR*);

   WaveToM4A_FUNC convertfunc = (WaveToM4A_FUNC)GetProcAddress(hMod, "WaveToM4A");
   if (convertfunc == NULL)
   {
      //AfxMessageBox is ok to call in a thread.  Still normally you'd want to return an enum
      //and message elsewhere :)
      AfxMessageBox(NEVER_TRANSLATE("Transcode method not found"));
      goto Done;
   }

   convertfunc(m_strInput.LockBuffer(), m_strOutput.LockBuffer());
   m_strInput.UnlockBuffer();
   m_strOutput.UnlockBuffer();

   FreeLibrary(hMod);

Done:
   if (m_hwnd != 0)
      ::PostMessage(m_hwnd, WM_USER_NOTIFY_FINISH, MAKEWPARAM(0, 0), MAKELPARAM(0, 0));

   return bOK;
}

