#include "LibraryCloseError.hpp"

using std::string;

namespace redengine {
namespace platform {

	LibraryCloseError::LibraryCloseError(const string& msg) : LibraryError(msg) {}

	LibraryCloseError::LibraryCloseError(const LibraryCloseError& error) : LibraryError(error) {}

	REDSTRAIN_DEFINE_ERROR(LibraryCloseError) {
		out << "Failed to close library: " << msg;
	}

}}
