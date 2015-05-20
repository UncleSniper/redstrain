#ifndef REDSTRAIN_MOD_CMDLINE_UNRECOGNIZEDOPTIONERROR_HPP
#define REDSTRAIN_MOD_CMDLINE_UNRECOGNIZEDOPTIONERROR_HPP

#include "OptionError.hpp"

namespace redengine {
namespace cmdline {

	class REDSTRAIN_CMDLINE_API UnrecognizedOptionError : public OptionError {

	  public:
		UnrecognizedOptionError(const std::string&, OptionType, char);
		UnrecognizedOptionError(const UnrecognizedOptionError&);

		REDSTRAIN_DECLARE_ERROR(UnrecognizedOptionError)

	};

}}

#endif /* REDSTRAIN_MOD_CMDLINE_UNRECOGNIZEDOPTIONERROR_HPP */
