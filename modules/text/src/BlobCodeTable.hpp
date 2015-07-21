#ifndef REDSTRAIN_MOD_TEXT_BLOBCODETABLE_HPP
#define REDSTRAIN_MOD_TEXT_BLOBCODETABLE_HPP

#include <cstring>
#include <redstrain/platform/Endianness.hpp>

#include "types.hpp"
#include "CodeTable.hpp"
#include "UnrepresentableCharacterError.hpp"

namespace redengine {
namespace text {

	template<typename CharT>
	class BlobCodeTable : public CodeTable<CharT> {

	  private:
		static const size_t TABLE_SIZE = static_cast<size_t>(256u) * sizeof(CharT);
		static const size_t TRIE_OFFSET = TABLE_SIZE + static_cast<size_t>(4u);
		static const size_t NODE_SIZE = static_cast<size_t>(16u * 4u);

	  private:
		const char *const data;
		const size_t size;

	  private:
		const char* seekToNode(uint32_t nodeIndex, unsigned childIndex = 0u) const {
			size_t offset = TRIE_OFFSET + static_cast<size_t>(nodeIndex) * NODE_SIZE
					+ static_cast<size_t>(childIndex * 4u);
			return data + offset;
		}

	  public:
		BlobCodeTable(const char* data, size_t size) : data(data), size(size) {}
		BlobCodeTable(const BlobCodeTable& table) : CodeTable<CharT>(table), data(table.data), size(table.size) {}

		inline const char* getBlobData() const {
			return data;
		}

		inline size_t getBlobSize() const {
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
