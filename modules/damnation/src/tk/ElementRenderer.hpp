#ifndef REDSTRAIN_MOD_DAMNATION_TK_ELEMENTRENDERER_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_ELEMENTRENDERER_HPP

#include "DamnRenderer.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class Widget;
	class DrawContext;

	template<typename ElementT>
	class ElementRenderer : public DamnRenderer {

	  public:
		typedef ElementT Element;

	  public:
		enum SelectionState {
			UNSELECTED,
			SELECTED,
			PRIMARY
		};

	  public:
		ElementRenderer() {}
		ElementRenderer(const ElementRenderer& renderer) : DamnRenderer(renderer) {}

		virtual Widget& getRenditionFor(bool, const ElementT&, SelectionState, DrawContext*) = 0;
		virtual void clearCachedRenditionColors() = 0;

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_ELEMENTRENDERER_HPP */
