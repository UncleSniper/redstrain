#ifndef REDSTRAIN_MOD_LOCALE_MISSINGMESSAGEKEYERROR_HPP
#define REDSTRAIN_MOD_LOCALE_MISSINGMESSAGEKEYERROR_HPP

#include "MessageKeyError.hpp"

namespace redengine {
namespace locale {

	class REDSTRAIN_LOCALE_API MissingMessageKeyError : public MessageKeyError {

	  public:
		MissingMessageKeyError(const std::string&, const std::string&);
		MissingMessageKeyError(const MissingMessageKeyError&);

		REDSTRAIN_DECLARE_ERROR(MissingMessageKeyError)

	};

}}

#endif /* REDSTRAIN_MOD_LOCALE_MISSINGMESSAGEKEYERROR_HPP */
