#include "Flavor.hpp"

using std::string;

namespace redengine {
namespace build {

	const Flavor Flavor::SHIPPED("shipped");
	const Flavor Flavor::CLEAN("clean");
	const Flavor Flavor::STATIC("static");
	const Flavor Flavor::DYNAMIC("dynamic");

	Flavor::Flavor(const string& name) : name(name) {}

	Flavor::Flavor(const Flavor& flavor) : name(flavor.name) {}

	Flavor& Flavor::operator=(const Flavor& flavor) {
		name = flavor.name;
		return *this;
	}

}}
