#ifndef REDSTRAIN_MOD_VFS_INVALIDURIESCAPEERROR_HPP
#define REDSTRAIN_MOD_VFS_INVALIDURIESCAPEERROR_HPP

#include "URIEscapeError.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API InvalidURIEscapeError : public URIEscapeError {

	  public:
		InvalidURIEscapeError(const std::string&, util::MemorySize);
		InvalidURIEscapeError(const text::String16&, util::MemorySize);
		InvalidURIEscapeError(const text::String32&, util::MemorySize);
		InvalidURIEscapeError(const InvalidURIEscapeError&);

		REDSTRAIN_DECLARE_ERROR(InvalidURIEscapeError)

	};

}}

#endif /* REDSTRAIN_MOD_VFS_INVALIDURIESCAPEERROR_HPP */
