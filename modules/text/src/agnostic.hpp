#ifndef REDSTRAIN_MOD_TEXT_AGNOSTIC_HPP
#define REDSTRAIN_MOD_TEXT_AGNOSTIC_HPP

#include <redstrain/io/OutputStream.hpp>

#include "api.hpp"
#include "types.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API StringSink16 {

	  private:
		String16 value;

	  public:
		StringSink16();
		StringSink16(const String16&);
		StringSink16(const StringSink16&);

		inline String16& getValue() {
			return value;
		}

		inline const String16& getValue() const {
			return value;
		}

	};

	class REDSTRAIN_TEXT_API StreamSink16 {

	  private:
		io::OutputStream<Char16>& stream;

	  public:
		StreamSink16(io::OutputStream<Char16>&);
		StreamSink16(const StreamSink16&);

		inline io::OutputStream<Char16>& getStream() {
			return stream;
		}

		inline const io::OutputStream<Char16>& getStream() const {
			return stream;
		}

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_AGNOSTIC_HPP */
