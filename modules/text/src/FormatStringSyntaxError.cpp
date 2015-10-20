#include "FormatStringSyntaxError.hpp"

namespace redengine {
namespace text {

	FormatStringSyntaxError::FormatStringSyntaxError(size_t offset) : offset(offset) {}

	FormatStringSyntaxError::FormatStringSyntaxError(const FormatStringSyntaxError& error)
			: FormattingError(error), offset(error.offset) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(FormatStringSyntaxError)

}}
