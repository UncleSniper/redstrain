#ifndef REDSTRAIN_MOD_PLATFORM_PROCESSERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_PROCESSERROR_HPP

#include <redstrain/error/IOError.hpp>

#include "api.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API ProcessError : public error::IOError {

	  public:
		ProcessError();
		ProcessError(const ProcessError&);

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(ProcessError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_PROCESSERROR_HPP */
