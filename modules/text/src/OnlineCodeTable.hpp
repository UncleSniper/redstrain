#ifndef REDSTRAIN_MOD_TEXT_ONLINECODETABLE_HPP
#define REDSTRAIN_MOD_TEXT_ONLINECODETABLE_HPP

#include <redstrain/error/handler.hpp>
#include <redstrain/util/IntegerBounds.hpp>
#include <redstrain/protostr/ProtocolReader.hpp>

#include "types.hpp"
#include "CodeTable.hpp"
#include "MappingLookupIOError.hpp"
#include "UnrepresentableCharacterError.hpp"

namespace redengine {
namespace text {

	template<typename CharT>
	class OnlineCodeTable : public CodeTable<CharT> {

	  private:
		static const util::FileSize TABLE_SIZE = static_cast<util::FileSize>(256u) * sizeof(CharT);
		static const util::FileSize TRIE_OFFSET = TABLE_SIZE + static_cast<util::FileSize>(4u);
		static const util::FileSize NODE_SIZE = static_cast<util::FileSize>(16u * 4u);

	  private:
		io::InputStream<char>& stream;
		protostr::ProtocolReader proto;
		util::FileSize dataOffset;

		void seekToNode(uint32_t nodeIndex, unsigned childIndex = 0u) {
			util::FileSize offset = dataOffset + TRIE_OFFSET + static_cast<util::FileSize>(nodeIndex) * NODE_SIZE
					+ static_cast<util::FileSize>(childIndex * 4u);
			stream.seek(static_cast<util::FileOffset>(offset), io::Stream::OFFSET_FROM_START);
		}

	  public:
		OnlineCodeTable(io::InputStream<char>& stream) : stream(stream), proto(stream), dataOffset(stream.tell()) {}
		OnlineCodeTable(const OnlineCodeTable& table)
				: CodeTable<CharT>(table), stream(table.stream), proto(stream), dataOffset(table.dataOffset) {}

		inline io::InputStream<char>& getInputStream() {
			return stream;
		}

		inline const io::InputStream<char>& getInputStream() const {
			return stream;
		}

		inline util::FileSize getDataOffset() const {
			return dataOffset;
		}

		virtual void close() {
			try {
				stream.close();
			}
			catch(const error::IOError& error) {
				error::wrapError<error::IOError, MappingLookupIOError>(error);
			}
		}

		virtual char encodeCharacter(CharT value) {
			try {
				unsigned remaining = static_cast<unsigned>(sizeof(CharT)) * 2u;
				uint32_t root = static_cast<uint32_t>(0u);
				for(; root != util::IntegerBounds<uint32_t>::MAX && remaining; --remaining) {
					seekToNode(root, static_cast<unsigned>((value >> ((remaining - 1u) * 4u))
							& static_cast<CharT>(0x0Fu)));
					root = proto.readUInt32();
				}
				if(root == util::IntegerBounds<uint32_t>::MAX)
					throw UnrepresentableCharacterError(static_cast<Char16>(value));
				seekToNode(root);
				return static_cast<char>(proto.readInt8());
			}
			catch(const error::IOError& error) {
				error::wrapError<error::IOError, MappingLookupIOError>(error);
				return '\0';
			}
		}

		virtual CharT decodeCharacter(char eight) {
			try {
				unsigned index = static_cast<unsigned>(static_cast<unsigned char>(eight));
				util::FileSize offset = dataOffset
						+ static_cast<util::FileSize>(index) * static_cast<util::FileSize>(sizeof(CharT));
				stream.seek(static_cast<util::FileOffset>(offset), io::Stream::OFFSET_FROM_START);
				return protostr::readProtocolPrimitive<CharT>(proto);
			}
			catch(const error::IOError& error) {
				error::wrapError<error::IOError, MappingLookupIOError>(error);
				return static_cast<CharT>(0u);
			}
		}

		virtual bool supportsConcurrentLookup() const {
			return false;
		}

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_ONLINECODETABLE_HPP */
