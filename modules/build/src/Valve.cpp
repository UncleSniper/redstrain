#include "Valve.hpp"

namespace redengine {
namespace build {

	Valve::Valve() : open(false) {}

	Valve::Valve(const Valve& value) : ReferenceCounted(value), open(value.open) {}

}}
