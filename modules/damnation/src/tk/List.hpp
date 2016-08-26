#ifndef REDSTRAIN_MOD_DAMNATION_TK_LIST_HPP
#define REDSTRAIN_MOD_DAMNATION_TK_LIST_HPP

#include <redstrain/util/Unref.hpp>
#include <redstrain/util/StringUtils.hpp>
#include <redstrain/error/ProgrammingError.hpp>

#include "Axis.hpp"
#include "ListModel.hpp"
#include "LeafWidget.hpp"
#include "ListItemRenderer.hpp"
#include "SingleListSelectionModel.hpp"

namespace redengine {
namespace damnation {
namespace tk {

	template<typename ElementT>
	class List : public LeafWidget, private ListModelListener<ElementT>, private ListSelectionListener {

	  private:
		static const int HAVE_MINIMUM_SIZE   = 01;
		static const int HAVE_MAXIMUM_SIZE   = 02;
		static const int HAVE_PREFERRED_SIZE = 04;

	  private:
		class ModelListener : public ListModelListener<ElementT> {

		  private:
			List& list;

		  public:
			ModelListener(List& list) : list(list) {}

			ModelListener(const ModelListener& listener)
					: ListModelListener<ElementT>(listener), list(listener.list) {}

			virtual void elementsInserted(SequenceModelEvent& event) {
				const ModelElementIndex start = event.getStartIndex(), end = event.getEndIndex();
				if(list.greedyGeometryUpdates)
					list.recalculateGeometry();
				else if(list.cachedSizes) {
					bool active = list.isFocused();
					ModelElementIndex index;
					for(index = start; index < end; ++index) {
						Widget& rendition = list.renderer->getRenditionFor(active,
								list.model->getElementAt(index), list.selectionStateOf(index), NULL);
						if(list.cachedSizes & List::HAVE_MINIMUM_SIZE) {
							const Size size(rendition.getMinimumSize());
							if(size.width > list.minimumSize.width)
								list.minimumSize.width = size.width;
							list.minimumSize.height += size.height;
						}
						if(list.cachedSizes & List::HAVE_MAXIMUM_SIZE) {
							const Size size(rendition.getMaximumSize());
							if(size.width != Widget::NO_SIZE_LIMIT
									&& (list.maximumSize.width == Widget::NO_SIZE_LIMIT
									|| size.width < list.maximumSize.width))
								list.maximumSize.width = size.width;
							if(size.height == Widget::NO_SIZE_LIMIT)
								list.maximumSize.height = Widget::NO_SIZE_LIMIT;
							else if(list.maximumSize.height != Widget::NO_SIZE_LIMIT)
								list.maximumSize.height += size.height;
						}
						if(list.cachedSizes & List::HAVE_PREFERRED_SIZE) {
							const Size size(rendition.getPreferredSize());
							if(size.width > list.preferredSize.width)
								list.preferredSize.width = size.width;
							list.preferredSize.height += size.height;
						}
					}
					list.notifyListContentGeometryChanged();
				}
				if(end > start)
					list.selectionModel->listElementsInserted(start, end - start,
							end >= list->model->getElementCount());
			}

			virtual void elementsRemoved(SequenceModelEvent& event) {
				const ModelElementIndex start = event.getStartIndex(), end = event.getEndIndex();
				if(list.greedyGeometryUpdates || end != start + static_cast<ModelElementIndex>(1u)
						|| !event.getOldElement())
					list.recalculateGeometry();
				else if(list.cachedSizes) {
					Widget& rendition = list.renderer->getRenditionFor(list.isFocused(),
							*event.getOldElement(), list.selectionStateOf(start), NULL);
					if(list.cachedSizes & List::HAVE_MINIMUM_SIZE)
						list.minimumSize.height -= rendition.getMinimumSize().height;
					if((list.cachedSizes & List::HAVE_MAXIMUM_SIZE)
							&& list.maximumSize.height != Widget::NO_SIZE_LIMIT) {
						const Size size(rendition.getMaximumSize());
						if(size.height != Widget::NO_SIZE_LIMIT)
							list.maximumSize.height -= size.height;
					}
					if(list.cachedSizes & List::HAVE_PREFERRED_SIZE)
						list.preferredSize.height -= rendition.getPreferredSize().height;
					list.notifyListContentGeometryChanged();
				}
				if(end > start)
					list.selectionModel->listElementsRemoved(start, end, start >= list->model->getElementCount());
			}

			virtual void elementReplaced(SequenceModelEvent& event) {
				const ModelElementIndex start = event.getStartIndex(), end = event.getEndIndex();
				if(list.greedyGeometryUpdates || end != start + static_cast<ModelElementIndex>(1u)
						|| !event.getOldElement())
					list.recalculateGeometry();
				else if(list.cachedSizes) {
					bool active = list.isFocused();
					ElementRenderer::SelectionState state = list.selectionStateOf(start);
					Widget& oldRendition = list.renderer->getRenditionFor(active,
							*event.getOldElement(), state, NULL);
					const Size oldMinimum(oldRendition.getMinimumSize());
					const Size oldMaximum(oldRendition.getMaximumSize());
					const Size oldPreferred(oldRendition.getPreferredSize());
					const ElementT* newElement = event.getNewElement();
					if(!newElement)
						newElement = &list.model->getElementAt(start);
					Widget& newRendition = list.renderer->getRenditionFor(active, *newElement, state, NULL);
					if(list.cachedSizes & List::HAVE_MINIMUM_SIZE) {
						const Size newMinimum(rendition.getMinimumSize());
						if(newMinimum.width > list.minimumSize.width)
							list.minimumSize.width = newMinimum.width;
						list.minimumSize.height += newMinimum.height;
						list.minimumSize.height = list.minimumSize.height > oldMinimum.height
								? list.minimumSize.height - oldMinimum.height : 0u;
					}
					if(list.cachedSizes & List::HAVE_MAXIMUM_SIZE) {
						const Size newMaximum(rendition.getMaximumSize());
						if(newMaximum.height == Widget::NO_SIZE_LIMIT)
							list.maximumSize.height = Widget::NO_SIZE_LIMIT;
						else if(list.maximumSize.height != Widget::NO_SIZE_LIMIT
								&& oldMaximum.height != Widget::NO_SIZE_LIMIT) {
							list.maximumSize.height += newMaximum.height;
							list.maximumSize.height = list.maximumSize.height > oldMaximum.height
									? list.maximumSize.height - oldMaximum.height : 0u;
						}
						if(newMaximum.width != Widget::NO_SIZE_LIMIT
								&& (list.maximumSize.width == Widget::NO_SIZE_LIMIT
								|| newMaximum.width < list.maximumSize.width))
							list.maximumSize.width = newMaximum.width;
					}
					if(list.cachedSizes & List::HAVE_PREFERRED_SIZE) {
						const Size newPreferred(rendition.getPreferredSize());
						if(newPreferred.width > list.preferredSize.width)
							list.preferredSize.width = newPreferred.width;
						list.preferredSize.height += newPreferred.height;
						list.preferredSize.height = list.preferredSize.height > oldPreferred.height
								? list.preferredSize.height - oldPreferred.height : 0u;
					}
					list.notifyListContentGeometryChanged();
				}
			}

			virtual void elementModified(SequenceModelEvent&) {
				list.recalculateGeometry();
			}

		};

		class SelectionListener : public ListSelectionListener {

		  private:
			List& list;

		  public:
			SelectionListener(List& list) : list(list) {}

			SelectionListener(const SelectionListener& listener)
					: ListSelectionListener(listener), list(listener.list) {}

			virtual void primaryListSelectionChanged(PrimaryListSelectionEvent& event) {
				ModelElementIndex selectedIndex = event.getNewIndex();
				if(selectedIndex != ListSelectionModel::NO_SELECTION)
					list.makeRectVisible(list.getItemGeometry(selectedIndex), GRAV_WEST);
			}

			virtual void listItemSelectionChanged(SecondaryListSelectionEvent&) {}

		};

		class CalculateSizeElementAppender : public util::Appender<ElementT> {

		  private:
			const List& list;
			Size size;
			const bool active;
			ModelElementIndex index;
			const int mode;
			DrawContext *const context;

		  public:
			CalculateSizeElementAppender(const List& list, unsigned width, unsigned height, bool active, int mode,
					DrawContext* context) : list(list), size(width, height), active(active),
					index(static_cast<ModelElementIndex>(0u)), mode(mode), context(context) {}

			CalculateSizeElementAppender(const CalculateSizeElementAppender& appender)
					: Appender<ElementT>(appender), list(appender.list), size(appender.size),
					active(appender.active), index(appender.index), mode(appender.mode), context(appender.context) {}

			inline const Size& getSize() const {
				return size;
			}

			virtual void append(const ElementT& element) {
				Widget& rendition = list.renderer->getRenditionFor(active, element,
						list.selectionStateOf(index), context);
				switch(mode) {
					case List::HAVE_MINIMUM_SIZE:
						{
							const Size min(rendition.getMinimumSize());
							if(min.width > size.width)
								size.width = min.width;
							size.height += min.height;
						}
						break;
					case List::HAVE_MAXIMUM_SIZE:
						{
							const Size max(rendition.getMaximumSize());
							if(max.width != Widget::NO_SIZE_LIMIT
									&& (size.width == Widget::NO_SIZE_LIMIT || max.width < size.width))
								size.width = max.width;
							if(max.height == Widget::NO_SIZE_LIMIT)
								size.height = Widget::NO_SIZE_LIMIT;
							else if(size.height != Widget::NO_SIZE_LIMIT)
								size.height += max.height;
						}
						break;
					case List::HAVE_PREFERRED_SIZE:
						{
							const Size pref(rendition.getPreferredSize());
							if(pref.width > size.width)
								size.width = pref.width;
							size.height += pref.height;
						}
						break;
					default:
						throw error::ProgrammingError("Unrecognized mode in "
								"List::CalculateSizeElementAppender::append(): "
								+ util::StringUtils::toString(mode));
				}
				++index;
			}

		};

		class CalculateDimensionElementAppender : public util::Appender<ElementT> {

		  private:
			const List& list;
			unsigned size;
			const unsigned constraint;
			const bool active;
			ModelElementIndex index;
			const Axis axis;

		  public:
			CalculateDimensionElementAppender(const List& list, unsigned constraint, bool active, Axis axis)
					: list(list), size(0u), constraint(constraint), active(active),
					index(static_cast<ModelElementIndex>(0u)), axis(axis) {}

			CalculateDimensionElementAppender(const CalculateDimensionElementAppender& appender)
					: Appender<ElementT>(appender), list(appender.list), size(appender.size),
					constraint(appender.constraint), active(appender.active), index(appender.index),
					axis(appender.axis) {}

			inline unsigned getSize() const {
				return size;
			}

			virtual void append(const ElementT& element) {
				Widget& rendition = list.renderer->getRenditionFor(active, element,
						list.selectionStateOf(index), NULL);
				switch(axis) {
					case AXS_VERTICAL:
						size += rendition.getHeightForWidth(constraint);
						break;
					case AXS_HORIZONTAL:
						{
							unsigned w = rendition.getWidthForHeight(constraint);
							if(w > size)
								size = w;
						}
						break;
					default:
						throw error::ProgrammingError("Unrecognized Axis in "
								"List::CalculateDimensionElementAppender::append(): "
								+ util::StringUtils::toString(static_cast<int>(axis)));
				}
				++index;
			}

		};

		class DrawElementAppender : public util::Appender<ElementT> {

		  private:
			const List& list;
			ModelElementIndex index;
			unsigned progress;
			const bool active;
			const Rectangle& area;
			DrawContext& context;

		  public:
			DrawElementAppender(const List& list, bool active, const Rectangle& area, DrawContext& context)
					: list(list), index(static_cast<ModelElementIndex>(0u)), progress(0u), active(active),
					area(area), context(context) {}

			DrawElementAppender(const DrawElementAppender& appender) : Appender<ElementT>(appender),
					list(appender.list), index(appender.index), progress(appender.progress), active(appender.active),
					area(appender.area), context(appender.context) {}

			inline unsigned getProgress() const {
				return progress;
			}

			virtual void append(const ElementT& element) {
				if(progress >= area.height)
					return;
				Widget& rendition = list.renderer->getRenditionFor(active, element,
						list.selectionStateOf(index), &context);
				Size rsize(rectangle.height < list.preferredSize.height
						? rendition.getMinimumSize() : rendition.getPreferredSize());
				unsigned available = area.height - progress;
				if(rsize.height > available)
					rsize.height = available;
				rsize.width = area.width;
				rendition.setSize(rsize);
				rendition.draw(Point(area.row + static_cast<int>(progress), area.column), context);
				progress += rsize.height;
				++index;
			}

		};

		class FindRenditionRowElementAppender : public util::Appender<ElementT> {

		  private:
			List& list;
			const ModelElementIndex targetIndex;
			ModelElementIndex index;
			unsigned progress, rheight;
			bool found;
			const bool active;
			const Size containerSize;
			bool useMinimum;

		  public:
			FindRenditionRowElementAppender(List& list, ModelElementIndex targetIndex)
					: list(list), targetIndex(targetIndex), index(static_cast<ModelElementIndex>(0u)),
					progress(0u), rheight(0u), found(false), active(list.isFocused()),
					containerSize(list.getSizeWithinBorder()) {
				if(!(list.cachedSizes & List::HAVE_PREFERRED_SIZE)) {
					list.preferredSize = list.calculatePreferredSize(NULL);
					list.cachedSizes |= List::HAVE_PREFERRED_SIZE;
				}
				useMinimum = containerSize.height < list.preferredSize.height;
			}

			FindRenditionRowElementAppender(const FindRenditionRowElementAppender& appender)
					: Appender<ElementT>(appender), list(appender.list), targetIndex(appender.targetIndex),
					index(appender.index), progress(appender.progress), rheight(appender.rheight),
					found(appender.found), active(appender.active), containerSize(appender.containerSize),
					useMinimum(appender.useMinimum) {}

			inline unsigned getRenditionRow() const {
				return progress;
			}

			inline unsigned getRenditionWidth() const {
				return containerSize.width;
			}

			inline unsigned getRenditionHeight() const {
				return rheight;
			}

			virtual void append(const ElementT& element) {
				if(found)
					return;
				Widget& rendition = list.renderer->getRenditionFor(active, element,
						list.selectionStateOf(index), NULL);
				const Size rsize(useMinimum ? rendition.getMinimumSize() : rendition.getPreferredSize());
				unsigned available = containerSize.height - progress;
				if(rsize.height > available)
					rsize.height = available;
				if(index == targetIndex) {
					rheight = rsize.height;
					found = true;
					return;
				}
				progress += rsize.height;
				if(progress >= containerSize.height) {
					progress = containerSize.height;
					found = true;
					return;
				}
				++index;
			}

		};

	  private:
		ListModel<ElementT>* model;
		ModelListener modelListener;
		ListItemRenderer<ElementT>* renderer;
		SingleListSelectionModel defaultSelectionModel;
		ListSelectionModel* selectionModel;
		SelectionListener selectionListener;
		Size minimumSize, maximumSize, preferredSize;
		int cachedSizes;
		bool greedyGeometryUpdates;

	  private:
		void recalculateGeometry() {
			cachedSizes = 0;
			notifyListContentGeometryChanged();
		}

		void updateForNewModel() {
			recalculateGeometry();
		}

		void updateForNewRenderer() {
			recalculateGeometry();
		}

		ElementRenderer::SelectionState selectionStateOf(ModelElementIndex index) const {
			if(index == selectionModel->getPrimaryListSelectionIndex())
				return ElementRenderer::PRIMARY;
			if(selectionModel->isListIndexSelected(index))
				return ElementRenderer::SELECTED;
			return ElementRenderer::UNSELECTED;
		}

		Size calculateMinimumSize(DrawContext* context) {
			CalculateSizeElementAppender sink(*this, 0u, 0u, isFocused(), List::HAVE_MINIMUM_SIZE, context);
			model->getElements(sink);
			return sink.getSize();
		}

		Size calculateMaximumSize(DrawContext* context) {
			CalculateSizeElementAppender sink(*this, Widget::NO_SIZE_LIMIT, 0u,
					isFocused(), List::HAVE_MAXIMUM_SIZE, context);
			model->getElements(sink);
			return sink.getSize();
		}

		Size calculatePreferredSize(DrawContext* context) {
			CalculateSizeElementAppender sink(*this, 0u, 0u, isFocused(), List::HAVE_PREFERRED_SIZE, context);
			model->getElements(sink);
			return sink.getSize();
		}

	  protected:
		virtual Size getMinimumSizeWithinBorder() {
			if(!(cachedSizes & List::HAVE_MINIMUM_SIZE)) {
				minimumSize = calculateMinimumSize(NULL);
				cachedSizes |= List::HAVE_MINIMUM_SIZE;
			}
			return minimumSize;
		}

		virtual Size getMaximumSizeWithinBorder() {
			if(!(cachedSizes & List::HAVE_MAXIMUM_SIZE)) {
				maximumSize = calculateMaximumSize(NULL);
				cachedSizes |= List::HAVE_MAXIMUM_SIZE;
			}
			return maximumSize;
		}

		virtual Size getPreferredSizeWithinBorder() {
			if(!(cachedSizes & List::HAVE_PREFERRED_SIZE)) {
				preferredSize = calculatePreferredSize(NULL);
				cachedSizes |= List::HAVE_PREFERRED_SIZE;
			}
			return preferredSize;
		}

		virtual unsigned getHeightForWidthWithinBorder(unsigned width) {
			CalculateDimensionElementAppender sink(*this, width, isFocused(), AXS_VERTICAL);
			model->getElements(sink);
			return size.getSize();
		}

		virtual unsigned getWidthForHeightWithinBorder(unsigned height) {
			ModelElementIndex count = mode->getElementCount();
			const unsigned heightPerElement
					= count ? static_cast<unsigned>(static_cast<ModelElementIndex>(height) / count) : height;
			CalculateDimensionElementAppender sink(*this, heightPerElement, isFocused(), AXS_HORIZONTAL);
			model->getElements(sink);
			return size.getSize();
		}

		virtual void drawIntoBorder(const Rectangle& rectangle, DrawContext& context) {
			if(!rectangle.width || !rectangle.height)
				return;
			if(!(cachedSizes & List::HAVE_MINIMUM_SIZE)) {
				minimumSize = calculateMinimumSize(&context);
				cachedSizes |= List::HAVE_MINIMUM_SIZE;
			}
			if(!(cachedSizes & List::HAVE_PREFERRED_SIZE)) {
				preferredSize = calculatePreferredSize(&context);
				cachedSizes |= List::HAVE_PREFERRED_SIZE;
			}
			bool active = isFocused();
			DrawElementAppender sink(*this, active, rectangle, context);
			model->getElements(sink);
			unsigned progress = sink.getProgress();
			if(progress < rectangle.height) {
				ClippingTerminalCanvas& canvas = context.getCanvas();
				canvas.setBackgroundColor((active ? background : inactiveBackground).get(context));
				canvas.fill(Rectangle(area.row + static_cast<int>(progress), area.column,
						area.width, area.height - progress), ' ');
			}
		}

		virtual void notifyListContentGeometryChanged() {
			repack();
		}

	  public:
		List(ListModel<ElementT>& model, ListItemRenderer<ElementT>& renderer)
				: model(&model), modelListener(*this), renderer(&renderer), selectionModel(&defaultSelectionModel),
				selectionListener(*this), minimumSize(0u, 0u), maximumSize(0u, 0u), preferredSize(0u, 0u),
				cachedSizes(0), greedyGeometryUpdates(false) {
			model.addListModelListener(modelListener);
			defaultSelectionModel.addListSelectionListener(selectionListener);
			model.ref();
			renderer.ref();
			defaultSelectionModel.ref();
			util::Unref<ListModel<ElementT> > unrefModel(&model);
			util::Unref<ListItemRenderer<ElementT> > unrefRenderer(&renderer);
			updateForNewModel();
			unrefModel.set();
			unrefRenderer.set();
		}

		List(const List& list) : model(list.model), modelListener(*this), renderer(list.renderer),
				selectionModel(list.selectionModel == &list.defaultSelectionModel
				? &defaultSelectionModel : list.selectionModel), selectionListener(*this),
				minimumSize(list.minimumSize), maximumSize(list.maximumSize), preferredSize(list.preferredSize),
				cachedSizes(list.cachedSizes), greedyGeometryUpdates(list.greedyGeometryUpdates) {
			model->addListModelListener(modelListener);
			selectionModel->addListSelectionListener(selectionListener);
			model->ref();
			renderer->ref();
			selectionModel->ref();
			util::Unref<ListModel<ElementT> > unrefModel(model);
			util::Unref<ListItemRenderer<ElementT> > unrefRenderer(renderer);
			util::Unref<ListSelectionModel> unrefSelectionModel(selectionModel);
			updateForNewModel();
			unrefModel.set();
			unrefRenderer.set();
			unrefSelectionModel.set();
		}

		virtual ~List() {
			model->unref();
			renderer->unref();
			selectionModel->unref();
		}

		inline ListModel<ElementT>& getListModel() const {
			return *model;
		}

		void setListModel(ListModel<ElementT>& model) {
			if(this->model == &model)
				return;
			model.addListModelListener(modelListener);
			this->model->removeListModelListener(modelListener);
			model.ref();
			this->model->unref();
			this->model = &model;
			updateForNewModel();
		}

		inline ListItemRenderer<ElementT>& getListItemRenderer() const {
			return *renderer;
		}

		void setListItemRenderer(ListItemRenderer<ElementT>& renderer) {
			if(this->renderer == &renderer)
				return;
			renderer.ref();
			this->renderer->unref();
			this->renderer = &renderer;
			updateForNewRenderer();
		}

		inline SingleListSelectionModel& getDefaultListSelectionModel() {
			return defaultSelectionModel;
		}

		inline const SingleListSelectionModel& getDefaultListSelectionModel() const {
			return defaultSelectionModel;
		}

		inline ListSelectionModel& getListSelectionModel() const {
			return *selectionModel;
		}

		void setListSelectionModel(ListSelectionModel& selectionModel) {
			if(this->selectionModel == &selectionModel)
				return;
			selectionModel.addListSelectionListener(selectionListener);
			this->selectionModel->removeListSelectionListener(selectionListener);
			selectionModel.ref();
			this->selectionModel->unref();
			this->selectionModel = &selectionModel;
		}

		inline bool isGreedyGeometryUpdates() const {
			return greedyGeometryUpdates;
		}

		inline void setGreedyGeometryUpdates(bool greedyGeometryUpdates) {
			this->greedyGeometryUpdates = greedyGeometryUpdates;
		}

		Rectangle getItemGeometry(ModelElementIndex index) {
			FindRenditionRowElementAppender sink(*this, index);
			model->getElements(sink);
			Rectangle geometry(static_cast<int>(sink.getRenditionRow()), 0,
					sink.getRenditionWidth(), sink.getRenditionHeight());
			Border* border = getBorder();
			if(border) {
				const Insets insets(border->getBorderSize());
				geometry.row += static_cast<int>(insets.top);
				geometry.column = static_cast<int>(insets.left);
			}
			return geometry;
		}

		virtual bool takesFocus() {
			return true;
		}

	};

}}}

#endif /* REDSTRAIN_MOD_DAMNATION_TK_LIST_HPP */
