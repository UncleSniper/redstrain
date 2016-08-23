#ifndef REDSTRAIN_MOD_DAMNATION_TK_CONSTRAINTPANEL_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_CONSTRAINTPANEL_HPP

#include <vector>
#include <redstrain/util/Delete.hpp>

#include "AbstractContainer.hpp"
#include "ChildIndexOutOfBoundsError.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	template<typename ConstraintT>
	class ConstraintPanel : public AbstractContainer {

	  public:
		typedef ConstraintT Constraint;

	  protected:
		class Child {

		  private:
			Widget *const widget;
			const ConstraintT constraint;

		  public:
			Child(Widget& widget, const ConstraintT& constraint) : widget(&widget), constraint(constraint) {}
			Child(const Child& child) : widget(child.widget), constraint(child.constraint) {}

			inline Widget* getWidget() const {
				return widget;
			}

			inline const ConstraintT& getConstraint() const {
				return constraint;
			}

		};

	  protected:
		typedef std::vector<Child*> Children;

	  protected:
		Children children;

	  protected:
		virtual void drawIntoBorder(const Rectangle& destination, DrawContext& context) {
			typename Children::const_iterator begin(children.begin()), end(children.end());
			for(; begin != end; ++begin) {
				Child& child = **begin;
				const Point position(child.getWidget()->getPosition());
				child.getWidget()->draw(Point(destination.row + position.row,
						destination.column + position.column), context);
			}
		}

	  public:
		ConstraintPanel(const char* className)
				: AbstractContainer(className ? className : "redengine.damnation.tk.ConstraintPanel") {}

		ConstraintPanel(const ConstraintPanel& panel) : Widget(panel), AbstractContainer(panel) {}

		virtual ~ConstraintPanel() {
			typename Children::const_iterator begin(children.begin()), end(children.end());
			for(; begin != end; ++begin) {
				Child& child = **begin;
				delete child.getWidget();
				delete &child;
			}
		}

		void add(Widget& child, const ConstraintT& constraint) {
			if(getIndexOfChild(child) != Container::INVALID_CHILD_INDEX)
				return;
			util::Delete<Child> c(new Child(child, constraint));
			children.push_back(*c);
			struct PopBackChild {
				Children* children;
				PopBackChild(Children* children) : children(children) {}
				PopBackChild(const PopBackChild& pop) : children(pop.children) {}
				~PopBackChild() {
					if(children)
						children->pop_back();
				}
			};
			PopBackChild popChild(&children);
			child.setParent(this);
			popChild.children = NULL;
			c.set();
		}

		const ConstraintT* getConstraint(const Widget& child) {
			typename Children::const_iterator begin(children.begin()), end(children.end());
			for(; begin != end; ++begin) {
				if((*begin)->getWidget() == &child)
					return &(*begin)->getConstraint();
			}
			return NULL;
		}

		const ConstraintT& getConstraint(util::MemorySize index) {
			if(index >= static_cast<util::MemorySize>(children.size()))
				throw ChildIndexOutOfBoundsError(index);
			return children[static_cast<typename Children::size_type>(index)]->getConstraint();
		}

		virtual util::MemorySize getChildCount() {
			return static_cast<util::MemorySize>(children.size());
		}

		virtual Widget& getChild(util::MemorySize index) {
			if(index >= static_cast<util::MemorySize>(children.size()))
				throw ChildIndexOutOfBoundsError(index);
			return *children[static_cast<typename Children::size_type>(index)]->getWidget();
		}

		virtual void getChildren(util::Appender<Widget*>& sink) {
			typename Children::const_iterator begin(children.begin()), end(children.end());
			for(; begin != end; ++begin)
				sink.append((*begin)->getWidget());
			sink.doneAppending();
		}

		virtual util::MemorySize getIndexOfChild(Widget& child) {
			util::MemorySize index = static_cast<util::MemorySize>(0u);
			typename Children::const_iterator begin(children.begin()), end(children.end());
			for(; begin != end; ++begin, ++index) {
				if((*begin)->getWidget() == &child)
					return index;
			}
			return Container::INVALID_CHILD_INDEX;
		}

		virtual bool removeChild(Widget& child) {
			typename Children::iterator begin(children.begin()), end(children.end());
			for(; begin != end; ++begin) {
				if((*begin)->getWidget() == &child) {
					delete *begin;
					children.erase(begin);
					return true;
				}
			}
			return false;
		}

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_CONSTRAINTPANEL_HPP */
