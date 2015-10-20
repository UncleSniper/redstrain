#ifndef REDSTRAIN_MOD_TEXT_FORMATSTRINGSYNTAXERROR_HPP
#define REDSTRAIN_MOD_TEXT_FORMATSTRINGSYNTAXERROR_HPP

#include "FormattingError.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API FormatStringSyntaxError : public FormattingError {

	  private:
		const size_t offset;

	  public:
		FormatStringSyntaxError(size_t);
		FormatStringSyntaxError(const FormatStringSyntaxError&);

		inline size_t getOffset() const {
			return offset;
		}

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(FormatStringSyntaxError)

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_FORMATSTRINGSYNTAXERROR_HPP */
