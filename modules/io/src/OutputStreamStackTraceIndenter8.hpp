#ifndef REDSTRAIN_MOD_IO_OUTPUTSTREAMSTACKTRACEINDENTER8_HPP
#define REDSTRAIN_MOD_IO_OUTPUTSTREAMSTACKTRACEINDENTER8_HPP

#include <redstrain/error/StackTraceIndenterBase8.hpp>

#include "FormattedOutputStream.hpp"
#include "api.hpp"

namespace redengine {
namespace io {

	class REDSTRAIN_IO_API OutputStreamStackTraceIndenter8 : public error::StackTraceIndenterBase8 {

	  private:
		OutputStream<char>& stream;
		FormattedOutputStream<char> formatted;

	  protected:
		virtual void writeString(const std::string&);

	  public:
		OutputStreamStackTraceIndenter8(OutputStream<char>&);
		OutputStreamStackTraceIndenter8(const OutputStreamStackTraceIndenter8&);

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

		virtual void endLine();

	};

}}

#endif /* REDSTRAIN_MOD_IO_OUTPUTSTREAMSTACKTRACEINDENTER8_HPP */
