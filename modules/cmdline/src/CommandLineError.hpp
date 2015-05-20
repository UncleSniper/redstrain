#ifndef REDSTRAIN_MOD_CMDLINE_COMMANDLINEERROR_HPP
#define REDSTRAIN_MOD_CMDLINE_COMMANDLINEERROR_HPP

#include <redstrain/error/Error.hpp>

namespace redengine {
namespace cmdline {

	class CommandLineError : public error::Error {

	  public:
		CommandLineError();
		CommandLineError(const CommandLineError&);

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(CommandLineError)

	};

}}

#endif /* REDSTRAIN_MOD_CMDLINE_COMMANDLINEERROR_HPP */
