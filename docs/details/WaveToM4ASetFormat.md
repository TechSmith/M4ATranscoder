## WaveToM4ASetFormat

```cpp
WAVETOM4A_EXTERN int WaveToM4ASetFormat(WaveToM4AHandle ppHandle, int formatIndex);
```

## Note
Unlike WaveToM4ACreate and WaveToM4AFree this function simply just takes a WaveToM4AHandle (void*); not a pointer to a WaveToM4AHandle.

## Info
This function allow you to setup your encode bitrate.  You might want to get a list of the available bitrates first using [WaveToM4AGetFormats]; and so this function you specify the format index (0-based) to use.

This function returns WAVETOM4A_SUCCESS (0) on success.

## Usage
```cpp
WaveToM4ahandle handle;
HModule hMod;
//...
WAVETOM4AGETFORMATS_FUNC formatsfunc = (WAVETOM4AGETFORMATS_FUNC)GetProcAddress(hMod, "WaveToM4AGetFormats");
WAVEFORMATEX* pFormats = NULL;
if (WAVETOM4A_SUCCESS == formatsfunc(handle, ppFormats, nCount))
{
   WAVETOM4ASETFORMAT_FUNC setFormatfunc = (WAVETOM4ASETFORMAT_FUNC)GetProcAddress(hMod, "WaveToM4ASetFormat");
   if( WAVETOM4A_SUCCESS == setFormatfunc(handle, 0) )
   {
      //...
   }
   delete[] pFormats;
}
```
