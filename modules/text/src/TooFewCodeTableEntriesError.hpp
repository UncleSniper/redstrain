#ifndef REDSTRAIN_MOD_TEXT_TOOFEWCODETABLEENTRIESERROR_HPP
#define REDSTRAIN_MOD_TEXT_TOOFEWCODETABLEENTRIESERROR_HPP

#include "WrongNumberOfCodeTableEntriesError.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API TooFewCodeTableEntriesError : public WrongNumberOfCodeTableEntriesError {

	  private:
		const unsigned entryCount;

	  public:
		TooFewCodeTableEntriesError(unsigned);
		TooFewCodeTableEntriesError(const TooFewCodeTableEntriesError&);

		inline unsigned getEntryCount() const {
			return entryCount;
		}

		REDSTRAIN_DECLARE_ERROR(TooFewCodeTableEntriesError)

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_TOOFEWCODETABLEENTRIESERROR_HPP */
