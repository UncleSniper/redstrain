#include "TooManyCodeTableEntriesError.hpp"

namespace redengine {
namespace text {

	TooManyCodeTableEntriesError::TooManyCodeTableEntriesError() {}

	TooManyCodeTableEntriesError::TooManyCodeTableEntriesError(const TooManyCodeTableEntriesError& error)
			: WrongNumberOfCodeTableEntriesError(error) {}

	REDSTRAIN_DEFINE_ERROR(TooManyCodeTableEntriesError) {
		out << "Too many entries in code table definition: Expected 256";
	}

}}
