#include <redstrain/platform/ObjectLocker.hpp>

#include "TableCodec16.hpp"

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

	size_t TableCodec16::encodeBlock(const Char16* input, size_t insize, char* output, size_t outsize,
			size_t& outcount) {
		if(outsize < insize)
			insize = outsize;
		ObjectLocker<CodeTable<Char16> > locker(table.supportsConcurrentLookup() ? NULL : &table);
		for(outcount = static_cast<size_t>(0u); outcount < insize; ++outcount)
			output[outcount] = table.encodeCharacter(input[outcount]);
		locker.release();
		return insize;
	}

	size_t TableCodec16::decodeBlock(const char* input, size_t insize, Char16* output, size_t outsize,
			size_t& outcount) {
		if(outsize < insize)
			insize = outsize;
		ObjectLocker<CodeTable<Char16> > locker(table.supportsConcurrentLookup() ? NULL : &table);
		for(outcount = static_cast<size_t>(0u); outcount < insize; ++outcount)
			output[outcount] = table.decodeCharacter(input[outcount]);
		locker.release();
		return insize;
	}

}}
