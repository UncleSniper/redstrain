#ifndef REDSTRAIN_MOD_IO_OUTPUTSTREAMERRORHANDLER8_HPP
#define REDSTRAIN_MOD_IO_OUTPUTSTREAMERRORHANDLER8_HPP

#include <redstrain/error/ErrorHandlerBase8.hpp>
#include <redstrain/error/IndentingErrorHandler.hpp>

#include "FormattedOutputStream.hpp"
#include "api.hpp"

namespace redengine {
namespace io {

	class OutputStreamStackTraceSink8;
	class OutputStreamStackTraceIndenter8;

	class REDSTRAIN_IO_API OutputStreamErrorHandler8
			: public error::ErrorHandlerBase8, public error::IndentingErrorHandler {

	  private:
		OutputStream<char>& stream;
		FormattedOutputStream<char> formatted;

	  protected:
		virtual void writeString(const std::string&);
		virtual void writeStackTrace(const error::StackTrace*);
		virtual void configureStackTraceSink(OutputStreamStackTraceSink8&, OutputStreamStackTraceIndenter8&);

	  public:
		OutputStreamErrorHandler8(OutputStream<char>&, Indenter&);
		OutputStreamErrorHandler8(const OutputStreamErrorHandler8&);
		virtual ~OutputStreamErrorHandler8();

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

#endif /* REDSTRAIN_MOD_IO_OUTPUTSTREAMERRORHANDLER8_HPP */
