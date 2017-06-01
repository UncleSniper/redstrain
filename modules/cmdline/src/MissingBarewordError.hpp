#ifndef REDSTRAIN_MOD_CMDLINE_MISSINGBAREWORDERROR_HPP
#define REDSTRAIN_MOD_CMDLINE_MISSINGBAREWORDERROR_HPP

#include "CommandLineError.hpp"

namespace redengine {
namespace cmdline {

	class REDSTRAIN_CMDLINE_API MissingBarewordError : public CommandLineError {

	  private:
		const std::string word;

	  public:
		MissingBarewordError(const std::string&);
		MissingBarewordError(const MissingBarewordError&);
		virtual ~MissingBarewordError();

		inline const std::string& getWord() const {
			return word;
		}

		REDSTRAIN_DECLARE_ERROR(MissingBarewordError)

	};

}}

#endif /* REDSTRAIN_MOD_CMDLINE_MISSINGBAREWORDERROR_HPP */
