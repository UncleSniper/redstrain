#include "Linkage.hpp"

using std::string;

namespace redengine {
namespace build {

	Linkage::Linkage(const string& target, LinkMode mode) : target(target), mode(mode) {}

	Linkage::Linkage(const Linkage& linkage)
			: Invocation(linkage), sources(linkage.sources), target(linkage.target), mode(linkage.mode) {}

	void Linkage::addSource(const string& source) {
		sources.push_back(source);
	}

	void Linkage::clearSources() {
		sources.clear();
	}

	void Linkage::getSources(PathIterator& begin, PathIterator& end) const {
		begin = sources.begin();
		end = sources.end();
	}

	const char* Linkage::getLinkModeName(LinkMode mode) {
		switch(mode) {
			#define clamp(c) case c: return #c;
			clamp(STATIC_EXECUTABLE)
			clamp(DYNAMIC_EXECUTABLE)
			clamp(STATIC_LIBRARY)
			clamp(DYNAMIC_LIBRARY)
			#undef clamp
			default:
				return NULL;
		}
	}

}}
