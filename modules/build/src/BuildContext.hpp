#ifndef REDSTRAIN_MOD_BUILD_BUILDCONTEXT_HPP
#define REDSTRAIN_MOD_BUILD_BUILDCONTEXT_HPP

#include <map>
#include <set>
#include <deque>
#include <string>
#include <redstrain/util/ReferenceCounted.hpp>

#include "api.hpp"

namespace redengine {
namespace build {

	class Valve;
	class Action;
	class Trigger;
	class BuildUI;

	class REDSTRAIN_BUILD_API BuildContext {

	  private:
		typedef std::set<Trigger*> Triggers;
		typedef std::deque<Action*> ActionQueue;
		typedef ActionQueue::const_iterator ActionQueueIterator;
		typedef std::set<Action*> ActionSet;
		typedef ActionSet::const_iterator ActionSetIterator;
		typedef std::map<std::string, Valve*> Valves;
		typedef Valves::iterator ValveIterator;
		typedef Valves::const_iterator ConstValveIterator;

	  public:
		typedef Triggers::const_iterator TriggerIterator;

	  private:
		BuildUI& ui;
		Triggers triggers;
		ActionQueue actionQueue;
		ActionSet actionSet;
		Valves valves;

	  private:
		BuildContext(const BuildContext&);

	  public:
		BuildContext(BuildUI&);
		~BuildContext();

		inline BuildUI& getUI() {
			return ui;
		}

		inline const BuildUI& getUI() const {
			return ui;
		}

		bool addTrigger(Trigger*);
		bool removeTrigger(Trigger*);
		void clearTriggers();
		void getTriggers(TriggerIterator&, TriggerIterator&) const;

		Valve* getValve(const std::string&) const;
		void addValve(const std::string&, Valve*);
		bool removeValve(const std::string&);
		void clearValves();

		void spinTriggers();
		void predictTriggers();
		bool queueAction(Action*);
		void performActions();
		void predictActions();

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_BUILDCONTEXT_HPP */
