#ifndef REDSTRAIN_MOD_VFS_READONLYFILESYSTEMERROR_HPP
#define REDSTRAIN_MOD_VFS_READONLYFILESYSTEMERROR_HPP

#include "VFSIOError.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API ReadOnlyFilesystemError : public VFSIOError {

	  public:
		ReadOnlyFilesystemError();
		ReadOnlyFilesystemError(const ReadOnlyFilesystemError&);

		REDSTRAIN_DECLARE_ERROR(ReadOnlyFilesystemError)

	};

}}

#endif /* REDSTRAIN_MOD_VFS_READONLYFILESYSTEMERROR_HPP */
