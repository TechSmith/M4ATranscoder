#include "stdafx.h"

#include "M4ATranscoder.h"

LPCWSTR INPUT_FILE_NAME = L"C:\\Users\\s.elgas\\Desktop\\03-141230_1744.wav";
LPCWSTR OUTPUT_FILE_NAME = L"C:\\Users\\s.elgas\\Desktop\\Output.m4a";

M4ATranscoder::M4ATranscoder()
{
   m_Handle.Attach(CreateEvent(NULL, FALSE, FALSE, NULL));
   CComPtr<IMFTopology> topology;

   HRESULT hr = S_OK;

   hr = MFCreateMediaSession(NULL, &m_MediaSession);
   hr = m_MediaSession->BeginGetEvent(this, NULL);

   CreateMediaSrc();

   CComPtr<IMFPresentationDescriptor> pres_desc;
   CComPtr<IMFStreamDescriptor> stream_desc;
   DWORD stream_count;
   BOOL selected;
   hr = m_Source->CreatePresentationDescriptor(&pres_desc);
   hr = pres_desc->GetStreamDescriptorCount(&stream_count);
   ATLASSERT(stream_count == 1);
   hr = pres_desc->GetStreamDescriptorByIndex(0, &selected, &stream_desc);
   ATLASSERT(selected == TRUE);

   ConfigureOutput(stream_desc, topology);
   hr = m_MediaSession->SetTopology(0, topology);
}

HRESULT M4ATranscoder::CreateMediaSrc()
{
   HRESULT hr;
   MF_OBJECT_TYPE object_type;
   CComPtr<IMFSourceResolver> src_resolver;

   hr = MFCreateSourceResolver(&src_resolver);
   hr = src_resolver->CreateObjectFromURL(
      INPUT_FILE_NAME,
      MF_RESOLUTION_MEDIASOURCE,
      NULL,
      &object_type,
      (IUnknown**)&m_Source);
   ATLASSERT(object_type == MF_OBJECT_MEDIASOURCE);
   return hr;
}

void TraceWavFormatEx(const WAVEFORMATEX * const wfx)
{
   ATLTRACE(_T("WAVEFORMATEX * = %p\n")
      _T("wFormatTag = 0x%x(%u)\nnChannels = %u\nnSamplesPerSec = %u\nnAvgBytesPerSec = %u\n")
      _T("nBlockAlign = %u\nwBitsPerSample = %u\ncbSize = %u\nExtra bytes = "),
      wfx,
      wfx->wFormatTag, wfx->wFormatTag,
      wfx->nChannels,
      wfx->nSamplesPerSec,
      wfx->nAvgBytesPerSec,
      wfx->nBlockAlign,
      wfx->wBitsPerSample,
      wfx->cbSize);
   const unsigned char *const p = (const unsigned char *const)&wfx[1];
   for (unsigned n = 0; n < wfx->cbSize; n++) {
      ATLTRACE(_T("%02x "), p[n]);
   }
   ATLTRACE(_T("\n"));
}

HRESULT M4ATranscoder::ConfigureOutput(CComPtr<IMFStreamDescriptor> stream_desc,
   CComPtr<IMFTopology>& topology)
{
   HRESULT hr;
   CComPtr<IMFMediaType> in_mfmt;
   CComPtr<IMFMediaTypeHandler> mt_handler;
   GUID major_type_guid = GUID_NULL;
   GUID subtype;
   DWORD mt_count;

   hr = stream_desc->GetMediaTypeHandler(&mt_handler);
   hr = mt_handler->GetMajorType(&major_type_guid);
   ATLASSERT(major_type_guid == MFMediaType_Audio);
   hr = mt_handler->GetMediaTypeCount(&mt_count);
   ATLASSERT(mt_count == 1);
   hr = mt_handler->GetMediaTypeByIndex(0, &in_mfmt);
   hr = in_mfmt->GetGUID(MF_MT_SUBTYPE, &subtype);

   WAVEFORMATEX *in_wfx;
   UINT32 wfx_size;
   MFCreateWaveFormatExFromMFMediaType(in_mfmt, &in_wfx, &wfx_size);
   const WORD in_ch = in_wfx->nChannels;
   const WORD in_freq = in_wfx->wBitsPerSample;
   ATLASSERT(in_wfx->wFormatTag == WAVE_FORMAT_PCM);
   ATLTRACE(_T("Input Media Type\n"));
   TraceWavFormatEx(in_wfx);
   CoTaskMemFree(in_wfx);

   CComPtr<IMFTranscodeProfile> x_prof;
   CComPtr<IMFCollection> available_types;
   DWORD dwFlags = MFT_ENUM_FLAG_ALL & (~MFT_ENUM_FLAG_FIELDOFUSE);

   hr = MFCreateTranscodeProfile(&x_prof);
   hr = MFTranscodeGetAudioOutputAvailableTypes(
      MFAudioFormat_AAC,
      dwFlags | MFT_ENUM_FLAG_SORTANDFILTER,
      NULL,
      &available_types);
   hr = available_types->GetElementCount(&mt_count);

   CComPtr<IMFAttributes> attr;
   CComPtr<IMFAttributes> attr_container;

   for (DWORD index = 0; index < mt_count; index++) {
      CComQIPtr<IMFMediaType> out_mfmt;
      hr = available_types->GetElement(index, (IUnknown**)&out_mfmt);
      WAVEFORMATEX *out_wfx;
      hr = MFCreateWaveFormatExFromMFMediaType(out_mfmt, &out_wfx, &wfx_size);
      const WORD out_ch = out_wfx->nChannels;
      const WORD out_freq = out_wfx->wBitsPerSample;
      TraceWavFormatEx(out_wfx);
      CoTaskMemFree(out_wfx);
      if (out_ch == in_ch && out_freq == in_freq) {
         hr = MFCreateAttributes(&attr, 0);
         hr = out_mfmt->CopyAllItems(attr);
         break;
      }
   }
   hr = x_prof->SetAudioAttributes(attr);
   hr = MFCreateAttributes(&attr_container, 1);
   hr = attr_container->SetGUID(
      MF_TRANSCODE_CONTAINERTYPE,
      MFTranscodeContainerType_MPEG4);
   hr = x_prof->SetContainerAttributes(attr_container);
   hr = MFCreateTranscodeTopology(
      m_Source,
      OUTPUT_FILE_NAME,
      x_prof,
      &topology);
   return hr;
}

HRESULT STDMETHODCALLTYPE M4ATranscoder::GetParameters(
   /* [out] */ __RPC__out DWORD *pdwFlags,
   /* [out] */ __RPC__out DWORD *pdwQueue)
{
   return S_OK;
}

HRESULT STDMETHODCALLTYPE M4ATranscoder::Invoke(
   /* [in] */ __RPC__in_opt IMFAsyncResult *pAsyncResult)
{
   HRESULT hr = S_OK;
   MediaEventType me_type;
   CComPtr<IMFMediaEvent> media_event;
   try {
      hr = m_MediaSession->EndGetEvent(pAsyncResult, &media_event);
      hr = media_event->GetType(&me_type);
      if (me_type == MESessionClosed) {
         SetEvent(m_Handle);
      }
      else {
         media_event.p->AddRef();
         ::PostMessage(NULL, WM_MF_EVENT,
            (WPARAM)(IMFMediaEvent*)media_event, (LPARAM)0);
         hr = m_MediaSession->BeginGetEvent(this, NULL);
      }
   }
   catch (CAtlException &) {}
   return hr;
}