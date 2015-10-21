#include "LocalizationError.hpp"

namespace redengine {
namespace locale {

	LocalizationError::LocalizationError() {}

	LocalizationError::LocalizationError(const LocalizationError& error) : Error(error) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(LocalizationError)

}}
