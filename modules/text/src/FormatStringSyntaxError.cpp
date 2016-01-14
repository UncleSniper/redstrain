#include "FormatStringSyntaxError.hpp"

using redengine::util::MemorySize;

namespace redengine {
namespace text {

	FormatStringSyntaxError::FormatStringSyntaxError(MemorySize offset) : offset(offset) {}

	FormatStringSyntaxError::FormatStringSyntaxError(const FormatStringSyntaxError& error)
			: FormattingError(error), offset(error.offset) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(FormatStringSyntaxError)

}}
