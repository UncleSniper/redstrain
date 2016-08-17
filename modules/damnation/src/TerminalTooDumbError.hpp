#ifndef REDSTRAIN_MOD_DAMNATION_TERMINALTOODUMBERROR_HPP
#define REDSTRAIN_MOD_DAMNATION_TERMINALTOODUMBERROR_HPP

#include "DamnError.hpp"

namespace redengine {
namespace damnation {

	class REDSTRAIN_DAMNATION_API TerminalTooDumbError : public DamnError {

	  public:
		TerminalTooDumbError();
		TerminalTooDumbError(const TerminalTooDumbError&);

		REDSTRAIN_DECLARE_ERROR(TerminalTooDumbError)

	};

}}

#endif /* REDSTRAIN_MOD_DAMNATION_TERMINALTOODUMBERROR_HPP */
