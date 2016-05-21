#include "URI.hpp"
#include "URIComparator.hpp"

namespace redengine {
namespace vfs {

	URIComparator::URIComparator(const URI& uri) : uri(uri) {}

	URIComparator::URIComparator(const URIComparator& comparator) : uri(comparator.uri) {}

#define makeRel(oper) \
	bool URIComparator::operator oper(const URIComparator& other) { \
		return uri.toString16() oper other.uri.toString16(); \
	}

	makeRel(==)
	makeRel(!=)
	makeRel(<)
	makeRel(<=)
	makeRel(>)
	makeRel(>=)

}}
