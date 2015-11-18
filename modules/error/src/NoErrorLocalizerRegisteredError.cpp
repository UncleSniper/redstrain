#include "NoErrorLocalizerRegisteredError.hpp"

using std::string;

namespace redengine {
namespace error {

	NoErrorLocalizerRegisteredError::NoErrorLocalizerRegisteredError(const string& exceptionClass)
			: exceptionClass(exceptionClass) {}

	NoErrorLocalizerRegisteredError::NoErrorLocalizerRegisteredError(const NoErrorLocalizerRegisteredError& error)
			: Error(error), StateError(error), exceptionClass(error.exceptionClass) {}

	NoErrorLocalizerRegisteredError::~NoErrorLocalizerRegisteredError() {}

	REDSTRAIN_DEFINE_ERROR(NoErrorLocalizerRegisteredError) {
		out << "Cannot localize error message: No localizer function registered for class " << exceptionClass;
	}

}}
