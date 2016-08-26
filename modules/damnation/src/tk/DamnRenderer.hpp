#ifndef REDSTRAIN_MOD_DAMNATION_TK_DAMNRENDERER_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_DAMNRENDERER_HPP

#include <redstrain/util/ReferenceCounted.hpp>

#include "../api.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class REDSTRAIN_DAMNATION_API DamnRenderer : public util::ReferenceCounted {

	  public:
		DamnRenderer();
		DamnRenderer(const DamnRenderer&);

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_DAMNRENDERER_HPP */
