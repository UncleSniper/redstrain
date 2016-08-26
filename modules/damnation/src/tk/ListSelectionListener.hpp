#ifndef REDSTRAIN_MOD_DAMNATION_TK_LISTSELECTIONLISTENER_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_LISTSELECTIONLISTENER_HPP

#include "DamnEvent.hpp"
#include "DamnListener.hpp"
#include "types.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	class ListSelectionModel;

	class REDSTRAIN_DAMNATION_API ListSelectionListener : public DamnListener {

	  public:
		class REDSTRAIN_DAMNATION_API ListSelectionEvent : public DamnEvent {

		  private:
			ListSelectionModel& selectionModel;

		  public:
			ListSelectionEvent(ListSelectionModel&);
			ListSelectionEvent(const ListSelectionEvent&);

			inline ListSelectionModel& getSelectionModel() {
				return selectionModel;
			}

			inline const ListSelectionModel& getSelectionModel() const {
				return selectionModel;
			}

		};

		class REDSTRAIN_DAMNATION_API PrimaryListSelectionEvent : public ListSelectionEvent {

		  private:
			ModelElementIndex oldIndex, newIndex;

		  public:
			PrimaryListSelectionEvent(ListSelectionModel&, ModelElementIndex, ModelElementIndex);
			PrimaryListSelectionEvent(const PrimaryListSelectionEvent&);

			inline ModelElementIndex getOldIndex() const {
				return oldIndex;
			}

			inline void setOldIndex(ModelElementIndex oldIndex) {
				this->oldIndex = oldIndex;
			}

			inline ModelElementIndex getNewIndex() const {
				return newIndex;
			}

			inline void setNewIndex(ModelElementIndex newIndex) {
				this->newIndex = newIndex;
			}

		};

		class REDSTRAIN_DAMNATION_API SecondaryListSelectionEvent : public ListSelectionEvent {

		  private:
			ModelElementIndex fromIndex, toIndex;
			bool oldState;

		  public:
			SecondaryListSelectionEvent(ListSelectionModel&, ModelElementIndex, ModelElementIndex, bool);
			SecondaryListSelectionEvent(const SecondaryListSelectionEvent&);

			inline ModelElementIndex getFromIndex() const {
				return fromIndex;
			}

			inline void setFromIndex(ModelElementIndex fromIndex) {
				this->fromIndex = fromIndex;
			}

			inline ModelElementIndex getToIndex() const {
				return toIndex;
			}

			inline void setToIndex(ModelElementIndex toIndex) {
				this->toIndex = toIndex;
			}

			inline bool wasSelected() const {
				return oldState;
			}

			inline bool isNowSelected() const {
				return !oldState;
			}

			inline void setOldState(bool oldState) {
				this->oldState = oldState;
			}

		};

	  public:
		ListSelectionListener();
		ListSelectionListener(const ListSelectionListener&);

		virtual void primaryListSelectionChanged(PrimaryListSelectionEvent&) = 0;
		virtual void listItemSelectionChanged(SecondaryListSelectionEvent&) = 0;

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_LISTSELECTIONLISTENER_HPP */
