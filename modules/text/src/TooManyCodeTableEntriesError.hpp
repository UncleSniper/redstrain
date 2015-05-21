#ifndef REDSTRAIN_MOD_TEXT_TOOMANYCODETABLEENTRIESERROR_HPP
#define REDSTRAIN_MOD_TEXT_TOOMANYCODETABLEENTRIESERROR_HPP

#include "WrongNumberOfCodeTableEntriesError.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API TooManyCodeTableEntriesError : public WrongNumberOfCodeTableEntriesError {

	  public:
		TooManyCodeTableEntriesError();
		TooManyCodeTableEntriesError(const TooManyCodeTableEntriesError&);

		REDSTRAIN_DECLARE_ERROR(TooManyCodeTableEntriesError)

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_TOOMANYCODETABLEENTRIESERROR_HPP */
