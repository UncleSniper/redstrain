#ifndef REDSTRAIN_MOD_TEXT_CACHEDCODETABLE_HPP
#define REDSTRAIN_MOD_TEXT_CACHEDCODETABLE_HPP

#include <map>
#include <cstring>
#include <redstrain/util/Delete.hpp>
#include <redstrain/protostr/ProtocolReader.hpp>
#include <redstrain/protostr/ProtocolWriter.hpp>

#include "CodeTable.hpp"
#include "UnrepresentableCharacterError.hpp"

namespace redengine {
namespace text {

	template<typename CharT>
	class CachedCodeTable : public CodeTable<CharT> {

	  private:
		struct Trie {

			bool isLeaf;
			union {
				Trie* children[16];
				char value;
			} data;

			Trie() : isLeaf(false) {
				memset(data.children, 0, sizeof(data.children));
			}

			Trie(char value) : isLeaf(true) {
				data.value = value;
			}

			~Trie() {
				if(isLeaf)
					return;
				unsigned u;
				for(u = 0u; u < 16u; ++u) {
					if(data.children[u])
						delete data.children[u];
				}
			}

			Trie* clone() const {
				if(isLeaf)
					return new Trie(data.value);
				util::Delete<Trie> trie(new Trie);
				unsigned u;
				for(u = 0u; u < 16u; ++u)
					if(data.children[u])
						trie->data.children[u] = data.children[u]->clone();
				return trie.set();
			}

			void mapPointers(std::map<const Trie*, uint32_t>& map) const {
				uint32_t id = static_cast<uint32_t>(map.size());
				map[this] = id;
				if(isLeaf)
					return;
				unsigned u;
				for(u = 0u; u < 16u; ++u)
					if(data.children[u])
						data.children[u]->mapPointers(map);
			}

			void saveTo(protostr::ProtocolWriter& proto, std::map<const Trie*, uint32_t>& ids) {
				unsigned u;
				if(isLeaf) {
					proto.writeInt8(data.value);
					for(u = 0u; u < 3u; ++u)
						proto.writeInt8(static_cast<int8_t>(0));
					for(u = 0u; u < 15u; ++u)
						proto.writeUInt32(static_cast<uint32_t>(0u));
				}
				else {
					for(u = 0u; u < 15u; ++u)
						proto.writeUInt32(data.children[u] ? ids[data.children[u]]
								: util::IntegerBounds<uint32_t>::MAX);
				}
			}

		};

	  private:
		CharT decTable[256];
		Trie* encTrie;

	  public:
		CachedCodeTable() : encTrie(NULL) {
			memset(decTable, 0, sizeof(decTable));
		}

		CachedCodeTable(const CachedCodeTable& table)
				: CodeTable<CharT>(table), encTrie(table.encTrie ? table.encTrie->clone() : NULL) {
			memcpy(decTable, table.decTable, sizeof(decTable));
		}

		virtual ~CachedCodeTable() {
			if(encTrie)
				delete encTrie;
		}

		void map(char eight, CharT value) {
			decTable[static_cast<unsigned>(static_cast<unsigned char>(eight))] = value;
			Trie** root = &encTrie;
			unsigned remaining = static_cast<unsigned>(sizeof(CharT)) * 2u;
			for(; remaining; --remaining) {
				if(!*root)
					*root = new Trie;
				root = (*root)->data.children + ((value >> ((remaining - 1u) * 4u)) & static_cast<CharT>(0x0Fu));
				value >>= 4;
			}
			*root = new Trie(eight);
		}

		virtual char encodeCharacter(CharT value) {
			const Trie* root = encTrie;
			unsigned remaining = static_cast<unsigned>(sizeof(CharT)) * 2u;
			for(; root && remaining; --remaining)
				root = root->data.children[(value >> ((remaining - 1u) * 4u)) & 0x0Fu];
			if(!root)
				throw UnrepresentableCharacterError(static_cast<Char16>(value));
			return root->data.value;
		}

		virtual CharT decodeCharacter(char eight) {
			return decTable[static_cast<unsigned>(static_cast<unsigned char>(eight))];
		}

		void saveTo(io::OutputStream<char>& binaryOutput) const {
			protostr::ProtocolWriter proto(binaryOutput);
			unsigned u;
			for(u = 0u; u < 256u; ++u)
				protostr::writeProtocolPrimitive<CharT>(proto, decTable[u]);
			std::map<const Trie*, uint32_t> ids;
			if(encTrie)
				encTrie->mapPointers(ids);
			proto.writeUInt32(static_cast<uint32_t>(ids.size()));
			if(encTrie)
				encTrie->saveTo(proto, ids);
			else {
				for(u = 0u; u < 15u; ++u)
					proto.writeUInt32(util::IntegerBounds<uint32_t>::MAX);
			}
		}

		void loadFrom(io::InputStream<char>& binaryInput) {
			protostr::ProtocolReader proto(binaryInput);
			unsigned u;
			CharT c;
			for(u = 0u; u < 256u; ++u) {
				protostr::readProtocolPrimitive<CharT>(proto, c);
				map(static_cast<char>(static_cast<unsigned char>(u)), c);
			}
		}

		virtual bool supportsConcurrentLookup() const {
			return true;
		}

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_CACHEDCODETABLE_HPP */
