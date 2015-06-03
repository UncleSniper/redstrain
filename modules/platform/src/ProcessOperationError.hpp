#ifndef REDSTRAIN_MOD_PLATFORM_PROCESSOPERATIONERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_PROCESSOPERATIONERROR_HPP

#include "Process.hpp"
#include "ProcessError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API ProcessOperationError : public ProcessError {

	  public:
		enum Operation {
			START,
			TERMINATE,
			KILL,
			INTERRUPT,
			WAIT
		};

	  private:
		const Operation operation;
		const Process::ErrorCode code;

	  public:
		ProcessOperationError(Operation, Process::ErrorCode);
		ProcessOperationError(const ProcessOperationError&);

		inline Operation getOperation() const {
			return operation;
		}

		inline Process::ErrorCode getErrorCode() const {
			return code;
		}

		REDSTRAIN_DECLARE_ERROR(ProcessOperationError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_PROCESSOPERATIONERROR_HPP */
