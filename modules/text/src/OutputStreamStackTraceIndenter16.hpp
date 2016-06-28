#ifndef REDSTRAIN_MOD_TEXT_OUTPUTSTREAMSTACKTRACEINDENTER16_HPP
#define REDSTRAIN_MOD_TEXT_OUTPUTSTREAMSTACKTRACEINDENTER16_HPP

#include <redstrain/io/FormattedOutputStream.hpp>

#include "StackTraceIndenterBase16.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API OutputStreamStackTraceIndenter16 : public StackTraceIndenterBase16 {

	  private:
		io::OutputStream<Char16>& stream;
		io::FormattedOutputStream<Char16> formatted;

	  protected:
		virtual void writeString(const String16&);

	  public:
		OutputStreamStackTraceIndenter16(io::OutputStream<Char16>&);
		OutputStreamStackTraceIndenter16(const OutputStreamStackTraceIndenter16&);

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

		virtual void endLine();

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_OUTPUTSTREAMSTACKTRACEINDENTER16_HPP */
