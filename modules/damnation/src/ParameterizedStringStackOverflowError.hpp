#ifndef REDSTRAIN_MOD_DAMNATION_PARAMETERIZEDSTRINGSTACKOVERFLOWERROR_HPP
#define REDSTRAIN_MOD_DAMNATION_PARAMETERIZEDSTRINGSTACKOVERFLOWERROR_HPP

#include "TParmError.hpp"

namespace redengine {
namespace damnation {

	class REDSTRAIN_DAMNATION_API ParameterizedStringStackOverflowError : public TParmError {

	  public:
		ParameterizedStringStackOverflowError();
		ParameterizedStringStackOverflowError(const ParameterizedStringStackOverflowError&);

		REDSTRAIN_DECLARE_ERROR(ParameterizedStringStackOverflowError)

	};

}}

#endif /* REDSTRAIN_MOD_DAMNATION_PARAMETERIZEDSTRINGSTACKOVERFLOWERROR_HPP */
