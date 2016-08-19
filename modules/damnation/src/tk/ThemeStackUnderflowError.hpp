#ifndef REDSTRAIN_MOD_DAMNATION_TK_THEMESTACKUNDERFLOWERROR_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_THEMESTACKUNDERFLOWERROR_HPP

#include <redstrain/error/StateError.hpp>

#include "../DamnError.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class REDSTRAIN_DAMNATION_API ThemeStackUnderflowError : public DamnError, public error::StateError {

	  public:
		ThemeStackUnderflowError();
		ThemeStackUnderflowError(const ThemeStackUnderflowError&);

		REDSTRAIN_DECLARE_ERROR(ThemeStackUnderflowError)

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_THEMESTACKUNDERFLOWERROR_HPP */
