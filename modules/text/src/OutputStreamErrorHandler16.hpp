#ifndef REDSTRAIN_MOD_TEXT_OUTPUTSTREAMERRORHANDLER16_HPP
#define REDSTRAIN_MOD_TEXT_OUTPUTSTREAMERRORHANDLER16_HPP

#include <redstrain/io/FormattedOutputStream.hpp>
#include <redstrain/error/IndentingErrorHandler.hpp>

#include "ErrorHandlerBase16.hpp"

namespace redengine {
namespace text {

	class OutputStreamStackTraceSink16;
	class OutputStreamStackTraceIndenter16;

	class REDSTRAIN_TEXT_API OutputStreamErrorHandler16
			: virtual public ErrorHandlerBase16, public error::IndentingErrorHandler {

	  private:
		io::OutputStream<Char16>& stream;
		io::FormattedOutputStream<Char16> formatted;

	  protected:
		virtual void writeString(const String16&);
		virtual void writeStackTrace(const error::StackTrace*);
		virtual void configureStackTraceSink(OutputStreamStackTraceSink16&, OutputStreamStackTraceIndenter16&);

	  public:
		OutputStreamErrorHandler16(io::OutputStream<Char16>&, Indenter&);
		OutputStreamErrorHandler16(const OutputStreamErrorHandler16&);
		virtual ~OutputStreamErrorHandler16();

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

#endif /* REDSTRAIN_MOD_TEXT_OUTPUTSTREAMERRORHANDLER16_HPP */
