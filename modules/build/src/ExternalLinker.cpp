#include "ExternalLinker.hpp"

using std::string;
using redengine::redmond::Architecture;

namespace redengine {
namespace build {

	// ======== ExternalLinkage ========

	ExternalLinker::ExternalLinkage::ExternalLinkage(const string& executable, const string& target, LinkMode mode)
			: Linkage(target, mode), ExternalInvocation(executable) {}

	ExternalLinker::ExternalLinkage::ExternalLinkage(const ExternalLinkage& linkage)
			: Invocation(linkage), Linkage(linkage), ExternalInvocation(linkage),
			libraryDirectories(linkage.libraryDirectories), libraries(linkage.libraries) {}

	void ExternalLinker::ExternalLinkage::addLibraryDirectory(const string& directory) {
		if(!directory.empty())
			libraryDirectories.push_back(directory);
	}

	void ExternalLinker::ExternalLinkage::addLibrary(const string& library) {
		if(!library.empty())
			libraries.push_back(library);
	}

	// ======== ExternalLinker ========

	ExternalLinker::ExternalLinker(const string& executable, Architecture architecture)
			: ExternalTool(executable), Linker(architecture) {}

	ExternalLinker::ExternalLinker(const ExternalLinker& linker) : ExternalTool(linker), Linker(linker) {}

}}
