#include <sstream>

#include "IndexOutOfBoundsError.hpp"

using std::string;
using std::stringstream;

namespace redengine {
namespace error {

	static std::string makeMessage(size_t index) {
		stringstream ss;
		ss << "Index out of bounds: " << index;
		return ss.str();
	}

	static std::string makeMessage(const std::string& message, size_t index) {
		stringstream ss;
		ss << message << ": " << index;
		return ss.str();
	}

	IndexOutOfBoundsError::IndexOutOfBoundsError(size_t index)
			: IllegalArgumentError(makeMessage(index)), index(index) {}

	IndexOutOfBoundsError::IndexOutOfBoundsError(const string& message, size_t index)
			: IllegalArgumentError(makeMessage(message, index)), index(index) {}

	IndexOutOfBoundsError::IndexOutOfBoundsError(const IndexOutOfBoundsError& error)
			: Error(error), IllegalArgumentError(error), index(error.index) {}

	REDSTRAIN_DEFINE_ERROR(IndexOutOfBoundsError) {
		out << getRawMessage();
	}

}}
