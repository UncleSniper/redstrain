#ifndef REDSTRAIN_MOD_TEXT_L10N_HPP
#define REDSTRAIN_MOD_TEXT_L10N_HPP

#include <redstrain/error/Error.hpp>

#include "types.hpp"

namespace redengine {
namespace text {

	std::string guessSystemLocale();
	std::string guessTerminalCharset();

	String16 getErrorMessage(const error::Error&, const locale::Locale&);

}}

#endif /* REDSTRAIN_MOD_TEXT_L10N_HPP */
