#ifndef REDSTRAIN_MOD_LOCALE_BLOBMESSAGELOADER_HPP
#define REDSTRAIN_MOD_LOCALE_BLOBMESSAGELOADER_HPP

#include <redstrain/util/Delete.hpp>
#include <redstrain/io/MemoryInputStream.hpp>

#include "Locale.hpp"
#include "BlobMessageMapping.hpp"

namespace redengine {
namespace locale {

	template<typename CharT>
	class BlobMessageLoader : public MessageLoader<CharT> {

	  private:
		BlobMessageMapping& mapping;

	  public:
		BlobMessageLoader(BlobMessageMapping& mapping);
		BlobMessageLoader(const BlobMessageLoader&);

		inline BlobMessageMapping& getMapping() {
			return mapping;
		}

		inline const BlobMessageMapping& getMapping() const {
			return mapping;
		}

		virtual MessageCache<CharT>* loadMessages(const Locale& locale) {
			const BlobMessageMapping::Blob* blob = mapping.getBlob(locale.getLanguage(), locale.getCountry());
			if(!blob)
				return NULL;
			util::Delete<MessageCache<CharT> > cache(new MessageCache<CharT>);
			io::MemoryInputStream<char> input(blob->getData(), blob->getSize());
			cache->loadFrom(input);
			return cache.set();
		}

	};

}}

#endif /* REDSTRAIN_MOD_LOCALE_BLOBMESSAGELOADER_HPP */
