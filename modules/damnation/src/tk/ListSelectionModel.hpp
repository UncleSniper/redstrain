#ifndef REDSTRAIN_MOD_DAMNATION_TK_LISTSELECTIONMODEL_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_LISTSELECTIONMODEL_HPP

#include <redstrain/util/Appender.hpp>
#include <redstrain/util/IntegerBounds.hpp>

#include "DamnSelectionModel.hpp"
#include "types.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class ListSelectionListener;

	class REDSTRAIN_DAMNATION_API ListSelectionModel : public DamnSelectionModel {

	  public:
		enum REDSTRAIN_DAMNATION_API DragDirection {
			DRAG_UP,
			DRAG_DOWN
		};

	  public:
		static const ModelElementIndex NO_SELECTION = util::IntegerBounds<ModelElementIndex>::MAX;

	  public:
		ListSelectionModel();
		ListSelectionModel(const ListSelectionModel&);

		virtual ModelElementIndex getPrimaryListSelectionIndex() = 0;
		virtual bool isListIndexSelected(ModelElementIndex) = 0;
		virtual void setPrimarySelection(ModelElementIndex) = 0;
		virtual void movePrimarySelection(ModelElementIndex) = 0;
		virtual void toggleSelectionAt(ModelElementIndex) = 0;
		virtual void dragPrimarySelection(ModelElementIndex, DragDirection) = 0;
		virtual void clearSelection() = 0;
		virtual void listElementsInserted(ModelElementIndex, ModelElementIndex, bool) = 0;
		virtual void listElementsRemoved(ModelElementIndex, ModelElementIndex, bool) = 0;
		virtual void getListSelectionListeners(util::Appender<ListSelectionListener*>&) = 0;
		virtual bool addListSelectionListener(ListSelectionListener&) = 0;
		virtual bool removeListSelectionListener(ListSelectionListener&) = 0;
		virtual void clearListSelectionListeners() = 0;

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_LISTSELECTIONMODEL_HPP */
