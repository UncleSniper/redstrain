#ifndef REDSTRAIN_MOD_IO_NUMBERFORMATERROR_HPP
#define REDSTRAIN_MOD_IO_NUMBERFORMATERROR_HPP

#include <redstrain/error/IllegalArgumentError.hpp>

#include "api.hpp"

namespace redengine {
namespace io {

	class REDSTRAIN_IO_API NumberFormatError : public error::IllegalArgumentError {

	  public:
		NumberFormatError(const std::string&);
		NumberFormatError(const NumberFormatError&);

		REDSTRAIN_DECLARE_ERROR(NumberFormatError)

	};

}}

#endif /* REDSTRAIN_MOD_IO_NUMBERFORMATERROR_HPP */
