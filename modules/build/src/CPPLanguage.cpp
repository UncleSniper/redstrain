#include "CPPLanguage.hpp"

using std::string;

namespace redengine {
namespace build {

	CPPLanguage::CPPLanguage(Compiler& compiler) : CompiledLanguage("C++", compiler) {}

	CPPLanguage::CPPLanguage(const CPPLanguage& cpp) : CompiledLanguage(cpp) {}

	static const char *const SOURCE_SUFFIXES[] = {
		".cpp",
		".cxx",
		NULL
	};

	static const char *const HEADER_SUFFIXES[] = {
		".hpp",
		".hxx",
		NULL
	};

	Language::ArtifactType CPPLanguage::classifyFile(const string& path) {
		return Language::classifyFileBySuffix(path, SOURCE_SUFFIXES, HEADER_SUFFIXES);
	}

}}
