#ifndef REDSTRAIN_MOD_VFS_URIERROR_HPP
#define REDSTRAIN_MOD_VFS_URIERROR_HPP

#include "VFSError.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API URIError : public VFSError {

	  public:
		URIError();
		URIError(const URIError&);

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(URIError)

	};

}}

#endif /* REDSTRAIN_MOD_VFS_URIERROR_HPP */
