#include "IllegalStandardStreamSpecifierError.hpp"

namespace redengine {
namespace platform {

	IllegalStandardStreamSpecifierError::IllegalStandardStreamSpecifierError(Console::StandardHandle specifier)
			: specifier(specifier) {}

	IllegalStandardStreamSpecifierError::IllegalStandardStreamSpecifierError(const
			IllegalStandardStreamSpecifierError& error)
			: Error(error), IllegalArgumentError(error), specifier(specifier) {}

	REDSTRAIN_DEFINE_ERROR(IllegalStandardStreamSpecifierError) {
		out << "Illegal standard stream specifier: " << static_cast<int>(specifier);
	}

}}
