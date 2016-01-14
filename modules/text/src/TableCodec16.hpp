#ifndef REDSTRAIN_MOD_TEXT_TABLECODEC16_HPP
#define REDSTRAIN_MOD_TEXT_TABLECODEC16_HPP

#include "Encoder16.hpp"
#include "Decoder16.hpp"
#include "CodeTable.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API TableCodec16 : public Encoder16, public Decoder16 {

	  private:
		CodeTable<Char16>& table;

	  public:
		TableCodec16(CodeTable<Char16>&);
		TableCodec16(const TableCodec16&);
		virtual ~TableCodec16();

		inline CodeTable<Char16>& getCodeTable() {
			return table;
		}

		inline const CodeTable<Char16>& getCodeTable() const {
			return table;
		}

		virtual util::MemorySize encodeBlock(const Char16*, util::MemorySize,
				char*, util::MemorySize, util::MemorySize&);
		virtual util::MemorySize decodeBlock(const char*, util::MemorySize,
				Char16*, util::MemorySize, util::MemorySize&);

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_TABLECODEC16_HPP */
