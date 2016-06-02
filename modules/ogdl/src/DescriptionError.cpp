#include "DescriptionError.hpp"

using redengine::text::String16;

namespace redengine {
namespace ogdl {

	DescriptionError::DescriptionError(const String16& file, unsigned line, unsigned column)
			: file(file), line(line), column(column) {}

	DescriptionError::DescriptionError(const DescriptionError& error)
			: Error(error), OGDLError(error), file(error.file), line(error.line), column(error.column) {}

	DescriptionError::~DescriptionError() {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(DescriptionError)

}}
