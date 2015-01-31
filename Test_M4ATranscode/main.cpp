#include <Windows.h>
#include <atlstr.h>

typedef void(*WaveToM4A_FUNC)(WCHAR*, WCHAR*);

int main()
{
   HMODULE hMod;
   CString strDLLPath = _T("WavToM4A.dll");
   hMod = LoadLibrary(strDLLPath);

   WCHAR strInput[] = L"HugeWAV.wav";
   WCHAR strOutput[] = L"Output.m4a";

   WaveToM4A_FUNC convertfunc = (WaveToM4A_FUNC)GetProcAddress(hMod, "WaveToM4A");
   if (convertfunc != NULL)
   {
      convertfunc((WCHAR*)&strInput, (WCHAR*)&strOutput);
   }
}
