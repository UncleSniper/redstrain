#ifndef REDSTRAIN_MOD_TEXT_L10N_HPP
#define REDSTRAIN_MOD_TEXT_L10N_HPP

#include <redstrain/error/Error.hpp>

#include "api.hpp"
#include "types.hpp"

namespace redengine {
namespace text {

	REDSTRAIN_TEXT_API std::string guessSystemLocale();
	REDSTRAIN_TEXT_API std::string guessTerminalCharset();

	REDSTRAIN_TEXT_API String16 getErrorMessage(const error::Error&, const locale::Locale&);

}}

#endif /* REDSTRAIN_MOD_TEXT_L10N_HPP */
