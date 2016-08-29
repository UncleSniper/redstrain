#ifndef REDSTRAIN_MOD_DAMNATION_TK_ABSTRACTDEFAULTELEMENTRENDERER_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_ABSTRACTDEFAULTELEMENTRENDERER_HPP

#include <redstrain/util/StringUtils.hpp>
#include <redstrain/error/ProgrammingError.hpp>

#include "Label.hpp"
#include "DrawContext.hpp"
#include "ElementRenderer.hpp"

namespace redengine {
namespace damnation {
namespace tk {

#define REDSTRAIN_DAMNATION_ABSTRACTDEFAULTELEMENTRENDERER_COLOR(typeTail, placement, fallback) \
	"org.unclesniper.damnation.abstract." #typeTail, \
	"", \
	#placement, \
	Color::fallback

	template<
		typename ElementT,
		typename StringifierT = void (*)(Label&, const ElementT&)
	>
	class AbstractDefaultElementRenderer : public virtual ElementRenderer<ElementT> {

	  public:
		typedef StringifierT Stringifier;
		typedef typename ElementRenderer<ElementT>::SelectionState SelectionState;

	  protected:
		StringifierT stringifier;
		Color primaryForeground, primaryBackground;
		Color selectedForeground, selectedBackground;
		Color unselectedForeground, unselectedBackground;
		Color primaryInactiveForeground, primaryInactiveBackground;
		Color selectedInactiveForeground, selectedInactiveBackground;
		Color unselectedInactiveForeground, unselectedInactiveBackground;
		Label rendition;

	  public:
		AbstractDefaultElementRenderer(const StringifierT& stringifier) : stringifier(stringifier),
				primaryForeground(REDSTRAIN_DAMNATION_ABSTRACTDEFAULTELEMENTRENDERER_COLOR(
					primarySelection.active.fg, primaryActiveForeground, SIMPLE_BLACK)),
				primaryBackground(REDSTRAIN_DAMNATION_ABSTRACTDEFAULTELEMENTRENDERER_COLOR(
					primarySelection.active.bg, primaryActiveBackground, SIMPLE_WHITE)),
				selectedForeground(REDSTRAIN_DAMNATION_ABSTRACTDEFAULTELEMENTRENDERER_COLOR(
					selectedText.active.fg, selectedActiveForeground, SIMPLE_BLACK)),
				selectedBackground(REDSTRAIN_DAMNATION_ABSTRACTDEFAULTELEMENTRENDERER_COLOR(
					selectedText.active.bg, selectedActiveBackground, SIMPLE_WHITE)),
				unselectedForeground(REDSTRAIN_DAMNATION_ABSTRACTDEFAULTELEMENTRENDERER_COLOR(
					text.fg, unselectedActiveForeground, SIMPLE_WHITE)),
				unselectedBackground(REDSTRAIN_DAMNATION_ABSTRACTDEFAULTELEMENTRENDERER_COLOR(
					window.active.bg, unselectedActiveBackground, SIMPLE_BLACK)),
				primaryInactiveForeground(REDSTRAIN_DAMNATION_ABSTRACTDEFAULTELEMENTRENDERER_COLOR(
					primarySelection.inactive.fg, primaryInactiveForeground, SIMPLE_BLACK)),
				primaryInactiveBackground(REDSTRAIN_DAMNATION_ABSTRACTDEFAULTELEMENTRENDERER_COLOR(
					primarySelection.inactive.bg, primaryInactiveBackground, SIMPLE_WHITE)),
				selectedInactiveForeground(REDSTRAIN_DAMNATION_ABSTRACTDEFAULTELEMENTRENDERER_COLOR(
					selectedText.inactive.fg, selectedInactiveForeground, SIMPLE_BLACK)),
				selectedInactiveBackground(REDSTRAIN_DAMNATION_ABSTRACTDEFAULTELEMENTRENDERER_COLOR(
					selectedText.inactive.bg, selectedInactiveBackground, SIMPLE_WHITE)),
				unselectedInactiveForeground(REDSTRAIN_DAMNATION_ABSTRACTDEFAULTELEMENTRENDERER_COLOR(
					text.fg, unselectedInactiveForeground, SIMPLE_WHITE)),
				unselectedInactiveBackground(REDSTRAIN_DAMNATION_ABSTRACTDEFAULTELEMENTRENDERER_COLOR(
					window.inactive.bg, unselectedInactiveBackground, SIMPLE_BLACK)),
				rendition(text::String16()) {}

		AbstractDefaultElementRenderer(const AbstractDefaultElementRenderer& renderer)
				: ElementRenderer<ElementT>(renderer), stringifier(renderer.stringifier),
				primaryForeground(renderer.primaryForeground), primaryBackground(renderer.primaryBackground),
				selectedForeground(renderer.selectedForeground), selectedBackground(renderer.selectedBackground),
				unselectedForeground(renderer.unselectedForeground),
				unselectedBackground(renderer.unselectedBackground),
				primaryInactiveForeground(renderer.primaryInactiveForeground),
				primaryInactiveBackground(renderer.primaryInactiveBackground),
				selectedInactiveForeground(renderer.selectedInactiveForeground),
				selectedInactiveBackground(renderer.selectedInactiveBackground),
				unselectedInactiveForeground(renderer.unselectedInactiveForeground),
				unselectedInactiveBackground(renderer.unselectedInactiveBackground),
				rendition(text::String16()) {}

		virtual ~AbstractDefaultElementRenderer() {}

		inline StringifierT& getStringifier() {
			return stringifier;
		}

		inline const StringifierT& getStringifier() const {
			return stringifier;
		}

		inline Color& getPrimaryForeground() {
			return primaryForeground;
		}

		inline const Color& getPrimaryForeground() const {
			return primaryForeground;
		}

		void setPrimaryForeground(const Color& color) {
			primaryForeground = color;
		}

		inline Color& getPrimaryBackground() {
			return primaryBackground;
		}

		inline const Color& getPrimaryBackground() const {
			return primaryBackground;
		}

		void setPrimaryBackground(const Color& color) {
			primaryBackground = color;
		}

		inline Color& getSelectedForeground() {
			return selectedForeground;
		}

		inline const Color& getSelectedForeground() const {
			return selectedForeground;
		}

		void setSelectedForeground(const Color& color) {
			selectedForeground = color;
		}

		inline Color& getSelectedBackground() {
			return selectedBackground;
		}

		inline const Color& getSelectedBackground() const {
			return selectedBackground;
		}

		void setSelectedBackground(const Color& color) {
			selectedBackground = color;
		}

		inline Color& getUnselectedForeground() {
			return unselectedForeground;
		}

		inline const Color& getUnselectedForeground() const {
			return unselectedForeground;
		}

		void setUnselectedForeground(const Color& color) {
			unselectedForeground = color;
		}

		inline Color& getUnselectedBackground() {
			return unselectedBackground;
		}

		inline const Color& getUnselectedBackground() const {
			return unselectedBackground;
		}

		void setUnselectedBackground(const Color& color) {
			unselectedBackground = color;
		}

		inline Color& getPrimaryInactiveForeground() {
			return primaryInactiveForeground;
		}

		inline const Color& getPrimaryInactiveForeground() const {
			return primaryInactiveForeground;
		}

		void setPrimaryInactiveForeground(const Color& color) {
			primaryInactiveForeground = color;
		}

		inline Color& getPrimaryInactiveBackground() {
			return primaryInactiveBackground;
		}

		inline const Color& getPrimaryInactiveBackground() const {
			return primaryInactiveBackground;
		}

		void setPrimaryInactiveBackground(const Color& color) {
			primaryInactiveBackground = color;
		}

		inline Color& getSelectedInactiveForeground() {
			return selectedInactiveForeground;
		}

		inline const Color& getSelectedInactiveForeground() const {
			return selectedInactiveForeground;
		}

		void setSelectedInactiveForeground(const Color& color) {
			selectedInactiveForeground = color;
		}

		inline Color& getSelectedInactiveBackground() {
			return selectedInactiveBackground;
		}

		inline const Color& getSelectedInactiveBackground() const {
			return selectedInactiveBackground;
		}

		void setSelectedInactiveBackground(const Color& color) {
			selectedInactiveBackground = color;
		}

		inline Color& getUnselectedInactiveForeground() {
			return unselectedInactiveForeground;
		}

		inline const Color& getUnselectedInactiveForeground() const {
			return unselectedInactiveForeground;
		}

		void setUnselectedInactiveForeground(const Color& color) {
			unselectedInactiveForeground = color;
		}

		inline Color& getUnselectedInactiveBackground() {
			return unselectedInactiveBackground;
		}

		inline const Color& getUnselectedInactiveBackground() const {
			return unselectedInactiveBackground;
		}

		void setUnselectedInactiveBackground(const Color& color) {
			unselectedInactiveBackground = color;
		}

		unsigned getPrimaryForegroundColor() const {
			return primaryForeground.getLocal();
		}

		void setPrimaryForegroundColor(unsigned color) {
			primaryForeground.setLocal(color);
		}

		unsigned getPrimaryBackgroundColor() const {
			return primaryBackground.getLocal();
		}

		void setPrimaryBackgroundColor(unsigned color) {
			primaryBackground.setLocal(color);
		}

		unsigned getSelectedForegroundColor() const {
			return selectedForeground.getLocal();
		}

		void setSelectedForegroundColor(unsigned color) {
			selectedForeground.setLocal(color);
		}

		unsigned getSelectedBackgroundColor() const {
			return selectedBackground.getLocal();
		}

		void setSelectedBackgroundColor(unsigned color) {
			selectedBackground.setLocal(color);
		}

		unsigned getUnselectedForegroundColor() const {
			return unselectedForeground.getLocal();
		}

		void setUnselectedForegroundColor(unsigned color) {
			unselectedForeground.setLocal(color);
		}

		unsigned getUnselectedBackgroundColor() const {
			return unselectedBackground.getLocal();
		}

		void setUnselectedBackgroundColor(unsigned color) {
			unselectedBackground.setLocal(color);
		}

		unsigned getPrimaryInactiveForegroundColor() const {
			return primaryInactiveForeground.getLocal();
		}

		void setPrimaryInactiveForegroundColor(unsigned color) {
			primaryInactiveForeground.setLocal(color);
		}

		unsigned getPrimaryInactiveBackgroundColor() const {
			return primaryInactiveBackground.getLocal();
		}

		void setPrimaryInactiveBackgroundColor(unsigned color) {
			primaryInactiveBackground.setLocal(color);
		}

		unsigned getSelectedInactiveForegroundColor() const {
			return selectedInactiveForeground.getLocal();
		}

		void setSelectedInactiveForegroundColor(unsigned color) {
			selectedInactiveForeground.setLocal(color);
		}

		unsigned getSelectedInactiveBackgroundColor() const {
			return selectedInactiveBackground.getLocal();
		}

		void setSelectedInactiveBackgroundColor(unsigned color) {
			selectedInactiveBackground.setLocal(color);
		}

		unsigned getUnselectedInactiveForegroundColor() const {
			return unselectedInactiveForeground.getLocal();
		}

		void setUnselectedInactiveForegroundColor(unsigned color) {
			unselectedInactiveForeground.setLocal(color);
		}

		unsigned getUnselectedInactiveBackgroundColor() const {
			return unselectedInactiveBackground.getLocal();
		}

		void setUnselectedInactiveBackgroundColor(unsigned color) {
			unselectedInactiveBackground.setLocal(color);
		}

		virtual Widget& getRenditionFor(bool active, const ElementT& element,
				SelectionState selected, DrawContext* context) {
			stringifier(rendition, element);
			if(context) {
				if(active) {
					switch(selected) {
						case ElementRenderer<ElementT>::UNSELECTED:
							rendition.setForegroundColor(unselectedForeground.get(*context));
							rendition.setInactiveBackgroundColor(unselectedBackground.get(*context));
							break;
						case ElementRenderer<ElementT>::SELECTED:
							rendition.setForegroundColor(selectedForeground.get(*context));
							rendition.setInactiveBackgroundColor(selectedBackground.get(*context));
							break;
						case ElementRenderer<ElementT>::PRIMARY:
							rendition.setForegroundColor(primaryForeground.get(*context));
							rendition.setInactiveBackgroundColor(primaryBackground.get(*context));
							break;
						default:
							throw error::ProgrammingError("Unrecognized SelectionState in "
									"AbstractDefaultElementRenderer::getRenditionFor(): "
									+ util::StringUtils::toString(static_cast<int>(selected)));
					}
				}
				else {
					switch(selected) {
						case ElementRenderer<ElementT>::UNSELECTED:
							rendition.setForegroundColor(unselectedInactiveForeground.get(*context));
							rendition.setInactiveBackgroundColor(unselectedInactiveBackground.get(*context));
							break;
						case ElementRenderer<ElementT>::SELECTED:
							rendition.setForegroundColor(selectedInactiveForeground.get(*context));
							rendition.setInactiveBackgroundColor(selectedInactiveBackground.get(*context));
							break;
						case ElementRenderer<ElementT>::PRIMARY:
							rendition.setForegroundColor(primaryInactiveForeground.get(*context));
							rendition.setInactiveBackgroundColor(primaryInactiveBackground.get(*context));
							break;
						default:
							throw error::ProgrammingError("Unrecognized SelectionState in "
									"AbstractDefaultElementRenderer::getRenditionFor(): "
									+ util::StringUtils::toString(static_cast<int>(selected)));
					}
				}
			}
			rendition.setSetCursorPosition(active && selected == ElementRenderer<ElementT>::PRIMARY);
			return rendition;
		}

		virtual void clearCachedRenditionColors() {
			primaryForeground.clearCache();
			primaryBackground.clearCache();
			selectedForeground.clearCache();
			selectedBackground.clearCache();
			unselectedForeground.clearCache();
			unselectedBackground.clearCache();
			primaryInactiveForeground.clearCache();
			primaryInactiveBackground.clearCache();
			selectedInactiveForeground.clearCache();
			selectedInactiveBackground.clearCache();
			unselectedInactiveForeground.clearCache();
			unselectedInactiveBackground.clearCache();
		}

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_ABSTRACTDEFAULTELEMENTRENDERER_HPP */
