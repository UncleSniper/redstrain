#ifndef REDSTRAIN_MOD_UTIL_WITHALIGN_HPP
#define REDSTRAIN_MOD_UTIL_WITHALIGN_HPP

#include "AlignOf.hpp"

namespace redengine {
namespace util {

	template<size_t Alignment>
	class WithAlign {

	  private:
		template<typename PrimitiveT, size_t PrimitiveAlignment, typename NextT>
		struct Link {

			typedef PrimitiveT Primitive;
			typedef NextT Next;

			static const size_t ALIGNMENT = PrimitiveAlignment;

		};

		struct EndLink {};

		typedef int (*Function)(int);
		typedef int (EndLink::*Method)(int);

#define REDSTRAIN_UTIL_WITHALIGN_CASE(primitive, name, prev) \
		typedef Link<primitive, AlignOf<primitive>::ALIGNMENT, prev ## Link> name ## Link;

		REDSTRAIN_UTIL_WITHALIGN_CASE(Method, Method, End)
		REDSTRAIN_UTIL_WITHALIGN_CASE(Function, Function, Method)
		REDSTRAIN_UTIL_WITHALIGN_CASE(void*, Pointer, Function)
		REDSTRAIN_UTIL_WITHALIGN_CASE(long double, LongDouble, Pointer)
		REDSTRAIN_UTIL_WITHALIGN_CASE(double, Double, LongDouble)
		REDSTRAIN_UTIL_WITHALIGN_CASE(float, Float, Double)
		REDSTRAIN_UTIL_WITHALIGN_CASE(long long, LongLong, Float)
		REDSTRAIN_UTIL_WITHALIGN_CASE(long, Long, LongLong)
		REDSTRAIN_UTIL_WITHALIGN_CASE(int, Int, Long)
		REDSTRAIN_UTIL_WITHALIGN_CASE(short, Short, Int)
		REDSTRAIN_UTIL_WITHALIGN_CASE(char, Char, Short)
		typedef CharLink LastLink;

		template<int Dummy, typename LinkT>
		struct Lookup {

			template<int Dummy2, bool>
			struct Found {

				typedef typename Lookup<Dummy2, typename LinkT::Next>::Primitive Primitive;

			};

			template<int Dummy2>
			struct Found<Dummy2, true> {

				typedef typename LinkT::Primitive Primitive;

			};

			typedef typename Found<Dummy, LinkT::ALIGNMENT == Alignment>::Primitive Primitive;

		};

		template<int Dummy>
		struct Lookup<Dummy, EndLink> {

			typedef char Primitive;

		};

	  public:
		typedef typename Lookup<0, LastLink>::Primitive Primitive;

	};

}}

#endif /* REDSTRAIN_MOD_UTIL_WITHALIGN_HPP */
