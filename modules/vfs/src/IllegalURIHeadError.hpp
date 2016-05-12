#ifndef REDSTRAIN_MOD_VFS_ILLEGALURIHEADERROR_HPP
#define REDSTRAIN_MOD_VFS_ILLEGALURIHEADERROR_HPP

#include "InvalidURIPartError.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API IllegalURIHeadError : public InvalidURIPartError {

	  public:
		IllegalURIHeadError(const std::string&, util::MemorySize);
		IllegalURIHeadError(const text::String16&, util::MemorySize);
		IllegalURIHeadError(const text::String32&, util::MemorySize);
		IllegalURIHeadError(const IllegalURIHeadError&);

		REDSTRAIN_DECLARE_ERROR(IllegalURIHeadError)

	};

}}

#endif /* REDSTRAIN_MOD_VFS_ILLEGALURIHEADERROR_HPP */
