#include "InputAction.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	// ======== InputSequenceEvent ========

	InputAction::InputSequenceEvent::InputSequenceEvent(Widget& widget, const KeySequence& keys)
			: widget(&widget), keys(keys) {}

	InputAction::InputSequenceEvent::InputSequenceEvent(const InputSequenceEvent& event)
			: DamnEvent(event), widget(event.widget), keys(event.keys) {}

	InputAction::InputSequenceEvent::~InputSequenceEvent() {}

	void InputAction::InputSequenceEvent::getKeySequence(KeySequence::iterator& begin, KeySequence::iterator& end) {
		begin = keys.begin();
		end = keys.end();
	}

	void InputAction::InputSequenceEvent::getKeySequence(KeySequence::const_iterator& begin,
			KeySequence::const_iterator& end) const {
		begin = keys.begin();
		end = keys.end();
	}

	void InputAction::InputSequenceEvent::setKeySequence(const KeySequence& keys) {
		this->keys = keys;
	}

	void InputAction::InputSequenceEvent::addKey(const KeySym& key) {
		keys.push_back(key);
	}

	void InputAction::InputSequenceEvent::clearKeySequence() {
		keys.clear();
	}

	// ======== InputAction ========

	InputAction::InputAction() {}

	InputAction::InputAction(const InputAction& action) : DamnListener(action) {}

}}}
