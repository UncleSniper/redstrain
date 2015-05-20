#include "parseopt.hpp"
#include "WordAction.hpp"
#include "OptionLogic.hpp"
#include "OptionParser.hpp"
#include "AmbiguousOptionError.hpp"
#include "UnrecognizedOptionError.hpp"
#include "ConflictingOptionsError.hpp"
#include "ExcessOptionArgumentError.hpp"
#include "MissingOptionArgumentError.hpp"

using redengine::platform::Console;
using redengine::platform::Pathname;
using redengine::io::FileOutputStream;
using redengine::io::FormattedOutputStream;

namespace redengine {
namespace cmdline {

	bool parseOpts(char** argv, int argc, OptionParser& parser, OptionErrorHandler& handler) {
		try {
			parser.parseWords(argv, argc);
			parser.endCommandLine();
			return true;
		}
		catch(const AmbiguousOptionError& error) {
			handler.handleOptionError(error);
		}
		catch(const UnrecognizedOptionError& error) {
			handler.handleOptionError(error);
		}
		catch(const ExcessOptionArgumentError& error) {
			handler.handleOptionError(error);
		}
		catch(const MissingOptionArgumentError& error) {
			handler.handleOptionError(error);
		}
		catch(const UnexpectedBarewordError& error) {
			handler.handleOptionError(error);
		}
		catch(const MissingBarewordError& error) {
			handler.handleOptionError(error);
		}
		catch(const ConflictingOptionsError& error) {
			handler.handleOptionError(error);
		}
		return false;
	}

	bool parseOpts(char** argv, int argc, OptionParser& parser, FormattedOutputStream<char>& conerr) {
		DefaultOptionErrorHandler handler(Pathname::basename(*argv), conerr);
		return parseOpts(argv, argc, parser, handler);
	}

	bool parseOpts(char** argv, int argc, OptionParser& parser) {
		FileOutputStream stdErr(Console::getStandardHandle(Console::STANDARD_ERROR));
		FormattedOutputStream<char> conerr(stdErr);
		return parseOpts(argv, argc, parser, conerr);
	}

	bool parseOpts(char** argv, int argc, const OptionLogic& logic, OptionErrorHandler& handler,
			WordAction* nonOptionAction) {
		OptionParser parser(logic, nonOptionAction ? nonOptionAction : &parser);
		return parseOpts(argv, argc, parser, handler);
	}

	bool parseOpts(char** argv, int argc, const OptionLogic& logic, FormattedOutputStream<char>& conerr,
			WordAction* nonOptionAction) {
		DefaultOptionErrorHandler handler(Pathname::basename(*argv), conerr);
		return parseOpts(argv, argc, logic, handler, nonOptionAction);
	}

	bool parseOpts(char** argv, int argc, const OptionLogic& logic, WordAction* nonOptionAction) {
		FileOutputStream stdErr(Console::getStandardHandle(Console::STANDARD_ERROR));
		FormattedOutputStream<char> conerr(stdErr);
		return parseOpts(argv, argc, logic, conerr, nonOptionAction);
	}

}}
