#ifndef REDSTRAIN_MOD_DAMNATION_TK_DAMNSELECTIONMODEL_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_DAMNSELECTIONMODEL_HPP

#include <redstrain/util/ReferenceCounted.hpp>

#include "../api.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class REDSTRAIN_DAMNATION_API DamnSelectionModel : public util::ReferenceCounted {

	  public:
		DamnSelectionModel();
		DamnSelectionModel(const DamnSelectionModel&);

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_DAMNSELECTIONMODEL_HPP */
