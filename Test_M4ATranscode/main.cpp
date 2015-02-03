#include <Windows.h>
#include <atlstr.h>

// uncomment to test features
//#define TEST_CANCEL_TRANSCODE

typedef void(*WaveToM4A_FUNC)(WCHAR*, WCHAR*);
typedef void(*CancelWaveToM4A_FUNC)(void);

DWORD WINAPI TranscoderThread(LPVOID lpParam);

int main()
{
   HMODULE hMod;
   CString strDLLPath = _T("WavToM4A.dll");
   hMod = LoadLibrary(strDLLPath);
   CancelWaveToM4A_FUNC cancelfunc = (CancelWaveToM4A_FUNC)GetProcAddress(hMod, "CancelWaveToM4A");

   // spawn transcoder in a separate thread
   DWORD transcoderThreadId;
   HANDLE hTranscoderThread = CreateThread(
      NULL,
      0,
      TranscoderThread,
      hMod,
      0,
      &transcoderThreadId);

#ifdef TEST_CANCEL_TRANSCODE
   Sleep(2000);
   cancelfunc();
#endif

   // wait for transcoding to finish
   WaitForSingleObject(hTranscoderThread, INFINITE);
   CloseHandle(hTranscoderThread);

   return 0;
}

DWORD WINAPI TranscoderThread(LPVOID lpParam)
{
   HMODULE hMod = (HMODULE)lpParam;

   WCHAR strInput[] = L"HugeWAV.wav";
   WCHAR strOutput[] = L"Output.m4a";

   WaveToM4A_FUNC convertfunc = (WaveToM4A_FUNC)GetProcAddress(hMod, "WaveToM4A");
   if (convertfunc != NULL)
   {
      convertfunc((WCHAR*)&strInput, (WCHAR*)&strOutput);
   }

   return 0;
}