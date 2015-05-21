#include "TextError.hpp"

namespace redengine {
namespace text {

	TextError::TextError() {}

	TextError::TextError(const TextError& error) : Error(error) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(TextError)

}}
