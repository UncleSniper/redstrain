#ifndef REDSTRAIN_MOD_VFS_ACCESSDENIEDERROR_HPP
#define REDSTRAIN_MOD_VFS_ACCESSDENIEDERROR_HPP

#include "VFSIOError.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API AccessDeniedError : public VFSIOError {

	  public:
		AccessDeniedError();
		AccessDeniedError(const AccessDeniedError&);

		REDSTRAIN_DECLARE_ERROR(AccessDeniedError)

	};

}}

#endif /* REDSTRAIN_MOD_VFS_ACCESSDENIEDERROR_HPP */
