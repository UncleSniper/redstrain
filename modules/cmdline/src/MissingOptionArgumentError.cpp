#include "MissingOptionArgumentError.hpp"

using std::string;

namespace redengine {
namespace cmdline {

	MissingOptionArgumentError::MissingOptionArgumentError(const string& name, OptionType type, char initiatorSymbol)
			: OptionError(name, type, initiatorSymbol) {}

	MissingOptionArgumentError::MissingOptionArgumentError(const MissingOptionArgumentError& error)
			: OptionError(error) {}

	REDSTRAIN_DEFINE_ERROR(MissingOptionArgumentError) {
		out << "Command line option is missing required argument: " << initiatorSymbol;
		if(type == DOUBLE_OPTION)
			out << initiatorSymbol;
		out << name;
	}

}}
