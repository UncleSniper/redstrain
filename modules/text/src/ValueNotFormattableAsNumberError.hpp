#ifndef REDSTRAIN_MOD_TEXT_VALUENOTFORMATTABLEASNUMBERERROR_HPP
#define REDSTRAIN_MOD_TEXT_VALUENOTFORMATTABLEASNUMBERERROR_HPP

#include "FormattingError.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API ValueNotFormattableAsNumberError : public FormattingError {

	  public:
		ValueNotFormattableAsNumberError();
		ValueNotFormattableAsNumberError(const ValueNotFormattableAsNumberError&);

		REDSTRAIN_DECLARE_ERROR(ValueNotFormattableAsNumberError)

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_VALUENOTFORMATTABLEASNUMBERERROR_HPP */
