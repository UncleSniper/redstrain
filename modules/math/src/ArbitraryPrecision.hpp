#ifndef REDSTRAIN_MOD_MATH_ARBITRARYPRECISION_HPP
#define REDSTRAIN_MOD_MATH_ARBITRARYPRECISION_HPP

#include <redstrain/util/types.hpp>

#include "api.hpp"

namespace redengine {
namespace math {

	class REDSTRAIN_MATH_API ArbitraryPrecision {

	  public:
		enum CopyPolicy {
			COPY_PRIVATE,
			COPY_SHARED,
			COPY_THREADED
		};

		enum InitPolicy {
			INIT_COPY,
			INIT_MOVE
		};

		struct REDSTRAIN_MATH_API IntegerData {

			bool sign;
			unsigned* digits;
			util::MemorySize size;

			IntegerData();
			IntegerData(bool, const unsigned*, util::MemorySize);
			IntegerData(bool, unsigned*, util::MemorySize, InitPolicy);
			IntegerData(IntegerData&, InitPolicy);
			IntegerData(const IntegerData&);
			virtual ~IntegerData();

			IntegerData& operator=(const IntegerData&);
			void assign(const IntegerData&);
			void assign(IntegerData&, InitPolicy);
			void assign(bool, unsigned*, util::MemorySize, InitPolicy = INIT_COPY);

			util::MemorySize usedSize() const;
			void compact(const unsigned*&, util::MemorySize&) const;
			int sign() const;

		};

	  public:
		static int intCmp(const IntegerData&, const IntegerData&);
		static void intAdd(const IntegerData&, const IntegerData&, bool&, unsigned*&, util::MemorySize&);
		static void intAdd(IntegerData&, const IntegerData&);
		static void intSub(const IntegerData&, const IntegerData&, bool&, unsigned*&, util::MemorySize&);
		static void intSub(IntegerData&, const IntegerData&);
		static void intMul(const IntegerData&, const IntegerData&, bool&, unsigned*&, util::MemorySize&);
		static void intMul(IntegerData&, const IntegerData&);
		static void intDiv(const IntegerData&, const IntegerData&, bool&, unsigned*&, util::MemorySize&);
		static void intDiv(IntegerData&, const IntegerData&);
		static void intMod(const IntegerData&, const IntegerData&, bool&, unsigned*&, util::MemorySize&);
		static void intMod(IntegerData&, const IntegerData&);
		static void intDiv(const IntegerData&, const IntegerData&,
				bool&, unsigned*&, util::MemorySize&, bool&, unsigned*&, util::MemorySize&);

	};

}}

#endif /* REDSTRAIN_MOD_MATH_ARBITRARYPRECISION_HPP */
