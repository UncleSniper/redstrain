#ifndef REDSTRAIN_MOD_LOCALE_L10N_HPP
#define REDSTRAIN_MOD_LOCALE_L10N_HPP

#include <redstrain/text/types.hpp>

#include "MessageLoader.hpp"

namespace redengine {
namespace locale {

	MessageLoader<text::Char16>* newCoreBlobMessageLoader16();

}}

#endif /* REDSTRAIN_MOD_LOCALE_L10N_HPP */
