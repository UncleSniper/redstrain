#ifndef REDSTRAIN_MOD_BUILD_BUILDCONTEXT_HPP
#define REDSTRAIN_MOD_BUILD_BUILDCONTEXT_HPP

#include <map>
#include <set>
#include <deque>
#include <string>
#include <redstrain/util/ReferenceCounted.hpp>
#ifdef TESTING_REDSTRAIN_BUILD_API
#include <redstrain/io/streamtypes.hpp>
#endif /* TESTING_REDSTRAIN_BUILD_API */

#include "api.hpp"

namespace redengine {
namespace build {

	class Action;
	class Trigger;
	class BuildUI;
	class ValveGroup;
	class StaticValve;

	class REDSTRAIN_BUILD_API BuildContext {

	  private:
		typedef std::set<Trigger*> Triggers;
		typedef std::deque<Action*> ActionQueue;
		typedef ActionQueue::const_iterator ActionQueueIterator;
		typedef std::set<Action*> ActionSet;
		typedef ActionSet::const_iterator ActionSetIterator;
		typedef std::map<std::string, StaticValve*> Valves;
		typedef Valves::iterator ValveIterator;
		typedef Valves::const_iterator ConstValveIterator;
		typedef std::set<ValveGroup*> Groups;

	  public:
		typedef Triggers::const_iterator TriggerIterator;
		typedef Groups::const_iterator ValveGroupIterator;

	  private:
		BuildUI& ui;
		Triggers triggers;
		ActionQueue actionQueue;
		ActionSet actionSet;
		Valves valves;
		ActionSet alreadyPerformed;
		Groups groups;

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

		StaticValve* getValve(const std::string&) const;
		void addValve(const std::string&, StaticValve*);
		bool removeValve(const std::string&);
		void clearValves();
		StaticValve& getOrMakeValve(const std::string&);

		bool addValveGroup(ValveGroup*);
		bool removeValveGroup(ValveGroup*);
		void clearValveGroups();
		void getValveGroups(ValveGroupIterator&, ValveGroupIterator&) const;
		void forceValveGroups();

		bool wasActionPerformed(Action*) const;
		void clearPerformedActions();

		void spinTriggers();
		void predictTriggers();
		bool queueAction(Action*);
		void performActions();
		void predictActions();

#ifdef TESTING_REDSTRAIN_BUILD_API
		void dumpContext(io::DefaultConfiguredOutputStream<char>::Stream&) const;
#endif /* TESTING_REDSTRAIN_BUILD_API */

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_BUILDCONTEXT_HPP */
