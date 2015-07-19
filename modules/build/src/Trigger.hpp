#ifndef REDSTRAIN_MOD_BUILD_TRIGGER_HPP
#define REDSTRAIN_MOD_BUILD_TRIGGER_HPP

#include <set>
#include <list>
#include <redstrain/util/ReferenceCounted.hpp>

#include "api.hpp"

namespace redengine {
namespace build {

	class Valve;
	class Action;
	class BuildContext;

	class REDSTRAIN_BUILD_API Trigger : public util::ReferenceCounted {

	  private:
		typedef std::list<Action*> Actions;
		typedef std::set<Valve*> Valves;

	  public:
		typedef Actions::const_iterator ActionIterator;
		typedef Valves::const_iterator ValveIterator;

	  private:
		Actions actions;
		Valves valves;

	  private:
		bool areValvesOpen() const;

	  protected:
		virtual bool isTriggered(BuildContext&) = 0;
		virtual bool wouldTrigger(BuildContext&) = 0;

	  public:
		Trigger();
		Trigger(const Trigger&);
		virtual ~Trigger();

		void addAction(Action*);
		bool removeAction(Action*);
		void clearActions();
		void getActions(ActionIterator&, ActionIterator&) const;

		void addValve(Valve*);
		bool removeValve(Valve*);
		void clearValves();
		void getValves(ValveIterator&, ValveIterator&) const;

		void spin(BuildContext&);
		void predictSpin(BuildContext&);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_TRIGGER_HPP */
