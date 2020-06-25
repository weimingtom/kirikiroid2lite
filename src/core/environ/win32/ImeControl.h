

#ifndef __IME_CONTROL_H__
#define __IME_CONTROL_H__

//#include <imm.h>
#include "TVPSysFont.h"

class ImeControl {
public:
	enum {
		ModeDisable,
		ModeClose,
		ModeOpen,
		ModeDontCare,
		ModeSAlpha,
		ModeAlpha,
		ModeHira,
		ModeSKata,
		ModeKata,
		ModeChinese,
		ModeSHanguel,
		ModeHanguel,
	};

private:
	HWND hWnd_;
	HIMC hOldImc_;
	bool default_open_;
	int mode_;

public:
	ImeControl( HWND hWnd ) {
		hWnd_ = hWnd;
		hOldImc_ = INVALID_HANDLE_VALUE;
		mode_ = ModeDontCare;
		default_open_ = IsOpen();
	}
	~ImeControl() {
	}
	bool IsOpen() {
		if( hOldImc_ != INVALID_HANDLE_VALUE ) return false;

		HIMC hImc = ::ImmGetContext(hWnd_);
		BOOL result = ::ImmGetOpenStatus(hImc);
		::ImmReleaseContext(hWnd_,hImc);
		return 0!=result;
	}
	void Open() {
		Enable();
		HIMC hImc = ::ImmGetContext(hWnd_);
		::ImmSetOpenStatus(hImc,TRUE);
		::ImmReleaseContext(hWnd_,hImc);
	}
	void Close() {
		Enable();
		HIMC hImc = ::ImmGetContext(hWnd_);
		::ImmSetOpenStatus(hImc,FALSE);
		::ImmReleaseContext(hWnd_,hImc);
	}
	bool IsEnableThisLocale() {
		HKL hKl = ::GetKeyboardLayout(0);
		return 0!=::ImmIsIME(hKl);
	}
	// ImmSetStatusWindowPos �֐����Ăяo���ƁA�A�v���P�[�V������ IMN_SETSTATUSWINDOWPOS ���b�Z�[�W�����M����܂��B
	void SetStatusPosition( int x, int y ) {
		POINT pt = {x,y};
		HIMC hImc = ::ImmGetContext(hWnd_);
		::ImmSetStatusWindowPos( hImc, &pt );
		::ImmReleaseContext(hWnd_,hImc);
	}
	void GetStatusPosition( int &x, int &y ) {
		POINT pt = {0,0};
		HIMC hImc = ::ImmGetContext(hWnd_);
		::ImmGetStatusWindowPos( hImc, &pt );
		::ImmReleaseContext(hWnd_,hImc);
		x = pt.x;
		y = pt.y;
	}
	void Reset() {
		if( mode_ == ModeDisable ) {
			Open();
		}
	}
	/**
	 ���̃X���b�h��IME�𖳌��ɂ���
	 */
	void Disable() {
		if( hOldImc_ == INVALID_HANDLE_VALUE ) {
			hOldImc_ = ::ImmAssociateContext(hWnd_,0);
		}
	}
	void Enable() {
		if( hOldImc_ != INVALID_HANDLE_VALUE ) {
			::ImmAssociateContext(hWnd_,hOldImc_);
			hOldImc_ = INVALID_HANDLE_VALUE;
		}
	}
	// ���̊֐����Ăяo���ƁA�A�v���P�[�V������ IMN_SETCOMPOSITIONFONT ���b�Z�[�W�����M����܂��B
	void SetCompositionFont( tTVPSysFont* font ) {
		LOGFONT logfont={0};
		font->GetFont(&logfont);
		HIMC hImc = ::ImmGetContext(hWnd_);
		::ImmSetCompositionFont( hImc, &logfont );
		::ImmReleaseContext(hWnd_,hImc);
	}
	void SetCompositionWindow( int x, int y ) {
		COMPOSITIONFORM pos;
		pos.dwStyle = CFS_POINT;
		pos.ptCurrentPos.x = x;
		pos.ptCurrentPos.y = y;
		HIMC hImc = ::ImmGetContext(hWnd_);
		::ImmSetCompositionWindow( hImc, &pos );
		::ImmReleaseContext(hWnd_,hImc);
	}
	void GetCompositionWindow( int &x, int &y ) {
		COMPOSITIONFORM pos = {0};
		pos.dwStyle = CFS_POINT;
		HIMC hImc = ::ImmGetContext(hWnd_);
		::ImmGetCompositionWindow( hImc, &pos );
		::ImmReleaseContext(hWnd_,hImc);
		x = pos.ptCurrentPos.x;
		y = pos.ptCurrentPos.y;
	}
	/**
	 * conversion : ���̓��[�h�̒l���w�肵�܂��B
	 * 		IME_CMODE_ALPHANUMERIC(0x0000)	�p�����[�h
	 * 		IME_CMODE_NATIVE(0x0001)		�Ή��������(ON)�E�p������(OFF) ���[�h
	 * 		IME_CMODE_CHINESE
	 * 		IME_CMODE_HANGEUL
	 * 		IME_CMODE_JAPANESE �ł���`���Ă���
	 * 		IME_CMODE_KATAKANA(0x0002)		�J�^�J�i(ON)�E�Ђ炪��(OFF) ���[�h
	 * 		IME_CMODE_FULLSHAPE(0x0008)		�S�p���[�h
	 * 		IME_CMODE_ROMAN(0x0010)			���[�}�����[�h
	 * 		IME_CMODE_CHARCODE(0x0020)		�L�����N�^���̓��[�h
	 * 		IME_CMODE_HANJACONVERT(0x0040)	�n���O�������ϊ����[�h
	 * 		IME_CMODE_SOFTKBD(0x0080)		�\�t�g�L�[�{�[�h���[�h
	 * 		IME_CMODE_NOCONVERSION(0x0100)	���ϊ����[�h
	 * 		IME_CMODE_EUDC(0x0200)			EUD�ϊ����[�h
	 * 		IME_CMODE_SYMBOL(0x0400)		�V���{�����[�h
	 * sentence : �ϊ����[�h�̒l���w�肵�܂��B
	 * 		IME_SMODE_NONE(0x0000)			���ϊ�
	 * 		IME_SMODE_PLURALCLAUSE(0x0001)	������D��
	 * 		IME_SMODE_SINGLECONVERT(0x0002)	�P�ϊ�
	 * 		IME_SMODE_AUTOMATIC(0x0004)		�����ϊ�
	 * 		IME_SMODE_PHRASEPREDICT(0x0008)	�A���ߕϊ�
	 */
	/*
	bool SetConversionStatus( int conversion, int sentence ) {
		return 0!=::ImmSetConversionStatus( hImc_, conversion, sentence );
	}
	*/
	/**
	 * @param mode : 
ModeDisable ���w�肷��ƁAIME�͖����ɂȂ�܂��BIME���g�p�������͂͂ł��܂��񂵁A���[�U�̑���ł�IME��L���ɂ��邱�Ƃ͂ł��܂���B : Disable
ModeClose ���w�肷��ƁAIME�͖����ɂȂ�܂��BimDisable�ƈقȂ�A���[�U�̑����IME��L���ɂ��邱�Ƃ��ł��܂��B : Close
ModeOpen ���w�肷��ƁAIME�͗L���ɂȂ�܂��B : Open
ModeDontCare ���w�肷��ƁAIME�̗L��/�����̏�Ԃ́A�O�̏�Ԃ������p���܂��B���[�U�̑���ɂ����IME��L���ɂ����薳���ɂ����肷�邱�Ƃ��ł��܂��B���{����͂ɂ����ẮA���p/�S�p���������[�U�Ɏ��R�ɓ��͂�����ꍇ�̈�ʓI�ȃ��[�h�ł��B
ModeSAlpha ���w�肷��ƁAIME�͗L���ɂȂ�A���p�A���t�@�x�b�g���̓��[�h�ɂȂ�܂��B : IME_CMODE_ALPHANUMERIC
ModeAlpha ���w�肷��ƁAIME�͗L���ɂȂ�A�S�p�A���t�@�x�b�g���̓��[�h�ɂȂ�܂��B : IME_CMODE_FULLSHAPE
ModeHira ���w�肷��ƁAIME�͗L���ɂȂ�A�Ђ炪�ȓ��̓��[�h�ɂȂ�܂��B
ModeSKata ���w�肷��ƁAIME�͗L���ɂȂ�A���p�J�^�J�i���̓��[�h�ɂȂ�܂��B : IME_CMODE_KATAKANA
ModeKata ���w�肷��ƁAIME�͗L���ɂȂ�A�S�p�J�^�J�i���̓��[�h�ɂȂ�܂��B : IME_CMODE_KATAKANA IME_CMODE_NATIVE
ModeChinese ���w�肷��ƁAIME�͗L���ɂȂ�A2�o�C�g��������͂��󂯕t���郂�[�h�ɂȂ�܂��B���{����ł͎g�p�ł��܂���B : IME_CMODE_CHINESE
ModeSHanguel ���w�肷��ƁAIME�͗L���ɂȂ�A1�o�C�g�؍�����͂��󂯕t���郂�[�h�ɂȂ�܂��B���{����ł͎g�p�ł��܂���B : IME_CMODE_HANJACONVERT
ModeHanguel ���w�肷��ƁAIME�͗L���ɂȂ�A2�o�C�g�؍�����͂��󂯕t���郂�[�h�ɂȂ�܂��B���{����ł͎g�p�ł��܂���B : IME_CMODE_HANGEUL
	 */
	void SetIme( int mode ) {
		mode_ = mode;
		HIMC hImc = ::ImmGetContext(hWnd_);
		DWORD conversion, sentence;
		::ImmGetConversionStatus( hImc, &conversion, &sentence );
		switch( mode ) {
		case ModeDisable:
			if( hOldImc_ == INVALID_HANDLE_VALUE ) {
				hOldImc_ = ::ImmAssociateContext(hWnd_,0);
			}
			break;
		case ModeClose:
			::ImmSetOpenStatus(hImc,FALSE);
			break;
		case ModeOpen:
			::ImmSetOpenStatus(hImc,TRUE);
			break;
		case ModeDontCare:
			break;
		case ModeSAlpha:
			::ImmSetOpenStatus(hImc,TRUE);
			::ImmSetConversionStatus( hImc, IME_CMODE_ALPHANUMERIC, sentence );
			break;
		case ModeAlpha:
			::ImmSetOpenStatus(hImc,TRUE);
			::ImmSetConversionStatus( hImc, IME_CMODE_ALPHANUMERIC | IME_CMODE_FULLSHAPE, sentence );
			break;
		case ModeHira:
			::ImmSetOpenStatus(hImc,TRUE);
			::ImmSetConversionStatus( hImc, IME_CMODE_NATIVE | IME_CMODE_FULLSHAPE, sentence );
			break;
		case ModeSKata:
			::ImmSetOpenStatus(hImc,TRUE);
			::ImmSetConversionStatus( hImc, IME_CMODE_NATIVE | IME_CMODE_KATAKANA, sentence );
			break;
		case ModeKata:
			::ImmSetOpenStatus(hImc,TRUE);
			::ImmSetConversionStatus( hImc, IME_CMODE_NATIVE | IME_CMODE_KATAKANA | IME_CMODE_FULLSHAPE, sentence );
			break;
		case ModeChinese:
			::ImmSetOpenStatus(hImc,TRUE);
			::ImmSetConversionStatus( hImc, IME_CMODE_NATIVE | IME_CMODE_FULLSHAPE, sentence );
			break;
		case ModeSHanguel:
			::ImmSetOpenStatus(hImc,TRUE);
			::ImmSetConversionStatus( hImc, IME_CMODE_NATIVE, sentence );
			break;
		case ModeHanguel:
			::ImmSetOpenStatus(hImc,TRUE);
			::ImmSetConversionStatus( hImc, IME_CMODE_NATIVE | IME_CMODE_FULLSHAPE, sentence );
			break;
		}
		::ImmReleaseContext(hWnd_,hImc);
	}
};


#endif // __IME_CONTROL_H__
