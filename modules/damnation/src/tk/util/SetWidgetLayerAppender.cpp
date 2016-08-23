#include "SetWidgetLayerAppender.hpp"

namespace redengine {
namespace damnation {
namespace tk {
namespace util {

	SetWidgetLayerAppender::SetWidgetLayerAppender(Layer* layer) : layer(layer) {}

	SetWidgetLayerAppender::SetWidgetLayerAppender(const SetWidgetLayerAppender& appender)
			: Appender<Widget*>(appender), layer(appender.layer) {}

	void SetWidgetLayerAppender::append(Widget *const& widget) {
		widget->setLayer(layer);
	}

}}}}
