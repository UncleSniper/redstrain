#ifndef REDSTRAIN_MOD_TEXT_UNREPRESENTABLECHARACTERERROR_HPP
#define REDSTRAIN_MOD_TEXT_UNREPRESENTABLECHARACTERERROR_HPP

#include "types.hpp"
#include "RenditionError.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API UnrepresentableCharacterError : RenditionError {

	  private:
		const Char32 badChar;

	  public:
		UnrepresentableCharacterError(Char32);
		UnrepresentableCharacterError(const UnrepresentableCharacterError&);

		inline Char32 getBadChar32() const {
			return badChar;
		}

		REDSTRAIN_DECLARE_ERROR(UnrepresentableCharacterError)

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_UNREPRESENTABLECHARACTERERROR_HPP */
