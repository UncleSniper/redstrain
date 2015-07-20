#include <redstrain/util/StringUtils.hpp>
#include <redstrain/platform/Pathname.hpp>

#include "Language.hpp"

using std::string;
using redengine::util::Appender;
using redengine::util::StringUtils;
using redengine::platform::Pathname;

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

	Language::ArtifactType Language::classifyFileBySuffix(const string& path,
			const char *const* sourceSuffixes, const char *const* headerSuffixes) {
		string bn(Pathname::basename(path));
		string::size_type bnlen = bn.length();
		for(; *sourceSuffixes; ++sourceSuffixes) {
			string suffix(*sourceSuffixes);
			if(bnlen > suffix.length() && StringUtils::endsWith(bn, suffix))
				return AT_SOURCE;
		}
		for(; *headerSuffixes; ++headerSuffixes) {
			string suffix(*headerSuffixes);
			if(bnlen > suffix.length() && StringUtils::endsWith(bn, suffix))
				return AT_HEADER;
		}
		return AT_NONE;
	}

	void Language::sinkSupportedFlavors(const Flavor *const* flavors, Appender<Flavor>& sink) {
		for(; *flavors; ++flavors)
			sink.append(**flavors);
		sink.doneAppending();
	}

}}
