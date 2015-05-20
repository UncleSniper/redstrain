#include "ExcessOptionArgumentError.hpp"

using std::string;

namespace redengine {
namespace cmdline {

	ExcessOptionArgumentError::ExcessOptionArgumentError(const string& name, OptionType type, char initiatorSymbol)
			: OptionError(name, type, initiatorSymbol) {}

	ExcessOptionArgumentError::ExcessOptionArgumentError(const ExcessOptionArgumentError& error)
			: OptionError(error) {}

	REDSTRAIN_DEFINE_ERROR(ExcessOptionArgumentError) {
		out << "Command line option takes no argument: " << initiatorSymbol;
		if(type == DOUBLE_OPTION)
			out << initiatorSymbol;
		out << name;
	}

}}
