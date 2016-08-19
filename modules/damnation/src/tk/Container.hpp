#ifndef REDSTRAIN_MOD_DAMNATION_TK_CONTAINER_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_CONTAINER_HPP

#include <redstrain/util/IntegerBounds.hpp>
#include <redstrain/util/types.hpp>

#include "Widget.hpp"
#include "Gravity.hpp"
#include "../Rectangle.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class REDSTRAIN_DAMNATION_API Container : public Widget {

	  public:
		static const util::MemorySize INVALID_CHILD_INDEX = util::IntegerBounds<util::MemorySize>::MAX;

	  public:
		Container();
		Container(const Container&);

		virtual util::MemorySize getChildCount() = 0;
		virtual Widget& getChild(util::MemorySize) = 0;
		virtual void getChildren(util::Appender<Widget*>&) = 0;
		virtual util::MemorySize getIndexOfChild(Widget&) = 0;
		virtual bool removeChild(Widget&) = 0;
		virtual void makeChildRectVisible(Widget&, const Rectangle&, Gravity) = 0;

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_CONTAINER_HPP */
