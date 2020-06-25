//---------------------------------------------------------------------------
/*
	Risa [�肳]      alias �g���g��3 [kirikiri-3]
	 stands for "Risa Is a Stagecraft Architecture"
	Copyright (C) 2000 W.Dee <dee@kikyou.info> and contributors

	See details of license at "license.txt"
*/
//---------------------------------------------------------------------------
//! @file
//! @brief Win32 GDI �o�R�ł�FreeType Face
/**
 * @note	�t�H���g������t�H���g�t�@�C�����𓾂铮�삪OS���ƂɈقȂ邽�߁A
 *			tFreeTypeFace ���v���b�g�t�H�[�����ƂɈقȂ��������ƂȂ�B
 */
//#include "../prec.h"
#include "tjsCommHead.h"
#include "NativeFreeTypeFace.h"
#include "FreeType.h"
#include "MsgIntf.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4819)
#endif
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_TRUETYPE_TAGS_H
#include FT_TRUETYPE_TABLES_H
#include FT_TRUETYPE_UNPATENTED_H
#ifdef _MSC_VER
#pragma warning(pop)
#endif

//---------------------------------------------------------------------------

#define TVP_TT_TABLE_ttcf  (('t' << 0) + ('t' << 8) + ('c' << 16) + ('f' << 24))
#define TVP_TT_TABLE_name  (('n' << 0) + ('a' << 8) + ('m' << 16) + ('e' << 24))

//---------------------------------------------------------------------------
/**
 * �R���X�g���N�^
 * @param fontname	�t�H���g��
 * @param options	�I�v�V����
 */
tNativeFreeTypeFace::tNativeFreeTypeFace(const std::wstring &fontname,
	tjs_uint32 options)
{
	// �t�B�[���h�̃N���A
	FaceName = fontname;
	Face = NULL;
	memset(&Stream, 0, sizeof(Stream));
	DC = NULL;
	OldFont = NULL;
	IsTTC = false;

	unsigned char *name_content    = NULL; // Windows ����擾���� name �^�O�̓��e
	unsigned char *name_content_ft = NULL; // FreeType ����擾���� name �^�O�̓��e
	tjs_int name_content_size;

	// TrueType ���C�u�������t�b�N
	try
	{
		// �w��̃t�H���g���������f�o�C�X�R���e�L�X�g���쐬����
		DC = GetDC(0);
		LOGFONT l;
		l.lfHeight = -12;
		l.lfWidth = 0;
		l.lfEscapement = 0;
		l.lfOrientation = 0;
		l.lfWeight = 400;
		l.lfItalic = FALSE;
		l.lfUnderline = FALSE;
		l.lfStrikeOut = FALSE;
		l.lfCharSet = DEFAULT_CHARSET;
		l.lfOutPrecision = OUT_DEFAULT_PRECIS;
		l.lfQuality = DEFAULT_QUALITY;
		l.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
		TJS_strcpy( l.lfFaceName, fontname.c_str() );
		l.lfFaceName[LF_FACESIZE-1] = TJS_W('\0');

		HFONT newfont = CreateFontIndirect(&l);
		OldFont = static_cast<HFONT>(SelectObject(DC, newfont));
		ZeroMemory( &TextMetric, sizeof(TextMetric) );
		::GetTextMetrics( DC, &TextMetric );

		// ���̃t�H���g�� GetFontData API �ň����邩�ǂ�����
		// 'name' �^�O�̓��e���擾���悤�Ƃ��邱�ƂŃ`�F�b�N����
		// (name �^�O�� GetFontData �������悤�� TrueType/OpenType �t�H���g�ɂ�
		//  �K�������Ă���)
		DWORD result = GetFontData(DC, TVP_TT_TABLE_name, 0, NULL, 0);
		if(result == GDI_ERROR)
		{
			// �G���[; GetFontData �ł͈����Ȃ�����
			TVPThrowExceptionMessage( TJS_W("Font '%1$s' cannot be used"), fontname );
		}

		//- ���̎��_�� result �� name �^�O�̓��e������̂ɕK�v�ȃo�C�g��
		name_content_size = result;
		name_content = new unsigned char [name_content_size]; // ���������m��
		name_content_ft = new unsigned char [name_content_size]; // ���������m��

		//- name �^�O�̓��e���������ɓǂݍ���
		result = GetFontData(DC, TVP_TT_TABLE_name, 0, name_content, name_content_size);
		if(result == GDI_ERROR)
		{
			// �G���[; �������ɓǂݍ��ނ��Ƃ��o���Ȃ�����
			TVPThrowExceptionMessage( TJS_W("Font '%1$s' cannot be used"), fontname );
		}

		// �t�H���g�t�@�C���̃T�C�Y���擾����
		tjs_int fontsize;

		//- TTC (True Type Collection) �t�@�C���̃`�F�b�N
		//- GetFontData API �̎d�l�ł́ATTC �t�@�C���ɑ΂��ẮA���ݑI������Ă���
		//- �t�H���g�ɑ΂����񂵂��Ԃ��Ȃ��B������ FreeType �� TTC �t�@�C���S�̂�
		//- ����K�v�Ƃ���B���̏ꍇ�AGetFontData �� 'ttcf' �𓾂�悤�Ɏw�������
		//- �t�@�C���S�̂̏��𓾂邱�Ƃ��o����B
		//- �Q�� : microsoft.public.win32.programmer.gdi GetFontData and TTC fonts
		unsigned char buf[4];
		result = GetFontData(DC, TVP_TT_TABLE_ttcf, 0, &buf, 1);
		if(result == 1)
		{
			// TTC �t�@�C�����Ǝv����
			result = GetFontData(DC, TVP_TT_TABLE_ttcf, 0, NULL, 0);
			IsTTC = true;
		}
		else
		{
			result = GetFontData(DC, 0, 0, NULL, 0);
		}

		if(result == GDI_ERROR)
		{
			// �G���[; GetFontData �ł͈����Ȃ�����
			TVPThrowExceptionMessage( TJS_W("Font '%1$s' cannot be used"), fontname );
		}
		fontsize = result;

		// FT_StreamRec �̊e�t�B�[���h�𖄂߂�
		FT_StreamRec * fsr = &Stream;
		fsr->base = 0;
		fsr->size = fontsize;
		fsr->pos = 0;
		fsr->descriptor.pointer = this;
		fsr->pathname.pointer = NULL;
		fsr->read = IoFunc;
		fsr->close = CloseFunc;

		// FreeType �ŊJ��
		// ������ 0 �Ԃ� Face ���J��
		// (���̎��_�ŊJ�����Ƃ��o���Ȃ���Η�O�𔭐�������)
		int index = 0;
		if(!OpenFaceByIndex(index))
		{
			TVPThrowExceptionMessage( TJS_W("Font '%1$s' cannot be used"), fontname );
		}

		// GDI�����ݑI�����Ă���t�@�C����FreeType���A�N�Z�X���Ă���t�@�C����
		// ���ۂɍ��v���Ă��邩�ǂ������Aname �^�O�̈�v�Ō���B
		// �Ƃ��� TTC �t�@�C���̏ꍇ�́Aname �^�O�̈�v�����Ȃ���Aface �̃C���f�b
		// �N�X��������₵�Ȃ���A�ΏۂƂ���t�H���g��T���Ȃ���΂Ȃ�Ȃ��B
		while(true)
		{
			// FreeType ����Aname �^�O�̃T�C�Y���擾����
			FT_ULong length = 0;
			FT_Error err = FT_Load_Sfnt_Table(Face, TTAG_name, 0, NULL, &length);
			if(err)
			{
				TVPThrowExceptionMessage( TJS_W("Font '%1$s' cannot be used"), fontname );
			}

			// FreeType ���瓾�� name �^�O�̒����� Windows ���瓾�������Ɣ�r
			if(length == name_content_size)
			{
				// FreeType ���� name �^�O���擾
				err = FT_Load_Sfnt_Table(Face, TTAG_name, 0, name_content_ft, &length);
				if(err)
				{
					TVPThrowExceptionMessage( TJS_W("Font '%1$s' cannot be used"), fontname );
				}
				// FreeType ����ǂݍ��� name �^�O�̓��e�ƁAWindows ����ǂݍ���
				// name �^�O�̓��e���r����B
				// ��v���Ă���΂��� index �̃t�H���g���g���B
				if(!memcmp(name_content, name_content_ft, name_content_size))
				{
					// ��v����
					// face �͊J�����܂�
					break; // ���[�v�𔲂���
				}
			}

			// ��v���Ȃ�����
			// �C���f�b�N�X������₵�A���� face ���J��
			index ++;

			if(!OpenFaceByIndex(index))
			{
				// ��v���� face ���Ȃ��܂� �C���f�b�N�X���͈͂𒴂����ƌ�����
				// index �� 0 �ɐݒ肵�Ă��� index ���J���A���[�v�𔲂���
				index = 0;
				if(!OpenFaceByIndex(index))
				{
					TVPThrowExceptionMessage( TJS_W("Font '%1$s' cannot be used"), fontname );
				}
				break;
			}
		}

	}
	catch(...)
	{
		Clear();
		if(name_content) delete [] name_content;
		if(name_content_ft) delete [] name_content_ft;
		__debugbreak(); throw;
	}
	delete [] name_content;
	delete [] name_content_ft;

}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/**
 * �f�X�g���N�^
 */
tNativeFreeTypeFace::~tNativeFreeTypeFace()
{
	Clear();
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/**
 * FreeType �� Face �I�u�W�F�N�g��Ԃ�
 */
FT_Face tNativeFreeTypeFace::GetFTFace() const
{
	return Face;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
/**
 * �`��ł��Ȃ������̎��ɕ`�悷�镶���R�[�h
 */
tjs_char tNativeFreeTypeFace::GetDefaultChar() const
{
	FT_UInt ret = FT_Get_Char_Index( Face, TextMetric.tmDefaultChar );
	if( ret != 0 ) return TextMetric.tmDefaultChar;
	ret = FT_Get_Char_Index( Face, TextMetric.tmBreakChar );
	if( ret != 0 ) return TextMetric.tmBreakChar;
	return L' ';
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
/**
 * ���̃t�H���g�t�@�C���������Ă���t�H���g��z��Ƃ��ĕԂ�
 * @param dest	�i�[��z��
 */
void tNativeFreeTypeFace::GetFaceNameList(std::vector<std::wstring> & dest) const
{
	// ����Face�̏ꍇ�A����Face�͓��肳��Ă��邽�߁A���p�\��
	// Face ���͏��1�ŁA�t�H���g���͂��̃I�u�W�F�N�g���\�z���ꂽ�ۂɓn���ꂽ
	// �t�H���g���ƂȂ�
	dest.clear();
	dest.push_back(FaceName);
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
/**
 * �S�ẴI�u�W�F�N�g���������
 */
void tNativeFreeTypeFace::Clear()
{
	if(Face) FT_Done_Face(Face), Face = NULL;
	if(OldFont && DC)
	{
		HFONT font = static_cast<HFONT>(SelectObject(DC, OldFont));
		DeleteObject(font);
		OldFont = NULL;
	}
	if(DC) ReleaseDC(0, DC), DC = NULL;
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
/**
 * FreeType �p �X�g���[���ǂݍ��݊֐�
 * @param stream	FT_Stream�ւ̃|�C���^
 * @param offset	�X�g���[���擪����̃I�t�Z�b�g
 * @param buffer	�i�[��o�b�t�@
 * @param count		�ǂݏo���o�C�g��
 * @return	���o�C�g�ǂݍ��܂ꂽ��
 */
unsigned long tNativeFreeTypeFace::IoFunc(
			FT_Stream stream,
			unsigned long   offset,
			unsigned char*  buffer,
			unsigned long   count )
{
	if(count != 0)
	{
		tNativeFreeTypeFace * _this =
			static_cast<tNativeFreeTypeFace*>(stream->descriptor.pointer);
		DWORD result = GetFontData(_this->DC, 
				_this->IsTTC ? TVP_TT_TABLE_ttcf : 0,
				offset, buffer, count);
		if(result == GDI_ERROR)
		{
			// �G���[
			return 0;
		}
		return result;
	}
	return 0;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/**
 * FreeType �p �X�g���[���폜�֐�
 * @param stream	FT_Stream�ւ̃|�C���^
 */
void tNativeFreeTypeFace::CloseFunc( FT_Stream  stream )
{
	// �������Ȃ�
}
//---------------------------------------------------------------------------

extern FT_Library FreeTypeLibrary;
//---------------------------------------------------------------------------
/**
 * �w��C���f�b�N�X��Face���J��
 * @param index	�J��index
 * @return	Face���J����� true �����łȂ���� false
 */
bool tNativeFreeTypeFace::OpenFaceByIndex(int index)
{
	if(Face) FT_Done_Face(Face), Face = NULL;

	FT_Parameter parameters[1];
	parameters[0].tag = FT_PARAM_TAG_UNPATENTED_HINTING; // Apple�̓���������s��
	parameters[0].data = NULL;

	FT_Open_Args args;
	memset(&args, 0, sizeof(args));
	args.flags = FT_OPEN_STREAM | FT_OPEN_PARAMS;
	args.stream = &Stream;
	args.driver = 0;
	args.num_params = 1;
	args.params = parameters;

	FT_Error err = FT_Open_Face(FreeTypeLibrary, &args, index, &Face);
	return err == 0;
}
//---------------------------------------------------------------------------





//---------------------------------------------------------------------------


