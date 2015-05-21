#ifndef REDSTRAIN_MOD_TEXT_UTF8ENCODER16_HPP
#define REDSTRAIN_MOD_TEXT_UTF8ENCODER16_HPP

#include "Encoder16.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API UTF8Encoder16 : public Encoder16 {

	  private:
		unsigned pending;
		Char16 partial;

	  public:
		UTF8Encoder16();
		UTF8Encoder16(const UTF8Encoder16&);

		virtual size_t encodeBlock(const Char16*, size_t, char*, size_t, size_t&);

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_UTF8ENCODER16_HPP */
