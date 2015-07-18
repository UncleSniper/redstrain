#include "ExternalLinker.hpp"

using std::string;

namespace redengine {
namespace build {

	// ======== ExternalLinkage ========

	ExternalLinker::ExternalLinkage::ExternalLinkage(const string& executable, const string& target, LinkMode mode)
			: Linkage(target, mode), ExternalInvocation(executable) {}

	ExternalLinker::ExternalLinkage::ExternalLinkage(const ExternalLinkage& linkage)
			: Invocation(linkage), Linkage(linkage), ExternalInvocation(linkage) {}

	// ======== ExternalLinker ========

	ExternalLinker::ExternalLinker(const string& executable, Architecture architecture)
			: ExternalTool(executable), Linker(architecture) {}

	ExternalLinker::ExternalLinker(const ExternalLinker& linker) : ExternalTool(linker), Linker(linker) {}

}}
