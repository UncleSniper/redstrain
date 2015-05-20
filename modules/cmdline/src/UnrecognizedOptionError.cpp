#include "UnrecognizedOptionError.hpp"

using std::string;

namespace redengine {
namespace cmdline {

	UnrecognizedOptionError::UnrecognizedOptionError(const string& name, OptionType type, char initiatorSymbol)
			: OptionError(name, type, initiatorSymbol) {}

	UnrecognizedOptionError::UnrecognizedOptionError(const UnrecognizedOptionError& error) : OptionError(error) {}

	REDSTRAIN_DEFINE_ERROR(UnrecognizedOptionError) {
		out << "Unrecognized command line option: " << initiatorSymbol;
		if(type == DOUBLE_OPTION)
			out << initiatorSymbol;
		out << name;
	}

}}
