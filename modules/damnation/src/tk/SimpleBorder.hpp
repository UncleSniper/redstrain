#include "Color.hpp"
#include "Border.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class REDSTRAIN_DAMNATION_API SimpleBorder : public Border {

	  public:
		static const unsigned FALLBACK_FOREGROUND = Color::SIMPLE_WHITE;
		static const unsigned FALLBACK_BACKGROUND = Color::SIMPLE_BLACK;
		static const unsigned FALLBACK_INACTIVE_FOREGROUND = Color::SIMPLE_WHITE;
		static const unsigned FALLBACK_INACTIVE_BACKGROUND = Color::SIMPLE_BLACK;

	  private:
		static const Insets INSETS;

	  private:
		bool doubleLines;
		Color foreground, background, inactiveForeground, inactiveBackground;

	  public:
		SimpleBorder(bool = false);
		SimpleBorder(const SimpleBorder&);
		virtual ~SimpleBorder();

		inline bool isDoubleLines() const {
			return doubleLines;
		}

		inline void setDoubleLines(bool doubleLines) {
			this->doubleLines = doubleLines;
		}

		inline Color& getForeground() {
			return foreground;
		}

		inline const Color& getForeground() const {
			return foreground;
		}

		void setForeground(const Color&);

		inline Color& getBackground() {
			return background;
		}

		inline const Color& getBackground() const {
			return background;
		}

		void setBackground(const Color&);

		inline Color& getInactiveForeground() {
			return inactiveForeground;
		}

		inline const Color& getInactiveForeground() const {
			return inactiveForeground;
		}

		void setInactiveForeground(const Color&);

		inline Color& getInactiveBackground() {
			return inactiveBackground;
		}

		inline const Color& getInactiveBackground() const {
			return inactiveBackground;
		}

		void setInactiveBackground(const Color&);

		unsigned getForegroundColor() const;
		void setForegroundColor(unsigned);
		unsigned getBackgroundColor() const;
		void setBackgroundColor(unsigned);
		unsigned getInactiveForegroundColor() const;
		void setInactiveForegroundColor(unsigned);
		unsigned getInactiveBackgroundColor() const;
		void setInactiveBackgroundColor(unsigned);

		virtual Insets getBorderSize();
		virtual void drawBorder(const Rectangle&, bool, DrawContext&);

	};

}}}
