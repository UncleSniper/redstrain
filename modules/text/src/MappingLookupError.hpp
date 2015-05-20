#ifndef REDSTRAIN_MOD_TEXT_MAPPINGLOOKUPERROR_HPP
#define REDSTRAIN_MOD_TEXT_MAPPINGLOOKUPERROR_HPP

#include "TextError.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API MappingLookupError : public TextError {

	  public:
		MappingLookupError();
		MappingLookupError(const MappingLookupError& error);

		REDSTRAIN_DECLARE_ERROR(MappingLookupError)

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_MAPPINGLOOKUPERROR_HPP */
