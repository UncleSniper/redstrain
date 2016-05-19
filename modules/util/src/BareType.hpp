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

	};

	template<typename T>
	class BareType<const T> {

	  public:
		typedef T Bare;
		typedef const T Const;
		typedef T& Ref;
		typedef const T& ConstRef;

	};

	template<typename T>
	class BareType<T&> {

	  public:
		typedef T Bare;
		typedef const T Const;
		typedef T& Ref;
		typedef const T& ConstRef;

	};

	template<typename T>
	class BareType<const T&> {

	  public:
		typedef T Bare;
		typedef const T Const;
		typedef T& Ref;
		typedef const T& ConstRef;

	};

}}

#endif /* REDSTRAIN_MOD_UTIL_BARETYPE_HPP */
