#include "OGDLError.hpp"

namespace redengine {
namespace ogdl {

	OGDLError::OGDLError() {}

	OGDLError::OGDLError(const OGDLError& error) : Error(error) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(OGDLError)

}}
