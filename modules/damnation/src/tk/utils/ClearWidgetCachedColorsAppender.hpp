#ifndef REDSTRAIN_MOD_DAMNATION_TK_UTIL_CLEARWIDGETCACHEDCOLORSAPPENDER_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_UTIL_CLEARWIDGETCACHEDCOLORSAPPENDER_HPP

#include <redstrain/util/Appender.hpp>

#include "../Widget.hpp"

namespace redengine {
namespace damnation {
namespace tk {
namespace utils {

	class REDSTRAIN_DAMNATION_API ClearWidgetCachedColorsAppender : public redengine::util::Appender<Widget*> {

	  public:
		ClearWidgetCachedColorsAppender();
		ClearWidgetCachedColorsAppender(const ClearWidgetCachedColorsAppender&);

		virtual void append(Widget *const&);

	};

}}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_UTIL_CLEARWIDGETCACHEDCOLORSAPPENDER_HPP */
