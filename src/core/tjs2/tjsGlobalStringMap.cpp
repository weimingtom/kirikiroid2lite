//---------------------------------------------------------------------------
/*
	TJS2 Script Engine
	Copyright (C) 2000 W.Dee <dee@kikyou.info> and contributors

	See details of license at "license.txt"
*/
//---------------------------------------------------------------------------
// TJS Global String Map
//---------------------------------------------------------------------------
#include "tjsCommHead.h"

#include "tjsGlobalStringMap.h"
#include "tjsHashSearch.h"
#include <pthread.h>
#if defined(ANDROID)
#include <android/log.h>
#endif

/*
	Global String Map is a large string hash table, to share the string
	which is already 'known' by the hash table, using TJS2 string heap
	management mechanism. This will dramatically decrease string heap
	size which is used for constant strings (member names, string literals,
	etc ...).
*/

#define TJS_GLOBAL_STRING_MAP_SIZE 5000



namespace TJS
{

//---------------------------------------------------------------------------
// tTJSGlobalStringMap - hash map to keep constant strings shared
//---------------------------------------------------------------------------
class tTJSGlobalStringMap;
static tTJSGlobalStringMap * TJSGlobalStringMap = NULL;
struct tTJSEmptyClass {};
class tTJSGlobalStringMap
{
	tTJSHashCache<tTJSString, tTJSEmptyClass, tTJSHashFunc<ttstr>, 1024> Hash;

	tjs_int RefCount;

public:
	tTJSGlobalStringMap()  : Hash (TJS_GLOBAL_STRING_MAP_SIZE)
	{
		RefCount = 1;
		TJSGlobalStringMap = this;
	}

protected:
	~tTJSGlobalStringMap()
	{
		TJSGlobalStringMap = NULL;
	}

public:
	tTJSString _Map(const tTJSString & string)
	{
		// Search Hash, and return the string which to be shared

		const tTJSString * key;
		tTJSEmptyClass * v;

		tjs_uint32 hash = tTJSHashFunc<ttstr>::Make(string);

		if(Hash.FindAndTouchWithHash(string, hash, key, v))
		{
			ttstr ret(*key);
			if(ret.GetHint()) *(ret.GetHint()) = hash;
			return ret;
		}
		else
		{
			Hash.AddWithHash(string, hash, tTJSEmptyClass());
			ttstr ret(string);
			if(ret.GetHint()) *(ret.GetHint()) = hash;
			return ret;
		}
	}

protected:
	void _AddRef() { RefCount ++; }
	void _Release() { if(RefCount == 1) delete this; else RefCount --; }

public:
	static void AddRef()
	{
		if(TJSGlobalStringMap)
			TJSGlobalStringMap->_AddRef();
		else
			new tTJSGlobalStringMap();
	}

	static void Release()
	{
		if(TJSGlobalStringMap)
			TJSGlobalStringMap->_Release();
	}

	static ttstr Map(const ttstr & string)
	{
		if(TJSGlobalStringMap)
			return TJSGlobalStringMap->_Map(string);
		else
			return string;
	}
};
//---------------------------------------------------------------------------
void TJSAddRefGlobalStringMap()
{
	tTJSGlobalStringMap::AddRef();
}
//---------------------------------------------------------------------------
void TJSReleaseGlobalStringMap()
{
	tTJSGlobalStringMap::Release();
}
//---------------------------------------------------------------------------
static pthread_mutex_t *_mutex = NULL;
ttstr TJSMapGlobalStringMap(const ttstr & string)
{
	// for multi-thread
	if (_mutex == NULL)
	{
#if defined(ANDROID)
		__android_log_print(ANDROID_LOG_ERROR, "tjsGlobalStringMap.cpp", "%s", "*** *** TJSMapGlobalStringMap not release");
		//FIXME: not release this lock
#endif
		_mutex = new pthread_mutex_t();
		pthread_mutex_init(_mutex, NULL);
	}
	pthread_mutex_lock(_mutex);
	ttstr str = tTJSGlobalStringMap::Map(string);
	pthread_mutex_unlock(_mutex);
	return str;
}
//---------------------------------------------------------------------------




} // namespace TJS

