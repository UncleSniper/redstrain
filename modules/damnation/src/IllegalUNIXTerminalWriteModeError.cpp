#include "IllegalUNIXTerminalWriteModeError.hpp"

namespace redengine {
namespace damnation {

	IllegalUNIXTerminalWriteModeError::IllegalUNIXTerminalWriteModeError(UNIXTerminalBinding::WriteMode mode)
			: mode(mode) {}

	IllegalUNIXTerminalWriteModeError::IllegalUNIXTerminalWriteModeError(const
			IllegalUNIXTerminalWriteModeError& error)
			: Error(error), IllegalArgumentError(error), DamnError(error), mode(error.mode) {}

	REDSTRAIN_DEFINE_ERROR(IllegalUNIXTerminalWriteModeError) {
		out << "Illegal write mode for UNIX terminal: " << static_cast<int>(mode);
	}

}}
