#include "ReferenceCounted.hpp"

namespace redengine {
namespace util {

	ReferenceCounted::ReferenceCounted(const ReferenceCounted&) : refcount(1u) {}

	ReferenceCounted::ReferenceCounted() : refcount(1u) {}

	ReferenceCounted::~ReferenceCounted() {}

}}
