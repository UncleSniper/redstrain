#ifndef REDSTRAIN_MOD_BUILD_REQUISITION_HPP
#define REDSTRAIN_MOD_BUILD_REQUISITION_HPP

#include "Trigger.hpp"

namespace redengine {
namespace build {

	class Requisition : public Trigger {

	  private:
		bool required, wouldBeRequired;

	  protected:
		virtual bool isTriggered();
		virtual bool wouldTrigger();

	  public:
		Requisition();
		Requisition(const Requisition&);

		void require();
		void wouldRequire();

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_REQUISITION_HPP */
