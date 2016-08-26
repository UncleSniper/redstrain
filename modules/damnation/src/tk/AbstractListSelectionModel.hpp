#ifndef REDSTRAIN_MOD_DAMNATION_TK_ABSTRACTLISTSELECTIONMODEL_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_ABSTRACTLISTSELECTIONMODEL_HPP

#include <redstrain/util/ListenerSet.hpp>

#include "ListSelectionModel.hpp"
#include "ListSelectionListener.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class REDSTRAIN_DAMNATION_API AbstractListSelectionModel : public ListSelectionModel {

	  private:
		util::ListenerSet<ListSelectionListener> selectionListeners;

	  protected:
		void firePrimaryListSelectionChanged(ListSelectionListener::PrimaryListSelectionEvent&) const;
		void fireListItemSelectionChanged(ListSelectionListener::SecondaryListSelectionEvent&) const;

	  public:
		AbstractListSelectionModel();
		AbstractListSelectionModel(const AbstractListSelectionModel&);
		virtual ~AbstractListSelectionModel();

		virtual void getListSelectionListeners(util::Appender<ListSelectionListener*>&);
		virtual bool addListSelectionListener(ListSelectionListener&);
		virtual bool removeListSelectionListener(ListSelectionListener&);
		virtual void clearListSelectionListeners();

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_ABSTRACTLISTSELECTIONMODEL_HPP */
