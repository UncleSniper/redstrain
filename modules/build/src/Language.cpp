#include "Language.hpp"

using std::string;

namespace redengine {
namespace build {

	Language::Language(const string& name) : name(name) {}

	Language::Language(const Language& language) : name(language.name) {}

	Language::~Language() {}

	Flavor Language::getShippedSourceFlavor() {
		return Flavor::SHIPPED;
	}

	Flavor Language::getShippedHeaderFlavor() {
		return Flavor::SHIPPED;
	}

	Flavor Language::getHeaderExposeTransformFlavor() {
		return Flavor::HEADER;
	}

	Flavor Language::getCleanFlavor() {
		return Flavor::CLEAN;
	}

}}
