#ifndef REDSTRAIN_MOD_DAMNATION_TK_INPUTACTION_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_INPUTACTION_HPP

#include <list>

#include "DamnEvent.hpp"
#include "DamnListener.hpp"
#include "../KeySym.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class Widget;

	class REDSTRAIN_DAMNATION_API InputAction : public DamnListener {

	  public:
		class REDSTRAIN_DAMNATION_API InputSequenceEvent : public DamnEvent {

		  public:
			typedef std::list<KeySym> KeySequence;

		  private:
			Widget* widget;
			KeySequence keys;

		  public:
			InputSequenceEvent(Widget&, const KeySequence& = KeySequence());
			InputSequenceEvent(const InputSequenceEvent&);
			virtual ~InputSequenceEvent();

			inline Widget& getWidget() const {
				return *widget;
			}

			inline void setWidget(Widget& widget) {
				this->widget = &widget;
			}

			inline KeySequence& getKeySequence() {
				return keys;
			}

			inline const KeySequence& getKeySequence() const {
				return keys;
			}

			void getKeySequence(KeySequence::iterator&, KeySequence::iterator&);
			void getKeySequence(KeySequence::const_iterator&, KeySequence::const_iterator&) const;
			void setKeySequence(const KeySequence&);
			void addKey(const KeySym&);
			void clearKeySequence();

		};

	  public:
		InputAction();
		InputAction(const InputAction&);

		virtual void bindingTyped(InputSequenceEvent&) = 0;

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_INPUTACTION_HPP */
