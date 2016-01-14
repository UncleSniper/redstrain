#ifndef REDSTRAIN_MOD_CRYPTO_PADDING_HPP
#define REDSTRAIN_MOD_CRYPTO_PADDING_HPP

#include <redstrain/util/types.hpp>

#include "api.hpp"

namespace redengine {
namespace crypto {

	class REDSTRAIN_CRYPTO_API Padding {

	  public:
		Padding();
		Padding(const Padding&);
		virtual ~Padding();

		virtual void padBlock(char*, util::MemorySize, util::MemorySize) = 0;
		virtual util::MemorySize unpadBlock(char*, util::MemorySize) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_CRYPTO_PADDING_HPP */
