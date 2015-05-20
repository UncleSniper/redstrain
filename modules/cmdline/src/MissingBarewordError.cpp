#include "MissingBarewordError.hpp"

using std::string;

namespace redengine {
namespace cmdline {

	MissingBarewordError::MissingBarewordError(const string& word) : word(word) {}

	MissingBarewordError::MissingBarewordError(const MissingBarewordError& error)
			: CommandLineError(error), word(error.word) {}

	REDSTRAIN_DEFINE_ERROR(MissingBarewordError) {
		out << "Missing required non-option argument: " << word;
	}

}}
