#include "Requisition.hpp"

namespace redengine {
namespace build {

	Requisition::Requisition() : required(false), wouldBeRequired(false) {}

	Requisition::Requisition(const Requisition& requisition)
			: Trigger(requisition), required(requisition.required), wouldBeRequired(requisition.wouldBeRequired) {}

	void Requisition::require() {
		required = true;
	}

	void Requisition::wouldRequire() {
		wouldBeRequired = true;
	}

	bool Requisition::isTriggered() {
		bool r = required;
		required = false;
		return r;
	}

	bool Requisition::wouldTrigger() {
		bool r = wouldBeRequired;
		wouldBeRequired = false;
		return r;
	}

}}
