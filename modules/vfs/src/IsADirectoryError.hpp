#ifndef REDSTRAIN_MOD_VFS_ISADIRECTORYERROR_HPP
#define REDSTRAIN_MOD_VFS_ISADIRECTORYERROR_HPP

#include "VFSIOError.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API IsADirectoryError : public VFSIOError {

	  public:
		IsADirectoryError();
		IsADirectoryError(const IsADirectoryError&);

		REDSTRAIN_DECLARE_ERROR(IsADirectoryError)

	};

}}

#endif /* REDSTRAIN_MOD_VFS_ISADIRECTORYERROR_HPP */
