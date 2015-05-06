#include "LibraryResolveError.hpp"

using std::string;

namespace redengine {
namespace platform {

	LibraryResolveError::LibraryResolveError(const string& msg) : LibraryError(msg) {}

	LibraryResolveError::LibraryResolveError(const LibraryResolveError& error)
			: Error(error), LibraryError(error) {}

	REDSTRAIN_DEFINE_ERROR(LibraryResolveError) {
		out << "Failed to resolve symbol in library: " << msg;
	}

}}
