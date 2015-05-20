#ifndef REDSTRAIN_MOD_CMDLINE_MISSINGOPTIONARGUMENTERROR_HPP
#define REDSTRAIN_MOD_CMDLINE_MISSINGOPTIONARGUMENTERROR_HPP

#include "OptionError.hpp"

namespace redengine {
namespace cmdline {

	class REDSTRAIN_CMDLINE_API MissingOptionArgumentError : public OptionError {

	  public:
		MissingOptionArgumentError(const std::string&, OptionType, char);
		MissingOptionArgumentError(const MissingOptionArgumentError&);

		REDSTRAIN_DECLARE_ERROR(MissingOptionArgumentError)

	};

}}

#endif /* REDSTRAIN_MOD_CMDLINE_MISSINGOPTIONARGUMENTERROR_HPP */
