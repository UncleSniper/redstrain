#include "ClearWidgetCachedColorsAppender.hpp"

namespace redengine {
namespace damnation {
namespace tk {
namespace util {

	ClearWidgetCachedColorsAppender::ClearWidgetCachedColorsAppender() {}

	ClearWidgetCachedColorsAppender::ClearWidgetCachedColorsAppender(const ClearWidgetCachedColorsAppender& appender)
			: Appender<Widget*>(appender) {}

	void ClearWidgetCachedColorsAppender::append(Widget *const& widget) {
		widget->clearCachedColors();
	}

}}}}
