#ifdef TESTING_REDSTRAIN_BUILD_API
#include <redstrain/io/streamoperators.hpp>
#endif /* TESTING_REDSTRAIN_BUILD_API */

#include "StaticValve.hpp"

#ifdef TESTING_REDSTRAIN_BUILD_API
using redengine::io::DefaultConfiguredOutputStream;
using redengine::io::endln;
using redengine::io::shift;
using redengine::io::indent;
using redengine::io::unshift;
using redengine::io::operator<<;
#endif /* TESTING_REDSTRAIN_BUILD_API */

namespace redengine {
namespace build {

	StaticValve::StaticValve() : open(false) {}

	StaticValve::StaticValve(const StaticValve& valve) : Valve(valve), open(valve.open) {}

	bool StaticValve::isOpen(BuildContext&) {
		return open;
	}

#ifdef TESTING_REDSTRAIN_BUILD_API
	void StaticValve::dumpValve(DefaultConfiguredOutputStream<char>::Stream& stream) const {
		stream << indent << "StaticValve " << this << endln;
	}
#endif /* TESTING_REDSTRAIN_BUILD_API */

}}
