## WaveToM4A

```cpp
WAVETOM4A_EXTERN int WaveToM4A(WaveToM4AHandle ppHandle, IM4AProgress* pProgress);
```

## Note
Unlike WaveToM4ACreate and WaveToM4AFree this function simply just takes a WaveToM4AHandle (void*); not a pointer to a WaveToM4AHandle.

## Info
This function will do the transcoding.  You need to first call [WaveToM4AInit] such that the input and output files are specified.  You should also call WaveToM4ASetFormat or else you get the default bitrate which won't be desirable.

This function is not asyncronous.  So in your client code it may be desired to launch a thread.

## IM4AProgress
I'm pretty sure you can specify NULL as the second argument.  But if you do you won't be able to know the percentage complete or cancel your transcoding.

To use a IM4AProgress; simply inherit off of it and provide code for the methods.

This function returns WAVETOM4A_SUCCESS (0) on success.

## Canceling
If you cancel your transcode the output file will be deleted.

## Usage
```cpp
WaveToM4ahandle handle;
HModule hMod;
//...
WAVETOM4A_FUNC convertfunc = (WAVETOM4A_FUNC)GetProcAddress(hMod, "WaveToM4A");
if( WAVETOM4A_SUCCESS == convertfunc(handle, (IM4AProgress*)this) )
{
  //...
}
```
