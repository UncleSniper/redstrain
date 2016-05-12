#ifndef REDSTRAIN_MOD_VFS_INCOMPLETEURIESCAPEERROR_HPP
#define REDSTRAIN_MOD_VFS_INCOMPLETEURIESCAPEERROR_HPP

#include "URIEscapeError.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API IncompleteURIEscapeError : public URIEscapeError {

	  public:
		IncompleteURIEscapeError(const std::string&, util::MemorySize);
		IncompleteURIEscapeError(const text::String16&, util::MemorySize);
		IncompleteURIEscapeError(const text::String32&, util::MemorySize);
		IncompleteURIEscapeError(const IncompleteURIEscapeError&);

		REDSTRAIN_DECLARE_ERROR(IncompleteURIEscapeError)

	};

}}

#endif /* REDSTRAIN_MOD_VFS_INCOMPLETEURIESCAPEERROR_HPP */
