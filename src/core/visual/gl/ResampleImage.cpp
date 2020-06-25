/******************************************************************************/
/**
 * �g��k������������
 * ----------------------------------------------------------------------------
 * 	Copyright (C) T.Imoto <http://www.kaede-software.com>
 * ----------------------------------------------------------------------------
 * @author		T.Imoto
 * @date		2014/04/02
 * @note
 *****************************************************************************/

#define _USE_MATH_DEFINES
#include "tjsCommHead.h"

#include <float.h>
#include <math.h>
#include <cmath>
#include <vector>

//#include "tvpgl_ia32_intf.h"
//#include "DetectCPU.h"
#include "LayerBitmapIntf.h"
#include "LayerBitmapImpl.h"
#include "WeightFunctor.h"
#include "ThreadIntf.h"

#include "aligned_allocator.h"
#include "ResampleImageInternal.h"


extern void TVPResampleImageAVX2( const tTVPResampleClipping &clip, const tTVPImageCopyFuncBase* blendfunc,
	iTVPBaseBitmap *dest, const tTVPRect &destrect, const iTVPBaseBitmap *src, const tTVPRect &srcrect,
	tTVPBBStretchType type, tjs_real typeopt );

extern void TVPResampleImageSSE2( const tTVPResampleClipping &clip, const tTVPImageCopyFuncBase* blendfunc,
	iTVPBaseBitmap *dest, const tTVPRect &destrect, const iTVPBaseBitmap *src, const tTVPRect &srcrect,
	tTVPBBStretchType type, tjs_real typeopt );

void tTVPBlendParameter::setFunctionFromParam() {
#define TVP_BLEND_4(basename) /* blend for 4 types (normal, opacity, HDA, HDA opacity) */ \
	if( opa_ == 255 ) {                                                                   \
		if( !hda_ ) {                                                                     \
			copy_func = basename;                                                         \
		} else {                                                                          \
			copy_func = basename##_HDA;                                                   \
		}                                                                                 \
	} else {                                                                              \
		if( !hda_ ) {                                                                     \
			blend_func = basename##_o;                                                    \
		} else {                                                                          \
			blend_func = basename##_HDA_o;                                                \
		}                                                                                 \
	}

	blend_func = NULL;
	copy_func = NULL;
	switch( method_ ) {
	case bmCopy:
		if( opa_ == 255 && hda_ ) {
			copy_func = TVPCopyColor;
		} else if( !hda_ ) {
			blend_func = TVPConstAlphaBlend;
		} else {
			blend_func = TVPConstAlphaBlend_HDA;
		}
		break;
	case bmCopyOnAlpha:
		if( opa_ == 255 )
			copy_func = TVPCopyOpaqueImage;
		else
			blend_func = TVPConstAlphaBlend_d;
		break;
	case bmAlpha:
		TVP_BLEND_4(TVPAlphaBlend);
		break;
	case bmAlphaOnAlpha:
		if( opa_ == 255 )
			copy_func = TVPAlphaBlend_d;
		else
			blend_func = TVPAlphaBlend_do;
		break;
	case bmAdd:
		TVP_BLEND_4(TVPAddBlend);
		break;
	case bmSub:
		TVP_BLEND_4(TVPSubBlend);
		break;
	case bmMul:
		TVP_BLEND_4(TVPMulBlend);
		break;
	case bmDodge:
		TVP_BLEND_4(TVPColorDodgeBlend);
		break;
	case bmDarken:
		TVP_BLEND_4(TVPDarkenBlend);
		break;
	case bmLighten:
		TVP_BLEND_4(TVPLightenBlend);
		break;
	case bmScreen:
		TVP_BLEND_4(TVPScreenBlend);
		break;
	case bmAddAlpha:
		TVP_BLEND_4(TVPAdditiveAlphaBlend);
		break;
	case bmAddAlphaOnAddAlpha:
		if( opa_ == 255 ) {
			copy_func = TVPAdditiveAlphaBlend_a;
		} else {
			blend_func = TVPAdditiveAlphaBlend_ao;
		}
		break;
	case bmAddAlphaOnAlpha:
		// Not yet implemented
		break;
	case bmAlphaOnAddAlpha:
		if( opa_ == 255 ) {
			copy_func = TVPAlphaBlend_a;
		} else {
			blend_func = TVPAlphaBlend_ao;
		}
		break;
	case bmCopyOnAddAlpha:
		if( opa_ == 255 )
			copy_func = TVPCopyOpaqueImage;
		else
			blend_func = TVPConstAlphaBlend_a;
		break;
	case bmPsNormal:
		TVP_BLEND_4(TVPPsAlphaBlend);
		break;
	case bmPsAdditive:
		TVP_BLEND_4(TVPPsAddBlend);
		break;
	case bmPsSubtractive:
		TVP_BLEND_4(TVPPsSubBlend);
		break;
	case bmPsMultiplicative:
		TVP_BLEND_4(TVPPsMulBlend);
		break;
	case bmPsScreen:
		TVP_BLEND_4(TVPPsScreenBlend);
		break;
	case bmPsOverlay:
		TVP_BLEND_4(TVPPsOverlayBlend);
		break;
	case bmPsHardLight:
		TVP_BLEND_4(TVPPsHardLightBlend);
		break;
	case bmPsSoftLight:
		TVP_BLEND_4(TVPPsSoftLightBlend);
		break;
	case bmPsColorDodge:
		TVP_BLEND_4(TVPPsColorDodgeBlend);
		break;
	case bmPsColorDodge5:
		TVP_BLEND_4(TVPPsColorDodge5Blend);
		break;
	case bmPsColorBurn:
		TVP_BLEND_4(TVPPsColorBurnBlend);
		break;
	case bmPsLighten:
		TVP_BLEND_4(TVPPsLightenBlend);
		break;
	case bmPsDarken:
		TVP_BLEND_4(TVPPsDarkenBlend);
		break;
	case bmPsDifference:
		TVP_BLEND_4(TVPPsDiffBlend);
		break;
	case bmPsDifference5:
		TVP_BLEND_4(TVPPsDiff5Blend);
		break;
	case bmPsExclusion:
		TVP_BLEND_4(TVPPsExclusionBlend);
		break;
	}
#undef TVP_BLEND_4
}

void tTVPResampleClipping::setClipping( const tTVPRect &cliprect, const tTVPRect &destrect ) {
	const int dstwidth = destrect.get_width();
	const int dstheight = destrect.get_height();

	offsetx_ = 0;
	offsety_ = 0;
	width_ = dstwidth;
	height_ = dstheight;
	dst_left_ = destrect.left;
	dst_top_ = destrect.top;

	// �㕔�N���b�s���O
	if( cliprect.top > destrect.top ) {
		offsety_ = cliprect.top - destrect.top;			// �N���b�s���O�I�t�Z�b�g
		dst_top_ = cliprect.top;
	}
	// �����N���b�s���O
	if( cliprect.bottom < destrect.bottom ) {
		height_ -= destrect.bottom - cliprect.bottom;	// �͂ݏo����
	}
	// ���N���b�s���O
	if( cliprect.left > destrect.left ) {
		offsetx_ = cliprect.left - destrect.left;	// �N���b�s���O�I�t�Z�b�g
		dst_left_ = cliprect.left;
	}
	// �E�N���b�s���O
	if( cliprect.right < destrect.right ) {
		width_ -= destrect.right - cliprect.right;	// �͂ݏo����
	}
}

/**
 * �e���ł̃E�F�C�g�����炩���ߌv�Z���Ă���
 */
template<typename TWeight=float, int NAlign=4>
struct AxisParam {
	typedef TWeight weight_t;
	typedef std::vector<weight_t,aligned_allocator<weight_t,NAlign> > weight_vector_t;
	static const int ALIGN_DIV = (NAlign/4);
	static const int ALIGN_OFFSET = ALIGN_DIV-1;

	std::vector<int> start_;	// �J�n�C���f�b�N�X
	std::vector<int> length_;	// �e�v�f����
	std::vector<int> min_length_;
	weight_vector_t weight_;

	static const int toAlign( int& length ) {
		length = ((length+ALIGN_OFFSET)/ALIGN_DIV)*ALIGN_DIV;
	}
};

static inline void AxisParamCalculateWeight( float* weight, float*& output, int& len, int leftedge, int rightedge ) {
	// len �ɂ͂͂ݏo���������܂܂�Ă���̂ŁA�܂��͂��̕������J�b�g����
	// ���[or�E�[�̎��A�͂ݏo�����̃E�F�C�g��[�ɉ��Z����
	if( leftedge ) {
		// ���[����͂ݏo���������Z
		int i = 1;
		for( ; i <= leftedge; i++ ) {
			weight[0] += weight[i];
		}
		// ���Z���������ړ�
		for( int j = 1; i < len; i++, j++ ) {
			weight[j] = weight[i];
		}
		// �͂ݏo�������̒������J�b�g
		len -= leftedge;
	}
	if( rightedge ) {
		// �E�[����͂ݏo���������Z
		int i = len - rightedge;
		int r = i - 1;
		for( ; i < len; i++ ) {
			weight[r] += weight[i];
		}
		// �͂ݏo�������̒������J�b�g
		len -= rightedge;
	}
	// ���v�l�����߂�
	float* w = weight;
	float sum = 0.0f;
	for( int i = 0; i < len; i++ ) {
		sum += *w;
		w++;
	}

	// EPSILON ��菬�����ꍇ�� 0 ��ݒ�
	float rcp;
	if( sum < FLT_EPSILON ) {
		rcp = 0.0f;
	} else {
		rcp = 1.0f / sum;
	}
	// ���K��
	w = weight;
	for( int i = 0; i < len; i++ ) {
		*output = (*w) * rcp;
		output++;
		w++;
	}
}

template<typename TParam, typename TWeightFunc>
static void AxisParamCalculateAxis( TParam& param, int srcstart, int srcend, int srclength, int dstlength, float tap, TWeightFunc& func) {
	param.start_.clear();
	param.start_.reserve( dstlength );
	param.length_.clear();
	param.length_.reserve( dstlength );
	// �܂��͋������v�Z
	if( srclength <= dstlength ) { // �g��
		float rangex = tap;
		int maxrange = ((int)rangex*2+2);
		std::vector<float> work( maxrange, 0.0f );
		float* weight = &work[0];
		int length = (dstlength * maxrange + dstlength);
#ifdef _DEBUG
		param.weight_.resize( length );
#else
		param.weight_.reserve( length );
#endif
		typename TParam::weight_t* output = &param.weight_[0];
		for( int x = 0; x < dstlength; x++ ) {
			float cx = (x+0.5f)*(float)srclength/(float)dstlength + srcstart;
			int left = (int)std::floor(cx-rangex);
			int right = (int)std::floor(cx+rangex);
			int start = left;
			int leftedge = 0;
			if( left < srcstart ) {
				leftedge = srcstart - left;
				start = srcstart;
			}
			int rightedge = 0;
			if( right >= srcend ) {
				rightedge = right - srcend;
			}
			param.start_.push_back( start );
			int len = right - left;
			float dist = left + 0.5f - cx;
			float* w = weight;
			for( int sx = 0; sx < len; sx++ ) {
				*w = func( std::abs(dist) );
				dist += 1.0f;
				w++;
			}
			AxisParamCalculateWeight( weight, output, len, leftedge, rightedge );
			param.length_.push_back( len );
		}
	} else { // �k��
		float rangex = tap*(float)srclength/(float)dstlength;
		int maxrange = ((int)rangex*2+2);
		std::vector<float> work( maxrange, 0.0f );
		float* weight = &work[0];
		int length = (srclength * maxrange + srclength);
#ifdef _DEBUG
		param.weight_.resize( length );
#else
		param.weight_.reserve( length );
#endif
		typename TParam::weight_t* output = &param.weight_[0];
		const float delta = (float)dstlength/(float)srclength; // �]������W�ł̈ʒu����
		for( int x = 0; x < dstlength; x++ ) {
			float cx = (x+0.5f)*(float)srclength/(float)dstlength + srcstart;
			int left = (int)std::floor(cx-rangex);
			int right = (int)std::floor(cx+rangex);
			int start = left;
			int leftedge = 0;
			if( left < srcstart ) {
				leftedge = srcstart - left;
				start = srcstart;
			}
			int rightedge = 0;
			if( right >= srcend ) {
				rightedge = right - srcend;
			}
			param.start_.push_back( start );
			// �]������W�ł̈ʒu
			int len = right-left;
			float dist = (left+0.5f-cx) * delta;
			float* w = weight;
			for( int sx = 0; sx < len; sx++ ) {
				*w = func( std::abs(dist) );
				dist += delta;
				w++;
			}
			AxisParamCalculateWeight( weight, output, len, leftedge, rightedge );
			param.length_.push_back( len );
		}
	}
}

/**
 * �̈敽�σo�[�W����
 */
template<typename TParam>
static void AxisParamCalculateAxisAreaAvg( TParam& param, int srcstart, int srcend, int srclength, int dstlength ) {
	if( dstlength <= srclength ) { // �k���̂�
		TVPCalculateAxisAreaAvg( srcstart, srcend, srclength, dstlength, param.start_, param.length_, param.weight_ );
		TVPNormalizeAxisAreaAvg( param.length_, param.weight_ );
	}
}


void TJS_USERENTRY ResamplerFunc( void* p );

class Resampler {
	AxisParam<> paramx_;
	AxisParam<> paramy_;

public:
	/** �}���`�X���b�h���p */
	struct ThreadParameter {
		Resampler* sampler_;
		int start_;
		int end_;
		int width_;

		const float* wstarty_;
		const iTVPBaseBitmap* src_;
		const tTVPRect* srcrect_;
		iTVPBaseBitmap* dest_;
		const tTVPRect* destrect_;

		const tTVPResampleClipping* clip_;
		const tTVPImageCopyFuncBase* blendfunc_;
	};

	/** �c�����̊g��k������ */
	inline void samplingVertical( int y, tjs_uint32* dstbits, int dstheight, int srcwidth, const iTVPBaseBitmap *src, const tTVPRect &srcrect, const float*& wstarty ) {
		const int top = paramy_.start_[y];
		const int len = paramy_.length_[y];
		const int bottom = top + len;
		const float* weighty = wstarty;
		const tjs_uint32* srctop = (const tjs_uint32*)src->GetScanLine(top) + srcrect.left;
		tjs_int stride = src->GetPitchBytes()/(int)sizeof(tjs_uint32);
		for( int x = 0; x < srcwidth; x++ ) {
			weighty = wstarty;
			float color_element[4] = {0.0f,0.0f,0.0f,0.0f};
			const tjs_uint32* src = &srctop[x];
			for( int sy = top; sy < bottom; sy++ ) {
				const float weight = *weighty;
				tjs_uint32 color = *src;
				color_element[0] += (color&0xff)*weight;
				color_element[1] += ((color>>8)&0xff)*weight;
				color_element[2] += ((color>>16)&0xff)*weight;
				color_element[3] += ((color>>24)&0xff)*weight;
				weighty++;
				src += stride;
			}
			tjs_uint32 color = (tjs_uint32)((color_element[0] > 255) ? 255 : (color_element[0] < 0) ? 0 : color_element[0]);
			color += (tjs_uint32)((color_element[1] > 255) ? 255 : (color_element[1] < 0) ? 0 : color_element[1]) << 8;
			color += (tjs_uint32)((color_element[2] > 255) ? 255 : (color_element[2] < 0) ? 0 : color_element[2]) << 16;
			color += (tjs_uint32)((color_element[3] > 255) ? 255 : (color_element[3] < 0) ? 0 : color_element[3]) << 24;
			*dstbits = color;
			dstbits++;
		}
		wstarty = weighty;
	}

	/** �������̊g��k������ */
	inline void samplingHorizontal( tjs_uint32* dstbits, const int offsetx, const int dstwidth, const tjs_uint32* srcbits ) {
		const float* weightx = &paramx_.weight_[0];
		// �܂�offset�����X�L�b�v
		for( int x = 0; x < offsetx; x++ ) {
			weightx += paramx_.length_[x];
		}
		const tjs_uint32* src = srcbits;
		for( int x = offsetx; x < dstwidth; x++ ) {
			const int left = paramx_.start_[x];
			int right = left + paramx_.length_[x];
			float color_element[4] = {0.0f,0.0f,0.0f,0.0f};
			for( int sx = left; sx < right; sx++ ) {
				const float weight = *weightx;
				tjs_uint32 color = srcbits[sx];
				color_element[0] += (color&0xff)*weight;
				color_element[1] += ((color>>8)&0xff)*weight;
				color_element[2] += ((color>>16)&0xff)*weight;
				color_element[3] += ((color>>24)&0xff)*weight;
				weightx++;
			}
			tjs_uint32 color = (tjs_uint32)((color_element[0] > 255) ? 255 : (color_element[0] < 0) ? 0 : color_element[0]);
			color += (tjs_uint32)((color_element[1] > 255) ? 255 : (color_element[1] < 0) ? 0 : color_element[1]) << 8;
			color += (tjs_uint32)((color_element[2] > 255) ? 255 : (color_element[2] < 0) ? 0 : color_element[2]) << 16;
			color += (tjs_uint32)((color_element[3] > 255) ? 255 : (color_element[3] < 0) ? 0 : color_element[3]) << 24;
			*dstbits = color;
			dstbits++;
		}
	}

	void ResampleImage( const tTVPResampleClipping &clip, const tTVPImageCopyFuncBase* blendfunc, iTVPBaseBitmap *dest, const tTVPRect &destrect, const iTVPBaseBitmap *src, const tTVPRect &srcrect ) {
		const int srcwidth = srcrect.get_width();
		const int dstheight = destrect.get_height();
#ifdef _DEBUG
		std::vector<tjs_uint32> work(srcwidth);
#else
		std::vector<tjs_uint32> work;
		work.reserve( srcwidth );
#endif
		const float* wstarty = &paramy_.weight_[0];
		// �N���b�s���O�����X�L�b�v
		for( int y = 0; y < clip.offsety_; y++ ) {
			wstarty += paramy_.length_[y];
		}
		tjs_uint32* workbits = &work[0];
		tjs_int dststride = dest->GetPitchBytes()/(int)sizeof(tjs_uint32);
		tjs_uint32* dstbits = (tjs_uint32*)dest->GetScanLineForWrite(clip.dst_top_) + clip.dst_left_;
		if( blendfunc == NULL ) {
			for( int y = clip.offsety_; y < clip.height_; y++ ) {
				samplingVertical( y, workbits, dstheight, srcwidth, src, srcrect, wstarty );
				samplingHorizontal( dstbits, clip.offsetx_, clip.width_, workbits );
				dstbits += dststride;
			}
		} else {	// �P���R�s�[�ȊO�́A��x�e���|�����ɏ����o���Ă��獇������
#ifdef _DEBUG
			std::vector<tjs_uint32> dstwork(clip.getDestWidth());
#else
			std::vector<tjs_uint32> dstwork;
			dstwork.reserve( clip.getDestWidth() );
#endif
			tjs_uint32* midbits = &dstwork[0];	// �r�������p�o�b�t�@
			for( int y = clip.offsety_; y < clip.height_; y++ ) {
				samplingVertical( y, workbits, dstheight, srcwidth, src, srcrect, wstarty );
				samplingHorizontal( midbits, clip.offsetx_, clip.width_, workbits ); // �ꎞ�o�b�t�@�ɂ܂��R�s�[, �͈͊O�͏������Ȃ�
				(*blendfunc)( dstbits, midbits, clip.getDestWidth() );
				dstbits += dststride;
			}
		}
	}
	void ResampleImageMT( const tTVPResampleClipping &clip, const tTVPImageCopyFuncBase* blendfunc, iTVPBaseBitmap *dest, const tTVPRect &destrect, const iTVPBaseBitmap *src, const tTVPRect &srcrect, tjs_int threadNum ) {
		const int srcwidth = srcrect.get_width();
		const float* wstarty = &paramy_.weight_[0];
		// �N���b�s���O�����X�L�b�v
		for( int y = 0; y < clip.offsety_; y++ ) {
			wstarty += paramy_.length_[y];
		}
		int offset = clip.offsety_;
		const int height = clip.getDestHeight();

//		TVPBeginThreadTask(threadNum);
		std::vector<ThreadParameter> params(threadNum);
		for( int i = 0; i < threadNum; i++ ) {
			ThreadParameter* param = &params[i];
			param->sampler_ = this;
			param->start_ = height * i / threadNum + offset;
			param->end_ = height * (i + 1) / threadNum + offset;
			param->width_ = srcwidth;
			param->wstarty_ = wstarty;
			param->src_ = src;
			param->srcrect_ = &srcrect;
			param->dest_ = dest;
			param->destrect_ = &destrect;
			param->clip_ = &clip;
			param->blendfunc_ = blendfunc;
			int top = param->start_;
			int bottom = param->end_;
		//	TVPExecThreadTask(&ResamplerFunc, TVP_THREAD_PARAM(param));
			if( i < (threadNum-1) ) {
				for( int y = top; y < bottom; y++ ) {
					int len = paramy_.length_[y];
					wstarty += len;
				}
			}
		}
		//		TVPEndThreadTask();
		TVPExecThreadTask(threadNum, [&](int i){
			ResamplerFunc(&params[i]);
		});
	}
public:
	/** �V���O���X���b�h */
	template<typename TWeightFunc>
	void Resample(const tTVPResampleClipping &clip, const tTVPImageCopyFuncBase* blendfunc, iTVPBaseBitmap *dest, const tTVPRect &destrect, const iTVPBaseBitmap *src, const tTVPRect &srcrect, float tap, TWeightFunc& func) {
		const int srcwidth = srcrect.get_width();
		const int srcheight = srcrect.get_height();
		const int dstwidth = destrect.get_width();
		const int dstheight = destrect.get_height();
		AxisParamCalculateAxis( paramx_, 0, srcwidth, srcwidth, dstwidth, tap, func );
		AxisParamCalculateAxis( paramy_, srcrect.top, srcrect.bottom, srcheight, dstheight, tap, func );
		ResampleImage( clip, blendfunc, dest, destrect, src, srcrect );
	}
	template<typename TWeightFunc>
	void ResampleMT(const tTVPResampleClipping &clip, const tTVPImageCopyFuncBase* blendfunc, iTVPBaseBitmap *dest, const tTVPRect &destrect, const iTVPBaseBitmap *src, const tTVPRect &srcrect, float tap, TWeightFunc& func) {
		const int srcwidth = srcrect.get_width();
		const int srcheight = srcrect.get_height();
		const int dstwidth = destrect.get_width();
		const int dstheight = destrect.get_height();
		int maxwidth = srcwidth > dstwidth ? srcwidth : dstwidth;
		int maxheight = srcheight > dstheight ? srcheight : dstheight;
		int threadNum = 1;
		int pixelNum = maxwidth*(int)tap*maxheight + maxheight*(int)tap*maxwidth;
		if( pixelNum >= 50 * 500 ) {
			threadNum = TVPGetThreadNum();
		}
		if( threadNum == 1 ) { // �ʐς����Ȃ��X���b�h��1�̎��͂��̂܂܎��s
			Resample( clip, blendfunc, dest, destrect, src, srcrect, tap, func );
			return;
		}
		AxisParamCalculateAxis( paramx_, 0, srcwidth, srcwidth, dstwidth, tap, func );
		AxisParamCalculateAxis( paramy_, srcrect.top, srcrect.bottom, srcheight, dstheight, tap, func );
		ResampleImageMT( clip, blendfunc, dest, destrect, src, srcrect, threadNum );
	}
	void ResampleAreaAvg( const tTVPResampleClipping &clip, const tTVPImageCopyFuncBase* blendfunc, iTVPBaseBitmap *dest, const tTVPRect &destrect, const iTVPBaseBitmap *src, const tTVPRect &srcrect ) {
		const int srcwidth = srcrect.get_width();
		const int srcheight = srcrect.get_height();
		const int dstwidth = destrect.get_width();
		const int dstheight = destrect.get_height();
		if( dstwidth > srcwidth || dstheight > srcheight ) return;
		AxisParamCalculateAxisAreaAvg( paramx_, 0, srcwidth, srcwidth, dstwidth );
		AxisParamCalculateAxisAreaAvg( paramy_, srcrect.top, srcrect.bottom, srcheight, dstheight );
		ResampleImage( clip, blendfunc, dest, destrect, src, srcrect );
	}
	void ResampleAreaAvgMT( const tTVPResampleClipping &clip, const tTVPImageCopyFuncBase* blendfunc, iTVPBaseBitmap *dest, const tTVPRect &destrect, const iTVPBaseBitmap *src, const tTVPRect &srcrect ) {
		const int srcwidth = srcrect.get_width();
		const int srcheight = srcrect.get_height();
		const int dstwidth = destrect.get_width();
		const int dstheight = destrect.get_height();
		if( dstwidth > srcwidth || dstheight > srcheight ) return;
		int maxwidth = srcwidth > dstwidth ? srcwidth : dstwidth;
		int maxheight = srcheight > dstheight ? srcheight : dstheight;
		int threadNum = 1;
		int pixelNum = maxwidth*maxheight;
		if( pixelNum >= 50 * 500 ) {
			threadNum = TVPGetThreadNum();
		}
		if( threadNum == 1 ) { // �ʐς����Ȃ��X���b�h��1�̎��͂��̂܂܎��s
			ResampleAreaAvg( clip, blendfunc, dest, destrect, src, srcrect );
			return;
		}
		AxisParamCalculateAxisAreaAvg( paramx_, 0, srcwidth, srcwidth, dstwidth );
		AxisParamCalculateAxisAreaAvg( paramy_, srcrect.top, srcrect.bottom, srcheight, dstheight );
		ResampleImageMT( clip, blendfunc, dest, destrect, src, srcrect, threadNum );
	}
};

void TJS_USERENTRY ResamplerFunc( void* p ) {
	Resampler::ThreadParameter* param = (Resampler::ThreadParameter*)p;
	const int width = param->width_;
#ifdef _DEBUG
	std::vector<tjs_uint32> work(width);
#else
	std::vector<tjs_uint32> work;
	work.reserve( width );
#endif

	iTVPBaseBitmap* dest = param->dest_;
	const tTVPRect& destrect = *param->destrect_;
	const iTVPBaseBitmap* src = param->src_;
	const tTVPRect& srcrect = *param->srcrect_;

	const int srcwidth = srcrect.get_width();
	const int dstwidth = destrect.get_width();
	const int dstheight = destrect.get_height();
	const float* wstarty = param->wstarty_;
	tjs_uint32* workbits = &work[0];
	tjs_int dststride = dest->GetPitchBytes()/(int)sizeof(tjs_uint32);
	tjs_uint32* dstbits = (tjs_uint32*)dest->GetScanLineForWrite(param->start_+destrect.top) + param->clip_->dst_left_;
	if( param->blendfunc_ == NULL ) {
		for( int y = param->start_; y < param->end_; y++ ) {
			param->sampler_->samplingVertical( y, workbits, dstheight, srcwidth, src, srcrect, wstarty );
			param->sampler_->samplingHorizontal( dstbits, param->clip_->offsetx_, param->clip_->width_, workbits );
			dstbits += dststride;
		}
	} else {	// �P���R�s�[�ȊO
#ifdef _DEBUG
		std::vector<tjs_uint32> dstwork(param->clip_->getDestWidth());
#else
		std::vector<tjs_uint32> dstwork;
		dstwork.reserve( param->clip_->getDestWidth() );
#endif
		tjs_uint32* midbits = &dstwork[0];	// �r�������p�o�b�t�@
		for( int y = param->start_; y < param->end_; y++ ) {
			param->sampler_->samplingVertical( y, workbits, dstheight, srcwidth, src, srcrect, wstarty );
			param->sampler_->samplingHorizontal( midbits, param->clip_->offsetx_, param->clip_->width_, workbits ); // �ꎞ�o�b�t�@�ɂ܂��R�s�[, �͈͊O�͏������Ȃ�
			(*param->blendfunc_)( dstbits, midbits, param->clip_->getDestWidth() );
			dstbits += dststride;
		}
	}
}

void TVPBicubicResample(const tTVPResampleClipping &clip, const tTVPImageCopyFuncBase* blendfunc, iTVPBaseBitmap *dest, const tTVPRect &destrect, const iTVPBaseBitmap *src, const tTVPRect &srcrect, float sharpness) {
	BicubicWeight weightfunc(sharpness);
	Resampler sampler;
	sampler.ResampleMT( clip, blendfunc, dest, destrect, src, srcrect, BicubicWeight::RANGE, weightfunc );
}
void TVPAreaAvgResample(const tTVPResampleClipping &clip, const tTVPImageCopyFuncBase* blendfunc, iTVPBaseBitmap *dest, const tTVPRect &destrect, const iTVPBaseBitmap *src, const tTVPRect &srcrect) {
	Resampler sampler;
	sampler.ResampleAreaAvgMT( clip, blendfunc, dest, destrect, src, srcrect );
}
template<typename TWeightFunc>
void TVPWeightResample(const tTVPResampleClipping &clip, const tTVPImageCopyFuncBase* blendfunc, iTVPBaseBitmap *dest, const tTVPRect &destrect, const iTVPBaseBitmap *src, const tTVPRect &srcrect) {
	TWeightFunc weightfunc;
	Resampler sampler;
	sampler.ResampleMT( clip, blendfunc, dest, destrect, src, srcrect, TWeightFunc::RANGE, weightfunc );
}

/**
 * �g��k������
 * @param dest : �������ݐ�摜
 * @param destrect : �������ݐ��`
 * @param src : �ǂݍ��݌��摜
 * @param srcrect : �ǂݍ��݌���`
 * @param type : �g��k���t�B���^�^�C�v
 * @param typeopt : �g��k���t�B���^�^�C�v�I�v�V����
 * @param method : �u�����h���@
 * @param opa : �s�����x
 * @param hda : �������ݐ�A���t�@�ێ�
 */
void TVPResampleImage( const tTVPRect &cliprect, iTVPBaseBitmap *dest, const tTVPRect &destrect, const iTVPBaseBitmap *src, const tTVPRect &srcrect,
	tTVPBBStretchType type, tjs_real typeopt, tTVPBBBltMethod method, tjs_int opa, bool hda ) {
	// �N���b�s���O����
	tTVPResampleClipping clip;
	clip.setClipping( cliprect, destrect );
	if( clip.getDestWidth() <= 0 || clip.getDestHeight() <= 0 ) return;

	// �u�����h�����֐���o�^
	tTVPImageCopyFuncBase* func = NULL;
	if( hda || opa != 255 || method != bmCopy ) {
		tTVPBlendParameter blendparam( method, opa, hda );
		blendparam.setFunctionFromParam();
		if( blendparam.blend_func ) {
			func = new tTVPBlendImageFunc(blendparam.opa_,blendparam.blend_func);
		} else if( blendparam.copy_func ) {
			func = new tTVPCopyImageFunc(blendparam.copy_func);
		}
	}

	try {
#if 0
		tjs_uint32 CpuFeature = TVPGetCPUType();
		if( (CpuFeature & TVP_CPU_HAS_AVX2) ) {
			TVPResampleImageAVX2( clip, func, dest, destrect, src, srcrect, type, typeopt );
		} else if( (CpuFeature & TVP_CPU_HAS_SSE2) ) {
			TVPResampleImageSSE2( clip, func, dest, destrect, src, srcrect, type, typeopt );
		} else
#endif
		{
			 // C�o�[�W�����͌Œ菬���_�łȂ��B�x���Ȃ�B
			switch( type ) {
			case stLinear:
				TVPWeightResample<BilinearWeight>(clip, func, dest, destrect, src, srcrect );
				break;
			case stCubic:
				TVPBicubicResample(clip, func, dest, destrect, src, srcrect, (float)typeopt );
				break;
			case stLanczos2:
				TVPWeightResample<LanczosWeight<2> >(clip, func, dest, destrect, src, srcrect );
				break;
			case stLanczos3:
				TVPWeightResample<LanczosWeight<3> >(clip, func, dest, destrect, src, srcrect );
				break;
			case stSpline16:
				TVPWeightResample<Spline16Weight>(clip, func, dest, destrect, src, srcrect );
				break;
			case stSpline36:
				TVPWeightResample<Spline36Weight>(clip, func, dest, destrect, src, srcrect );
				break;
			case stAreaAvg:
				TVPAreaAvgResample(clip, func, dest, destrect, src, srcrect );
				break;
			case stGaussian:
				TVPWeightResample<GaussianWeight>(clip, func, dest, destrect, src, srcrect );
				break;
			case stBlackmanSinc:
				TVPWeightResample<BlackmanSincWeight>(clip, func, dest, destrect, src, srcrect );
				break;
			case stSemiFastLinear:
				TVPWeightResample<BilinearWeight>(clip, func, dest, destrect, src, srcrect );
				break;
			case stFastCubic:
				TVPBicubicResample(clip, func, dest, destrect, src, srcrect, (float)typeopt );
				break;
			case stFastLanczos2:
				TVPWeightResample<LanczosWeight<2> >(clip, func, dest, destrect, src, srcrect );
				break;
			case stFastSpline16:
				TVPWeightResample<Spline16Weight>(clip, func, dest, destrect, src, srcrect );
				break;
			case stFastLanczos3:
				TVPWeightResample<LanczosWeight<3> >(clip, func, dest, destrect, src, srcrect );
				break;
			case stFastSpline36:
				TVPWeightResample<Spline36Weight>(clip, func, dest, destrect, src, srcrect );
				break;
			case stFastAreaAvg:
				TVPAreaAvgResample(clip, func, dest, destrect, src, srcrect );
				break;
			case stFastGaussian:
				TVPWeightResample<GaussianWeight>(clip, func, dest, destrect, src, srcrect );
				break;
			case stFastBlackmanSinc:
				TVPWeightResample<BlackmanSincWeight>(clip, func, dest, destrect, src, srcrect );
				break;
			default:
				throw L"Not supported yet.";
				break;
			}
		}
	} catch(...) {
		if( func ) delete func;
#if defined(ANDROID)
		throw;
#elif defined(_MSC_VER)	
		__debugbreak(); throw;
#else
		throw;
#endif	
	}
	if( func ) delete func;
}
