#ifndef REDSTRAIN_MOD_TEXT_UNARYFORMATTINGBASEERROR_HPP
#define REDSTRAIN_MOD_TEXT_UNARYFORMATTINGBASEERROR_HPP

#include "FormattingError.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API UnaryFormattingBaseError : public FormattingError {

	  public:
		UnaryFormattingBaseError();
		UnaryFormattingBaseError(const UnaryFormattingBaseError&);

		REDSTRAIN_DECLARE_ERROR(UnaryFormattingBaseError)

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_UNARYFORMATTINGBASEERROR_HPP */
