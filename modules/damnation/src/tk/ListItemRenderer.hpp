#ifndef REDSTRAIN_MOD_DAMNATION_TK_LISTITEMRENDERER_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_LISTITEMRENDERER_HPP

#include "ElementRenderer.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	template<typename ElementT>
	class ListItemRenderer : public virtual ElementRenderer<ElementT> {

	  public:
		ListItemRenderer() {}
		ListItemRenderer(const ListItemRenderer& renderer) : ElementRenderer<ElementT>(renderer) {}

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_LISTITEMRENDERER_HPP */
