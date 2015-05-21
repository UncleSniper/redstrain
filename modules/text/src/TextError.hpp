#ifndef REDSTRAIN_MOD_TEXT_TEXTERROR_HPP
#define REDSTRAIN_MOD_TEXT_TEXTERROR_HPP

#include <redstrain/error/Error.hpp>

#include "api.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API TextError : public error::Error {

	  public:
		TextError();
		TextError(const TextError&);

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(TextError)

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_TEXTERROR_HPP */
