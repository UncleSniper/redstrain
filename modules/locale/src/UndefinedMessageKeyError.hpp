#ifndef REDSTRAIN_MOD_LOCALE_UNDEFINEDMESSAGEKEYERROR_HPP
#define REDSTRAIN_MOD_LOCALE_UNDEFINEDMESSAGEKEYERROR_HPP

#include "MessageKeyError.hpp"

namespace redengine {
namespace locale {

	class REDSTRAIN_LOCALE_API UndefinedMessageKeyError : public MessageKeyError {

	  public:
		UndefinedMessageKeyError(const std::string&, const std::string&, unsigned);
		UndefinedMessageKeyError(const UndefinedMessageKeyError&);

		REDSTRAIN_DECLARE_ERROR(UndefinedMessageKeyError)

	};

}}

#endif /* REDSTRAIN_MOD_LOCALE_UNDEFINEDMESSAGEKEYERROR_HPP */
