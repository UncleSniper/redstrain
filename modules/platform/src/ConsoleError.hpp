#ifndef REDSTRAIN_MOD_PLATFORM_CONSOLEERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_CONSOLEERROR_HPP

#include <redstrain/error/IOError.hpp>

#include "File.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API ConsoleError : public error::IOError {

	  public:
		enum Operation {
			CLEAR,
			SET_CURSOR_POSITION,
			SET_TEXT_ATTRIBUTE
		};

	  private:
		const File::ErrorCode code;
		const Operation operation;

	  public:
		ConsoleError(File::ErrorCode, Operation);
		ConsoleError(const ConsoleError&);

		inline File::ErrorCode getErrorCode() const {
			return code;
		}

		inline Operation getOperation() const {
			return operation;
		}

		REDSTRAIN_DECLARE_ERROR(ConsoleError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_CONSOLEERROR_HPP */
