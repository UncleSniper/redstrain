#include "ProviderInstance.hpp"

using std::string;

namespace redengine {
namespace qu6ntum {

	ProviderInstance::ProviderInstance(ProviderGroup& group, ProviderInstanceID instanceID,
			const string& libraryPath) : group(group), instanceID(instanceID), libraryPath(libraryPath),
			library(libraryPath), instanceState(IS_MAPPED) {}

	ProviderInstance::ProviderInstance(const ProviderInstance& instance)
			: group(instance.group), instanceID(instance.instanceID), libraryPath(instance.libraryPath),
			library(instance.libraryPath), instanceState(instance.instanceState) {}

	ProviderInstance::~ProviderInstance() {
		library.close();
	}

	void ProviderInstance::initialize() {
		//TODO
	}

	void ProviderInstance::decommission() {
		//TODO
	}

}}
