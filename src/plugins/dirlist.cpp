//---------------------------------------------------------------------------
//#include <windows.h>
//#include "tp_stub.h"
#include "tp_stub.h"
#include "ncbind/ncbind.hpp"
//---------------------------------------------------------------------------

#define NCB_MODULE_NAME TJS_W("dirlist.dll")


//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class tGetDirListFunction : public tTJSDispatch
{
	tjs_error TJS_INTF_METHOD FuncCall(
		tjs_uint32 flag, const tjs_char * membername, tjs_uint32 *hint,
		tTJSVariant *result,
		tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
	{
		if(membername) return TJS_E_MEMBERNOTFOUND;

		if(numparams < 1) return TJS_E_BADPARAMCOUNT;

		ttstr dir(*param[0]);

		if(dir.GetLastChar() != TJS_W('/'))
			TVPThrowExceptionMessage(TJS_W("'/' must be specified at the end of given directory name."));

		dir = TVPNormalizeStorageName(dir);

		iTJSDispatch2 * array = TJSCreateArrayObject();
		if (!result) return TJS_S_OK;
		try {
			tTJSArrayNI* ni;
			array->NativeInstanceSupport(TJS_NIS_GETINSTANCE, TJSGetArrayClassID(), (iTJSNativeInstance**)&ni);
			TVPGetLocalName(dir);
			TVPGetLocalFileListAt(dir, [ni](const ttstr &name, tTVPLocalFileInfo* s) {
				if (s->Mode & (S_IFREG | S_IFDIR)) {
					ni->Items.push_back(name);
				}
			});
			*result = tTJSVariant(array, array);
			array->Release();
		}
		catch (...) {
			array->Release();
#if defined(ANDROID)
			throw;
#elif defined(_MSC_VER)
			__debugbreak(); throw;
#else
			throw;
#endif			
		}

		return TJS_S_OK;
	}
} * GetDirListFunction;
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
static void PostRegistCallback()
{
	tTJSVariant val;

	iTJSDispatch2 * global = TVPGetScriptDispatch();

	GetDirListFunction = new tGetDirListFunction();

	val = tTJSVariant(GetDirListFunction);

	GetDirListFunction->Release();


	global->PropSet(
		TJS_MEMBERENSURE, 
		TJS_W("getDirList"), 
		NULL,
		&val, 
		global
		);


	global->Release();

	val.Clear();
}
NCB_POST_REGIST_CALLBACK(PostRegistCallback);
