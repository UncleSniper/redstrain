#ifndef REDSTRAIN_MOD_CRYPTO_ALGORITHM_HPP
#define REDSTRAIN_MOD_CRYPTO_ALGORITHM_HPP

#include <redstrain/util/types.hpp>

#include "api.hpp"

namespace redengine {
namespace crypto {

	class REDSTRAIN_CRYPTO_API Algorithm {

	  public:
		template<unsigned Bits>
		class BytesNeeded {

		  public:
			static const unsigned BYTES = (Bits + 7u) / 8u;

		};

	  public:
		Algorithm();
		Algorithm(const Algorithm&);
		virtual ~Algorithm();

		virtual util::MemorySize getKeySize() const = 0;
		virtual void reset() = 0;

		static util::MemorySize bytesNeeded(util::MemorySize);

	};

}}

#endif /* REDSTRAIN_MOD_CRYPTO_ALGORITHM_HPP */
