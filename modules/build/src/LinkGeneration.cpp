#include <redstrain/util/Delete.hpp>
#ifdef TESTING_REDSTRAIN_BUILD_API
#include <redstrain/io/streamoperators.hpp>
#endif /* TESTING_REDSTRAIN_BUILD_API */

#include "Linker.hpp"
#include "LinkGeneration.hpp"
#include "LinkerConfiguration.hpp"

using std::list;
using redengine::util::Delete;
#ifdef TESTING_REDSTRAIN_BUILD_API
using redengine::io::DefaultConfiguredOutputStream;
using redengine::io::endln;
using redengine::io::shift;
using redengine::io::indent;
using redengine::io::unshift;
using redengine::io::operator<<;
#endif /* TESTING_REDSTRAIN_BUILD_API */

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

#ifdef TESTING_REDSTRAIN_BUILD_API
	void LinkGeneration::dumpGeneration(DefaultConfiguredOutputStream<char>::Stream& stream) const {
		stream << indent << "LinkGeneration {" << endln << shift;
		stream << indent << "mode = ";
		switch(mode) {
			#define clamp(name) \
				case Linkage::name: \
					stream << #name; \
					break;
			clamp(STATIC_EXECUTABLE)
			clamp(DYNAMIC_EXECUTABLE)
			clamp(STATIC_LIBRARY)
			clamp(DYNAMIC_LIBRARY)
			#undef clamp
			default:
				stream << "<unknown>";
				break;
		}
		stream << endln;
		stream << unshift << indent << '}' << endln;
	}
#endif /* TESTING_REDSTRAIN_BUILD_API */

}}
