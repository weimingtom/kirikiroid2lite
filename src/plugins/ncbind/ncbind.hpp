#ifndef _ncbind_hpp_
#define _ncbind_hpp_

#include "PluginStub.h"
#include "ncb_invoke.hpp"
#include <map>
#include <list>

////////////////////////////////////////

#define NCB_WARN(n)     TVPAddLog(ttstr(n))
#define NCB_WARN_2(a,b) TVPAddLog(ttstr(a) + ttstr(b))
#define NCB_WARN_W(str) NCB_WARN(TJS_W(str))

#if (defined(DEBUG) || defined(_DEBUG))
#define NCB_LOG(n)     NCB_WARN(n)
#define NCB_LOG_2(a,b) NCB_WARN_2(a,b)
#define NCB_LOG_W(str) NCB_WARN_W(str)
#else
#define NCB_LOG_VOID   ((void)0)
#define NCB_LOG(n)     NCB_LOG_VOID
#define NCB_LOG_2(a,b) NCB_LOG_VOID
#define NCB_LOG_W(str) NCB_LOG_VOID
#endif


////////////////////////////////////////
struct ncbTypedefs {
	typedef tjs_char const*           NameT;
	typedef tjs_uint32                FlagsT;
	typedef tjs_int32                 IdentT;

	typedef MethodCaller              CallerT;
	typedef tTJSNativeInstanceType    InstanceTypeT;
	typedef tTJSNativeClassForPlugin  ClassObjectT;

	template <typename T> struct Tag { typedef T Type; };

	template <int  N> struct NumTag  { enum { n = N }; };
	template <bool B> struct BoolTag { enum { b = B }; };

	static inline tTJSVariantType GetVariantType(tTJSVariant const &var) { return (const_cast<tTJSVariant*>(&var))->Type(); }
//	static inline tTJSVariantType GetVariantType(tTJSVariant &var)       { return var.Type(); }

	typedef tTJSNativeClassMethodCallback CallbackT;

	template <class T> 
	struct CallbackWithInstance {
		typedef tjs_error (TJS_INTF_METHOD    *Type)(tTJSVariant *result, tjs_int numparams, tTJSVariant **param, T *nativeInstance);
	};

	template <typename A, typename B> struct TypeEqual       { enum { NotEqual, Result = false }; };
	template <typename A>             struct TypeEqual<A, A> { enum {    Equal, Result = true  }; };
	template <bool F, int A,      int B>      struct IntSelect              { enum { Result = B }; };
	template <        int A,      int B>      struct IntSelect< true, A, B> { enum { Result = A }; };
	template <bool F, typename A, typename B> struct TypeSelect             { typedef B Result; };
	template <        typename A, typename B> struct TypeSelect<true, A, B> { typedef A Result; };
	template <bool F, typename ERR> struct TypeAssert            { typedef void Result; };
	template <        typename ERR> struct TypeAssert<true, ERR> { typedef typename ERR::CompileError Result; };
};

template <class T>
struct ncbSubClassCheck { enum { IsSubClass = false }; };

////////////////////////////////////////
template <class T>
struct ncbClassInfo {
	typedef T NativeClassT;

	typedef ncbTypedefs::NameT        NameT;
	typedef ncbTypedefs::IdentT       IdentT;
	typedef ncbTypedefs::ClassObjectT ClassObjectT;

	static inline NameT         GetName()        { return _info.name; }
	static inline IdentT        GetID()          { return _info.id; }
	static inline ClassObjectT *GetClassObject() { return _info.obj; }
	static inline bool          IsSubClass()     { return ncbSubClassCheck<NativeClassT>::IsSubClass; }

	static inline bool Set(NameT name, IdentT id, ClassObjectT *obj) {
		if (_info.initialized) return false;
		_info.name = name;
		_info.id   = id;
		_info.obj  = obj;
		return (_info.initialized = true);
	}
	static inline void Clear() {
		_info.name = 0;
		_info.id   = 0;
		_info.obj  = 0;
		_info.initialized = false;
	}
private:
	typedef struct info {
		info() : initialized(false), name(0), id(0), obj(0) {}

		bool initialized;
		NameT name;
		IdentT id;
		ClassObjectT *obj;
	} InfoT;
	static InfoT _info;
};
template <> struct ncbClassInfo<void> {};
template <class T> typename ncbClassInfo<T>::InfoT ncbClassInfo<T>::_info;


////////////////////////////////////////
template <class T>
struct ncbInstanceAdaptor : public tTJSNativeInstance {
	typedef T NativeClassT;
	typedef ncbInstanceAdaptor<NativeClassT> AdaptorT;
	typedef ncbClassInfo<NativeClassT>       ClassInfoT;

	/*constructor*/ ncbInstanceAdaptor() : _instance(0), _sticky(false) {}
	/*destructor*/ ~ncbInstanceAdaptor() { _deleteInstance(); }

	//tjs_error TJS_INTF_METHOD Construct(tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *tjs_obj);

	void TJS_INTF_METHOD Invalidate() { _deleteInstance(); }

private:
	NativeClassT *_instance;

	bool _sticky;

	void _deleteInstance() {
		if (_instance && !_sticky) delete _instance;
		_instance = 0;
		_sticky   = false;
	}

public:
	void setSticky() { _sticky = true; }

	//--------------------------------------

	static AdaptorT *GetAdaptor(iTJSDispatch2 *obj, bool err = false) {
		iTJSNativeInstance* adp = 0;
		if (!obj) {
			if (err) TVPThrowExceptionMessage(TJS_W("No instance."));
			return 0;
		}
		if (TJS_FAILED(obj->NativeInstanceSupport(TJS_NIS_GETINSTANCE, ClassInfoT::GetID(), &adp))) {
			if (err) TVPThrowExceptionMessage(TJS_W("Invalid instance type."));
			return 0;
		}
		return static_cast<AdaptorT*>(adp);
	}

	static NativeClassT *GetNativeInstance(iTJSDispatch2 *obj, bool err = false) {
		AdaptorT *adp = GetAdaptor(obj, err);
		return adp ? adp->_instance : 0;
	}

	static bool SetNativeInstance(iTJSDispatch2 *obj, NativeClassT *instance, bool err = false) {
		AdaptorT *adp = GetAdaptor(obj, err);
		if (!adp) return false;
		adp->_instance = instance;
		return true;
	}

	static bool SetAdaptorWithNativeInstance(iTJSDispatch2 *obj, NativeClassT *instance, bool err = false) {
		AdaptorT *adp = GetAdaptor(obj, false);
		if (adp) {
			if (adp->_instance) adp->_deleteInstance();
		} else if (!(adp = new AdaptorT())) {
			if (err) TVPThrowExceptionMessage(TJS_W("Create adaptor failed."));
			return false;
		}
		adp->_instance = instance;
		iTJSNativeInstance *ni = static_cast<iTJSNativeInstance*>(adp);
		if (TJS_FAILED(obj->NativeInstanceSupport(TJS_NIS_REGISTER, ClassInfoT::GetID(), &ni))) {
			if (err) TVPThrowExceptionMessage(TJS_W("Adaptor registration failed."));
			return false;
		}
		return true;
	}

	static iTJSDispatch2* CreateAdaptor(NativeClassT *inst, bool sticky = false, bool err = false) {
		typename ClassInfoT::ClassObjectT *clsobj = ClassInfoT::GetClassObject();
		if (!clsobj) {
			if (err) TVPThrowExceptionMessage(TJS_W("No class object."));
			return 0;
		}

		iTJSDispatch2 *global = TVPGetScriptDispatch(), *obj = 0;
		tTJSVariant dummy, *param = &dummy;
		tjs_error r = clsobj->CreateNew(0, 0, 0, &obj, 1, &param, global);
		if (global) global->Release();

		if (TJS_FAILED(r) || !obj) {
			if (err) TVPThrowExceptionMessage(TJS_W("Can't create instance"));
			return 0;
		}
		AdaptorT *adp = GetAdaptor(obj, err);
		if (adp) {
			adp->_instance = inst;
			if (sticky) adp->setSticky();
		}
		return obj;
	}

	static iTJSNativeInstance* TJS_INTF_METHOD CreateEmptyAdaptor() {
		return static_cast<iTJSNativeInstance*>(new AdaptorT());
	}
};

////////////////////////////////////////
struct ncbTypeConvertor {

	template <typename FROM, typename TO>
	struct Conversion {
	private:
		typedef char OK;
		typedef struct { char ng[2]; } NG;
		static OK check(TO);
		static NG check(...);
		static FROM wrap();
	public:
		enum {
			Exists = (sizeof(check(wrap())) == sizeof(OK)),
			Same   = false
		};
	};
	template <typename T> struct Conversion<T, T>       { enum { Exists = true,  Same = true  }; };
	template <typename T> struct Conversion<T, void>    { enum { Exists = false, Same = false }; };
	template <typename T> struct Conversion<void, T>    { enum { Exists = false, Same = false }; };
//	template <>           struct Conversion<void, void> { enum { Exists = false, Same = true  }; };
#define NCB_INNER_CONVERSION_SPECIALIZATION \
	template <> struct ncbTypeConvertor::Conversion<void, void> { enum { Exists = false, Same = true  }; };

	template <typename T> struct Stripper             { typedef T Type; };
	template <typename T> struct Stripper<T*>         { typedef typename Stripper<T>::Type Type; };
	template <typename T> struct Stripper<T&>         { typedef typename Stripper<T>::Type Type; };
	template <typename T> struct Stripper<const    T> { typedef typename Stripper<T>::Type Type; };
//	template <typename T> struct Stripper<volatile T> { typedef typename Stripper<T>::Type Type; };

	template <typename T> struct ToPointer            { static T* Get(T &t) { return &t; } };
	template <typename T> struct ToPointer<T&>        { static T* Get(T &t) { return &t; } };
	template <typename T> struct ToPointer<T*>        { static T* Get(T* t) { return  t; } };
	template <typename T> struct ToPointer<T const&>  { static T* Get(T const &t) { return const_cast<T*>(&t); } };
	template <typename T> struct ToPointer<T const*>  { static T* Get(T const *t) { return const_cast<T*>( t); } };

	template <typename T> struct ToTarget             { static T& Get(T *t) { return *t; } };
	template <typename T> struct ToTarget<T&>         { static T& Get(T *t) { return *t; } };
	template <typename T> struct ToTarget<T*>         { static T* Get(T *t) { return  t; } };

	template <typename T> struct NonConst             { typedef T  Type; };
	template <typename T> struct NonConst<const T>    { typedef T  Type; };
	template <typename T> struct NonConst<const T&>   { typedef T& Type; };
	template <typename T> struct NonConst<const T*>   { typedef T* Type; };

	template <typename T> struct NonReference         { typedef T Type; };
	template <typename T> struct NonReference<T&>     { typedef T Type; };

	struct DirectCopy {
		template <typename DST, typename SRC>
		inline void operator()(DST &dst, SRC const &src) const { dst = src; }
	};

	template <typename CAST>
	struct CastCopy {
		template <typename DST, typename SRC>
		inline void operator()(DST &dst, SRC const &src) const { dst = static_cast<DST>(static_cast<CAST>(src)); }
	};

	template <typename T, bool SRCF>
	struct SpecialMap {
		enum { Exists = false, Modifier = false, IsSource = SRCF };
		typedef T Type;
	};

	struct NCB_COMPILE_ERROR_NoImplement;

	struct SelectConvertorTypeBase {
	protected:
		template <bool EXP, class THEN, class ELSE> struct ifelse                   { typedef ELSE Type; };
		template <          class THEN, class ELSE> struct ifelse<true, THEN, ELSE> { typedef THEN Type; };

		template <typename T, bool IsSrcF>
		struct hasSpecial {
			typedef typename Stripper<T>::Type StripT;
			typedef SpecialMap<T,      IsSrcF> ThisMapT;
			typedef SpecialMap<StripT, IsSrcF> StripMapT;
			enum {
				exthis  = ThisMapT::Exists,
				exstrip = StripMapT::Exists && StripMapT::Modifier,
				Exists = exthis | exstrip,
			};
			typedef typename ifelse<exthis,  typename ThisMapT::Type,
			/*    */typename ifelse<exstrip, typename StripMapT::Type, void>::Type
				/*                */>::Type Type;
		};
		template <typename T> struct wrap { typedef T Type; };

		template <typename SRC, typename DST>
		struct directSelect {
			typedef typename ifelse<Conversion<SRC, DST>::Exists, DirectCopy, NCB_COMPILE_ERROR_NoImplement>::Type Type;
		};
	};

	template <typename SRC, typename DST>
	struct SelectConvertorType : public SelectConvertorTypeBase {
	private:
		typedef hasSpecial<SRC, true > SrcSpecialT;
		typedef hasSpecial<DST, false> DstSpecialT;

		struct specialSelect {
			typedef typename ifelse<DstSpecialT::Exists, typename DstSpecialT::Type, typename SrcSpecialT::Type>::Type Type;
		};
		typedef typename ifelse<
			SrcSpecialT::Exists || DstSpecialT::Exists, specialSelect, directSelect<SRC, DST>
				>::Type select;
	public:
		typedef typename select::Type Type;
	};
};
// ncbTypeConvertor::Conversion の特殊化
       NCB_INNER_CONVERSION_SPECIALIZATION
#undef NCB_INNER_CONVERSION_SPECIALIZATION

//--------------------------------------
struct ncbStrictResultConvertor {};

//--------------------------------------

#define NCB_TYPECONV_MAPSET(mapsel, type, conv, mod) \
	template <> struct ncbTypeConvertor::SpecialMap<type, mapsel> { \
		enum { Exists = true, Modifier = mod, IsSource = mapsel }; \
		typedef conv Type; \
	}
#define NCB_TYPECONV_SRCMAP_SET(type, conv, mod) NCB_TYPECONV_MAPSET(true,  type, conv, mod)
#define NCB_TYPECONV_DSTMAP_SET(type, conv, mod) NCB_TYPECONV_MAPSET(false, type, conv, mod)

#define NCB_TYPECONV_DIRECT(type) \
	NCB_TYPECONV_SRCMAP_SET(type, ncbTypeConvertor::DirectCopy, false); \
	NCB_TYPECONV_DSTMAP_SET(type, ncbTypeConvertor::DirectCopy, false)

#define NCB_TYPECONV_CAST(type, cast) \
	NCB_TYPECONV_SRCMAP_SET(type, ncbTypeConvertor::CastCopy<cast>, false); \
	NCB_TYPECONV_DSTMAP_SET(type, ncbTypeConvertor::CastCopy<cast>, false)

#define NCB_TYPECONV_CAST_INTEGER(type)    NCB_TYPECONV_CAST(type, tTVInteger)
#define NCB_TYPECONV_CAST_REAL(type)       NCB_TYPECONV_CAST(type, tTVReal)

NCB_TYPECONV_CAST_INTEGER(   signed char);
NCB_TYPECONV_CAST_INTEGER( unsigned char);
NCB_TYPECONV_CAST_INTEGER(signed int);
NCB_TYPECONV_CAST_INTEGER(unsigned int);
NCB_TYPECONV_CAST_INTEGER(  signed short);
NCB_TYPECONV_CAST_INTEGER(unsigned short);
NCB_TYPECONV_CAST_INTEGER(signed long);
NCB_TYPECONV_CAST_INTEGER(unsigned long);
NCB_TYPECONV_CAST_REAL(            float);
NCB_TYPECONV_CAST_REAL(           double);
NCB_TYPECONV_CAST(            bool, bool);

struct ncbNarrowCharConvertor {
	struct ToNChar {
		ToNChar() : _nstr(0) {}
		~ToNChar() {
			if (_nstr) {
				//				NCB_LOG_W("~ncbVariatToNChar > delete[]");
				delete[] _nstr;
			}
		}
		template <typename DST>
		inline void operator()(DST &dst, tTJSVariant const &src) {
			if (ncbTypedefs::GetVariantType(src) == tvtString) {
				tTJSString s(src.AsStringNoAddRef());
				tjs_int len = s.GetNarrowStrLen();

//				NCB_LOG_W("ncbVariatToNChar::operator() > new tjs_nchar[]");
				_nstr = new tjs_nchar[len+1];
				s.ToNarrowStr(_nstr, len+1);
			}
			dst = static_cast<DST>(_nstr);
		}
	private:
		tjs_nchar *_nstr;
	};
	struct ToVariant {
		template <typename SRC>
		inline void operator()(tTJSVariant &dst, SRC const &src) const {
//			NCB_LOG_2("ncbNCharToVariatTo::operator() : ", src);
			dst = tTJSString(src);
		}
	};
};
#define NCB_TYPECONV_NARROW_STRING(type) \
	NCB_TYPECONV_SRCMAP_SET(type, ncbNarrowCharConvertor::ToVariant, false); \
	NCB_TYPECONV_DSTMAP_SET(type, ncbNarrowCharConvertor::ToNChar,   false)

NCB_TYPECONV_NARROW_STRING(         char const*);
NCB_TYPECONV_NARROW_STRING(  signed char const*);
NCB_TYPECONV_NARROW_STRING(unsigned char const*);

struct ncbWideCharConvertor {
	struct ToWChar {
		template <typename DST>
		inline void operator()(DST &dst, tTJSVariant const &src) {
			dst = static_cast<DST>(src.GetString());
		}
	};
};
#define NCB_TYPECONV_WIDE_STRING(type) \
	NCB_TYPECONV_SRCMAP_SET(type, ncbTypeConvertor::CastCopy<tjs_char const*>, false); \
	NCB_TYPECONV_DSTMAP_SET(type, ncbWideCharConvertor::ToWChar,               false)

NCB_TYPECONV_WIDE_STRING(tjs_char const*);


template <class StringT>
struct ncbStringConvertor {
	typedef ncbTypedefs      DefsT;
	typedef ncbTypeConvertor ConvT;
	typedef DefsT::NumTag<sizeof(typename StringT::value_type)> SizeTagT;

	template <class STR>
	inline void operator()(tTJSVariant &dst, STR const &src) const {
		dst = (ConvT::ToPointer<STR const&>::Get(src))->c_str();
	}
	template <class STR>
	inline void operator()(STR &dst, tTJSVariant const &src) {
		if (ncbTypedefs::GetVariantType(src) == tvtString) {
			tTJSString str(src.AsStringNoAddRef());
			set(str, SizeTagT());
		}
		dst = ConvT::ToTarget<STR>::Get(&_temp);
	}
	inline void set(tTJSString const &str, DefsT::NumTag<sizeof(tjs_nchar)>) { // for Narrow char
		tjs_int len = str.GetNarrowStrLen();
		tjs_nchar tmp[len+1];
		str.ToNarrowStr(tmp, len+1);
		_temp.assign(tmp, len);
	}
	inline void set(tTJSString const &str, DefsT::NumTag<sizeof(tjs_char)>) { // for Wide char
		_temp = str.c_str();
	}
private:
	StringT _temp;
};
#define NCB_TYPECONV_STL_STRING(type) \
	NCB_TYPECONV_SRCMAP_SET(type, ncbStringConvertor<type>, true); \
	NCB_TYPECONV_DSTMAP_SET(type, ncbStringConvertor<type>, true)


struct ncbNativeObjectBoxing {
	typedef tTJSVariant VarT;
	typedef ncbTypeConvertor ConvT;
	struct Boxing : public ncbStrictResultConvertor {
		template <typename T> struct box            { static T* Get(T const &t) { return           new T(t); } enum { Sticky = false }; };
		template <typename T> struct box<T&>        { static T* Get(T       &t) { return                &t ; } enum { Sticky = true  }; };
		template <typename T> struct box<T const&>  { static T* Get(T const &t) { return const_cast<T*>(&t); } enum { Sticky = true  }; };
		template <typename T> struct box<T*>        { static T* Get(T       *t) { return                 t ; } enum { Sticky = false }; };
		template <typename T> struct box<T const*>  { static T* Get(T const *t) { return const_cast<T*>( t); } enum { Sticky = false }; };

		template <typename SRC>
		inline void operator ()(VarT &dst, SRC src, ncbTypedefs::Tag<SRC> const&) const {
			typedef SRC                                     TargetT;
			typedef typename ConvT::Stripper<TargetT>::Type ClassT;
			typedef ncbInstanceAdaptor<ClassT>              AdaptorT;

			ClassT *p = box<TargetT>::Get(src);						
			bool const s = box<TargetT>::Sticky;					
			iTJSDispatch2 *adpobj = AdaptorT::CreateAdaptor(p, s);	
			dst = tTJSVariant(adpobj, adpobj);						
			adpobj->Release();										
		}

		template <typename SRC>
		inline void operator ()(VarT &dst, SRC &src, ncbTypedefs::Tag<SRC&> const &tag) const {
			operator()<SRC&> (dst, src, tag);
		}
	};

	struct Unboxing {
		template <typename DST>
		inline void operator ()(DST &dst, VarT const &src) const {
			typedef DST                                     TargetT;
			typedef typename ConvT::Stripper<TargetT>::Type ClassT;
			typedef ncbInstanceAdaptor<ClassT>              AdaptorT;

			iTJSDispatch2 *obj = src.AsObjectNoAddRef();			
			ClassT *p = AdaptorT::GetNativeInstance(obj, true);		
			dst = ConvT::ToTarget<TargetT>::Get(p);					
		}
	};
};

#define NCB_TYPECONV_BOXING(type) \
	NCB_TYPECONV_SRCMAP_SET(type, ncbNativeObjectBoxing::Boxing,   true); \
	NCB_TYPECONV_DSTMAP_SET(type, ncbNativeObjectBoxing::Unboxing, true)



#define NCB_SET_TOVARIANT_CONVERTOR(type, convertor) \
	template <> struct ncbTypeConvertor::SelectConvertorType<type, tTJSVariant> { typedef convertor Type; }

#define NCB_SET_TOVALUE_CONVERTOR(type, convertor) \
	template <> struct ncbTypeConvertor::SelectConvertorType<tTJSVariant, type> { typedef convertor Type; }

#define NCB_SET_CONVERTOR(type, convertor) \
	NCB_SET_TOVARIANT_CONVERTOR(type, convertor); \
	NCB_SET_TOVALUE_CONVERTOR(  type, convertor) \

NCB_SET_TOVARIANT_CONVERTOR(void, struct {});

struct ncbDispatchConvertor {
	inline void operator ()(tTJSVariant &dst, iTJSDispatch2* &src) const {
		dst = tTJSVariant(src, src);
		src->Release();
	}
	inline void operator ()(iTJSDispatch2* &dst, tTJSVariant const &src) const {
		dst = src.AsObjectNoAddRef();
	}
	inline void operator ()(iTJSDispatch2 const* &dst, tTJSVariant const &src) const {
		dst = src.AsObjectNoAddRef();
	}
};
NCB_SET_TOVARIANT_CONVERTOR(iTJSDispatch2*,       ncbDispatchConvertor);
NCB_SET_TOVALUE_CONVERTOR(  iTJSDispatch2*,       ncbDispatchConvertor);
NCB_SET_TOVALUE_CONVERTOR(  iTJSDispatch2 const*, ncbDispatchConvertor);



struct ncbPropAccessor {
	typedef ncbTypedefs   DefsT;
	typedef DefsT::NameT  NameT;
	typedef DefsT::FlagsT FlagsT;
	typedef DefsT::NameT  KeyT;
	typedef tjs_int32     IndexT;
	typedef tjs_int       CountT;
	typedef tjs_uint32    SizeT;
	typedef tjs_error     ErrorT;
	typedef tjs_uint32*   HintT;
	typedef tTJSVariant   VariantT;

	ncbPropAccessor(iTJSDispatch2 *obj, bool addref = true) : _obj(obj) {
		if (addref) _obj->AddRef();
	}
	ncbPropAccessor(ncbPropAccessor const &ref) : _obj(ref._obj) { _obj->AddRef(); }
	ncbPropAccessor(tTJSVariant var) : _obj(var.AsObject()) {
		//_obj->AddRef();
	}
    ncbPropAccessor(NameT name) {
	    tTJSVariant val;
	    iTJSDispatch2 *global = TVPGetScriptDispatch();
	    if (global) {
	        global->PropGet(0, name, 0, &val, global);
		}
	    _obj = val.AsObject();
	}
	virtual ~ncbPropAccessor() {
	    if (_obj) {
		    _obj->Release();
		}
	}
	CountT GetCount() const {
		CountT sz;
		ErrorT r = _obj->GetCount(&sz, 0, 0, _obj);
		return (r == TJS_S_OK) ? sz : 0;
	}
	CountT GetArrayCount() const {
		VariantT var;
		_obj->PropGet(0, TJS_W("count"), 0, &var, _obj);
		return (CountT)var;
	}
	template <typename TargetT>
	TargetT GetValue(IndexT ofs, DefsT::Tag<TargetT> const &tag, FlagsT f = 0) {
		VariantT var;
		_obj->PropGetByNum(f, ofs, &var, _obj);
		return _toTarget(var, tag);
	}
	template <typename TargetT>
	TargetT GetValue(iTJSDispatch2 *obj, IndexT ofs, DefsT::Tag<TargetT> const &tag, FlagsT f = 0) {
		VariantT var;
		_obj->PropGetByNum(f, ofs, &var, obj);
		return _toTarget(var, tag);
	}
	tjs_int getIntValue(IndexT ofs, tjs_int defaultValue=0) {
		if (HasValue(ofs)) {
			return GetValue(ofs, DefsT::Tag<tjs_int>());
		} else {
			return defaultValue;
		}
	}
	tjs_real getRealValue(IndexT ofs, tjs_real defaultValue=0) {
		if (HasValue(ofs)) {
			return GetValue(ofs, DefsT::Tag<tjs_real>());
		} else {
			return defaultValue;
		}
	}
	ttstr getStrValue(IndexT ofs, ttstr const &defaultValue=ttstr("")) {
		if (HasValue(ofs)) {
			return GetValue(ofs, DefsT::Tag<ttstr>());
		} else {
			return defaultValue;
		}
	}
	template <typename TargetT>
	TargetT GetValue(KeyT key, DefsT::Tag<TargetT> const &tag, FlagsT f = 0, HintT hint = 0) {
		VariantT var;
		_obj->PropGet(f, key, hint, &var, _obj);
		return _toTarget(var, tag);
	}
	template <typename TargetT>
	TargetT GetValue(iTJSDispatch2* obj, KeyT key, DefsT::Tag<TargetT> const &tag, FlagsT f = 0, HintT hint = 0) {
		VariantT var;
		_obj->PropGet(f, key, hint, &var, obj);
		return _toTarget(var, tag);
	}
	tjs_int getIntValue(KeyT key, tjs_int defaultValue=0) {
		if (HasValue(key)) {
			return GetValue(key, DefsT::Tag<tjs_int>());
		} else {
			return defaultValue;
		}
	}
	tjs_real getRealValue(KeyT key, tjs_real defaultValue=0) {
		if (HasValue(key)) {
			return GetValue(key, DefsT::Tag<tjs_real>());
		} else {
			return defaultValue;
		}
	}
	ttstr getStrValue(KeyT key, ttstr const &defaultValue=ttstr("")) {
		if (HasValue(key)) {
			return GetValue(key, DefsT::Tag<ttstr>());
		} else {
			return defaultValue;
		}
	}
	bool checkVariant(IndexT ofs, VariantT &var) {
		return TJS_SUCCEEDED(_obj->PropGetByNum(TJS_MEMBERMUSTEXIST, ofs, &var, _obj));
	}
	bool checkVariant(KeyT key, VariantT &var) {
		return TJS_SUCCEEDED(_obj->PropGet(TJS_MEMBERMUSTEXIST, key, 0, &var, _obj));
	}
	bool HasValue(IndexT ofs, tTJSVariantType *type = 0) {
		VariantT var;
		bool ret = TJS_SUCCEEDED(_obj->PropGetByNum(TJS_MEMBERMUSTEXIST, ofs, &var, _obj));
		if (ret && type) *type = var.Type();
		return ret;
	}
	bool HasValue(iTJSDispatch2 *obj, IndexT ofs, tTJSVariantType *type = 0) {
		VariantT var;
		bool ret = TJS_SUCCEEDED(_obj->PropGetByNum(TJS_MEMBERMUSTEXIST, ofs, &var, obj));
		if (ret && type) *type = var.Type();
		return ret;
	}
	bool HasValue(KeyT key, HintT hint = 0, tTJSVariantType *type = 0) {
		VariantT var;
		bool ret = TJS_SUCCEEDED(_obj->PropGet(TJS_MEMBERMUSTEXIST, key, hint, &var, _obj));
		if (ret && type) *type = var.Type();
		return ret;
	}
	bool HasValue(iTJSDispatch2 *obj, KeyT key, HintT hint = 0, tTJSVariantType *type = 0) {
		VariantT var;
		bool ret = TJS_SUCCEEDED(_obj->PropGet(TJS_MEMBERMUSTEXIST, key, hint, &var, obj));
		if (ret && type) *type = var.Type();
		return ret;
	}
	template <typename TargetT>
	bool SetValue(IndexT ofs, TargetT const &val, FlagsT f = TJS_MEMBERENSURE) {
		VariantT var;
		_toVariant(var, val);
		return (_obj->PropSetByNum(f, ofs, &var, _obj) == TJS_S_OK);
	}
	template <typename TargetT>
	bool SetValue(iTJSDispatch2 *obj, IndexT ofs, TargetT const &val, FlagsT f = TJS_MEMBERENSURE) {
		VariantT var;
		_toVariant(var, val);
		return (_obj->PropSetByNum(f, ofs, &var, obj) == TJS_S_OK);
	}
	template <typename TargetT>
	bool SetValue(KeyT key, TargetT const &val, FlagsT f = TJS_MEMBERENSURE, HintT hint = 0) {
		VariantT var;
		_toVariant(var, val);
		return (_obj->PropSet(f, key, hint, &var, _obj) == TJS_S_OK);
	}
	template <typename TargetT>
	bool SetValue(iTJSDispatch2 *obj, KeyT key, TargetT const &val, FlagsT f = TJS_MEMBERENSURE, HintT hint = 0) {
		VariantT var;
		_toVariant(var, val);
		return (_obj->PropSet(f, key, hint, &var, obj) == TJS_S_OK);
	}
	bool IsValid() const { return _obj != 0; }
	iTJSDispatch2* GetDispatch() const { return _obj; }
	operator iTJSDispatch2*   () const { return _obj; }

#undef  FOREACH_START
#define FOREACH_START 1
#undef  FOREACH_END
#define FOREACH_END   FOREACH_MAX
#define FCALL_PRM_EXT(num) tTJSVariant          param ## num
#define FCALL_SET_EXT(num) params[ num - 1 ] = &param ## num;
#undef  FOREACH
#define FOREACH \
	tjs_error TJS_INTF_METHOD FuncCall(tjs_uint32 flag, const tjs_char *membername, tjs_uint32 *hint, tTJSVariant *result, FOREACH_COMMA_EXT(FCALL_PRM_EXT) ) { \
		tTJSVariant *params[FOREACH_COUNT]; FOREACH_SPACE_EXT(FCALL_SET_EXT) \
		return _obj->FuncCall(flag, membername, hint, result, FOREACH_COUNT, params, _obj); \
	}
#include FOREACH_INCLUDE
#undef  FOREACH
#define FOREACH \
	tjs_error TJS_INTF_METHOD FuncCall(iTJSDispatch2 *obj, tjs_uint32 flag, const tjs_char *membername, tjs_uint32 *hint, tTJSVariant *result, FOREACH_COMMA_EXT(FCALL_PRM_EXT) ) { \
		tTJSVariant *params[FOREACH_COUNT]; FOREACH_SPACE_EXT(FCALL_SET_EXT) \
		return _obj->FuncCall(flag, membername, hint, result, FOREACH_COUNT, params, obj); \
	}
#include FOREACH_INCLUDE
#undef  FCALL_PRM_EXT
#undef  FCALL_SET_EXT
	tjs_error TJS_INTF_METHOD FuncCall(tjs_uint32 flag, const tjs_char *membername, tjs_uint32 *hint, tTJSVariant *result) {
		return _obj->FuncCall(flag, membername, hint, result, 0, NULL, _obj);
	}
	tjs_error TJS_INTF_METHOD FuncCall(iTJSDispatch2 *obj, tjs_uint32 flag, const tjs_char *membername, tjs_uint32 *hint, tTJSVariant *result) {
		return _obj->FuncCall(flag, membername, hint, result, 0, NULL, obj);
	}
protected:
	iTJSDispatch2 *_obj;

	template <typename TargetT>
	TargetT _toTarget(VariantT &v, DefsT::Tag<TargetT> const&) {
		typedef typename ncbTypeConvertor::SelectConvertorType<VariantT, TargetT>::Type ToTargetT;
		TargetT r;
		ToTargetT conv;
		conv(r, v);
		return r;
	}
	template <typename TargetT>
	void _toVariant(VariantT &v, TargetT const &r) {
		typedef typename ncbTypeConvertor::SelectConvertorType<TargetT, VariantT>::Type ToVariantT;
		ToVariantT conv;
		conv(v, r);
	}
	VariantT _toTarget( VariantT &v, DefsT::Tag<VariantT> const&) { return v; }
	void     _toVariant(VariantT &v, VariantT const &r) { v = r; }
	iTJSDispatch2* _toTarget( VariantT &v, DefsT::Tag<iTJSDispatch2*> const&) { return v; }
	void           _toVariant(VariantT &v, iTJSDispatch2*  const &r) { v = VariantT(r, r); }
	void           _toVariant(VariantT &v, ncbPropAccessor  const &r) { _toVariant(v, r._obj);  }
	void           _toVariant(VariantT &v, ncbPropAccessor* const &r) { _toVariant(v, r->_obj); }
};

struct ncbArrayAccessor : public ncbPropAccessor {
	ncbArrayAccessor() : ncbPropAccessor(TJSCreateArrayObject(), false) {}
private:
	ncbArrayAccessor(ncbArrayAccessor const&);
};
struct ncbDictionaryAccessor : public ncbPropAccessor {
	ncbDictionaryAccessor() : ncbPropAccessor(TJSCreateDictionaryObject(), false) {}
private:
	ncbDictionaryAccessor(ncbDictionaryAccessor const&);
};


////////////////////////////////////////
struct ncbIMethodObject {
	typedef iTJSDispatch2*             DispatchT;
	typedef ncbTypedefs::FlagsT        FlagsT;
	typedef ncbTypedefs::InstanceTypeT TypesT;

	virtual DispatchT GetDispatch() const = 0;
	virtual FlagsT    GetFlags()    const = 0;
	virtual TypesT    GetType()     const = 0;
	virtual void      Release()     const = 0;
};

////////////////////////////////////////

struct ncbNativeClassMethodBase : public tTJSDispatch {
	typedef tTJSDispatch             BaseT;
	typedef ncbNativeClassMethodBase ThisClassT;
	typedef ncbTypedefs              DefsT;
	typedef DefsT::NameT             NameT;
	typedef DefsT::FlagsT            FlagsT;
	typedef DefsT::InstanceTypeT     TypesT;
	typedef ncbIMethodObject const*  iMethodT;
	typedef tTJSNativeInstanceType   MethodTypeT;

	ncbNativeClassMethodBase(MethodTypeT t) : _type(t), _name(0) {
		_imethod = this;
		switch (t) { 
		case nitMethod:   _name = TJS_W("Function"); break;
		case nitProperty: _name = TJS_W("Property"); break;
		default: break;
		}
	}
	~ncbNativeClassMethodBase() {}

	tjs_error TJS_INTF_METHOD IsInstanceOf(
		tjs_uint32 flag, const tjs_char *membername, tjs_uint32 *hint, 
		const tjs_char *classname, iTJSDispatch2 *objthis)
	{
		return (!membername && _name && !TJS_stricmp(classname, _name)) ? TJS_S_TRUE
			: BaseT::IsInstanceOf(flag, membername, hint, classname, objthis);
	}

private:
	MethodTypeT const _type;
	NameT _name;

	typedef DefsT::CallerT CallerT;

	//--------------------------------------
	virtual TypesT GetType() const { return _type; }

	virtual FlagsT GetFlags() const { return 0; }

	struct iMethod : public ncbIMethodObject {
		typedef ncbNativeClassMethodBase MethodObjectT;
		void operator = (MethodObjectT *mo) { _this = mo; }
		DispatchT GetDispatch() const { return static_cast<DispatchT>(_this); }
		FlagsT    GetFlags()    const { return _this->GetFlags(); }
		TypesT    GetType()     const { return _this->GetType(); }
		void      Release()     const {}
	private:
		MethodObjectT *_this;
	} _imethod;

protected:
	iMethodT GetIMethod() const { return &_imethod; }



	//--------------------------------------
protected:
	template <typename T>
	struct traits {
		typedef CallerT::tMethodTraits<T> TraitsT;
		typedef typename TraitsT::ResultType ResultT;
		typedef typename TraitsT::ClassType ClassT;
		typedef typename TraitsT::ArgsType  ArgsT;
		enum { ArgsCount = TraitsT::ArgsCount };
	};

	//--------------------------------------
private:
	template <typename METHOD>
	struct paramsFunctor {
		typedef traits<METHOD> TraitsT;
		typedef typename TraitsT::ResultT RES;
		typedef typename TraitsT::ArgsT   ARGS;
		enum { ARGC = TraitsT::ArgsCount };

		typedef tTJSVariant      VariantT;
		typedef ncbTypeConvertor ConvT;
		typedef typename     ConvT::SelectConvertorType<RES, VariantT>::Type ResultConvT;
		enum { StrictResult = ncbTypeConvertor::Conversion<ResultConvT*, ncbStrictResultConvertor*>::Exists };

		template <typename T> struct TypeWrap           { typedef T        Type; static inline T        Restore(Type t) { return  t; } };
		template <typename T> struct TypeWrap<T&>       { typedef T*       Type; static inline T&       Restore(Type t) { return *t; } };
		template <typename T> struct TypeWrap<T const&> { typedef T const* Type; static inline T const& Restore(Type t) { return *t; } };

		template <typename T> struct ArgsExtor { typedef typename ConvT::SelectConvertorType<VariantT, typename TypeWrap<T>::Type>::Type Type; };
		typedef CallerT::tArgsExtract<ArgsExtor, ARGS, ARGC> ArgsConvT;

		paramsFunctor(VariantT *r, tjs_int n, VariantT const *const *p) : _numparams(n), _result(r),_param(p) {}

		template <int N, typename T>
		inline T operator ()(CallerT::tNumTag<N> const& /*index*/, CallerT::tTypeTag<T> const& /*type*/) {
			typedef typename TypeWrap<T>::Type ParamT;
			ParamT ret;
			(CallerT::tArgsSelect<ArgsConvT, N>::Get(_aconv))(ret, (_numparams >= N) ? *(_param[N - 1]) : VariantT());
			return TypeWrap<T>::Restore(ret);
		}

		inline bool operator ()() const { return true; }

		template <typename ResultT>
		inline bool operator()(ResultT r, CallerT::tTypeTag<ResultT> const&) {
			return SetResult(r, DefsT::Tag<ResultT>(),  DefsT::BoolTag<StrictResult>());
		}

		template <typename ResultT>
		inline bool SetResult(ResultT r, DefsT::Tag<ResultT> const&, DefsT::BoolTag<false> const&) {
			if (_result) _rconv(*_result, r); // ncbToVariantConvertor で返り値に変換
			return true;
		}

		template <typename ResultT>
		inline bool SetResult(ResultT r, DefsT::Tag<ResultT> const &tag, DefsT::BoolTag<true> const&) {
			if (_result) _rconv(*_result, r, tag);
			return true;
		}

		template <typename ResultT>
		inline bool operator()(ResultT &r, CallerT::tTypeTag<ResultT&> const& tag) {
			return  operator()<ResultT &>(r, tag);
		}
		template <typename ResultT, bool B>
		inline bool SetResult(ResultT &r, DefsT::Tag<ResultT&> const &tag, DefsT::BoolTag<B> const &sel) {
			return  SetResult<ResultT &>(r, tag, sel);
		}


	private:
		ArgsConvT   _aconv;
		ResultConvT _rconv;

		tjs_int                _numparams;
		VariantT             * _result;
		VariantT const *const* _param;
	};

	template <class CLASS, class FUNCT>
	struct paramsFunctorWithInstance : public FUNCT {
		typedef FUNCT       BaseT;
		typedef CLASS       ClassT;
		typedef tTJSVariant VariantT;
		paramsFunctorWithInstance(VariantT *r, tjs_int n, VariantT const *const *p, ClassT *inst) : BaseT(r, n+1, p-1), _inst(inst) {}
		template <int N, typename T> inline T operator ()(CallerT::tNumTag<N> const &idx, CallerT::tTypeTag<T> const &tag) { return BaseT::operator ()(idx, tag); }
		template <       typename T> inline T operator ()(CallerT::tNumTag<1> const &,    CallerT::tTypeTag<T> const &   ) { return _inst; }
		template <typename ResultT>  inline bool operator()(ResultT  r, CallerT::tTypeTag<ResultT>  const &tag) { return BaseT::operator()(r, tag); }
		template <typename ResultT>  inline bool operator()(ResultT &r, CallerT::tTypeTag<ResultT&> const &tag) { return BaseT::operator()(r, tag); }
		/*                         */inline bool operator()() const { return BaseT::operator ()(); }
	private:
		ClassT *_inst;
	};


	//--------------------------------------
private:
	template <class T>
	struct nativeInstanceGetterBase {
		typedef T  ClassT;
		typedef ncbInstanceAdaptor<ClassT> AdaptorT;
		typedef iTJSDispatch2* DispatchT;
		typedef tjs_error      ErrorT;

		nativeInstanceGetterBase() : _error(TJS_S_OK) {}

		inline ClassT *GetNativeInstance(DispatchT objthis) {
			ClassT *r = AdaptorT::GetNativeInstance(objthis); // 実インスタンスのポインタ
			if (!r) SetError(TJS_E_NATIVECLASSCRASH);
			return r;
		}
		inline bool SetNativeInstance(DispatchT objthis, ClassT *obj) {
			SetError(TJS_S_OK);
			return AdaptorT::SetAdaptorWithNativeInstance(objthis, obj);
		}
		inline ErrorT  GetError() const   { return _error; }
		inline void    SetError(ErrorT e) { _error = e; }

		inline ClassT* Get(DispatchT objthis) { return GetNativeInstance(objthis); }
	private:
		ErrorT _error;
	};

	template <class T>
	struct nativeInstanceGetter : public nativeInstanceGetterBase<T> {};

	template <class FROM, class TO, typename CONV>
	struct bridgeInstanceGetter : public nativeInstanceGetterBase<FROM> {
		typedef                          nativeInstanceGetterBase<FROM> BaseT;
		inline TO* Get(typename BaseT::DispatchT objthis) {
			FROM *obj = BaseT::GetNativeInstance(objthis);
			return obj ? conv(obj) : 0;
		}
	private:
		CONV conv;
	};

	typedef struct dummyGetter { dummyGetter() {} } const noInstanceGetter;

	//--------------------------------------
protected:
	template <bool  IsAny>  struct invokeHookAll   { template <typename T> static inline typename T::ResultT Do(T &t) { return t(); } };
	template <class ClassT> struct invokeHookClass { template <typename T> static inline typename T::ResultT Do(T &t) { return invokeHookAll<T::Hook>::Do(t); } };

	//--------------------------------------
protected:
	struct doInvokeBase {
		typedef tjs_error ResultT;
		enum { Hook = true };
		enum { ivsMethod, ivsProxy, ivsConstructor, ivsFactory };

		typedef tTJSVariant  * RetT;
		typedef tjs_int        NumT;
		typedef tTJSVariant ** ArgsT;
		typedef iTJSDispatch2            *ObjT;
		doInvokeBase(RetT r, NumT n, ArgsT p, ObjT o)
			: _result(r), _numparams(n), _param(p), _objthis(o) { if (r) r->Clear(); }
	protected:
		RetT  const _result;
		NumT  const _numparams;
		ArgsT const _param;
		ObjT  const _objthis;

		template <typename MethodT, class ClassT, class FunctorT>
		ResultT CallInvoke(MethodT const &m, ClassT *inst, DefsT::Tag<FunctorT>, DefsT::NumTag<ivsMethod>) const {
			return CallerT::Invoke(FunctorT(_result, _numparams, _param),       m, inst) ? TJS_S_OK : TJS_E_FAIL;
		}
		template <typename MethodT, class ClassT, class FunctorT>
		ResultT CallInvoke(MethodT const &m, ClassT *inst, DefsT::Tag<FunctorT>, DefsT::NumTag<ivsProxy>) const {
			return CallerT::Invoke(FunctorT(_result, _numparams, _param, inst), m, inst) ? TJS_S_OK : TJS_E_FAIL;
		}
		template <typename MethodT, class ClassT, class FunctorT>
		ResultT CallInvoke(MethodT const &,  ClassT *inst, DefsT::Tag<FunctorT>, DefsT::NumTag<ivsConstructor>) const {
			try {
				FunctorT fnct(_result, _numparams, _param);
				if (!(inst = CallerT::Factory(fnct, CallerT::tTypeTag<ClassT>(), CallerT::tTypeTag<typename FunctorT::TraitsT::ArgsT>()))) {
					TVPThrowExceptionMessage(TJS_W("NativeClassInstance creation faild."));
					return TJS_E_FAIL;
				}
				if (!ncbInstanceAdaptor<ClassT>::SetNativeInstance(_objthis, inst)) {
					delete inst;
					return TJS_E_NATIVECLASSCRASH;
				}
			} catch (...) {
				if (inst) delete inst;
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
		template <typename MethodT, class ClassT, class FunctorT>
		ResultT CallInvoke(MethodT const &m, ClassT *inst, DefsT::Tag<FunctorT>, DefsT::NumTag<ivsFactory>) const {
			typedef ncbInstanceAdaptor<ClassT> AdaptorT;
			try {
				if (!(inst = CallerT::Factory(FunctorT(_result, _numparams, _param, _objthis), m))) {
					TVPThrowExceptionMessage(TJS_W("NativeClassInstance creation faild."));
					return TJS_E_FAIL;
				}
				if (!ncbInstanceAdaptor<ClassT>::SetNativeInstance(_objthis, inst)) {
					delete inst;
					return TJS_E_NATIVECLASSCRASH;
				}
			} catch (...) {
				if (inst) delete inst;
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

		template <class ClassT, typename GetterT>
		ResultT GetInstance(ClassT **obj, GetterT &g) const {
			*obj = g.Get(_objthis);
			return g.GetError();
		}
		template <class ANY>
		ResultT GetInstance(ANY**, noInstanceGetter&) const { return TJS_S_OK; }
	};
	template <class SELECTOR>
	struct doInvoke : public doInvokeBase {
		typedef SELECTOR SelectorT;
		typedef typename SelectorT::RefClassT    RefClassT;    
		typedef typename SelectorT::ClassT       ClassT;       
		typedef typename SelectorT::MethodT      MethodT;      
		typedef typename SelectorT::GetInstanceT GetInstanceT; 
		typedef typename SelectorT::FunctorT     FunctorT;     
		//enum { InvokeSelect = SelectorT::InvokeSelect };     
		//enum { ArgsCount = SelectorT::ArgsCount };           

		doInvoke(MethodT const &m, RetT r, NumT n, ArgsT p, ObjT o) : doInvokeBase(r, n, p, o), _m(m) {}
		inline ResultT operator()() const {
			if (_numparams < SelectorT::ArgsCount) return TJS_E_BADPARAMCOUNT;

			ClassT *inst = 0;
			GetInstanceT gi;
			ResultT r = GetInstance(&inst, gi);
			if (TJS_FAILED(r)) return r;

			return CallInvoke(_m, inst, DefsT::Tag<FunctorT>(), DefsT::NumTag<SelectorT::InvokeSelect>());
		}
		inline ResultT Invoke() const {
			return invokeHookClass<RefClassT>::Do(*this);
		}
	private:
		MethodT const &_m;
	};
	//--------------------------------------
public:
	struct InvokeType {
		struct ivtCtor {};
		struct ivtFactory {};
		struct ivtNormal {};
		template <class BASE>
		struct ivtProxy { typedef BASE BaseT; };
		template <typename METHOD>
		struct ivtBridge {
			typedef typename traits<METHOD>::ClassT BridgeT;
			typedef typename ncbTypeConvertor::Stripper<typename traits<METHOD>::ResultT>::Type TargetT;
		};

		template <class REFCLASS, typename METHOD, class SEL = ivtNormal>
		struct InvokeCommand {
			typedef REFCLASS RefClassT;
			typedef METHOD   MethodT;
			typedef traits<MethodT> TraitsT;
			typedef typename TraitsT::ClassT ClassT;
			typedef typename DefsT::TypeSelect<
				(DefsT::TypeEqual<ClassT, void>::Result), noInstanceGetter, nativeInstanceGetter<RefClassT> >::Result GetInstanceT;
			// GetInstanceT=     (ClassT==void) ?         noInstanceGetter: nativeInstanceGetter<RefClassT>;
			typedef paramsFunctor<MethodT> FunctorT;
			enum {
				InvokeSelect = doInvokeBase::ivsMethod,
				ArgsCount    = TraitsT::ArgsCount,
				Flags = (DefsT::IntSelect<
						 DefsT::TypeEqual<ClassT, void>::Result, TJS_STATICMEMBER, 0>::Result)
					//   Flags =         (ClassT==void) ?        TJS_STATICMEMBER :0;
			};
		};
		template <class REFCLASS, typename METHOD, typename BRIDGE>
		struct InvokeCommand<REFCLASS, METHOD, ivtBridge<BRIDGE> > {
			typedef                            ivtBridge<BRIDGE> BridgeT;
			typedef REFCLASS RefClassT;
			typedef METHOD   MethodT;
			typedef typename BridgeT::TargetT ClassT;
			typedef traits<MethodT> TraitsT;
			typedef bridgeInstanceGetter<RefClassT, ClassT, typename BridgeT::BridgeT> GetInstanceT;
			typedef paramsFunctor<MethodT> FunctorT;
			enum {
				InvokeSelect = doInvokeBase::ivsMethod,
				ArgsCount    = TraitsT::ArgsCount,
				Flags        = 0,
			};
		};
		template <class REFCLASS, typename METHOD>
		struct InvokeCommand<REFCLASS, METHOD, ivtProxy<ivtNormal> > : public InvokeCommand<REFCLASS, METHOD> {
			typedef /*                                                      */InvokeCommand<REFCLASS, METHOD> BaseT;
			typedef REFCLASS ClassT;
			typedef nativeInstanceGetter<ClassT> GetInstanceT;
			typedef paramsFunctorWithInstance<REFCLASS, typename BaseT::FunctorT> FunctorT;
			enum {
				InvokeSelect = doInvokeBase::ivsProxy,
				ArgsCount    = BaseT::ArgsCount - 1,
				Flags        = 0,
			};
		};
		template <class REFCLASS, typename METHOD, class BRIDGE>
		struct InvokeCommand<      REFCLASS, METHOD, ivtProxy< ivtBridge<BRIDGE> > >
			: public InvokeCommand<REFCLASS, METHOD,           ivtBridge<BRIDGE> > {
			typedef  InvokeCommand<REFCLASS, METHOD,           ivtBridge<BRIDGE> > BaseT;
			typedef paramsFunctorWithInstance<typename BaseT::ClassT, typename BaseT::FunctorT> FunctorT;
			enum {
				InvokeSelect = doInvokeBase::ivsProxy,
				ArgsCount    = BaseT::ArgsCount - 1,
				Flags        = 0,
			};
		};
		template <class CLASS, typename METHOD>
		struct InvokeCommand<CLASS, METHOD, ivtCtor> {
			typedef CLASS RefClassT;
			typedef CLASS ClassT;
			typedef void* MethodT;
			typedef noInstanceGetter GetInstanceT;
			typedef traits<METHOD> TraitsT;
			typedef paramsFunctor<METHOD> FunctorT;
			enum {
				InvokeSelect = doInvokeBase::ivsConstructor,
				ArgsCount = TraitsT::ArgsCount
			};
		};
		template <class CLASS, typename METHOD>
		struct InvokeCommand<CLASS, METHOD, ivtFactory> {
			typedef CLASS RefClassT;
			typedef CLASS ClassT;
			typedef METHOD   MethodT;
			typedef noInstanceGetter GetInstanceT;
			typedef traits<METHOD> TraitsT;
			typedef paramsFunctorWithInstance<iTJSDispatch2, paramsFunctor<METHOD> > FunctorT;
			enum {
				InvokeSelect = doInvokeBase::ivsFactory,
				ArgsCount = TraitsT::ArgsCount - 1
			};
			struct NoInstanceReturn {};
			typedef typename DefsT::TypeAssert<
				!DefsT::TypeEqual<typename TraitsT::ResultT, ClassT*>::Result,
			/**/NoInstanceReturn>::Result CheckResultType;

			struct NoStaticMethod {};
			typedef typename DefsT::TypeAssert<
				!DefsT::TypeEqual<typename TraitsT::ClassT, void>::Result,
			/**/NoStaticMethod >::Result CheckClassType;
		};
		
		template <class REFCLASS, typename GETTER, typename SETTER, class SEL>
		struct PropertyCommand {
			typedef InvokeCommand<REFCLASS, GETTER, SEL> GetCommandT;
			typedef InvokeCommand<REFCLASS, SETTER, SEL> SetCommandT;
		};
	};
};



////////////////////////////////////////
template <class CommandT>
struct ncbNativeClassMethod : public ncbNativeClassMethodBase { 
	typedef ncbNativeClassMethod ThisClassT;
	typedef typename CommandT::MethodT MethodT;

	ncbNativeClassMethod(MethodT m) : ncbNativeClassMethodBase(nitMethod), _method(m) {
		if (!_method) TVPThrowExceptionMessage(TJS_W("No method pointer."));
	} 

	tjs_error  TJS_INTF_METHOD FuncCall(
		tjs_uint32 flag, const tjs_char * membername, tjs_uint32 *hint, 
		tTJSVariant *result, tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
	{
		if (membername) return BaseT::FuncCall(flag, membername, hint, result, numparams, param, objthis);

		if (!objthis) return TJS_E_NATIVECLASSCRASH;

		return (doInvoke<CommandT>(_method, result, numparams, param, objthis)).Invoke();
	}
	static iMethodT Create(MethodT m, bool create = true) { return !create ? 0 : (new ThisClassT(m))->GetIMethod(); }

protected:
	MethodT const _method;

private:
	FlagsT GetFlags() const { return CommandT::Flags; }
};

////////////////////////////////////////
template <class CommandT>
struct ncbNativeClassConstructor : public ncbNativeClassMethodBase {
	typedef ncbNativeClassConstructor ThisClassT;
	typedef typename CommandT::MethodT MethodT;

	ncbNativeClassConstructor(MethodT m) : ncbNativeClassMethodBase(nitMethod), _method(m) {
		if (!_method && (int)CommandT::InvokeSelect == (int)doInvokeBase::ivsFactory)
			TVPThrowExceptionMessage(TJS_W("No factory pointer."));
	}

	tjs_error  TJS_INTF_METHOD FuncCall(
		tjs_uint32 flag, const tjs_char * membername, tjs_uint32 *hint, 
		tTJSVariant *result, tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
	{
		if (membername) return BaseT::FuncCall(flag, membername, hint, result, numparams, param, objthis);

		if ((numparams == 1) && (ncbTypedefs::GetVariantType(*param[0]) == tvtVoid)) {
//			NCB_LOG_W("Constructor(void)");
			return TJS_S_OK;
		}
		return (doInvoke<CommandT>(_method, result, numparams, param, objthis)).Invoke();
	}

	static iMethodT Create(MethodT m, bool create = true) { return !create ? 0 : (new ThisClassT(m))->GetIMethod(); }

protected:
	MethodT const _method;
};

template <class ClassT>
struct ncbNativeClassFactory : public ncbNativeClassMethodBase {
	typedef ncbNativeClassFactory ThisClassT;
	typedef tjs_error (TJS_INTF_METHOD *MethodT)(ClassT **result, tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis);

	ncbNativeClassFactory(MethodT m) : ncbNativeClassMethodBase(nitMethod), _method(m) {
		if (!_method) TVPThrowExceptionMessage(TJS_W("No factory pointer."));
	}

	tjs_error  TJS_INTF_METHOD FuncCall(
		tjs_uint32 flag, const tjs_char * membername, tjs_uint32 *hint, 
		tTJSVariant *result, tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
	{
		if (membername) return BaseT::FuncCall(flag, membername, hint, result, numparams, param, objthis);

		if ((numparams == 1) && (ncbTypedefs::GetVariantType(*param[0]) == tvtVoid)) {
//			NCB_LOG_W("Constructor(void)");
			return TJS_S_OK;
		}
		ClassT *inst = 0;
		tjs_error r = _method(&inst, numparams, param, objthis);
		if (r != TJS_S_OK) return r;
		if (!ncbInstanceAdaptor<ClassT>::SetNativeInstance(objthis, inst)) {
			delete inst;
			return TJS_E_NATIVECLASSCRASH;
		}
		return TJS_S_OK;
	}

	static iMethodT Create(MethodT m, bool create = true) { return !create ? 0 : (new ThisClassT(m))->GetIMethod(); }

protected:
	MethodT const _method;
};


////////////////////////////////////////

template <class PropCommandT>
struct ncbNativeClassProperty : public ncbNativeClassMethodBase {
	typedef ncbNativeClassProperty ThisClassT;
	typedef typename PropCommandT::GetCommandT GetCommandT;
	typedef typename PropCommandT::SetCommandT SetCommandT;
	typedef typename GetCommandT::MethodT GetterT;
	typedef typename SetCommandT::MethodT SetterT;

	ncbNativeClassProperty(GetterT get, SetterT set) : ncbNativeClassMethodBase(nitProperty), _getter(get), _setter(set) {}

	tjs_error TJS_INTF_METHOD PropGet(
		tjs_uint32 flag, const tjs_char * membername, tjs_uint32 *hint, 
		tTJSVariant *result, iTJSDispatch2 *objthis)
	{
		if (membername) return BaseT::PropGet(flag, membername, hint, result, objthis);

		if (!_getter) return TJS_E_ACCESSDENYED;
		if (!objthis) return TJS_E_NATIVECLASSCRASH;

		return (doInvoke<GetCommandT>(_getter, result, 0, 0, objthis)).Invoke();
	}

	tjs_error TJS_INTF_METHOD PropSet(
		tjs_uint32 flag, const tjs_char *membername, tjs_uint32 *hint, 
		const tTJSVariant *param, iTJSDispatch2 *objthis)
	{
		if (membername) return BaseT::PropSet(flag, membername, hint, param, objthis);

		if (!_setter) return TJS_E_ACCESSDENYED;
		if (!objthis) return TJS_E_NATIVECLASSCRASH;
		if (!param)   return TJS_E_FAIL;

		return (doInvoke<SetCommandT>(_setter, 0, 1, const_cast<tTJSVariant**>(&param), objthis)).Invoke();
	}

	static iMethodT Create(GetterT g, SetterT s, bool create = true) { return !create ? 0 : (new ThisClassT(g, s))->GetIMethod(); }

private:
	FlagsT GetFlags() const { return GetCommandT::Flags; }

protected:
	GetterT const _getter;
	SetterT const _setter;
};


////////////////////////////////////////
template <typename T> struct ncbRawCallbackMethod;

template <class T>
struct ncbRawCallbackMethod<
/*        */tjs_error (TJS_INTF_METHOD *         )(tTJSVariant *result, tjs_int numparams, tTJSVariant **param, T *nativeInstance) > : public ncbNativeClassMethodBase {
	typedef tjs_error (TJS_INTF_METHOD *CallbackT)(tTJSVariant *result, tjs_int numparams, tTJSVariant **param, T *nativeInstance);
	typedef T                                NativeClassT;
	typedef ncbRawCallbackMethod             ThisClassT;
	typedef ncbInstanceAdaptor<NativeClassT> AdaptorT;


	ncbRawCallbackMethod(CallbackT m, FlagsT f)
		: ncbNativeClassMethodBase(nitMethod), // TJSオブジェクト的には Function
		  _callback(m), _flag(f)
	{
		if (!_callback) TVPThrowExceptionMessage(TJS_W("No callback pointer."));
	}

	tjs_error  TJS_INTF_METHOD FuncCall(
		tjs_uint32 flag, const tjs_char * membername, tjs_uint32 *hint, 
		tTJSVariant *result, tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
	{
		if (membername) return BaseT::FuncCall(flag, membername, hint, result, numparams, param, objthis);

		if (!objthis) return TJS_E_NATIVECLASSCRASH;

		if (result) result->Clear();

		NativeClassT *obj = 0;
		if (!(_flag & TJS_STATICMEMBER)) {
			//< 実インスタンスのポインタ
			obj = AdaptorT::GetNativeInstance(objthis); 
			if (!obj) return TJS_E_NATIVECLASSCRASH;
		}
		return _callback(result, numparams, param, obj);     //< Callback呼び出し
	}

	static iMethodT Create(CallbackT cb, FlagsT f, bool create = true) { return !create ? 0 : (new ThisClassT(cb, f))->GetIMethod(); }

protected:
	CallbackT const _callback;
	FlagsT    const _flag;

private:
	FlagsT    GetFlags()    const { return _flag; }
};

template <>
struct ncbRawCallbackMethod<tTJSNativeClassMethodCallback> : public ncbIMethodObject {
	typedef ncbRawCallbackMethod ThisClassT;
	typedef tTJSNativeClassMethodCallback CallbackT;
	typedef ncbNativeClassMethodBase::iMethodT iMethodT;

	ncbRawCallbackMethod(CallbackT m, FlagsT f) : _dispatch(TJSCreateNativeClassMethod(m)), _flags(f) {}

	DispatchT GetDispatch() const { return _dispatch; }
	FlagsT    GetFlags()    const { return _flags; }
	TypesT    GetType()     const { return nitMethod; }
	void      Release()     const { delete this; }

	tjs_error  TJS_INTF_METHOD FuncCall(
		tjs_uint32 flag, const tjs_char * membername, tjs_uint32 *hint, 
		tTJSVariant *result, tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
	{
		return _dispatch->FuncCall(flag, membername, hint, result, numparams, param, objthis);
	}

	static iMethodT Create(CallbackT cb, FlagsT f, bool create = true) { return !create ? 0 : (new ThisClassT(cb, f)); }
private:
	DispatchT const _dispatch;
	FlagsT    const _flags;
};


////////////////////////////////////////

// ncbRawCallbackMethod と AccessDenied を返すダミー型のセレクタ
template <typename CallbackT>
struct ncbRawCallbackPropertySelector {
	typedef ncbRawCallbackMethod<CallbackT> Type;
};
template <>
struct ncbRawCallbackPropertySelector<int> {
	typedef struct AccessDenied {
		AccessDenied(int, ncbNativeClassMethodBase::FlagsT) {}
		tjs_error  TJS_INTF_METHOD FuncCall(
			tjs_uint32 flag, const tjs_char * membername, tjs_uint32 *hint, 
			tTJSVariant *result, tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *objthis)
		{
			return TJS_E_ACCESSDENYED;
		}
	} Type;
};

template <typename GETTER, typename SETTER>
struct ncbRawCallbackProperty : public ncbNativeClassMethodBase {
	typedef ncbRawCallbackProperty ThisClassT;
	typedef GETTER GetCallbackT;
	typedef SETTER SetCallbackT;
	typedef typename ncbRawCallbackPropertySelector<GetCallbackT>::Type GetterT;
	typedef typename ncbRawCallbackPropertySelector<SetCallbackT>::Type SetterT;

	ncbRawCallbackProperty(GetCallbackT get, SetCallbackT set, FlagsT f)
		: ncbNativeClassMethodBase(nitProperty), // TJSオブジェクト的には Property
		  _getter(get, f), _setter(set, f), _flag(f) {} 

	tjs_error TJS_INTF_METHOD PropGet(
		tjs_uint32 flag, const tjs_char * membername, tjs_uint32 *hint, 
		tTJSVariant *result, iTJSDispatch2 *objthis)
	{
		if (membername) return BaseT::PropGet(flag, membername, hint, result, objthis);
		return _getter.FuncCall(flag, membername, hint, result, 0, 0, objthis);
	}

	tjs_error TJS_INTF_METHOD PropSet(
		tjs_uint32 flag, const tjs_char *membername, tjs_uint32 *hint, 
		const tTJSVariant *param, iTJSDispatch2 *objthis)
	{
		if (membername) return BaseT::PropSet(flag, membername, hint, param, objthis);
		tTJSVariant *params[1] = { const_cast<tTJSVariant *>(param) };
		return _setter.FuncCall(flag, membername, hint, 0, 1, params, objthis);
	}

	FlagsT GetFlags() const { return _flag; }

	/// factory
	static iMethodT Create(GetCallbackT get, SetCallbackT set, FlagsT f, bool create = true) { return !create ? 0 : (new ThisClassT(get, set, f))->GetIMethod(); }
protected:
	GetterT _getter;
	SetterT _setter;
	FlagsT _flag;
};


template <class T>
struct ncbSubClassItem;


////////////////////////////////////////

template <class IMPL>
struct ncbRegistClass : public ncbNativeClassMethodBase::InvokeType {
	typedef ncbTypedefs _DefsT;
	typedef IMPL     _ImplementT;
	typedef typename _ImplementT::NativeClassT _ClassT;
	typedef typename _ImplementT::NameT        _NameT;
	typedef typename _ImplementT::StringT      _StringT;
	typedef typename _ImplementT::FlagsT       _FlagsT;
	typedef typename _ImplementT::ItemT        _ItemT;

	ncbRegistClass(_ImplementT &d, bool r) : _impl(d), _isRegist(r), Normal(), Proxy() { Begin(); }
	~ncbRegistClass() { End(); }

private:
	_ImplementT &_impl;
	bool  const _isRegist;

	ivtNormal           const Normal;
	ivtProxy<ivtNormal> const Proxy;

	template <class BRIDGE> struct      Bridge { typedef BRIDGE BridgeT; };
	template <class BRIDGE> struct ProxyBridge { typedef BRIDGE BridgeT; };
	template <class BRIDGE> struct BridgeProxy { typedef BRIDGE BridgeT; };
	template <class METHOD>          ivtBridge<METHOD>        getBridgeType(METHOD) const { return          ivtBridge<METHOD>  (); }
	template <class METHOD> ivtProxy<ivtBridge<METHOD> > getProxyBridgeType(METHOD) const { return ivtProxy<ivtBridge<METHOD> >(); }

	typedef _ClassT       Class;
	typedef _ClassT       ClassT;
	typedef _ClassT const Const;
	typedef void          Static;

	template <typename BASE, typename METHOD>
	struct MethodType {
		typedef typename _DefsT::CallerT::tMethodResolver<
			typename _DefsT::CallerT::tMethodTraits<METHOD>::ResultType, BASE,
		/**/typename _DefsT::CallerT::tMethodTraits<METHOD>::ArgsType>::Type Type;
	};

public:
	template <typename T>
	struct TypeWrap { typedef T Type; };
	_NameT   GetName() const			{ return _impl.GetName(); }
	_NameT   GetName(_NameT  n) const	{ return n; }
	template <typename OTHER>
	_StringT GetName(OTHER s) const		{ return _StringT(s); }
	void Begin()						{ if (_isRegist) _impl.RegistBegin();            else _impl.UnregistBegin(); }
	void End()							{ if (_isRegist) _impl.RegistEnd();              else _impl.UnregistEnd(); }
	void DoItem(_NameT   n, _ItemT t)	{ if (_isRegist) _impl.RegistItem(n,         t); else _impl.UnregistItem(n);         }
	void DoItem(_StringT s, _ItemT t)	{ if (_isRegist) _impl.RegistItem(s.c_str(), t); else _impl.UnregistItem(s.c_str()); }

	template <typename NAME, typename MethodT, class IVT>
	void Method(NAME n, MethodT m, IVT const&) {
		DoItem(GetName(n), ncbNativeClassMethod< InvokeCommand<ClassT, MethodT, IVT> >::Create(m, _isRegist));
	}

	template <typename NAME, typename GetterT, typename SetterT, class IVT>
	void Property(NAME n, GetterT g, SetterT s, IVT const&) {
		DoItem(GetName(n), ncbNativeClassProperty< PropertyCommand<ClassT, GetterT, SetterT, IVT> >::Create(g, s, _isRegist));
	}

	template <typename NAME, typename GetterT, class IVT> void Property(NAME n, GetterT g, int, IVT const &tag) { Property(n, g, static_cast<GetterT>(0), tag); }
	template <typename NAME, typename SetterT, class IVT> void Property(NAME n, int, SetterT s, IVT const &tag) { Property(n, static_cast<SetterT>(0), s, tag); }

	template <typename NAME, typename MethodT>                   void Method(  NAME n, MethodT m)            { Method(  n, m,    Normal); }
	template <typename NAME, typename GetterT, typename SetterT> void Property(NAME n, GetterT g, SetterT s) { Property(n, g, s, Normal); }

	template <typename N, typename M, class B>             void Method(  N n, M m,           Bridge<B> const&) { Method(  n, m,    getBridgeType(     &B::operator())); }
	template <typename N, typename M, class B>             void Method(  N n, M m,      ProxyBridge<B> const&) { Method(  n, m,    getProxyBridgeType(&B::operator())); }
	template <typename N, typename M, class B>             void Method(  N n, M m,      BridgeProxy<B> const&) { Method(  n, m,    getProxyBridgeType(&B::operator())); }
	template <typename N, typename G, typename S, class B> void Property(N n, G g, S s,      Bridge<B> const&) { Property(n, g, s, getBridgeType(     &B::operator())); }
	template <typename N, typename G, typename S, class B> void Property(N n, G g, S s, ProxyBridge<B> const&) { Property(n, g, s, getProxyBridgeType(&B::operator())); }
	template <typename N, typename G, typename S, class B> void Property(N n, G g, S s, BridgeProxy<B> const&) { Property(n, g, s, getProxyBridgeType(&B::operator())); }

	template <typename MethodT>
	void Constructor(TypeWrap<MethodT>) {
		DoItem(GetName(), ncbNativeClassConstructor< InvokeCommand<ClassT, MethodT, ivtCtor> >::Create(0, _isRegist));
	}
	void Constructor(int dummy = 0) { Constructor(TypeWrap<void (_ClassT::*)()>()); }
#undef  FOREACH_START
#define FOREACH_START 1
#undef  FOREACH_END
#define FOREACH_END   FOREACH_MAX
#define CTOR_PRM_EXT(n) typename T ## n
#define CTOR_ARG_EXT(n)          T ## n
	// Constructor<T1, T2, ...>(0); で ClassT(T1, T2, ...) のコンストラクタを登録する
#undef  FOREACH
#define FOREACH \
	template <FOREACH_COMMA_EXT(CTOR_PRM_EXT) > \
		void    Constructor(         typename _DefsT::CallerT::tMethodType<void, _ClassT, FOREACH_COMMA_EXT(CTOR_ARG_EXT)>::Type) { \
			/**/Constructor(TypeWrap<typename _DefsT::CallerT::tMethodType<void, _ClassT, FOREACH_COMMA_EXT(CTOR_ARG_EXT)>::Type>()); \
		}
#include FOREACH_INCLUDE
#undef  CTOR_PRM_EXT
#undef  CTOR_ARG_EXT

	template <typename MethodT>
	void Factory(MethodT m) {
		DoItem(GetName(), ncbNativeClassConstructor< InvokeCommand<ClassT, MethodT, ivtFactory> >::Create(m, _isRegist));
	}

	void RawCallback(typename ncbNativeClassFactory<ClassT>::MethodT m) { Factory(m); }
	void Factory(    typename ncbNativeClassFactory<ClassT>::MethodT m) {
		DoItem(GetName(),     ncbNativeClassFactory<ClassT>::Create(m, _isRegist));
	}

	template <typename NAME, typename MethodT>
	void RawCallback(NAME n, MethodT m, _FlagsT flags) {
		DoItem(GetName(n), ncbRawCallbackMethod<MethodT>::Create(m, flags, _isRegist));
	}

	template <typename NAME, typename GetterT, typename SetterT>
	void RawCallback(NAME n, GetterT g, SetterT s, _FlagsT flags) {
		DoItem(GetName(n), ncbRawCallbackProperty<GetterT, SetterT>::Create(g, s, flags, _isRegist));
	}

	template <typename NAME, typename CLASS>
	void SubClass(NAME n, TypeWrap<CLASS>) {
		typedef ncbSubClassItem<CLASS> SubClassT;
		if (!SubClassT::Setup(GetName(n), _isRegist))
			TVPThrowExceptionMessage(TJS_W("SubClass registration failed."));
		DoItem(GetName(n), SubClassT::Create(_isRegist));
	}

	template <typename NAME, typename VALUE>
	void Variant(NAME n, VALUE const v, _FlagsT flag = TJS_STATICMEMBER) {
		typedef typename ncbTypeConvertor::SelectConvertorType<tTJSVariant, VALUE>::Type ConvT;
		_StringT s(n);
		_NameT name = s.c_str();
		if (!_isRegist)_impl.UnregistItem( name);
		else {
			ConvT cv;
			tTJSVariant var;
			cv(var, v);
			_impl.RegistVariant(name, var, flag);
		}
	}

	void Regist();
};

////////////////////////////////////////

struct ncbRegistNativeClassBase {
	typedef ncbNativeClassMethodBase::NameT      NameT;
	typedef ncbNativeClassMethodBase::iMethodT   ItemT;
	typedef ncbNativeClassMethodBase::FlagsT     FlagsT;
	typedef tTJSString                           StringT;
	ncbRegistNativeClassBase(NameT name) : _className(name) {}

	void   RegistBegin()			{}
	void   RegistItem(NameT, ItemT)	{}
	void   RegistEnd()				{}
	void UnregistBegin()			{}
	void UnregistItem(NameT)		{}
	void UnregistEnd()				{}
	void   RegistVariant(NameT, tTJSVariant const &, FlagsT) {}

	NameT  GetName() const {return _className; }
protected:
	NameT const _className;
};

template <class CLASS>
struct ncbRegistNativeClass : public ncbRegistNativeClassBase {
	typedef                          ncbRegistNativeClassBase BaseT;
	typedef CLASS                                NativeClassT;
	typedef ncbInstanceAdaptor<NativeClassT>     AdaptorT;

	typedef ncbClassInfo<NativeClassT>           ClassInfoT;
	typedef typename ClassInfoT::IdentT          IdentT;
	typedef typename ClassInfoT::ClassObjectT    ClassObjectT;

	ncbRegistNativeClass(NameT n) : BaseT(n), _classobj(0), _hasCtor(false) {}

	void RegistBegin() {
		NCB_LOG_2(TJS_W("BeginRegistClass: "), _className);

		_classobj = TJSCreateNativeClassForPlugin(_className, AdaptorT::CreateEmptyAdaptor);

		IdentT id  = TJSRegisterNativeClass(_className);

		if (!ClassInfoT::Set(_className, id, _classobj))
			TVPThrowExceptionMessage(TJS_W("Already registerd class."));

		TJSNativeClassSetClassID(_classobj, id);

		TJSNativeClassRegisterNCM(_classobj, TJS_W("finalize"),
								  TJSCreateNativeClassMethod(EmptyCallback),
								  _className, nitMethod);
	}

	void RegistVariant(NameT name, tTJSVariant const &val, FlagsT flg) {
		_classobj->PropSet(TJS_MEMBERENSURE | flg, name, 0, &val, _classobj);
	}

	void RegistItem(NameT name, ItemT item) {
		NCB_LOG_2(TJS_W("  RegistItem: "), name);
		if (name == _className) {
			if (_hasCtor) TVPAddLog(tTJSString(TJS_W("Multiple constructors.(")) + _className + TJS_W(")"));
			_hasCtor = true;
		}
		if (item) {
			TJSNativeClassRegisterNCM(_classobj, name, item->GetDispatch(), _className, item->GetType(), item->GetFlags());
			item->Release();
		}
	}

	void RegistEnd() {
		_AddDummyConstructor();

		iTJSDispatch2 *global = TVPGetScriptDispatch();
		if (!global) {
			NCB_WARN_W("No Global Dispatch, Regist failed.");
			return;
		}

		tTJSVariant val(static_cast<iTJSDispatch2*>(_classobj));

		_classobj->Release();

		global->PropSet(
			TJS_MEMBERENSURE,
			_className,
			0,
			&val,
			global
			);

		global->Release();


		NCB_LOG_2(TJS_W("EndRegistClass: "), _className);
	}

	void UnregistEnd() {
		iTJSDispatch2 *global = TVPGetScriptDispatch();
		if (global) {
			global->DeleteMember(0, _className, 0, global);
			global->Release();
		}
		_RemoveClassInfo();
		NCB_LOG_2(TJS_W("EndUnregistClass: "), _className);
	}
	void UnregistBegin() {
		NCB_LOG_2(TJS_W("BeginUnregistClass: "), _className);
	}

private:
	ClassObjectT *_classobj;	
	bool          _hasCtor;		

	static tjs_error TJS_INTF_METHOD EmptyCallback(  tTJSVariant *, tjs_int, tTJSVariant **, iTJSDispatch2 *) { return TJS_S_OK; }

	static tjs_error TJS_INTF_METHOD NotImplCallback(tTJSVariant *, tjs_int, tTJSVariant **, iTJSDispatch2 *) { return TJS_E_NOTIMPL; }

protected:
	void _AddDummyConstructor() const {
		if (!_hasCtor) TJSNativeClassRegisterNCM(_classobj, _className,
												 TJSCreateNativeClassMethod(NotImplCallback),
												 _className, nitMethod);
	}
	void _RemoveClassInfo() const {
		NCB_LOG_2(TJS_W("  RemoveClassInfo: "), _className);
		ClassInfoT::Clear();
	}
};

////////////////////////////////////////
template <class CLASS>
struct ncbRegistSubClass : public ncbRegistNativeClass<CLASS> {
	typedef                       ncbRegistNativeClass<CLASS> BaseT;
	ncbRegistSubClass(typename BaseT::NameT n) : BaseT(n) {}
	void RegistEnd() {
		BaseT::_AddDummyConstructor();
		NCB_LOG_2(TJS_W("EndSubClass: "), BaseT::_className);
	}
	void UnregistEnd() {
		BaseT::_RemoveClassInfo();
		NCB_LOG_2(TJS_W("EndUnregistSubClass: "), BaseT::_className);
	}
};

template <class T>
struct ncbSubClassItem : public ncbIMethodObject {
	typedef ncbSubClassItem ThisClassT;
	typedef ncbNativeClassMethodBase::iMethodT iMethodT;
	typedef ncbClassInfo<T> ClassInfoT;

	DispatchT GetDispatch() const { return static_cast<DispatchT>(ClassInfoT::GetClassObject()); }
	FlagsT    GetFlags()    const { return TJS_STATICMEMBER; }
	TypesT    GetType()     const { return nitClass; }
	void      Release()     const { delete this; }

	/// factory
	static iMethodT Create(bool create = true) { return !create ? 0 : (new ThisClassT()); }

	static bool Setup(ncbTypedefs::NameT name, bool isRegist) {
		typedef ncbRegistSubClass<T> DelegateT;
		typedef ncbRegistClass<DelegateT> RegistT;

		if (isRegist && ClassInfoT::GetClassObject()) return false;
		DelegateT d(name);
		{
			RegistT r(d, isRegist);
			r.Regist();
		}
		return isRegist ? (ClassInfoT::GetClassObject() != 0) : true;
	}
};


////////////////////////////////////////

template <class CLASS>
struct ncbAttachTJS2Class : public ncbRegistNativeClassBase {
	typedef                        ncbRegistNativeClassBase BaseT;
	typedef CLASS                                NativeClassT;
	typedef ncbClassInfo<NativeClassT>           ClassInfoT;
	typedef typename ClassInfoT::IdentT          IdentT;
	typedef typename ClassInfoT::ClassObjectT    ClassObjectT;

	ncbAttachTJS2Class(NameT nativeClass, NameT tjs2Class) : BaseT(nativeClass), _tjs2ClassName(tjs2Class) {}

	void RegistBegin() {
		NCB_LOG_2(TJS_W("BeginAttachTJS2Class: "), _className);

		_global = TVPGetScriptDispatch();
		_tjs2ClassObj = GetGlobalObject(_tjs2ClassName, _global);

		IdentT id  = TJSRegisterNativeClass(_className);
		NCB_LOG_2(TJS_W("  ID: "), (tjs_int)id);

		if (!ClassInfoT::Set(_className, id, 0)) {
			TVPThrowExceptionMessage(TJS_W("Already registerd class:"), ttstr(_className));
		}
	}

	void RegistVariant(NameT name, tTJSVariant const &val, FlagsT flg) {
		_tjs2ClassObj->PropSet(TJS_MEMBERENSURE | flg, name, 0, &val, ((flg & TJS_STATICMEMBER) ? _global : _tjs2ClassObj));
	}

	void RegistItem(NameT name, ItemT item) {
		NCB_LOG_2(TJS_W("  RegistItem: "), name);
		if (!item) return;
		if (name == _className) {
			TVPThrowExceptionMessage(TJS_W("Constructor attached: "), ttstr(_className));
		}
		iTJSDispatch2 *dsp = item->GetDispatch();
		tTJSVariant val(dsp);
		dsp->Release();
		FlagsT flg = item->GetFlags();
		RegistVariant(name, val, flg);
		item->Release();
	}

	void RegistEnd() {
		if (_global) _global->Release();
		_global = 0;
		NCB_LOG_2(TJS_W("EndAttachClass: "), _className);
	}

	void UnregistBegin() {
		NCB_LOG_2(TJS_W("BeginDetach: "), _className);
		iTJSDispatch2 *global = TVPGetScriptDispatch();
		if (global) {
			_tjs2ClassObj = GetGlobalObject(_tjs2ClassName, global);
			global->Release();
		} else {
			_tjs2ClassObj = NULL;
		}
	}
	void UnregistItem(NameT name) {
		NCB_LOG_2(TJS_W("DetachItem: "), name);
		if (_tjs2ClassObj) {
			_tjs2ClassObj->DeleteMember(0, name, 0, _tjs2ClassObj);
		}
	}
	void UnregistEnd() {
		ClassInfoT::Clear();
		NCB_LOG_2(TJS_W("EndDetach: "), _className);
	}
protected:
	NameT const    _tjs2ClassName;
	iTJSDispatch2* _tjs2ClassObj;	
	iTJSDispatch2* _global;

	static iTJSDispatch2* GetGlobalObject(NameT name, iTJSDispatch2 *global) {
		tTJSVariant val;
		if (global) global->PropGet(0, name, 0, &val, global);
		else {
			NCB_WARN_W("No Global Dispatch.");
			TVPExecuteExpression(name, &val);
		}
		return val.AsObjectNoAddRef();
	}
};




////////////////////////////////////////
struct ncbAutoRegister {
	typedef ncbAutoRegister ThisClassT;
	typedef void (*CallBackT)();
	typedef ncbTypedefs DefsT;
	typedef DefsT::NameT NameT;
	typedef DefsT::BoolTag<true>  TrueTagT;
	typedef DefsT::BoolTag<false> FalseTagT;

	enum LineT {
		PreRegist = 0,
		ClassRegist,
		PostRegist,
		LINE_COUNT };
#define NCB_INNER_AUTOREGISTER_LINES_INSTANCE { 0, 0, 0 }
	NameT modulename;
	ncbAutoRegister(NameT name, LineT line) : modulename(name), _next(_top[line]) { _top[line] = this; }

	static void AllRegist(  LineT line) {
#ifdef _DEBUG
		NCB_LOG_2(TJS_W("AllRegist:"), line);
#endif
		for (ThisClassT const* p = _top[line]; p; p = p->_next) {
			ttstr name = p->modulename;
			name.ToLowerCase();
			_internal_plugins[name].lists[line].push_back(p);//p->Regist();
		}
	}
	static void AllUnregist(LineT line) {
#ifdef _DEBUG
		NCB_LOG_2(TJS_W("AllUnregist:"), line);
#endif
		for (ThisClassT const* p = _top[line]; p; p = p->_next)
			p->Unregist();
	}

	static void AllRegist()   { for (int line = 0; line < LINE_COUNT; line++) AllRegist(  static_cast<LineT>(line)); }
	static void AllUnregist() { for (int line = 0; line < LINE_COUNT; line++) AllUnregist(static_cast<LineT>(line)); }
	static bool LoadModule(const ttstr &_name);
protected:
	virtual void Regist()   const = 0;
	virtual void Unregist() const = 0;
private:
	ncbAutoRegister();
	/****/ ThisClassT const* _next;
	static ThisClassT const* _top[LINE_COUNT];

    struct INTERNAL_PLUGIN_LISTS {
        std::list<ThisClassT const*> lists[LINE_COUNT];
    };

	static std::map<ttstr, INTERNAL_PLUGIN_LISTS > _internal_plugins;
};

////////////////////////////////////////
template <class T>
struct  ncbNativeClassAutoRegister : public ncbAutoRegister {
	/**/ncbNativeClassAutoRegister(NameT modulename, NameT n) : ncbAutoRegister(modulename, ClassRegist), _name(n) {}

	typedef ncbRegistNativeClass<T> DelegateT;
	typedef ncbRegistClass<DelegateT> RegistT;
private:
	NameT const _name;
protected:
	void Regist()   const { DelegateT d(_name); { RegistT r(d, true);  r.Regist(); } }
	void Unregist() const { DelegateT d(_name); { RegistT r(d, false); r.Regist(); } }
};

#define NCB_REGISTER_CLASS_COMMON(cls, tmpl, init) \
	template    struct ncbClassInfo<cls>; \
/*	template <>        ncbClassInfo<cls>::InfoT ncbClassInfo<cls>::_info;*/ \
	static tmpl<cls> tmpl ## _ ## cls init; \
	template <> void   tmpl<cls>::RegistT::Regist()

#define NCB_REGISTER_CLASS_DELAY(name, cls) \
	NCB_REGISTER_CLASS_COMMON(cls, ncbNativeClassAutoRegister, (NCB_MODULE_NAME, TJS_W(# name)))

#define NCB_REGISTER_CLASS(cls) NCB_REGISTER_CLASS_DIFFER(cls, cls)
#define NCB_REGISTER_CLASS_DIFFER(name, cls) \
	NCB_TYPECONV_BOXING(cls); \
	NCB_REGISTER_CLASS_COMMON(cls, ncbNativeClassAutoRegister, (NCB_MODULE_NAME, TJS_W(# name)))

#define NCB_REGISTER_SUBCLASS_DELAY(cls) \
	template <> struct ncbSubClassCheck<cls> { enum { IsSubClass = true }; }; \
	template    struct ncbClassInfo<cls>; \
/*	template <>        ncbClassInfo<cls>::InfoT ncbClassInfo<cls>::_info;*/ \
	template <> void   ncbRegistClass<ncbRegistSubClass<cls> >::Regist()

#define NCB_REGISTER_SUBCLASS(cls) \
	NCB_TYPECONV_BOXING(cls); \
	NCB_REGISTER_SUBCLASS_DELAY(cls)

////////////////////////////////////////
template <class T>
struct  ncbAttachTJS2ClassAutoRegister : public ncbAutoRegister {
	/**/ncbAttachTJS2ClassAutoRegister(NameT modulename, NameT ncn, NameT tjscn) : ncbAutoRegister(modulename, ClassRegist), _nativeClassName(ncn), _tjs2ClassName(tjscn) {}

	typedef ncbAttachTJS2Class<T>     DelegateT;
	typedef ncbRegistClass<DelegateT> RegistT;
protected:
	void Regist()   const { DelegateT d(_nativeClassName, _tjs2ClassName); { RegistT r(d, true);  r.Regist(); } }
	void Unregist() const { DelegateT d(_nativeClassName, _tjs2ClassName); { RegistT r(d, false); r.Regist(); } }
private:
	NameT const _nativeClassName;
	NameT const _tjs2ClassName;
};

////////////////////////////////////////
template <class T>
struct  ncbRequireClassAutoRegister : public ncbAutoRegister {
	/**/ncbRequireClassAutoRegister(NameT modulename, NameT name, NameT sub = 0) : ncbAutoRegister(modulename, ClassRegist), _className(name), _expName(sub) {}

	typedef ncbClassInfo<T> ClassInfoT;
protected:
	void Regist()   const {
		NCB_LOG_2(TJS_W("RequireClass: "), _className);

		tTJSVariant val;
		TVPExecuteExpression(ttstr(_expName ? _expName : _className), &val);
		if (val.Type() != tvtObject)
			TVPThrowExceptionMessage(TJS_W("Require class not found."));

		if (!ClassInfoT::Set(_className, TJSFindNativeClassID(_className), val.AsObjectNoAddRef()))
			TVPThrowExceptionMessage(TJS_W("Already registerd class."));
	}
	void Unregist() const {}
private:
	NameT const _className;
	NameT const _expName;
};

////////////////////////////////////////
#define NCB_GET_INSTANCE_HOOK(cls) \
	template <> struct ncbNativeClassMethodBase::nativeInstanceGetter<cls> : public ncbNativeClassMethodBase::nativeInstanceGetterBase<cls>

#define NCB_GET_INSTANCE_HOOK_CLASS nativeInstanceGetter

#define NCB_INSTANCE_GETTER(objthis) \
	inline ClassT* Get(DispatchT objthis)

#define NCB_ATTACHED_INSTANCE_DELAY_CREATE(cls) \
	NCB_GET_INSTANCE_HOOK(cls) { NCB_INSTANCE_GETTER(objthis) { \
		ClassT* obj = GetNativeInstance(objthis); \
		if (!obj) { obj = new ClassT(); SetNativeInstance(objthis, obj); } \
		return obj; } }

#define NCB_ATTACH_CLASS_WITH_HOOK(cls, attach) \
	template <> struct ncbNativeClassMethodBase::nativeInstanceGetter<cls>; \
	NCB_REGISTER_CLASS_COMMON(cls, ncbAttachTJS2ClassAutoRegister, (NCB_MODULE_NAME, TJS_W(# cls), TJS_W(# attach)))

#define NCB_ATTACH_CLASS(cls, attach) \
	NCB_ATTACHED_INSTANCE_DELAY_CREATE(cls); \
	NCB_ATTACH_CLASS_WITH_HOOK(cls, attach)

#define NCB_REQUIRE_CLASS(cls)          template struct ncbClassInfo<cls>; static ncbRequireClassAutoRegister<cls> ncbRequireClass_ ## cls(TJS_W(# cls), 0)
#define NCB_REQUIRE_SUBCLASS(cls, name) template struct ncbClassInfo<cls>; static ncbRequireClassAutoRegister<cls> ncbRequireClass_ ## cls(TJS_W(# cls), TJS_W(# name))


////////////////////////////////////////
#define NCB_METHOD_RAW_CALLBACK(name, cb, flag)         RawCallback(TJS_W(# name), cb,          flag)
#define NCB_PROPERTY_RAW_CALLBACK(name, get, set, flag) RawCallback(TJS_W(# name), get,    set, flag)
#define NCB_PROPERTY_RAW_CALLBACK_RO(name, get, flag)   RawCallback(TJS_W(# name), get, (int)0, flag)
#define NCB_PROPERTY_RAW_CALLBACK_WO(name, set, flag)   RawCallback(TJS_W(# name), (int)0, set, flag)

#define NCB_CONSTRUCTOR(cargs)                          Constructor(TypeWrap<void (Class::*) cargs >())

#define NCB_METHOD_DIFFER(name, method)                 Method(TJS_W(# name), &Class::method)
#define NCB_METHOD(method)                              NCB_METHOD_DIFFER(method, method)

#define NCB_METHOD_CAST(tag, result, method, args)      static_cast<MethodType<tag, result (*) args >::Type>(&method)  // tag = { Class, Const, Static }
#define NCB_METHOD_DETAIL(name, T,R,M,A)                Method(TJS_W(# name), NCB_METHOD_CAST(T, R, M, A))

#define NCB_PROPERTY(name,get,set)                      Property(TJS_W(# name), &Class::get, &Class::set)
#define NCB_PROPERTY_RO(name,get)                       Property(TJS_W(# name), &Class::get, (int)0)
#define NCB_PROPERTY_WO(name,set)                       Property(TJS_W(# name), (int)0, &Class::set)

#define NCB_PROPERTY_DETAIL(N,RT,RR,RM,RA,WT,WR,WM,WA)  Property(TJS_W(# N),    NCB_METHOD_CAST(RT,RR,RM,RA), NCB_METHOD_CAST(WT,WR,WM,WA))
#define NCB_PROPERTY_DETAIL_RO(name, RT,RR,RM,RA)       Property(TJS_W(# name), NCB_METHOD_CAST(RT,RR,RM,RA), (int)0)
#define NCB_PROPERTY_DETAIL_WO(name, WT,WR,WM,WA)       Property(TJS_W(# name), (int)0,                       NCB_METHOD_CAST(WT,WR,WM,WA))

#define NCB_METHOD_PROXY(name, method)                  Method(TJS_W(# name), &method, Proxy)
#define NCB_METHOD_PROXY_DETAIL(name, T,R,M,A)          Method(TJS_W(# name), NCB_METHOD_CAST(T,R,M,A), Proxy)
#define NCB_PROPERTY_PROXY(name,get,set)                Property(TJS_W(# name), &get, &set, Proxy)
#define NCB_PROPERTY_PROXY_RO(name,get)                 Property(TJS_W(# name), &Class::get, (int)0, Proxy)
#define NCB_PROPERTY_PROXY_WO(name,set)                 Property(TJS_W(# name), (int)0, &Class::set, Proxy)
#define NCB_PROPERTY_PROXY_DETAIL(N,RT,RR,RM,RA,WT,WR,WM,WA)\
	/*                                                */Property(TJS_W(# N),    NCB_METHOD_CAST(RT,RR,RM,RA), NCB_METHOD_CAST(WT,WR,WM,WA)), Proxy)
#define NCB_PROPERTY_PROXY_DETAIL_RO(name, RT,RR,RM,RA) Property(TJS_W(# name), NCB_METHOD_CAST(RT,RR,RM,RA), (int)0,                        Proxy)
#define NCB_PROPERTY_PROXY_DETAIL_WO(name, WT,WR,WM,WA) Property(TJS_W(# name), (int)0,                       NCB_METHOD_CAST(WT,WR,WM,WA)), Proxy)

#define NCB_SUBCLASS(name, cls)                         SubClass(TJS_W(# name), TypeWrap<cls>())


////////////////////////////////////////

struct  ncbNativeFunctionAutoRegister : public ncbAutoRegister {
	ncbNativeFunctionAutoRegister(NameT name) : ncbAutoRegister(name, ClassRegist) {}
protected:
	template <typename MethodT>
	static void RegistFunction(NameT name, NameT attach, MethodT m) {
		typedef ncbNativeClassMethodBase::InvokeType IVT;
		typedef ncbNativeClassMethod< IVT::InvokeCommand<void, MethodT, IVT::ivtNormal> > MethodObjectT;
		iTJSDispatch2 *dsp = GetDispatch(attach);
		if (!dsp) return;
		RegistItem(dsp, name, new MethodObjectT(m));
	}
	static void RegistFunction(NameT name, NameT attach, ncbTypedefs::CallbackT m) {
		iTJSDispatch2 *dsp = GetDispatch(attach);
		if (!dsp) return;
		RegistItem(dsp, name, TJSCreateNativeClassMethod(m));
	}
	template <typename T>
	static inline void RegistItem(iTJSDispatch2 *dsp, NameT name, T *mobj) {
		if (mobj) {
			tTJSVariant val(mobj);
			mobj->Release();
			dsp->PropSet(TJS_MEMBERENSURE, name, 0, &val, dsp);
		}
		dsp->Release();
	}

	static void UnregistFunction(NameT name, NameT attach) {
		iTJSDispatch2 *dsp = GetDispatch(attach);
		if (!dsp) return;
		dsp->DeleteMember(0, name, 0, dsp);
		dsp->Release();
	}

private:
	static iTJSDispatch2* GetDispatch(NameT attach) {
		iTJSDispatch2 *ret, *global = TVPGetScriptDispatch();
		if (!global) return 0;

		if (!attach) ret = global;
		else {
			tTJSVariant val;
			NameT p = attach;
			while (*p) if (*p++ == TJS_W('.')) break;
			if (!*p) {
				global->PropGet(0, attach, 0, &val, global);
			} else {
				TVPExecuteExpression(ttstr(attach), &val);
			}
			ret = val.AsObject();
			global->Release();
		}
		return ret;
	}
};
template <typename DUMMY>
struct ncbNativeFunctionAutoRegisterTempl;

#define NCB_REGISTER_FUNCTION_COMMON(name, tag, attach, function) \
	struct ncbFunctionTag_ ## tag {}; \
	template <> struct ncbNativeFunctionAutoRegisterTempl<ncbFunctionTag_ ## tag> : public ncbNativeFunctionAutoRegister \
	{	ncbNativeFunctionAutoRegisterTempl() : ncbNativeFunctionAutoRegister(NCB_MODULE_NAME){} \
		void Regist()   const { RegistFunction(TJS_W(# name), attach, &function); } \
		void Unregist() const { UnregistFunction(TJS_W(# name), attach); } }; \
	static ncbNativeFunctionAutoRegisterTempl<ncbFunctionTag_ ## tag> ncbFunctionAutoRegister_ ## tag

#define NCB_REGISTER_FUNCTION(name, function)                    NCB_REGISTER_FUNCTION_COMMON(name, name, 0, function)
#define NCB_ATTACH_FUNCTION(name, attach, function)              NCB_REGISTER_FUNCTION_COMMON(name, attach ## _ ## name, TJS_W(# attach), function)
#define NCB_ATTACH_FUNCTION_WITHTAG(name, tag, attach, function) NCB_REGISTER_FUNCTION_COMMON(name, tag ## _ ## name, TJS_W(# attach), function)




////////////////////////////////////////
struct ncbCallbackAutoRegister : public ncbAutoRegister {
	typedef void (*CallbackT)();

	ncbCallbackAutoRegister(NameT name, LineT line, CallbackT init, CallbackT term)
		: ncbAutoRegister(name, line), _init(init), _term(term) {}
protected:
	void Regist()   const { if (_init) _init(); }
	void Unregist() const { if (_term) _term(); }
private:
	CallbackT _init, _term;
};

#define NCB_REGISTER_CALLBACK(pos, init, term, tag) static ncbCallbackAutoRegister ncbCallbackAutoRegister_ ## pos ## _ ## tag (NCB_MODULE_NAME, ncbAutoRegister::pos, init, term)
#define NCB_PRE_REGIST_CALLBACK(cb)    NCB_REGISTER_CALLBACK(PreRegist,  &cb, 0, cb ## _0)
#define NCB_POST_REGIST_CALLBACK(cb)   NCB_REGISTER_CALLBACK(PostRegist, &cb, 0, cb ## _0)
#define NCB_PRE_UNREGIST_CALLBACK(cb)  NCB_REGISTER_CALLBACK(PreRegist,  0, &cb, 0_ ## cb)
#define NCB_POST_UNREGIST_CALLBACK(cb) NCB_REGISTER_CALLBACK(PostRegist, 0, &cb, 0_ ## cb)


#endif
