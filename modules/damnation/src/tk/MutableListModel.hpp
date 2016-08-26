#ifndef REDSTRAIN_MOD_DAMNATION_TK_MUTABLELISTMODEL_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_MUTABLELISTMODEL_HPP

#include "ListModel.hpp"
#include "MutableSequenceModel.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	template<typename ElementT>
	class MutableListModel : public ListModel<ElementT>, public MutableSequenceModel<ElementT> {

	  public:
		MutableListModel() {}

		MutableListModel(const MutableListModel& model) : SequenceModel<ElementT>(model),
				ListModel<ElementT>(model), MutableSequenceModel<ElementT>(model) {}

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_MUTABLELISTMODEL_HPP */
