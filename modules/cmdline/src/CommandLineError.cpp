#include "CommandLineError.hpp"

namespace redengine {
namespace cmdline {

	CommandLineError::CommandLineError() {}

	CommandLineError::CommandLineError(const CommandLineError& error) : Error(error) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(CommandLineError)

}}
