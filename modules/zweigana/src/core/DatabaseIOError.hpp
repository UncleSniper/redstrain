#ifndef REDSTRAIN_MOD_ZWEIGANA_CORE_DATABASEIOERROR_HPP
#define REDSTRAIN_MOD_ZWEIGANA_CORE_DATABASEIOERROR_HPP

#include <redstrain/error/IOError.hpp>
#include <redstrain/platform/File.hpp>

#include "../DatabaseError.hpp"

namespace redengine {
namespace zweigana {
namespace core {

	class REDSTRAIN_ZWEIGANA_API DatabaseIOError : public DatabaseError, public error::IOError {

	  public:
		enum Operation {
			OPEN,
			READ,
			WRITE,
			SYNC,
			MMAP,
			CLOSE
		};

	  private:
		const Operation operation;
		const platform::File::ErrorCode errorCode;

	  public:
		DatabaseIOError(Operation, platform::File::ErrorCode);
		DatabaseIOError(const DatabaseIOError&);

		inline Operation getOperation() const {
			return operation;
		}

		inline platform::File::ErrorCode getErrorCode() const {
			return errorCode;
		}

		REDSTRAIN_DECLARE_ERROR(DatabaseIOError)

	};

}}}

#endif /* REDSTRAIN_MOD_ZWEIGANA_CORE_DATABASEIOERROR_HPP */
