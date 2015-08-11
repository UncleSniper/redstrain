#ifndef REDSTRAIN_MOD_BUILD_MULTIGOAL_HPP
#define REDSTRAIN_MOD_BUILD_MULTIGOAL_HPP

#include <redstrain/util/UniqueList.hpp>

#include "Goal.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API MultiGoal : public Goal {

	  private:
		typedef util::UniqueList<Goal*> Goals;

	  public:
		typedef Goals::Iterator GoalIterator;

	  private:
		Goals goals;

	  public:
		MultiGoal();
		MultiGoal(const MultiGoal&);
		virtual ~MultiGoal();

		bool addGoal(Goal&);
		bool removeGoal(Goal&);
		void clearGoals();
		void getGoals(GoalIterator&, GoalIterator&) const;

		virtual void attain(BuildContext&);
		virtual void wouldAttain(BuildContext&);

		virtual void dumpGoal(io::DefaultConfiguredOutputStream<char>::Stream&) const;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_MULTIGOAL_HPP */
