//---------------------------------------------------------------------------
/*
	Risa [�肳]      alias �g���g��3 [kirikiri-3]
	 stands for "Risa Is a Stagecraft Architecture"
	Copyright (C) 2000 W.Dee <dee@kikyou.info> and contributors

	See details of license at "license.txt"
*/
//---------------------------------------------------------------------------
//! @file
//! @brief Phase Vocoder �̎���
//---------------------------------------------------------------------------

/*
	Phase Vocoder (�t�F�[�Y �{�R�[�_ ; �ʑ��{�R�[�_)�̎���

	�Q�l����:

		http://www.panix.com/~jens/pvoc-dolson.par
			Phase Vocoder �̃`���[�g���A���B�u�~���[�W�V�����ɂ��킩��悤�Ɂv
			������Ă���A���w���s�t�����h���[�B

		http://www.dspdimension.com/
			����(�I�[�v���\�[�X�ł͂Ȃ�)�� Time Stretcher/Pitch Shifter��
			DIRAC��A�e��A���S���Y���̐����A
			Pitch Shifter �̐����I�ȃ\�[�X�R�[�h�ȂǁB

		http://soundlab.cs.princeton.edu/software/rt_pvc/
			real-time phase vocoder analysis/synthesis library + visualization
			�\�[�X����B
*/

#include "tjsCommHead.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include "MathAlgorithms.h"
#include "PhaseVocoderDSP.h"
#include "RealFFT.h"
#include <string.h>

#include "tjsUtils.h"

//#include "tvpgl_ia32_intf.h"
#include "DetectCPU.h"

extern void InterleaveOverlappingWindow(float * __restrict dest, const float * __restrict const * __restrict src,
	float * __restrict win, int numch, size_t srcofs, size_t len);
extern void DeinterleaveApplyingWindow(float * __restrict dest[], const float * __restrict src,
					float * __restrict win, int numch, size_t destofs, size_t len);
#if 0
extern void InterleaveOverlappingWindow_sse(float * __restrict dest, const float * __restrict const * __restrict src,
					float * __restrict win, int numch, size_t srcofs, size_t len);
extern void DeinterleaveApplyingWindow_sse(float * __restrict dest[], const float * __restrict src,
					float * __restrict win, int numch, size_t destofs, size_t len);
#endif


//---------------------------------------------------------------------------
tRisaPhaseVocoderDSP::tRisaPhaseVocoderDSP(
				unsigned int framesize,
				unsigned int frequency, unsigned int channels) :
					InputBuffer(framesize * 4 * channels),
					OutputBuffer(framesize * 4 * channels)
		// InputBuffer �͍Œ�ł�
		// channels * (framesize + (framesize/oversamp)) �K�v�ŁA
		// OutputBuffer �͍Œ�ł�
		// channels * (framesize + (framesize/oversamp)*MAX_TIME_SCALE) �K�v
{
	// �t�B�[���h�̏�����
	FFTWorkIp = NULL;
	FFTWorkW = NULL;
	InputWindow = NULL;
	OutputWindow = NULL;
	AnalWork = NULL;
	SynthWork = NULL;
	LastAnalPhase = NULL;
	LastSynthPhase = NULL;

	FrameSize = framesize;
	OverSampling = 8;
	Frequency = frequency;
	Channels = channels;
	InputHopSize = OutputHopSize = FrameSize / OverSampling;

	TimeScale = 1.0;
	FrequencyScale = 1.0;
	RebuildParams = true; // �K������Ƀp�����[�^���č\�z����悤�ɐ^

	LastSynthPhaseAdjustCounter = 0;

	try
	{
		// ���[�N�Ȃǂ̊m��
		AnalWork  = (float **)TJSAlignedAlloc(sizeof(float *) * Channels, 4);
		SynthWork = (float **)TJSAlignedAlloc(sizeof(float *) * Channels, 4);
		for(unsigned int ch = 0; ch < Channels; ch++)
			AnalWork[ch] = NULL, SynthWork[ch] = NULL;
		for(unsigned int ch = 0; ch < Channels; ch++)
		{
			AnalWork[ch]  = (float *)TJSAlignedAlloc(sizeof(float) * (FrameSize), 4);
			SynthWork[ch] = (float *)TJSAlignedAlloc(sizeof(float) * (FrameSize), 4);
		}

		LastAnalPhase = (float **)TJSAlignedAlloc(sizeof(float *) * Channels, 4);
		for(unsigned int ch = 0; ch < Channels; ch++)
			LastAnalPhase[ch] = NULL;
		for(unsigned int ch = 0; ch < Channels; ch++)
		{
			LastAnalPhase[ch] = (float *)TJSAlignedAlloc(sizeof(float) * (FrameSize/2), 4);
			memset(LastAnalPhase[ch], 0, FrameSize/2 * sizeof(float)); // 0 �ŃN���A
		}

		LastSynthPhase = (float **)TJSAlignedAlloc(sizeof(float *) * Channels, 4);
		for(unsigned int ch = 0; ch < Channels; ch++)
			LastSynthPhase[ch] = NULL;
		for(unsigned int ch = 0; ch < Channels; ch++)
		{
			LastSynthPhase[ch] = (float *)TJSAlignedAlloc(sizeof(float) * (FrameSize/2), 4);
			memset(LastSynthPhase[ch], 0, FrameSize/2 * sizeof(float)); // 0 �ŃN���A
		}

		FFTWorkIp = (int *)TJSAlignedAlloc(sizeof(int) * (static_cast<int>(2+sqrt((double)FrameSize/4))), 4);
		FFTWorkIp[0] = FFTWorkIp[1] = 0;
		FFTWorkW = (float *)TJSAlignedAlloc(sizeof(float) * (FrameSize/2), 4);
		InputWindow = (float *)TJSAlignedAlloc(sizeof(float) * FrameSize, 4);
		OutputWindow = (float *)TJSAlignedAlloc(sizeof(float) * FrameSize, 4);
	}
	catch(...)
	{
		Clear();
		__debugbreak(); throw;
	}

	// ���o�̓o�b�t�@�̓��e���N���A
	float *bufp1;
	size_t buflen1;
	float *bufp2;
	size_t buflen2;

	InputBuffer.GetWritePointer(InputBuffer.GetSize(),
							bufp1, buflen1, bufp2, buflen2);
	if(bufp1) memset(bufp1, 0, sizeof(float)*buflen1);
	if(bufp2) memset(bufp2, 0, sizeof(float)*buflen2);

	OutputBuffer.GetWritePointer(OutputBuffer.GetSize(),
							bufp1, buflen1, bufp2, buflen2);
	if(bufp1) memset(bufp1, 0, sizeof(float)*buflen1);
	if(bufp2) memset(bufp2, 0, sizeof(float)*buflen2);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
tRisaPhaseVocoderDSP::~tRisaPhaseVocoderDSP()
{
	Clear();
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void tRisaPhaseVocoderDSP::SetTimeScale(float v)
{
	if(TimeScale != v)
	{
		TimeScale = v;
		RebuildParams = true;
		InputHopSize = OutputHopSize = FrameSize / OverSampling;
		OutputHopSize = static_cast<unsigned int>(InputHopSize * TimeScale) & ~1;
			// �� �����ɃA���C��(�d�v)
			// ���f�� re,im, re,im, ... �̔z�񂪋tFFT�ɂ�蓯����(���f���̌��~2��)
			// PCM�T���v���ɕϊ�����邽�߁APCM�T���v�����Q���ň���Ȃ��ƂȂ�Ȃ�.
			// ���̎��ۂ� OutputHopSize �ɏ]���� ExactTimeScale ���v�Z�����.
	}
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void tRisaPhaseVocoderDSP::SetFrequencyScale(float v)
{
	if(FrequencyScale != v)
	{
		FrequencyScale = v;
		RebuildParams = true;
	}
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void tRisaPhaseVocoderDSP::SetOverSampling(unsigned int v)
{
	if(v == 0)
	{
		// TimeScale �ɏ]���Ēl��ݒ�
		// ������臒l�͎��ۂ̃��X�j���O�ɂ�茈�肳�ꂽ���l�ł���A
		// �_���I�ȍ����͂Ȃ��B
		if(TimeScale <= 0.2) v = 2;
		else if(TimeScale <= 1.2) v = 4;
		else v = 8;
	}

	if(OverSampling != v)
	{
		OverSampling = v;
		InputHopSize = OutputHopSize = FrameSize / OverSampling;
		OutputHopSize = static_cast<unsigned int>(InputHopSize * TimeScale) & ~1;
		// ������OutputHopSize�̌v�Z�ɂ��Ă� tRisaPhaseVocoderDSP::SetTimeScale
		// ���Q�Ƃ̂���
		RebuildParams = true;
	}
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void tRisaPhaseVocoderDSP::Clear()
{
	// �S�Ẵo�b�t�@�Ȃǂ��������
	if(AnalWork)
	{
		for(unsigned int ch = 0; ch < Channels; ch++)
			TJSAlignedDealloc(AnalWork[ch]), AnalWork[ch] = NULL;
		TJSAlignedDealloc(AnalWork), AnalWork = NULL;
	}
	if(SynthWork)
	{
		for(unsigned int ch = 0; ch < Channels; ch++)
			TJSAlignedDealloc(SynthWork[ch]), SynthWork[ch] = NULL;
		TJSAlignedDealloc(SynthWork), SynthWork = NULL;
	}
	if(LastAnalPhase)
	{
		for(unsigned int ch = 0; ch < Channels; ch++)
			TJSAlignedDealloc(LastAnalPhase[ch]), LastAnalPhase[ch] = NULL;
		TJSAlignedDealloc(LastAnalPhase), LastAnalPhase = NULL;
	}
	if(LastSynthPhase)
	{
		for(unsigned int ch = 0; ch < Channels; ch++)
			TJSAlignedDealloc(LastSynthPhase[ch]), LastSynthPhase[ch] = NULL;
		TJSAlignedDealloc(LastSynthPhase), LastSynthPhase = NULL;
	}
	TJSAlignedDealloc(FFTWorkIp), FFTWorkIp = NULL;
	TJSAlignedDealloc(FFTWorkW), FFTWorkW = NULL;
	TJSAlignedDealloc(InputWindow), InputWindow = NULL;
	TJSAlignedDealloc(OutputWindow), OutputWindow = NULL;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
size_t tRisaPhaseVocoderDSP::GetInputFreeSize()
{
	return InputBuffer.GetFreeSize() / Channels;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
bool tRisaPhaseVocoderDSP::GetInputBuffer(
	size_t numsamplegranules,
	float * & p1, size_t & p1size,
	float * & p2, size_t & p2size)
{
	size_t numsamples = numsamplegranules * Channels;

	if(InputBuffer.GetFreeSize() < numsamples) return false; // �\���ȋ󂫗e�ʂ��Ȃ�

	InputBuffer.GetWritePointer(numsamples, p1, p1size, p2, p2size);

	p1size /= Channels;
	p2size /= Channels;

	InputBuffer.AdvanceWritePos(numsamples);

	return true;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
size_t tRisaPhaseVocoderDSP::GetOutputReadySize()
{
	return OutputBuffer.GetDataSize() / Channels;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
bool tRisaPhaseVocoderDSP::GetOutputBuffer(
	size_t numsamplegranules,
	const float * & p1, size_t & p1size,
	const float * & p2, size_t & p2size)
{
	size_t numsamples = numsamplegranules * Channels;

	if(OutputBuffer.GetDataSize() < numsamples) return false; // �\���ȏ����ς݃T���v�����Ȃ�

	OutputBuffer.GetReadPointer(numsamples, p1, p1size, p2, p2size);

	p1size /= Channels;
	p2size /= Channels;

	OutputBuffer.AdvanceReadPos(numsamples);

	return true;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
tRisaPhaseVocoderDSP::tStatus tRisaPhaseVocoderDSP::Process()
{
	const static bool use_sse = false;
#if 0
			(TVPCPUType & TVP_CPU_HAS_MMX) &&
			(TVPCPUType & TVP_CPU_HAS_SSE) &&
			(TVPCPUType & TVP_CPU_HAS_CMOV);
#endif

	// �p�����[�^�̍Čv�Z�̕K�v������ꍇ�͍Čv�Z������
	if(RebuildParams)
	{
		// ���֐��̌v�Z(�����ł�Vorbis I ��)
		float recovery_of_loss_of_vorbis_window = 2.0;
				//         1            1         2
				//  2  =  ��  1dx  /   ��   vorbis (x) dx
				//         0            0
				// where vobis = vorbis I window function
		float output_volume =
			TimeScale / FrameSize  / sqrt(FrequencyScale) / OverSampling * 2 *
											recovery_of_loss_of_vorbis_window;
		for(unsigned int i = 0; i < FrameSize; i++)
		{
			double x = ((double)i+0.5)/FrameSize;
			double window = sin(M_PI/2*sin(M_PI*x)*sin(M_PI*x));
			InputWindow[i]  = (float)(window);
			OutputWindow[i] = (float)(window *output_volume);
		}

		// ���̂ق��̃p�����[�^�̍Čv�Z
		OverSamplingRadian = (float)((2.0*M_PI)/OverSampling);
		OverSamplingRadianRecp = (float)(1.0/OverSamplingRadian);
		FrequencyPerFilterBand = (float)((double)Frequency/FrameSize);
		FrequencyPerFilterBandRecp = (float)(1.0/FrequencyPerFilterBand);
		ExactTimeScale = (float)OutputHopSize / InputHopSize;

		// �t���O��|��
		RebuildParams = false;
	}

	// ���̓o�b�t�@���̃f�[�^�͏\�����H
	if(InputBuffer.GetDataSize() < FrameSize * Channels)
		return psInputNotEnough; // ����Ȃ�

	// �o�̓o�b�t�@�̋󂫂͏\�����H
	if(OutputBuffer.GetFreeSize() < FrameSize * Channels)
		return psOutputFull; // ����Ȃ�

	// ���ꂩ�珑���������Ƃ��� OutputBuffer �̗̈�̍Ō�� OutputHopSize �T���v��
	// �O���j���[���� 0 �Ŗ��߂� (�I�[�o�[���b�v���ɂ͂ݏo�������Ȃ̂�)
	{
		float *p1, *p2;
		size_t p1len, p2len;

		OutputBuffer.GetWritePointer(OutputHopSize*Channels,
				p1, p1len, p2, p2len, (FrameSize - OutputHopSize)*Channels);
		memset(p1, 0, p1len * sizeof(float));
		if(p2) memset(p2, 0, p2len * sizeof(float));
	}

	// ���֐���K�p���A���̓o�b�t�@���� AnalWork �ɓǂݍ���
	{
		const float *p1, *p2;
		size_t p1len, p2len;
		InputBuffer.GetReadPointer(FrameSize*Channels, p1, p1len, p2, p2len);
		p1len /= Channels;
		p2len /= Channels;
#if 0
		if( use_sse ) {
			DeinterleaveApplyingWindow_sse(AnalWork, p1, InputWindow, Channels, 0, p1len);
			if(p2)
				DeinterleaveApplyingWindow_sse(AnalWork, p2, InputWindow + p1len, Channels, p1len, p2len);
		} else {
			DeinterleaveApplyingWindow(AnalWork, p1, InputWindow, Channels, 0, p1len);
			if(p2)
				DeinterleaveApplyingWindow(AnalWork, p2, InputWindow + p1len, Channels, p1len, p2len);
		}
#else
		DeinterleaveApplyingWindow(AnalWork, p1, InputWindow, Channels, 0, p1len);
		if(p2)
			DeinterleaveApplyingWindow(AnalWork, p2, InputWindow + p1len, Channels, p1len, p2len);
#endif
	}

	// �`�����l�����Ƃɏ���
	for(unsigned int ch = 0; ch < Channels; ch++)
	{
		//------------------------------------------------
		// ���
		//------------------------------------------------

		// ���Z�̍������������s����
#if 0
		if(use_sse) ProcessCore_sse(ch);
		else ProcessCore(ch);
#else
		ProcessCore(ch);
#endif
	}

	// ���֐���K�p���ASynthWork ����o�̓o�b�t�@�ɏ�������
	{
		float *p1, *p2;
		size_t p1len, p2len;

		OutputBuffer.GetWritePointer(FrameSize*Channels, p1, p1len, p2, p2len);
		p1len /= Channels;
		p2len /= Channels;
#if 0
		if( use_sse ) {
			InterleaveOverlappingWindow_sse(p1, SynthWork, OutputWindow, Channels, 0, p1len);
			if(p2)
				InterleaveOverlappingWindow_sse(p2, SynthWork, OutputWindow + p1len, Channels, p1len, p2len);
		} else {
			InterleaveOverlappingWindow(p1, SynthWork, OutputWindow, Channels, 0, p1len);
			if(p2)
				InterleaveOverlappingWindow(p2, SynthWork, OutputWindow + p1len, Channels, p1len, p2len);
		}
#else
		InterleaveOverlappingWindow(p1, SynthWork, OutputWindow, Channels, 0, p1len);
		if(p2)
			InterleaveOverlappingWindow(p2, SynthWork, OutputWindow + p1len, Channels, p1len, p2len);
#endif
	}

	// LastSynthPhase ���Ē������邩
	LastSynthPhaseAdjustCounter += LastSynthPhaseAdjustIncrement;
	if(LastSynthPhaseAdjustCounter >= LastSynthPhaseAdjustInterval)
	{
		// LastSynthPhase ���Ē�������J�E���g�ɂȂ���
		LastSynthPhaseAdjustCounter = 0;

		// �����ōs�������� LastSynthPhase �� unwrapping �ł���B
		// LastSynthPhase �͈ʑ��̍����ݐς����̂ő傫�Ȑ��l�ɂȂ��Ă������A
		// �K���ȊԊu�ł���� unwrapping ���Ȃ��ƁA������(���l���傫������)���x
		// �������������A����ɍ������o���Ȃ��Ȃ��Ă��܂��B
		// �������A���x���ۂ����΂悢���߁A���񂱂� unwrapping ���s���K�v�͂Ȃ��B
		// �����ł� LastSynthPhaseAdjustInterval/LastSynthPhaseAdjustIncrement �񂲂Ƃɒ������s���B
		for(unsigned int ch = 0; ch < Channels; ch++)
		{
			unsigned int framesize_d2 = FrameSize / 2;
			for(unsigned int i = 0; i < framesize_d2; i++)
			{
				long int n = static_cast<long int>(LastSynthPhase[ch][i] / (2.0*M_PI));
				LastSynthPhase[ch][i] -= static_cast<float>(n * (2.0*M_PI));
			}
		}
	}

	// ���o�̓o�b�t�@�̃|�C���^��i�߂�
	OutputBuffer.AdvanceWritePos(OutputHopSize * Channels);
	InputBuffer.AdvanceReadPos(InputHopSize * Channels);

	// �X�e�[�^�X = no error
	return psNoError;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void tRisaPhaseVocoderDSP::ProcessCore(int ch)
{
	unsigned int framesize_d2 = FrameSize / 2;
	float * analwork = AnalWork[ch];
	float * synthwork = SynthWork[ch];

	// FFT �����s����
	rdft(FrameSize, 1, analwork, FFTWorkIp, FFTWorkW); // Real DFT
	analwork[1] = 0.0; // analwork[1] = nyquist freq. power (�ǂ����݂��g���Ȃ��̂�0��)

	if(FrequencyScale != 1.0)
	{
		// �e�t�B���^�o���h���Ƃɕϊ�
		//-- �e�t�B���^�o���h���Ƃ̉��ʂƎ��g�������߂�B
		//-- FFT �����s����Ɗe�t�B���^�o���h���Ƃ̒l���o�Ă��邪�A
		//-- �t�B���^�o���h�Ƃ����o���h�p�X�t�B���^�̕��̒���
		//-- ���g���̃s�[�N���{���͂ǂ��ɂ���̂��́A�O��v�Z����
		//-- �ʑ��Ƃ̍����Ƃ��Ă݂Ȃ��Ƃ킩��Ȃ��B
		for(unsigned int i = 0; i < framesize_d2; i ++)
		{
			// �������W�n���ɍ��W�n
			float re = analwork[i*2  ];
			float im = analwork[i*2+1];

			float mag = sqrt(re*re + im*im); // mag = ��(re^2+im^2)
			float ang = VFast_arctan2(im, re); // ang = atan(im/re)

			// �O��̈ʑ��Ƃ̍����Ƃ�
			// --����: �����Ŏg�p���Ă���FFT�p�b�P�[�W�́A
			// --      �\�[�X�擪�̎Q�l�����ȂǂŎ����Ă���FFT��
			// --      �o�͂���镡�f���̋������̕������t�Ȃ̂�
			// --      (�������łĂ���̂�)���ӂ��K�v�B�����ł�������
			// --      �t�̕��Ƃ��Ĉ����B
			float tmp = LastAnalPhase[ch][i] - ang;
			LastAnalPhase[ch][i] = ang; // ����̒l��ۑ�

			// over sampling �̉e�����l������
			// -- �ʏ�AFrameSize �� FFT �̂P�����ł���Ƃ�����A
			// -- ���x��₤���߁AOverSampling �{�̎����ŉ��Z�����Ă���B
			// -- ���̂��߂ɐ�����ʑ��̂�����C������B
			tmp -= i * OverSamplingRadian;

			// unwrapping ������
			// -- tmp �� -M_PI �` +M_PI �͈̔͂Ɏ��܂�悤�ɂ���
			tmp = WrapPi_F1(tmp);

			// -M_PI�`+M_PI��-1.0�`+1.0�̕ψʂɕϊ�
			tmp =  tmp * OverSamplingRadianRecp;

			// tmp ���t�B���^�o���h��������̎��g���̕ψʂɕϊ����A
			// ����Ƀt�B���^�o���h�̒������g�������Z����
			// -- i * FrequencyPerFilterBand �̓t�B���^�o���h�̒������g����
			// -- �\���Atmp * FrequencyPerFilterBand �� �t�B���^�o���h��������
			// -- �̎��g���̕ψʂ�\���B���������킹�������A���̃t�B���^
			// -- �o���h���ł́u�^�v�̎��g���ł���B
			float freq = (i + tmp) *FrequencyPerFilterBand;

			// analwork �ɒl���i�[����
			analwork[i*2  ] = mag;
			analwork[i*2+1] = freq;
		}


		//------------------------------------------------
		// �ϊ�
		//------------------------------------------------

		// ���g���������̃��T���v�����O���s��
		float FrequencyScale_rcp = 1.0f / FrequencyScale;
		for(unsigned int i = 0; i < framesize_d2; i ++)
		{
			// i �ɑΉ�����C���f�b�N�X�𓾂�
			float fi = i * FrequencyScale_rcp;

			// floor(x) �� floor(x) + 1 �̊ԂŃo�C���j�A��Ԃ��s��
			unsigned int index = static_cast<unsigned int>(fi); // floor
			float frac = fi - index;

			if(index + 1 < framesize_d2)
			{
				synthwork[i*2  ] =
					analwork[index*2  ] +
					frac * (analwork[index*2+2]-analwork[index*2  ]);
				synthwork[i*2+1] =
					FrequencyScale * (
					analwork[index*2+1] +
					frac * (analwork[index*2+3]-analwork[index*2+1]) );
			}
			else if(index < framesize_d2)
			{
				synthwork[i*2  ] = analwork[index*2  ];
				synthwork[i*2+1] = analwork[index*2+1] * FrequencyScale;
			}
			else
			{
				synthwork[i*2  ] = 0.0;
				synthwork[i*2+1] = 0.0;
			}
		}


		//------------------------------------------------
		// ����
		//------------------------------------------------

		// �e�t�B���^�o���h���Ƃɕϊ�
		// ��{�I�ɂ͉�͂̋t�ϊ��ł���
		for(unsigned int i = 0; i < framesize_d2; i ++)
		{
			float mag  = synthwork[i*2  ];
			float freq = synthwork[i*2+1];

			// ���g������e�t�B���^�o���h�̒������g�������Z���A
			// �t�B���^�o���h�̒������g�������-1.0�`+1.0�̕ψ�
			// �ɕϊ�����
			float tmp = freq * FrequencyPerFilterBandRecp - (float)i;

			// -1.0�`+1.0�̕ψʂ�-M_PI�`+M_PI�̈ʑ��ɕϊ�
			tmp =  tmp * OverSamplingRadian;

			// OverSampling �ɂ��ʑ��̕␳
			tmp += i   * OverSamplingRadian;

			// TimeScale �ɂ��ʑ��̕␳
			// TimeScale �ŏo�͂����Ԏ������ɂ̂т��(���邢�͏k�߂�)�A
			// �ʑ��̍���������ɔ����Ă̂т�(�k��)
			tmp *= ExactTimeScale;

			// �O��̈ʑ��Ɖ��Z����
			// �����ł��������̕������t�ɂȂ�̂Œ���
			LastSynthPhase[ch][i] -= tmp;
			float ang = LastSynthPhase[ch][i];

			// �ɍ��W�n���������W�n
			float c, s;
			VFast_sincos(ang, s, c);
			synthwork[i*2  ] = mag * c;
			synthwork[i*2+1] = mag * s;
		}
	}
	else
	{
		// ���g���������ɃV�t�g���Ȃ��ꍇ

		// �e�t�B���^�o���h���Ƃɕϊ�
		//-- �e�t�B���^�o���h���Ƃ̉��ʂƎ��g�������߂�B
		//-- FFT �����s����Ɗe�t�B���^�o���h���Ƃ̒l���o�Ă��邪�A
		//-- �t�B���^�o���h�Ƃ����o���h�p�X�t�B���^�̕��̒���
		//-- ���g���̃s�[�N���{���͂ǂ��ɂ���̂��́A�O��v�Z����
		//-- �ʑ��Ƃ̍����Ƃ��Ă݂Ȃ��Ƃ킩��Ȃ��B
		for(unsigned int i = 0; i < framesize_d2; i ++)
		{
			// �������W�n���ɍ��W�n
			float re = analwork[i*2  ];
			float im = analwork[i*2+1];

			float mag = sqrt(re*re + im*im); // mag = ��(re^2+im^2)
			float ang = VFast_arctan2(im, re); // ang = atan(im/re)

			// �O��̈ʑ��Ƃ̍����Ƃ�
			// --����: �����Ŏg�p���Ă���FFT�p�b�P�[�W�́A
			// --      �\�[�X�擪�̎Q�l�����ȂǂŎ����Ă���FFT��
			// --      �o�͂���镡�f���̋������̕������t�Ȃ̂�
			// --      (�������łĂ���̂�)���ӂ��K�v�B�����ł�������
			// --      �t�̕��Ƃ��Ĉ����B
			float tmp = LastAnalPhase[ch][i] - ang;
			LastAnalPhase[ch][i] = ang; // ����̒l��ۑ�

			// phase shift
			float phase_shift = i * OverSamplingRadian;

			// over sampling �̉e�����l������
			// -- �ʏ�AFrameSize �� FFT �̂P�����ł���Ƃ�����A
			// -- ���x��₤���߁AOverSampling �{�̎����ŉ��Z�����Ă���B
			// -- ���̂��߂ɐ�����ʑ��̂�����C������B
			tmp -= phase_shift;

			// unwrapping ������
			// -- tmp �� -M_PI �` +M_PI �͈̔͂Ɏ��܂�悤�ɂ���
			tmp = WrapPi_F1(tmp);

//--
			// OverSampling �ɂ��ʑ��̕␳
			tmp += phase_shift;

			// TimeScale �ɂ��ʑ��̕␳
			// TimeScale �ŏo�͂����Ԏ������ɂ̂т��(���邢�͏k�߂�)�A
			// �ʑ��̍���������ɔ����Ă̂т�(�k��)
			tmp *= ExactTimeScale;

			// �O��̈ʑ��Ɖ��Z����
			// �����ł��������̕������t�ɂȂ�̂Œ���
			LastSynthPhase[ch][i] -= tmp;
			ang = LastSynthPhase[ch][i];

			// �ɍ��W�n���������W�n
			float c, s;
			VFast_sincos(ang, s, c);
			synthwork[i*2  ] = mag * c;
			synthwork[i*2+1] = mag * s;
		}
	}

	// FFT �����s����
	synthwork[1] = 0.0; // synthwork[1] = nyquist freq. power (�ǂ����݂��g���Ȃ��̂�0��)
	rdft(FrameSize, -1, SynthWork[ch], FFTWorkIp, FFTWorkW); // Inverse Real DFT
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


/*
	���̃\�[�X�R�[�h�ł͏ڂ����A���S���Y���̐����͍s��Ȃ��B
	��{�I�ȗ���̓v���[����C����łƕς��Ȃ��̂ŁA
	../opt_default/PhaseVocoderDSP_default.cpp ���Q�Ƃ̂��ƁB
*/

//---------------------------------------------------------------------------
#if 0
//---------------------------------------------------------------------------
void tRisaPhaseVocoderDSP::ProcessCore_sse(int ch)
{
	unsigned int framesize_d2 = FrameSize / 2;
	float * analwork = AnalWork[ch];
	float * synthwork = SynthWork[ch];

	// �ۂ߃��[�h��ݒ�
	SetRoundingModeToNearest_SSE();

	// FFT �����s����
	rdft(FrameSize, 1, analwork, FFTWorkIp, FFTWorkW); // Real DFT
	analwork[1] = 0.0; // analwork[1] = nyquist freq. power (�ǂ����݂��g���Ȃ��̂�0��)

	__m128 exact_time_scale = _mm_load1_ps(&ExactTimeScale);
	__m128 over_sampling_radian_v = _mm_load1_ps(&OverSamplingRadian);

	if(FrequencyScale != 1.0)
	{
		// �����ł� 4 ���f�� (8����) ���Ƃɏ������s���B
		__m128 over_sampling_radian_recp = _mm_load1_ps(&OverSamplingRadianRecp);
		__m128 frequency_per_filter_band = _mm_load1_ps(&FrequencyPerFilterBand);
		__m128 frequency_per_filter_band_recp = _mm_load1_ps(&FrequencyPerFilterBandRecp);

		for(unsigned int i = 0; i < framesize_d2; i += 4)
		{
			// �C���^�[���[�u���� +  �������W�n���ɍ��W�n
			__m128 aw3120 = *(__m128*)(analwork + i*2    );
			__m128 aw7654 = *(__m128*)(analwork + i*2 + 4);

			__m128 re3210 = _mm_shuffle_ps(aw3120, aw7654, _MM_SHUFFLE(2,0,2,0));
			__m128 im3210 = _mm_shuffle_ps(aw3120, aw7654, _MM_SHUFFLE(3,1,3,1));

			__m128 mag = _mm_sqrt_ps(_mm_add_ps(_mm_mul_ps(re3210,re3210), _mm_mul_ps(im3210,im3210)));
			__m128 ang = VFast_arctan2_F4_SSE(im3210, re3210);

			// �O��̈ʑ��Ƃ̍����Ƃ�
			__m128 lastp = *(__m128*)(LastAnalPhase[ch] + i);
			*(__m128*)(LastAnalPhase[ch] + i) = ang;
			ang = _mm_sub_ps(lastp, ang);

			// over sampling �̉e�����l������
			__m128 i_3210;
			i_3210 = _mm_cvtsi32_ss(i_3210, i);
			i_3210 = _mm_shuffle_ps(i_3210, i_3210, _MM_SHUFFLE(0,0,0,0));
			i_3210 = _mm_add_ps( i_3210, PM128(PFV_INIT) );

			__m128 phase_shift = _mm_mul_ps(i_3210, over_sampling_radian_v);
			ang = _mm_sub_ps( ang, phase_shift );

			// unwrapping ������
			ang = Wrap_Pi_F4_SSE(ang);

			// -M_PI�`+M_PI��-1.0�`+1.0�̕ψʂɕϊ�
			ang = _mm_mul_ps( ang, over_sampling_radian_recp );

			// tmp ���t�B���^�o���h��������̎��g���̕ψʂɕϊ����A
			// ����Ƀt�B���^�o���h�̒������g�������Z����
			__m128 freq = _mm_mul_ps( _mm_add_ps(ang, i_3210), frequency_per_filter_band );

			// analwork �ɒl���i�[����
			re3210 = mag;
			im3210 = freq;
			__m128 im10re10 = _mm_movelh_ps(re3210, im3210);
			__m128 im32re32 = _mm_movehl_ps(im3210, re3210);
			__m128 im1re1im0re0 = _mm_shuffle_ps(im10re10, im10re10, _MM_SHUFFLE(3,1,2,0));
			__m128 im3re3im2re2 = _mm_shuffle_ps(im32re32, im32re32, _MM_SHUFFLE(3,1,2,0));
			*(__m128*)(analwork + i*2    ) = im1re1im0re0;
			*(__m128*)(analwork + i*2 + 4) = im3re3im2re2;
		}


		//------------------------------------------------
		// �ϊ�
		//------------------------------------------------
		// ���g���������̃��T���v�����O���s��
		float FrequencyScale_rcp = 1.0f / FrequencyScale;
		for(unsigned int i = 0; i < framesize_d2; i ++)
		{
			// i �ɑΉ�����C���f�b�N�X�𓾂�
			float fi = i * FrequencyScale_rcp;

			// floor(x) �� floor(x) + 1 �̊ԂŃo�C���j�A��Ԃ��s��
			unsigned int index = static_cast<unsigned int>(fi); // floor
			float frac = fi - index;

			if(index + 1 < framesize_d2)
			{
				synthwork[i*2  ] =
					analwork[index*2  ] +
					frac * (analwork[index*2+2]-analwork[index*2  ]);
				synthwork[i*2+1] =
					FrequencyScale * (
					analwork[index*2+1] +
					frac * (analwork[index*2+3]-analwork[index*2+1]) );
			}
			else if(index < framesize_d2)
			{
				synthwork[i*2  ] = analwork[index*2  ];
				synthwork[i*2+1] = analwork[index*2+1] * FrequencyScale;
			}
			else
			{
				synthwork[i*2  ] = 0.0;
				synthwork[i*2+1] = 0.0;
			}
		}

		//------------------------------------------------
		// ����
		//------------------------------------------------

		// �e�t�B���^�o���h���Ƃɕϊ�
		// ��{�I�ɂ͉�͂̋t�ϊ��ł���
		for(unsigned int i = 0; i < framesize_d2; i += 4)
		{
			// �C���^�[���[�u����
			__m128 sw3120 = *(__m128*)(synthwork + i*2    );
			__m128 sw7654 = *(__m128*)(synthwork + i*2 + 4);

			__m128 mag  = _mm_shuffle_ps(sw3120, sw7654, _MM_SHUFFLE(2,0,2,0));
			__m128 freq = _mm_shuffle_ps(sw3120, sw7654, _MM_SHUFFLE(3,1,3,1));

			// i+3 i+2 i+1 i+0 ������
			__m128 i_3210;
			i_3210 = _mm_cvtsi32_ss(i_3210, i);
			i_3210 = _mm_shuffle_ps(i_3210, i_3210, _MM_SHUFFLE(0,0,0,0));
			i_3210 = _mm_add_ps(i_3210, PM128(PFV_INIT));

			// ���g������e�t�B���^�o���h�̒������g�������Z���A
			// �t�B���^�o���h�̒������g�������-1.0�`+1.0�̕ψ�
			// �ɕϊ�����
			__m128 ang = _mm_sub_ps(_mm_mul_ps(freq, frequency_per_filter_band_recp), i_3210);

			// -1.0�`+1.0�̕ψʂ�-M_PI�`+M_PI�̈ʑ��ɕϊ�
			ang = _mm_mul_ps( ang, over_sampling_radian_v );

			// OverSampling �ɂ��ʑ��̕␳
			ang = _mm_add_ps( ang, _mm_mul_ps( i_3210, over_sampling_radian_v ) );

			// TimeScale �ɂ��ʑ��̕␳
			ang = _mm_mul_ps( ang, exact_time_scale );

			// �O��̈ʑ��Ɖ��Z����
			// �����ł��������̕������t�ɂȂ�̂Œ���
			ang = _mm_sub_ps( *(__m128*)(LastSynthPhase[ch] + i), ang );
			*(__m128*)(LastSynthPhase[ch] + i) = ang;

			// �ɍ��W�n���������W�n
			__m128 sin, cos;
			VFast_sincos_F4_SSE(ang, sin, cos);
			__m128 re3210 = _mm_mul_ps( mag, cos );
			__m128 im3210 = _mm_mul_ps( mag, sin );

			// �C���^�[���[�u
			__m128 im10re10 = _mm_movelh_ps(re3210, im3210);
			__m128 im32re32 = _mm_movehl_ps(im3210, re3210);
			__m128 im1re1im0re0 = _mm_shuffle_ps(im10re10, im10re10, _MM_SHUFFLE(3,1,2,0));
			__m128 im3re3im2re2 = _mm_shuffle_ps(im32re32, im32re32, _MM_SHUFFLE(3,1,2,0));
			*(__m128*)(synthwork + i*2    ) = im1re1im0re0;
			*(__m128*)(synthwork + i*2 + 4) = im3re3im2re2;
		}
	}
	else
	{
		// ���g���������ɃV�t�g���Ȃ��ꍇ
		// �����ł� 4 ���f�� (8����) ���Ƃɏ������s���B
		for(unsigned int i = 0; i < framesize_d2; i += 4)
		{
			// �C���^�[���[�u���� +  �������W�n���ɍ��W�n
			__m128 aw3120 = *(__m128*)(analwork + i*2    );
			__m128 aw7654 = *(__m128*)(analwork + i*2 + 4);

			__m128 re3210 = _mm_shuffle_ps(aw3120, aw7654, _MM_SHUFFLE(2,0,2,0));
			__m128 im3210 = _mm_shuffle_ps(aw3120, aw7654, _MM_SHUFFLE(3,1,3,1));

			__m128 mag = _mm_sqrt_ps( _mm_add_ps(_mm_mul_ps(re3210,re3210), _mm_mul_ps(im3210,im3210)) );
			__m128 ang = VFast_arctan2_F4_SSE(im3210, re3210);

			// �O��̈ʑ��Ƃ̍����Ƃ�
			__m128 lastp = *(__m128*)(LastAnalPhase[ch] + i);
			*(__m128*)(LastAnalPhase[ch] + i) = ang;
			ang = _mm_sub_ps( lastp, ang );

			// over sampling �̉e�����l������
			__m128 i_3210;
			i_3210 = _mm_cvtsi32_ss(i_3210, i);
			i_3210 = _mm_shuffle_ps(i_3210, i_3210, _MM_SHUFFLE(0,0,0,0));
			i_3210 = _mm_add_ps( i_3210, PM128(PFV_INIT) );

			__m128 phase_shift = _mm_mul_ps( i_3210, over_sampling_radian_v );
			ang = _mm_sub_ps( ang, phase_shift );

			// unwrapping ������
			ang = Wrap_Pi_F4_SSE(ang);

			// OverSampling �ɂ��ʑ��̕␳
			ang = _mm_add_ps( ang, phase_shift );

			// TimeScale �ɂ��ʑ��̕␳
			ang = _mm_mul_ps( ang, exact_time_scale );

			// �O��̈ʑ��Ɖ��Z����
			// �����ł��������̕������t�ɂȂ�̂Œ���
			ang = _mm_sub_ps( *(__m128*)(LastSynthPhase[ch] + i), ang );
			*(__m128*)(LastSynthPhase[ch] + i) = ang;

			// �ɍ��W�n���������W�n
			__m128 sin, cos;
			VFast_sincos_F4_SSE(ang, sin, cos);
			re3210 = _mm_mul_ps( mag, cos );
			im3210 = _mm_mul_ps( mag, sin );

			// �C���^�[���[�u
			__m128 im10re10 = _mm_movelh_ps(re3210, im3210);
			__m128 im32re32 = _mm_movehl_ps(im3210, re3210);
			__m128 im1re1im0re0 = _mm_shuffle_ps(im10re10, im10re10, _MM_SHUFFLE(3,1,2,0));
			__m128 im3re3im2re2 = _mm_shuffle_ps(im32re32, im32re32, _MM_SHUFFLE(3,1,2,0));
			*(__m128*)(synthwork + i*2    ) = im1re1im0re0;
			*(__m128*)(synthwork + i*2 + 4) = im3re3im2re2;
		}
	}

	// FFT �����s����
	synthwork[1] = 0.0; // synthwork[1] = nyquist freq. power (�ǂ����݂��g���Ȃ��̂�0��)
	rdft_sse(FrameSize, -1, synthwork, FFTWorkIp, FFTWorkW); // Inverse Real DFT
}
//---------------------------------------------------------------------------
#endif
