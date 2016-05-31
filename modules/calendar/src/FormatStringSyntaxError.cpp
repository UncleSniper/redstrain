#include "FormatStringSyntaxError.hpp"

using redengine::util::MemorySize;

namespace redengine {
namespace calendar {

	FormatStringSyntaxError::FormatStringSyntaxError(MemorySize offset) : offset(offset) {}

	FormatStringSyntaxError::FormatStringSyntaxError(const FormatStringSyntaxError& error)
			: Error(error), FormattingError(error), offset(error.offset) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(FormatStringSyntaxError)

}}
