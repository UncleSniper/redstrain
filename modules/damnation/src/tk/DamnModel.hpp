#ifndef REDSTRAIN_MOD_DAMNATION_TK_DAMNMODEL_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_DAMNMODEL_HPP

#include <redstrain/util/ReferenceCounted.hpp>

#include "../api.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class REDSTRAIN_DAMNATION_API DamnModel : public util::ReferenceCounted {

	  public:
		DamnModel();
		DamnModel(const DamnModel&);

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_DAMNMODEL_HPP */
