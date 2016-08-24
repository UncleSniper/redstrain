#include "DrawContext.hpp"
#include "SimpleBorder.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	const Insets SimpleBorder::INSETS(1u, 1u, 1u, 1u);

	SimpleBorder::SimpleBorder(bool doubleLines) : doubleLines(doubleLines),
			foreground(
				"org.unclesniper.damnation.abstract.border.active.fg",
				"",
				"borderForeground",
				SimpleBorder::FALLBACK_FOREGROUND
			),
			background(
				"org.unclesniper.damnation.abstract.border.active.bg",
				"",
				"borderBackground",
				SimpleBorder::FALLBACK_BACKGROUND
			),
			inactiveForeground(
				"org.unclesniper.damnation.abstract.border.inactive.fg",
				"",
				"borderInactiveForeground",
				SimpleBorder::FALLBACK_INACTIVE_FOREGROUND
			),
			inactiveBackground(
				"org.unclesniper.damnation.abstract.border.inactive.bg",
				"",
				"borderInactiveBackground",
				SimpleBorder::FALLBACK_INACTIVE_BACKGROUND
			) {}

	SimpleBorder::SimpleBorder(const SimpleBorder& border) : Border(border), doubleLines(border.doubleLines),
			foreground(border.foreground), background(border.background),
			inactiveForeground(border.inactiveForeground), inactiveBackground(border.inactiveBackground) {}

	SimpleBorder::~SimpleBorder() {}

	void SimpleBorder::setForeground(const Color& foreground) {
		this->foreground = foreground;
	}

	void SimpleBorder::setBackground(const Color& background) {
		this->background = background;
	}

	void SimpleBorder::setInactiveForeground(const Color& inactiveForeground) {
		this->inactiveForeground = inactiveForeground;
	}

	void SimpleBorder::setInactiveBackground(const Color& inactiveBackground) {
		this->inactiveBackground = inactiveBackground;
	}

	unsigned SimpleBorder::getForegroundColor() const {
		return foreground.getLocal();
	}

	void SimpleBorder::setForegroundColor(unsigned color) {
		foreground.setLocal(color);
	}

	unsigned SimpleBorder::getBackgroundColor() const {
		return background.getLocal();
	}

	void SimpleBorder::setBackgroundColor(unsigned color) {
		background.setLocal(color);
	}

	unsigned SimpleBorder::getInactiveForegroundColor() const {
		return inactiveForeground.getLocal();
	}

	void SimpleBorder::setInactiveForegroundColor(unsigned color) {
		inactiveForeground.setLocal(color);
	}

	unsigned SimpleBorder::getInactiveBackgroundColor() const {
		return inactiveBackground.getLocal();
	}

	void SimpleBorder::setInactiveBackgroundColor(unsigned color) {
		inactiveBackground.setLocal(color);
	}

	Insets SimpleBorder::getBorderSize() {
		return SimpleBorder::INSETS;
	}

	void SimpleBorder::drawBorder(const Rectangle& rectangle, bool focused, DrawContext& context) {
		ClippingTerminalCanvas& canvas = context.getCanvas();
		if(focused) {
			canvas.setForegroundColor(foreground.get(context));
			canvas.setBackgroundColor(background.get(context));
		}
		else {
			canvas.setForegroundColor(inactiveForeground.get(context));
			canvas.setBackgroundColor(inactiveBackground.get(context));
		}
		canvas.box(rectangle, doubleLines);
	}

}}}
