#ifndef REDSTRAIN_MOD_TEXT_UTF8ENCODER16_HPP
#define REDSTRAIN_MOD_TEXT_UTF8ENCODER16_HPP

#include "TextCodec.hpp"
#include "api.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API UTF8Encoder16 : public TextCodec<Char16, char> {

	  private:
		unsigned pending;
		Char16 partial;

	  public:
		UTF8Encoder16();
		UTF8Encoder16(const UTF8Encoder16&);

		virtual util::MemorySize transcodeBlock(const Char16*, util::MemorySize,
				char*, util::MemorySize, util::MemorySize&);
		virtual void endCodeUnit();

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_UTF8ENCODER16_HPP */
