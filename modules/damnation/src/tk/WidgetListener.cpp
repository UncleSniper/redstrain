#include "WidgetListener.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	// ======== WidgetEvent ========

	WidgetListener::WidgetEvent::WidgetEvent(Widget& widget) : widget(&widget) {}

	WidgetListener::WidgetEvent::WidgetEvent(const WidgetEvent& event) : DamnEvent(event), widget(event.widget) {}

	// ======== ReparentEvent ========

	WidgetListener::ReparentEvent::ReparentEvent(Widget& child, Widget* oldParent, Widget* newParent)
			: WidgetEvent(child), oldParent(oldParent), newParent(newParent) {}

	WidgetListener::ReparentEvent::ReparentEvent(const ReparentEvent& event)
			: WidgetEvent(event), oldParent(event.oldParent), newParent(event.newParent) {}

	// ======== RelayerEvent ========

	WidgetListener::RelayerEvent::RelayerEvent(Widget& widget, Layer* oldLayer, Layer* newLayer)
			: WidgetEvent(widget), oldLayer(oldLayer), newLayer(newLayer) {}

	WidgetListener::RelayerEvent::RelayerEvent(const RelayerEvent& event)
			: WidgetEvent(event), oldLayer(event.oldLayer), newLayer(event.newLayer) {}

	// ======== RepositionEvent ========

	WidgetListener::RepositionEvent::RepositionEvent(Widget& widget, const Point& oldPosition,
			const Point& newPosition) : WidgetEvent(widget), oldPosition(oldPosition), newPosition(newPosition) {}

	WidgetListener::RepositionEvent::RepositionEvent(const RepositionEvent& event)
			: WidgetEvent(event), oldPosition(event.oldPosition), newPosition(event.newPosition) {}

	void WidgetListener::RepositionEvent::setOldPosition(const Point& oldPosition) {
		this->oldPosition = oldPosition;
	}

	void WidgetListener::RepositionEvent::setNewPosition(const Point& newPosition) {
		this->newPosition = newPosition;
	}

	// ======== ResizeEvent ========

	WidgetListener::ResizeEvent::ResizeEvent(Widget& widget, const Size& oldSize, const Size& newSize)
			: WidgetEvent(widget), oldSize(oldSize), newSize(newSize) {}

	WidgetListener::ResizeEvent::ResizeEvent(const ResizeEvent& event)
			: WidgetEvent(event), oldSize(event.oldSize), newSize(event.newSize) {}

	void WidgetListener::ResizeEvent::setOldSize(const Size& oldSize) {
		this->oldSize = oldSize;
	}

	void WidgetListener::ResizeEvent::setNewSize(const Size& newSize) {
		this->newSize = newSize;
	}

	// ======== RethemeEvent ========

	WidgetListener::RethemeEvent::RethemeEvent(Widget& widget, Theme* oldTheme, Theme* newTheme)
			: WidgetEvent(widget), oldTheme(oldTheme), newTheme(newTheme) {}

	WidgetListener::RethemeEvent::RethemeEvent(const RethemeEvent& event)
			: WidgetEvent(event), oldTheme(event.oldTheme), newTheme(event.newTheme) {}

	// ======== WidgetListener ========

	WidgetListener::WidgetListener() {}

	WidgetListener::WidgetListener(const WidgetListener& listener) : DamnListener(listener) {}

}}}
