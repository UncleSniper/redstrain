#include "UnsupportedOperationError.hpp"

using std::string;

namespace redengine {
namespace error {

	UnsupportedOperationError::UnsupportedOperationError(const string& operation) : operation(operation) {}

	UnsupportedOperationError::UnsupportedOperationError(const UnsupportedOperationError& error)
			: Error(error), operation(error.operation) {}

	UnsupportedOperationError::~UnsupportedOperationError() {}

	REDSTRAIN_DEFINE_ERROR(UnsupportedOperationError) {
		out << "Operation not supported: " << operation;
	}

}}
