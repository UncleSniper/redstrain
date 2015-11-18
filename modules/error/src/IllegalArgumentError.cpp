#include "IllegalArgumentError.hpp"

using std::string;

namespace redengine {
namespace error {

	IllegalArgumentError::IllegalArgumentError() {}

	IllegalArgumentError::IllegalArgumentError(const IllegalArgumentError& error) : Error(error) {}

	REDSTRAIN_DEFINE_ERROR(IllegalArgumentError) {
		out << "Illegal argument";
	}

}}
