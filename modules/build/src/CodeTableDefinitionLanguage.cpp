#include <redstrain/util/Unref.hpp>
#include <redstrain/util/StringUtils.hpp>

#include "FileArtifact.hpp"
#include "CodeTableDefinitionLanguage.hpp"
#include "CodeTableCompileGenerationAction.hpp"

using std::string;
using redengine::util::Unref;
using redengine::util::Appender;
using redengine::util::StringUtils;

namespace redengine {
namespace build {

	CodeTableDefinitionLanguage::CodeTableDefinitionLanguage() : FileConversionLanguage("code table definition") {}

	CodeTableDefinitionLanguage::CodeTableDefinitionLanguage(const CodeTableDefinitionLanguage& language)
			: FileConversionLanguage(language) {}

	static const char *const SOURCE_SUFFIXES[] = {
		".ctdef",
		NULL
	};

	Language::ArtifactType CodeTableDefinitionLanguage::classifyFile(const string& path) {
		return Language::classifyFileBySuffix(path, SOURCE_SUFFIXES, NULL);
	}

	string CodeTableDefinitionLanguage::getTargetBasename(const string& sourceBasename, const Flavor&,
			const Flavor&, const Component&) {
		string targetBasename(StringUtils::endsWith(sourceBasename, ".ctdef")
				? sourceBasename.substr(static_cast<string::size_type>(0u),
				sourceBasename.length() - static_cast<string::size_type>(6u))
				: sourceBasename);
		return targetBasename + ".redct";
	}

	GenerationAction<FileArtifact>* CodeTableDefinitionLanguage::newGenerationAction(FileArtifact* target,
			const Flavor&, const Flavor&, const Component& component, BuildContext& context) {
		Unref<CodeTableCompileGenerationAction> action(new CodeTableCompileGenerationAction(target));
		action->addIntermediateDirectories(component, context);
		return action.set();
	}

}}
