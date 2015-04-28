#ifndef REDSTRAIN_MOD_PLATFORM_OSBINDERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_OSBINDERROR_HPP

#include <redstrain/error/Error.hpp>

#include "api.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API OSBindError : public error::Error {

	  public:
		OSBindError();
		OSBindError(const OSBindError&);

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(OSBindError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_OSBINDERROR_HPP */
