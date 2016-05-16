#ifndef REDSTRAIN_MOD_VFS_MISSINGSCHEMESPECIFICURIPARTERROR_HPP
#define REDSTRAIN_MOD_VFS_MISSINGSCHEMESPECIFICURIPARTERROR_HPP

#include "InvalidURIError.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API MissingSchemeSpecificURIPartError : public InvalidURIError {

	  public:
		MissingSchemeSpecificURIPartError(const std::string&);
		MissingSchemeSpecificURIPartError(const text::String16&);
		MissingSchemeSpecificURIPartError(const text::String32&);
		MissingSchemeSpecificURIPartError(const MissingSchemeSpecificURIPartError&);

		REDSTRAIN_DECLARE_ERROR(MissingSchemeSpecificURIPartError)

	};

}}

#endif /* REDSTRAIN_MOD_VFS_MISSINGSCHEMESPECIFICURIPARTERROR_HPP */
