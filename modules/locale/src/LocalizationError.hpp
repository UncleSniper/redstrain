#ifndef REDSTRAIN_MOD_LOCALE_LOCALIZATIONERROR_HPP
#define REDSTRAIN_MOD_LOCALE_LOCALIZATIONERROR_HPP

#include <redstrain/error/Error.hpp>

#include "api.hpp"

namespace redengine {
namespace locale {

	class REDSTRAIN_LOCALE_API LocalizationError : public virtual error::Error {

	  public:
		LocalizationError();
		LocalizationError(const LocalizationError&);

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(LocalizationError)

	};

}}

#endif /* REDSTRAIN_MOD_LOCALE_LOCALIZATIONERROR_HPP */
