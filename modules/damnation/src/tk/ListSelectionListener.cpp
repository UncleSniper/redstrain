#include "ListSelectionListener.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	// ======== ListSelectionEvent ========

	ListSelectionListener::ListSelectionEvent::ListSelectionEvent(ListSelectionModel& model)
			: selectionModel(model) {}

	ListSelectionListener::ListSelectionEvent::ListSelectionEvent(const ListSelectionEvent& event)
			: DamnEvent(event), selectionModel(event.selectionModel) {}

	// ======== PrimaryListSelectionEvent ========

	ListSelectionListener::PrimaryListSelectionEvent::PrimaryListSelectionEvent(ListSelectionModel& model,
			ModelElementIndex oldIndex, ModelElementIndex newIndex)
			: ListSelectionEvent(model), oldIndex(oldIndex), newIndex(newIndex) {}

	ListSelectionListener::PrimaryListSelectionEvent::PrimaryListSelectionEvent(const
			PrimaryListSelectionEvent& event) : ListSelectionEvent(event),
			oldIndex(event.oldIndex), newIndex(event.newIndex) {}

	// ======== SecondaryListSelectionEvent ========

	ListSelectionListener::SecondaryListSelectionEvent::SecondaryListSelectionEvent(ListSelectionModel& model,
			ModelElementIndex fromIndex, ModelElementIndex toIndex, bool oldState)
			: ListSelectionEvent(model), fromIndex(fromIndex), toIndex(toIndex), oldState(oldState) {}

	ListSelectionListener::SecondaryListSelectionEvent::SecondaryListSelectionEvent(const
			SecondaryListSelectionEvent& event) : ListSelectionEvent(event),
			fromIndex(event.fromIndex), toIndex(event.toIndex), oldState(event.oldState) {}

	// ======== ListSelectionListener ========

	ListSelectionListener::ListSelectionListener() {}

	ListSelectionListener::ListSelectionListener(const ListSelectionListener& listener) : DamnListener(listener) {}

}}}
