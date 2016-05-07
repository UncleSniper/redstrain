#ifndef REDSTRAIN_MOD_TEXT_BLOBCODETABLE_HPP
#define REDSTRAIN_MOD_TEXT_BLOBCODETABLE_HPP

#include <cstring>
#include <redstrain/util/types.hpp>
#include <redstrain/platform/Endianness.hpp>

#include "CodeTable.hpp"
#include "UnrepresentableCharacterError.hpp"
#include "types.hpp"

namespace redengine {
namespace text {

	template<typename CharT>
	class BlobCodeTable : public CodeTable<CharT> {

	  private:
		static const util::MemorySize TABLE_SIZE
				= static_cast<util::MemorySize>(256u) * static_cast<util::MemorySize>(sizeof(CharT));
		static const util::MemorySize TRIE_OFFSET = TABLE_SIZE + static_cast<util::MemorySize>(4u);
		static const util::MemorySize NODE_SIZE = static_cast<util::MemorySize>(16u * 4u);

	  private:
		const char *const data;
		const util::MemorySize size;

	  private:
		const char* seekToNode(uint32_t nodeIndex, unsigned childIndex = 0u) const {
			util::MemorySize offset = TRIE_OFFSET + static_cast<util::MemorySize>(nodeIndex) * NODE_SIZE
					+ static_cast<util::MemorySize>(childIndex * 4u);
			return data + offset;
		}

	  public:
		BlobCodeTable(const char* data, util::MemorySize size) : data(data), size(size) {}
		BlobCodeTable(const BlobCodeTable& table) : CodeTable<CharT>(table), data(table.data), size(table.size) {}

		inline const char* getBlobData() const {
			return data;
		}

		inline util::MemorySize getBlobSize() const {
			return size;
		}

		virtual char encodeCharacter(CharT value) {
			unsigned remaining = static_cast<unsigned>(sizeof(CharT)) * 2u;
			uint32_t root = static_cast<uint32_t>(0u);
			for(; root != util::IntegerBounds<uint32_t>::MAX && remaining; --remaining) {
				memcpy(&root, seekToNode(root, static_cast<unsigned>((value >> ((remaining - 1u) * 4u))
						& static_cast<CharT>(0x0Fu))), sizeof(uint32_t));
				root = platform::Endianness<uint32_t>::convertBig(root);
			}
			if(root == util::IntegerBounds<uint32_t>::MAX)
				throw UnrepresentableCharacterError(static_cast<Char16>(value));
			return *seekToNode(root);
		}

		virtual CharT decodeCharacter(char eight) {
			unsigned index = static_cast<unsigned>(static_cast<unsigned char>(eight));
			size_t offset = static_cast<size_t>(index) * sizeof(CharT);
			CharT c;
			memcpy(&c, data + offset, sizeof(CharT));
			return platform::Endianness<CharT>::convertBig(c);
		}

		virtual bool supportsConcurrentLookup() const {
			return true;
		}

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_BLOBCODETABLE_HPP */
