#include "ObjectFileLanguage.hpp"

using std::string;

namespace redengine {
namespace build {

	ObjectFileLanguage::ObjectFileLanguage(const string& name) : Language(name) {}

	ObjectFileLanguage::ObjectFileLanguage(const ObjectFileLanguage& language) : Language(language) {}

	bool ObjectFileLanguage::isOneToOne(const Flavor&) {
		return false;
	}

	FileArtifact* ObjectFileLanguage::getHeaderExposeTransform(BuildContext&, const string&, FileArtifact&,
			const Flavor&, const string&, Component&, BuildArtifactMapper&, Flavor&) {
		return NULL;
	}

}}
