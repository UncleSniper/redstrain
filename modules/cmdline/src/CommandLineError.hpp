#ifndef REDSTRAIN_MOD_CMDLINE_COMMANDLINEERROR_HPP
#define REDSTRAIN_MOD_CMDLINE_COMMANDLINEERROR_HPP

#include <redstrain/error/Error.hpp>

#include "api.hpp"

namespace redengine {
namespace cmdline {

	class REDSTRAIN_CMDLINE_API CommandLineError : public error::Error {

	  public:
		CommandLineError();
		CommandLineError(const CommandLineError&);

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(CommandLineError)

	};

}}

#endif /* REDSTRAIN_MOD_CMDLINE_COMMANDLINEERROR_HPP */
