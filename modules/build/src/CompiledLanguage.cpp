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

	Component::GenerationHolder* getGenerationTrigger(const string&, const string&,
			const Flavor&, const string&, const Flavor&, Component&);
	Flavor getGeneratedSourceFlavor(const Flavor&, const Flavor&, const string&);
	Flavor getGeneratedHeaderFlavor(const Flavor&, const Flavor&, const string&);
	Component::GenerationHolder* getHeaderExposeTrigger(const string&, const string&,
			const Flavor&, const string&, const Flavor&);

}}
