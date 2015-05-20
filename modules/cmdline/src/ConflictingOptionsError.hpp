#ifndef REDSTRAIN_MOD_CMDLINE_CONFLICTINGOPTIONSERROR_HPP
#define REDSTRAIN_MOD_CMDLINE_CONFLICTINGOPTIONSERROR_HPP

#include "CommandLineError.hpp"

namespace redengine {
namespace cmdline {

	class REDSTRAIN_CMDLINE_API ConflictingOptionsError : public CommandLineError {

	  private:
		const std::string nameA, nameB;

	  public:
		ConflictingOptionsError(const std::string&, const std::string&);
		ConflictingOptionsError(const ConflictingOptionsError&);

		inline const std::string& getOptionAName() const {
			return nameA;
		}

		inline const std::string& getOptionBName() const {
			return nameB;
		}

		REDSTRAIN_DECLARE_ERROR(ConflictingOptionsError)

	};

}}

#endif /* REDSTRAIN_MOD_CMDLINE_CONFLICTINGOPTIONSERROR_HPP */
