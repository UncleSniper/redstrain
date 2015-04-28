#ifndef REDSTRAIN_MOD_PLATFORM_LIBRARYERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_LIBRARYERROR_HPP

#include <redstrain/error/IOError.hpp>

#include "api.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API LibraryError : public error::IOError {

	  protected:
		const std::string msg;

	  public:
		LibraryError(const std::string&);
		LibraryError(const LibraryError&);

		inline const std::string& getSystemMessage() const {
			return msg;
		}

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(LibraryError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_LIBRARYERROR_HPP */
