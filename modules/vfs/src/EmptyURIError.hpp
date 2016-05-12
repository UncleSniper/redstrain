#ifndef REDSTRAIN_MOD_VFS_EMPTYURIERROR_HPP
#define REDSTRAIN_MOD_VFS_EMPTYURIERROR_HPP

#include "InvalidURIError.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API EmptyURIError : public InvalidURIError {

	  public:
		EmptyURIError();
		EmptyURIError(const EmptyURIError&);

		REDSTRAIN_DECLARE_ERROR(EmptyURIError)

	};

}}

#endif /* REDSTRAIN_MOD_VFS_EMPTYURIERROR_HPP */
