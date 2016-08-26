#ifndef REDSTRAIN_MOD_DAMNATION_TK_LISTMODEL_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_LISTMODEL_HPP

#include "SequenceModel.hpp"
#include "ListModelListener.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	template<typename ElementT>
	class ListModel : public virtual SequenceModel<ElementT> {

	  public:
		ListModel() {}
		ListModel(const ListModel& model) : SequenceModel<ElementT>(model) {}

		virtual void getListModelListeners(util::Appender<ListModelListener<ElementT>*>&) = 0;
		virtual bool addListModelListener(ListModelListener<ElementT>&) = 0;
		virtual bool removeListModelListener(ListModelListener<ElementT>&) = 0;
		virtual void clearListModelListeners() = 0;

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_LISTMODEL_HPP */
