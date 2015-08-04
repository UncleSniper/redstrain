#include "BuildError.hpp"

namespace redengine {
namespace build {

	BuildError::BuildError() {}

	BuildError::BuildError(const BuildError& error) : Error(error) {}

}}
