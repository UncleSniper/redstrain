#include "NotASymbolicLinkError.hpp"

namespace redengine {
namespace vfs {

	NotASymbolicLinkError::NotASymbolicLinkError() {}

	NotASymbolicLinkError::NotASymbolicLinkError(const NotASymbolicLinkError& error)
			: Error(error), VFSIOError(error) {}

	REDSTRAIN_DEFINE_ERROR(NotASymbolicLinkError) {
		out << "Not a symbolic link";
	}

}}
