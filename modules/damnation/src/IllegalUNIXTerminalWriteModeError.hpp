#ifndef REDSTRAIN_MOD_DAMNATION_ILLEGALUNIXTERMINALWRITEMODE_HPP
#define REDSTRAIN_MOD_DAMNATION_ILLEGALUNIXTERMINALWRITEMODE_HPP

#include <redstrain/error/IllegalArgumentError.hpp>

#include "DamnError.hpp"
#include "UNIXTerminalBinding.hpp"

namespace redengine {
namespace damnation {

	class REDSTRAIN_DAMNATION_API IllegalUNIXTerminalWriteModeError
			: public error::IllegalArgumentError, public DamnError {

	  private:
		const UNIXTerminalBinding::WriteMode mode;

	  public:
		IllegalUNIXTerminalWriteModeError(UNIXTerminalBinding::WriteMode);
		IllegalUNIXTerminalWriteModeError(const IllegalUNIXTerminalWriteModeError&);

		inline UNIXTerminalBinding::WriteMode getWriteMode() const {
			return mode;
		}

		REDSTRAIN_DECLARE_ERROR(IllegalUNIXTerminalWriteModeError)

	};

}}

#endif /* REDSTRAIN_MOD_DAMNATION_ILLEGALUNIXTERMINALWRITEMODE_HPP */
