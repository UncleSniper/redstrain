#ifndef REDSTRAIN_MOD_VFS_MISSINGURISCHEMEERROR_HPP
#define REDSTRAIN_MOD_VFS_MISSINGURISCHEMEERROR_HPP

#include "InvalidURIError.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API MissingURISchemeError : public InvalidURIError {

	  public:
		MissingURISchemeError(const std::string&);
		MissingURISchemeError(const text::String16&);
		MissingURISchemeError(const text::String32&);
		MissingURISchemeError(const MissingURISchemeError&);

		REDSTRAIN_DECLARE_ERROR(MissingURISchemeError)

	};

}}

#endif /* REDSTRAIN_MOD_VFS_MISSINGURISCHEMEERROR_HPP */
