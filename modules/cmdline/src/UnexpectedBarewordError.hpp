#ifndef REDSTRAIN_MOD_CMDLINE_UNEXPECTEDBAREWORDERROR_HPP
#define REDSTRAIN_MOD_CMDLINE_UNEXPECTEDBAREWORDERROR_HPP

#include "CommandLineError.hpp"

namespace redengine {
namespace cmdline {

	class UnexpectedBarewordError : public CommandLineError {

	  private:
		const std::string word;

	  public:
		UnexpectedBarewordError(const std::string&);
		UnexpectedBarewordError(const UnexpectedBarewordError&);

		inline const std::string& getWord() const {
			return word;
		}

		REDSTRAIN_DECLARE_ERROR(UnexpectedBarewordError)

	};

}}

#endif /* REDSTRAIN_MOD_CMDLINE_UNEXPECTEDBAREWORDERROR_HPP */
