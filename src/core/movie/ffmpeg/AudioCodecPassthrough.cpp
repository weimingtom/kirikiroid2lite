#include "AudioCodecPassthrough.h"
#include "Codecs.h"
#include "StreamInfo.h"
#include <algorithm>
#include "AEFactory.h"

extern "C" {
#include "libavcodec/avcodec.h"
}

#include "ProcessInfo.h"
#include "Clock.h"
NS_KRMOVIE_BEGIN

#define TRUEHD_BUF_SIZE 61440

CDVDAudioCodecPassthrough::CDVDAudioCodecPassthrough(CProcessInfo &processInfo) :
  CDVDAudioCodec(processInfo),
  m_buffer(NULL),
  m_bufferSize(0),
  m_trueHDoffset(0)
{
}

CDVDAudioCodecPassthrough::~CDVDAudioCodecPassthrough(void)
{
  Dispose();
}

bool CDVDAudioCodecPassthrough::Open(CDVDStreamInfo &hints, CDVDCodecOptions &options)
{
  AEAudioFormat format;
  format.m_dataFormat = AE_FMT_RAW;
  format.m_sampleRate = hints.samplerate;
  switch (hints.codec)
  {
    case AV_CODEC_ID_AC3:
      format.m_streamInfo.m_type = CAEStreamInfo::STREAM_TYPE_AC3;
      format.m_streamInfo.m_sampleRate = hints.samplerate;
      m_processInfo.SetAudioDecoderName("PT_AC3");
      break;

    case AV_CODEC_ID_EAC3:
      format.m_streamInfo.m_type = CAEStreamInfo::STREAM_TYPE_EAC3;
      format.m_streamInfo.m_sampleRate = hints.samplerate;
      m_processInfo.SetAudioDecoderName("PT_EAC3");
      break;

    case AV_CODEC_ID_DTS:
      format.m_streamInfo.m_type = CAEStreamInfo::STREAM_TYPE_DTSHD;
      format.m_streamInfo.m_sampleRate = hints.samplerate;
      m_processInfo.SetAudioDecoderName("PT_DTSHD");
      break;

    case AV_CODEC_ID_TRUEHD:
      format.m_streamInfo.m_type = CAEStreamInfo::STREAM_TYPE_TRUEHD;
      format.m_streamInfo.m_sampleRate = hints.samplerate;
      m_trueHDBuffer.reset(new uint8_t[TRUEHD_BUF_SIZE]);
      m_processInfo.SetAudioDecoderName("PT_TRUEHD");
      break;

    default:
      format.m_streamInfo.m_type = CAEStreamInfo::STREAM_TYPE_NULL;
  }

  bool ret = CAEFactory::SupportsRaw(format);

  m_parser.SetCoreOnly(false);
  if (!ret && hints.codec == AV_CODEC_ID_DTS)
  {
    format.m_streamInfo.m_type = CAEStreamInfo::STREAM_TYPE_DTSHD_CORE;
    ret = CAEFactory::SupportsRaw(format);

    // only get the dts core from the parser if we don't support dtsHD
    m_parser.SetCoreOnly(true);

    m_processInfo.SetAudioDecoderName("PT_DTS");
  }

  m_dataSize = 0;
  m_bufferSize = 0;
  m_backlogSize = 0;
  m_currentPts = DVD_NOPTS_VALUE;
  m_nextPts = DVD_NOPTS_VALUE;
  return ret;
}

void CDVDAudioCodecPassthrough::Dispose()
{
  if (m_buffer)
  {
    delete[] m_buffer;
    m_buffer = NULL;
  }

  m_bufferSize = 0;
}

int CDVDAudioCodecPassthrough::Decode(uint8_t* pData, int iSize, double dts, double pts)
{
  int used = 0;
  int skip = 0;
  if (m_backlogSize)
  {
    m_dataSize = m_bufferSize;
    unsigned int consumed = m_parser.AddData(m_backlogBuffer, m_backlogSize, &m_buffer, &m_dataSize);
    m_bufferSize = std::max(m_bufferSize, m_dataSize);
    if (consumed != m_backlogSize)
    {
      memmove(m_backlogBuffer, m_backlogBuffer+consumed, m_backlogSize-consumed);
    }
    m_backlogSize -= consumed;
  }

  // get rid of potential side data
  if (pData)
  {
    AVPacket pkt;
    av_init_packet(&pkt);
    pkt.data = pData;
    pkt.size = iSize;
    int didSplit = av_packet_split_side_data(&pkt);
    if (didSplit)
    {
      skip = iSize - pkt.size;
      pData = pkt.data;
      iSize = pkt.size;
      av_packet_free_side_data(&pkt);
    }
  }

  if (pData)
  {
    if (m_currentPts == DVD_NOPTS_VALUE)
    {
      if (m_nextPts != DVD_NOPTS_VALUE)
      {
        m_currentPts = m_nextPts;
        m_nextPts = DVD_NOPTS_VALUE;
      }
      else if (pts != DVD_NOPTS_VALUE)
      {
        m_currentPts = pts;
      }
    }

    m_nextPts = pts;
  }

  if (pData && !m_backlogSize)
  {
    if (iSize <= 0)
      return used + skip;

    m_dataSize = m_bufferSize;
    used = m_parser.AddData(pData, iSize, &m_buffer, &m_dataSize);
    m_bufferSize = std::max(m_bufferSize, m_dataSize);

    if (used != iSize)
    {
      m_backlogSize = iSize - used;
      memcpy(m_backlogBuffer, pData + used, m_backlogSize);
      used = iSize;
    }
  }
  else if (pData)
  {
    memcpy(m_backlogBuffer + m_backlogSize, pData, iSize);
    m_backlogSize += iSize;
    used = iSize;
  }

  if (!m_dataSize)
    return used + skip;

  if (m_dataSize)
  {
    m_format.m_dataFormat = AE_FMT_RAW;
    m_format.m_streamInfo = m_parser.GetStreamInfo();
    m_format.m_sampleRate = m_parser.GetSampleRate();
    m_format.m_frameSize = 1;
    CAEChannelInfo layout;
    for (unsigned int i=0; i<m_parser.GetChannels(); i++)
    {
      layout += AE_CH_RAW;
    }
    m_format.m_channelLayout = layout;
  }

  if (m_format.m_streamInfo.m_type == CAEStreamInfo::STREAM_TYPE_TRUEHD)
  {
    if (!m_trueHDoffset)
      memset(m_trueHDBuffer.get(), 0, TRUEHD_BUF_SIZE);

    memcpy(&(m_trueHDBuffer.get())[m_trueHDoffset], m_buffer, m_dataSize);
    uint8_t highByte = (m_dataSize >> 8) & 0xFF;
    uint8_t lowByte = m_dataSize & 0xFF;
    m_trueHDBuffer[m_trueHDoffset+2560-2] = highByte;
    m_trueHDBuffer[m_trueHDoffset+2560-1] = lowByte;
    m_trueHDoffset += 2560;

    if (m_trueHDoffset / 2560 == 24)
    {
      m_dataSize = m_trueHDoffset;
      m_trueHDoffset = 0;
    }
    else
      m_dataSize = 0;
  }

  return used + skip;
}

void CDVDAudioCodecPassthrough::GetData(DVDAudioFrame &frame)
{
  frame.nb_frames = GetData(frame.data);

  if (frame.nb_frames == 0)
    return;

  frame.passthrough = true;
  frame.format = m_format;
  frame.planes = 1;
  frame.bits_per_sample = 8;
  frame.duration = DVD_MSEC_TO_TIME(frame.format.m_streamInfo.GetDuration());
  frame.pts = m_currentPts;
  m_currentPts = DVD_NOPTS_VALUE;
  m_dataSize = 0;
}

int CDVDAudioCodecPassthrough::GetData(uint8_t** dst)
{
  if (m_format.m_streamInfo.m_type == CAEStreamInfo::STREAM_TYPE_TRUEHD)
    *dst = m_trueHDBuffer.get();
  else
    *dst = m_buffer;
  return m_dataSize;
}

void CDVDAudioCodecPassthrough::Reset()
{
  m_trueHDoffset = 0;
  m_dataSize = 0;
  m_bufferSize = 0;
  m_backlogSize = 0;
  m_currentPts = DVD_NOPTS_VALUE;
  m_nextPts = DVD_NOPTS_VALUE;
  m_parser.Reset();
}

int CDVDAudioCodecPassthrough::GetBufferSize()
{
  return (int)m_parser.GetBufferSize();
}
NS_KRMOVIE_END