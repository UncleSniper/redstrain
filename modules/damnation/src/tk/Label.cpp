#include <redstrain/text/Transcode.hpp>
#include <redstrain/util/CountingAppender.hpp>
#include <redstrain/util/PlusAssignAppender.hpp>
#include <redstrain/util/stloperators.hpp>

#include "Label.hpp"
#include "DrawContext.hpp"
#include "DefaultLineBreaker.hpp"

using std::string;
using redengine::text::String16;
using redengine::text::Transcode;
using redengine::util::MemorySize;
using redengine::util::CountingAppender;
using redengine::util::PlusAssignAppender;
using redengine::util::operator+=;

namespace redengine {
namespace damnation {
namespace tk {

	Label::Label(const string& caption, const char* className)
			: LeafWidget(className ? className : "redengine.damnation.tk.Label"),
			caption(Transcode::utf8ToBMP(caption)), lineBreaker(&DefaultLineBreaker::instance),
			breakingCached(false), brokenWidth(static_cast<MemorySize>(0u)), foreground(
				"org.unclesniper.damnation.abstract.text.fg",
				string(className ? className : "redengine.damnation.tk.Label") + ".foreground.fg",
				"labelForeground",
				Color::SIMPLE_WHITE
			), setCursorPosition(false) {
		addLocalColor(foreground);
		lineBreaker->ref();
	}

	Label::Label(const String16& caption, const char* className)
			: LeafWidget(className ? className : "redengine.damnation.tk.Label"),
			caption(caption), lineBreaker(&DefaultLineBreaker::instance), breakingCached(false),
			brokenWidth(static_cast<MemorySize>(0u)), foreground(
				"org.unclesniper.damnation.abstract.text.fg",
				string(className ? className : "redengine.damnation.tk.Label") + ".foreground.fg",
				"labelForeground",
				Color::SIMPLE_WHITE
			), setCursorPosition(false) {
		addLocalColor(foreground);
		lineBreaker->ref();
	}

	Label::Label(const Label& label) : Widget(label), LeafWidget(label), caption(label.caption),
			lineBreaker(label.lineBreaker), brokenLines(label.brokenLines), breakingCached(label.breakingCached),
			brokenWidth(label.brokenWidth), foreground(label.foreground),
			setCursorPosition(label.setCursorPosition) {
		addLocalColor(foreground);
		if(lineBreaker)
			lineBreaker->ref();
	}

	Label::~Label() {
		if(lineBreaker)
			lineBreaker->unref();
	}

	void Label::setCaption(const String16& caption) {
		this->caption = caption;
		brokenLines.clear();
		breakingCached = false;
	}

	string Label::getCaptionUTF8() const {
		return Transcode::bmpToUTF8(caption);
	}

	void Label::setCaption(const string& caption) {
		this->caption = Transcode::utf8ToBMP(caption);
		brokenLines.clear();
		breakingCached = false;
	}

	void Label::setLineBreaker(LineBreaker* lineBreaker) {
		if(lineBreaker == this->lineBreaker)
			return;
		if(lineBreaker)
			lineBreaker->ref();
		if(this->lineBreaker)
			this->lineBreaker->unref();
		this->lineBreaker = lineBreaker;
		brokenLines.clear();
		breakingCached = false;
	}

	void Label::setForeground(const Color& color) {
		foreground = color;
	}

	unsigned Label::getForegroundColor() const {
		return foreground.getLocal();
	}

	void Label::setForegroundColor(unsigned color) {
		foreground.setLocal(color);
	}

	bool Label::takesFocus() {
		return false;
	}

	void Label::rebreak(MemorySize width) {
		Lines old;
		if(lineBreaker) {
			PlusAssignAppender<Lines, String16> sink(old);
			lineBreaker->breakIntoLines(caption, width, sink);
		}
		else
			old.push_back(caption);
		brokenLines.clear();
		Lines::const_iterator begin(old.begin()), end(old.end());
		for(; begin != end; ++begin) {
			if(begin->length() > static_cast<String16::size_type>(width))
				brokenLines.push_back(begin->substr(static_cast<String16::size_type>(0u),
						static_cast<String16::size_type>(width)));
			else
				brokenLines.push_back(*begin);
		}
		brokenWidth = width;
		breakingCached = true;
	}

	Size Label::getMinimumSizeWithinBorder() {
		return Size(static_cast<unsigned>(caption.length()), 1u);
	}

	Size Label::getMaximumSizeWithinBorder() {
		return Size(static_cast<unsigned>(caption.length()), 1u);
	}

	Size Label::getPreferredSizeWithinBorder() {
		return Size(static_cast<unsigned>(caption.length()), 1u);
	}

	unsigned Label::getHeightForWidthWithinBorder(unsigned width) {
		if(breakingCached && static_cast<MemorySize>(width) == brokenWidth)
			return static_cast<unsigned>(brokenLines.size());
		if(!lineBreaker)
			return 1u;
		CountingAppender<String16, unsigned> sink;
		lineBreaker->breakIntoLines(caption, static_cast<MemorySize>(width), sink);
		return sink.getCount();
	}

	unsigned Label::getWidthForHeightWithinBorder(unsigned height) {
		return height ? static_cast<unsigned>(caption.length()) : 0u;
	}

	void Label::drawIntoBorder(const Rectangle& rectangle, DrawContext& context) {
		if(!rectangle.width || !rectangle.height)
			return;
		if(!breakingCached || brokenWidth != static_cast<MemorySize>(rectangle.width))
			rebreak(static_cast<MemorySize>(rectangle.width));
		ClippingTerminalCanvas& canvas = context.getCanvas();
		canvas.setForegroundColor(foreground.get(context));
		canvas.setBackgroundColor((isFocused() ? background : inactiveBackground).get(context));
		int r = rectangle.row, bottom = r + static_cast<int>(rectangle.height);
		const unsigned c = rectangle.column < 0 ? 0u : rectangle.column;
		Lines::const_iterator begin(brokenLines.begin()), end(brokenLines.end());
		for(; begin != end; ++begin, ++r) {
			if(r >= bottom)
				break;
			if(r < 0)
				continue;
			canvas.moveTo(static_cast<unsigned>(r), c);
			canvas.write(*begin);
		}
		if(setCursorPosition)
			context.setCursorPosition(Point(rectangle.row < 0 ? 0 : rectangle.row, static_cast<int>(c)), false);
	}

}}}
