#ifndef REDSTRAIN_MOD_DAMNATION_PARAMETERIZEDSTRINGENDSWITHINFORMATTINGERROR_HPP
#define REDSTRAIN_MOD_DAMNATION_PARAMETERIZEDSTRINGENDSWITHINFORMATTINGERROR_HPP

#include "TParmError.hpp"

namespace redengine {
namespace damnation {

	class REDSTRAIN_DAMNATION_API ParameterizedStringEndsWithinFormattingError : public TParmError {

	  public:
		ParameterizedStringEndsWithinFormattingError();
		ParameterizedStringEndsWithinFormattingError(const ParameterizedStringEndsWithinFormattingError&);

		REDSTRAIN_DECLARE_ERROR(ParameterizedStringEndsWithinFormattingError)

	};

}}

#endif /* REDSTRAIN_MOD_DAMNATION_PARAMETERIZEDSTRINGENDSWITHINFORMATTINGERROR_HPP */
