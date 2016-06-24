#ifndef REDSTRAIN_MOD_IO_OUTPUTSTREAMSYMBOLSINK8_HPP
#define REDSTRAIN_MOD_IO_OUTPUTSTREAMSYMBOLSINK8_HPP

#include <redstrain/redmond/unmangle/SymbolSinkBase8.hpp>
#include <redstrain/redmond/unmangle/BoundedSymbolSink.hpp>
#include <redstrain/redmond/unmangle/SymbolWidthAdapter.hpp>
#include <redstrain/redmond/unmangle/IndentingSymbolSink.hpp>
#include <redstrain/platform/ConsoleHighlightingSymbolSink.hpp>

#include "FormattedOutputStream.hpp"
#include "api.hpp"

namespace redengine {
namespace io {

	class REDSTRAIN_IO_API OutputStreamSymbolSink8 : public redmond::unmangle::SymbolSinkBase8,
			public redmond::unmangle::BoundedSymbolSink, public redmond::unmangle::IndentingSymbolSink,
			public platform::ConsoleHighlightingSymbolSink {

	  private:
		OutputStream<char>& stream;
		FormattedOutputStream<char> formatted;
		bool lastWasGreater, lastWasLess;

	  protected:
		virtual void writeString(const std::string&);
		virtual void notifyIndented(unsigned);
		virtual void notifyConsoleChanged();

	  public:
		OutputStreamSymbolSink8(OutputStream<char>&, Indenter&, platform::Console* = NULL, int = 0);
		OutputStreamSymbolSink8(const OutputStreamSymbolSink8&);

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

#endif /* REDSTRAIN_MOD_IO_OUTPUTSTREAMSYMBOLSINK8_HPP */
