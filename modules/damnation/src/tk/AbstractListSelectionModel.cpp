#include <redstrain/util/FireEventAppender.hpp>

#include "AbstractListSelectionModel.hpp"

using redengine::util::Appender;
using redengine::util::FireEventAppender;

namespace redengine {
namespace damnation {
namespace tk {

	AbstractListSelectionModel::AbstractListSelectionModel() {}

	AbstractListSelectionModel::AbstractListSelectionModel(const AbstractListSelectionModel& model)
			: ListSelectionModel(model), selectionListeners(model.selectionListeners) {}

	AbstractListSelectionModel::~AbstractListSelectionModel() {}

	void AbstractListSelectionModel::getListSelectionListeners(Appender<ListSelectionListener*>& sink) {
		selectionListeners.getListeners(sink);
	}

	bool AbstractListSelectionModel::addListSelectionListener(ListSelectionListener& selectionListener) {
		return selectionListeners.addListener(selectionListener);
	}

	bool AbstractListSelectionModel::removeListSelectionListener(ListSelectionListener& selectionListener) {
		return selectionListeners.removeListener(selectionListener);
	}

	void AbstractListSelectionModel::clearListSelectionListeners() {
		selectionListeners.clearListeners();
	}

	void AbstractListSelectionModel::firePrimaryListSelectionChanged(
			ListSelectionListener::PrimaryListSelectionEvent& event) const {
		FireEventAppender<ListSelectionListener, ListSelectionListener::PrimaryListSelectionEvent>
				sink(&ListSelectionListener::primaryListSelectionChanged, event);
		selectionListeners.getListeners(sink);
	}

	void AbstractListSelectionModel::fireListItemSelectionChanged(
			ListSelectionListener::SecondaryListSelectionEvent& event) const {
		FireEventAppender<ListSelectionListener, ListSelectionListener::SecondaryListSelectionEvent>
				sink(&ListSelectionListener::listItemSelectionChanged, event);
		selectionListeners.getListeners(sink);
	}

}}}
