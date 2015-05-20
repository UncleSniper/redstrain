#include "OptionError.hpp"

using std::ostream;

namespace redengine {
namespace cmdline {

	OptionError::OptionError(const std::string& name, OptionType type, char initiatorSymbol)
			: name(name), type(type), initiatorSymbol(initiatorSymbol) {}

	OptionError::OptionError(const OptionError& error)
			: CommandLineError(error), name(error.name), type(error.type), initiatorSymbol(error.initiatorSymbol) {}

	void OptionError::printOptionErrorMessage(ostream& out) const {
		out << "Command line option error: " << initiatorSymbol;
		if(type == DOUBLE_OPTION)
			out << initiatorSymbol;
		out << name;
	}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(OptionError)

}}
