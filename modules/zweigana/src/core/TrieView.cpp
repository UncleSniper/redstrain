#include <redstrain/util/vlarray.hpp>

#include "TrieView.hpp"
#include "Database.hpp"

using redengine::text::Char16;
using redengine::text::String16;
using redengine::util::FileSize;
using redengine::util::MemorySize;
using redengine::error::IndexOutOfBoundsError;

namespace redengine {
namespace zweigana {
namespace core {

	// ======== BinaryKey ========

	TrieView::BinaryKey::BinaryKey() {}

	TrieView::BinaryKey::BinaryKey(const BinaryKey&) {}

	TrieView::BinaryKey::~BinaryKey() {}

	// ======== String16ReferenceBinaryKey ========

	TrieView::String16ReferenceBinaryKey::String16ReferenceBinaryKey(const String16& string) : string(string) {}

	TrieView::String16ReferenceBinaryKey::String16ReferenceBinaryKey(const String16ReferenceBinaryKey& key)
			: BinaryKey(key), string(key.string) {}

	MemorySize TrieView::String16ReferenceBinaryKey::getKeyByteCount() {
		return static_cast<MemorySize>(string.length()) * static_cast<MemorySize>(2u);
	}

	char TrieView::String16ReferenceBinaryKey::getKeyByte(MemorySize index) {
		if(index >= static_cast<MemorySize>(string.length()) * static_cast<MemorySize>(2u))
			throw IndexOutOfBoundsError(index);
		Char16 c = string[static_cast<String16::size_type>(index / static_cast<MemorySize>(2u))];
		return static_cast<char>(static_cast<unsigned char>(index % static_cast<MemorySize>(2u)
				? static_cast<Char16>(c & static_cast<Char16>(0xFFu))
				: static_cast<Char16>(c >> 8)));
	}

	// ======== String16CopyBinaryKey ========

	TrieView::String16CopyBinaryKey::String16CopyBinaryKey(const String16& string) : string(string) {}

	TrieView::String16CopyBinaryKey::String16CopyBinaryKey(const String16CopyBinaryKey& key)
			: BinaryKey(key), string(key.string) {}

	MemorySize TrieView::String16CopyBinaryKey::getKeyByteCount() {
		return static_cast<MemorySize>(string.length()) * static_cast<MemorySize>(2u);
	}

	char TrieView::String16CopyBinaryKey::getKeyByte(MemorySize index) {
		if(index >= static_cast<MemorySize>(string.length()) * static_cast<MemorySize>(2u))
			throw IndexOutOfBoundsError(index);
		Char16 c = string[static_cast<String16::size_type>(index / static_cast<MemorySize>(2u))];
		return static_cast<char>(static_cast<unsigned char>(index % static_cast<MemorySize>(2u)
				? static_cast<Char16>(c & static_cast<Char16>(0xFFu))
				: static_cast<Char16>(c >> 8)));
	}

	// ======== Database ========

	TrieView::TrieView(Database& dbase, FileSize treeOffset, MemorySize valueSize)
			: dbase(dbase), treeOffset(treeOffset), valueSize(valueSize) {
		char buffer[16];
		const char* read = dbase.readBlock(buffer, treeOffset, static_cast<MemorySize>(16u));
		rootOffset = Database::getInt<FileSize>(read);
		elementCount = Database::getInt<FileSize>(read + 8);
	}

	TrieView::TrieView(Database& dbase, MemorySize valueSize) : dbase(dbase), valueSize(valueSize),
			rootOffset(Database::INVALID_OFFSET), elementCount(static_cast<FileSize>(0u)) {
		char buffer[16];
		Database::putInt<FileSize>(buffer, rootOffset);
		Database::putInt<FileSize>(buffer + 8, elementCount);
		treeOffset = dbase.writeBlock(buffer, static_cast<MemorySize>(16u));
	}

	TrieView::TrieView(const TrieView& trie) : dbase(trie.dbase), treeOffset(trie.treeOffset),
			valueSize(trie.valueSize), rootOffset(trie.rootOffset), elementCount(trie.elementCount) {}

	TrieView::~TrieView() {}

	static const MemorySize nodeBaseSize = static_cast<MemorySize>(132u);

	const char* TrieView::getImpl(BinaryKey& key, char* valueBuffer) const {
		if(!elementCount)
			return NULL;
		MemorySize bcount = key.getKeyByteCount(), nbSize = valueSize > static_cast<MemorySize>(8u)
				? valueSize : static_cast<MemorySize>(8u);
		REDSTRAIN_VLARRAY_DEFINE(char, nodeBufferBackend, nbSize)
		char* nodeBuffer = REDSTRAIN_VLARRAY_ACCESS(nodeBufferBackend);
		FileSize at = rootOffset;
		const char* read = dbase.readBlock(nodeBuffer, at, static_cast<MemorySize>(4u));
		uint32_t flags = Database::getInt<uint32_t>(read);
		MemorySize noffset, ncount = bcount * static_cast<MemorySize>(2u);
		unsigned bvalue, nibble;
		for(noffset = static_cast<MemorySize>(0u); noffset < ncount; ++noffset) {
			if(noffset % static_cast<MemorySize>(2u))
				nibble = bvalue & 0xFu;
			else {
				bvalue = static_cast<unsigned>(static_cast<unsigned char>(key.getKeyByte(noffset
						/ static_cast<MemorySize>(2u))));
				nibble = bvalue >> 4;
			}
			if(!((flags >> nibble) & static_cast<uint32_t>(1u)))
				return NULL;
			read = dbase.readBlock(nodeBuffer, at + static_cast<FileSize>(4u + nibble * 8u),
					static_cast<MemorySize>(8u));
			at = Database::getInt<FileSize>(read);
			read = dbase.readBlock(nodeBuffer, at, static_cast<MemorySize>(4u));
			flags = Database::getInt<uint32_t>(read);
		}
		if(!((flags >> 16) & static_cast<uint32_t>(1u)))
			return NULL;
		if(!valueSize)
			return valueBuffer;
		return dbase.readBlock(valueBuffer, at + static_cast<FileSize>(nodeBaseSize), valueSize);
	}

	TrieView::PutResult TrieView::putImpl(BinaryKey& key, const char* value) {
		MemorySize bcount = key.getKeyByteCount(), noffset, ncount = bcount * static_cast<MemorySize>(2u);
		MemorySize nsSize = ncount + static_cast<MemorySize>(1u);
		REDSTRAIN_VLARRAY_DEFINE(FileSize, nodeStack, nsSize)
		MemorySize prefixLength = static_cast<MemorySize>(0u);
		unsigned bvalue, nibble;
		PutResult result = CREATED;
		MemorySize nbSize = nodeBaseSize + valueSize;
		REDSTRAIN_VLARRAY_DEFINE(char, nodeBufferBackend, nbSize)
		char* nodeBuffer = REDSTRAIN_VLARRAY_ACCESS(nodeBufferBackend);
		const char* read;
		// find loop
		if(elementCount) {
			FileSize at = rootOffset;
			read = dbase.readBlock(nodeBuffer, at, static_cast<MemorySize>(4u));
			uint32_t flags = Database::getInt<uint32_t>(read);
			for(noffset = static_cast<MemorySize>(0u); noffset < ncount; ++noffset) {
				REDSTRAIN_VLARRAY_ACCESS(nodeStack)[prefixLength++] = at;
				if(noffset % static_cast<MemorySize>(2u))
					nibble = bvalue & 0xFu;
				else {
					bvalue = static_cast<unsigned>(static_cast<unsigned char>(key.getKeyByte(noffset
							/ static_cast<MemorySize>(2u))));
					nibble = bvalue >> 4;
				}
				if(!((flags >> nibble) & static_cast<uint32_t>(1u)))
					break;
				read = dbase.readBlock(nodeBuffer, at + static_cast<FileSize>(4u + nibble * 8u),
						static_cast<MemorySize>(8u));
				at = Database::getInt<FileSize>(read);
				read = dbase.readBlock(nodeBuffer, at, static_cast<MemorySize>(4u));
				flags = Database::getInt<uint32_t>(read);
			}
			if(noffset == ncount) {
				REDSTRAIN_VLARRAY_ACCESS(nodeStack)[prefixLength++] = at;
				if((flags >> 16) & static_cast<uint32_t>(1u)) {
					if(!valueSize)
						return NOT_MODIFIED;
					read = dbase.readBlock(nodeBuffer, at + static_cast<FileSize>(nodeBaseSize), valueSize);
					if(!memcmp(read, value, static_cast<size_t>(valueSize)))
						return NOT_MODIFIED;
					result = OVERWRITTEN;
				}
				else
					result = CREATED;
			}
		}
		// update loop
		if(prefixLength > ncount) {
			read = dbase.readBlock(nodeBuffer, REDSTRAIN_VLARRAY_ACCESS(nodeStack)[ncount], nodeBaseSize);
			Database::putInt<uint32_t>(nodeBuffer,
					Database::getInt<uint32_t>(read) | static_cast<uint32_t>(0x10000u));
			if(read != nodeBuffer)
				memcpy(nodeBuffer + 4, read + 4, static_cast<size_t>(128u));
		}
		else
			Database::putInt<uint32_t>(nodeBuffer, static_cast<uint32_t>(0x10000u));
		if(valueSize)
			memcpy(nodeBuffer + nodeBaseSize, value, static_cast<size_t>(valueSize));
		FileSize newRoot = dbase.writeBlock(nodeBuffer, nodeBaseSize + valueSize);
		noffset = ncount;
		while(noffset) {
			--noffset;
			if(noffset % static_cast<MemorySize>(2u)) {
				bvalue = static_cast<unsigned>(static_cast<unsigned char>(key.getKeyByte(noffset
						/ static_cast<MemorySize>(2u))));
				nibble = bvalue >> 4;
			}
			else
				nibble = bvalue & 0xFu;
			if(noffset < prefixLength) {
				read = dbase.readBlock(nodeBuffer, REDSTRAIN_VLARRAY_ACCESS(nodeStack)[noffset],
						nodeBaseSize + valueSize);
				Database::putInt<uint32_t>(nodeBuffer,
						Database::getInt<uint32_t>(read) | (static_cast<uint32_t>(1u) << nibble));
				if(read != nodeBuffer)
					memcpy(nodeBuffer + 4, read + 4,
							static_cast<size_t>(nodeBaseSize + valueSize) - static_cast<size_t>(4u));
			}
			else
				Database::putInt<uint32_t>(nodeBuffer, static_cast<uint32_t>(1u) << nibble);
			Database::putInt<FileSize>(nodeBuffer + (4u + nibble * 8u), newRoot);
			newRoot = dbase.writeBlock(nodeBuffer, nodeBaseSize + valueSize);
		}
		FileSize newElementCount = elementCount;
		if(prefixLength < ncount)
			++newElementCount;
		Database::putInt<FileSize>(nodeBuffer, newRoot);
		Database::putInt<FileSize>(nodeBuffer + 8, newElementCount);
		treeOffset = dbase.writeBlock(nodeBuffer, static_cast<MemorySize>(16u));
		rootOffset = newRoot;
		elementCount = newElementCount;
		return result;
	}

	bool TrieView::removeImpl(BinaryKey& key) {
		if(!elementCount)
			return false;
		MemorySize bcount = key.getKeyByteCount(), noffset, ncount = bcount * static_cast<MemorySize>(2u);
		MemorySize nsSize = ncount + static_cast<MemorySize>(1u);
		REDSTRAIN_VLARRAY_DEFINE(FileSize, nodeStack, nsSize)
		unsigned bvalue, nibble;
		MemorySize nbSize = nodeBaseSize + valueSize;
		REDSTRAIN_VLARRAY_DEFINE(char, nodeBufferBackend, nbSize)
		char* nodeBuffer = REDSTRAIN_VLARRAY_ACCESS(nodeBufferBackend);
		// descend
		FileSize at = rootOffset;
		const char* read = dbase.readBlock(nodeBuffer, at, static_cast<MemorySize>(4u));
		uint32_t flags = Database::getInt<uint32_t>(read);
		for(noffset = static_cast<MemorySize>(0u); noffset < ncount; ++noffset) {
			REDSTRAIN_VLARRAY_ACCESS(nodeStack)[noffset] = at;
			if(noffset % static_cast<MemorySize>(2u))
				nibble = bvalue & 0xFu;
			else {
				bvalue = static_cast<unsigned>(static_cast<unsigned char>(key.getKeyByte(noffset
						/ static_cast<MemorySize>(2u))));
				nibble = bvalue >> 4;
			}
			if(!((flags >> nibble) & static_cast<uint32_t>(1u)))
				return false;
			read = dbase.readBlock(nodeBuffer, at + static_cast<FileSize>(4u + nibble * 8u),
					static_cast<MemorySize>(8u));
			at = Database::getInt<FileSize>(read);
			read = dbase.readBlock(nodeBuffer, at, static_cast<MemorySize>(4u));
			flags = Database::getInt<uint32_t>(read);
		}
		if(!((flags >> 16) & static_cast<uint32_t>(1u)))
			return false;
		// ascend
		FileSize newRoot;
		if(flags & static_cast<uint32_t>(0x10000u)) {
			read = dbase.readBlock(nodeBuffer, at, nodeBaseSize + valueSize);
			Database::putInt<uint32_t>(nodeBuffer,
					Database::getInt<uint32_t>(read) & ~static_cast<uint32_t>(0x10000u));
			if(read != nodeBuffer)
				memcpy(nodeBuffer + 4, read + 4, static_cast<size_t>(128u));
			newRoot = dbase.writeBlock(nodeBuffer, nodeBaseSize + valueSize);
		}
		else
			newRoot = Database::INVALID_OFFSET;
		noffset = ncount;
		while(noffset) {
			--noffset;
			if(noffset % static_cast<MemorySize>(2u)) {
				bvalue = static_cast<unsigned>(static_cast<unsigned char>(key.getKeyByte(noffset
						/ static_cast<MemorySize>(2u))));
				nibble = bvalue >> 4;
			}
			else
				nibble = bvalue & 0xFu;
			read = dbase.readBlock(nodeBuffer, REDSTRAIN_VLARRAY_ACCESS(nodeStack)[noffset],
					nodeBaseSize + valueSize);
			if(newRoot == Database::INVALID_OFFSET) {
				flags = Database::getInt<uint32_t>(read) & ~(static_cast<uint32_t>(1u) << nibble);
				if(flags) {
					Database::putInt<uint32_t>(nodeBuffer, flags);
					if(read != nodeBuffer)
						memcpy(nodeBuffer + 4, read + 4,
								static_cast<size_t>(nodeBaseSize + valueSize) - static_cast<size_t>(4u));
					newRoot = dbase.writeBlock(nodeBuffer, nodeBaseSize + valueSize);
				}
			}
			else {
				if(read != nodeBuffer)
					memcpy(nodeBuffer, read, static_cast<size_t>(nodeBaseSize + valueSize));
				newRoot = dbase.writeBlock(nodeBuffer, nodeBaseSize + valueSize);
			}
		}
		FileSize newElementCount = elementCount - static_cast<FileSize>(1u);
		Database::putInt<FileSize>(nodeBuffer, newRoot);
		Database::putInt<FileSize>(nodeBuffer + 8, newElementCount);
		treeOffset = dbase.writeBlock(nodeBuffer, static_cast<MemorySize>(16u));
		rootOffset = newRoot;
		elementCount = newElementCount;
		return true;
	}

	void TrieView::clear() {
		char buffer[16];
		FileSize newElementCount = static_cast<FileSize>(0u);
		Database::putInt<FileSize>(buffer, Database::INVALID_OFFSET);
		Database::putInt<FileSize>(buffer + 8, newElementCount);
		treeOffset = dbase.writeBlock(buffer, static_cast<MemorySize>(8u));
		rootOffset = Database::INVALID_OFFSET;
		elementCount = newElementCount;
	}

}}}
