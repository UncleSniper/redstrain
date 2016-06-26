#ifndef REDSTRAIN_MOD_TEXT_OUTPUTSTREAMSYMBOLINDENTER16_HPP
#define REDSTRAIN_MOD_TEXT_OUTPUTSTREAMSYMBOLINDENTER16_HPP

#include <redstrain/io/FormattedOutputStream.hpp>

#include "SymbolSinkIndenterBase16.hpp"
#include "types.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API OutputStreamSymbolIndenter16 : public SymbolSinkIndenterBase16 {

	  private:
		io::OutputStream<Char16>& stream;
		io::FormattedOutputStream<Char16> formatted;

	  protected:
		virtual void writeString(const String16&);

	  public:
		OutputStreamSymbolIndenter16(io::OutputStream<Char16>&);
		OutputStreamSymbolIndenter16(const OutputStreamSymbolIndenter16&);

		inline io::OutputStream<Char16>& getStream() {
			return stream;
		}

		inline const io::OutputStream<Char16>& getStream() const {
			return stream;
		}

		inline io::FormattedOutputStream<Char16>& getFormattedStream() {
			return formatted;
		}

		inline const io::FormattedOutputStream<Char16>& getFormattedStream() const {
			return formatted;
		}

		virtual void startNewLine();

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_OUTPUTSTREAMSYMBOLINDENTER16_HPP */
