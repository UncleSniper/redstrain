#include "SingleListSelectionModel.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	SingleListSelectionModel::SingleListSelectionModel() : selectedIndex(static_cast<ModelElementIndex>(0u)) {}

	SingleListSelectionModel::SingleListSelectionModel(const SingleListSelectionModel& model)
			: AbstractListSelectionModel(model), selectedIndex(model.selectedIndex) {}

	ModelElementIndex SingleListSelectionModel::getPrimaryListSelectionIndex() {
		return selectedIndex;
	}

	bool SingleListSelectionModel::isListIndexSelected(ModelElementIndex index) {
		return index != ListSelectionModel::NO_SELECTION && index == selectedIndex;
	}

	void SingleListSelectionModel::setPrimarySelection(ModelElementIndex index) {
		if(index == selectedIndex)
			return;
		ListSelectionListener::PrimaryListSelectionEvent event(*this, selectedIndex, index);
		selectedIndex = index;
		firePrimaryListSelectionChanged(event);
	}

	void SingleListSelectionModel::movePrimarySelection(ModelElementIndex index) {
		setPrimarySelection(index);
	}

	void SingleListSelectionModel::toggleSelectionAt(ModelElementIndex index) {
		if(index != ListSelectionModel::NO_SELECTION)
			setPrimarySelection(index == selectedIndex ? ListSelectionModel::NO_SELECTION : index);
	}

	void SingleListSelectionModel::dragPrimarySelection(ModelElementIndex fromIndex, DragDirection direction) {
		if(direction == DRAG_UP) {
			if(fromIndex == ListSelectionModel::NO_SELECTION)
				setPrimarySelection(ListSelectionModel::NO_SELECTION);
			else if(fromIndex)
				setPrimarySelection(fromIndex - static_cast<ModelElementIndex>(1u));
		}
		else
			setPrimarySelection(fromIndex == ListSelectionModel::NO_SELECTION
					? static_cast<ModelElementIndex>(0u) : fromIndex + static_cast<ModelElementIndex>(1u));
	}

	void SingleListSelectionModel::clearSelection() {
		setPrimarySelection(ListSelectionModel::NO_SELECTION);
	}

	void SingleListSelectionModel::listElementsInserted(ModelElementIndex beforeIndex, ModelElementIndex count,
			bool backInserted) {
		if(selectedIndex == ListSelectionModel::NO_SELECTION)
			return;
		if(!beforeIndex && backInserted)
			selectedIndex = static_cast<ModelElementIndex>(0u);
		else if(selectedIndex >= beforeIndex)
			selectedIndex += count;
	}

	void SingleListSelectionModel::listElementsRemoved(ModelElementIndex fromIndex, ModelElementIndex toIndex,
			bool backRemoved) {
		if(selectedIndex == ListSelectionModel::NO_SELECTION)
			return;
		if(toIndex < fromIndex) {
			ModelElementIndex index = fromIndex;
			fromIndex = toIndex;
			toIndex = index;
		}
		if(selectedIndex >= toIndex)
			selectedIndex -= toIndex - fromIndex;
		else if(selectedIndex >= fromIndex && selectedIndex < toIndex) {
			if(backRemoved)
				setPrimarySelection(fromIndex ? fromIndex - static_cast<ModelElementIndex>(1u)
						: ListSelectionModel::NO_SELECTION);
			else
				setPrimarySelection(fromIndex);
		}
	}

}}}
