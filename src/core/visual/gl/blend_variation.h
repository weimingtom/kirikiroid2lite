/*

	TVP2 ( T Visual Presenter 2 )  A script authoring tool
	Copyright (C) 2000-2009 W.Dee <dee@kikyou.info> and contributors

	See details of license at "license.txt"


*/
/******************************************************************************/
/**
 * �A���t�@�̈������ɂ���Ĕ�������o���G�[�V���������₷�����邽�߂̃t�@���N�^
 * blend_func �́A
 * tjs_uint32 operator()( tjs_uint32 d, tjs_uint32 s, tjs_uint32 a )
 * �����t�@���N�^
 *****************************************************************************/

#ifndef __BLEND_VARIATION_H__
#define __BLEND_VARIATION_H__

//--------------------------------------------------------------------------------------------------------
// �A���t�@�̈���
//--------------------------------------------------------------------------------------------------------
// src�̃A���t�@���g�p����o�[�W����
template<class blend_func>
struct normal_op {
	blend_func	func_;
	inline normal_op() {}
	inline normal_op(tjs_uint32 opa) {}
	inline tjs_uint32 operator()( tjs_uint32 d, tjs_uint32 s ) const {
		tjs_uint32 a = (s>>24);
		return func_(d,s,a);
	}
};
//--------------------------------------------------------------------------------------------------------
// �����x���w�肷��o�[�W����
template<class blend_func>
struct translucent_op {
	const tjs_int	opa_;
	blend_func		func_;
	inline translucent_op() : opa_(255) {}
	inline translucent_op(tjs_uint32 opa) : opa_(opa) {}
	inline tjs_uint32 operator()( tjs_uint32 d, tjs_uint32 s ) const {
		tjs_uint32 a = ((s>>24)*opa_)>>8;
		return func_(d,s,a);
	}
};
// �\�[�X�̃A���t�@���g�p���Ȃ�
template<class blend_func>
struct translucent_nsa_op {
	const tjs_int	opa_;
	blend_func		func_;
	inline translucent_nsa_op() : opa_(255) {}
	inline translucent_nsa_op(tjs_uint32 opa) : opa_(opa) {}
	inline tjs_uint32 operator()( tjs_uint32 d, tjs_uint32 s ) const {
		return func_(d,s,opa_);
	}
};
//--------------------------------------------------------------------------------------------------------
// dest�A���t�@��ی삷��o�[�W����
template<class blend_func>
struct hda_op {
	blend_func	func_;
	inline hda_op(){}
	inline hda_op(tjs_uint32 opa){}
	inline tjs_uint32 operator()( tjs_uint32 d, tjs_uint32 s ) const {
		tjs_uint32 a = (s>>24);
		return (func_(d,s,a)&0x00ffffff) | (d&0xff000000);
	}
};
// �\�[�X�̃A���t�@���g�p���Ȃ�
template<class blend_func>
struct hda_nsa_op {
	blend_func	func_;
	inline hda_nsa_op(){}
	inline hda_nsa_op(tjs_uint32 opa){}
	inline tjs_uint32 operator()( tjs_uint32 d, tjs_uint32 s ) const {
		return (func_(d,s)&0x00ffffff) | (d&0xff000000);
	}
};
//--------------------------------------------------------------------------------------------------------
// dest�A���t�@��ی삵�A�����x���w�肷��o�[�W����
template<class blend_func>
struct hda_translucent_op {
	const tjs_uint32	opa_;
	blend_func		func_;
	inline hda_translucent_op() : opa_(255) {}
	inline hda_translucent_op(tjs_uint32 opa) : opa_(opa) {}
	inline tjs_uint32 operator()( tjs_uint32 d, tjs_uint32 s ) const {
		tjs_uint32 a = ((s>>24)*opa_)>>8;
		return (func_(d,s,a)&0x00ffffff) | (d&0xff000000);
	}
};
// �\�[�X�̃A���t�@���g�p���Ȃ�
template<class blend_func>
struct hda_translucent_nsa_op {
	const tjs_uint32	opa_;
	blend_func		func_;
	inline hda_translucent_nsa_op() : opa_(255) {}
	inline hda_translucent_nsa_op(tjs_uint32 opa) : opa_(opa) {}
	inline tjs_uint32 operator()( tjs_uint32 d, tjs_uint32 s ) const {
		return (func_(d,s,opa_)&0x00ffffff) | (d&0xff000000);
	}
};
//--------------------------------------------------------------------------------------------------------
// dest�̃A���t�@���g�p����o�[�W����
template<class blend_func>
struct dest_alpha_op {
	blend_func	func_;
	inline dest_alpha_op() {}
	inline dest_alpha_op(tjs_uint32 opa) {}
	inline tjs_uint32 operator()( tjs_uint32 d, tjs_uint32 s ) const {
#ifdef NOT_USE_TABLE
		tjs_uint32 sa = s >> 24;
		tjs_uint32 da = d >> 24;
		tjs_uint32 sopa;
		if( da ) {
			float at = (float)(da/255.0);
			float bt = (float)(sa/255.0);
			float c = bt / at;
			c /= (float)( (1.0 - bt + c) );
			tjs_uint32 sopa = (tjs_uint32)(c*255);
		} else {
			sopa = 255;
		}
		tjs_uint32 destalpha = (unsigned char)( 255 - (255-da)*(255-sa)/ 255 ) << 24;
#else
		tjs_uint32 addr = ((s >> 16) & 0xff00) + (d>>24);
		tjs_uint32 destalpha = TVPNegativeMulTable[addr]<<24;
		tjs_uint32 sopa = TVPOpacityOnOpacityTable[addr];
#endif
		return func_(d,s,sopa) + destalpha;
	}
	
	// (sa/da) / ( 1 - sa + (sa/da) )
	// 1 / (sa+da)

	// 1 - (1-da)*(1-sa)
	// 1 - sa - da + da*sa
	// ca = a + ba
	// ca = a + ba;
	// ca = a + ba;
	// ca = a + (((255-a)*da)/255
};
// �\�[�X�̃A���t�@���g�p���Ȃ� HDA �Ɠ������ʂɂȂ�̂ŁA�\�[�X�̃A���t�@���g�p���Ȃ��o�[�W�����͕s�v��
//--------------------------------------------------------------------------------------------------------
// dest�̃A���t�@���g�p����A�����x���w�肷��o�[�W����
template<class blend_func>
struct dest_alpha_translucent_op {
	const tjs_int	opa_;
	blend_func		func_;
	inline dest_alpha_translucent_op() : opa_(255) {}
	inline dest_alpha_translucent_op(tjs_uint32 opa) : opa_(opa) {}
	inline tjs_uint32 operator()( tjs_uint32 d, tjs_uint32 s ) const {
#ifdef NOT_USE_TABLE
		tjs_uint32 sa = ((s>>24)*opa_)>>8;
		tjs_uint32 da = d >> 24;
		tjs_uint32 sopa;
		if( da ) {
			float at = (float)(da/255.0);
			float bt = (float)(sa/255.0);
			float c = bt / at;
			c /= (float)( (1.0 - bt + c) );
			tjs_uint32 sopa = (tjs_uint32)(c*255);
		} else {
			sopa = 255;
		}
		tjs_uint32 destalpha = (unsigned char)( 255 - (255-da)*(255-sa)/ 255 ) << 24;
#else
		tjs_uint32 addr = (( (s>>24)*opa_) & 0xff00) + (d>>24);
		tjs_uint32 destalpha = TVPNegativeMulTable[addr]<<24;
		tjs_uint32 sopa = TVPOpacityOnOpacityTable[addr];
#endif
		return func_(d,s,sopa) + destalpha;
	}
};
//--------------------------------------------------------------------------------------------------------
#if 0
// �s�����o�[�W���� ����ł��������������
template<class blend_func>
struct opacity_op {
	blend_func	func_;
	inline opacity_op(){}
	inline opacity_op(tjs_uint32 opa){}
	inline tjs_uint32 operator()( tjs_uint32 d, tjs_uint32 s ) const {
		return func_(d,s,255);
	}
};
//--------------------------------------------------------------------------------------------------------
// �s�����o�[�W���� �����x���w�肷��
template<class blend_func>
struct opacity_translucent_op {
	const tjs_uint32	opa_;
	blend_func		func_;
	inline opacity_translucent_op() : opa_(255) {}
	inline opacity_translucent_op(tjs_uint32 opa) : opa_(opa) {}
	inline tjs_uint32 operator()( tjs_uint32 d, tjs_uint32 s ) const {
		return func_(d,s,opa_);
	}
};
//--------------------------------------------------------------------------------------------------------
#endif
#endif // __BLEND_VARIATION_H__
