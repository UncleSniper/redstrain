#ifndef REDSTRAIN_MOD_CALENDAR_FORMATTINGOPTIONS_HPP
#define REDSTRAIN_MOD_CALENDAR_FORMATTINGOPTIONS_HPP

#include "api.hpp"

namespace redengine {
namespace calendar {

	class REDSTRAIN_CALENDAR_API FormattingOptions {

	  public:
		enum Padding {
			PAD_ZERO,
			PAD_BLANK
		};

	  public:
		unsigned width;
		Padding padding;

	  public:
		FormattingOptions();
		FormattingOptions(const FormattingOptions&);

	};

}}

#endif /* REDSTRAIN_MOD_CALENDAR_FORMATTINGOPTIONS_HPP */
