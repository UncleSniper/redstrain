#include <redstrain/util/Unref.hpp>
#include <redstrain/util/StringUtils.hpp>

#include "Messages16CompileTransform.hpp"
#include "Messages16DefinitionLanguage.hpp"

using std::string;
using redengine::util::Unref;
using redengine::util::StringUtils;

namespace redengine {
namespace build {

	Messages16DefinitionLanguage::Messages16DefinitionLanguage()
			: FileConversionLanguage("localized messages definition") {}

	Messages16DefinitionLanguage::Messages16DefinitionLanguage(const Messages16DefinitionLanguage& language)
			: FileConversionLanguage(language) {}

	string Messages16DefinitionLanguage::getTargetBasename(const string& sourceBasename,
			const Flavor&, const Flavor&, Component&) {
		string targetBasename(StringUtils::endsWith(sourceBasename, ".msgdf")
				? sourceBasename.substr(static_cast<string::size_type>(0u),
				sourceBasename.length() - static_cast<string::size_type>(6u))
				: sourceBasename);
		return targetBasename + ".rlmg2";
	}

	Transform* Messages16DefinitionLanguage::getConversionTransform(FileArtifact& sourceArtifact,
			const Flavor&, FileArtifact&, const Flavor&, const Flavor&, Component& component,
			BuildContext& context) {
		Unref<Messages16CompileTransform> transform(new Messages16CompileTransform(sourceArtifact));
		transform->setSpecificationArtifact(component.getSourceFile("messages.msgsc", context));
		return transform.set();
	}

	static const char *const SOURCE_SUFFIXES[] = {
		".msgdf",
		NULL
	};

	Language::ArtifactType Messages16DefinitionLanguage::classifyFile(const string& path) {
		return Language::classifyFileBySuffix(path, SOURCE_SUFFIXES, NULL);
	}

}}
