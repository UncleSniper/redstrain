#ifndef REDSTRAIN_MOD_DAMNATION_TK_LAYER_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_LAYER_HPP

#include <map>
#include <vector>

#include "../api.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class Screen;
	class Widget;
	class StagePanel;

	class REDSTRAIN_DAMNATION_API Layer {

	  private:
		typedef std::vector<Widget*> Tabs;
		typedef std::map<Widget*, unsigned> InverseTabs;

	  public:
		static const unsigned INVALID_TAB_INDEX = ~0u;

	  private:
		Screen& screen;
		unsigned depth;
		StagePanel *const stage;
		Tabs tabs;
		InverseTabs inverseTabs;
		unsigned currentTabIndex;

	  private:
		Layer(const Layer&);

	  public:
		Layer(Screen&, unsigned);
		~Layer();

		inline Screen& getScreen() {
			return screen;
		}

		inline const Screen& getScreen() const {
			return screen;
		}

		inline unsigned getDepth() const {
			return depth;
		}

		inline StagePanel& getStage() {
			return *stage;
		}

		inline const StagePanel& getStage() const {
			return *stage;
		}

		inline unsigned getCurrentTabIndex() const {
			return currentTabIndex;
		}

		unsigned addTabIndex(Widget&);
		unsigned removeTabIndex(Widget&);
		void setTabIndex(Widget&, unsigned);
		Widget* getCurrentFocus() const;
		void setCurrentTabIndex(unsigned);
		void shiftFocus();
		void unshiftFocus();
		void notifyActiveChanged(bool);

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_LAYER_HPP */
