#ifndef REDSTRAIN_MOD_CMDLINE_PARSEOPT_HPP
#define REDSTRAIN_MOD_CMDLINE_PARSEOPT_HPP

#include <iostream>
#include <redstrain/util/Delete.hpp>
#include <redstrain/error/Error.hpp>
#include <redstrain/platform/Console.hpp>
#include <redstrain/platform/Pathname.hpp>
#include <redstrain/io/FileOutputStream.hpp>
#include <redstrain/io/FormattedOutputStream.hpp>

#include "StopExecution.hpp"
#include "MissingBarewordError.hpp"
#include "UnexpectedBarewordError.hpp"
#include "DefaultOptionErrorHandler.hpp"
#include "ConfigurationObjectOptionLogic.hpp"
#include "ArgumentMemberFunctionWordAction.hpp"

namespace redengine {
namespace cmdline {

	class WordAction;
	class OptionLogic;
	class OptionParser;

	bool parseOpts(char**, int, OptionParser&, OptionErrorHandler&);
	bool parseOpts(char**, int, OptionParser&, io::FormattedOutputStream<char>&);
	bool parseOpts(char**, int, OptionParser&);
	bool parseOpts(char**, int, const OptionLogic&, OptionErrorHandler&, WordAction* = NULL);
	bool parseOpts(char**, int, const OptionLogic&, io::FormattedOutputStream<char>&, WordAction* = NULL);
	bool parseOpts(char**, int, const OptionLogic&, WordAction* = NULL);

	template<typename ConfigT, typename BarewordReturnT>
	int runWithOptions(char** argv, int argc, const ConfigurationObjectOptionLogic<ConfigT>& logic,
			BarewordReturnT (ConfigT::*barewordHandler)(const std::string&),
			void (ConfigT::*optionsChecker)(),
			int (*mainFunction)(const std::string&, const ConfigT&)) {
		util::Delete<WordAction> nonOptionAction(
				new ArgumentMemberFunctionWordAction<ConfigT, BarewordReturnT>(logic.getBaseObjectHolder(),
				barewordHandler));
		try {
			io::FileOutputStream stdErr(platform::Console::getStandardHandle(platform::Console::STANDARD_ERROR));
			stdErr.setCloseOnDestroy(false);
			io::FormattedOutputStream<char> conerr(stdErr);
			DefaultOptionErrorHandler errorHandler(platform::Pathname::basename(*argv), conerr);
			if(!parseOpts(argv, argc, logic, errorHandler, *nonOptionAction))
				return 1;
			if(optionsChecker) {
				try {
					(logic.getBaseObject()->*optionsChecker)();
				}
				catch(const UnexpectedBarewordError& error) {
					errorHandler.handleOptionError(error);
					return 1;
				}
				catch(const MissingBarewordError& error) {
					errorHandler.handleOptionError(error);
					return 1;
				}
			}
			return mainFunction(errorHandler.getProgramName(), *logic.getBaseObject());
		}
		catch(const error::Error& error) {
			std::cerr << platform::Pathname::basename(*argv) << ": ";
			error.printMessage(std::cerr);
			std::cerr << std::endl;
		}
		catch(const StopExecution& stop) {
			return stop.getStatus();
		}
		catch(...) {
			std::cerr << platform::Pathname::basename(*argv) << ": Uncaught exception" << std::endl;
		}
		return 1;
	}

}}

#endif /* REDSTRAIN_MOD_CMDLINE_PARSEOPT_HPP */
