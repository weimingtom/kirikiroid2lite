//---------------------------------------------------------------------------
/*
	Risa [�肳]      alias �g���g��3 [kirikiri-3]
	 stands for "Risa Is a Stagecraft Architecture"
	Copyright (C) 2000 W.Dee <dee@kikyou.info> and contributors

	See details of license at "license.txt"
*/
//---------------------------------------------------------------------------
//! @file
// @brief Win32 GDI �o�R�ł�FreeType Face
//---------------------------------------------------------------------------
#ifndef _NATIVEFREETYPEFACE_H_
#define _NATIVEFREETYPEFACE_H_

#include "tvpfontstruc.h"
#include "FreeTypeFace.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4819)
#endif
#include <ft2build.h>
#include FT_FREETYPE_H
#ifdef _MSC_VER
#pragma warning(pop)
#endif

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//! @brief		Win32 GDI �o�R�ł�FreeType Face �N���X
//---------------------------------------------------------------------------
class tNativeFreeTypeFace : public tBaseFreeTypeFace
{
protected:
	std::wstring FaceName;	//!< Face�� = �t�H���g��
	FT_Face Face;	//!< FreeType face �I�u�W�F�N�g

private:
// 	HDC DC;			//!< �f�o�C�X�R���e�L�X�g
// 	HFONT OldFont;	//!< �f�o�C�X�R���e�L�X�g�Ɍ��X�o�^����Ă����Â��t�H���g
	bool IsTTC;		//!< TTC(TrueTypeCollection)�t�@�C���������Ă���ꍇ�ɐ^
	FT_StreamRec Stream;
//	TEXTMETRIC TextMetric;

public:
	tNativeFreeTypeFace(const std::wstring &fontname, tjs_uint32 options);
	virtual ~tNativeFreeTypeFace();

	virtual FT_Face GetFTFace() const;
	virtual void GetFaceNameList(std::vector<std::wstring> & dest) const; 

	bool GetIsTTC() const { return IsTTC; }
	tjs_char GetDefaultChar() const;

private:
	void Clear();
	static unsigned long IoFunc(
			FT_Stream stream,
			unsigned long   offset,
			unsigned char*  buffer,
			unsigned long   count );
	static void CloseFunc( FT_Stream  stream );

	bool OpenFaceByIndex(int index);

};
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------




#endif /*_NATIVEFREETYPEFACE_H_*/
