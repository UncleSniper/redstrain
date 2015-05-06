#include "ConsoleError.hpp"

namespace redengine {
namespace platform {

	ConsoleError::ConsoleError(File::ErrorCode code, Operation operation) : code(code), operation(operation) {}

	ConsoleError::ConsoleError(const ConsoleError& error)
			: Error(error), IOError(error), code(error.code), operation(error.operation) {}

	REDSTRAIN_DEFINE_ERROR(ConsoleError) {
		out << "Failed to ";
		switch(operation) {
			case CLEAR:
				out << "clear console";
				break;
			case SET_CURSOR_POSITION:
				out << "set console cursor position";
				break;
			case SET_TEXT_ATTRIBUTE:
				out << "set console text attributes";
				break;
			default:
				out << "perform console operation";
				break;
		}
		out << ": " << File::getErrorMessage(code);
	}

}}
