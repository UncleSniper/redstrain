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

		virtual char getInverseBreakChar(CharT breakChar) const {
			platform::ObjectLocker<CodeTable<CharT> > locker(table.supportsConcurrentLookup() ? NULL : &table);
			char ibc = table.encodeCharacter(breakChar);
			locker.release();
			return ibc;
		}

		virtual CharT getInverseBreakChar(char breakChar) const {
			platform::ObjectLocker<CodeTable<CharT> > locker(table.supportsConcurrentLookup() ? NULL : &table);
			CharT ibc = table.decodeCharacter(breakChar);
			locker.release();
			return ibc;
		}

		virtual util::MemorySize transcodeBlock(const CharT* input, util::MemorySize insize,
				char* output, util::MemorySize outsize, util::MemorySize& outcount) {
			if(outsize < insize)
				insize = outsize;
			char breakChar = static_cast<const TextCodec<CharT, char>*>(this)->getBreakChar();
			platform::ObjectLocker<CodeTable<CharT> > locker(table.supportsConcurrentLookup() ? NULL : &table);
			for(outcount = static_cast<util::MemorySize>(0u); outcount < insize; ++outcount) {
				char c = table.encodeCharacter(input[outcount]);
				output[outcount] = c;
				if(c == breakChar)
					break;
			}
			locker.release();
			return insize;
		}

		virtual util::MemorySize transcodeBlock(const char* input, util::MemorySize insize,
				CharT* output, util::MemorySize outsize, util::MemorySize& outcount) {
			if(outsize < insize)
				insize = outsize;
			CharT breakChar = static_cast<const TextCodec<char, CharT>*>(this)->getBreakChar();
			platform::ObjectLocker<CodeTable<CharT> > locker(table.supportsConcurrentLookup() ? NULL : &table);
			for(outcount = static_cast<util::MemorySize>(0u); outcount < insize; ++outcount) {
				CharT c = table.decodeCharacter(input[outcount]);
				output[outcount] = c;
				if(c == breakChar)
					break;
			}
			locker.release();
			return insize;
		}

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_TABLECODEC_HPP */
