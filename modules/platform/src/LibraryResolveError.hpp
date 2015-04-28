#ifndef REDSTRAIN_MOD_PLATFORM_LIBRARYRESOLVEERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_LIBRARYRESOLVEERROR_HPP

#include "LibraryError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API LibraryResolveError : public LibraryError {

	  public:
		LibraryResolveError(const std::string&);
		LibraryResolveError(const LibraryResolveError&);

		REDSTRAIN_DECLARE_ERROR(LibraryResolveError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_LIBRARYRESOLVEERROR_HPP */
