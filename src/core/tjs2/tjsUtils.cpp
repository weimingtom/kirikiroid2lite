//---------------------------------------------------------------------------
/*
	TJS2 Script Engine
	Copyright (C) 2000 W.Dee <dee@kikyou.info> and contributors

	See details of license at "license.txt"
*/
//---------------------------------------------------------------------------
// utility functions
//---------------------------------------------------------------------------
#include "tjsCommHead.h"

#include "tjsUtils.h"
#include <pthread.h>
#include "TickCount.h"
#include "Platform.h"
#if defined(ANDROID)
#include <android/log.h>
#endif

namespace TJS
{

struct tTJSCriticalSectionImpl {
	pthread_mutex_t _mutex;
	pthread_t* _tid;
	tTJSCriticalSectionImpl() : _tid(NULL) {
#if defined(ANDROID)
		__android_log_print(ANDROID_LOG_ERROR, "tjsUtils.cpp", "%s", "*** *** tTJSCriticalSectionImpl");
#endif
		pthread_mutex_init(&_mutex, NULL);
	}
	virtual ~tTJSCriticalSectionImpl() {
		pthread_mutex_destroy(&_mutex);
#if 1
if (_tid) {
delete _tid;
_tid = NULL;
}
#endif
	}
	bool lock();
	void unlock();
};

bool tTJSCriticalSectionImpl::lock() {
	pthread_t *id = new pthread_t();
	*id = pthread_self();
#if 0	
	pthread_mutex_lock(&_mutex);
	if (_tid != NULL && pthread_equal(*_tid, *id)) {
		return false;
	}
#else
//original method
	if (_tid != NULL && pthread_equal(*_tid, *id)) return false;
	pthread_mutex_lock(&_mutex);
#endif	
	_tid = id;
	return true;
}

void tTJSCriticalSectionImpl::unlock() {
#if 0	
	delete _tid;
	_tid = NULL;
#endif	
	pthread_mutex_unlock(&_mutex);
}

bool tTJSCriticalSection::lock() {
	return _impl->lock();
}

void tTJSCriticalSection::unlock() {
	_impl->unlock();
}

tTJSCriticalSection::tTJSCriticalSection() {
	_impl = new tTJSCriticalSectionImpl;
}

tTJSCriticalSection::~tTJSCriticalSection() {
	delete _impl;
}

void tTJSSpinLock::lock() {
#if 0
	while (atom_lock.test_and_set(boost::memory_order_acquire)) {
		boost::this_thread::yield();
//		TVPRelinquishCPU();
	}
#else
#if 0//!defined(ANDROID)	
	pthread_spin_lock(&atom_lock_spin);
#else
	pthread_mutex_lock(&atom_lock_spin);
#endif
#endif
}

void tTJSSpinLock::unlock() {
#if 0
	atom_lock.clear(boost::memory_order_release);
#else
#if 0//!defined(ANDROID)	
	pthread_spin_unlock(&atom_lock_spin);
#else
	pthread_mutex_unlock(&atom_lock_spin);
#endif
#endif
}

#if 1
tTJSSpinLock::tTJSSpinLock() {
#if 0
	unlock();
#else
#if 0 //!defined(ANDROID)	
	atom_lock_spin = 0;
	pthread_spin_init(&atom_lock_spin, PTHREAD_PROCESS_PRIVATE);	 
#else
	//atom_lock_spin = 0;
#if defined(ANDROID)
	__android_log_print(ANDROID_LOG_ERROR, "tjsUtils.cpp", "%s", "*** *** tTJSSpinLock");
#endif
	pthread_mutex_init(&atom_lock_spin, NULL);	
#endif
#endif
}
#endif
tTJSSpinLockHolder::tTJSSpinLockHolder(tTJSSpinLock &lock) {
	lock.lock();
	Lock = &lock;
}

tTJSSpinLockHolder::~tTJSSpinLockHolder() {
	if (Lock) {
		Lock->unlock();
	}
}

//---------------------------------------------------------------------------
iTJSDispatch2 * TJSObjectTraceTarget;
//---------------------------------------------------------------------------
static void TJSTrimStringLength(tTJSString &str, tjs_int len)
{
	if(str.GetLen() > len)
	{
		tjs_char *p = str.Independ();
		p[len] = 0; // trim length
		if(len >= 3)
		{
			p[len-1] = TJS_W('.');
			p[len-2] = TJS_W('.');
			p[len-3] = TJS_W('.');
		}
		str.FixLen();
	}
}
//---------------------------------------------------------------------------
const tjs_char * TJSVariantTypeToTypeString(tTJSVariantType type)
{
	switch(type)
	{
	case tvtVoid: return (const tjs_char *)TJS_W("void");
	case tvtInteger: return (const tjs_char *)TJS_W("int");
	case tvtReal: return (const tjs_char *)TJS_W("real");
	case tvtString: return (const tjs_char *)TJS_W("string");
	case tvtOctet: return (const tjs_char *)TJS_W("octet");
	case tvtObject: return (const tjs_char *)TJS_W("object");
	}
	return (const tjs_char *)TJS_W("unknown");
}
//---------------------------------------------------------------------------
tTJSString TJSVariantToReadableString(const tTJSVariant &val,
	tjs_int maxlen)
{
	// convert given variant to human-readable string
	// ( eg. "(string)\"this is a\\nstring\"" )

	tTJSVariantType type = val.Type();

	switch(type)
	{
	case tvtVoid:
	  {
		tTJSString str(TJS_W("(void)"));
		TJSTrimStringLength(str, maxlen);
		return str;
	  }
	case tvtInteger:
	  {
		tTJSString str(TJS_W("(int)"));
		str += (tTJSString)val;
		TJSTrimStringLength(str, maxlen);
		return str;
	  }
	case tvtReal:
	  {
		tTJSString str(TJS_W("(real)"));
		str += (tTJSString)val;
		TJSTrimStringLength(str, maxlen);
		return str;
	  }
	case tvtString:
	  {
		tTJSString str(TJS_W("(string)\""));
		str += ttstr(val).EscapeC();
		str += TJS_W("\"");
		TJSTrimStringLength(str, maxlen);
		return str;
	  }
	case tvtOctet:
	  {
		// TODO: octet conversion
		tTJSString str(TJS_W("(octet)<% "));
		tTJSVariantString * s = TJSOctetToListString(val.AsOctetNoAddRef());
		try
		{
			str += s;
		}
		catch(...)
		{
			if(s) s->Release();
#if defined(ANDROID)
			throw;
#elif defined(_MSC_VER)	
			__debugbreak(); throw;
#else
			throw;
#endif	
		}
		if(s) s->Release();
		str += TJS_W(" %>");
		TJSTrimStringLength(str, maxlen);
		return str;
	  }
	case tvtObject:
	  {
		tTJSString str(TJS_W("(object)"));
		try
		{
			str += ttstr(val);
		}
		catch(...)
		{
		}
		TJSTrimStringLength(str, maxlen);
		return str;
	  }
	}
	return TJS_W("");
}
//---------------------------------------------------------------------------
tTJSString TJSVariantToExpressionString(const tTJSVariant &val)
{
	// convert given variant to string which can be interpret as an expression.
	// this function does not convert objects ( returns empty string )

	tTJSVariantType type = val.Type();

	switch(type)
	{
	case tvtVoid:
	  {
		return TJS_W("void");
	  }
	case tvtInteger:
	  {
		return ttstr(val);
	  }
	case tvtReal:
	  {
		tTJSString str;
		tTJSVariantString *s = TJSRealToHexString(val.AsReal());
		try
		{
			str = s;
		}
		catch(...)
		{
			if(s) s->Release();
#if defined(ANDROID)
			throw;
#elif defined(_MSC_VER)	
			__debugbreak(); throw;
#else
			throw;
#endif	
		}
		if(s) s->Release();
		return str + TJS_W(" /") TJS_W("* ") + ttstr(val) + TJS_W(" *") TJS_W("/");
	  }
	case tvtString:
	  {
		tTJSString str(TJS_W("\""));
		str += ttstr(val).EscapeC();
		str += TJS_W("\"");
		return str;
	  }
	case tvtOctet:
	  {
		tTJSString str(TJS_W("<% "));
		tTJSVariantString * s = TJSOctetToListString(val.AsOctetNoAddRef());
		try
		{
			str += s;
		}
		catch(...)
		{
			if(s) s->Release();
#if defined(ANDROID)
			throw;
#elif defined(_MSC_VER)	
			__debugbreak(); throw;
#else
			throw;
#endif	
		}
		if(s) s->Release();
		str += TJS_W(" %>");
		return str;
	  }
	case tvtObject:
	  {
		return TJS_W("");
	  }
	}
	return TJS_W("");
}
//---------------------------------------------------------------------------




//---------------------------------------------------------------------------
// TJSAlignedAlloc : aligned memory allocator
//---------------------------------------------------------------------------
// template classes to determine an integer type which have the same size as void *.
struct tTJSPointerSizeEnum { enum tTJSPointerSize { size = sizeof(void*) }; };
template <tjs_int size> struct tTJSPointerSizedIntegerBase { typedef long type; };
template <> struct tTJSPointerSizedIntegerBase<8> { typedef tjs_uint64 type; };
template <> struct tTJSPointerSizedIntegerBase<4> { typedef tjs_uint32 type; };
struct tTJSPointerSizedInteger : public tTJSPointerSizedIntegerBase< tTJSPointerSizeEnum::size > {};
//---------------------------------------------------------------------------
void * TJSAlignedAlloc(tjs_uint bytes, tjs_uint align_bits)
{
	// aligned memory allocation is to be used to gain performance on some processors.
	tjs_int align = 1 << align_bits;
	void *ptr = (void *)(new tjs_uint8[bytes + align + sizeof(void*)]);
	void *org_ptr = ptr;
	tTJSPointerSizedInteger::type *iptr =
		reinterpret_cast<tTJSPointerSizedInteger::type *>(&ptr);
	*iptr += align + sizeof(void*);
	*iptr &= ~(tTJSPointerSizedInteger::type)(align - 1);
	(reinterpret_cast<void**>(ptr))[-1] = org_ptr;
	return ptr;
}
//---------------------------------------------------------------------------
void TJSAlignedDealloc(void *ptr)
{
	delete [] (tjs_uint8*)((reinterpret_cast<void**>(ptr))[-1]);
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
// floating-point class checker
//---------------------------------------------------------------------------
tjs_uint32 TJSGetFPClass(tjs_real r)
{
	tjs_uint64 *ui64 = (tjs_uint64*)&r;

	if(TJS_IEEE_D_IS_NaN(*ui64))
	{
		if(TJS_IEEE_D_SIGN_MASK & *ui64)
			return TJS_FC_CLASS_NAN | TJS_FC_SIGN_MASK;
		else
			return TJS_FC_CLASS_NAN;
	}
	if(TJS_IEEE_D_IS_INF(*ui64))
	{
		if(TJS_IEEE_D_SIGN_MASK & *ui64)
			return TJS_FC_CLASS_INF | TJS_FC_SIGN_MASK;
		else
			return TJS_FC_CLASS_INF;
	}
	if(TJS_IEEE_D_SIGN_MASK & *ui64)
		return TJS_FC_CLASS_NORMAL | TJS_FC_SIGN_MASK;
	else
		return TJS_FC_CLASS_NORMAL;
}
//---------------------------------------------------------------------------

}
