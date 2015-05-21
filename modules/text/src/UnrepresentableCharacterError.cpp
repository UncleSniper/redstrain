#include <cstdio>

#include "UnrepresentableCharacterError.hpp"

namespace redengine {
namespace text {

	UnrepresentableCharacterError::UnrepresentableCharacterError(Char16 badChar) : badChar(badChar) {}

	UnrepresentableCharacterError::UnrepresentableCharacterError(const UnrepresentableCharacterError& error)
			: RenditionError(error), badChar(error.badChar) {}

	REDSTRAIN_DEFINE_ERROR(UnrepresentableCharacterError) {
		char buf[17];
		sprintf(buf, "%0lX", static_cast<unsigned long>(badChar));
		out << "Character not representable in chosen encoding: " << buf;
	}

}}
