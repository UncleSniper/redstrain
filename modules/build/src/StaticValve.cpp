#include <redstrain/io/streamoperators.hpp>

#include "StaticValve.hpp"

using redengine::io::DefaultConfiguredOutputStream;
using redengine::io::endln;
using redengine::io::shift;
using redengine::io::indent;
using redengine::io::unshift;
using redengine::io::operator<<;

namespace redengine {
namespace build {

	StaticValve::StaticValve() : open(false) {}

	StaticValve::StaticValve(const StaticValve& valve) : Valve(valve), open(valve.open) {}

	bool StaticValve::isOpen(BuildContext&) {
		return open;
	}

	void StaticValve::dumpValve(DefaultConfiguredOutputStream<char>::Stream& stream) const {
		stream << indent << "StaticValve " << this << endln;
	}

}}
