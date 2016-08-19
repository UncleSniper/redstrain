#include "CannotSetParentOfWidgetError.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	CannotSetParentOfWidgetError::CannotSetParentOfWidgetError() {}

	CannotSetParentOfWidgetError::CannotSetParentOfWidgetError(const CannotSetParentOfWidgetError& error)
			: Error(error), DamnError(error), IllegalArgumentError(error) {}

	REDSTRAIN_DEFINE_ERROR(CannotSetParentOfWidgetError) {
		out << "Cannot set parent of widget, add the widget to the parent instead";
	}

}}}
