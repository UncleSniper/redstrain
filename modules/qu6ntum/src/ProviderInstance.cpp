#include "ProviderInstance.hpp"

using std::string;

namespace redengine {
namespace qu6ntum {

	ProviderInstance::ProviderInstance(ProviderGroup& group, ProviderInstanceID instanceID,
			const string& libraryPath) : group(group), instanceID(instanceID), libraryPath(libraryPath),
			library(libraryPath) {}

	ProviderInstance::ProviderInstance(const ProviderInstance& instance)
			: group(instance.group), instanceID(instance.instanceID), libraryPath(instance.libraryPath),
			library(instance.libraryPath) {}

	ProviderInstance::~ProviderInstance() {
		library.close();
	}

}}
