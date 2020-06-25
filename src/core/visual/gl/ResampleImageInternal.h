/******************************************************************************/
/**
 * �g��k�������g�p�w�b�_
 * ----------------------------------------------------------------------------
 * 	Copyright (C) T.Imoto <http://www.kaede-software.com>
 * ----------------------------------------------------------------------------
 * @author		T.Imoto
 * @date		2014/04/04
 * @note
 *****************************************************************************/


#ifndef __RESAMPLE_IMAGE_INTERNAL_H__
#define __RESAMPLE_IMAGE_INTERNAL_H__

struct tTVPImageCopyFuncBase {
	virtual ~tTVPImageCopyFuncBase() {}
	virtual void operator() ( tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len ) const = 0;
};
struct tTVPBlendImageFunc : public tTVPImageCopyFuncBase {
	tjs_int opa_;
	void (*blend_func_)(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa);

	tTVPBlendImageFunc( tjs_int opa, void (*blend_func)(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa) )
	: opa_(opa), blend_func_(blend_func) {}

	virtual void operator() ( tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len ) const {
		blend_func_( dest, src, len, opa_ );
	}
};
struct tTVPCopyImageFunc : public tTVPImageCopyFuncBase {
	void (*copy_func_)(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len);

	tTVPCopyImageFunc( void (*copy_func)(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len) )
	: copy_func_(copy_func) {}

	virtual void operator() ( tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len ) const {
		copy_func_( dest, src, len );
	}
};
/**
 * �u�����h�p�����[�^
 * �u�����h�֐��|�C���^�͊g��k�����l�����Ȃ�����
 */
struct tTVPBlendParameter {
	/** �u�����h���� */
	tTVPBBBltMethod method_;
	/** �s�����x */
	tjs_int opa_;
	/** �]����A���t�@�ێ��L�� */
	bool hda_;
	/** �s�����x���l�����ău�����h���� */
	void (*blend_func)(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len, tjs_int opa);
	/** �R�s�[����(�\�[�X��f�X�e�B�l�[�V�����̃A���t�@���l�������ꍇ������) */
	void (*copy_func)(tjs_uint32 *dest, const tjs_uint32 *src, tjs_int len);

	tTVPBlendParameter() : method_(bmCopy), opa_(255), hda_(false), blend_func(NULL), copy_func(NULL) {}
	tTVPBlendParameter( tTVPBBBltMethod method, tjs_int opa, bool hda ) : method_(method), opa_(opa), hda_(hda), blend_func(NULL), copy_func(NULL) {}

	/**
	 * �p�����[�^�����Ɋ֐��|�C���^�����肷��
	 */
	void setFunctionFromParam();
};
/**
 * �N���b�s���O�p�����[�^
 */
struct tTVPResampleClipping {
	tjs_int offsetx_;	// �N���b�s���O����鍶�[��
	tjs_int offsety_;	// �N���b�s���O������[��
	tjs_int width_;		// �R�s�[���Aoffsetx_���܂�ł���
	tjs_int height_;	// �R�s�[�����Aoffsety_���܂�ł���
	tjs_int dst_left_;	// ���ۂ̃R�s�[�捶�[
	tjs_int dst_top_;	// ���ۂ̃R�s�[���[

	void setClipping( const tTVPRect &cliprect, const tTVPRect &destrect );

	/** ���ۂɃR�s�[����镝 */
	inline tjs_int getDestWidth() const { return width_ - offsetx_; }
	/** ���ۂɃR�s�[����鍂�� */
	inline tjs_int getDestHeight() const { return height_ - offsety_; }
};


/**
 * �ʐϕ��σp�����[�^�p
 */
template<typename TVector>
void TVPCalculateAxisAreaAvg( int srcstart, int srcend, int srclength, int dstlength, std::vector<int>& start, std::vector<int>& length, TVector& weight ) {
	start.clear();
	start.reserve( dstlength );
	length.clear();
	length.reserve( dstlength );
	weight.clear();
	int wlength = srclength + dstlength;
	weight.reserve( wlength );
	int delta = 0;
	int srctarget = srclength;
	int len = 0;
	start.push_back( 0 );
	for( int x = 0; x < srclength; x++ ) {
		if( (delta + dstlength) <= srctarget ) {	// ���E�ɒB���Ă��Ȃ�
			weight.push_back( 1.0f );
			len++;
		} else { // ���E���܂�����
			int d = (delta + dstlength) - srctarget;
			weight.push_back( (float)(dstlength - d) / (float)dstlength ); // �O�̗̈�
			length.push_back( len+1 );

			start.push_back( x );
			len = 1;
			weight.push_back( (float)d / (float)dstlength );
			srctarget += srclength;
		}
		delta += dstlength;
	}
	length.push_back( len );
}

/**
 * �ʐϕ��σp�����[�^���K���p
 */
template<typename TVector>
void TVPNormalizeAxisAreaAvg( std::vector<int>& length, TVector& weight ) {
	// ���K��
	const int count = (const int)length.size();
	float* wstart = &weight[0];
	for( int i = 0; i < count; i++ ) {
		int len = length[i];
		// ���v�l�����߂�
		float* w = wstart;
		float sum = 0.0f;
		for( int j = 0; j < len; j++ ) {
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
		w = wstart;
		for( int i = 0; i < len; i++ ) {
			*w *= rcp;
			w++;
		}
		wstart = w;
	}
}

#endif // __RESAMPLE_IMAGE_INTERNAL_H__
