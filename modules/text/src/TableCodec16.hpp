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

		virtual size_t encodeBlock(const Char16*, size_t, char*, size_t, size_t&);
		virtual size_t decodeBlock(const char*, size_t, Char16*, size_t, size_t&);

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_TABLECODEC16_HPP */
