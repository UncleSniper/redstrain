#ifndef REDSTRAIN_MOD_ERROR_VFSIOERROR_HPP
#define REDSTRAIN_MOD_ERROR_VFSIOERROR_HPP

#include <redstrain/error/IOError.hpp>

#include "VFSError.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API VFSIOError : public VFSError, public error::IOError {

	  public:
		VFSIOError();
		VFSIOError(const VFSIOError&);

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(VFSIOError)

	};

}}

#endif /* REDSTRAIN_MOD_ERROR_VFSIOERROR_HPP */
