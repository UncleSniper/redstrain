#ifndef REDSTRAIN_MOD_BUILD_STRATUMVALVE_HPP
#define REDSTRAIN_MOD_BUILD_STRATUMVALVE_HPP

#include <set>

#include "Valve.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API StratumValve : public Valve {

	  private:
		typedef std::set<Action*> ActionSet;

	  public:
		typedef ActionSet::const_iterator ActionIterator;

	  private:
		ActionSet actions;

	  public:
		StratumValve();
		StratumValve(const StratumValve&);
		virtual ~StratumValve();

		bool addAction(Action*);
		bool removeAction(Action*);
		void clearActions();
		void getActions(ActionIterator&, ActionIterator&) const;

		virtual bool isOpen(BuildContext&);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_STRATUMVALVE_HPP */
