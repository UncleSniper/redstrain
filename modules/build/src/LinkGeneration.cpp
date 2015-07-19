#include <redstrain/util/Delete.hpp>

#include "Linker.hpp"
#include "LinkGeneration.hpp"
#include "LinkerConfiguration.hpp"

using std::list;
using redengine::util::Delete;

namespace redengine {
namespace build {

	LinkGeneration::LinkGeneration(Linker& linker, Linkage::LinkMode mode, LinkerConfiguration& configuration)
			: linker(linker), mode(mode), configuration(configuration) {}

	LinkGeneration::LinkGeneration(const LinkGeneration& generation) : Generation<FileArtifact>(generation),
			linker(generation.linker), mode(generation.mode), configuration(generation.configuration) {}

	void LinkGeneration::generate(const list<FileArtifact*>& sources, FileArtifact* target, BuildContext&) {
		if(sources.empty() || !target)
			return;
		Delete<Linkage> linkage(linker.newLinkage(target->getPathname(), mode));
		list<FileArtifact*>::const_iterator begin(sources.begin()), end(sources.end());
		for(; begin != end; ++begin)
			linkage->addSource((*begin)->getPathname());
		configuration.applyConfiguration(**linkage);
		linkage->invoke();
	}

}}
