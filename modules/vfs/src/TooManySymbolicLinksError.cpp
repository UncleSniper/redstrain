#include "TooManySymbolicLinksError.hpp"

using std::string;

namespace redengine {
namespace vfs {

	TooManySymbolicLinksError::TooManySymbolicLinksError(const string& path) : path(path) {}

	TooManySymbolicLinksError::TooManySymbolicLinksError(const TooManySymbolicLinksError& error)
			: Error(error), VFSIOError(error), path(error.path) {}

	REDSTRAIN_DEFINE_ERROR(TooManySymbolicLinksError) {
		out << "Too many symbolic links";
		if(!path.empty())
			out << ": " << path;
	}

}}
