#ifndef REDSTRAIN_MOD_LOCALE_MESSAGEKEYOUTOFBOUNDSERROR_HPP
#define REDSTRAIN_MOD_LOCALE_MESSAGEKEYOUTOFBOUNDSERROR_HPP

#include <redstrain/error/IndexOutOfBoundsError.hpp>

#include "LocalizationError.hpp"

namespace redengine {
namespace locale {

	class REDSTRAIN_LOCALE_API MessageKeyOutOfBoundsError
			: public LocalizationError, public error::IndexOutOfBoundsError {

	  public:
		MessageKeyOutOfBoundsError(size_t);
		MessageKeyOutOfBoundsError(const MessageKeyOutOfBoundsError&);

		REDSTRAIN_DECLARE_ERROR(MessageKeyOutOfBoundsError)

	};

}}

#endif /* REDSTRAIN_MOD_LOCALE_MESSAGEKEYOUTOFBOUNDSERROR_HPP */
