#ifndef REDSTRAIN_MOD_CMDLINE_EXCESSOPTIONARGUMENTERROR_HPP
#define REDSTRAIN_MOD_CMDLINE_EXCESSOPTIONARGUMENTERROR_HPP

#include "OptionError.hpp"

namespace redengine {
namespace cmdline {

	class ExcessOptionArgumentError : public OptionError {

	  public:
		ExcessOptionArgumentError(const std::string&, OptionType, char);
		ExcessOptionArgumentError(const ExcessOptionArgumentError&);

		REDSTRAIN_DECLARE_ERROR(ExcessOptionArgumentError)

	};

}}

#endif /* REDSTRAIN_MOD_CMDLINE_EXCESSOPTIONARGUMENTERROR_HPP */
