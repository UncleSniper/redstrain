#ifndef REDSTRAIN_MOD_TEXT_UTF16DECODER8_HPP
#define REDSTRAIN_MOD_TEXT_UTF16DECODER8_HPP

#include <redstrain/util/ByteOrder.hpp>

#include "UTF16Decoder16.hpp"
#include "tweaks.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API UTF16Decoder8 : public TextCodec<char, Char32> {

	  private:
		static const int IFL_BOM_READ  = 00100;
		static const int IFL_MASK      = 00100;

	  private:
		util::ByteOrder byteOrder, defaultByteOrder;
		int flags;
		UTF16Decoder16 codec1632;
		union {
			char buffer8[REDSTRAIN_TEXT_UTF16DECODER8_BUFFER_SIZE * 2u];
			Char16 buffer16[REDSTRAIN_TEXT_UTF16DECODER8_BUFFER_SIZE];
		} buffer;
		util::MemorySize bufferFill, bufferOffset;

	  public:
		UTF16Decoder8();
		UTF16Decoder8(const UTF16Decoder8&);
		virtual ~UTF16Decoder8();

		inline util::ByteOrder getByteOrder() const {
			return byteOrder;
		}

		void setByteOrder(util::ByteOrder);

		inline util::ByteOrder getDefaultByteOrder() const {
			return byteOrder;
		}

		void setDefaultByteOrder(util::ByteOrder);

		virtual util::MemorySize transcodeBlock(const char*, util::MemorySize,
				Char32*, util::MemorySize, util::MemorySize&);
		virtual void endCodeUnit();

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_UTF16DECODER8_HPP */
