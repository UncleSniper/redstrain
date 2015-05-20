#ifndef REDSTRAIN_MOD_CMDLINE_OPTIONERROR_HPP
#define REDSTRAIN_MOD_CMDLINE_OPTIONERROR_HPP

#include "CommandLineError.hpp"

namespace redengine {
namespace cmdline {

	class OptionError : public CommandLineError {

	  public:
		enum OptionType {
			SINGLE_OPTION,
			DOUBLE_OPTION
		};

	  protected:
		const std::string name;
		const OptionType type;
		const char initiatorSymbol;

	  protected:
		void printOptionErrorMessage(std::ostream&) const;

	  public:
		OptionError(const std::string&, OptionType, char);
		OptionError(const OptionError&);

		inline const std::string& getOptionName() const {
			return name;
		}

		inline OptionType getOptionType() const {
			return type;
		}

		inline char getInitiatorSymbol() const {
			return initiatorSymbol;
		}

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(OptionError)

	};

}}

#endif /* REDSTRAIN_MOD_CMDLINE_OPTIONERROR_HPP */
