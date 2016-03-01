#include "DatabaseIOError.hpp"

using redengine::platform::File;

namespace redengine {
namespace zweigana {
namespace core {

	DatabaseIOError::DatabaseIOError(Operation operation, File::ErrorCode errorCode)
			: operation(operation), errorCode(errorCode) {}

	DatabaseIOError::DatabaseIOError(const DatabaseIOError& error)
			: Error(error), DatabaseError(error), IOError(error), operation(error.operation),
			errorCode(error.errorCode) {}

	REDSTRAIN_DEFINE_ERROR(DatabaseIOError) {
		out << "Failed to ";
		switch(operation) {
			case OPEN:
				out << "open";
				break;
			case READ:
				out << "read from";
				break;
			case WRITE:
				out << "write to";
				break;
			case SYNC:
				out << "synchronize";
				break;
			case MMAP:
				out << "memory-map";
				break;
			case CLOSE:
				out << "close";
				break;
			default:
				out << "perform requested operation on";
				break;
		}
		out << " database: " << File::getErrorMessage(errorCode);
	}

}}}
