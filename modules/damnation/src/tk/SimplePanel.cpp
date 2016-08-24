#include "SimplePanel.hpp"
#include "ChildIndexOutOfBoundsError.hpp"

using std::vector;
using redengine::util::Appender;
using redengine::util::MemorySize;

namespace redengine {
namespace damnation {
namespace tk {

	SimplePanel::SimplePanel(const char* className)
			: AbstractContainer(className ? className : "redengine.damnation.tk.SimplePanel") {}

	SimplePanel::SimplePanel(const SimplePanel& panel) : Widget(panel), AbstractContainer(panel) {}

	SimplePanel::~SimplePanel() {
		Children::const_iterator begin(children.begin()), end(children.end());
		for(; begin != end; ++begin)
			delete *begin;
	}

	struct PopBackSimplePanelChild {

		vector<Widget*>* children;

		PopBackSimplePanelChild(vector<Widget*>* children) : children(children) {}

		PopBackSimplePanelChild(const PopBackSimplePanelChild& pop) : children(pop.children) {}

		~PopBackSimplePanelChild() {
			if(children)
				children->pop_back();
		}

	};

	void SimplePanel::add(Widget& child) {
		if(getIndexOfChild(child) != Container::INVALID_CHILD_INDEX)
			return;
		children.push_back(&child);
		PopBackSimplePanelChild popChild(&children);
		child.setParent(this);
		popChild.children = NULL;
	}

	void SimplePanel::drawIntoBorder(const Rectangle& destination, DrawContext& context) {
		Children::const_iterator begin(children.begin()), end(children.end());
		for(; begin != end; ++begin) {
			Widget& c = **begin;
			const Point position(c.getPosition());
			c.draw(Point(destination.row + position.row, destination.column + position.column), context);
		}
	}

	MemorySize SimplePanel::getChildCount() {
		return static_cast<MemorySize>(children.size());
	}

	Widget& SimplePanel::getChild(MemorySize index) {
		if(index >= static_cast<MemorySize>(children.size()))
			throw ChildIndexOutOfBoundsError(index);
		return *children[static_cast<Children::size_type>(index)];
	}

	void SimplePanel::getChildren(Appender<Widget*>& sink) {
		Children::const_iterator begin(children.begin()), end(children.end());
		for(; begin != end; ++begin)
			sink.append(*begin);
		sink.doneAppending();
	}

	MemorySize SimplePanel::getIndexOfChild(Widget& child) {
		MemorySize index = static_cast<MemorySize>(0u);
		Children::const_iterator begin(children.begin()), end(children.end());
		for(; begin != end; ++begin, ++index) {
			if(*begin == &child)
				return index;
		}
		return Container::INVALID_CHILD_INDEX;
	}

	bool SimplePanel::removeChild(Widget& child) {
		Children::iterator begin(children.begin()), end(children.end());
		for(; begin != end; ++begin) {
			if(*begin == &child) {
				children.erase(begin);
				return true;
			}
		}
		return false;
	}

}}}
