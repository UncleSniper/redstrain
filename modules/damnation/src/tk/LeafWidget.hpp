#ifndef REDSTRAIN_MOD_DAMNATION_TK_LEAFWIDGET_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_LEAFWIDGET_HPP

#include <deque>
#include <redstrain/algorithm/SequenceMap.hpp>

#include "InputAction.hpp"
#include "AbstractWidget.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class REDSTRAIN_DAMNATION_API LeafWidget : public AbstractWidget {

	  private:
		struct PendingInputAction {

			InputAction* action;
			InputAction::InputSequenceEvent event;

			PendingInputAction(InputAction&, const InputAction::InputSequenceEvent&);
			PendingInputAction(InputAction&, Widget&, const InputAction::InputSequenceEvent::KeySequence&);
			PendingInputAction(const PendingInputAction&);
			~PendingInputAction();

			PendingInputAction& operator=(const PendingInputAction&);

		};

	  private:
		typedef algorithm::SequenceMap<
			KeySym,
			InputAction*,
			void,
			util::unrefDestructor<InputAction>
		> KeyBindings;
		typedef std::deque<KeySym> KeyQueue;
		typedef std::deque<PendingInputAction> ActionQueue;

	  private:
		struct ClearActionQueue {

			ActionQueue* queue;

			ClearActionQueue(ActionQueue*);
			ClearActionQueue(const ClearActionQueue&);
			~ClearActionQueue();

		};

	  private:
		KeyBindings keyBindings;
		KeyBindings::Node *inputState, *lastInputAction;
		util::MemorySize lastInputLength, currentInputDepth;
		KeyQueue inputQueue;
		ActionQueue inputActionQueue;

	  private:
		void addDefaultKeyBindings();
		bool feedKey(const KeySym&);
		bool feedInputQueue();

	  protected:
		void clearKeyBindingState();

		virtual void updateChildrenForNewLayer();
		virtual void clearDescendantColors();

		virtual void processInputLocally(InputListener::InputEvent&);
		virtual void clearInputState();

		virtual void handleUnboundKey(const KeySym&);

	  public:
		LeafWidget(const char* = NULL);
		LeafWidget(const LeafWidget&);
		virtual ~LeafWidget();

		virtual void pack();

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_LEAFWIDGET_HPP */
