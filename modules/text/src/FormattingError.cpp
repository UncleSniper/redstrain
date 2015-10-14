#include "FormattingError.hpp"

namespace redengine {
namespace text {

	FormattingError::FormattingError() {}

	FormattingError::FormattingError(const FormattingError& error) : TextError(error) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(FormattingError)

}}
