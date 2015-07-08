#ifndef REDSTRAIN_MOD_VFS_NOTASYMBOLICLINKERROR_HPP
#define REDSTRAIN_MOD_VFS_NOTASYMBOLICLINKERROR_HPP

#include "VFSIOError.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API NotASymbolicLinkError : public VFSIOError {

	  public:
		NotASymbolicLinkError();
		NotASymbolicLinkError(const NotASymbolicLinkError&);

		REDSTRAIN_DECLARE_ERROR(NotASymbolicLinkError)

	};

}}

#endif /* REDSTRAIN_MOD_VFS_NOTASYMBOLICLINKERROR_HPP */
