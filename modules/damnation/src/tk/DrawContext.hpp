#ifndef REDSTRAIN_MOD_DAMNATION_TK_DRAWCONTEXT_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_DRAWCONTEXT_HPP

#include <deque>

#include "Color.hpp"
#include "../ClippingTerminalCanvas.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class Theme;

	class REDSTRAIN_DAMNATION_API DrawContext : public Color::ColorSource {

	  public:
		class REDSTRAIN_DAMNATION_API PopTheme {

		  private:
			DrawContext* context;

		  public:
			PopTheme(DrawContext* = NULL);
			PopTheme(const PopTheme&);
			~PopTheme();

			inline DrawContext* get() const {
				return context;
			}

			DrawContext* set(DrawContext* = NULL);

		};

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
		Point cursorPosition;
		bool cursorVisible;

	  public:
		DrawContext(TerminalCanvas&, Theme&);
		DrawContext(const DrawContext&);
		virtual ~DrawContext();

		inline ClippingTerminalCanvas& getCanvas() {
			return canvas;
		}

		inline const ClippingTerminalCanvas& getCanvas() const {
			return canvas;
		}

		inline const Point& getCursorPosition() const {
			return cursorPosition;
		}

		inline bool isCursorVisible() const {
			return cursorVisible;
		}

		void setCursorPosition(const Point&, bool);

		void pushTheme(Theme*);
		void popTheme();
		void reset();
		Theme& getTheme() const;
		void setTheme(Theme&);

		virtual unsigned getColor(const std::string&);

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_DRAWCONTEXT_HPP */
