#ifndef REDSTRAIN_MOD_DAMNATION_ILLEGALFORMATTINGINPARAMETERIZEDSTRINGERROR_HPP
#define REDSTRAIN_MOD_DAMNATION_ILLEGALFORMATTINGINPARAMETERIZEDSTRINGERROR_HPP

#include "TParmError.hpp"

namespace redengine {
namespace damnation {

	class REDSTRAIN_DAMNATION_API IllegalFormattingInParameterizedStringError : public TParmError {

	  public:
		IllegalFormattingInParameterizedStringError();
		IllegalFormattingInParameterizedStringError(const IllegalFormattingInParameterizedStringError&);

		REDSTRAIN_DECLARE_ERROR(IllegalFormattingInParameterizedStringError)

	};

}}

#endif /* REDSTRAIN_MOD_DAMNATION_ILLEGALFORMATTINGINPARAMETERIZEDSTRINGERROR_HPP */
