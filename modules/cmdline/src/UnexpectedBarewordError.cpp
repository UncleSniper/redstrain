#include "UnexpectedBarewordError.hpp"

using std::string;

namespace redengine {
namespace cmdline {

	UnexpectedBarewordError::UnexpectedBarewordError(const string& word) : word(word) {}

	UnexpectedBarewordError::UnexpectedBarewordError(const UnexpectedBarewordError& error)
			: CommandLineError(error), word(error.word) {}

	REDSTRAIN_DEFINE_ERROR(UnexpectedBarewordError) {
		out << "Unexpected non-option argument: " << word;
	}

}}
