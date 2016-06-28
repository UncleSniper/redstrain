#ifndef REDSTRAIN_MOD_IO_CONSOLESTACKTRACESINK8_HPP
#define REDSTRAIN_MOD_IO_CONSOLESTACKTRACESINK8_HPP

#include "FileOutputStream.hpp"
#include "OutputStreamStackTraceIndenter8.hpp"
#include "ConsoleOutputStreamStackTraceSink8.hpp"

namespace redengine {
namespace io {

	class REDSTRAIN_IO_API ConsoleStackTraceSink8 : public ConsoleOutputStreamStackTraceSink8 {

	  private:
		FileOutputStream stream;
		OutputStreamStackTraceIndenter8 indenter;
		platform::Console console;

	  public:
		ConsoleStackTraceSink8(platform::Console::StandardHandle);
		ConsoleStackTraceSink8(const ConsoleStackTraceSink8&);
		virtual ~ConsoleStackTraceSink8();

		inline FileOutputStream& getFileStream() {
			return stream;
		}

		inline const FileOutputStream& getFileStream() const {
			return stream;
		}

		inline OutputStreamStackTraceIndenter8& getIndenter() {
			return indenter;
		}

		inline const OutputStreamStackTraceIndenter8& getIndenter() const {
			return indenter;
		}

		inline platform::Console& getConsole() {
			return console;
		}

		inline const platform::Console& getConsole() const {
			return console;
		}

	};

}}

#endif /* REDSTRAIN_MOD_IO_CONSOLESTACKTRACESINK8_HPP */
