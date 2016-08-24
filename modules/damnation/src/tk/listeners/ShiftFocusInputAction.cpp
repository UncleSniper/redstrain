#include "ShiftFocusInputAction.hpp"
#include "../Widget.hpp"

namespace redengine {
namespace damnation {
namespace tk {
namespace listeners {

	ShiftFocusInputAction::ShiftFocusInputAction(Layer* layer, bool backward) : layer(layer), backward(backward) {}

	ShiftFocusInputAction::ShiftFocusInputAction(const ShiftFocusInputAction& action)
			: InputAction(action), layer(action.layer), backward(action.backward) {}

	void ShiftFocusInputAction::bindingTyped(InputSequenceEvent& event) {
		Layer* l = layer ? layer : event.getWidget().getLayer();
		if(l) {
			if(backward)
				l->unshiftFocus();
			else
				l->shiftFocus();
		}
	}

	static ShiftFocusInputAction defaultForward(NULL, false), defaultBackward(NULL, true);

	InputAction& ShiftFocusInputAction::getUnboundInstance(bool backward) {
		return backward ? defaultBackward : defaultForward;
	}

}}}}
