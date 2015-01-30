#include <Windows.h>
#include <atlstr.h>

typedef void(*OurEntry)();

int main()
{
   HMODULE hMod;
   CString strDLLPath = _T("C:\\Users\\s.elgas\\Documents\\Visual Studio 2013\\Projects\\WavToM4A\\Debug\\WavToM4A.dll");
   hMod = LoadLibrary(strDLLPath);

   OurEntry ent = (OurEntry)GetProcAddress(hMod, "SomeFunction");
   if (ent != NULL)
   {
      ent();
   }

}