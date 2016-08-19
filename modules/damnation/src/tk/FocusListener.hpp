#ifndef REDSTRAIN_MOD_DAMNATION_TK_FOCUSLISTENER_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_FOCUSLISTENER_HPP

#include "DamnEvent.hpp"
#include "DamnListener.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class Widget;

	class REDSTRAIN_DAMNATION_API FocusListener : public DamnListener {

	  public:
		class REDSTRAIN_DAMNATION_API FocusEvent : public DamnEvent {

		  private:
			Widget* widget;

		  public:
			FocusEvent(Widget&);
			FocusEvent(const FocusEvent&);

			inline Widget& getWidget() const {
				return *widget;
			}

			inline void setWidget(Widget& widget) {
				this->widget = &widget;
			}

		};

	  public:
		FocusListener();
		FocusListener(const FocusListener&);

		virtual void focusGained(FocusEvent&) = 0;
		virtual void focusLost(FocusEvent&) = 0;
		virtual void activeLayerGained(FocusEvent&) = 0;
		virtual void activeLayerLost(FocusEvent&) = 0;

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_FOCUSLISTENER_HPP */
