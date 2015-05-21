#include "TooFewCodeTableEntriesError.hpp"

namespace redengine {
namespace text {

	TooFewCodeTableEntriesError::TooFewCodeTableEntriesError(unsigned entryCount) : entryCount(entryCount) {}

	TooFewCodeTableEntriesError::TooFewCodeTableEntriesError(const TooFewCodeTableEntriesError& error)
			: WrongNumberOfCodeTableEntriesError(error), entryCount(error.entryCount) {}

	REDSTRAIN_DEFINE_ERROR(TooFewCodeTableEntriesError) {
		out << "Too few entries in code table definition: Expected 256, but got " << entryCount;
	}

}}
