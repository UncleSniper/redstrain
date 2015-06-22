#ifndef REDSTRAIN_MOD_UTIL_TYPEUTILS_HPP
#define REDSTRAIN_MOD_UTIL_TYPEUTILS_HPP

namespace redengine {
namespace util {

	template<typename A, typename B>
	class IsSameType {

	  private:
		template<typename, typename>
		struct Same {
			static const bool SAME = false;
		};

		template<typename T>
		struct Same<T, T> {
			static const bool SAME = true;
		};

	  public:
		static const bool SAME = Same<A, B>::SAME;

	};

	template<typename ObjectT, typename ConstraintT>
	class IsA {

	  private:
		static double acceptObject(ConstraintT&);
		static char acceptObject(...);

	  public:
		static const bool IS = sizeof(acceptObject(*reinterpret_cast<ObjectT*>(0))) == sizeof(double);

	};

}}

#endif /* REDSTRAIN_MOD_UTIL_TYPEUTILS_HPP */
