#ifndef REDSTRAIN_MOD_TEXT_NUMBERFORMATADDRESSSPACEEXCEEDEDERROR_HPP
#define REDSTRAIN_MOD_TEXT_NUMBERFORMATADDRESSSPACEEXCEEDEDERROR_HPP

#include "FormattingError.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API NumberFormatAddressSpaceExceededError : public FormattingError {

	  public:
		NumberFormatAddressSpaceExceededError();
		NumberFormatAddressSpaceExceededError(const NumberFormatAddressSpaceExceededError&);

		REDSTRAIN_DECLARE_ERROR(NumberFormatAddressSpaceExceededError)

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_NUMBERFORMATADDRESSSPACEEXCEEDEDERROR_HPP */
