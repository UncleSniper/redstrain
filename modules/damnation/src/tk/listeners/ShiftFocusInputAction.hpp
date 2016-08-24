#ifndef REDSTRAIN_MOD_DAMNATION_TK_LISTENERS_SHIFTFOCUSINPUTACTION_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_LISTENERS_SHIFTFOCUSINPUTACTION_HPP

#include "../Layer.hpp"
#include "../InputAction.hpp"

namespace redengine {
namespace damnation {
namespace tk {
namespace listeners {

	class REDSTRAIN_DAMNATION_API ShiftFocusInputAction : public InputAction {

	  private:
		Layer* layer;
		bool backward;

	  public:
		ShiftFocusInputAction(Layer*, bool);
		ShiftFocusInputAction(const ShiftFocusInputAction&);

		inline Layer* getLayer() const {
			return layer;
		}

		inline void setLayer(Layer* layer) {
			this->layer = layer;
		}

		inline bool isBackward() const {
			return backward;
		}

		inline void setBackward(bool backward) {
			this->backward = backward;
		}

		virtual void bindingTyped(InputSequenceEvent&);

		static InputAction& getUnboundInstance(bool);

	};

}}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_LISTENERS_SHIFTFOCUSINPUTACTION_HPP */
