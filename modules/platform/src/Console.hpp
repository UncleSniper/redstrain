#ifndef REDSTRAIN_MOD_PLATFORM_CONSOLE_HPP
#define REDSTRAIN_MOD_PLATFORM_CONSOLE_HPP

#include "File.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API Console {

	  public:
		enum StandardHandle {
			STANDARD_INPUT,
			STANDARD_OUTPUT,
			STANDARD_ERROR
		};

		enum Color {
			BLACK,
			RED,
			GREEN,
			YELLOW,
			BLUE,
			MAGENTA,
			CYAN,
			WHITE
		};

		enum Intensity {
			NORMAL,
			BRIGHT
		};

	  private:
		File file;

	  public:
		Console(const File&);
		Console(StandardHandle);
		Console(const Console&);

		inline File& getFile() {
			return file;
		}

		inline const File& getFile() const {
			return file;
		}

		void clear();
		void home();
		void setForeground(Color);
		void setBackground(Color);
		void setIntensity(Intensity);
		void resetAttributes();
		void setCursorPosition(unsigned, unsigned);

		static File::Handle getStandardHandle(StandardHandle);
		static bool isConsole(File::Handle);

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_CONSOLE_HPP */
