#ifndef REDSTRAIN_MOD_VFS_MALFORMEDURIPORTERROR_HPP
#define REDSTRAIN_MOD_VFS_MALFORMEDURIPORTERROR_HPP

#include "InvalidURIPortError.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API MalformedURIPortError : public InvalidURIPortError {

	  public:
		MalformedURIPortError(const std::string&);
		MalformedURIPortError(const text::String16&);
		MalformedURIPortError(const text::String32&);
		MalformedURIPortError(const MalformedURIPortError&);

		REDSTRAIN_DECLARE_ERROR(MalformedURIPortError)

	};

}}

#endif /* REDSTRAIN_MOD_VFS_MALFORMEDURIPORTERROR_HPP */
