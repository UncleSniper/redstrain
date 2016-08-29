#ifndef REDSTRAIN_MOD_DAMNATION_TK_ABSTRACTLISTMODEL_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_ABSTRACTLISTMODEL_HPP

#include <redstrain/util/ListenerSet.hpp>
#include <redstrain/util/FireEventAppender.hpp>

#include "ListModel.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	template<typename ElementT>
	class AbstractListModel : public virtual ListModel<ElementT> {

	  private:
		util::ListenerSet<ListModelListener<ElementT> > modelListeners;

	  protected:
		void fireElementsInserted(typename SequenceModelListener<ElementT>::SequenceModelEvent& event) const {
			util::FireEventAppender<
				ListModelListener<ElementT>,
				typename SequenceModelListener<ElementT>::SequenceModelEvent
			> sink(&ListModelListener<ElementT>::elementsInserted, event);
			modelListeners.getListeners(sink);
		}

		void fireElementsRemoved(typename SequenceModelListener<ElementT>::SequenceModelEvent& event) const {
			util::FireEventAppender<
				ListModelListener<ElementT>,
				typename SequenceModelListener<ElementT>::SequenceModelEvent
			> sink(&ListModelListener<ElementT>::elementsRemoved, event);
			modelListeners.getListeners(sink);
		}

		void fireElementReplaced(typename SequenceModelListener<ElementT>::SequenceModelEvent& event) const {
			util::FireEventAppender<
				ListModelListener<ElementT>,
				typename SequenceModelListener<ElementT>::SequenceModelEvent
			> sink(&ListModelListener<ElementT>::elementReplaced, event);
			modelListeners.getListeners(sink);
		}

		void fireElementModified(typename SequenceModelListener<ElementT>::SequenceModelEvent& event) const {
			util::FireEventAppender<
				ListModelListener<ElementT>,
				typename SequenceModelListener<ElementT>::SequenceModelEvent
			> sink(&ListModelListener<ElementT>::elementModified, event);
			modelListeners.getListeners(sink);
		}

	  public:
		AbstractListModel() {}

		AbstractListModel(const AbstractListModel& model) : SequenceModel<ElementT>(model),
				ListModel<ElementT>(model), modelListeners(model.modelListeners) {}

		virtual ~AbstractListModel() {}

		virtual void getListModelListeners(util::Appender<ListModelListener<ElementT>*>& sink) {
			modelListeners.getListeners(sink);
		}

		virtual bool addListModelListener(ListModelListener<ElementT>& modelListener) {
			return modelListeners.addListener(modelListener);
		}

		virtual bool removeListModelListener(ListModelListener<ElementT>& modelListener) {
			return modelListeners.removeListener(modelListener);
		}

		virtual void clearListModelListeners() {
			modelListeners.clearListeners();
		}

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_ABSTRACTLISTMODEL_HPP */
