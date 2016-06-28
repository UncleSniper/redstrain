#ifndef REDSTRAIN_MOD_TEXT_OUTPUTSTREAMSTACKTRACESINK16_HPP
#define REDSTRAIN_MOD_TEXT_OUTPUTSTREAMSTACKTRACESINK16_HPP

#include <redstrain/io/FormattedOutputStream.hpp>
#include <redstrain/error/IndentingStackTraceSink.hpp>

#include "StackTraceSinkBase16.hpp"

namespace redengine {
namespace text {

	class OutputStreamSymbolSink16;
	class OutputStreamSymbolIndenter16;

	class REDSTRAIN_TEXT_API OutputStreamStackTraceSink16
			: public virtual StackTraceSinkBase16, public error::IndentingStackTraceSink {

	  private:
		io::OutputStream<Char16>& stream;
		io::FormattedOutputStream<Char16> formatted;

	  protected:
		virtual void writeHeader(const String16&);
		virtual void writeFrame(const String16&);
		virtual void writeSymbol(const redmond::unmangle::CPPSymbol&);
		virtual void beginFrameModule(util::MemorySize);
		virtual void writeFrameModule(const String16&);
		virtual void configureSymbolSink(OutputStreamSymbolSink16&, OutputStreamSymbolIndenter16&);

	  public:
		OutputStreamStackTraceSink16(io::OutputStream<Char16>&, Indenter&);
		OutputStreamStackTraceSink16(const OutputStreamStackTraceSink16&);
		virtual ~OutputStreamStackTraceSink16();

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

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_OUTPUTSTREAMSTACKTRACESINK16_HPP */
