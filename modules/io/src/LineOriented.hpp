#ifndef REDSTRAIN_MOD_IO_LINEORIENTED_HPP
#define REDSTRAIN_MOD_IO_LINEORIENTED_HPP

#include <redstrain/platform/platform.hpp>

#include "api.hpp"

namespace redengine {
namespace io {

	class REDSTRAIN_IO_API LineOriented {

	  public:
		enum LinebreakPolicy {
			AUTO_LINEBREAKS,
			UNIX_LINEBREAKS,
			MACOS_LINEBREAKS,
			WINDOWS_LINEBREAKS
		};

	  public:
#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS
		static const LinebreakPolicy PLATFORM_DEFAULT_LINEBREAK_POLICY = WINDOWS_LINEBREAKS;
#else /* non-Windows */
		static const LinebreakPolicy PLATFORM_DEFAULT_LINEBREAK_POLICY = UNIX_LINEBREAKS;
#endif /* OS-specific */

	  protected:
		LinebreakPolicy linebreaks;

	  public:
		LineOriented(LinebreakPolicy = AUTO_LINEBREAKS);
		LineOriented(const LineOriented&);

		inline LinebreakPolicy getLinebreakPolicy() const {
			return linebreaks;
		}

		inline void setLinebreakPolicy(LinebreakPolicy newLinebreaks) {
			linebreaks = newLinebreaks;
		}

	};

}}

#endif /* REDSTRAIN_MOD_IO_LINEORIENTED_HPP */
