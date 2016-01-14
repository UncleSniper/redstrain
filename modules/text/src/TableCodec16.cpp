#include <redstrain/platform/ObjectLocker.hpp>

#include "TableCodec16.hpp"

using redengine::util::MemorySize;
using redengine::platform::ObjectLocker;

namespace redengine {
namespace text {

	TableCodec16::TableCodec16(CodeTable<Char16>& table) : table(table) {
		ObjectLocker<CodeTable<Char16> > locker(&table);
		table.ref();
		locker.release();
	}

	TableCodec16::TableCodec16(const TableCodec16& codec) : Encoder16(codec), Decoder16(codec), table(codec.table) {
		ObjectLocker<CodeTable<Char16> > locker(&table);
		table.ref();
		locker.release();
	}

	TableCodec16::~TableCodec16() {
		ObjectLocker<CodeTable<Char16> > locker(&table);
		table.unref();
	}

	MemorySize TableCodec16::encodeBlock(const Char16* input, MemorySize insize, char* output, MemorySize outsize,
			MemorySize& outcount) {
		if(outsize < insize)
			insize = outsize;
		ObjectLocker<CodeTable<Char16> > locker(table.supportsConcurrentLookup() ? NULL : &table);
		for(outcount = static_cast<MemorySize>(0u); outcount < insize; ++outcount)
			output[outcount] = table.encodeCharacter(input[outcount]);
		locker.release();
		return insize;
	}

	MemorySize TableCodec16::decodeBlock(const char* input, MemorySize insize, Char16* output, MemorySize outsize,
			MemorySize& outcount) {
		if(outsize < insize)
			insize = outsize;
		ObjectLocker<CodeTable<Char16> > locker(table.supportsConcurrentLookup() ? NULL : &table);
		for(outcount = static_cast<MemorySize>(0u); outcount < insize; ++outcount)
			output[outcount] = table.decodeCharacter(input[outcount]);
		locker.release();
		return insize;
	}

}}
