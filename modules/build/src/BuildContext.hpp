#ifndef REDSTRAIN_MOD_BUILD_BUILDCONTEXT_HPP
#define REDSTRAIN_MOD_BUILD_BUILDCONTEXT_HPP

#include <map>
#include <string>
#include <redstrain/util/MapKeyIterator.hpp>
#include <redstrain/io/streamtypes.hpp>

#include "api.hpp"

namespace redengine {
namespace build {

	class Goal;
	class BuildUI;

	class REDSTRAIN_BUILD_API BuildContext {

	  private:
		typedef std::map<std::string, Goal*> Goals;
		typedef Goals::iterator GoalIterator;
		typedef Goals::const_iterator ConstGoalIterator;

	  public:
		typedef util::MapKeyIterator<std::string, Goal*> GoalNameIterator;

	  private:
		BuildUI& ui;
		time_t virtualTime;
		Goals goals;

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

		Goal* getGoal(const std::string&) const;
		bool addGoal(const std::string&, Goal&);
		bool removeGoal(const std::string&);
		void clearGoals();
		void getGoals(GoalNameIterator&, GoalNameIterator&) const;

		void dumpContext(io::DefaultConfiguredOutputStream<char>::Stream&) const;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_BUILDCONTEXT_HPP */
