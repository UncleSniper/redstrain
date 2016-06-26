#ifndef REDSTRAIN_MOD_TEXT_OUTPUTSTREAMSYMBOLSINK16_HPP
#define REDSTRAIN_MOD_TEXT_OUTPUTSTREAMSYMBOLSINK16_HPP

#include <redstrain/io/FormattedOutputStream.hpp>
#include <redstrain/redmond/unmangle/BoundedSymbolSink.hpp>
#include <redstrain/redmond/unmangle/SymbolWidthAdapter.hpp>
#include <redstrain/redmond/unmangle/IndentingSymbolSink.hpp>
#include <redstrain/platform/ConsoleHighlightingSymbolSink.hpp>

#include "SymbolSinkBase16.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API OutputStreamSymbolSink16 : public SymbolSinkBase16,
			public redmond::unmangle::BoundedSymbolSink, public redmond::unmangle::IndentingSymbolSink,
			public platform::ConsoleHighlightingSymbolSink {

	  private:
		io::OutputStream<Char16>& stream;
		io::FormattedOutputStream<Char16> formatted;
		bool lastWasGreater, lastWasLess;

	  protected:
		virtual void writeString(const String16&);
		virtual void notifyIndented(unsigned);
		virtual void notifyConsoleChanged();

	  public:
		OutputStreamSymbolSink16(io::OutputStream<Char16>&, Indenter&, platform::Console* = NULL, int = 0);
		OutputStreamSymbolSink16(const OutputStreamSymbolSink16&);

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

		virtual void startNewLine(int);

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_OUTPUTSTREAMSYMBOLSINK16_HPP */
