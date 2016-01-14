#include "InvalidFormattingItemReferenceError.hpp"

using redengine::util::MemorySize;

namespace redengine {
namespace text {

	InvalidFormattingItemReferenceError::InvalidFormattingItemReferenceError(Reference reference, MemorySize offset)
			: reference(reference), offset(offset) {}

	InvalidFormattingItemReferenceError::InvalidFormattingItemReferenceError(const
			InvalidFormattingItemReferenceError& error)
			: FormattingError(error), reference(error.reference), offset(error.offset) {}

	REDSTRAIN_DEFINE_ERROR(InvalidFormattingItemReferenceError) {
		out << "Formatting item reference out of range at offset " << offset << ": " << reference;
	}

}}
