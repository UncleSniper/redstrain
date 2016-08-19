#include "InputListener.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	// ======== InputEvent ========

	InputListener::InputEvent::InputEvent(Widget* widget, const KeySym& key) : widget(widget), key(key) {}

	InputListener::InputEvent::InputEvent(const InputEvent& event)
			: DamnEvent(event), widget(event.widget), key(event.key) {}

	void InputListener::InputEvent::setKey(const KeySym& key) {
		this->key = key;
	}

	// ======== InputListener ========

	InputListener::InputListener() {}

	InputListener::InputListener(const InputListener& listener) : DamnListener(listener) {}

}}}
