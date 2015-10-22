#ifndef REDSTRAIN_MOD_LOCALE_DUPLICATEMESSAGEKEYERROR_HPP
#define REDSTRAIN_MOD_LOCALE_DUPLICATEMESSAGEKEYERROR_HPP

#include "MessageKeyError.hpp"

namespace redengine {
namespace locale {

	class REDSTRAIN_LOCALE_API DuplicateMessageKeyError : public MessageKeyError {

	  public:
		DuplicateMessageKeyError(const std::string&, const std::string&, unsigned);
		DuplicateMessageKeyError(const DuplicateMessageKeyError&);

		REDSTRAIN_DECLARE_ERROR(DuplicateMessageKeyError)

	};

}}

#endif /* REDSTRAIN_MOD_LOCALE_DUPLICATEMESSAGEKEYERROR_HPP */
