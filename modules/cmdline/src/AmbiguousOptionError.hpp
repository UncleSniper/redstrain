#ifndef REDSTRAIN_MOD_CMDLINE_AMBIGUOUSOPTIONERROR_HPP
#define REDSTRAIN_MOD_CMDLINE_AMBIGUOUSOPTIONERROR_HPP

#include "OptionError.hpp"

namespace redengine {
namespace cmdline {

	class AmbiguousOptionError : public OptionError {

	  public:
		AmbiguousOptionError(const std::string&, char);
		AmbiguousOptionError(const AmbiguousOptionError&);

		REDSTRAIN_DECLARE_ERROR(AmbiguousOptionError)

	};

}}

#endif /* REDSTRAIN_MOD_CMDLINE_AMBIGUOUSOPTIONERROR_HPP */
