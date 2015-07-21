#ifndef REDSTRAIN_MOD_TEXT_ONLINECODETABLE_HPP
#define REDSTRAIN_MOD_TEXT_ONLINECODETABLE_HPP

#include <redstrain/error/handler.hpp>
#include <redstrain/protostr/ProtocolReader.hpp>

#include "CodeTable.hpp"
#include "MappingLookupIOError.hpp"

namespace redengine {
namespace text {

	template<typename CharT>
	class OnlineCodeTable : public CodeTable<CharT> {

	  private:
		static const size_t TABLE_SIZE = static_cast<size_t>(256u) * sizeof(CharT);
		static const size_t TRIE_OFFSET = TABLE_SIZE + static_cast<size_t>(4u);
		static const size_t NODE_SIZE = static_cast<size_t>(16u * 4u);

	  private:
		io::InputStream<char>& stream;
		protostr::ProtocolReader proto;
		size_t dataOffset;

		void seekToNode(uint32_t nodeIndex, unsigned childIndex = 0u) {
			size_t offset = dataOffset + TRIE_OFFSET + static_cast<size_t>(nodeIndex) * NODE_SIZE
					+ static_cast<size_t>(childIndex * 4u);
			stream.seek(offset, io::Stream::OFFSET_FROM_START);
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

		inline size_t getDataOffset() const {
			return dataOffset;
		}

		virtual void close() {
			try {
				stream.close();
			}
			catch(const io::IOError& error) {
				error::wrapError<io::IOError, MappingLookupIOError>(error);
			}
		}

		virtual char encodeCharacter(CharT value) {
			try {
				unsigned remaining = static_cast<unsigned>(sizeof(CharT)) * 2u;
				uint32_t root = static_cast<uint32_t>(0u);
				for(; root != usdo::util::IntegerBounds<uint32_t>::MAX && remaining; --remaining) {
					seekToNode(root, static_cast<unsigned>((value >> ((remaining - 1u) * 4u))
							& static_cast<CharT>(0x0Fu)));
					proto.readUInt32(root);
				}
				if(root == usdo::util::IntegerBounds<uint32_t>::MAX)
					throw UnrepresentableCharacterError(static_cast<Char16>(value));
				seekToNode(root);
				int8_t c;
				proto.readInt8(c);
				return c;
			}
			catch(const io::IOError& error) {
				error::wrapError<io::IOError, MappingLookupIOError>(error);
				return '\0';
			}
		}

		virtual CharT decodeCharacter(char eight) {
			try {
				unsigned index = static_cast<unsigned>(static_cast<unsigned char>(eight));
				size_t offset = dataOffset + static_cast<size_t>(index) * sizeof(CharT);
				stream.seek(offset, io::Stream::OFFSET_FROM_START);
				return protostr::readProtocolPrimitive<CharT>(proto);
			}
			catch(const io::IOError& error) {
				error::wrapError<io::IOError, MappingLookupIOError>(error);
				return static_cast<CharT>(0u);
			}
		}

		virtual bool supportsConcurrentLookup() const {
			return false;
		}

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_ONLINECODETABLE_HPP */
