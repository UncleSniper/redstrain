#include "CodeTableDefinitionError.hpp"

namespace redengine {
namespace text {

	CodeTableDefinitionError::CodeTableDefinitionError() {}

	CodeTableDefinitionError::CodeTableDefinitionError(const CodeTableDefinitionError& error) : TextError(error) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(CodeTableDefinitionError)

}}
