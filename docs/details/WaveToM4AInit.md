## WaveToM4AInit

```cpp
WAVETOM4A_EXTERN int WaveToM4AInit(WaveToM4AHandle ppHandle);
```

## Note
Unlinke WaveToM4ACreate and WaveToM4AFree this function simply just takes a WaveToM4AHandle (void*); not a pointer to a WaveToM4AHandle.

## Info
This function allow you to setup your encode.  You specify the input wave file and the output M4A file.  This library doesn't create the output directory if it doesn't exist.  So make sure the resulting destination is correct.  At the end of this function the output doesn't exist yet.  That is WaveToM4A.

This function returns WAVETOM4A_SUCCESS (0) on success.

## Usage
```cpp
WaveToM4ahandle handle;
HModule hMod;
CString strInput, strOutput;
//...
WAVETOM4AINIT_FUNC initfunc = (WAVETOM4AINIT_FUNC)GetProcAddress(hMod, "WaveToM4AInit");
if (WAVETOM4A_SUCCESS == initfunc(handle, strInput.LockBuffer(), strOutput.LockBuffer()))
{
  //...
}
```
