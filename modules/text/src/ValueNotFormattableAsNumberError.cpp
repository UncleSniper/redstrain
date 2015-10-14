#include "ValueNotFormattableAsNumberError.hpp"

namespace redengine {
namespace text {

	ValueNotFormattableAsNumberError::ValueNotFormattableAsNumberError() {}

	ValueNotFormattableAsNumberError::ValueNotFormattableAsNumberError(const ValueNotFormattableAsNumberError& error)
			: FormattingError(error) {}

	REDSTRAIN_DEFINE_ERROR(ValueNotFormattableAsNumberError) {
		out << "Supplied value cannot be formatted as a number";
	}

}}
