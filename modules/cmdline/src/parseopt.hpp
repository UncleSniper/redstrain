#ifndef REDSTRAIN_MOD_CMDLINE_PARSEOPT_HPP
#define REDSTRAIN_MOD_CMDLINE_PARSEOPT_HPP

#include <cstdlib>
#include <iostream>
#include <redstrain/util/Delete.hpp>
#include <redstrain/error/Error.hpp>
#include <redstrain/platform/Console.hpp>
#include <redstrain/platform/Pathname.hpp>
#include <redstrain/error/ErrorHandler.hpp>
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
			int (*mainFunction)(const std::string&, const ConfigT&),
			error::ErrorHandler& errorHandler) {
		util::Delete<WordAction> nonOptionAction(
				new ArgumentMemberFunctionWordAction<ConfigT, BarewordReturnT>(logic.getBaseObjectHolder(),
				barewordHandler));
		try {
			io::FileOutputStream stdErr(platform::Console::getStandardHandle(platform::Console::STANDARD_ERROR));
			stdErr.setCloseOnDestroy(false);
			io::FormattedOutputStream<char> conerr(stdErr);
			DefaultOptionErrorHandler optionErrorHandler(platform::Pathname::basename(*argv), conerr);
			if(!parseOpts(argv, argc, logic, optionErrorHandler, *nonOptionAction))
				return 1;
			if(optionsChecker) {
				try {
					(logic.getBaseObject()->*optionsChecker)();
				}
				catch(const UnexpectedBarewordError& error) {
					optionErrorHandler.handleOptionError(error);
					return 1;
				}
				catch(const MissingBarewordError& error) {
					optionErrorHandler.handleOptionError(error);
					return 1;
				}
			}
			return mainFunction(optionErrorHandler.getProgramName(), *logic.getBaseObject());
		}
		catch(const error::Error& error) {
			errorHandler.handleErrorCompletely(error);
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
