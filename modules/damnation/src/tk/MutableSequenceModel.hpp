#ifndef REDSTRAIN_MOD_DAMNATION_TK_MUTABLESEQUENCEMODEL_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_MUTABLESEQUENCEMODEL_HPP

#include "SequenceModel.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	template<typename ElementT>
	class MutableSequenceModel : public virtual SequenceModel<ElementT> {

	  public:
		MutableSequenceModel() {}
		MutableSequenceModel(const MutableSequenceModel& model) : SequenceModel<ElementT>(model) {}

		virtual void insertElement(ModelElementIndex, const ElementT&) = 0;
		virtual void appendElement(const ElementT&) = 0;
		virtual void removeElementAt(ModelElementIndex) = 0;
		virtual void removeElementsAt(ModelElementIndex, ModelElementIndex) = 0;
		virtual void setElementAt(ModelElementIndex, const ElementT&) = 0;

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_MUTABLESEQUENCEMODEL_HPP */
