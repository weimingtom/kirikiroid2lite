#include "DemuxFFmpeg.h"
extern "C" {
#include "libavutil/dict.h"
#include "libavutil/opt.h"
}
#include "Clock.h"
#include "DemuxPacket.h"
#include <algorithm>
#include "InputStream.h"
NS_KRMOVIE_BEGIN

struct StereoModeConversionMap
{
	const char*          name;
	const char*          mode;
};

// we internally use the matroska string representation of stereoscopic modes.
// This struct is a conversion map to convert stereoscopic mode values
// from asf/wmv to the internally used matroska ones
static const struct StereoModeConversionMap WmvToInternalStereoModeMap[] =
{
	{ "SideBySideRF", "right_left" },
	{ "SideBySideLF", "left_right" },
	{ "OverUnderRT", "bottom_top" },
	{ "OverUnderLT", "top_bottom" },
	{}
};

#define FF_MAX_EXTRADATA_SIZE ((1 << 28) - FF_INPUT_BUFFER_PADDING_SIZE)
#define FFMPEG_FILE_BUFFER_SIZE   32768 // default reading size for ffmpeg

std::string CDemuxStreamAudioFFmpeg::GetStreamName()
{
	if (!m_stream)
		return "";
	if (!m_description.empty())
		return m_description;
	else
		return CDemuxStream::GetStreamName();
}

std::string CDemuxStreamVideoFFmpeg::GetStreamName()
{
	if (!m_stream)
		return "";
	if (!m_description.empty())
		return m_description;
	else
		return CDemuxStream::GetStreamName();
}

static int interrupt_cb(void* ctx)
{
	CDVDDemuxFFmpeg* demuxer = static_cast<CDVDDemuxFFmpeg*>(ctx);
	if (demuxer && demuxer->Aborted())
		return 1;
	return 0;
}

static int dvd_file_read(void *h, uint8_t* buf, int size)
{
	if (interrupt_cb(h))
		return AVERROR_EXIT;

	InputStream* pInputStream = static_cast<CDVDDemuxFFmpeg*>(h)->m_pInput;
	return pInputStream->Read(buf, size);
}

static int64_t dvd_file_seek(void *h, int64_t pos, int whence)
{
	if (interrupt_cb(h))
		return AVERROR_EXIT;

	InputStream* pInputStream = static_cast<CDVDDemuxFFmpeg*>(h)->m_pInput;
	if (whence == AVSEEK_SIZE) {
		return pInputStream->GetLength();
	} else {
		return pInputStream->Seek(pos, whence & ~AVSEEK_FORCE);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

CDVDDemuxFFmpeg::CDVDDemuxFFmpeg() : IDemux()
{
	m_pFormatContext = NULL;
	m_ioContext = NULL;
	m_currentPts = DVD_NOPTS_VALUE;
	m_bMatroska = false;
	m_bAVI = false;
	m_speed = DVD_PLAYSPEED_NORMAL;
	m_program = UINT_MAX;
	m_pkt.result = -1;
	memset(&m_pkt.pkt, 0, sizeof(AVPacket));
	m_streaminfo = true; /* set to true if we want to look for streams before playback */
	m_checkvideo = false;
}


CDVDDemuxFFmpeg::~CDVDDemuxFFmpeg()
{
	Dispose();
//	ff_flush_avutil_log_buffers();
}

bool CDVDDemuxFFmpeg::Aborted()
{
	if (m_timeout.IsTimePast())
		return true;

// 	CDVDInputStreamFFmpeg * input = dynamic_cast<CDVDInputStreamFFmpeg*>(m_pInput);
// 	if (input && input->Aborted())
// 		return true;

	return false;
}

bool CDVDDemuxFFmpeg::Open(InputStream* pInput, bool streaminfo, bool fileinfo)
{
	AVInputFormat* iformat = NULL;
	m_streaminfo = streaminfo;
	m_currentPts = DVD_NOPTS_VALUE;
	m_speed = DVD_PLAYSPEED_NORMAL;
	m_program = UINT_MAX;

	const AVIOInterruptCB int_cb = { interrupt_cb, this };

	if (!pInput) return false;

	m_pInput = pInput;
	m_pInput->AddRef();
	const std::string & strFile = pInput->GetFileName();
	
	// open the demuxer
	m_pFormatContext = avformat_alloc_context();
	m_pFormatContext->interrupt_callback = int_cb;

	// try to abort after 30 seconds
	m_timeout.Set(30000);
#if 0
	if (m_pInput->IsStreamType(DVDSTREAM_TYPE_FFMPEG))
	{
		// special stream type that makes avformat handle file opening
		// allows internal ffmpeg protocols to be used
		AVDictionary *options = GetFFMpegOptionsFromInput();

		CURL url = m_pInput->GetURL();

		int result = -1;
		if (url.IsProtocol("mms"))
		{
			// try mmsh, then mmst
			url.SetProtocol("mmsh");
			url.SetProtocolOptions("");
			result = avformat_open_input(&m_pFormatContext, url.Get().c_str(), iformat, &options);
			if (result < 0)
			{
				url.SetProtocol("mmst");
				strFile = url.Get();
			}
		}
		if (result < 0 && avformat_open_input(&m_pFormatContext, strFile.c_str(), iformat, &options) < 0)
		{
			CLog::Log(LOGDEBUG, "Error, could not open file %s", CURL::GetRedacted(strFile).c_str());
			Dispose();
			av_dict_free(&options);
			return false;
		}
		av_dict_free(&options);
	} else
#endif
	{
		unsigned char* buffer = (unsigned char*)av_malloc(FFMPEG_FILE_BUFFER_SIZE);
		m_ioContext = avio_alloc_context(buffer, FFMPEG_FILE_BUFFER_SIZE, 0, this, dvd_file_read, NULL, dvd_file_seek);
// 		m_ioContext->max_packet_size = m_pInput->GetBlockSize();
// 		if (m_ioContext->max_packet_size)
// 			m_ioContext->max_packet_size *= FFMPEG_FILE_BUFFER_SIZE / m_ioContext->max_packet_size;

// 		if (!m_pInput->Seek(pos, SEEK_POSSIBLE))
// 			m_ioContext->seekable = 0;

		if (iformat == NULL)
		{
			// let ffmpeg decide which demuxer we have to open
			av_probe_input_buffer(m_ioContext, &iformat, strFile.c_str(), NULL, 0, 0);

			// Use the more low-level code in case we have been built against an old
			// FFmpeg without the above av_probe_input_buffer(), or in case we only
			// want to probe for spdif (DTS or IEC 61937) compressed audio
			// specifically, or in case the file is a wav which may contain DTS or
			// IEC 61937 (e.g. ac3-in-wav) and we want to check for those formats.
			if ((iformat && strcmp(iformat->name, "wav") == 0))
			{
				AVProbeData pd;
				uint8_t probe_buffer[FFMPEG_FILE_BUFFER_SIZE + AVPROBE_PADDING_SIZE];

				// init probe data
				pd.buf = probe_buffer;
				pd.filename = strFile.c_str();

				// av_probe_input_buffer might have changed the buffer_size beyond our allocated amount
				int buffer_size = std::min((int)FFMPEG_FILE_BUFFER_SIZE, m_ioContext->buffer_size);
				buffer_size = m_ioContext->max_packet_size ? m_ioContext->max_packet_size : buffer_size;
				// read data using avformat's buffers
				pd.buf_size = avio_read(m_ioContext, pd.buf, buffer_size);
				if (pd.buf_size <= 0)
				{
					return false;
				}
				memset(pd.buf + pd.buf_size, 0, AVPROBE_PADDING_SIZE);

				// restore position again
				avio_seek(m_ioContext, 0, SEEK_SET);

				// the advancedsetting is for allowing the user to force outputting the
				// 44.1 kHz DTS wav file as PCM, so that an A/V receiver can decode
				// it (this is temporary until we handle 44.1 kHz passthrough properly)
				if ( (iformat && strcmp(iformat->name, "wav") == 0 ))
				{
					// check for spdif and dts
					// This is used with wav files and audio CDs that may contain
					// a DTS or AC3 track padded for S/PDIF playback. If neither of those
					// is present, we assume it is PCM audio.
					// AC3 is always wrapped in iec61937 (ffmpeg "spdif"), while DTS
					// may be just padded.
					AVInputFormat *iformat2;
					iformat2 = av_find_input_format("spdif");

					if (iformat2 && iformat2->read_probe(&pd) > AVPROBE_SCORE_MAX / 4)
					{
						iformat = iformat2;
					} else
					{
						// not spdif or no spdif demuxer, try dts
						iformat2 = av_find_input_format("dts");

						if (iformat2 && iformat2->read_probe(&pd) > AVPROBE_SCORE_MAX / 4)
						{
							iformat = iformat2;
// 						} else if (trySPDIFonly)
// 						{
// 							// not dts either, return false in case we were explicitely
// 							// requested to only check for S/PDIF padded compressed audio
// 							CLog::Log(LOGDEBUG, "%s - not spdif or dts file, fallbacking", __FUNCTION__);
// 							return false;
						}
					}
				}
			}

			if (!iformat)
			{
			//	CLog::Log(LOGERROR, "%s - error probing input format, %s", __FUNCTION__, CURL::GetRedacted(strFile).c_str());
				return false;
			} else
			{
// 				if (iformat->name)
// 					CLog::Log(LOGDEBUG, "%s - probing detected format [%s]", __FUNCTION__, iformat->name);
// 				else
// 					CLog::Log(LOGDEBUG, "%s - probing detected unnamed format", __FUNCTION__);
			}
		}


		m_pFormatContext->pb = m_ioContext;

		AVDictionary *options = NULL;
		if (iformat->name && (strcmp(iformat->name, "mp3") == 0 || strcmp(iformat->name, "mp2") == 0))
		{
//			CLog::Log(LOGDEBUG, "%s - setting usetoc to 0 for accurate VBR MP3 seek", __FUNCTION__);
			av_dict_set(&options, "usetoc", "0", 0);
		}

// 		if (StringUtils::StartsWith(content, "audio/l16"))
// 		{
// 			int channels = 2;
// 			int samplerate = 44100;
// 			GetL16Parameters(channels, samplerate);
// 			av_dict_set_int(&options, "channels", channels, 0);
// 			av_dict_set_int(&options, "sample_rate", samplerate, 0);
// 		}

		if (avformat_open_input(&m_pFormatContext, strFile.c_str(), iformat, &options) < 0)
		{
//			CLog::Log(LOGERROR, "%s - Error, could not open file %s", __FUNCTION__, CURL::GetRedacted(strFile).c_str());
			Dispose();
			av_dict_free(&options);
			return false;
		}
		av_dict_free(&options);
	}

	// Avoid detecting framerate if advancedsettings.xml says so
// 	if (g_advancedSettings.m_videoFpsDetect == 0)
// 		m_pFormatContext->fps_probe_size = 0;

	// analyse very short to speed up mjpeg playback start
	if (iformat && (strcmp(iformat->name, "mjpeg") == 0) && m_ioContext->seekable == 0)
		av_opt_set_int(m_pFormatContext, "analyzeduration", 500000, 0);

	bool skipCreateStreams = false;
//	bool isBluray = pInput->IsStreamType(DVDSTREAM_TYPE_BLURAY);
	if (iformat && (strcmp(iformat->name, "mpegts") == 0) && !fileinfo /*&& !isBluray*/)
	{
		av_opt_set_int(m_pFormatContext, "analyzeduration", 500000, 0);
		m_checkvideo = true;
		skipCreateStreams = true;
	} else if (!iformat || (strcmp(iformat->name, "mpegts") != 0))
	{
		m_streaminfo = true;
	}

// 	if (iformat && (strcmp(iformat->name, "mov,mp4,m4a,3gp,3g2,mj2") == 0))
// 	{
// 		if (URIUtils::IsRemote(strFile))
// 			m_pFormatContext->iformat->flags |= AVFMT_NOGENSEARCH;
// 	}

	// we need to know if this is matroska or avi later
	m_bMatroska = strncmp(m_pFormatContext->iformat->name, "matroska", 8) == 0;	// for "matroska.webm"
	m_bAVI = strcmp(m_pFormatContext->iformat->name, "avi") == 0;

	if (m_streaminfo)
	{
		for (unsigned int i = 0; i < m_pFormatContext->nb_streams; i++)
		{
			AVStream *st = m_pFormatContext->streams[i];
			if (st->codec->codec_type == AVMEDIA_TYPE_AUDIO && st->codec->codec_id == AV_CODEC_ID_DTS)
			{
				AVCodec* pCodec = avcodec_find_decoder_by_name("dcadec");
				if (pCodec)
					st->codec->codec = pCodec;
			}
		}
		/* to speed up dvd switches, only analyse very short */
// 		if (m_pInput->IsStreamType(DVDSTREAM_TYPE_DVD))
// 			av_opt_set_int(m_pFormatContext, "analyzeduration", 500000, 0);

	//	CLog::Log(LOGDEBUG, "%s - avformat_find_stream_info starting", __FUNCTION__);
		int iErr = avformat_find_stream_info(m_pFormatContext, NULL);
		if (iErr < 0)
		{
		//	CLog::Log(LOGWARNING, "could not find codec parameters for %s", CURL::GetRedacted(strFile).c_str());
			if (
// 				m_pInput->IsStreamType(DVDSTREAM_TYPE_DVD)
// 				|| m_pInput->IsStreamType(DVDSTREAM_TYPE_BLURAY) ||
				(m_pFormatContext->nb_streams == 1 && m_pFormatContext->streams[0]->codec->codec_id == AV_CODEC_ID_AC3)
				|| m_checkvideo)
			{
				// special case, our codecs can still handle it.
			}
			else
			{
				Dispose();
				return false;
			}
		}
	//	CLog::Log(LOGDEBUG, "%s - av_find_stream_info finished", __FUNCTION__);

		if (m_checkvideo)
		{
			// make sure we start video with an i-frame
			ResetVideoStreams();
		}
	}
	else
	{
		m_program = 0;
		m_checkvideo = true;
		skipCreateStreams = true;
	}

	// reset any timeout
	m_timeout.SetInfinite();

	// if format can be nonblocking, let's use that
	m_pFormatContext->flags |= AVFMT_FLAG_NONBLOCK;

	// print some extra information
	av_dump_format(m_pFormatContext, 0, strFile.c_str(), 0);

	UpdateCurrentPTS();

	// in case of mpegts and we have not seen pat/pmt, defer creation of streams
	if (!skipCreateStreams || m_pFormatContext->nb_programs > 0)
	{
		unsigned int nProgram(~0);
		if (m_pFormatContext->nb_programs > 0)
		{

			// select the corrrect program if requested
// 			CVariant programProp(pInput->GetProperty("program"));
// 			if (!programProp.isNull())
// 			{
// 				int programNumber = programProp.asInteger();
// 
// 				for (unsigned int i = 0; i < m_pFormatContext->nb_programs; ++i)
// 				{
// 					if (m_pFormatContext->programs[i]->program_num == programNumber)
// 					{
// 						nProgram = i;
// 						break;
// 					}
// 				}
// 			}
		}
		CreateStreams(nProgram);
	}

	// allow IsProgramChange to return true
	if (skipCreateStreams && GetNrOfStreams() == 0)
		m_program = 0;

	m_displayTime = 0;
	m_dtsAtDisplayTime = DVD_NOPTS_VALUE;

	return true;
}

void CDVDDemuxFFmpeg::Dispose()
{
	m_pkt.result = -1;
	av_packet_unref(&m_pkt.pkt);

	if (m_pFormatContext)
	{
		for (unsigned int i = 0; i < m_pFormatContext->nb_streams; i++)
		{
			avcodec_close(m_pFormatContext->streams[i]->codec);
		}

		if (m_ioContext && m_pFormatContext->pb && m_pFormatContext->pb != m_ioContext)
		{
		//	CLog::Log(LOGWARNING, "CDVDDemuxFFmpeg::Dispose - demuxer changed our byte context behind our back, possible memleak");
			m_ioContext = m_pFormatContext->pb;
		}
		avformat_close_input(&m_pFormatContext);
	}

	if (m_ioContext)
	{
		av_free(m_ioContext->buffer);
		av_free(m_ioContext);
	}

	m_ioContext = NULL;
	m_pFormatContext = NULL;
	m_speed = DVD_PLAYSPEED_NORMAL;

	DisposeStreams();

	SAFE_RELEASE(m_pInput);
}

void CDVDDemuxFFmpeg::Reset()
{
	InputStream* pInputStream = m_pInput;
	pInputStream->AddRef();
	Dispose();
	Open(pInputStream, m_streaminfo);
	pInputStream->Release();
}

void CDVDDemuxFFmpeg::Flush()
{
	if (m_pFormatContext)
	{
		if (m_pFormatContext->pb)
			avio_flush(m_pFormatContext->pb);
		avformat_flush(m_pFormatContext);
	}

	m_currentPts = DVD_NOPTS_VALUE;

	m_pkt.result = -1;
	av_packet_unref(&m_pkt.pkt);

	m_displayTime = 0;
	m_dtsAtDisplayTime = DVD_NOPTS_VALUE;
}

void CDVDDemuxFFmpeg::Abort()
{
	m_timeout.SetExpired();
}

void CDVDDemuxFFmpeg::SetSpeed(int iSpeed)
{
	if (!m_pFormatContext)
		return;

	if (m_speed == iSpeed)
		return;

	if (m_speed != DVD_PLAYSPEED_PAUSE && iSpeed == DVD_PLAYSPEED_PAUSE)
	{
//		m_pInput->Pause(m_currentPts);
		av_read_pause(m_pFormatContext);
	} else if (m_speed == DVD_PLAYSPEED_PAUSE && iSpeed != DVD_PLAYSPEED_PAUSE)
	{
//		m_pInput->Pause(m_currentPts);
		av_read_play(m_pFormatContext);
	}
	m_speed = iSpeed;

	AVDiscard discard = AVDISCARD_NONE;
	if (m_speed > 4 * DVD_PLAYSPEED_NORMAL)
		discard = AVDISCARD_NONKEY;
	else if (m_speed > 2 * DVD_PLAYSPEED_NORMAL)
		discard = AVDISCARD_BIDIR;
	else if (m_speed < DVD_PLAYSPEED_PAUSE)
		discard = AVDISCARD_NONKEY;


	for (unsigned int i = 0; i < m_pFormatContext->nb_streams; i++)
	{
		if (m_pFormatContext->streams[i])
		{
			if (m_pFormatContext->streams[i]->discard != AVDISCARD_ALL)
				m_pFormatContext->streams[i]->discard = discard;
		}
	}
}

AVDictionary *CDVDDemuxFFmpeg::GetFFMpegOptionsFromInput()
{
	AVDictionary *options = nullptr;
#if 0
	const CDVDInputStreamFFmpeg *const input =
		dynamic_cast<CDVDInputStreamFFmpeg*>(m_pInput);

	CURL url = m_pInput->GetURL();

	if (url.IsProtocol("http") || url.IsProtocol("https"))
	{
		std::map<std::string, std::string> protocolOptions;
		url.GetProtocolOptions(protocolOptions);
		std::string headers;
		bool hasUserAgent = false;
		bool hasCookies = false;
		for (std::map<std::string, std::string>::const_iterator it = protocolOptions.begin(); it != protocolOptions.end(); ++it)
		{
			std::string name = it->first;
			StringUtils::ToLower(name);
			const std::string &value = it->second;

			if (name == "seekable")
				av_dict_set(&options, "seekable", value.c_str(), 0);
			// map some standard http headers to the ffmpeg related options
			else if (name == "user-agent")
			{
				av_dict_set(&options, "user-agent", value.c_str(), 0);
				CLog::Log(LOGDEBUG, "CDVDDemuxFFmpeg::GetFFMpegOptionsFromInput() adding ffmpeg option 'user-agent: %s'", value.c_str());
				hasUserAgent = true;
			} else if (name == "cookie")
			{
				CLog::Log(LOGDEBUG, "CDVDDemuxFFmpeg::GetFFMpegOptionsFromInput() adding ffmpeg option 'cookies: %s'", value.c_str());
				headers.append(it->first).append(": ").append(value).append("\r\n");
				hasCookies = true;
			}
			// other standard headers (see https://en.wikipedia.org/wiki/List_of_HTTP_header_fields) are appended as actual headers
			else if (name == "accept" || name == "accept-language" || name == "accept-datetime" ||
				name == "authorization" || name == "cache-control" || name == "connection" || name == "content-md5" ||
				name == "date" || name == "expect" || name == "forwarded" || name == "from" || name == "if-match" ||
				name == "if-modified-since" || name == "if-none-match" || name == "if-range" || name == "if-unmodified-since" ||
				name == "max-forwards" || name == "origin" || name == "pragma" || name == "range" || name == "referer" ||
				name == "te" || name == "upgrade" || name == "via" || name == "warning" || name == "x-requested-with" ||
				name == "dnt" || name == "x-forwarded-for" || name == "x-forwarded-host" || name == "x-forwarded-proto" ||
				name == "front-end-https" || name == "x-http-method-override" || name == "x-att-deviceid" ||
				name == "x-wap-profile" || name == "x-uidh" || name == "x-csrf-token" || name == "x-request-id" ||
				name == "x-correlation-id")
			{
				if (name == "authorization")
				{
					CLog::Log(LOGDEBUG, "CDVDDemuxFFmpeg::GetFFMpegOptionsFromInput() adding custom header option '%s: ***********'", it->first.c_str());
				} else
				{
					CLog::Log(LOGDEBUG, "CDVDDemuxFFmpeg::GetFFMpegOptionsFromInput() adding custom header option '%s: %s'", it->first.c_str(), value.c_str());
				}
				headers.append(it->first).append(": ").append(value).append("\r\n");
			}
			// we don't add blindly all options to headers anymore
			// if anybody wants to pass options to ffmpeg, explicitly prefix those
			// to be identified here
			else
			{
				CLog::Log(LOGDEBUG, "CDVDDemuxFFmpeg::GetFFMpegOptionsFromInput() ignoring header option '%s: %s'", it->first.c_str(), value.c_str());
			}
		}
		if (!hasUserAgent)
		{
			// set default xbmc user-agent.
			av_dict_set(&options, "user-agent", g_advancedSettings.m_userAgent.c_str(), 0);
		}

		if (!headers.empty())
			av_dict_set(&options, "headers", headers.c_str(), 0);

		if (!hasCookies)
		{
			std::string cookies;
			if (XFILE::CCurlFile::GetCookies(url, cookies))
				av_dict_set(&options, "cookies", cookies.c_str(), 0);
		}
	}

	if (input)
	{
		const std::string host = input->GetProxyHost();
		if (!host.empty() && input->GetProxyType() == "http")
		{
			std::ostringstream urlStream;

			const uint16_t port = input->GetProxyPort();
			const std::string user = input->GetProxyUser();
			const std::string password = input->GetProxyPassword();

			urlStream << "http://";

			if (!user.empty()) {
				urlStream << user;
				if (!password.empty())
					urlStream << ":" << password;
				urlStream << "@";
			}

			urlStream << host << ':' << port;

			av_dict_set(&options, "http_proxy", urlStream.str().c_str(), 0);
		}

		// rtmp options
		if (url.IsProtocol("rtmp") || url.IsProtocol("rtmpt") ||
			url.IsProtocol("rtmpe") || url.IsProtocol("rtmpte") ||
			url.IsProtocol("rtmps"))
		{
			static const std::map<std::string, std::string> optionmap =
			{ { { "SWFPlayer", "rtmp_swfurl" },
			{ "PageURL", "rtmp_pageurl" },
			{ "PlayPath", "rtmp_playpath" },
			{ "TcUrl", "rtmp_tcurl" },
			{ "IsLive", "rtmp_live" },
			{ "playpath", "rtmp_playpath" },
			{ "swfurl", "rtmp_swfurl" },
			{ "swfvfy", "rtmp_swfverify" },
				} };

			for (const auto& it : optionmap)
			{
				if (input->GetItem().HasProperty(it.first))
				{
					av_dict_set(&options, it.second.c_str(),
						input->GetItem().GetProperty(it.first).asString().c_str(), 0);
				}
			}

			CURL tmpUrl = url;
			std::vector<std::string> opts = StringUtils::Split(tmpUrl.Get(), " ");
			if (opts.size() > 1) // inline rtmp options
			{
				std::string swfurl;
				bool swfvfy = false;
				for (size_t i = 1; i < opts.size(); ++i)
				{
					std::vector<std::string> value = StringUtils::Split(opts[i], "=");
					auto it = optionmap.find(value[0]);
					if (it != optionmap.end())
					{
						if (value[0] == "swfurl" || value[0] == "SWFPlayer")
							swfurl = value[1];
						if (value[0] == "swfvfy" && value[1] == "true")
							swfvfy = true;
						else
							av_dict_set(&options, it->second.c_str(), value[1].c_str(), 0);
					}
					if (swfvfy)
						av_dict_set(&options, "rtmp_swfverify", swfurl.c_str(), 0);
				}
				tmpUrl = CURL(opts.front());
			}
		}
	}
#endif
	return options;
}

double CDVDDemuxFFmpeg::ConvertTimestamp(int64_t pts, int den, int num)
{
	if (pts == (int64_t)AV_NOPTS_VALUE)
		return DVD_NOPTS_VALUE;

	// do calculations in floats as they can easily overflow otherwise
	// we don't care for having a completly exact timestamp anyway
	double timestamp = (double)pts * num / den;
	double starttime = 0.0f;

//	CDVDInputStream::IMenus* menu = dynamic_cast<CDVDInputStream::IMenus*>(m_pInput);
// 	if (/*!menu &&*/ m_pFormatContext->start_time != (int64_t)AV_NOPTS_VALUE)
// 		starttime = (double)m_pFormatContext->start_time / AV_TIME_BASE;

	if (timestamp > starttime)
		timestamp -= starttime;
	// allow for largest possible difference in pts and dts for a single packet
	else if (timestamp + 0.5f > starttime)
		timestamp = 0;

	return timestamp*DVD_TIME_BASE;
}

DemuxPacket* CDVDDemuxFFmpeg::Read()
{
	DemuxPacket* pPacket = NULL;
	// on some cases where the received packet is invalid we will need to return an empty packet (0 length) otherwise the main loop (in CVideoPlayer)
	// would consider this the end of stream and stop.
	bool bReturnEmpty = false;
	{ CSingleLock lock(m_critSection); // open lock scope
	if (m_pFormatContext)
	{
		// assume we are not eof
		if (m_pFormatContext->pb)
			m_pFormatContext->pb->eof_reached = 0;

		// check for saved packet after a program change
		if (m_pkt.result < 0)
		{
			// keep track if ffmpeg doesn't always set these
			m_pkt.pkt.size = 0;
			m_pkt.pkt.data = NULL;

			// timeout reads after 100ms
			m_timeout.Set(20000);
			m_pkt.result = av_read_frame(m_pFormatContext, &m_pkt.pkt);
			m_timeout.SetInfinite();
		}

		if (m_pkt.result == AVERROR(EINTR) || m_pkt.result == AVERROR(EAGAIN))
		{
			// timeout, probably no real error, return empty packet
			bReturnEmpty = true;
		} else if (m_pkt.result < 0)
		{
			Flush();
		} else if (IsProgramChange())
		{
			// update streams
			CreateStreams(m_program);

			pPacket = DemuxPacket::Allocate(0);
			pPacket->iStreamId = DMX_SPECIALID_STREAMCHANGE;
			pPacket->demuxerId = m_demuxerId;

			return pPacket;
		}
		// check size and stream index for being in a valid range
		else if (m_pkt.pkt.size < 0 ||
			m_pkt.pkt.stream_index < 0 ||
			m_pkt.pkt.stream_index >= (int)m_pFormatContext->nb_streams)
		{
			// XXX, in some cases ffmpeg returns a negative packet size
			if (m_pFormatContext->pb && !m_pFormatContext->pb->eof_reached)
			{
			//	CLog::Log(LOGERROR, "CDVDDemuxFFmpeg::Read() no valid packet");
				bReturnEmpty = true;
				Flush();
			}
			//else
			//	CLog::Log(LOGERROR, "CDVDDemuxFFmpeg::Read() returned invalid packet and eof reached");

			m_pkt.result = -1;
			av_packet_unref(&m_pkt.pkt);
		} else
		{
			ParsePacket(&m_pkt.pkt);

			AVStream *stream = m_pFormatContext->streams[m_pkt.pkt.stream_index];

			if (IsVideoReady())
			{
				if (m_program != UINT_MAX)
				{
					/* check so packet belongs to selected program */
					for (unsigned int i = 0; i < m_pFormatContext->programs[m_program]->nb_stream_indexes; i++)
					{
						if (m_pkt.pkt.stream_index == (int)m_pFormatContext->programs[m_program]->stream_index[i])
						{
							pPacket = DemuxPacket::Allocate(m_pkt.pkt.size);
							break;
						}
					}

					if (!pPacket)
						bReturnEmpty = true;
				} else
					pPacket = DemuxPacket::Allocate(m_pkt.pkt.size);
			} else
				bReturnEmpty = true;

			if (pPacket)
			{
				if (m_bMatroska && stream->codec && stream->codec->codec_type == AVMEDIA_TYPE_VIDEO)
				{ // matroska can store different timestamps
					// for different formats, for native stored
					// stuff it is pts, but for ms compatibility
					// tracks, it is really dts. sadly ffmpeg
					// sets these two timestamps equal all the
					// time, so we select it here instead
					if (stream->codec->codec_tag == 0)
						m_pkt.pkt.dts = AV_NOPTS_VALUE;
					else
						m_pkt.pkt.pts = AV_NOPTS_VALUE;
				}

				if (m_bAVI && stream->codec && stream->codec->codec_type == AVMEDIA_TYPE_VIDEO)
				{
					// AVI's always have borked pts, specially if m_pFormatContext->flags includes
					// AVFMT_FLAG_GENPTS so always use dts
					m_pkt.pkt.pts = AV_NOPTS_VALUE;
				}

				// copy contents into our own packet
				pPacket->iSize = m_pkt.pkt.size;

				// maybe we can avoid a memcpy here by detecting where pkt.destruct is pointing too?
				if (m_pkt.pkt.data)
					memcpy(pPacket->pData, m_pkt.pkt.data, pPacket->iSize);

				pPacket->pts = ConvertTimestamp(m_pkt.pkt.pts, stream->time_base.den, stream->time_base.num);
				pPacket->dts = ConvertTimestamp(m_pkt.pkt.dts, stream->time_base.den, stream->time_base.num);
				pPacket->duration = DVD_SEC_TO_TIME((double)m_pkt.pkt.duration * stream->time_base.num / stream->time_base.den);

// 				CDVDInputStream::IDisplayTime *inputStream = m_pInput->GetIDisplayTime();
// 				if (inputStream)
// 				{
// 					int dispTime = inputStream->GetTime();
// 					if (m_displayTime != dispTime)
// 					{
// 						m_displayTime = dispTime;
// 						if (pPacket->dts != DVD_NOPTS_VALUE)
// 						{
// 							m_dtsAtDisplayTime = pPacket->dts;
// 						}
// 					}
// 					if (m_dtsAtDisplayTime != DVD_NOPTS_VALUE && pPacket->dts != DVD_NOPTS_VALUE)
// 					{
// 						pPacket->dispTime = m_displayTime;
// 						pPacket->dispTime += DVD_TIME_TO_MSEC(pPacket->dts - m_dtsAtDisplayTime);
// 					}
// 				}

				// used to guess streamlength
				if (pPacket->dts != DVD_NOPTS_VALUE && (pPacket->dts > m_currentPts || m_currentPts == DVD_NOPTS_VALUE))
					m_currentPts = pPacket->dts;


				// check if stream has passed full duration, needed for live streams
				bool bAllowDurationExt = (stream->codec && (stream->codec->codec_type == AVMEDIA_TYPE_VIDEO || stream->codec->codec_type == AVMEDIA_TYPE_AUDIO));
				if (bAllowDurationExt && m_pkt.pkt.dts != (int64_t)AV_NOPTS_VALUE)
				{
					int64_t duration;
					duration = m_pkt.pkt.dts;
					if (stream->start_time != (int64_t)AV_NOPTS_VALUE)
						duration -= stream->start_time;

					if (duration > stream->duration)
					{
						stream->duration = duration;
						duration = av_rescale_rnd(stream->duration, (int64_t)stream->time_base.num * AV_TIME_BASE, stream->time_base.den, AV_ROUND_NEAR_INF);
						if ((m_pFormatContext->duration == (int64_t)AV_NOPTS_VALUE)
							|| (m_pFormatContext->duration != (int64_t)AV_NOPTS_VALUE && duration > m_pFormatContext->duration))
							m_pFormatContext->duration = duration;
					}
				}

				// store internal id until we know the continuous id presented to player
				// the stream might not have been created yet
				pPacket->iStreamId = m_pkt.pkt.stream_index;
			}
			m_pkt.result = -1;
			av_packet_unref(&m_pkt.pkt);
		}
	}
	} // end of lock scope
	if (bReturnEmpty && !pPacket)
		pPacket = DemuxPacket::Allocate(0);

	if (!pPacket)
		return nullptr;

	// check streams, can we make this a bit more simple?
	if (pPacket && pPacket->iStreamId >= 0)
	{
		CDemuxStream *stream = GetStream(pPacket->iStreamId);
		if (!stream ||
			stream->pPrivate != m_pFormatContext->streams[pPacket->iStreamId] ||
			stream->codec != m_pFormatContext->streams[pPacket->iStreamId]->codec->codec_id)
		{
			// content has changed, or stream did not yet exist
			stream = AddStream(pPacket->iStreamId);
		}
		// we already check for a valid m_streams[pPacket->iStreamId] above
		else if (stream->type == STREAM_AUDIO)
		{
			if (((CDemuxStreamAudio*)stream)->iChannels != m_pFormatContext->streams[pPacket->iStreamId]->codec->channels ||
				((CDemuxStreamAudio*)stream)->iSampleRate != m_pFormatContext->streams[pPacket->iStreamId]->codec->sample_rate)
			{
				// content has changed
				stream = AddStream(pPacket->iStreamId);
			}
		} else if (stream->type == STREAM_VIDEO)
		{
			if (((CDemuxStreamVideo*)stream)->iWidth != m_pFormatContext->streams[pPacket->iStreamId]->codec->width ||
				((CDemuxStreamVideo*)stream)->iHeight != m_pFormatContext->streams[pPacket->iStreamId]->codec->height)
			{
				// content has changed
				stream = AddStream(pPacket->iStreamId);
			}
		}
		if (!stream)
		{
		//	CLog::Log(LOGERROR, "CDVDDemuxFFmpeg::AddStream - internal error, stream is null");
			DemuxPacket::Free(pPacket);
			return NULL;
		}

		pPacket->iStreamId = stream->uniqueId;
		pPacket->demuxerId = m_demuxerId;
	}
	return pPacket;
}

bool CDVDDemuxFFmpeg::SeekTime(int time, bool backwords, double *startpts)
{
	bool hitEnd = false;

	if (!m_pInput)
		return false;

	if (time < 0)
	{
		time = 0;
		hitEnd = true;
	}

	m_pkt.result = -1;
	av_packet_unref(&m_pkt.pkt);

// 	CDVDInputStream::IPosTime* ist = m_pInput->GetIPosTime();
// 	if (ist)
// 	{
// 		if (!ist->PosTime(time))
// 			return false;
// 
// 		if (startpts)
// 			*startpts = DVD_NOPTS_VALUE;
// 
// 		Flush();
// 
// 		return true;
// 	}

// 	if (!m_pInput->Seek(0, SEEK_POSSIBLE) &&
// 		!m_pInput->IsStreamType(DVDSTREAM_TYPE_FFMPEG))
// 	{
// 		CLog::Log(LOGDEBUG, "%s - input stream reports it is not seekable", __FUNCTION__);
// 		return false;
// 	}

	int64_t seek_pts = (int64_t)time * (AV_TIME_BASE / 1000);
	// most demuxer seek under dts
//	bool ismp3 = m_pFormatContext->iformat && (strcmp(m_pFormatContext->iformat->name, "mp3") == 0);
// 	if (m_pFormatContext->start_time != (int64_t)AV_NOPTS_VALUE && !ismp3)
// 		seek_pts += m_pFormatContext->start_time;

	int ret;
	{
		CSingleLock lock(m_critSection);
		ret = av_seek_frame(m_pFormatContext, -1, seek_pts, backwords ? AVSEEK_FLAG_BACKWARD : 0);

		// demuxer can return failure, if seeking behind eof
		if (ret < 0 && m_pFormatContext->duration &&
			seek_pts >= (m_pFormatContext->duration /*+ m_pFormatContext->start_time*/))
		{
			// force eof
			// files of realtime streams may grow
// 			if (!m_pInput->IsRealtime())
// 				m_pInput->Close();
// 			else
// 				ret = 0;
		} else if (ret < 0 /*&& m_pInput->IsEOF()*/)
			ret = 0;

		if (ret >= 0)
			UpdateCurrentPTS();
	}

// 	if (m_currentPts == DVD_NOPTS_VALUE)
// 		CLog::Log(LOGDEBUG, "%s - unknown position after seek", __FUNCTION__);
// 	else
// 		CLog::Log(LOGDEBUG, "%s - seek ended up on time %d", __FUNCTION__, (int)(m_currentPts / DVD_TIME_BASE * 1000));

	// in this case the start time is requested time
	if (startpts)
		*startpts = DVD_MSEC_TO_TIME(time);

	if (ret >= 0)
	{
		if (!hitEnd)
			return true;
		else
			return false;
	} else
		return false;
}

bool CDVDDemuxFFmpeg::SeekByte(int64_t pos)
{
	CSingleLock lock(m_critSection);
	int ret = av_seek_frame(m_pFormatContext, -1, pos, AVSEEK_FLAG_BYTE);

	if (ret >= 0)
		UpdateCurrentPTS();

	m_pkt.result = -1;
	av_packet_unref(&m_pkt.pkt);

	return (ret >= 0);
}

void CDVDDemuxFFmpeg::UpdateCurrentPTS()
{
	m_currentPts = DVD_NOPTS_VALUE;

	int idx = av_find_default_stream_index(m_pFormatContext);
	if (idx >= 0)
	{
		AVStream *stream = m_pFormatContext->streams[idx];
		if (stream && stream->cur_dts != (int64_t)AV_NOPTS_VALUE)
		{
			double ts = ConvertTimestamp(stream->cur_dts, stream->time_base.den, stream->time_base.num);
			m_currentPts = ts;
		}
	}
}

int CDVDDemuxFFmpeg::GetStreamLength()
{
	if (!m_pFormatContext)
		return 0;

	if (m_pFormatContext->duration < 0)
		return 0;

	return (int)(m_pFormatContext->duration / (AV_TIME_BASE / 1000));
}

/**
* @brief Finds stream based on unique id
*/
CDemuxStream* CDVDDemuxFFmpeg::GetStream(int iStreamId) const
{
	auto it = m_streams.find(iStreamId);
	if (it != m_streams.end())
		return it->second;

	return nullptr;
}

std::vector<CDemuxStream*> CDVDDemuxFFmpeg::GetStreams() const
{
	std::vector<CDemuxStream*> streams;

	for (auto& iter : m_streams)
		streams.push_back(iter.second);

	return streams;
}

int CDVDDemuxFFmpeg::GetNrOfStreams() const
{
	return m_streams.size();
}

double CDVDDemuxFFmpeg::SelectAspect(AVStream* st, bool& forced)
{
	// for stereo modes, use codec aspect ratio
	AVDictionaryEntry *entry = av_dict_get(st->metadata, "stereo_mode", NULL, 0);
	if (entry)
	{
		forced = false;
		return av_q2d(st->codecpar->sample_aspect_ratio);
	}

	// trust matroshka container
	if (m_bMatroska && st->sample_aspect_ratio.num != 0)
	{
		forced = true;
		return av_q2d(st->sample_aspect_ratio);
	}

	forced = false;
	/* if stream aspect is 1:1 or 0:0 use codec aspect */
	if ((st->sample_aspect_ratio.den == 1 || st->sample_aspect_ratio.den == 0) &&
		(st->sample_aspect_ratio.num == 1 || st->sample_aspect_ratio.num == 0) &&
		st->codec->sample_aspect_ratio.num != 0)
	{
		return av_q2d(st->codec->sample_aspect_ratio);
	}

	forced = true;
	if (st->sample_aspect_ratio.num != 0)
		return av_q2d(st->sample_aspect_ratio);

	return 0.0;
}

void CDVDDemuxFFmpeg::CreateStreams(unsigned int program)
{
	DisposeStreams();

	// add the ffmpeg streams to our own stream map
	if (m_pFormatContext->nb_programs)
	{
		// check if desired program is available
		if (program < m_pFormatContext->nb_programs && m_pFormatContext->programs[program]->nb_stream_indexes > 0)
		{
			m_program = program;
		} else
			m_program = UINT_MAX;

		// look for first non empty stream and discard nonselected programs
		for (unsigned int i = 0; i < m_pFormatContext->nb_programs; i++)
		{
			if (m_program == UINT_MAX && m_pFormatContext->programs[i]->nb_stream_indexes > 0)
			{
				m_program = i;
			}

			if (i != m_program)
				m_pFormatContext->programs[i]->discard = AVDISCARD_ALL;
		}
		if (m_program != UINT_MAX)
		{
			// add streams from selected program
			for (unsigned int i = 0; i < m_pFormatContext->programs[m_program]->nb_stream_indexes; i++)
			{
				AddStream(m_pFormatContext->programs[m_program]->stream_index[i]);
			}
		}
	} else
		m_program = UINT_MAX;

	// if there were no programs or they were all empty, add all streams
	if (m_program == UINT_MAX)
	{
		for (unsigned int i = 0; i < m_pFormatContext->nb_streams; i++)
			AddStream(i);
	}
}

void CDVDDemuxFFmpeg::DisposeStreams()
{
	std::map<int, CDemuxStream*>::iterator it;
	for (it = m_streams.begin(); it != m_streams.end(); ++it)
		delete it->second;
	m_streams.clear();
}

CDemuxStream* CDVDDemuxFFmpeg::AddStream(int streamIdx)
{
	AVStream* pStream = m_pFormatContext->streams[streamIdx];
	if (pStream)
	{
		CDemuxStream* stream = NULL;

		switch (pStream->codec->codec_type)
		{
		case AVMEDIA_TYPE_AUDIO:
		{
			CDemuxStreamAudioFFmpeg* st = new CDemuxStreamAudioFFmpeg(this, pStream);
			stream = st;
			st->iChannels = pStream->codec->channels;
			st->iSampleRate = pStream->codec->sample_rate;
			st->iBlockAlign = pStream->codec->block_align;
			st->iBitRate = pStream->codec->bit_rate;
			st->iBitsPerSample = pStream->codec->bits_per_raw_sample;
			st->iChannelLayout = pStream->codec->channel_layout;
			if (st->iBitsPerSample == 0)
				st->iBitsPerSample = pStream->codec->bits_per_coded_sample;

			if (av_dict_get(pStream->metadata, "title", NULL, 0))
				st->m_description = av_dict_get(pStream->metadata, "title", NULL, 0)->value;

			break;
		}
		case AVMEDIA_TYPE_VIDEO:
		{
			CDemuxStreamVideoFFmpeg* st = new CDemuxStreamVideoFFmpeg(this, pStream);
			stream = st;
			if (strcmp(m_pFormatContext->iformat->name, "flv") == 0)
				st->bVFR = true;
			else
				st->bVFR = false;

			// never trust pts in avi files with h264.
			if (m_bAVI && pStream->codec->codec_id == AV_CODEC_ID_H264)
				st->bPTSInvalid = true;

			AVRational r_frame_rate = av_stream_get_r_frame_rate(pStream);

			//average fps is more accurate for mkv files
			if (m_bMatroska && pStream->avg_frame_rate.den && pStream->avg_frame_rate.num)
			{
				st->iFpsRate = pStream->avg_frame_rate.num;
				st->iFpsScale = pStream->avg_frame_rate.den;
			} else if (r_frame_rate.den && r_frame_rate.num)
			{
				st->iFpsRate = r_frame_rate.num;
				st->iFpsScale = r_frame_rate.den;
			} else
			{
				st->iFpsRate = 0;
				st->iFpsScale = 0;
			}
			
			st->iWidth = pStream->codec->width;
			st->iHeight = pStream->codec->height;
			st->fAspect = SelectAspect(pStream, st->bForcedAspect) * pStream->codec->width / pStream->codec->height;
			st->iOrientation = 0;
			st->iBitsPerPixel = pStream->codec->bits_per_coded_sample;

			AVDictionaryEntry *rtag = av_dict_get(pStream->metadata, "rotate", NULL, 0);
			if (rtag)
				st->iOrientation = atoi(rtag->value);

			// detect stereoscopic mode
			std::string stereoMode = GetStereoModeFromMetadata(pStream->metadata);
			// check for metadata in file if detection in stream failed
			if (stereoMode.empty())
				stereoMode = GetStereoModeFromMetadata(m_pFormatContext->metadata);
			if (!stereoMode.empty())
				st->stereo_mode = stereoMode;
			
			if (av_dict_get(pStream->metadata, "title", NULL, 0))
				st->m_description = av_dict_get(pStream->metadata, "title", NULL, 0)->value;

			break;
		}
		case AVMEDIA_TYPE_DATA:
		{
			stream = new CDemuxStream();
			stream->type = STREAM_DATA;
			break;
		}
#if 0
		case AVMEDIA_TYPE_SUBTITLE:
		{
			if (pStream->codec->codec_id == AV_CODEC_ID_DVB_TELETEXT && CSettings::GetInstance().GetBool(CSettings::SETTING_VIDEOPLAYER_TELETEXTENABLED))
			{
				CDemuxStreamTeletext* st = new CDemuxStreamTeletext();
				stream = st;
				stream->type = STREAM_TELETEXT;
				break;
			} else
			{
				CDemuxStreamSubtitleFFmpeg* st = new CDemuxStreamSubtitleFFmpeg(this, pStream);
				stream = st;

				if (av_dict_get(pStream->metadata, "title", NULL, 0))
					st->m_description = av_dict_get(pStream->metadata, "title", NULL, 0)->value;

				break;
			}
		}
		case AVMEDIA_TYPE_ATTACHMENT:
		{ //mkv attachments. Only bothering with fonts for now.
			if (pStream->codec->codec_id == AV_CODEC_ID_TTF
				|| pStream->codec->codec_id == AV_CODEC_ID_OTF
				)
			{
				std::string fileName = "special://temp/fonts/";
				XFILE::CDirectory::Create(fileName);
				AVDictionaryEntry *nameTag = av_dict_get(pStream->metadata, "filename", NULL, 0);
				if (!nameTag)
				{
					CLog::Log(LOGERROR, "%s: TTF attachment has no name", __FUNCTION__);
				} else
				{
					fileName += nameTag->value;
					XFILE::CFile file;
					if (pStream->codec->extradata && file.OpenForWrite(fileName))
					{
						if (file.Write(pStream->codec->extradata, pStream->codec->extradata_size) != pStream->codec->extradata_size)
						{
							file.Close();
							XFILE::CFile::Delete(fileName);
							CLog::Log(LOGDEBUG, "%s: Error saving font file \"%s\"", __FUNCTION__, fileName.c_str());
						}
					}
				}
			}
			stream = new CDemuxStream();
			stream->type = STREAM_NONE;
			break;
		}
#endif
		default:
		{
			stream = new CDemuxStream();
			stream->type = STREAM_NONE;
			break;
		}
		}

		// generic stuff
		if (pStream->duration != (int64_t)AV_NOPTS_VALUE)
			stream->iDuration = (int)((pStream->duration / AV_TIME_BASE) & 0xFFFFFFFF);

		stream->codec = pStream->codec->codec_id;
		stream->codec_fourcc = pStream->codec->codec_tag;
		stream->profile = pStream->codec->profile;
		stream->level = pStream->codec->level;
		//stream->realtime = m_pInput->IsRealtime();
		stream->realtime = false;

		stream->source = STREAM_SOURCE_DEMUX;
		stream->pPrivate = pStream;
		stream->flags = (CDemuxStream::EFlags)pStream->disposition;

		AVDictionaryEntry *langTag = av_dict_get(pStream->metadata, "language", NULL, 0);
		if (!langTag)
		{
			// only for avi audio streams
			if ((strcmp(m_pFormatContext->iformat->name, "avi") == 0) && (pStream->codecpar && pStream->codecpar->codec_type == AVMEDIA_TYPE_AUDIO))
			{
				// only defined for streams 1 to 9
				if ((streamIdx > 0) && (streamIdx < 10))
				{
					// search for language information in RIFF-Header ("IAS1": first language - "IAS9": ninth language)
					char riff_tag_string[5] = { 'I', 'A', 'S', (char)(streamIdx + '0'), '\0' };
					langTag = av_dict_get(m_pFormatContext->metadata, riff_tag_string, NULL, 0);
					if (!langTag && (streamIdx == 1))
					{
						// search for language information in RIFF-Header ("ILNG": language)
						langTag = av_dict_get(m_pFormatContext->metadata, "language", NULL, 0);
					}
				}
			}
		}
		if (langTag)
			strncpy(stream->language, langTag->value, 3);

		if (stream->type != STREAM_NONE && pStream->codec->extradata && pStream->codec->extradata_size > 0)
		{
			stream->ExtraSize = pStream->codec->extradata_size;
			stream->ExtraData = new uint8_t[pStream->codec->extradata_size];
			memcpy(stream->ExtraData, pStream->codec->extradata, pStream->codec->extradata_size);
		}

#ifdef HAVE_LIBBLURAY
		if (m_pInput->IsStreamType(DVDSTREAM_TYPE_BLURAY))
		{
			static_cast<CDVDInputStreamBluray*>(m_pInput)->GetStreamInfo(pStream->id, stream->language);
			stream->dvdNavId = pStream->id;
		}
#endif

		stream->uniqueId = pStream->index;
		stream->demuxerId = m_demuxerId;

		AddStream(stream->uniqueId, stream);
		return stream;
	} else
		return NULL;
}

/**
* @brief Adds or updates a demux stream based in ffmpeg id
*/
void CDVDDemuxFFmpeg::AddStream(int streamIdx, CDemuxStream* stream)
{
	std::pair<std::map<int, CDemuxStream*>::iterator, bool> res;

	res = m_streams.insert(std::make_pair(streamIdx, stream));
	if (res.second)
	{
		/* was new stream */
		stream->uniqueId = streamIdx;
	} else
	{
		delete res.first->second;
		res.first->second = stream;
	}
// 	if (g_advancedSettings.m_logLevel > LOG_LEVEL_NORMAL)
// 		CLog::Log(LOGDEBUG, "CDVDDemuxFFmpeg::AddStream ID: %d", streamIdx);
}

std::string CDVDDemuxFFmpeg::GetStreamCodecName(int iStreamId)
{
	CDemuxStream *stream = GetStream(iStreamId);
	std::string strName;
	if (stream)
	{
#ifdef FF_PROFILE_DTS_HD_MA
		/* use profile to determine the DTS type */
		if (stream->codec == AV_CODEC_ID_DTS)
		{
			if (stream->profile == FF_PROFILE_DTS_HD_MA)
				strName = "dtshd_ma";
			else if (stream->profile == FF_PROFILE_DTS_HD_HRA)
				strName = "dtshd_hra";
			else
				strName = "dca";

			return strName;
		}
#endif

		AVCodec *codec = avcodec_find_decoder(stream->codec);
		if (codec)
			strName = codec->name;
	}
	return strName;
}

bool CDVDDemuxFFmpeg::IsProgramChange()
{
	if (m_program == UINT_MAX)
		return false;

	if (m_program == 0 && !m_pFormatContext->nb_programs)
		return false;

	if (m_pFormatContext->programs[m_program]->nb_stream_indexes != m_streams.size())
		return true;

	if (m_program >= m_pFormatContext->nb_programs)
		return true;

	for (unsigned int i = 0; i < m_pFormatContext->programs[m_program]->nb_stream_indexes; i++)
	{
		int idx = m_pFormatContext->programs[m_program]->stream_index[i];
		CDemuxStream *stream = GetStream(idx);
		if (!stream)
			return true;
		if (m_pFormatContext->streams[idx]->codec->codec_id != stream->codec)
			return true;
	}
	return false;
}

std::string CDVDDemuxFFmpeg::GetStereoModeFromMetadata(AVDictionary *pMetadata)
{
	std::string stereoMode;
	AVDictionaryEntry *tag = NULL;

	// matroska
	tag = av_dict_get(pMetadata, "stereo_mode", NULL, 0);
	if (tag && tag->value)
		stereoMode = tag->value;

	// asf / wmv
	if (stereoMode.empty())
	{
		tag = av_dict_get(pMetadata, "Stereoscopic", NULL, 0);
		if (tag && tag->value)
		{
			tag = av_dict_get(pMetadata, "StereoscopicLayout", NULL, 0);
			if (tag && tag->value)
				stereoMode = ConvertCodecToInternalStereoMode(tag->value, WmvToInternalStereoModeMap);
		}
	}

	return stereoMode;
}

std::string CDVDDemuxFFmpeg::ConvertCodecToInternalStereoMode(const std::string &mode, const StereoModeConversionMap *conversionMap)
{
	size_t i = 0;
	while (conversionMap[i].name)
	{
		if (mode == conversionMap[i].name)
			return conversionMap[i].mode;
		i++;
	}
	return "";
}

void CDVDDemuxFFmpeg::ParsePacket(AVPacket *pkt)
{
	AVStream *st = m_pFormatContext->streams[pkt->stream_index];
	CDemuxStream *stream = GetStream(pkt->stream_index);

	// if the stream is new, tell ffmpeg to parse the stream
	if (!stream && !st->parser)
	{
		st->need_parsing = AVSTREAM_PARSE_FULL;
	}

	// split extradata
	if (st->parser && st->parser->parser->split && !st->codec->extradata)
	{
		int i = st->parser->parser->split(st->codec, pkt->data, pkt->size);
		if (i > 0 && i < FF_MAX_EXTRADATA_SIZE)
		{
			// Found extradata, fill it in. This will cause
			// a new stream to be created and used.
			st->codec->extradata_size = i;
			st->codec->extradata = (uint8_t*)av_malloc(st->codec->extradata_size + FF_INPUT_BUFFER_PADDING_SIZE);
			if (st->codec->extradata)
			{
			//	CLog::Log(LOGDEBUG, "CDVDDemuxFFmpeg::Read() fetching extradata, extradata_size(%d)", st->codec->extradata_size);
				memcpy(st->codec->extradata, pkt->data, st->codec->extradata_size);
				memset(st->codec->extradata + i, 0, FF_INPUT_BUFFER_PADDING_SIZE);
			} else
			{
				st->codec->extradata_size = 0;
			}
		}
	}

	// for video we need a decoder to get desired information into codec context
	if (st->codec->codec_type == AVMEDIA_TYPE_VIDEO && st->codec->extradata &&
		(!st->codec->width || st->codec->pix_fmt == AV_PIX_FMT_NONE))
	{
		// open a decoder, it will be cleared down by ffmpeg on closing the stream
		if (!st->codec->codec)
		{
			const AVCodec* codec;
			AVDictionary *thread_opt = NULL;
			codec = avcodec_find_decoder(st->codec->codec_id);
			// Force thread count to 1 since the h264 decoder will not extract
			// SPS and PPS to extradata during multi-threaded decoding
			av_dict_set(&thread_opt, "threads", "1", 0);
			int res = avcodec_open2(st->codec, codec, &thread_opt);
// 			if (res < 0)
// 				CLog::Log(LOGERROR, "CDVDDemuxFFmpeg::ParsePacket() unable to open codec %d", res);
			av_dict_free(&thread_opt);
		}

		// We don't need to actually decode here
		// we just want to transport SPS data into codec context
		st->codec->skip_idct = AVDISCARD_ALL;
		// extradata is not decoded if skip_frame >= AVDISCARD_NONREF
		//    st->codec->skip_frame = AVDISCARD_ALL;
		st->codec->skip_loop_filter = AVDISCARD_ALL;

		// We are looking for an IDR frame
		AVFrame picture;
		memset(&picture, 0, sizeof(AVFrame));
		picture.pts = picture.pkt_dts = picture.pkt_pts = picture.best_effort_timestamp = AV_NOPTS_VALUE;
		picture.pkt_pos = -1;
		picture.key_frame = 1;
		picture.format = -1;

		int got_picture = 0;
		avcodec_decode_video2(st->codec, &picture, &got_picture, pkt);
		av_frame_unref(&picture);
	}
}

bool CDVDDemuxFFmpeg::IsVideoReady()
{
	AVStream *st;
	bool hasVideo = false;

	if (!m_checkvideo)
		return true;

	if (m_program == 0 && !m_pFormatContext->nb_programs)
		return false;

	if (m_program != UINT_MAX)
	{
		for (unsigned int i = 0; i < m_pFormatContext->programs[m_program]->nb_stream_indexes; i++)
		{
			int idx = m_pFormatContext->programs[m_program]->stream_index[i];
			st = m_pFormatContext->streams[idx];
			if (st->codec->codec_type == AVMEDIA_TYPE_VIDEO)
			{
				if (st->codec->width && st->codec->pix_fmt != AV_PIX_FMT_NONE)
					return true;
				hasVideo = true;
			}
		}
	} else
	{
		for (unsigned int i = 0; i < m_pFormatContext->nb_streams; i++)
		{
			st = m_pFormatContext->streams[i];
			if (st->codec->codec_type == AVMEDIA_TYPE_VIDEO)
			{
				if (st->codec->width && st->codec->pix_fmt != AV_PIX_FMT_NONE)
					return true;
				hasVideo = true;
			}
		}
	}
	return !hasVideo;
}

void CDVDDemuxFFmpeg::ResetVideoStreams()
{
	AVStream *st;
	for (unsigned int i = 0; i < m_pFormatContext->nb_streams; i++)
	{
		st = m_pFormatContext->streams[i];
		if (st->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			av_freep(&st->codec->extradata);
			st->codec->extradata_size = 0;
			st->codec->width = 0;
		}
	}
}

NS_KRMOVIE_END
