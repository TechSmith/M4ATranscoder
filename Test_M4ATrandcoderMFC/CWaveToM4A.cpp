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

   HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

   DWORD dwRet = pThiz->TranscoderWorker() ? 1 : 0;

   CoUninitialize();

   pThiz->m_hTranscoderThread = 0;

   return dwRet;
}

bool CWaveToM4A::TranscoderWorker()
{
   bool bOK = false;
   HMODULE hMod = NULL;
   WaveToM4AHandle handle = NULL;

   CString strDLLPath = NEVER_TRANSLATE("WavToM4A.dll");
   hMod = LoadLibrary(strDLLPath);
   if (!hMod)
   {
      //AfxMessageBox is ok to call in a thread.  Still normally you'd want to return an enum
      //and message elsewhere :)
      AfxMessageBox(NEVER_TRANSLATE("Unable to open transcode library"));
      goto Done;
   }

   //Creation
   WAVETOM4ACREATE_FUNC createW2M4Afunc = (WAVETOM4ACREATE_FUNC)GetProcAddress(hMod, "WaveToM4ACreate");
   if (createW2M4Afunc == NULL)
   {
      //AfxMessageBox is ok to call in a thread.  Still normally you'd want to return an enum
      //and message elsewhere :)
      AfxMessageBox(NEVER_TRANSLATE("Transcode create method not found"));
      goto Done;
   }

   if (WAVETOM4A_SUCCESS != createW2M4Afunc(&handle))
   {
      //I would say out of memory but right now it'll actually throw an exception; but the intent
      //was if out of memory would return a non-success from the create function.
      goto Done;
   }

   //Usage
   WAVETOM4A_FUNC convertfunc = (WAVETOM4A_FUNC)GetProcAddress(hMod, "WaveToM4A");
   if (convertfunc == NULL)
   {
      AfxMessageBox(NEVER_TRANSLATE("Transcode method not found"));
      goto Done;
   }

   convertfunc(handle, m_strInput.LockBuffer(), m_strOutput.LockBuffer(), (IM4AProgress*)this);
   m_strInput.UnlockBuffer();
   m_strOutput.UnlockBuffer();

Done:
   //Cleanup
   WAVETOM4AFREE_FUNC freeW2M4Afunc = (WAVETOM4AFREE_FUNC)GetProcAddress(hMod, "WaveToM4AFree");
   if (freeW2M4Afunc == NULL)
   {
      //AfxMessageBox is ok to call in a thread.  Still normally you'd want to return an enum
      //and message elsewhere :)
      AfxMessageBox(NEVER_TRANSLATE("Transcode free method not found"));
      goto Exit;
   }

   if ( handle != NULL )
      freeW2M4Afunc(&handle);

   if( hMod != NULL )
      FreeLibrary(hMod);

Exit:
   if (m_hwnd != 0)
      ::PostMessage(m_hwnd, WM_USER_NOTIFY_FINISH, MAKEWPARAM(0, 0), MAKELPARAM(0, 0));

   return bOK;
}

