#include "PressButtonInputAction.hpp"

namespace redengine {
namespace damnation {
namespace tk {
namespace listeners {

	PressButtonInputAction::PressButtonInputAction(Pressable* button) : button(button) {}

	PressButtonInputAction::PressButtonInputAction(const PressButtonInputAction& action)
			: InputAction(action), button(action.button) {}

	void PressButtonInputAction::bindingTyped(InputSequenceEvent& event) {
		if(button)
			button->press();
		else {
			Pressable* pressable = dynamic_cast<Pressable*>(&event.getWidget());
			if(pressable)
				pressable->press();
		}
	}

	static PressButtonInputAction defaultInstance(NULL);

	InputAction& PressButtonInputAction::getUnboundInstance() {
		return defaultInstance;
	}

}}}}
