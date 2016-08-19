#ifndef REDSTRAIN_MOD_DAMNATION_TK_BORDER_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_BORDER_HPP

#include <redstrain/util/ReferenceCounted.hpp>

#include "Insets.hpp"
#include "../Rectangle.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class DrawContext;

	class REDSTRAIN_DAMNATION_API Border : public util::ReferenceCounted {

	  public:
		Border();
		Border(const Border&);

		virtual Insets getBorderSize() = 0;
		virtual void drawBorder(const Rectangle&, bool, DrawContext&) = 0;

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_BORDER_HPP */
