#ifndef REDSTRAIN_MOD_VFS_VFSERROR_HPP
#define REDSTRAIN_MOD_VFS_VFSERROR_HPP

#include <redstrain/error/Error.hpp>

#include "api.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API VFSError : public virtual error::Error {

	  public:
		VFSError();
		VFSError(const VFSError&);

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(VFSError)

	};

}}

#endif /* REDSTRAIN_MOD_VFS_VFSERROR_HPP */
