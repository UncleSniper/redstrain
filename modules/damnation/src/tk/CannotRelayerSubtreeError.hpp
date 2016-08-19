#ifndef REDSTRAIN_MOD_DAMNATION_TK_CANNOTRELAYERSUBTREEERROR_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_CANNOTRELAYERSUBTREEERROR_HPP

#include <redstrain/error/StateError.hpp>

#include "../DamnError.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class REDSTRAIN_DAMNATION_API CannotRelayerSubtreeError : public DamnError, public error::StateError {

	  public:
		CannotRelayerSubtreeError();
		CannotRelayerSubtreeError(const CannotRelayerSubtreeError&);

		REDSTRAIN_DECLARE_ERROR(CannotRelayerSubtreeError)

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_CANNOTRELAYERSUBTREEERROR_HPP */
