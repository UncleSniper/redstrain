#include "Flavor.hpp"

using std::string;

namespace redengine {
namespace build {

	Flavor::Flavor(const string& name) : name(name) {}

	Flavor::Flavor(const Flavor& flavor) : name(flavor.name) {}

	Flavor& Flavor::operator=(const Flavor& flavor) {
		name = flavor.name;
		return *this;
	}

}}
