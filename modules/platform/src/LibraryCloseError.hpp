#ifndef REDSTRAIN_MOD_PLATFORM_LIBRARYCLOSEERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_LIBRARYCLOSEERROR_HPP

#include "LibraryError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API LibraryCloseError : public LibraryError {

	  public:
		LibraryCloseError(const std::string&);
		LibraryCloseError(const LibraryCloseError&);

		REDSTRAIN_DECLARE_ERROR(LibraryCloseError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_LIBRARYCLOSEERROR_HPP */
