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

}}
