#include "StaticValve.hpp"

namespace redengine {
namespace build {

	StaticValve::StaticValve() : open(false) {}

	StaticValve::StaticValve(const StaticValve& valve) : Valve(valve), open(valve.open) {}

	bool StaticValve::isOpen(BuildContext&) {
		return open;
	}

}}
