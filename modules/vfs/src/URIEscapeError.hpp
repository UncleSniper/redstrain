#ifndef REDSTRAIN_MOD_VFS_URIESCAPEERROR_HPP
#define REDSTRAIN_MOD_VFS_URIESCAPEERROR_HPP

#include <redstrain/util/types.hpp>

#include "InvalidURIPartError.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API URIEscapeError : public InvalidURIPartError {

	  public:
		URIEscapeError(const std::string&, util::MemorySize);
		URIEscapeError(const text::String16&, util::MemorySize);
		URIEscapeError(const text::String32&, util::MemorySize);
		URIEscapeError(const URIEscapeError&);

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(URIEscapeError)

	};

}}

#endif /* REDSTRAIN_MOD_VFS_URIESCAPEERROR_HPP */
