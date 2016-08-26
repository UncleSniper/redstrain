#ifndef REDSTRAIN_MOD_DAMNATION_TK_DEFAULTLISTITEMRENDERER_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_DEFAULTLISTITEMRENDERER_HPP

#include "ListItemRenderer.hpp"
#include "AbstractDefaultElementRenderer.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	template<
		typename ElementT,
		typename StringifierT = typename AbstractDefaultElementRenderer<ElementT>::Stringifier
	>
	class DefaultListItemRenderer
			: public AbstractDefaultElementRenderer<ElementT, StringifierT>, ListItemRenderer<ElementT> {

	  public:
		DefaultListItemRenderer(const StringifierT& stringifier)
				: AbstractDefaultElementRenderer<ElementT, StringifierT>(stringifier) {}

		DefaultListItemRenderer(const DefaultListItemRenderer& renderer)
				: ElementRenderer<ElementT>(renderer),
				AbstractDefaultElementRenderer<ElementT, StringifierT>(renderer),
				ListItemRenderer<ElementT>(renderer) {}

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_DEFAULTLISTITEMRENDERER_HPP */
