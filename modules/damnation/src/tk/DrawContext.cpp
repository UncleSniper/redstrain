#include "Theme.hpp"
#include "DrawContext.hpp"
#include "ThemeStackUnderflowError.hpp"

using std::string;
using redengine::util::MemorySize;

namespace redengine {
namespace damnation {
namespace tk {

	// ======== PopTheme ========

	DrawContext::PopTheme::PopTheme(DrawContext* context) : context(context) {}

	DrawContext::PopTheme::PopTheme(const PopTheme& pop) : context(pop.context) {}

	DrawContext::PopTheme::~PopTheme() {
		if(context)
			context->popTheme();
	}

	DrawContext* DrawContext::PopTheme::set(DrawContext* newContext) {
		DrawContext* oldContext = context;
		context = newContext;
		return oldContext;
	}

	// ======== ThemeSegment ========

	DrawContext::ThemeSegment::ThemeSegment(Theme& theme) : theme(&theme), nulls(static_cast<MemorySize>(0u)) {
		theme.ref();
	}

	DrawContext::ThemeSegment::ThemeSegment(const ThemeSegment& segment)
			: theme(segment.theme), nulls(segment.nulls) {
		theme->ref();
	}

	DrawContext::ThemeSegment::~ThemeSegment() {
		theme->unref();
	}

	DrawContext::ThemeSegment& DrawContext::ThemeSegment::operator=(const ThemeSegment& segment) {
		segment.theme->ref();
		theme->unref();
		theme = segment.theme;
		nulls = segment.nulls;
		return *this;
	}

	// ======== DrawContext ========

	DrawContext::DrawContext(TerminalCanvas& terminal, Theme& rootTheme)
			: canvas(terminal), cursorPosition(0, 0), cursorVisible(true) {
		themes.push_back(ThemeSegment(rootTheme));
	}

	DrawContext::DrawContext(const DrawContext& context) : ColorSource(context), themes(context.themes),
			canvas(const_cast<TerminalCanvas&>(context.canvas.getTerminal())),
			cursorPosition(context.cursorPosition), cursorVisible(context.cursorVisible) {}

	DrawContext::~DrawContext() {}

	void DrawContext::setCursorPosition(const Point& position, bool visible) {
		cursorPosition = position;
		cursorVisible = visible;
	}

	void DrawContext::pushTheme(Theme* theme) {
		if(theme)
			themes.push_back(ThemeSegment(*theme));
		else
			++themes.back().nulls;
	}

	void DrawContext::popTheme() {
		ThemeSegment& segment = themes.back();
		if(segment.nulls)
			--segment.nulls;
		else if(themes.size() > static_cast<ThemeStack::size_type>(1u))
			themes.pop_back();
		else
			throw ThemeStackUnderflowError();
	}

	unsigned DrawContext::getColor(const string& key) {
		ThemeStack::const_reverse_iterator begin(themes.rbegin()), end(themes.rend());
		for(; begin != end; ++begin) {
			unsigned color = begin->theme->getColor(key);
			if(color != Color::NO_COLOR)
				return color;
		}
		return Color::NO_COLOR;
	}

	void DrawContext::reset() {
		ThemeStack::size_type tcount;
		for(tcount = themes.size(); tcount > static_cast<ThemeStack::size_type>(1u); --tcount)
			themes.pop_back();
		themes.back().nulls = static_cast<MemorySize>(0u);
		canvas.clearClip();
	}

	Theme& DrawContext::getTheme() const {
		return *themes.front().theme;
	}

	void DrawContext::setTheme(Theme& theme) {
		theme.ref();
		Theme*& rootTheme = themes.front().theme;
		rootTheme->unref();
		rootTheme = &theme;
	}

}}}
