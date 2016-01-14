#ifndef REDSTRAIN_MOD_CRYPTO_ANSIX939PADDING_HPP
#define REDSTRAIN_MOD_CRYPTO_ANSIX939PADDING_HPP

#include "Padding.hpp"

namespace redengine {
namespace crypto {

	class ANSIX939Padding : public Padding {

	  public:
		ANSIX939Padding();
		ANSIX939Padding(const ANSIX939Padding&);

		virtual void padBlock(char*, util::MemorySize, util::MemorySize);
		virtual util::MemorySize unpadBlock(char*, util::MemorySize);

	};

}}

#endif /* REDSTRAIN_MOD_CRYPTO_ANSIX939PADDING_HPP */
