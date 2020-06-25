//---------------------------------------------------------------------------
/*
	Risa [�肳]      alias �g���g��3 [kirikiri-3]
	 stands for "Risa Is a Stagecraft Architecture"
	Copyright (C) 2000 W.Dee <dee@kikyou.info> and contributors

	See details of license at "license.txt"
*/
//---------------------------------------------------------------------------
//! @file
//! @brief ���w�֐��Q
//---------------------------------------------------------------------------

#ifndef TVP_MATHALGOLITHMS_H
#define TVP_MATHALGOLITHMS_H

#define _USE_MATH_DEFINES
#include <math.h>
#include "xmmlib.h"


//---------------------------------------------------------------------------

// ����

//---------------------------------------------------------------------------
/**
 * atan2 �̍����� (1x float, C�����)
 * @note	���x�͂��܂�ǂ��Ȃ��B10bit���炢�B @r
 *			���T: http://www.dspguru.com/comp.dsp/tricks/alg/fxdatan2.htm
 */
static inline float VFast_arctan2(float y, float x)
{
   static const float coeff_1 = (float)(M_PI/4);
   static const float coeff_2 = 3*coeff_1;
   float angle;
   float abs_y = fabs(y)+(float)1e-10;     // kludge to prevent 0/0 condition
   if (x>=0)
   {
      float r = (x - abs_y) / (x + abs_y);
      angle = coeff_1 - coeff_1 * r;
   }
   else
   {
      float r = (x + abs_y) / (abs_y - x);
      angle = coeff_2 - coeff_1 * r;
   }
   if (y < 0)
     return(-angle);     // negate if in quad III or IV
   else
     return(angle);
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
static inline float VFast_atan2_madd(float a, float b, float c) { return a*b+c; }
static inline float VFast_atan2_nmsub(float a, float b, float c) { return -(a*b-c); }
static inline float VFast_atan2_round(float a) { return (float)((a>0)?(int)(a+0.5):(int)(a-0.5)); }

/**
 * sincos �̍����� (1x float, C�����)
 * @note	���T: http://arxiv.org/PS_cache/cs/pdf/0406/0406049.pdf
 */
static inline void VFast_sincos(float v, float &sin, float &cos)
{
	const float  ss1 =  1.5707963235f  ;
	const float  ss2 =  -0.645963615f  ;
	const float  ss3 =  0.0796819754f  ;
	const float  ss4 =  -0.0046075748f ;
	const float  cc1 =  -1.2336977925f ;
	const float  cc2 =  0.2536086171f  ;
	const float  cc3 =  -0.0204391631f ;

	float s1, s2, c1, c2, fixmag1;
	float x1=VFast_atan2_madd(v, (float)(1.0/(2.0*3.1415926536)), (float)(0.0));
	/* q1=x/2pi reduced onto (-0.5,0.5), q2=q1**2 */
	float q1=VFast_atan2_nmsub(VFast_atan2_round(x1), (float)(1.0), x1);
	float q2=VFast_atan2_madd(q1, q1, (float)(0.0));
	s1= VFast_atan2_madd(q1,
			VFast_atan2_madd(q2,
				VFast_atan2_madd(q2,
					VFast_atan2_madd(q2, (float)(ss4),
								(float)(ss3)),
									(float)( ss2)),
							(float)(ss1)),
						(float)(0.0));
	c1= VFast_atan2_madd(q2,
			VFast_atan2_madd(q2,
				VFast_atan2_madd(q2, (float)(cc3),
				(float)(cc2)),
			(float)(cc1)),
		(float)(1.0));

	/* now, do one out of two angle-doublings to get sin & cos theta/2 */
	c2=VFast_atan2_nmsub(s1, s1, VFast_atan2_madd(c1, c1, (float)(0.0)));
	s2=VFast_atan2_madd((float)(2.0), VFast_atan2_madd(s1, c1, (float)(0.0)), (float)(0.0));

	/* now, cheat on the correction for magnitude drift...
	if the pair has drifted to (1+e)*(cos, sin),
	the next iteration will be (1+e)**2*(cos, sin)
	which is, for small e, (1+2e)*(cos,sin).
	However, on the (1+e) error iteration,
	sin**2+cos**2=(1+e)**2=1+2e also,
	so the error in the square of this term
	will be exactly the error in the magnitude of the next term.
	Then, multiply final result by (1-e) to correct */

	/* this works with properly normalized sine-cosine functions, but un-normalized is more */
	fixmag1=VFast_atan2_nmsub(s2,s2, VFast_atan2_nmsub(c2, c2, (float)(2.0)));

	c1=VFast_atan2_nmsub(s2, s2, VFast_atan2_madd(c2, c2, (float)(0.0)));
	s1=VFast_atan2_madd((float)(2.0), VFast_atan2_madd(s2, c2, (float)(0.0)), (float)(0.0));
	cos=VFast_atan2_madd(c1, fixmag1, (float)(0.0));
	sin=VFast_atan2_madd(s1, fixmag1, (float)(0.0));
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/**
 * Phase Wrapping(radian��-PI�`PI�Ƀ��b�v����) (1x float, C�����)
 */
static inline float WrapPi_F1(float v)
{
	int rad_unit = static_cast<int>(v*(1.0/M_PI));
	if (rad_unit >= 0) rad_unit += rad_unit&1;
	else rad_unit -= rad_unit&1;
	v -= (float)(M_PI*(double)rad_unit);
	return v;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/**
 * ���֐���K�p���Ȃ���̃C���^�[���[�u����
 * @param dest		�i�[��(����)
 * @param src		�\�[�X
 * @param win		���֐�
 * @param numch		�`�����l����
 * @param destofs	dest�̏����J�n�ʒu
 * @param len		��������T���v����
 *					(�e�`�����l�����Ƃ̐�; ���ۂɏ��������T���v��
 *					���̑��v��len*numch�ɂȂ�)
 */
void DeinterleaveApplyingWindow(float * __restrict dest[], const float * __restrict src,
					float * __restrict win, int numch, size_t destofs, size_t len);
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/**
 * ���֐���K�p���Ȃ���̃C���^�[���[�u+�I�[�o�[���b�s���O
 * @param dest		�i�[��
 * @param src		�\�[�X(����)
 * @param win		���֐�
 * @param numch		�`�����l����
 * @param srcofs	src�̏����J�n�ʒu
 * @param len		��������T���v����
 *					(�e�`�����l�����Ƃ̐�; ���ۂɏ��������T���v��
 *					���̑��v��len*numch�ɂȂ�)
 */
void  InterleaveOverlappingWindow(float * __restrict dest,
	const float * __restrict const * __restrict src,
	float * __restrict win, int numch, size_t srcofs, size_t len);
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

#if defined(_M_IX86)||defined(_M_X64)
//---------------------------------------------------------------------------
// �萔�Ȃ�
//---------------------------------------------------------------------------
extern _ALIGN16(const tjs_uint32) TVP_VFASTATAN2_C1[4] ;
extern _ALIGN16(const tjs_uint32) TVP_VFASTATAN2_C1_XOR_C2[4] ;
extern _ALIGN16(const float) TVP_VFASTATAN2_E [4] ;
extern _ALIGN16(const float) TVP_VFASTSINCOS_SS1[4] ;
extern _ALIGN16(const float) TVP_VFASTSINCOS_SS2[4] ;
extern _ALIGN16(const float) TVP_VFASTSINCOS_SS3[4] ;
extern _ALIGN16(const float) TVP_VFASTSINCOS_SS4[4] ;
extern _ALIGN16(const float) TVP_VFASTSINCOS_CC1[4] ;
extern _ALIGN16(const float) TVP_VFASTSINCOS_CC2[4] ;
extern _ALIGN16(const float) TVP_VFASTSINCOS_CC3[4] ;
extern _ALIGN16(const float) TVP_V_R_PI[4] ;
extern _ALIGN16(const float) TVP_V_R_2PI[4] ;
extern _ALIGN16(const float) TVP_V_PI[4] ;
extern _ALIGN16(const float) TVP_V_2PI[4] ;
extern _ALIGN16(const tjs_uint32) TVP_V_I32_1[4];


//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/**
 * atan2 �̍����� (4x float, SSE��)
 * @note	���x�͂��܂�ǂ��Ȃ��B10bit���炢�B @r
 *			���T: http://www.dspguru.com/comp.dsp/tricks/alg/fxdatan2.htm
 */
static inline __m128 VFast_arctan2_F4_SSE(__m128 y, __m128 x)
{
	__m128 abs_y = _mm_add_ps(_mm_and_ps(y, PM128(PABSMASK)), PM128(TVP_VFASTATAN2_E));
//   float abs_y = fabs(y)+1e-10;     // kludge to prevent 0/0 condition

	__m128 x_sign = _mm_and_ps(x, PM128(PCS_RRRR));// 0x80000000 if x < 0
	__m128 x_mask = _mm_cmple_ps(x, PM128(PFV_0)); // 0xffffffff if x <= 0
	__m128 abs_y2 = _mm_xor_ps(abs_y , x_sign);
	__m128 abs_y1 = _mm_xor_ps(abs_y2, PM128(PCS_RRRR));
	__m128 r      = _mm_div_ps(_mm_add_ps(x, abs_y1), _mm_add_ps(x, abs_y2));
	r             = _mm_xor_ps(r, x_sign);
	__m128 coeff_1_or_2 = _mm_xor_ps(
							_mm_and_ps(x_mask, PM128(TVP_VFASTATAN2_C1_XOR_C2)),
							PM128(TVP_VFASTATAN2_C1)); // x<=0?coeff_2:coeff_1
/*
	__m128 coeff_1_or_2 = _mm_or_ps(
		_mm_and_ps   (x_mask, PM128(TVP_VFASTATAN2_C1)),
		_mm_andnot_ps(x_mask, PM128(TVP_VFASTATAN2_C2))); // x>=0?coeff_1:coeff_2
*/
	__m128 angle  = _mm_sub_ps(coeff_1_or_2, _mm_mul_ps(PM128(TVP_VFASTATAN2_C1), r));
/*
   if (x>=0)
   {
      float r =    (x - abs_y) / (x + abs_y)  ;
      angle = coeff_1 - coeff_1 * r;
   }
   else
   {
      float r = -( (x + abs_y) / (x - abs_y)  );
      angle = coeff_2 - coeff_1 * r;
   }
*/
	__m128 y_sign = _mm_and_ps(y, PM128(PCS_RRRR));
	return _mm_xor_ps(angle, y_sign);
/*
   if (y < 0)
     return(-angle);     // negate if in quad III or IV
   else
     return(angle);
*/
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/**
 * SSE�̊ۂ߃��[�h���ŋߒl�ɐݒ肷��
 * @note	���̃��\�b�h�� SSE �Ŏg�p����ۂ߃��[�h
 *			��ݒ肷��B�����K�v�Ƃ���e�ِ����Ăяo���O�ɂP��Ăяo�����ƁB
 *			����SSE���g�p���Ă���֐����Ԃɋ��܂�Ɗۂ߃��[�h���ς��\��
 *			������̂Œ��ӂ��邱�ƁB���̂悤�ȏꍇ�͍Ăт�����Ăяo����
 *			�ۂ߃��[�h���Đݒ肷�邱�ƁB
 */
static inline void SetRoundingModeToNearest_SSE()
{
	_MM_SET_ROUNDING_MODE(_MM_ROUND_NEAREST);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/**
 * sincos �̍����� (4x float, SSE��)
 * @note	���T: http://arxiv.org/PS_cache/cs/pdf/0406/0406049.pdf  @r
 *			�Ăяo���ɐ旧���� Risa_SetRoundingModeToNearest_SSE ���ĂԂ��ƁB
 */
static inline void VFast_sincos_F4_SSE2(__m128 v, __m128 &sin, __m128 &cos)
{
	__m128 s1, s2, c1, c2, fixmag1;

	__m128 x1 = _mm_mul_ps(v, PM128(TVP_V_R_2PI));
//	float x1=madd(v, (float)(1.0/(2.0*3.1415926536)), (float)(0.0));

	/* q1=x/2pi reduced onto (-0.5,0.5), q2=q1**2 */
	__m128i r0 = _mm_cvtps_epi32(x1);
	__m128 q1, q2;
	q1 = _mm_cvtepi32_ps(r0);
	q1 = _mm_sub_ps(x1, q1);

	q2 = _mm_mul_ps(q1, q1);

//	float q1=nmsub(round(x1), (float)(1.0), x1); // q1 = x1 - round(x1)
//	float q2=madd(q1, q1, (float)(0.0));

	s1 = _mm_add_ps(_mm_mul_ps(q2, PM128(TVP_VFASTSINCOS_SS4)), PM128(TVP_VFASTSINCOS_SS3));
		// s1 = (q2 * ss4 + ss3)
	s1 = _mm_add_ps(_mm_mul_ps(s1, q2), PM128(TVP_VFASTSINCOS_SS2));
		// s1 = (q2 * (q2 * ss4 + ss3) + ss2)
	s1 = _mm_add_ps(_mm_mul_ps(s1, q2), PM128(TVP_VFASTSINCOS_SS1));
		// s1 = (q2 * (q2 * (q2 * ss4 + ss3) + ss2) + ss1)
	s1 = _mm_mul_ps(s1, q1);

//	s1 = q1 * (q2 * (q2 * (q2 * ss4 + ss3) + ss2) + ss1);
//	s1= madd(q1,
//			madd(q2,
//				madd(q2,
//					madd(q2, (float)(ss4),
//								(float)(ss3)),
//									(float)( ss2)),
//							(float)(ss1)),
//						(float)(0.0));


	c1 = _mm_add_ps(_mm_mul_ps(q2, PM128(TVP_VFASTSINCOS_CC3)), PM128(TVP_VFASTSINCOS_CC2));
		// c1 = (q2 * cc3 + cc2)
	c1 = _mm_add_ps(_mm_mul_ps(c1, q2), PM128(TVP_VFASTSINCOS_CC1));
		// c1 =  (q2 * (q2 * cc3 + cc2) + cc1 )
	c1 = _mm_add_ps(_mm_mul_ps(c1, q2), PM128(PFV_1));
//	c1= (q2 *  (q2 * (q2 * cc3 + cc2) + cc1 ) + 1.0);
//	c1= madd(q2,
//			madd(q2,
//				madd(q2, (float)(cc3),
//				(float)(cc2)),
//			(float)(cc1)),
//		(float)(1.0));

	/* now, do one out of two angle-doublings to get sin & cos theta/2 */
	c2 = _mm_sub_ps( _mm_mul_ps(c1, c1), _mm_mul_ps(s1, s1));
//	c2=nmsub(s1, s1, madd(c1, c1, (float)(0.0))); // c2 = (c1*c1) - (s1*s1)
	s2 = _mm_mul_ps(_mm_mul_ps(s1, c1), PM128(PFV_2));
//	s2=madd((float)(2.0), madd(s1, c1, (float)(0.0)), (float)(0.0)); // s2=2*s1*c1

	/* now, cheat on the correction for magnitude drift...
	if the pair has drifted to (1+e)*(cos, sin),
	the next iteration will be (1+e)**2*(cos, sin)
	which is, for small e, (1+2e)*(cos,sin).
	However, on the (1+e) error iteration,
	sin**2+cos**2=(1+e)**2=1+2e also,
	so the error in the square of this term
	will be exactly the error in the magnitude of the next term.
	Then, multiply final result by (1-e) to correct */

	/* this works with properly normalized sine-cosine functions, but un-normalized is more */
	__m128 c2_c2 = _mm_mul_ps(c2, c2);
	__m128 s2_s2 = _mm_mul_ps(s2, s2);
	fixmag1 = _mm_sub_ps(_mm_sub_ps(PM128(PFV_2), c2_c2), s2_s2);
//	fixmag1=nmsub(s2,s2, nmsub(c2, c2, (float)(2.0))); // fixmag1 = ( 2.0 - c2*c2 ) - s2*s2

	c1 = _mm_sub_ps(c2_c2, s2_s2);
//	c1=nmsub(s2, s2, madd(c2, c2, (float)(0.0))); // c1 = c2*c2 - s2*s2
	s1 = _mm_mul_ps(_mm_mul_ps(s2, c2), PM128(PFV_2));
//	s1=madd((float)(2.0), madd(s2, c2, (float)(0.0)), (float)(0.0));
	cos = _mm_mul_ps(c1, fixmag1);
//	cos=madd(c1, fixmag1, (float)(0.0));
	sin = _mm_mul_ps(s1, fixmag1);
//	sin=madd(s1, fixmag1, (float)(0.0));
}
#if defined(_M_IX86)
static inline void VFast_sincos_F4_SSE(__m128 v, __m128 &sin, __m128 &cos)
{
	__m128 s1, s2, c1, c2, fixmag1;

	__m128 x1 = _mm_mul_ps(v, PM128(TVP_V_R_2PI));
//	float x1=madd(v, (float)(1.0/(2.0*3.1415926536)), (float)(0.0));

	/* q1=x/2pi reduced onto (-0.5,0.5), q2=q1**2 */
	__m64 r0 = _mm_cvt_ps2pi(x1);
	__m64 r1 = _mm_cvt_ps2pi(_mm_movehl_ps(x1, x1));
	__m128 q1, q2;
	q1 = _mm_movelh_ps(_mm_cvtpi32_ps(q1, r0), _mm_cvtpi32_ps(q1, r1));
	q1 = _mm_sub_ps(x1, q1);

	q2 = _mm_mul_ps(q1, q1);

//	float q1=nmsub(round(x1), (float)(1.0), x1); // q1 = x1 - round(x1)
//	float q2=madd(q1, q1, (float)(0.0));

	s1 = _mm_add_ps(_mm_mul_ps(q2, PM128(TVP_VFASTSINCOS_SS4)), PM128(TVP_VFASTSINCOS_SS3));
		// s1 = (q2 * ss4 + ss3)
	s1 = _mm_add_ps(_mm_mul_ps(s1, q2), PM128(TVP_VFASTSINCOS_SS2));
		// s1 = (q2 * (q2 * ss4 + ss3) + ss2)
	s1 = _mm_add_ps(_mm_mul_ps(s1, q2), PM128(TVP_VFASTSINCOS_SS1));
		// s1 = (q2 * (q2 * (q2 * ss4 + ss3) + ss2) + ss1)
	s1 = _mm_mul_ps(s1, q1);

//	s1 = q1 * (q2 * (q2 * (q2 * ss4 + ss3) + ss2) + ss1);
//	s1= madd(q1,
//			madd(q2,
//				madd(q2,
//					madd(q2, (float)(ss4),
//								(float)(ss3)),
//									(float)( ss2)),
//							(float)(ss1)),
//						(float)(0.0));


	c1 = _mm_add_ps(_mm_mul_ps(q2, PM128(TVP_VFASTSINCOS_CC3)), PM128(TVP_VFASTSINCOS_CC2));
		// c1 = (q2 * cc3 + cc2)
	c1 = _mm_add_ps(_mm_mul_ps(c1, q2), PM128(TVP_VFASTSINCOS_CC1));
		// c1 =  (q2 * (q2 * cc3 + cc2) + cc1 )
	c1 = _mm_add_ps(_mm_mul_ps(c1, q2), PM128(PFV_1));
//	c1= (q2 *  (q2 * (q2 * cc3 + cc2) + cc1 ) + 1.0);
//	c1= madd(q2,
//			madd(q2,
//				madd(q2, (float)(cc3),
//				(float)(cc2)),
//			(float)(cc1)),
//		(float)(1.0));

	/* now, do one out of two angle-doublings to get sin & cos theta/2 */
	c2 = _mm_sub_ps( _mm_mul_ps(c1, c1), _mm_mul_ps(s1, s1));
//	c2=nmsub(s1, s1, madd(c1, c1, (float)(0.0))); // c2 = (c1*c1) - (s1*s1)
	s2 = _mm_mul_ps(_mm_mul_ps(s1, c1), PM128(PFV_2));
//	s2=madd((float)(2.0), madd(s1, c1, (float)(0.0)), (float)(0.0)); // s2=2*s1*c1

	/* now, cheat on the correction for magnitude drift...
	if the pair has drifted to (1+e)*(cos, sin),
	the next iteration will be (1+e)**2*(cos, sin)
	which is, for small e, (1+2e)*(cos,sin).
	However, on the (1+e) error iteration,
	sin**2+cos**2=(1+e)**2=1+2e also,
	so the error in the square of this term
	will be exactly the error in the magnitude of the next term.
	Then, multiply final result by (1-e) to correct */

	/* this works with properly normalized sine-cosine functions, but un-normalized is more */
	__m128 c2_c2 = _mm_mul_ps(c2, c2);
	__m128 s2_s2 = _mm_mul_ps(s2, s2);
	fixmag1 = _mm_sub_ps(_mm_sub_ps(PM128(PFV_2), c2_c2), s2_s2);
//	fixmag1=nmsub(s2,s2, nmsub(c2, c2, (float)(2.0))); // fixmag1 = ( 2.0 - c2*c2 ) - s2*s2

	c1 = _mm_sub_ps(c2_c2, s2_s2);
//	c1=nmsub(s2, s2, madd(c2, c2, (float)(0.0))); // c1 = c2*c2 - s2*s2
	s1 = _mm_mul_ps(_mm_mul_ps(s2, c2), PM128(PFV_2));
//	s1=madd((float)(2.0), madd(s2, c2, (float)(0.0)), (float)(0.0));
	cos = _mm_mul_ps(c1, fixmag1);
//	cos=madd(c1, fixmag1, (float)(0.0));
	sin = _mm_mul_ps(s1, fixmag1);
//	sin=madd(s1, fixmag1, (float)(0.0));

	_mm_empty();
}
#elif defined(_M_X64)
#define VFast_sincos_F4_SSE VFast_sincos_F4_SSE2
#endif
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/**
 * Phase Wrapping(radian��-PI�`PI�Ƀ��b�v����) (4x float, SSE��)
 */
static inline __m128 Wrap_Pi_F4_SSE2(__m128 v)
{
	// v �� M_PI �Ŋ���
	__m128 v_quant = _mm_mul_ps(v, PM128(TVP_V_R_PI)); // v_quant = v/M_PI

	// v_quant�������_�ȉ���؂�̂ĂĐ����ɕϊ�
	__m128i q = _mm_cvttps_epi32(v_quant);
	// ���̏ꍇ��v_quant&1�𑫂��A���̏ꍇ�͈���
	// a = v_quant,    v_quant = a + ( (0 - (a&1)) & ((a>>31)|1) )
	q =
		_mm_add_epi32(
			q,
			_mm_and_si128(
				_mm_sub_epi32(
					_mm_setzero_si128(),
					_mm_and_si128(q, PM128I(TVP_V_I32_1))
				),
				_mm_or_si128(
					_mm_srai_epi32(q, 31),
					PM128I(TVP_V_I32_1)
				)
			)
		);
	// �����������ɖ߂��AM_PI ��������
	v_quant = _mm_cvtepi32_ps(q);
	v_quant = _mm_mul_ps(v_quant, PM128(TVP_V_PI));

	// ����� v �������
	v = _mm_sub_ps(v, v_quant);

	// �߂�
	return v;
}
#if defined(_M_IX86)
static inline __m128 Wrap_Pi_F4_SSE(__m128 v)
{
	// v �� M_PI �Ŋ���
	__m128 v_quant = _mm_mul_ps(v, PM128(TVP_V_R_PI)); // v_quant = v/M_PI

	// v_quant�������_�ȉ���؂�̂ĂĐ����ɕϊ�
	__m64 q0 = _mm_cvtt_ps2pi(v_quant); 
	__m64 q1 = _mm_cvtt_ps2pi(_mm_movehl_ps(v_quant, v_quant));

	// ���̏ꍇ��v_quant&1�𑫂��A���̏ꍇ�͈���
	// a = v_quant,    v_quant = a + ( (0 - (a&1)) & ((a>>31)|1) )

	q0 =
		_mm_add_pi32(
			q0,
			_mm_and_si64(
				_mm_sub_pi32(
					_mm_setzero_si64(),
					_mm_and_si64(q0, PM64(TVP_V_I32_1))
				),
				_mm_or_si64(
					_mm_srai_pi32(q0, 31),
					PM64(TVP_V_I32_1)
				)
			)
		);

	q1 =
		_mm_add_pi32(
			q1,
			_mm_and_si64(
				_mm_sub_pi32(
					_mm_setzero_si64(),
					_mm_and_si64(q1, PM64(TVP_V_I32_1))
				),
				_mm_or_si64(
					_mm_srai_pi32(q1, 31),
					PM64(TVP_V_I32_1)
				)
			)
		);

	// �����������ɖ߂��AM_PI ��������
	v_quant = _mm_movelh_ps(_mm_cvtpi32_ps(v, q0), _mm_cvtpi32_ps(v, q1));
	v_quant = _mm_mul_ps(v_quant, PM128(TVP_V_PI));

	// ����� v �������
	v = _mm_sub_ps(v, v_quant);

	// MMX�g���I���
	_mm_empty();

	// �߂�
	return v;
}
#elif defined(_M_X64)
// x64 �̎���SSE2���g��
#define Wrap_Pi_F4_SSE Wrap_Pi_F4_SSE2
#endif
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
/**
 * ���֐���K�p���Ȃ���̃C���^�[���[�u����
 * @param dest		�i�[��(����)
 * @param src		�\�[�X
 * @param win		���֐�
 * @param numch		�`�����l����
 * @param destofs	dest�̏����J�n�ʒu
 * @param len		��������T���v����
 *					(�e�`�����l�����Ƃ̐�; ���ۂɏ��������T���v��
 *					���̑��v��len*numch�ɂȂ�)
 */
void DeinterleaveApplyingWindow_sse(float * __restrict dest[], const float * __restrict src,
					float * __restrict win, int numch, size_t destofs, size_t len);
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/**
 * ���֐���K�p���Ȃ���̃C���^�[���[�u+�I�[�o�[���b�s���O
 * @param dest		�i�[��
 * @param src		�\�[�X(����)
 * @param win		���֐�
 * @param numch		�`�����l����
 * @param srcofs	src�̏����J�n�ʒu
 * @param len		��������T���v����
 *					(�e�`�����l�����Ƃ̐�; ���ۂɏ��������T���v��
 *					���̑��v��len*numch�ɂȂ�)
 */
void  InterleaveOverlappingWindow_sse(float * __restrict dest, const float * __restrict const * __restrict src,
					float * __restrict win, int numch, size_t srcofs, size_t len);
//---------------------------------------------------------------------------
#endif


#endif
