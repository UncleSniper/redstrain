#ifndef REDSTRAIN_MOD_DAMNATION_TK_WIDGETLISTENER_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_WIDGETLISTENER_HPP

#include "DamnEvent.hpp"
#include "DamnListener.hpp"
#include "../Size.hpp"
#include "../Point.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class Layer;
	class Theme;
	class Widget;

	class REDSTRAIN_DAMNATION_API WidgetListener : public DamnListener {

	  public:
		class REDSTRAIN_DAMNATION_API WidgetEvent : public DamnEvent {

		  private:
			Widget* widget;

		  public:
			WidgetEvent(Widget&);
			WidgetEvent(const WidgetEvent&);

			inline Widget& getWidget() const {
				return *widget;
			}

			inline void setWidget(Widget& widget) {
				this->widget = &widget;
			}

		};

		class REDSTRAIN_DAMNATION_API ReparentEvent : public WidgetEvent {

		  private:
			Widget *oldParent, *newParent;

		  public:
			ReparentEvent(Widget&, Widget*, Widget*);
			ReparentEvent(const ReparentEvent&);

			inline Widget* getOldParent() const {
				return oldParent;
			}

			inline void setOldParent(Widget* oldParent) {
				this->oldParent = oldParent;
			}

			inline Widget* getNewParent() const {
				return newParent;
			}

			inline void setNewParent(Widget* newParent) {
				this->newParent = newParent;
			}

		};

		class REDSTRAIN_DAMNATION_API RelayerEvent : public WidgetEvent {

		  private:
			Layer *oldLayer, *newLayer;

		  public:
			RelayerEvent(Widget&, Layer*, Layer*);
			RelayerEvent(const RelayerEvent&);

			inline Layer* getOldLayer() const {
				return oldLayer;
			}

			inline void setOldLayer(Layer* oldLayer) {
				this->oldLayer = oldLayer;
			}

			inline Layer* getNewLayer() const {
				return newLayer;
			}

			inline void setNewLayer(Layer* newLayer) {
				this->newLayer = newLayer;
			}

		};

		class REDSTRAIN_DAMNATION_API RepositionEvent : public WidgetEvent {

		  private:
			Point oldPosition, newPosition;

		  public:
			RepositionEvent(Widget&, const Point&, const Point&);
			RepositionEvent(const RepositionEvent&);

			inline Point& getOldPosition() {
				return oldPosition;
			}

			inline const Point& getOldPosition() const {
				return oldPosition;
			}

			void setOldPosition(const Point&);

			inline Point& getNewPosition() {
				return newPosition;
			}

			inline const Point& getNewPosition() const {
				return newPosition;
			}

			void setNewPosition(const Point&);

		};

		class REDSTRAIN_DAMNATION_API ResizeEvent : public WidgetEvent {

		  private:
			Size oldSize, newSize;

		  public:
			ResizeEvent(Widget&, const Size&, const Size&);
			ResizeEvent(const ResizeEvent&);

			inline Size& getOldSize() {
				return oldSize;
			}

			inline const Size& getOldSize() const {
				return oldSize;
			}

			void setOldSize(const Size&);

			inline Size& getNewSize() {
				return newSize;
			}

			inline const Size& getNewSize() const {
				return newSize;
			}

			void setNewSize(const Size&);

		};

		class REDSTRAIN_DAMNATION_API RethemeEvent : public WidgetEvent {

		  private:
			Theme *oldTheme, *newTheme;

		  public:
			RethemeEvent(Widget&, Theme*, Theme*);
			RethemeEvent(const RethemeEvent&);

			inline Theme* getOldTheme() const {
				return oldTheme;
			}

			inline void setOldTheme(Theme* oldTheme) {
				this->oldTheme = oldTheme;
			}

			inline Theme* getNewTheme() const {
				return newTheme;
			}

			inline void setNewTheme(Theme* newTheme) {
				this->newTheme = newTheme;
			}

		};

	  public:
		WidgetListener();
		WidgetListener(const WidgetListener&);

		virtual void parentChanged(ReparentEvent&) = 0;
		virtual void layerChanged(RelayerEvent&) = 0;
		virtual void repositioned(RepositionEvent&) = 0;
		virtual void resized(ResizeEvent&) = 0;
		virtual void themeChanged(RethemeEvent&) = 0;

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_WIDGETLISTENER_HPP */
