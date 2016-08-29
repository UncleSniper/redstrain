#include <redstrain/util/StringUtils.hpp>
#include <redstrain/error/ProgrammingError.hpp>
#include <redstrain/error/UnsupportedOperationError.hpp>

#include "Headed.hpp"
#include "ScrollPanel.hpp"
#include "ChildIndexOutOfBoundsError.hpp"

using std::string;
using redengine::util::Appender;
using redengine::util::MemorySize;
using redengine::util::StringUtils;
using redengine::error::ProgrammingError;
using redengine::error::UnsupportedOperationError;

namespace redengine {
namespace damnation {
namespace tk {

#define COLOR_PARAMS(typeTail, useTail, placement, fallback) \
	"org.unclesniper.damnation.abstract." #typeTail, \
	string(className ? className : "redengine.damnation.tk.ScrollPanel") + "." #useTail, \
	#placement, \
	Color::fallback

#define COLOR_INIT \
	scrollBarLineForeground(COLOR_PARAMS(sideBar.fg, scrollBarLine.fg, scrollBarLineForeground, SIMPLE_WHITE)), \
	scrollBarLineBackground(COLOR_PARAMS(sideBar.bg, scrollBarLine.bg, scrollBarLineBackground, SIMPLE_BLACK)), \
	scrollBarBlockForeground(COLOR_PARAMS(sideBar.fg, scrollBarBlock.fg, scrollBarBlockForeground, SIMPLE_WHITE)), \
	scrollBarBlockBackground(COLOR_PARAMS(sideBar.bg, scrollBarBlock.bg, scrollBarBlockBackground, SIMPLE_BLACK))

	ScrollPanel::ScrollPanel(Widget& view, const char* className) : AbstractContainer(className), view(&view),
			headed(NULL), verticalScrollBarPolicy(SP_WHEN_NEEDED), horizontalScrollBarPolicy(SP_WHEN_NEEDED),
			preferredSize(0u, 0u), COLOR_INIT {
		addLocalColors();
		view.setParent(this);
		mangleNewView();
	}

	ScrollPanel::ScrollPanel(Widget& view, const Size& preferredSize, const char* className)
			: AbstractContainer(className), view(&view), headed(NULL), verticalScrollBarPolicy(SP_WHEN_NEEDED),
			horizontalScrollBarPolicy(SP_WHEN_NEEDED), preferredSize(preferredSize), COLOR_INIT {
		addLocalColors();
		view.setParent(this);
		mangleNewView();
	}

	ScrollPanel::ScrollPanel(Widget& view, ScrollBarPolicy verticalScrollBarPolicy,
			ScrollBarPolicy horizontalScrollBarPolicy, const char* className)
			: AbstractContainer(className), view(&view), headed(NULL),
			verticalScrollBarPolicy(verticalScrollBarPolicy), horizontalScrollBarPolicy(horizontalScrollBarPolicy),
			preferredSize(0u, 0u), COLOR_INIT {
		addLocalColors();
		view.setParent(this);
		mangleNewView();
	}

	ScrollPanel::ScrollPanel(Widget& view, const Size& preferredSize, ScrollBarPolicy verticalScrollBarPolicy,
			ScrollBarPolicy horizontalScrollBarPolicy, const char* className)
			: AbstractContainer(className), view(&view), headed(NULL),
			verticalScrollBarPolicy(verticalScrollBarPolicy), horizontalScrollBarPolicy(horizontalScrollBarPolicy),
			preferredSize(preferredSize), COLOR_INIT {
		addLocalColors();
		view.setParent(this);
		mangleNewView();
	}

	ScrollPanel::ScrollPanel(const ScrollPanel& panel) : Widget(panel), AbstractContainer(panel), view(panel.view),
			headed(panel.headed), verticalScrollBarPolicy(panel.verticalScrollBarPolicy),
			horizontalScrollBarPolicy(panel.horizontalScrollBarPolicy), preferredSize(panel.preferredSize),
			scrollBarLineForeground(panel.scrollBarLineForeground),
			scrollBarLineBackground(panel.scrollBarLineBackground),
			scrollBarBlockForeground(panel.scrollBarBlockForeground),
			scrollBarBlockBackground(panel.scrollBarBlockBackground) {
		throw UnsupportedOperationError("redengine::damnation::tk::ScrollPanel::ScrollPanel(const ScrollPanel&)");
	}

	ScrollPanel::~ScrollPanel() {
		delete view;
	}

	void ScrollPanel::addLocalColors() {
		addLocalColor(scrollBarLineForeground);
		addLocalColor(scrollBarLineBackground);
		addLocalColor(scrollBarBlockForeground);
		addLocalColor(scrollBarBlockBackground);
	}

	void ScrollPanel::mangleNewView() {
		headed = dynamic_cast<Headed*>(view);
		if(!headed)
			return;
		if(headed->isInlineHeader() || headed->isHeaderExternallyDrawn())
			headed = NULL;
		else
			headed->setHeaderExternallyDrawn(true);
	}

	void ScrollPanel::setView(Widget& view) {
		if(&view == this->view)
			return;
		this->view->setParent(NULL);
		if(headed) {
			headed->setHeaderExternallyDrawn(false);
			headed = NULL;
		}
		this->view = &view;
		view.setParent(this);
		mangleNewView();
	}

	void ScrollPanel::setPreferredSize(const Size& preferredSize) {
		this->preferredSize = preferredSize;
	}

	void ScrollPanel::setScrollBarLineForeground(const Color& color) {
		scrollBarLineForeground = color;
	}

	void ScrollPanel::setScrollBarLineBackground(const Color& color) {
		scrollBarLineBackground = color;
	}

	void ScrollPanel::setScrollBarBlockForeground(const Color& color) {
		scrollBarBlockForeground = color;
	}

	void ScrollPanel::setScrollBarBlockBackground(const Color& color) {
		scrollBarBlockBackground = color;
	}

	unsigned ScrollPanel::getScrollBarLineForegroundColor() const {
		return scrollBarLineForeground.getLocal();
	}

	void ScrollPanel::setScrollBarLineForegroundColor(unsigned color) {
		scrollBarLineForeground.setLocal(color);
	}

	unsigned ScrollPanel::getScrollBarLineBackgroundColor() const {
		return scrollBarLineBackground.getLocal();
	}

	void ScrollPanel::setScrollBarLineBackgroundColor(unsigned color) {
		scrollBarLineBackground.setLocal(color);
	}

	unsigned ScrollPanel::getScrollBarBlockForegroundColor() const {
		return scrollBarBlockForeground.getLocal();
	}

	void ScrollPanel::setScrollBarBlockForegroundColor(unsigned color) {
		scrollBarBlockForeground.setLocal(color);
	}

	unsigned ScrollPanel::getScrollBarBlockBackgroundColor() const {
		return scrollBarBlockBackground.getLocal();
	}

	void ScrollPanel::setScrollBarBlockBackgroundColor(unsigned color) {
		scrollBarBlockBackground.setLocal(color);
	}

	MemorySize ScrollPanel::getChildCount() {
		return static_cast<MemorySize>(1u);
	}

	Widget& ScrollPanel::getChild(MemorySize index) {
		if(index)
			throw ChildIndexOutOfBoundsError(index);
		return *view;
	}

	void ScrollPanel::getChildren(Appender<Widget*>& sink) {
		sink.append(view);
		sink.doneAppending();
	}

	MemorySize ScrollPanel::getIndexOfChild(Widget& child) {
		return &child == view ? static_cast<MemorySize>(0u) : Container::INVALID_CHILD_INDEX;
	}

	bool ScrollPanel::removeChild(Widget&) {
		return false;
	}

	void ScrollPanel::pack() {
		view->setSize(view->getPreferredSize());
		view->pack();
		mangleNewView();
		//TODO: adjust position in case view size changed (heed headed!)
	}

	Size ScrollPanel::getMinimumSizeWithinBorder() {
		return Size(2u, 2u);
	}

	Size ScrollPanel::getMaximumSizeWithinBorder() {
		return Size(Widget::NO_SIZE_LIMIT, Widget::NO_SIZE_LIMIT);
	}

	Size ScrollPanel::getPreferredSizeWithinBorder() {
		if(preferredSize.width && preferredSize.height)
			return preferredSize;
		mangleNewView();
		//TODO: heed headed
		const Size cps(view->getPreferredSize());
		return Size(
			preferredSize.width ? preferredSize.width
					: cps.width + (horizontalScrollBarPolicy == SP_ALWAYS ? 1u : 0u),
			preferredSize.height ? preferredSize.height
					: cps.height + (verticalScrollBarPolicy == SP_ALWAYS ? 1u : 0u)
		);
	}

	unsigned ScrollPanel::getHeightForWidthWithinBorder(unsigned width) {
		if(!width)
			return 0u;
		if(preferredSize.height)
			return preferredSize.height;
		mangleNewView();
		//TODO: heed headed
		const Size size(view->getPreferredSize());
		unsigned sbheight;
		switch(verticalScrollBarPolicy) {
			case SP_ALWAYS:
				sbheight = 1u;
				break;
			case SP_WHEN_NEEDED:
				sbheight = size.width > width ? 1u : 0u;
				break;
			case SP_NEVER:
				sbheight = 0u;
				break;
			default:
				throw ProgrammingError("Unrecognized ScrollBarPolicy in "
						"ScrollPanel::getHeightForWidthWithinBorder(): "
						+ StringUtils::toString(static_cast<int>(verticalScrollBarPolicy)));
		}
		return size.height + sbheight;
	}

	unsigned ScrollPanel::getWidthForHeightWithinBorder(unsigned height) {
		if(!height)
			return 0u;
		if(preferredSize.width)
			return preferredSize.width;
		mangleNewView();
		//TODO: heed headed
		const Size size(view->getPreferredSize());
		unsigned sbwidth;
		switch(horizontalScrollBarPolicy) {
			case SP_ALWAYS:
				sbwidth = 1u;
				break;
			case SP_WHEN_NEEDED:
				sbwidth = size.height > height ? 1u : 0u;
				break;
			case SP_NEVER:
				sbwidth = 0u;
				break;
			default:
				throw ProgrammingError("Unrecognized ScrollBarPolicy in "
						"ScrollPanel::getWidthForHeightWithinBorder(): "
						+ StringUtils::toString(static_cast<int>(horizontalScrollBarPolicy)));
		}
		return size.width + sbwidth;
	}

	void ScrollPanel::drawIntoBorder(const Rectangle&, DrawContext&) {
		mangleNewView();
		//TODO
	}

	Widget* ScrollPanel::getFocusTarget() const {
		return view;
	}

	void ScrollPanel::makeChildRectVisibleLocally(MemorySize, Widget&, const Rectangle&, Gravity) {
		//TODO
	}

}}}
