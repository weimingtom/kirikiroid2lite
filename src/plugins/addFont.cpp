#include "ncbind/ncbind.hpp"

#include <vector>
#include "FontImpl.h"
using namespace std;

#define NCB_MODULE_NAME TJS_W("addFont.dll")

struct FontEx
{
	static tjs_error TJS_INTF_METHOD addFont(tTJSVariant *result,
											 tjs_int numparams,
											 tTJSVariant **param,
											 iTJSDispatch2 *objthis) {
		if (numparams < 1) return TJS_E_BADPARAMCOUNT;

		ttstr filename = TVPGetPlacedPath(*param[0]);
		if (filename.length()) {
			int ret = TVPEnumFontsProc(filename);
			if (result) {
				*result = (int)ret;
			}
			return TJS_S_OK;
		}
		return TJS_S_OK;
	}
};

NCB_ATTACH_CLASS(FontEx, System) {
	RawCallback("addFont", &FontEx::addFont, TJS_STATICMEMBER);
}
