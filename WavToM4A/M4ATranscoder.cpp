#include "stdafx.h"

#include "M4ATranscoder.h"
#include <M4ATranscoder/M4ATranscoderAPI.h>

void M4ATranscoder::SetSourceDuration()
{
   m_SourceDuration = 0;
   CComPtr<IMFPresentationDescriptor> pDescriptor = NULL;
   ATLASSERT(m_Source != NULL);
   HRESULT hr = m_Source->CreatePresentationDescriptor(&pDescriptor);
   if (SUCCEEDED(hr))
   {
      hr = pDescriptor->GetUINT64(MF_PD_DURATION, (UINT64*)(&m_SourceDuration));
   }
}

void M4ATranscoder::SetPresentationClock()
{
   ATLASSERT(m_MediaSession != NULL);
   CComPtr<IMFClock> pClock = NULL;
   HRESULT hr = m_MediaSession->GetClock(&pClock);
   ATLASSERT(SUCCEEDED(hr));
   hr = pClock->QueryInterface(IID_PPV_ARGS(&m_Clock));
   ATLASSERT(SUCCEEDED(hr));
}

#define FOREVER   true
bool M4ATranscoder::Transcode(WCHAR* pstrInput, WCHAR* pstrOutput, IM4AProgress* pProgress)
{
   CComPtr<IMFTopology> topology;
   HRESULT hr = MFCreateMediaSession(NULL, &m_MediaSession);

   MF_OBJECT_TYPE object_type;
   CComPtr<IMFSourceResolver> src_resolver;

   hr = MFCreateSourceResolver(&src_resolver);
   hr = src_resolver->CreateObjectFromURL(
      pstrInput,
      MF_RESOLUTION_MEDIASOURCE,
      NULL,
      &object_type,
      (IUnknown**)&m_Source);
   ATLASSERT(object_type == MF_OBJECT_MEDIASOURCE);

   CComPtr<IMFPresentationDescriptor> pres_desc;
   CComPtr<IMFStreamDescriptor> stream_desc;
   DWORD stream_count;
   BOOL selected;
   hr = m_Source->CreatePresentationDescriptor(&pres_desc);
   hr = pres_desc->GetStreamDescriptorCount(&stream_count);
   ATLASSERT(stream_count == 1);
   hr = pres_desc->GetStreamDescriptorByIndex(0, &selected, &stream_desc);
   ATLASSERT(selected == TRUE);

   SetSourceDuration();
   SetPresentationClock();

   ConfigureOutput(pstrOutput, stream_desc, topology);
   hr = m_MediaSession->SetTopology(0, topology);

   PROPVARIANT props;
   props.intVal = 0;
   hr = m_MediaSession->Start(NULL, &props);
   while (FOREVER)
   {
      CComPtr<IMFMediaEvent> pEvent;
      MediaEventType eType = 0;
      m_MediaSession->GetEvent(MF_EVENT_FLAG_NO_WAIT, &pEvent);
      if (pEvent)
         pEvent->GetType(&eType);

      if (eType == MEEndOfPresentation)
      {
         m_MediaSession->Stop();
      }
      if (eType == MESessionEnded || eType == MESessionClosed)
      {
         m_MediaSession->Shutdown();
         break;
      }


      if (pProgress && pProgress->GetCanceled())
      {
         // closing the session will eventually trigger an MESessionClosed event
         m_MediaSession->Close();
      }

      if (pProgress)
      {
         double dProgress = GetEncodingProgress();
         if (dProgress > 0.)
            pProgress->SetProgress(dProgress);
      }

      Sleep(100);
   }

   // if we just canceled, remove the leftover file
   if (pProgress->GetCanceled() && !::DeleteFile(pstrOutput) )
   {
      ATLTRACE(_T("DeleteFile failed (%d)"), GetLastError());
      return false;
   }

   return true;
}
#undef FOREVER

double M4ATranscoder::GetEncodingProgress()
{
   double dProgress = -1.;
   if (m_Clock && m_SourceDuration != 0)
   {
      MFTIME pos = 0;
      m_Clock->GetTime(&pos);
      dProgress = ((100. * pos) / m_SourceDuration);
   }
   return dProgress;
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

HRESULT M4ATranscoder::ConfigureOutput(WCHAR* pstrOutput, CComPtr<IMFStreamDescriptor> stream_desc,
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
      pstrOutput,
      x_prof,
      &topology);
   return hr;
}
