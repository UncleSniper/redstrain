#ifndef REDSTRAIN_MOD_DAMNATION_TK_SEQUENCEMODELLISTENER_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_SEQUENCEMODELLISTENER_HPP

#include "DamnEvent.hpp"
#include "DamnListener.hpp"
#include "types.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	template<typename ElementT>
	class SequenceModelListener : public DamnListener {

	  public:
		typedef ElementT Element;

	  public:
		class SequenceModelEvent : public DamnEvent {

		  private:
			ModelElementIndex startIndex, endIndex;
			const ElementT *oldElement, *newElement;

		  public:
			SequenceModelEvent(ModelElementIndex startIndex, ModelElementIndex endIndex,
					const Element* oldElement, const ElementT* newElement)
					: startIndex(startIndex), endIndex(endIndex), oldElement(oldElement), newElement(newElement) {}

			SequenceModelEvent(const SequenceModelEvent& event) : DamnEvent(event), startIndex(event.startIndex),
					endIndex(event.endIndex), oldElement(event.oldElement), newElement(event.newElement) {}

			inline ModelElementIndex getStartIndex() const {
				return startIndex;
			}

			inline void setStartIndex(ModelElementIndex startIndex) {
				this->startIndex = startIndex;
			}

			inline ModelElementIndex getEndIndex() const {
				return endIndex;
			}

			inline void setEndIndex(ModelElementIndex endIndex) {
				this->endIndex = endIndex;
			}

			inline const ElementT* getOldElement() const {
				return oldElement;
			}

			inline void setOldElement(const ElementT* oldElement) {
				this->oldElement = oldElement;
			}

			inline const ElementT* getNewElement() const {
				return newElement;
			}

			inline void setNewElement(const ElementT* newElement) {
				this->newElement = newElement;
			}

		};

	  public:
		SequenceModelListener() {}
		SequenceModelListener(const SequenceModelListener& listener) : DamnListener(listener) {}

		virtual void elementsInserted(SequenceModelEvent&) = 0;
		virtual void elementsRemoved(SequenceModelEvent&) = 0;
		virtual void elementReplaced(SequenceModelEvent&) = 0;
		virtual void elementModified(SequenceModelEvent&) = 0;

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_SEQUENCEMODELLISTENER_HPP */
