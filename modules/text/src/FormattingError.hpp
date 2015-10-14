#ifndef REDSTRAIN_MOD_TEXT_FORMATTINGERROR_HPP
#define REDSTRAIN_MOD_TEXT_FORMATTINGERROR_HPP

#include "TextError.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API FormattingError : public TextError {

	  public:
		FormattingError();
		FormattingError(const FormattingError&);

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(FormattingError)

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_FORMATTINGERROR_HPP */
