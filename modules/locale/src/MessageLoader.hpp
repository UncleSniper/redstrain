#ifndef REDSTRAIN_MOD_LOCALE_MESSAGELOADER_HPP
#define REDSTRAIN_MOD_LOCALE_MESSAGELOADER_HPP

#include <redstrain/util/ReferenceCounted.hpp>

#include "MessageCache.hpp"

namespace redengine {
namespace locale {

	class Locale;

	template<typename CharT>
	class MessageLoader : public util::ReferenceCounted {

	  public:
		MessageLoader() {}
		MessageLoader(const MessageLoader& loader) : ReferenceCounted(loader) {}

		virtual MessageCache<CharT>* loadMessages(const Locale&) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_LOCALE_MESSAGELOADER_HPP */
