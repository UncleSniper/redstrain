#ifndef REDSTRAIN_MOD_BUILD_BUILDCONTEXT_HPP
#define REDSTRAIN_MOD_BUILD_BUILDCONTEXT_HPP

#include <set>
#include <deque>
#include <redstrain/util/ReferenceCounted.hpp>

#include "api.hpp"

namespace redengine {
namespace build {

	class Action;
	class Trigger;

	class REDSTRAIN_BUILD_API BuildContext {

	  private:
		typedef std::set<Trigger*> Triggers;
		typedef std::deque<Action*> ActionQueue;
		typedef ActionQueue::const_iterator ActionQueueIterator;
		typedef std::set<Action*> ActionSet;
		typedef ActionSet::const_iterator ActionSetIterator;

	  public:
		typedef Triggers::const_iterator TriggerIterator;

	  private:
		Triggers triggers;
		ActionQueue actionQueue;
		ActionSet actionSet;

	  private:
		BuildContext(const BuildContext&);

	  public:
		BuildContext();
		~BuildContext();

		bool addTrigger(Trigger*);
		bool removeTrigger(Trigger*);
		void clearTriggers();
		void getTriggers(TriggerIterator&, TriggerIterator&) const;

		void spinTriggers();
		void predictTriggers();
		bool queueAction(Action*);
		void performActions();
		void predictActions();

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_BUILDCONTEXT_HPP */
