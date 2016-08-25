#ifndef REDSTRAIN_MOD_DAMNATION_TK_UTIL_SETWIDGETLAYERAPPENDER_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_UTIL_SETWIDGETLAYERAPPENDER_HPP

#include <redstrain/util/Appender.hpp>

#include "../Widget.hpp"

namespace redengine {
namespace damnation {
namespace tk {
namespace utils {

	class REDSTRAIN_DAMNATION_API SetWidgetLayerAppender : public redengine::util::Appender<Widget*> {

	  private:
		Layer *const layer;

	  public:
		SetWidgetLayerAppender(Layer*);
		SetWidgetLayerAppender(const SetWidgetLayerAppender&);

		inline Layer* getLayer() const {
			return layer;
		}

		virtual void append(Widget *const&);

	};

}}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_UTIL_SETWIDGETLAYERAPPENDER_HPP */
