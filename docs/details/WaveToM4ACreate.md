## WaveToM4ACreate

```cpp
WAVETOM4A_EXTERN int WaveToM4ACreate(WaveToM4AHandle* ppHandle);
```

## Info
If you could only do one transcode at a time this function wouldn't even need to exist.  But the M4ATranscoder allows you to create independent transcodings.

This function create a transcoding object.  A WaveToM4AHandle is just a void*  But it is easier to just think about it as a handle.

This function returns WAVETOM4A_SUCCESS (0) on success and your handle would now be set.

## Usage
```cpp
WaveToM4ahandle handle;
HModule hMod;
//...
WAVETOM4ACREATE_FUNC createW2M4Afunc = (WAVETOM4ACREATE_FUNC)GetProcAddress(hMod, "WaveToM4ACreate");
if( WAVETOM4A_SUCCESS == createW2M4Afunc(&handle) )
{
  //...
}
```
