#ifndef REDSTRAIN_MOD_IO_OUTPUTSTREAMSTACKTRACESINK8_HPP
#define REDSTRAIN_MOD_IO_OUTPUTSTREAMSTACKTRACESINK8_HPP

#include <redstrain/error/StackTraceSinkBase8.hpp>
#include <redstrain/error/IndentingStackTraceSink.hpp>

#include "FormattedOutputStream.hpp"
#include "api.hpp"

namespace redengine {
namespace io {

	class OutputStreamSymbolSink8;
	class OutputStreamSymbolIndenter8;

	class REDSTRAIN_IO_API OutputStreamStackTraceSink8
			: public error::StackTraceSinkBase8, public error::IndentingStackTraceSink {

	  private:
		OutputStream<char>& stream;
		FormattedOutputStream<char> formatted;

	  protected:
		virtual void writeHeader(const std::string&);
		virtual void writeFrame(const std::string&);
		virtual void writeSymbol(const redmond::unmangle::CPPSymbol&);
		virtual void beginFrameModule(util::MemorySize);
		virtual void writeFrameModule(const std::string&);
		virtual void configureSymbolSink(OutputStreamSymbolSink8&, OutputStreamSymbolIndenter8&);

	  public:
		OutputStreamStackTraceSink8(OutputStream<char>&, Indenter&);
		OutputStreamStackTraceSink8(const OutputStreamStackTraceSink8&);
		virtual ~OutputStreamStackTraceSink8();

		inline OutputStream<char>& getStream() {
			return stream;
		}

		inline const OutputStream<char>& getStream() const {
			return stream;
		}

		inline FormattedOutputStream<char>& getFormattedStream() {
			return formatted;
		}

		inline const FormattedOutputStream<char>& getFormattedStream() const {
			return formatted;
		}

	};

}}

#endif /* REDSTRAIN_MOD_IO_OUTPUTSTREAMSTACKTRACESINK8_HPP */
