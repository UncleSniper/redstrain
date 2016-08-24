#ifndef REDSTRAIN_MOD_DAMNATION_TK_COMMANDLISTENER_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_COMMANDLISTENER_HPP

#include "DamnEvent.hpp"
#include "DamnListener.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class Widget;

	template<typename CommandT>
	class CommandListener : public DamnListener {

	  public:
		typedef CommandT Command;

	  public:
		class CommandEvent : public DamnEvent {

		  private:
			Widget* widget;
			CommandT command;

		  public:
			CommandEvent(Widget& widget, const CommandT& command) : widget(&widget), command(command) {}

			CommandEvent(const CommandEvent& event)
					: DamnEvent(event), widget(event.widget), command(event.command) {}

			virtual ~CommandEvent() {}

			inline Widget& getWidget() const {
				return *widget;
			}

			inline void setWidget(Widget& widget) {
				this->widget = &widget;
			}

			inline CommandT& getCommand() {
				return command;
			}

			inline const CommandT& getCommand() const {
				return command;
			}

		};

	  public:
		CommandListener() {}
		CommandListener(const CommandListener& listener) : DamnListener(listener) {}

		virtual void commandIssued(CommandEvent&) = 0;

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_COMMANDLISTENER_HPP */
