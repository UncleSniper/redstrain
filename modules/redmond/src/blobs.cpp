#include <set>

#include "blobs.hpp"

using std::set;
using std::string;

namespace redengine {
namespace redmond {

	static set<string> presentBlobs;

	REDSTRAIN_REDMOND_API void registerBlobModule(const string& name) {
		presentBlobs.insert(name);
	}

	REDSTRAIN_REDMOND_API bool hasBlobModule(const string& name) {
		return presentBlobs.find(name) != presentBlobs.end();
	}

	BlobModuleRegistrar::BlobModuleRegistrar(const string& name) : name(name) {
		registerBlobModule(name);
	}

	BlobModuleRegistrar::BlobModuleRegistrar(const BlobModuleRegistrar& registrar) : name(registrar.name) {}

}}
