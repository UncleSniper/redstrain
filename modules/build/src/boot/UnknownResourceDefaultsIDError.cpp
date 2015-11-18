#include "UnknownResourceDefaultsIDError.hpp"

namespace redengine {
namespace build {
namespace boot {

	UnknownResourceDefaultsIDError::UnknownResourceDefaultsIDError(Resources::Defaults defaultsID)
			: defaultsID(defaultsID) {}

	UnknownResourceDefaultsIDError::UnknownResourceDefaultsIDError(const UnknownResourceDefaultsIDError& error)
			: Error(error), IllegalArgumentError(error), defaultsID(error.defaultsID) {}

	REDSTRAIN_DEFINE_ERROR(UnknownResourceDefaultsIDError) {
		out << "Unknown resource defaults ID: " << static_cast<int>(defaultsID);
	}

}}}
