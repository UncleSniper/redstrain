#ifndef REDSTRAIN_MOD_VFS_URIPORTOUTOFRANGEERROR_HPP
#define REDSTRAIN_MOD_VFS_URIPORTOUTOFRANGEERROR_HPP

#include "InvalidURIPortError.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API URIPortOutOfRangeError : public InvalidURIPortError {

	  public:
		URIPortOutOfRangeError(const std::string&);
		URIPortOutOfRangeError(const text::String16&);
		URIPortOutOfRangeError(const text::String32&);
		URIPortOutOfRangeError(const URIPortOutOfRangeError&);

		REDSTRAIN_DECLARE_ERROR(URIPortOutOfRangeError)

	};

}}

#endif /* REDSTRAIN_MOD_VFS_URIPORTOUTOFRANGEERROR_HPP */
