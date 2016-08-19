#ifndef REDSTRAIN_MOD_DAMNATION_TK_INPUTLISTENER_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_INPUTLISTENER_HPP

#include "DamnEvent.hpp"
#include "DamnListener.hpp"
#include "../KeySym.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class Widget;

	class REDSTRAIN_DAMNATION_API InputListener : public DamnListener {

	  public:
		class REDSTRAIN_DAMNATION_API InputEvent : public DamnEvent {

		  private:
			Widget* widget;
			KeySym key;

		  public:
			InputEvent(Widget*, const KeySym&);
			InputEvent(const InputEvent&);

			inline Widget* getWidget() const {
				return widget;
			}

			inline void setWidget(Widget* widget) {
				this->widget = widget;
			}

			inline KeySym& getKey() {
				return key;
			}

			inline const KeySym& getKey() const {
				return key;
			}

			void setKey(const KeySym&);

		};

	  public:
		InputListener();
		InputListener(const InputListener&);

		virtual bool keyTyped(InputEvent&) = 0;

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_INPUTLISTENER_HPP */
