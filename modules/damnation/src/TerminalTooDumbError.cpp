#include "TerminalTooDumbError.hpp"

namespace redengine {
namespace damnation {

	TerminalTooDumbError::TerminalTooDumbError() {}

	TerminalTooDumbError::TerminalTooDumbError(const TerminalTooDumbError& error) : Error(error), DamnError(error) {}

	REDSTRAIN_DEFINE_ERROR(TerminalTooDumbError) {
		out << "Terminal is too dumb";
	}

}}
