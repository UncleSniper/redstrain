#ifndef REDSTRAIN_MOD_BUILD_TRIGGER_HPP
#define REDSTRAIN_MOD_BUILD_TRIGGER_HPP

#include <list>
#include <redstrain/util/ReferenceCounted.hpp>

#include "api.hpp"

namespace redengine {
namespace build {

	class Action;
	class BuildContext;

	class REDSTRAIN_BUILD_API Trigger : public util::ReferenceCounted {

	  private:
		typedef std::list<Action*> Actions;

	  public:
		typedef Actions::const_iterator ActionIterator;

	  private:
		Actions actions;

	  protected:
		virtual bool isTriggered() = 0;
		virtual bool wouldTrigger() = 0;

	  public:
		Trigger();
		Trigger(const Trigger&);
		virtual ~Trigger();

		void addAction(Action*);
		bool removeAction(Action*);
		void clearActions();
		void getActions(ActionIterator&, ActionIterator&) const;

		void spin(BuildContext&);
		void predictSpin(BuildContext&);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_TRIGGER_HPP */
