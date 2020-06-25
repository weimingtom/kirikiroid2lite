
#ifndef __EXTENSION_H__
#define __EXTENSION_H__

//---------------------------------------------------------------------------
// tTVPAtInstallClass
//---------------------------------------------------------------------------
extern void TVPAddClassHandler( const tjs_char* name, iTJSDispatch2* (*handler)(iTJSDispatch2*), const tjs_char* dependences );
struct tTVPAtInstallClass {
	tTVPAtInstallClass(const tjs_char* name, iTJSDispatch2* (*handler)(iTJSDispatch2*), const tjs_char* dependences) {
		TVPAddClassHandler(name, handler, dependences);
	}
};

/*

�ȉ��̂悤�ȏ����ŁAcpp �ɓ���Ă����ƁAVM��������A�N���X��ǉ�����i�K�Œǉ����Ă���܂��B
�ÓI�����N����ƃN���X�ǉ������plugin�̗l�ȋ@�\�ł��B
plugin�͎��s���ɃN���X���ǉ����܂����Aextension�̓r���h���鎞�ɁA�����N���X��I������`�ł��B
static tTVPAtInstallClass TVPInstallClassFoo
	(TJS_W("ClassFoo"), TVPCreateNativeClass_ClassFoo,TJS_W("Window,Layer"));
�Ăяo�����֐��ɂ�global���n�����̂ŁA�K�v�ł���΂������烁���o�Ȃǎ擾���܂��B
�o�^�͌Ăяo�������Ԃ�l��iTJSDispatch2��global�ɓo�^����̂ŁA�Ăяo���ꂽ�֐����œo�^����K�v
�͂���܂���B
�o�^���ˑ��N���X��3�ԖڂɎw��\�ł����A���݂̂Ƃ��떳������Ă��܂��B
*/
extern void TVPCauseAtInstallExtensionClass( iTJSDispatch2 *global );


#endif // __EXTENSION_H__
