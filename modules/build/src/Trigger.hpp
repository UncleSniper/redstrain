#ifndef REDSTRAIN_MOD_BUILD_TRIGGER_HPP
#define REDSTRAIN_MOD_BUILD_TRIGGER_HPP

#include <set>
#include <list>
#include <redstrain/util/ReferenceCounted.hpp>
#ifdef TESTING_REDSTRAIN_BUILD_API
#include <redstrain/io/streamtypes.hpp>
#endif /* TESTING_REDSTRAIN_BUILD_API */

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
		bool areValvesOpen(BuildContext&) const;

	  protected:
#ifdef TESTING_REDSTRAIN_BUILD_API
		void dumpTriggerAspects(io::DefaultConfiguredOutputStream<char>::Stream&) const;
#endif /* TESTING_REDSTRAIN_BUILD_API */

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

#ifdef TESTING_REDSTRAIN_BUILD_API
		virtual void dumpTrigger(io::DefaultConfiguredOutputStream<char>::Stream&) const = 0;
#endif /* TESTING_REDSTRAIN_BUILD_API */

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_TRIGGER_HPP */
