#include "FocusListener.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	// ======== FocusEvent ========

	FocusListener::FocusEvent::FocusEvent(Widget& widget) : widget(&widget) {}

	FocusListener::FocusEvent::FocusEvent(const FocusEvent& event) : DamnEvent(event), widget(event.widget) {}

	// ======== FocusListener ========

	FocusListener::FocusListener() {}

	FocusListener::FocusListener(const FocusListener& listener) : DamnListener(listener) {}

}}}
