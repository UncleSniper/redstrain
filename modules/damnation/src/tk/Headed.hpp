#ifndef REDSTRAIN_MOD_DAMNATION_TK_HEADED_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_HEADED_HPP

#include "Widget.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class REDSTRAIN_DAMNATION_API Headed : public virtual Widget {

	  public:
		Headed();
		Headed(const Headed&);

		virtual Widget* getHeaderWidget() = 0;
		virtual bool isInlineHeader() = 0;
		virtual bool isHeaderExternallyDrawn() = 0;
		virtual void setHeaderExternallyDrawn(bool) = 0;

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_HEADED_HPP */
