#ifndef REDSTRAIN_MOD_TEXT_TRANSCODE_HPP
#define REDSTRAIN_MOD_TEXT_TRANSCODE_HPP

#include <redstrain/io/InputStream.hpp>
#include <redstrain/io/OutputStream.hpp>

#include "api.hpp"
#include "types.hpp"

namespace redengine {
namespace text {

	class Encoder16;
	class Decoder16;

	struct REDSTRAIN_TEXT_API Transcode {

		static util::FileSize encode(io::InputStream<Char16>&, io::OutputStream<char>&, Encoder16&);
		static util::FileSize decode(io::InputStream<char>&, io::OutputStream<Char16>&, Decoder16&);

		static void encode(const String16&, std::string&, Encoder16&);
		static void decode(const std::string&, String16&, Decoder16&);

		static String16 utf8ToBMP(const std::string&);
		static std::string bmpToUTF8(const String16&);

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_TRANSCODE_HPP */
