#include <Windows.h>
#include <atlstr.h>

typedef void(*OurEntry)();

int main()
{
   HMODULE hMod;
   CString strDLLPath = _T("WavToM4A.dll");
   hMod = LoadLibrary(strDLLPath);

   OurEntry ent = (OurEntry)GetProcAddress(hMod, "SomeFunction");
   if (ent != NULL)
   {
      ent();
   }

}