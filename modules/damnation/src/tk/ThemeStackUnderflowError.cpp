#include "ThemeStackUnderflowError.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	ThemeStackUnderflowError::ThemeStackUnderflowError() {}

	ThemeStackUnderflowError::ThemeStackUnderflowError(const ThemeStackUnderflowError& error)
			: Error(error), DamnError(error), StateError(error) {}

	REDSTRAIN_DEFINE_ERROR(ThemeStackUnderflowError) {
		out << "Theme stack underflow";
	}

}}}
