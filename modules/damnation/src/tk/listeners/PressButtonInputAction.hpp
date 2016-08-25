#ifndef REDSTRAIN_MOD_DAMNATION_TK_LISTENERS_PRESSBUTTONINPUTACTION_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_LISTENERS_PRESSBUTTONINPUTACTION_HPP

#include "../Pressable.hpp"
#include "../InputAction.hpp"

namespace redengine {
namespace damnation {
namespace tk {
namespace listeners {

	class REDSTRAIN_DAMNATION_API PressButtonInputAction : public InputAction {

	  private:
		Pressable* button;

	  public:
		PressButtonInputAction(Pressable*);
		PressButtonInputAction(const PressButtonInputAction&);

		inline Pressable* getButton() const {
			return button;
		}

		inline void setButton(Pressable* button) {
			this->button = button;
		}

		virtual void bindingTyped(InputSequenceEvent&);

		static InputAction& getUnboundInstance();

	};

}}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_LISTENERS_PRESSBUTTONINPUTACTION_HPP */
