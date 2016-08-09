#ifndef REDSTRAIN_MOD_DAMNATION_PARAMETERIZEDSTRINGSTACKUNDERFLOWERROR_HPP
#define REDSTRAIN_MOD_DAMNATION_PARAMETERIZEDSTRINGSTACKUNDERFLOWERROR_HPP

#include "TParmError.hpp"

namespace redengine {
namespace damnation {

	class REDSTRAIN_DAMNATION_API ParameterizedStringStackUnderflowError : public TParmError {

	  public:
		ParameterizedStringStackUnderflowError();
		ParameterizedStringStackUnderflowError(const ParameterizedStringStackUnderflowError&);

		REDSTRAIN_DECLARE_ERROR(ParameterizedStringStackUnderflowError)

	};

}}

#endif /* REDSTRAIN_MOD_DAMNATION_PARAMETERIZEDSTRINGSTACKUNDERFLOWERROR_HPP */
