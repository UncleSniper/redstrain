#ifndef REDSTRAIN_MOD_DAMNATION_TK_SINGLELISTSELECTIONMODEL_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_SINGLELISTSELECTIONMODEL_HPP

#include "AbstractListSelectionModel.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class REDSTRAIN_DAMNATION_API SingleListSelectionModel : public AbstractListSelectionModel {

	  private:
		ModelElementIndex selectedIndex;

	  public:
		SingleListSelectionModel();
		SingleListSelectionModel(const SingleListSelectionModel&);

		virtual ModelElementIndex getPrimaryListSelectionIndex();
		virtual bool isListIndexSelected(ModelElementIndex);
		virtual void setPrimarySelection(ModelElementIndex);
		virtual void movePrimarySelection(ModelElementIndex);
		virtual void toggleSelectionAt(ModelElementIndex);
		virtual void dragPrimarySelection(ModelElementIndex, DragDirection);
		virtual void clearSelection();
		virtual void listElementsInserted(ModelElementIndex, ModelElementIndex, bool);
		virtual void listElementsRemoved(ModelElementIndex, ModelElementIndex, bool);

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_SINGLELISTSELECTIONMODEL_HPP */
