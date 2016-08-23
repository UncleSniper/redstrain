#ifndef REDSTRAIN_MOD_DAMNATION_TK_ABSTRACTCONTAINER_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_ABSTRACTCONTAINER_HPP

#include "Container.hpp"
#include "AbstractWidget.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class REDSTRAIN_DAMNATION_API AbstractContainer : public Container, public AbstractWidget {

	  protected:
		virtual void updateChildrenForNewLayer();
		virtual void clearDescendantColors();

		virtual void processInputLocally(InputListener::InputEvent&);
		virtual void clearInputState();

		virtual void makeChildRectVisibleLocally(util::MemorySize, Widget&, const Rectangle&, Gravity);
		virtual void makeChildVisibleLocally(util::MemorySize, Widget&, Gravity);

	  public:
		AbstractContainer(const char*);
		AbstractContainer(const AbstractContainer&);

		virtual bool takesFocus();

		virtual void makeChildRectVisible(Widget&, const Rectangle&, Gravity);
		virtual void makeChildVisible(Widget&, Gravity);

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_ABSTRACTCONTAINER_HPP */
