#ifndef REDSTRAIN_MOD_TEXT_TABLECODEC_HPP
#define REDSTRAIN_MOD_TEXT_TABLECODEC_HPP

#include <redstrain/platform/ObjectLocker.hpp>

#include "TextCodec.hpp"
#include "CodeTable.hpp"

namespace redengine {
namespace text {

	template<typename CharT>
	class TableCodec : public TextCodec<CharT, char>, public TextCodec<char, CharT> {

	  private:
		CodeTable<CharT>& table;

	  public:
		TableCodec(CodeTable<CharT>& table) : table(table) {
			platform::ObjectLocker<CodeTable<CharT> > locker(&table);
			table.ref();
			locker.release();
		}

		TableCodec(const TableCodec& codec) : TextCodec<CharT, char>(codec), TextCodec<char, CharT>(codec),
				table(codec.table) {
			platform::ObjectLocker<CodeTable<CharT> > locker(&table);
			table.ref();
			locker.release();
		}

		virtual ~TableCodec() {
			platform::ObjectLocker<CodeTable<CharT> > locker(&table);
			table.unref();
		}

		inline CodeTable<CharT>& getCodeTable() {
			return table;
		}

		inline const CodeTable<CharT>& getCodeTable() const {
			return table;
		}

		virtual util::MemorySize transcodeBlock(const CharT* input, util::MemorySize insize,
				char* output, util::MemorySize outsize, util::MemorySize& outcount) {
			if(outsize < insize)
				insize = outsize;
			platform::ObjectLocker<CodeTable<CharT> > locker(table.supportsConcurrentLookup() ? NULL : &table);
			for(outcount = static_cast<util::MemorySize>(0u); outcount < insize; ++outcount)
				output[outcount] = table.encodeCharacter(input[outcount]);
			locker.release();
			return insize;
		}

		virtual util::MemorySize transcodeBlock(const char* input, util::MemorySize insize,
				CharT* output, util::MemorySize outsize, util::MemorySize& outcount) {
			if(outsize < insize)
				insize = outsize;
			platform::ObjectLocker<CodeTable<CharT> > locker(table.supportsConcurrentLookup() ? NULL : &table);
			for(outcount = static_cast<util::MemorySize>(0u); outcount < insize; ++outcount)
				output[outcount] = table.decodeCharacter(input[outcount]);
			locker.release();
			return insize;
		}

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_TABLECODEC_HPP */
