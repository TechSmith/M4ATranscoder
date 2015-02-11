## WaveToM4AGetFormats

```cpp
WAVETOM4A_EXTERN int WaveToM4AInit(WaveToM4AHandle ppHandle, WAVEFORMATEX** ppFormats, int& nCount);
```

## Note
Unlike WaveToM4ACreate and WaveToM4AFree this function simply just takes a WaveToM4AHandle (void*); not a pointer to a WaveToM4AHandle.

## Info
This function allow you to setup your encode bitrate.  In order to really know the bitrates possible this function gets you a list of available bitrates and you just need to know the index (0-based) that you wish to encode with.

This function returns WAVETOM4A_SUCCESS (0) on success.

## Allocation
This function does allocate memory and it is the client who is responsible for deleting it.  See Usage below.

## Usage
```cpp
WaveToM4ahandle handle;
HModule hMod;
//...
WAVETOM4AGETFORMATS_FUNC formatsfunc = (WAVETOM4AGETFORMATS_FUNC)GetProcAddress(hMod, "WaveToM4AGetFormats");
WAVEFORMATEX* pFormats = NULL;
if (WAVETOM4A_SUCCESS == formatsfunc(handle, ppFormats, nCount))
{
   //...
   delete[] pFormats;
}
```
