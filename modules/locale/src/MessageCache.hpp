#ifndef REDSTRAIN_MOD_LOCALE_MESSAGECACHE_HPP
#define REDSTRAIN_MOD_LOCALE_MESSAGECACHE_HPP

#include <vector>
#include <redstrain/text/protostr.hpp>

#include "BadMessageCacheMagicError.hpp"
#include "MessageKeyOutOfBoundsError.hpp"

namespace redengine {
namespace locale {

	template<typename CharT>
	class MessageCache {

	  public:
		static const uint32_t FORMAT_MAGIC
				= static_cast<uint32_t>(0x3D573550) + static_cast<uint32_t>(sizeof(CharT));

	  public:
		typedef std::basic_string<CharT> String;

		class Item {

		  private:
			String key;
			String value;

		  public:
			Item(const String& key, const String& value) : key(key), value(value) {}
			Item(const Item& item) : key(item.key), value(item.value) {}

			inline const String& getKey() const {
				return key;
			}

			inline const String& getValue() const {
				return value;
			}

		};

	  private:
		typedef std::vector<Item> Items;

	  public:
		typedef typename Items::const_iterator ItemIterator;

	  private:
		Items items;

	  public:
		MessageCache() {}
		MessageCache(const MessageCache& cache) : items(cache.items) {}

		void addMessage(const Item& item) {
			items.push_back(item);
		}

		void addMessage(const String& key, const String& value) {
			items.push_back(Item(key, value));
		}

		void clear() {
			items.clear();
		}

		size_t getMessageCount() const {
			return static_cast<size_t>(items.size());
		}

		void getMessages(ItemIterator& begin, ItemIterator& end) const {
			begin = items.begin();
			end = items.end();
		}

		const Item& getMessage(size_t index) const {
			if(static_cast<typename Items::size_type>(index) >= items.size())
				throw MessageKeyOutOfBoundsError(index);
			return items[index];
		}

		void saveTo(io::OutputStream<char>& stream) const {
			protostr::ProtocolWriter writer(stream);
			writer.writeUInt32(FORMAT_MAGIC);
			writer.writeUInt32(static_cast<uint32_t>(items.size()));
			ItemIterator begin(items.begin()), end(items.end());
			for(; begin != end; ++begin)
				text::writeMultiByteString<uint16_t, CharT>(writer, begin->getValue());
			writer.flush();
		}

		void loadFrom(io::InputStream<char>& stream) {
			items.clear();
			protostr::ProtocolReader reader(stream);
			if(reader.readUInt32() != FORMAT_MAGIC)
				throw BadMessageCacheMagicError();
			uint32_t size = reader.readUInt32();
			items.reserve(static_cast<typename Items::size_type>(size));
			String value;
			for(; size; --size) {
				text::readMultiByteString<uint16_t, CharT>(reader, value);
				items.push_back(Item("", value));
			}
		}

	};

}}

#endif /* REDSTRAIN_MOD_LOCALE_MESSAGECACHE_HPP */
