#include "LibraryOpenError.hpp"

using std::string;

namespace redengine {
namespace platform {

	LibraryOpenError::LibraryOpenError(const string& msg) : LibraryError(msg) {}

	LibraryOpenError::LibraryOpenError(const LibraryOpenError& error) : Error(error), LibraryError(error) {}

	REDSTRAIN_DEFINE_ERROR(LibraryOpenError) {
		out << "Failed to open library: " << msg;
	}

}}
