#ifndef REDSTRAIN_MOD_DAMNATION_TK_PRESSABLE_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_PRESSABLE_HPP

#include "Widget.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class REDSTRAIN_DAMNATION_API Pressable : public virtual Widget {

	  public:
		Pressable();
		Pressable(const Pressable&);

		virtual void press() = 0;

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_PRESSABLE_HPP */
