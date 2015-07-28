#ifndef REDSTRAIN_MOD_BUILD_BUILDCONTEXT_HPP
#define REDSTRAIN_MOD_BUILD_BUILDCONTEXT_HPP

#include <map>
#include <set>
#include <deque>
#include <ctime>
#include <string>
#include <redstrain/util/ReferenceCounted.hpp>
#include <redstrain/io/streamtypes.hpp>

#include "api.hpp"

namespace redengine {
namespace build {

	class Action;
	class Trigger;
	class BuildUI;
	class ValveGroup;
	class StaticValve;
	class FileArtifact;

	class REDSTRAIN_BUILD_API BuildContext {

	  private:
		struct REDSTRAIN_BUILD_API PathPair {

			const std::string directory;
			const std::string basename;

			PathPair(const std::string&, const std::string&);
			PathPair(const PathPair&);

			bool operator==(const PathPair&) const;
			bool operator!=(const PathPair&) const;
			bool operator<(const PathPair&) const;

		};

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
		typedef std::map<PathPair, FileArtifact*> FileArtifacts;
		typedef FileArtifacts::const_iterator FileArtifactIterator;

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
		FileArtifacts fileArtifacts;
		time_t virtualTime;

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

		inline time_t getVirtualTime() const {
			return virtualTime;
		}

		time_t tickVirtualTime();

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
		bool performActions();
		bool predictActions();
		bool definitiveCycle();
		bool predictiveCycle();
		bool definitiveLoop();
		bool predictiveLoop();

		FileArtifact* internFileArtifact(const std::string&, const std::string&);

		void dumpContext(io::DefaultConfiguredOutputStream<char>::Stream&) const;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_BUILDCONTEXT_HPP */
