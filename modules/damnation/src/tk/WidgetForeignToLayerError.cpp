#include "WidgetForeignToLayerError.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	WidgetForeignToLayerError::WidgetForeignToLayerError() {}

	WidgetForeignToLayerError::WidgetForeignToLayerError(const WidgetForeignToLayerError& error)
			: Error(error), DamnError(error), IllegalArgumentError(error) {}

	REDSTRAIN_DEFINE_ERROR(WidgetForeignToLayerError) {
		out << "Widget is foreign to layer";
	}

}}}
