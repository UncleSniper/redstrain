#ifndef REDSTRAIN_MOD_DAMNATION_TK_DAMNLISTENER_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_DAMNLISTENER_HPP

#include <redstrain/util/ReferenceCounted.hpp>

#include "../api.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class REDSTRAIN_DAMNATION_API DamnListener : public util::ReferenceCounted {

	  public:
		DamnListener();
		DamnListener(const DamnListener&);

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_DAMNLISTENER_HPP */
