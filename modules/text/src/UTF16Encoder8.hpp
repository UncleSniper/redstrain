#ifndef REDSTRAIN_MOD_TEXT_UTF16ENCODER8_HPP
#define REDSTRAIN_MOD_TEXT_UTF16ENCODER8_HPP

#include <redstrain/util/ByteOrder.hpp>

#include "UTF16Encoder16.hpp"
#include "tweaks.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API UTF16Encoder8 : public TextCodec<Char32, char> {

	  public:
		static const int FL_SUPPRESS_BOM = 001;
		static const int FL_MASK         = 001;

	  private:
		static const int IFL_BOM_EMITTED = 00100;
		static const int IFL_MASK        = 00100;

	  private:
		util::ByteOrder byteOrder;
		int flags;
		UTF16Encoder16 codec3216;
		union {
			char buffer8[REDSTRAIN_TEXT_UTF16ENCODER8_BUFFER_SIZE * 2u];
			Char16 buffer16[REDSTRAIN_TEXT_UTF16ENCODER8_BUFFER_SIZE];
		} buffer;
		util::MemorySize bufferFill, bufferOffset;

	  public:
		UTF16Encoder8();
		UTF16Encoder8(const UTF16Encoder8&);
		virtual ~UTF16Encoder8();

		inline util::ByteOrder getByteOrder() const {
			return byteOrder;
		}

		void setByteOrder(util::ByteOrder);

		inline int getFlags() const {
			return flags & UTF16Encoder8::FL_MASK;
		}

		inline void setFlags(int flags) {
			this->flags = (this->flags & UTF16Encoder8::IFL_MASK) | (flags & UTF16Encoder8::FL_MASK);
		}

		virtual Char32 getInverseBreakChar(char) const;
		virtual util::MemorySize transcodeBlock(const Char32*, util::MemorySize,
				char*, util::MemorySize, util::MemorySize&);
		virtual void endCodeUnit();

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_UTF16ENCODER8_HPP */
