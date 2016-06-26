#ifndef REDSTRAIN_MOD_ERROR_STDOSTREAMSTACKTRACESINK_HPP
#define REDSTRAIN_MOD_ERROR_STDOSTREAMSTACKTRACESINK_HPP

#include <iostream>
#include <redstrain/redmond/unmangle/StdOStreamSymbolSink.hpp>
#include <redstrain/redmond/unmangle/StdOStreamSymbolIndenter.hpp>

#include "StackTraceSinkBase8.hpp"
#include "IndentingStackTraceSink.hpp"

namespace redengine {
namespace error {

	class REDSTRAIN_ERROR_API StdOStreamStackTraceSink
			: public StackTraceSinkBase8, public IndentingStackTraceSink {

	  private:
		std::ostream& stream;

	  protected:
		virtual void writeHeader(const std::string&);
		virtual void writeFrame(const std::string&);
		virtual void writeSymbol(const redmond::unmangle::CPPSymbol&);
		virtual void beginFrameModule(util::MemorySize);
		virtual void writeFrameModule(const std::string&);
		virtual void configureSymbolSink(redmond::unmangle::StdOStreamSymbolSink&,
				redmond::unmangle::StdOStreamSymbolIndenter&);

	  public:
		StdOStreamStackTraceSink(std::ostream&, Indenter&);
		StdOStreamStackTraceSink(const StdOStreamStackTraceSink&);

		inline std::ostream& getStream() {
			return stream;
		}

		inline const std::ostream& getStream() const {
			return stream;
		}

	};

}}

#endif /* REDSTRAIN_MOD_ERROR_STDOSTREAMSTACKTRACESINK_HPP */
