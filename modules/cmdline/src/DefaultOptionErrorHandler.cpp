#include <redstrain/io/streamoperators.hpp>

#include "AmbiguousOptionError.hpp"
#include "MissingBarewordError.hpp"
#include "UnrecognizedOptionError.hpp"
#include "UnexpectedBarewordError.hpp"
#include "ConflictingOptionsError.hpp"
#include "ExcessOptionArgumentError.hpp"
#include "DefaultOptionErrorHandler.hpp"
#include "MissingOptionArgumentError.hpp"

using std::string;
using redengine::io::FormattedOutputStream;
using redengine::io::operator<<;

namespace redengine {
namespace cmdline {

	DefaultOptionErrorHandler::DefaultOptionErrorHandler(const string& progname,
			FormattedOutputStream<char>& conerr) : progname(progname), conerr(conerr) {}

	DefaultOptionErrorHandler::DefaultOptionErrorHandler(const DefaultOptionErrorHandler& handler)
			: OptionErrorHandler(handler), progname(handler.progname), conerr(handler.conerr) {}

	void DefaultOptionErrorHandler::handleAnyOptionError(const CommandLineError& error) const {
		if(!progname.empty())
			conerr << progname << ": ";
		conerr.println(error.getMessage());
		if(!progname.empty())
			conerr << progname << ": ";
		conerr.println("Run with --help for usage information.");
	}

	void DefaultOptionErrorHandler::handleOptionError(const AmbiguousOptionError& error) {
		handleAnyOptionError(error);
	}

	void DefaultOptionErrorHandler::handleOptionError(const UnrecognizedOptionError& error) {
		handleAnyOptionError(error);
	}

	void DefaultOptionErrorHandler::handleOptionError(const ExcessOptionArgumentError& error) {
		handleAnyOptionError(error);
	}

	void DefaultOptionErrorHandler::handleOptionError(const MissingOptionArgumentError& error) {
		handleAnyOptionError(error);
	}

	void DefaultOptionErrorHandler::handleOptionError(const UnexpectedBarewordError& error) {
		handleAnyOptionError(error);
	}

	void DefaultOptionErrorHandler::handleOptionError(const MissingBarewordError& error) {
		handleAnyOptionError(error);
	}

	void DefaultOptionErrorHandler::handleOptionError(const ConflictingOptionsError& error) {
		handleAnyOptionError(error);
	}

}}
