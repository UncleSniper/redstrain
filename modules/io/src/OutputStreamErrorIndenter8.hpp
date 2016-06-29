#ifndef REDSTRAIN_MOD_IO_OUTPUTSTREAMERRORINDENTER8_HPP
#define REDSTRAIN_MOD_IO_OUTPUTSTREAMERRORINDENTER8_HPP

#include <redstrain/error/ErrorIndenterBase8.hpp>

#include "FormattedOutputStream.hpp"
#include "api.hpp"

namespace redengine {
namespace io {

	class REDSTRAIN_IO_API OutputStreamErrorIndenter8 : public error::ErrorIndenterBase8 {

	  private:
		OutputStream<char>& stream;
		FormattedOutputStream<char> formatted;

	  protected:
		virtual void writeString(const std::string&);

	  public:
		OutputStreamErrorIndenter8(OutputStream<char>&);
		OutputStreamErrorIndenter8(const OutputStreamErrorIndenter8&);
		virtual ~OutputStreamErrorIndenter8();

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

#endif /* REDSTRAIN_MOD_IO_OUTPUTSTREAMERRORINDENTER8_HPP */
