#include "Manifest.hpp"

using redengine::text::String16;

namespace redengine {
namespace qu6ntum {

	Manifest::Manifest() {}

	Manifest::Manifest(const Manifest& manifest) : requiredProviders(manifest.requiredProviders) {}

	void Manifest::getRequiredProviders(NameIterator& begin, NameIterator& end) const {
		begin = requiredProviders.begin();
		end = requiredProviders.end();
	}

	void Manifest::addRequiredProvider(const String16& providerName) {
		requiredProviders.push_back(providerName);
	}

	void Manifest::clearRequiredProviders() {
		requiredProviders.clear();
	}

}}
