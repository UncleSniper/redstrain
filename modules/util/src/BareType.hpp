#ifndef REDSTRAIN_MOD_UTIL_BARETYPE_HPP
#define REDSTRAIN_MOD_UTIL_BARETYPE_HPP

namespace redengine {
namespace util {

	template<typename T>
	class BareType {

	  public:
		typedef T Bare;
		typedef const T Const;
		typedef T& Ref;
		typedef const T& ConstRef;

	  public:
		static const bool WAS_CONST = false;
		static const bool WAS_REF = false;

	};

	template<typename T>
	class BareType<const T> {

	  public:
		typedef T Bare;
		typedef const T Const;
		typedef T& Ref;
		typedef const T& ConstRef;

	  public:
		static const bool WAS_CONST = true;
		static const bool WAS_REF = false;

	};

	template<typename T>
	class BareType<T&> {

	  public:
		typedef T Bare;
		typedef const T Const;
		typedef T& Ref;
		typedef const T& ConstRef;

	  public:
		static const bool WAS_CONST = false;
		static const bool WAS_REF = true;

	};

	template<typename T>
	class BareType<const T&> {

	  public:
		typedef T Bare;
		typedef const T Const;
		typedef T& Ref;
		typedef const T& ConstRef;

	  public:
		static const bool WAS_CONST = true;
		static const bool WAS_REF = true;

	};

}}

#endif /* REDSTRAIN_MOD_UTIL_BARETYPE_HPP */
