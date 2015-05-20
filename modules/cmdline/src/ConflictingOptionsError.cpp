#include "ConflictingOptionsError.hpp"

namespace redengine {
namespace cmdline {

	ConflictingOptionsError::ConflictingOptionsError(const std::string& nameA, const std::string& nameB)
			: nameA(nameA), nameB(nameB) {}

	ConflictingOptionsError::ConflictingOptionsError(const ConflictingOptionsError& error)
			: CommandLineError(error), nameA(error.nameA), nameB(error.nameB) {}

	REDSTRAIN_DEFINE_ERROR(ConflictingOptionsError) {
		out << "Options " << nameA << " and " << nameB << " cannot be used together";
	}

}}
