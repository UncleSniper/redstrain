#include <redstrain/util/PlusAssignAppender.hpp>
#include <redstrain/util/stloperators.hpp>

#include "LeafWidget.hpp"
#include "listeners/ShiftFocusInputAction.hpp"

using redengine::util::MemorySize;
using redengine::util::PlusAssignAppender;
using redengine::damnation::tk::listeners::ShiftFocusInputAction;
using redengine::util::operator+=;

namespace redengine {
namespace damnation {
namespace tk {

	// ======== PendingInputAction ========

	LeafWidget::PendingInputAction::PendingInputAction(InputAction& action,
			const InputAction::InputSequenceEvent& event) : action(&action), event(event) {
		action.ref();
	}

	LeafWidget::PendingInputAction::PendingInputAction(InputAction& action,
			Widget& widget, const InputAction::InputSequenceEvent::KeySequence& keys)
			: action(&action), event(widget, keys) {
		action.ref();
	}

	LeafWidget::PendingInputAction::PendingInputAction(const PendingInputAction& action)
			: action(action.action), event(action.event) {
		this->action->ref();
	}

	LeafWidget::PendingInputAction::~PendingInputAction() {
		action->unref();
	}

	LeafWidget::PendingInputAction& LeafWidget::PendingInputAction::operator=(const PendingInputAction& action) {
		action.action->ref();
		this->action->unref();
		this->action = action.action;
		event.setWidget(action.event.getWidget());
		event.setKeySequence(action.event.getKeySequence());
		return *this;
	}

	// ======== ClearActionQueue ========

	LeafWidget::ClearActionQueue::ClearActionQueue(ActionQueue* queue) : queue(queue) {}

	LeafWidget::ClearActionQueue::ClearActionQueue(const ClearActionQueue& clear) : queue(clear.queue) {}

	LeafWidget::ClearActionQueue::~ClearActionQueue() {
		if(queue)
			queue->clear();
	}

	// ======== LeafWidget ========

	LeafWidget::LeafWidget(const char* className)
			: AbstractWidget(className ? className : "redengine.damnation.tk.LeafWidget"),
			inputState(&keyBindings.getRoot()), lastInputAction(NULL),
			lastInputLength(static_cast<MemorySize>(0u)), currentInputDepth(static_cast<MemorySize>(0u)) {
		addDefaultKeyBindings();
	}

	LeafWidget::LeafWidget(const LeafWidget& widget) : Widget(widget), AbstractWidget(widget),
			inputState(&keyBindings.getRoot()), lastInputAction(NULL),
			lastInputLength(static_cast<MemorySize>(0u)), currentInputDepth(static_cast<MemorySize>(0u)) {
		addDefaultKeyBindings();
	}

	LeafWidget::~LeafWidget() {}

	void LeafWidget::addDefaultKeyBindings() {
		bindKey(KeySym::TAB, ShiftFocusInputAction::getUnboundInstance(false));
		bindKey(KeySym(KeySym::T_BACK_TAB, KeySym::M_NONE, '\0'), ShiftFocusInputAction::getUnboundInstance(true));
	}

	void LeafWidget::bindKey(const InputAction::InputSequenceEvent::KeySequence& keys, InputAction& action) {
		KeyBindings::Node* node = &keyBindings.getRoot();
		InputAction::InputSequenceEvent::KeySequence::const_iterator begin(keys.begin()), end(keys.end());
		for(; begin != end; ++begin)
			node = &node->put(*begin);
		action.ref();
		node->setValue(&action);
		clearKeyBindingState();
	}

	void LeafWidget::bindKey(const KeySym& key, InputAction& action) {
		InputAction::InputSequenceEvent::KeySequence keys;
		keys.push_back(key);
		bindKey(keys, action);
	}

	bool LeafWidget::unbindKey(const InputAction::InputSequenceEvent::KeySequence& keys) {
		KeyBindings::Node* node = &keyBindings.getRoot();
		InputAction::InputSequenceEvent::KeySequence::const_iterator begin(keys.begin()), end(keys.end());
		for(; begin != end; ++begin) {
			node = node->get(*begin);
			if(!node)
				return false;
		}
		return node->remove();
	}

	bool LeafWidget::unbindKey(const KeySym& key) {
		InputAction::InputSequenceEvent::KeySequence keys;
		keys.push_back(key);
		return unbindKey(keys);
	}

	bool LeafWidget::feedKey(const KeySym& key) {
		inputState = inputState->get(key);
		if(inputState) {
			if(inputState->hasValue()) {
				lastInputAction = inputState;
				lastInputLength = currentInputDepth + static_cast<MemorySize>(1u);
			}
			if(inputState->isLeaf())
				inputState = NULL;
		}
		if(inputState)
			return false;
		inputState = &keyBindings.getRoot();
		currentInputDepth = static_cast<MemorySize>(0u);
		if(lastInputAction) {
			InputAction::InputSequenceEvent::KeySequence seq;
			PlusAssignAppender<InputAction::InputSequenceEvent::KeySequence, KeySym> pathSink(seq);
			lastInputAction->getKeyPath(pathSink);
			inputActionQueue.push_back(PendingInputAction(*lastInputAction->getValue(), *this, seq));
			lastInputAction = NULL;
			for(; lastInputLength; --lastInputLength)
				inputQueue.pop_front();
		}
		else {
			lastInputLength = static_cast<MemorySize>(0u);
			const KeySym unbound(inputQueue.front());
			inputQueue.pop_front();
			handleUnboundKey(unbound);
		}
		return true;
	}

	bool LeafWidget::feedInputQueue() {
		KeyQueue::const_iterator begin(inputQueue.begin()), end(inputQueue.end());
		for(; begin != end; ++begin) {
			if(feedKey(*begin))
				return true;
		}
		return false;
	}

	void LeafWidget::clearKeyBindingState() {
		inputState = &keyBindings.getRoot();
		lastInputAction = NULL;
		lastInputLength = currentInputDepth = static_cast<MemorySize>(0u);
		inputQueue.clear();
	}

	void LeafWidget::updateChildrenForNewLayer() {}

	void LeafWidget::clearDescendantColors() {}

	void LeafWidget::processInputLocally(InputListener::InputEvent& event) {
		const KeySym& key = event.getKey();
		if(key.getType() == KeySym::T_NONE || key.getType() == KeySym::T_WINCH)
			return;
		inputQueue.push_back(key);
		if(feedKey(key))
			while(feedInputQueue()) {}
		ClearActionQueue clearActionQueue(&inputActionQueue);
		while(!inputActionQueue.empty()) {
			PendingInputAction pending(inputActionQueue.front());
			inputActionQueue.pop_front();
			pending.action->bindingTyped(pending.event);
		}
		clearActionQueue.queue = NULL;
	}

	void LeafWidget::clearInputState() {
		clearKeyBindingState();
	}

	void LeafWidget::handleUnboundKey(const KeySym&) {}

	void LeafWidget::pack() {}

}}}
