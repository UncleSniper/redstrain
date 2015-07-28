#include "Flavor.hpp"

using std::string;

namespace redengine {
namespace build {

	const Flavor Flavor::SHIPPED("shipped");
	const Flavor Flavor::CLEAN("clean");
	const Flavor Flavor::STATIC("static");
	const Flavor Flavor::DYNAMIC("dynamic");
	const Flavor Flavor::HEADER("header");
	const Flavor Flavor::GENERIC("generic");

	Flavor::Flavor(const string& name) : name(name) {}

	Flavor::Flavor(const Flavor& flavor) : name(flavor.name) {}

	Flavor& Flavor::operator=(const Flavor& flavor) {
		name = flavor.name;
		return *this;
	}

	bool Flavor::operator==(const Flavor& flavor) const {
		return name == flavor.name;
	}

}}
