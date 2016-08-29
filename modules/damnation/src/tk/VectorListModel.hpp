#ifndef REDSTRAIN_MOD_DAMNATION_TK_VECTORLISTMODEL_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_VECTORLISTMODEL_HPP

#include <vector>
#include <redstrain/util/destructors.hpp>

#include "MutableListModel.hpp"
#include "AbstractListModel.hpp"
#include "ListModelIndexOutOfBoundsError.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	template<typename ElementT>
	class VectorListModel : public AbstractListModel<ElementT>, public MutableListModel<ElementT> {

	  public:
		typedef std::vector<ElementT> Vector;

	  private:
		Vector vector;
		typedef typename Vector::size_type Size;
		typedef typename Vector::const_iterator Iterator;

	  public:
		VectorListModel() {}

		VectorListModel(const Vector& vector) : vector(vector) {}

		VectorListModel(const VectorListModel& model) : SequenceModel<ElementT>(model),
				ListModel<ElementT>(model), AbstractListModel<ElementT>(model), MutableListModel<ElementT>(model),
				vector(model.vector) {}

		virtual ~VectorListModel() {}

		virtual ModelElementIndex getElementCount() {
			return static_cast<ModelElementIndex>(vector.size());
		}

		virtual ElementT& getElementAt(ModelElementIndex index) {
			if(index >= static_cast<ModelElementIndex>(vector.size()))
				throw ListModelIndexOutOfBoundsError(index);
			return vector[static_cast<Size>(index)];
		}

		virtual void getElements(util::Appender<ElementT>& sink) {
			Iterator begin(vector.begin()), end(vector.end());
			for(; begin != end; ++begin)
				sink.append(*begin);
			sink.doneAppending();
		}

		virtual void notifyElementModified(ModelElementIndex index) {
			if(index >= static_cast<ModelElementIndex>(vector.size()))
				throw ListModelIndexOutOfBoundsError(index);
			typename SequenceModelListener<ElementT>::SequenceModelEvent
					event(index, index, NULL, &vector[static_cast<Size>(index)]);
			this->fireElementModified(event);
		}

		virtual void insertElement(ModelElementIndex index, const ElementT& element) {
			if(index > static_cast<ModelElementIndex>(vector.size()))
				throw ListModelIndexOutOfBoundsError(index);
			vector.insert(vector.begin() + static_cast<Size>(index), element);
			typename SequenceModelListener<ElementT>::SequenceModelEvent
					event(index, index + static_cast<ModelElementIndex>(1u), NULL, &element);
			this->fireElementsInserted(event);
		}

		virtual void appendElement(const ElementT& element) {
			ModelElementIndex index = static_cast<ModelElementIndex>(vector.size());
			vector.push_back(element);
			typename SequenceModelListener<ElementT>::SequenceModelEvent
					event(index, index + static_cast<ModelElementIndex>(1u), NULL, &element);
			this->fireElementsInserted(event);
		}

		virtual void removeElementAt(ModelElementIndex index) {
			if(index >= static_cast<ModelElementIndex>(vector.size()))
				throw ListModelIndexOutOfBoundsError(index);
			const ElementT oldElement(const_cast<const ElementT&>(vector[static_cast<Size>(index)]));
			vector.erase(vector.begin() + static_cast<Size>(index));
			typename SequenceModelListener<ElementT>::SequenceModelEvent
					event(index, index + static_cast<ModelElementIndex>(1u), &oldElement, NULL);
			this->fireElementsRemoved(event);
		}

		virtual void removeElementsAt(ModelElementIndex fromIndex, ModelElementIndex toIndex) {
			if(toIndex <= fromIndex)
				return;
			if(toIndex > static_cast<ModelElementIndex>(vector.size()))
				throw ListModelIndexOutOfBoundsError(toIndex);
			const ElementT oldElement(const_cast<const ElementT&>(vector[static_cast<Size>(fromIndex)]));
			vector.erase(vector.begin() + static_cast<Size>(fromIndex), vector.begin() + static_cast<Size>(toIndex));
			typename SequenceModelListener<ElementT>::SequenceModelEvent
					event(fromIndex, toIndex, &oldElement, NULL);
			this->fireElementsRemoved(event);
		}

		virtual void setElementAt(ModelElementIndex index, const ElementT& newElement) {
			if(index >= static_cast<ModelElementIndex>(vector.size()))
				throw ListModelIndexOutOfBoundsError(index);
			ElementT& element = vector[static_cast<Size>(index)];
			const ElementT oldElement(const_cast<const ElementT&>(element));
			element = newElement;
			typename SequenceModelListener<ElementT>::SequenceModelEvent
					event(index, index + static_cast<ModelElementIndex>(1u), &oldElement, &newElement);
			this->fireElementReplaced(event);
		}

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_VECTORLISTMODEL_HPP */
