#include <redstrain/text/Transcode.hpp>
#include <redstrain/util/StringUtils.hpp>
#include <redstrain/error/ProgrammingError.hpp>

#include "ProgressBar.hpp"
#include "DrawContext.hpp"

using std::string;
using redengine::util::Tristate;
using redengine::text::String16;
using redengine::text::Transcode;
using redengine::util::StringUtils;
using redengine::error::ProgrammingError;
using redengine::util::TRI_NO;
using redengine::util::TRI_YES;
using redengine::util::TRI_MAYBE;

namespace redengine {
namespace damnation {
namespace tk {

#define COLOR_PARAMS(typeTail, useTail, placement, fallback) \
	"org.unclesniper.damnation.abstract." #typeTail, \
	string(className ? className : "redengine.damnation.tk.ProgressBar") + "." #useTail, \
	#placement, \
	Color::fallback

	ProgressBar::ProgressBar(Axis direction, const char* className)
			: minimumValue(0), maximumValue(100), currentValue(0),
			drawSize(ProgressBar::DEFAULT_DRAW_SIZE), direction(direction),
			bar(COLOR_PARAMS(fill.fg, fill.fg, fillForeground, SIMPLE_WHITE)),
			captionEmptyForeground(COLOR_PARAMS(text.fg, captionEmpty.fg, captionEmptyForeground, SIMPLE_WHITE)),
			captionFilledForeground(COLOR_PARAMS(reverseFill.fg, captionFilled.fg,
				captionFilledForeground, SIMPLE_BLACK)),
			splitCaption(COLOR_PARAMS(splitText.opt, splitCaption.opt, splitCaption, FALSE)),
			usePartialBlocks(COLOR_PARAMS(usePartialBlocks.opt, usePartialBlocks.opt, usePartialBlocks, FALSE)),
			relyOnColor(COLOR_PARAMS(relyOnColor.opt, relyOnColor.opt, relyOnColor, FALSE)),
			renderCaption(COLOR_PARAMS(renderCriticalParts.opt, renderCaption.opt, renderCaption, FALSE)) {
		addLocalColors();
	}

	ProgressBar::ProgressBar(const ProgressBar& bar) : Widget(bar), LeafWidget(bar), minimumValue(bar.minimumValue),
			maximumValue(bar.maximumValue), currentValue(bar.currentValue), caption(bar.caption),
			drawSize(bar.drawSize), direction(bar.direction), bar(bar.bar),
			captionEmptyForeground(bar.captionEmptyForeground), captionFilledForeground(bar.captionFilledForeground),
			splitCaption(bar.splitCaption), usePartialBlocks(bar.usePartialBlocks), relyOnColor(bar.relyOnColor),
			renderCaption(bar.renderCaption) {
		addLocalColors();
	}

	ProgressBar::~ProgressBar() {}

	void ProgressBar::addLocalColors() {
		addLocalColor(bar);
		addLocalColor(captionEmptyForeground);
		addLocalColor(captionFilledForeground);
		addLocalColor(splitCaption);
		addLocalColor(usePartialBlocks);
		addLocalColor(relyOnColor);
		addLocalColor(renderCaption);
	}

	void ProgressBar::setCaption(const String16& caption) {
		this->caption = caption;
	}

	string ProgressBar::getCaptionUTF8() const {
		return Transcode::bmpToUTF8(caption);
	}

	void ProgressBar::setCaption(const string& caption) {
		this->caption = Transcode::utf8ToBMP(caption);
	}

	void ProgressBar::setDrawSize(unsigned drawSize) {
		this->drawSize = drawSize ? drawSize : ProgressBar::DEFAULT_DRAW_SIZE;
	}

	void ProgressBar::setBar(const Color& bar) {
		this->bar = bar;
	}

	void ProgressBar::setCaptionEmptyForeground(const Color& captionEmptyForeground) {
		this->captionEmptyForeground = captionEmptyForeground;
	}

	void ProgressBar::setCaptionFilledForeground(const Color& captionFilledForeground) {
		this->captionFilledForeground = captionFilledForeground;
	}

	void ProgressBar::setSplitCaption(const Color& splitCaption) {
		this->splitCaption = splitCaption;
	}

	void ProgressBar::setUsePartialBlocks(const Color& usePartialBlocks) {
		this->usePartialBlocks = usePartialBlocks;
	}

	void ProgressBar::setRelyOnColor(const Color& relyOnColor) {
		this->relyOnColor = relyOnColor;
	}

	void ProgressBar::setRenderCaption(const Color& renderCaption) {
		this->renderCaption = renderCaption;
	}

	unsigned ProgressBar::getBarColor() const {
		return bar.getLocal();
	}

	void ProgressBar::setBarColor(unsigned color) {
		bar.setLocal(color);
	}

	unsigned ProgressBar::getCaptionEmptyForegroundColor() const {
		return captionEmptyForeground.getLocal();
	}

	void ProgressBar::setCaptionEmptyForegroundColor(unsigned color) {
		captionEmptyForeground.setLocal(color);
	}

	unsigned ProgressBar::getCaptionFilledForegroundColor() const {
		return captionFilledForeground.getLocal();
	}

	void ProgressBar::setCaptionFilledForegroundColor(unsigned color) {
		captionFilledForeground.setLocal(color);
	}

#define makeGetOption(func, color) \
	Tristate ProgressBar::func() const { \
		unsigned value = color.getLocal(); \
		if(value == Color::NO_COLOR) \
			return TRI_MAYBE; \
		return value ? TRI_YES : TRI_NO; \
	}
#define makeSetOption(func, color) \
	void ProgressBar::func(Tristate option) { \
		switch(option) { \
			case TRI_NO: \
				color.setLocal(Color::FALSE); \
				break; \
			case TRI_YES: \
				color.setLocal(Color::TRUE); \
				break; \
			case TRI_MAYBE: \
				color.setLocal(Color::NO_COLOR); \
				break; \
			default: \
				throw ProgrammingError("Unrecognized Tristate in ProgressBar::" #func "(): " \
						+ StringUtils::toString(static_cast<int>(option))); \
		} \
	}

	makeGetOption(isSplitCaptionOption, splitCaption)
	makeSetOption(setSplitCaptionOption, splitCaption)
	makeGetOption(isUsePartialBlocksOption, usePartialBlocks)
	makeSetOption(setUsePartialBlocksOption, usePartialBlocks)
	makeGetOption(isRelyOnColorOption, relyOnColor)
	makeSetOption(setRelyOnColorOption, relyOnColor)
	makeGetOption(isRenderCaptionOption, renderCaption)
	makeSetOption(setRenderCaptionOption, renderCaption)

	bool ProgressBar::takesFocus() {
		return false;
	}

	Size ProgressBar::getMinimumSizeWithinBorder() {
		return Size(1u, 1u);
	}

	Size ProgressBar::getMaximumSizeWithinBorder() {
		return direction == AXS_HORIZONTAL ? Size(Widget::NO_SIZE_LIMIT, 1u) : Size(1u, Widget::NO_SIZE_LIMIT);
	}

	Size ProgressBar::getPreferredSizeWithinBorder() {
		return direction == AXS_HORIZONTAL ? Size(drawSize, 1u) : Size(1u, drawSize);
	}

	unsigned ProgressBar::getHeightForWidthWithinBorder(unsigned width) {
		if(!width)
			return 0u;
		return direction == AXS_HORIZONTAL ? 1u : drawSize;
	}

	unsigned ProgressBar::getWidthForHeightWithinBorder(unsigned height) {
		if(!height)
			return 0u;
		return direction == AXS_HORIZONTAL ? drawSize : 1u;
	}

	void ProgressBar::drawIntoBorder(const Rectangle& rectangle, DrawContext& context) {
		if(!rectangle.width || !rectangle.height)
			return;
		int min, max;
		if(minimumValue > maximumValue) {
			min = maximumValue;
			max = minimumValue;
		}
		else {
			min = minimumValue;
			max = maximumValue;
		}
		int value = currentValue;
		if(value < min)
			value = min;
		else if(value > max)
			value = max;
		unsigned space = static_cast<unsigned>(max - min);
		value -= min;
		bool hor;
		switch(direction) {
			case AXS_HORIZONTAL:
				hor = true;
				break;
			case AXS_VERTICAL:
				hor = false;
				break;
			default:
				throw ProgrammingError("Unrecognized Axis in ProgressBar::drawIntoBorder(): "
						+ StringUtils::toString(static_cast<int>(direction)));
		}
		unsigned color;
		color = splitCaption.get(context);
		const bool sc = color != Color::NO_COLOR && color;
		color = usePartialBlocks.get(context);
		const bool upb = color != Color::NO_COLOR && color;
		color = relyOnColor.get(context);
		const bool roc = color != Color::NO_COLOR && color;
		color = renderCaption.get(context);
		const bool rc = !caption.empty() && hor && color != Color::NO_COLOR && color;
		const bool maySplit = sc && !upb && roc;
		unsigned available = hor ? rectangle.width : rectangle.height;
		unsigned pos, filled, between, empty;
		if(upb) {
			pos = space ? available * 8u * static_cast<unsigned>(value) / space : 0u;
			filled = pos / 8u;
			between = pos % 8u ? 1u : 0u;
			empty = available - filled - between;
		}
		else {
			pos = space ? available * value / space : 0u;
			filled = pos;
			between = 0u;
			empty = available - filled;
		}
		ClippingTerminalCanvas& canvas = context.getCanvas();
		if(hor) {
			canvas.setBackgroundColor(bar.get(context));
			canvas.setForegroundColor(bar.get(context));
			canvas.fill(Rectangle(rectangle.row, rectangle.column, filled, rectangle.height),
					roc ? BS_BLANK : BS_FULL_BLOCK);
			unsigned clen;
			if(rc && (maySplit || filled >= empty)) {
				unsigned cavail = maySplit ? filled : (filled < 2u ? 0u : filled - 2u);
				clen = static_cast<unsigned>(caption.length());
				if(clen > cavail)
					clen = cavail;
				canvas.setForegroundColor(captionFilledForeground.get(context));
				int r = rectangle.row + static_cast<int>(rectangle.height - 1u) / 2;
				canvas.moveTo(r < 0 ? 0u : static_cast<unsigned>(r),
						rectangle.column < 0 ? 0u : static_cast<unsigned>(rectangle.column));
				canvas.write(caption.substr(static_cast<String16::size_type>(0u),
						static_cast<String16::size_type>(clen)));
				canvas.setForegroundColor(bar.get(context));
			}
			else
				clen = 0u;
			canvas.setBackgroundColor(background.get(context));
			if(between) {
				int cf = rectangle.column + static_cast<int>(filled);
				canvas.vline(
					rectangle.row < 0 ? 0u : static_cast<unsigned>(rectangle.row),
					cf < 0 ? 0u : static_cast<unsigned>(cf),
					rectangle.height,
					leftBlockSymbol(pos)
				);
			}
			canvas.setForegroundColor(background.get(context));
			canvas.fill(Rectangle(rectangle.row, rectangle.column + static_cast<int>(filled + between),
					empty, rectangle.height), BS_BLANK);
			if(rc && (maySplit || filled < empty)) {
				unsigned crest = static_cast<unsigned>(caption.length()) - clen;
				if(crest > empty)
					crest = empty;
				canvas.setForegroundColor(captionEmptyForeground.get(context));
				int r = rectangle.row + static_cast<int>(rectangle.height - 1u) / 2u;
				int c = rectangle.column + static_cast<int>(filled + between);
				canvas.moveTo(r < 0 ? 0u : static_cast<unsigned>(r), c < 0 ? 0u : static_cast<unsigned>(c));
				canvas.write(caption.substr(static_cast<String16::size_type>(clen),
						static_cast<String16::size_type>(crest)));
			}
		}
		else {
			canvas.setBackgroundColor(background.get(context));
			canvas.setForegroundColor(background.get(context));
			canvas.fill(Rectangle(rectangle.row, rectangle.column, rectangle.width, empty), BS_BLANK);
			canvas.setForegroundColor(bar.get(context));
			if(between) {
				int re = rectangle.row + static_cast<int>(empty);
				canvas.hline(
					re < 0 ? 0u : static_cast<unsigned>(re),
					rectangle.column,
					rectangle.width,
					lowerBlockSymbol(pos)
				);
			}
			canvas.setBackgroundColor(bar.get(context));
			canvas.fill(Rectangle(rectangle.row + static_cast<int>(empty + between), rectangle.column,
					rectangle.width, filled), roc ? BS_BLANK : BS_FULL_BLOCK);
		}
	}

}}}
