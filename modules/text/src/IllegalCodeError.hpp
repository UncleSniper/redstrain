#ifndef REDSTRAIN_MOD_TEXT_ILLEGALCODEERROR_HPP
#define REDSTRAIN_MOD_TEXT_ILLEGALCODEERROR_HPP

#include "RenditionError.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API IllegalCodeError : public RenditionError {

	  public:
		IllegalCodeError();
		IllegalCodeError(const IllegalCodeError&);

		REDSTRAIN_DECLARE_ERROR(IllegalCodeError)

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_ILLEGALCODEERROR_HPP */
