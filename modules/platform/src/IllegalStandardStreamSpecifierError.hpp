#ifndef REDSTRAIN_MOD_PLATFORM_ILLEGALSTANDARDSTREAMSPECIFIER_HPP
#define REDSTRAIN_MOD_PLATFORM_ILLEGALSTANDARDSTREAMSPECIFIER_HPP

#include <redstrain/error/IllegalArgumentError.hpp>

#include "Console.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API IllegalStandardStreamSpecifierError : public error::IllegalArgumentError {

	  private:
		const Console::StandardHandle specifier;

	  public:
		IllegalStandardStreamSpecifierError(Console::StandardHandle);
		IllegalStandardStreamSpecifierError(const IllegalStandardStreamSpecifierError&);

		inline Console::StandardHandle getStreamSpecifier() const {
			return specifier;
		}

		REDSTRAIN_DECLARE_ERROR(IllegalStandardStreamSpecifierError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_ILLEGALSTANDARDSTREAMSPECIFIER_HPP */
