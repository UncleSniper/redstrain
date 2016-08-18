#ifndef REDSTRAIN_MOD_DAMNATION_TK_DRAWCONTEXT_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_DRAWCONTEXT_HPP

#include <deque>

#include "../ClippingTerminalCanvas.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class Theme;

	class REDSTRAIN_DAMNATION_API DrawContext {

	  private:
		struct REDSTRAIN_DAMNATION_API ThemeSegment {

			Theme* theme;
			util::MemorySize nulls;

			ThemeSegment(Theme&);
			ThemeSegment(const ThemeSegment&);
			~ThemeSegment();

			ThemeSegment& operator=(const ThemeSegment&);

		};

	  private:
		typedef std::deque<ThemeSegment> ThemeStack;

	  private:
		ThemeStack themes;
		ClippingTerminalCanvas canvas;

	  public:
		DrawContext(TerminalCanvas&);
		DrawContext(const DrawContext&);

		inline ClippingTerminalCanvas& getCanvas() {
			return canvas;
		}

		inline const ClippingTerminalCanvas& getCanvas() const {
			return canvas;
		}

		void pushTheme(Theme&);
		void popTheme();
		unsigned getColor(const std::string&) const;
		void reset();
		DTheme& getTheme() const;
		void setTheme(Theme&);

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_DRAWCONTEXT_HPP */
