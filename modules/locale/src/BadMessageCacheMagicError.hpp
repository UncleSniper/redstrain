#ifndef REDSTRAIN_MOD_LOCALE_BADMESSAGECACHEMAGIC_HPP
#define REDSTRAIN_MOD_LOCALE_BADMESSAGECACHEMAGIC_HPP

#include "LocalizationError.hpp"

namespace redengine {
namespace locale {

	class REDSTRAIN_LOCALE_API BadMessageCacheMagicError : public LocalizationError {

	  public:
		BadMessageCacheMagicError();
		BadMessageCacheMagicError(const BadMessageCacheMagicError&);

		REDSTRAIN_DECLARE_ERROR(BadMessageCacheMagicError)

	};

}}

#endif /* REDSTRAIN_MOD_LOCALE_BADMESSAGECACHEMAGIC_HPP */
