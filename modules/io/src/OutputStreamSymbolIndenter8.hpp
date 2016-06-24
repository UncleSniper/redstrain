#ifndef REDSTRAIN_MOD_IO_OUTPUTSTREAMSYMBOLINDENTER8_HPP
#define REDSTRAIN_MOD_IO_OUTPUTSTREAMSYMBOLINDENTER8_HPP

#include <redstrain/redmond/unmangle/SymbolSinkIndenterBase8.hpp>

#include "FormattedOutputStream.hpp"
#include "api.hpp"

namespace redengine {
namespace io {

	class REDSTRAIN_IO_API OutputStreamSymbolIndenter8 : public redmond::unmangle::SymbolSinkIndenterBase8 {

	  private:
		OutputStream<char>& stream;
		FormattedOutputStream<char> formatted;

	  protected:
		virtual void writeString(const std::string&);

	  public:
		OutputStreamSymbolIndenter8(OutputStream<char>&);
		OutputStreamSymbolIndenter8(const OutputStreamSymbolIndenter8&);

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

		virtual void startNewLine();

	};

}}

#endif /* REDSTRAIN_MOD_IO_OUTPUTSTREAMSYMBOLINDENTER8_HPP */
