#include "AmbiguousOptionError.hpp"

namespace redengine {
namespace cmdline {

	AmbiguousOptionError::AmbiguousOptionError(const std::string& name, char initiatorSymbol)
			: OptionError(name, SINGLE_OPTION, initiatorSymbol) {}

	AmbiguousOptionError::AmbiguousOptionError(const AmbiguousOptionError& error) : OptionError(error) {}

	REDSTRAIN_DEFINE_ERROR(AmbiguousOptionError) {
		out << "Ambiguous command line option: Could be " << initiatorSymbol
				<< name[0] << " or " << initiatorSymbol << initiatorSymbol << name;
	}

}}
