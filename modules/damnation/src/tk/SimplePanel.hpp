#ifndef REDSTRAIN_MOD_DAMNATION_TK_SIMPLEPANEL_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_SIMPLEPANEL_HPP

#include <vector>

#include "AbstractContainer.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class REDSTRAIN_DAMNATION_API SimplePanel : public AbstractContainer {

	  protected:
		typedef std::vector<Widget*> Children;

	  protected:
		Children children;

	  protected:
		virtual void drawIntoBorder(const Rectangle&, DrawContext&);

	  public:
		SimplePanel(const char*);
		SimplePanel(const SimplePanel&);
		virtual ~SimplePanel();

		void add(Widget&);

		virtual util::MemorySize getChildCount();
		virtual Widget& getChild(util::MemorySize);
		virtual void getChildren(util::Appender<Widget*>&);
		virtual util::MemorySize getIndexOfChild(Widget&);
		virtual bool removeChild(Widget&);

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_SIMPLEPANEL_HPP */
