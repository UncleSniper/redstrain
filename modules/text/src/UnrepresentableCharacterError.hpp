#ifndef REDSTRAIN_MOD_TEXT_UNREPRESENTABLECHARACTERERROR_HPP
#define REDSTRAIN_MOD_TEXT_UNREPRESENTABLECHARACTERERROR_HPP

#include "types.hpp"
#include "RenditionError.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API UnrepresentableCharacterError : RenditionError {

	  private:
		const Char16 badChar;

	  public:
		UnrepresentableCharacterError(Char16);
		UnrepresentableCharacterError(const UnrepresentableCharacterError&);

		inline Char16 getBadChar16() const {
			return badChar;
		}

		REDSTRAIN_DECLARE_ERROR(UnrepresentableCharacterError)

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_UNREPRESENTABLECHARACTERERROR_HPP */
