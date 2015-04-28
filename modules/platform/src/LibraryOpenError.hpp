#ifndef REDSTRAIN_MOD_PLATFORM_LIBRARYOPENERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_LIBRARYOPENERROR_HPP

#include "LibraryError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API LibraryOpenError : public LibraryError {

	  public:
		LibraryOpenError(const std::string&);
		LibraryOpenError(const LibraryOpenError&);

		REDSTRAIN_DECLARE_ERROR(LibraryOpenError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_LIBRARYOPENERROR_HPP */
