## WaveToM4AFree

```cpp
WAVETOM4A_EXTERN int WaveToM4AFree(WaveToM4AHandle* ppHandle);
```

## Info
As mentioned in [WaveToM4ACreate] you can have more than one transcode at a time.  When you are done with a transcode simply call this function to clean it up.

This function deallocates a transcoding object.  A WaveToM4AHandle is just a void*  But it is easier to just think about it as a handle.

This function returns WAVETOM4A_SUCCESS (0) on success and your handle would now be set.

## Usage
```cpp
WaveToM4ahandle handle;
HModule hMod;
//...
WAVETOM4AFREE_FUNC freeW2M4Afunc = (WAVETOM4AFREE_FUNC)GetProcAddress(hMod, "WaveToM4AFree");
if( WAVETOM4A_SUCCESS == freeW2M4Afunc(&handle) )
{
  //...
}
```
