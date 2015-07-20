#include "CompiledLanguage.hpp"

using std::string;
using redengine::util::Appender;

namespace redengine {
namespace build {

	CompiledLanguage::CompiledLanguage(const string& name) : Language(name) {}

	CompiledLanguage::CompiledLanguage(const CompiledLanguage& language) : Language(language) {}

	static const Flavor *const FLAVORS[] = {
		&Flavor::STATIC,
		&Flavor::DYNAMIC,
		NULL
	};

	void getSupportedFlavors(Component::Type, Appender<Flavor>& sink) {
		Language::sinkSupportedFlavors(FLAVORS, sink);
	}

	bool isOneToOne(const Flavor&) {
		return true;
	}

	/*
	Component::GenerationHolder* getGenerationTrigger(const string& sourceDirectory, const string& sourceBasename,
			const Flavor& sourceFlavor, const string& targetDirectory, const Flavor& transformFlavor,
			Component& component) {
		//TODO
	}
	*/

	Flavor getGeneratedSourceFlavor(const Flavor&, const Flavor& transformFlavor, const string&) {
		return transformFlavor;
	}

	Flavor getGeneratedHeaderFlavor(const Flavor&, const Flavor& transformFlavor, const string&) {
		return transformFlavor;
	}

	/*
	Component::GenerationHolder* getHeaderExposeTrigger(const string& sourceDirectory, const string& sourceBasename,
			const Flavor&, const string& targetDirectory, const Flavor&) {
		//TODO
	}
	*/

}}
