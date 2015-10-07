#include <redstrain/util/StringUtils.hpp>

#include "CodeTableCompileTransform.hpp"
#include "CodeTableDefinitionLanguage.hpp"

using std::string;
using redengine::util::StringUtils;

namespace redengine {
namespace build {

	CodeTableDefinitionLanguage::CodeTableDefinitionLanguage() : FileConversionLanguage("code table definition") {}

	CodeTableDefinitionLanguage::CodeTableDefinitionLanguage(const CodeTableDefinitionLanguage& language)
			: FileConversionLanguage(language) {}

	string CodeTableDefinitionLanguage::getTargetBasename(const string& sourceBasename,
			const Flavor&, const Flavor&, Component&) {
		string targetBasename(StringUtils::endsWith(sourceBasename, ".ctdef")
				? sourceBasename.substr(static_cast<string::size_type>(0u),
				sourceBasename.length() - static_cast<string::size_type>(6u))
				: sourceBasename);
		return targetBasename + ".redct";
	}

	Transform* CodeTableDefinitionLanguage::getConversionTransform(FileArtifact& sourceArtifact,
			const Flavor&, const Flavor&, Component&) {
		return new CodeTableCompileTransform(sourceArtifact);
	}

	static const char *const SOURCE_SUFFIXES[] = {
		".ctdef",
		NULL
	};

	Language::ArtifactType CodeTableDefinitionLanguage::classifyFile(const string& path) {
		return Language::classifyFileBySuffix(path, SOURCE_SUFFIXES, NULL);
	}

}}
