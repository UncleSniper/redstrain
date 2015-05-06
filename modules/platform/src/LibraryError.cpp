#include "LibraryError.hpp"

using std::string;

namespace redengine {
namespace platform {

	LibraryError::LibraryError(const string& msg) : msg(msg) {}

	LibraryError::LibraryError(const LibraryError& error) : Error(error), IOError(error), msg(error.msg) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(LibraryError)

}}
