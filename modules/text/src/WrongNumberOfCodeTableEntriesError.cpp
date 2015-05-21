#include "WrongNumberOfCodeTableEntriesError.hpp"

namespace redengine {
namespace text {

	WrongNumberOfCodeTableEntriesError::WrongNumberOfCodeTableEntriesError() {}

	WrongNumberOfCodeTableEntriesError::WrongNumberOfCodeTableEntriesError(const WrongNumberOfCodeTableEntriesError&
			error) : CodeTableDefinitionError(error) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(WrongNumberOfCodeTableEntriesError)

}}
