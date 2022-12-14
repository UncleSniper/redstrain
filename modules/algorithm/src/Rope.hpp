#ifndef REDSTRAIN_MOD_ALGORITHM_ROPE_HPP
#define REDSTRAIN_MOD_ALGORITHM_ROPE_HPP

#include <redstrain/util/Delete.hpp>
#include <redstrain/util/Appender.hpp>
#include <redstrain/util/WithAlign.hpp>
#include <redstrain/error/ListIndexOutOfBoundsError.hpp>
#include <redstrain/util/types.hpp>
#include <redstrain/util/destructors.hpp>

#include "allocators.hpp"

namespace redengine {
namespace algorithm {

	template<
		typename ElementT,
		util::MemorySize SpareBytes = static_cast<util::MemorySize>(128u),
		util::MemorySize CombineBytes = static_cast<util::MemorySize>(64u)
	>
	class DefaultRopeTraits {

	  private:
		static const util::MemorySize _SPARE_SIZE = SpareBytes / static_cast<util::MemorySize>(sizeof(ElementT));
		static const util::MemorySize _COMBINE_LIMIT
				= CombineBytes / static_cast<util::MemorySize>(sizeof(ElementT));

	  public:
		static const util::MemorySize COMBINE_LIMIT
				= _COMBINE_LIMIT ? _COMBINE_LIMIT : static_cast<util::MemorySize>(1u);
		static const util::MemorySize MOVE_LIMIT
				= COMBINE_LIMIT * static_cast<util::MemorySize>(2u) / static_cast<util::MemorySize>(3u);
		static const util::MemorySize SPARE_SIZE
				= _SPARE_SIZE ? _SPARE_SIZE : static_cast<util::MemorySize>(1u);

	};

	template<
		typename ElementT,
		void (*Destructor)(ElementT&) = util::explicitDestructor<ElementT>,
		typename RopeTraitsT = DefaultRopeTraits<ElementT>,
		void* (*Allocator)(util::MemorySize) = standardAlloc,
		void (*Deallocator)(void*) = standardFree
	>
	class Rope {

	  public:
		typedef ElementT Element;
		typedef RopeTraitsT RopeTraits;

	  public:
		static const util::MemorySize COMBINE_LIMIT = RopeTraitsT::COMBINE_LIMIT;
		static const util::MemorySize MOVE_LIMIT = RopeTraitsT::MOVE_LIMIT;
		static const util::MemorySize SPARE_SIZE = RopeTraitsT::SPARE_SIZE;

	  private:
		typedef typename util::WithAlign<util::AlignOf<ElementT>::ALIGNMENT>::Primitive AlignElement;

		template<typename SubjectT, typename LeafT>
		class DeleteLeafNode : public util::Pointer<SubjectT> {

		  public:
			util::MemorySize count;

		  public:
			DeleteLeafNode(SubjectT* object = NULL)
					: util::Pointer<SubjectT>(object), count(static_cast<util::MemorySize>(0u)) {}

			DeleteLeafNode(const DeleteLeafNode& pointer)
					: util::Pointer<SubjectT>(pointer), count(pointer.count) {}

			~DeleteLeafNode() {
				if(!this->object)
					return;
				if(!this->object->height)
					static_cast<LeafT*>(this->object)->destroyElements(count,
							static_cast<util::MemorySize>(0u), static_cast<util::MemorySize>(0u));
				delete this->object;
			}

			using util::Pointer<SubjectT>::operator=;

			inline SubjectT* operator=(const DeleteLeafNode& pointer) {
				count = pointer.count;
				return this->object = pointer.object;
			}

		};

		struct Node {

			util::MemorySize height;

			Node(util::MemorySize height) : height(height) {}
			virtual ~Node() {}

			virtual Node* clone(util::MemorySize) const = 0;
			virtual void destroy(util::MemorySize, util::MemorySize, util::MemorySize) = 0;

		};

		class DeleteAnyNode : public util::Pointer<Node> {

		  public:
			util::MemorySize length;

		  public:
			DeleteAnyNode(Node* node = NULL, util::MemorySize length = static_cast<util::MemorySize>(0u))
					: util::Pointer<Node>(node), length(length) {}

			DeleteAnyNode(const DeleteAnyNode& pointer)
					: util::Pointer<Node>(pointer), length(pointer.length) {}

			~DeleteAnyNode() {
				if(this->object) {
					this->object->destroy(length,
							static_cast<util::MemorySize>(0u), static_cast<util::MemorySize>(0u));
					delete this->object;
				}
			}

			using util::Pointer<Node>::operator=;

			inline Node* operator=(const DeleteAnyNode& pointer) {
				length = pointer.length;
				return this->object = pointer.object;
			}

		};

		struct Leaf : Node {

			util::MemorySize size;
			AlignElement elements;

			Leaf(util::MemorySize size) : Node(static_cast<util::MemorySize>(0u)), size(size) {}

			inline ElementT* getElements() {
				return reinterpret_cast<ElementT*>(&elements);
			}

			void destroyElements(util::MemorySize length, util::MemorySize finalBegin, util::MemorySize finalEnd) {
				ElementT* eptr = reinterpret_cast<ElementT*>(&elements);
				util::MemorySize index;
				for(index = static_cast<util::MemorySize>(0u); index < length; ++index) {
					if(index >= finalBegin && index < finalEnd)
						Destructor(eptr[index]);
					else
						eptr[index].~ElementT();
				}
			}

			virtual Node* clone(util::MemorySize length) const {
				DeleteLeafNode<Leaf, Leaf> newLeaf(new(static_cast<size_t>(size)) Leaf(size));
				const ElementT* src = reinterpret_cast<const ElementT*>(&elements);
				ElementT* dest = reinterpret_cast<ElementT*>(&newLeaf->elements);
				for(; length; ++src, ++dest, --length) {
					new(dest) ElementT(*src);
					++newLeaf.count;
				}
				return newLeaf.set();
			}

			virtual void destroy(util::MemorySize length, util::MemorySize finalBegin, util::MemorySize finalEnd) {
				destroyElements(length, finalBegin, finalEnd);
			}

			static void* operator new(size_t allocSize, size_t nodeSize) {
				return Allocator(static_cast<util::MemorySize>(allocSize - sizeof(AlignElement)
						+ nodeSize * sizeof(ElementT)));
			}

			static void operator delete(void* address) {
				Deallocator(address);
			}

		};

		struct Concat : Node {

			Node *left, *right;
			util::MemorySize weight;

			Concat(Node* left, Node* right, util::MemorySize weight)
					: Node((left->height < right->height ? right->height : left->height)
					+ static_cast<util::MemorySize>(1u)),
					left(left), right(right), weight(weight) {}

			void fixHeight() {
				this->height = (left->height < right->height
						? right->height : left->height) + static_cast<util::MemorySize>(1u);
			}

			virtual Node* clone(util::MemorySize length) const {
				DeleteAnyNode newLeft(left->clone(weight), weight);
				DeleteAnyNode newRight(right->clone(length - weight), length - weight);
				Concat* cat = new Concat(*newLeft, *newRight, weight);
				newLeft.set();
				newRight.set();
				return cat;
			}

			virtual void destroy(util::MemorySize length, util::MemorySize finalBegin, util::MemorySize finalEnd) {
				left->destroy(weight, finalBegin, finalEnd);
				delete left;
				if(finalBegin >= weight)
					right->destroy(length - weight, finalBegin - weight, finalEnd - weight);
				else
					right->destroy(length - weight,
							static_cast<util::MemorySize>(0u), static_cast<util::MemorySize>(0u));
				delete right;
			}

			static void* operator new(size_t size) {
				return Allocator(static_cast<util::MemorySize>(size));
			}

			static void operator delete(void* address) {
				Deallocator(address);
			}

		};

		struct DestroyElements {

			ElementT* elements;
			util::MemorySize count;

			DestroyElements(ElementT* elements = NULL, util::MemorySize count = static_cast<util::MemorySize>(0u))
					: elements(elements), count(count) {}

			DestroyElements(const DestroyElements& destroy)
					: elements(destroy.elements), count(destroy.count) {}

			~DestroyElements() {
				if(!elements)
					return;
				for(; count; ++elements, --count)
					elements->~ElementT();
			}

		};

		class ElementArrayAppender : public util::Appender<ElementT> {

		  private:
			ElementT* array;
			DestroyElements& destroyer;

		  public:
			ElementArrayAppender(ElementT* array, DestroyElements& destroyer)
					: array(array), destroyer(destroyer) {}

			ElementArrayAppender(const ElementArrayAppender& appender)
					: util::Appender<ElementT>(appender), array(appender.array), destroyer(appender.destroyer) {}

			virtual void append(const ElementT& element) {
				new(array++) ElementT(element);
				++destroyer.count;
			}

		};

		class IteratorBase {

		  protected:
			struct PathLink {

				Node* node;
				util::MemorySize size, offset;
				PathLink* parent;

				PathLink(Node* node, util::MemorySize size, util::MemorySize offset, PathLink* parent)
						: node(node), size(size), offset(offset), parent(parent) {}

				static void* operator new(size_t size) {
					return Allocator(static_cast<util::MemorySize>(size));
				}

			};

		  protected:
			static void destroyPath(PathLink* path) {
				while(path) {
					PathLink* next = path->parent;
					delete path;
					path = next;
				}
			}

		  protected:
			struct DeletePath {

				PathLink* path;

				DeletePath(PathLink* path = NULL) : path(path) {}
				DeletePath(const DeletePath& dp) : path(dp.path) {}

				~DeletePath() {
					if(path)
						destroyPath(path);
				}

			};

		  private:
			struct ClearIterator {

				IteratorBase& iterator;
				PathLink** stack;

				ClearIterator(IteratorBase& iterator, PathLink** stack) : iterator(iterator), stack(stack) {}

				~ClearIterator() {
					if(stack) {
						destroyPath(*stack);
						iterator.offset = iterator.rope->cursize;
						iterator.path = NULL;
					}
				}

			};

		  protected:
			static PathLink* clonePath(PathLink* path) {
				DeletePath destroy;
				if(path->parent)
					destroy.path = clonePath(path->parent);
				PathLink* newPath = new PathLink(path->node, path->size, path->offset, destroy.path);
				destroy.path = NULL;
				return newPath;
			}

		  protected:
			const Rope* rope;
			util::MemorySize offset;
			PathLink *path;

		  protected:
			IteratorBase(const Rope* rope, util::MemorySize offset, PathLink* path)
					: rope(rope), offset(offset), path(path) {}

			IteratorBase(const Rope* rope, util::MemorySize offset, PathLink* path,
					util::MemorySize delta, bool moveForward, bool allowExcess)
					: rope(rope), offset(offset), path(path) {
				DeletePath destroyPath(path);
				if(moveForward)
					forward(delta, allowExcess);
				else
					backward(delta, allowExcess);
				destroyPath.path = NULL;
			}

			ElementT* deref() const {
				if(!rope || offset >= rope->cursize)
					throw error::ListIndexOutOfBoundsError(offset);
				return static_cast<Leaf*>(path->node)->getElements() + path->offset;
			}

			void buildStackFromOffset(util::MemorySize newOffset) {
				Node *node = rope->root, *next;
				DeletePath destroy;
				util::MemorySize index = newOffset, size = rope->cursize;
				while(node->height) {
					Concat* cat = static_cast<Concat*>(node);
					util::MemorySize coff, nextSize;
					if(index < cat->weight) {
						next = cat->left;
						coff = static_cast<util::MemorySize>(0u);
						nextSize = cat->weight;
					}
					else {
						next = cat->right;
						index -= cat->weight;
						coff = static_cast<util::MemorySize>(1u);
						nextSize = size - cat->weight;
					}
					destroy.path = path = new PathLink(node, size, coff, path);
					node = next;
					size = nextSize;
				}
				path = new PathLink(node, size, index, path);
				destroy.path = NULL;
				offset = newOffset;
			}

			void forward(util::MemorySize delta, bool allowExcess) {
				util::MemorySize target = offset + delta;
				if(allowExcess && target == rope->cursize) {
					if(path) {
						destroyPath(path);
						path = NULL;
					}
					offset = target;
					return;
				}
				if(target >= rope->cursize)
					throw error::ListIndexOutOfBoundsError(target);
				if(path->offset + delta < path->size) {
					path->offset += delta;
					offset = target;
					return;
				}
				// 'scanOffset' points after the end of the
				// interval we just ascended from
				util::MemorySize scanOffset = offset + (path->size - path->offset);
				util::Delete<PathLink> deleteLeafLink(path);
				PathLink *top = path->parent, *next;
				ClearIterator clearOnError(*this, &top);
				Concat* tcat;
				while(scanOffset < target) {
					tcat = static_cast<Concat*>(top->node);
					if(!top->offset && target < scanOffset + top->size - tcat->weight) {
						// need to descend into right child
						break;
					}
					if(!top->offset)
						scanOffset += top->size - tcat->weight;
					next = top->parent;
					delete top;
					top = next;
				}
				// might need to ascend further even if scanOffset == target,
				// so we can right-descend towards an actual leaf
				while(top->offset) {
					next = top->parent;
					delete top;
					top = next;
				}
				// from here on out, 'scanOffset' points to
				// the start of 'top'
				scanOffset -= static_cast<Concat*>(top->node)->weight;
				// now find the proper leaf
				while(top->node->height) {
					tcat = static_cast<Concat*>(top->node);
					if(target - scanOffset < tcat->weight) {
						// go left
						next = new PathLink(tcat->left, tcat->weight, static_cast<util::MemorySize>(0u), top);
					}
					else {
						// go right
						next = new PathLink(tcat->right, top->size - tcat->weight,
								static_cast<util::MemorySize>(0u), top);
						scanOffset += tcat->weight;
						++top->offset;
					}
					top = next;
				}
				top->offset = target - scanOffset;
				clearOnError.stack = NULL;
				path = top;
				offset = target;
			}

			void backward(util::MemorySize delta, bool allowExcess) {
				if(allowExcess && delta == offset + static_cast<util::MemorySize>(1u)) {
					if(path) {
						destroyPath(path);
						path = NULL;
					}
					offset = rope->cursize;
					return;
				}
				util::MemorySize target = offset - delta;
				if(delta > offset)
					throw error::ListIndexOutOfBoundsError(delta);
				if(path->offset >= delta) {
					path->offset -= delta;
					offset = target;
					return;
				}
				// 'scanOffset' points to the start of
				// the interval we just ascended from
				util::MemorySize scanOffset = offset - path->offset;
				util::Delete<PathLink> deleteLeafLink(path);
				PathLink *top = path->parent, *next;
				ClearIterator clearOnError(*this, &top);
				Concat* tcat;
				while(scanOffset > target) {
					tcat = static_cast<Concat*>(top->node);
					if(top->offset && target < scanOffset - tcat->weight) {
						// need to descend into left child
						break;
					}
					if(top->offset)
						scanOffset -= tcat->weight;
					next = top->parent;
					delete top;
					top = next;
				}
				// might need to ascend further even if scanOffset == target,
				// so we can left-descend towards an actual leaf
				while(!top->offset) {
					next = top->parent;
					delete top;
					top = next;
				}
				// from here on out, 'scanOffset' points to
				// the start of 'top'
				scanOffset -= static_cast<Concat*>(top->node)->weight;
				// now find the proper leaf
				while(top->node->height) {
					tcat = static_cast<Concat*>(top->node);
					if(target - scanOffset < tcat->weight) {
						// go left
						next = new PathLink(tcat->left, tcat->weight, static_cast<util::MemorySize>(0u), top);
					}
					else {
						// go right
						next = new PathLink(tcat->right, top->size - tcat->weight,
								static_cast<util::MemorySize>(0u), top);
						scanOffset += tcat->weight;
						++top->offset;
					}
					top = next;
				}
				top->offset = target - scanOffset;
				clearOnError.stack = NULL;
				path = top;
				offset = target;
			}

		  protected:
			IteratorBase(const Rope* rope, util::MemorySize offset) : rope(rope), offset(offset), path(NULL) {
				if(!rope)
					offset = static_cast<util::MemorySize>(0u);
				else if(offset < rope->cursize)
					buildStackFromOffset(offset);
				else
					offset = rope->cursize;
			}

			IteratorBase(const IteratorBase& iterator)
					: rope(iterator.rope), offset(iterator.offset),
					path(iterator.path ? clonePath(iterator.path) : NULL) {}

		  public:
			~IteratorBase() {
				if(path)
					destroyPath(path);
			}

			inline const Rope* getRope() const {
				return rope;
			}

			inline util::MemorySize index() const {
				return offset;
			}

		};

	  public:
		class ConstIterator : public IteratorBase {

		  protected:
			ConstIterator(const Rope* rope, util::MemorySize offset, typename IteratorBase::PathLink* path)
					: IteratorBase(rope, offset, path) {}
			ConstIterator(const Rope* rope, util::MemorySize offset, typename IteratorBase::PathLink* path,
					util::MemorySize delta, bool moveForward, bool allowExcess)
					: IteratorBase(rope, offset, path, delta, moveForward, allowExcess) {}

		  protected:
			void preincrement() {
				if(this->rope && this->offset < this->rope->cursize)
					this->forward(static_cast<util::MemorySize>(1u), true);
			}

			typename IteratorBase::PathLink* postincrement() {
				typename IteratorBase::DeletePath oldPath(this->path ? IteratorBase::clonePath(this->path) : NULL);
				this->forward(static_cast<util::MemorySize>(1u), true);
				typename IteratorBase::PathLink* returnPath = oldPath.path;
				oldPath.path = NULL;
				return returnPath;
			}

			void predecrement() {
				if(this->rope && this->rope->cursize) {
					if(this->offset >= this->rope->cursize)
						this->buildStackFromOffset(this->rope->cursize - static_cast<util::MemorySize>(1u));
					else
						this->backward(static_cast<util::MemorySize>(1u), false);
				}
			}

			typename IteratorBase::PathLink* postdecrement() {
				typename IteratorBase::DeletePath oldPath(this->path ? IteratorBase::clonePath(this->path) : NULL);
				if(this->offset >= this->rope->cursize)
					this->buildStackFromOffset(this->rope->cursize - static_cast<util::MemorySize>(1u));
				else
					this->backward(static_cast<util::MemorySize>(1u), false);
				typename IteratorBase::PathLink* returnPath = oldPath.path;
				oldPath.path = NULL;
				return returnPath;
			}

			void skipForward(util::MemorySize delta) {
				if(delta && this->rope && this->offset < this->rope->cursize)
					this->forward(delta, true);
			}

			void skipBackward(util::MemorySize delta) {
				if(this->rope && this->rope->cursize) {
					if(this->offset >= this->rope->cursize) {
						if(delta > this->rope->cursize)
							throw error::ListIndexOutOfBoundsError(delta);
						this->buildStackFromOffset(this->rope->cursize - delta);
					}
					else
						this->backward(delta, false);
				}
			}

		  public:
			ConstIterator() : IteratorBase(NULL, static_cast<util::MemorySize>(0u)) {}
			ConstIterator(const Rope* rope, util::MemorySize offset) : IteratorBase(rope, offset) {}
			ConstIterator(const ConstIterator& iterator) : IteratorBase(iterator) {}

			const ElementT& operator*() const {
				return *this->deref();
			}

			ConstIterator& operator++() {
				preincrement();
				return *this;
			}

			ConstIterator operator++(int) {
				if(!this->rope || this->offset >= this->rope->cursize)
					return *this;
				typename IteratorBase::PathLink* returnPath = postincrement();
				return ConstIterator(this->rope, this->offset - static_cast<util::MemorySize>(1u), returnPath);
			}

			ConstIterator& operator--() {
				predecrement();
				return *this;
			}

			ConstIterator operator--(int) {
				if(!this->rope || !this->rope->cursize)
					return *this;
				typename IteratorBase::PathLink* returnPath = postdecrement();
				return ConstIterator(this->rope, this->offset + static_cast<util::MemorySize>(1u), returnPath);
			}

			ConstIterator operator+(util::MemorySize delta) const {
				if(!this->rope || this->offset >= this->rope->cursize)
					return *this;
				return ConstIterator(this->rope, this->offset,
						IteratorBase::clonePath(this->path), delta, true, true);
			}

			ConstIterator operator-(util::MemorySize delta) const {
				if(!this->rope || !this->rope->cursize)
					return *this;
				return ConstIterator(this->rope, this->offset,
						IteratorBase::clonePath(this->path), delta, false, false);
			}

			ConstIterator& operator+=(util::MemorySize delta) {
				skipForward(delta);
				return *this;
			}

			ConstIterator& operator-=(util::MemorySize delta) {
				if(delta)
					skipBackward(delta);
				return *this;
			}

			bool operator==(const ConstIterator& iterator) const {
				return this->rope == iterator.rope && this->offset == iterator.offset;
			}

			bool operator!=(const ConstIterator& iterator) const {
				return this->rope != iterator.rope || this->offset != iterator.offset;
			}

			bool operator<(const ConstIterator& iterator) const {
				return this->offset < iterator.offset;
			}

			bool operator<=(const ConstIterator& iterator) const {
				return this->offset <= iterator.offset;
			}

			bool operator>(const ConstIterator& iterator) const {
				return this->offset > iterator.offset;
			}

			bool operator>=(const ConstIterator& iterator) const {
				return this->offset >= iterator.offset;
			}

			ptrdiff_t operator-(const ConstIterator& iterator) const {
				return this->offset >= iterator.offset
					? static_cast<ptrdiff_t>(this->offset - iterator.offset)
					: -static_cast<ptrdiff_t>(iterator.offset - this->offset);
			}

			bool valid() const {
				return this->rope && this->offset < this->rope->cursize;
			}

		};

		class Iterator : public ConstIterator {

		  private:
			Iterator(const Rope* rope, util::MemorySize offset, typename IteratorBase::PathLink* path)
					: ConstIterator(rope, offset, path) {}
			Iterator(const Rope* rope, util::MemorySize offset, typename IteratorBase::PathLink* path,
					util::MemorySize delta, bool moveForward, bool allowExcess)
					: ConstIterator(rope, offset, path, delta, moveForward, allowExcess) {}

		  public:
			Iterator() {}
			Iterator(const Rope* rope, util::MemorySize offset) : ConstIterator(rope, offset) {}
			Iterator(const Iterator& iterator) : ConstIterator(iterator) {}

			ElementT& operator*() const {
				return *this->deref();
			}

			Iterator& operator++() {
				this->preincrement();
				return *this;
			}

			Iterator operator++(int) {
				if(!this->rope || this->offset >= this->rope->cursize)
					return *this;
				typename IteratorBase::PathLink* returnPath = this->postincrement();
				return Iterator(this->rope, this->offset - static_cast<util::MemorySize>(1u), returnPath);
			}

			Iterator& operator--() {
				this->predecrement();
				return *this;
			}

			Iterator operator--(int) {
				if(!this->rope || !this->rope->cursize)
					return *this;
				typename IteratorBase::PathLink* returnPath = this->postdecrement();
				return Iterator(this->rope, this->offset + static_cast<util::MemorySize>(1u), returnPath);
			}

			Iterator operator+(util::MemorySize delta) const {
				if(!this->rope || this->offset >= this->rope->cursize)
					return *this;
				return Iterator(this->rope, this->offset + delta,
						IteratorBase::clonePath(this->path), delta, true, true);
			}

			Iterator operator-(util::MemorySize delta) const {
				if(!this->rope || !this->rope->cursize)
					return *this;
				return Iterator(this->rope, this->offset - delta,
						IteratorBase::clonePath(this->path), delta, false, false);
			}

			Iterator& operator+=(util::MemorySize delta) {
				this->skipForward(delta);
				return *this;
			}

			Iterator& operator-=(util::MemorySize delta) {
				if(delta)
					this->skipBackward(delta);
				return *this;
			}

			using ConstIterator::operator-;

		};

		class ConstReverseIterator : public IteratorBase {

		  protected:
			ConstReverseIterator(const Rope* rope, util::MemorySize offset, typename IteratorBase::PathLink* path)
					: IteratorBase(rope, offset, path) {}
			ConstReverseIterator(const Rope* rope, util::MemorySize offset, typename IteratorBase::PathLink* path,
					util::MemorySize delta, bool moveForward, bool allowExcess)
					: IteratorBase(rope, offset, path, delta, moveForward, allowExcess) {}

		  protected:
			void preincrement() {
				if(this->rope && this->offset < this->rope->cursize)
					this->backward(static_cast<util::MemorySize>(1u), true);
			}

			typename IteratorBase::PathLink* postincrement() {
				typename IteratorBase::DeletePath oldPath(this->path ? IteratorBase::clonePath(this->path) : NULL);
				this->backward(static_cast<util::MemorySize>(1u), true);
				typename IteratorBase::PathLink* returnPath = oldPath.path;
				oldPath.path = NULL;
				return returnPath;
			}

			void predecrement() {
				if(this->rope && this->rope->cursize) {
					if(this->offset >= this->rope->cursize)
						this->buildStackFromOffset(static_cast<util::MemorySize>(0u));
					else
						this->forward(static_cast<util::MemorySize>(1u), false);
				}
			}

			typename IteratorBase::PathLink* postdecrement() {
				typename IteratorBase::DeletePath oldPath(this->path ? IteratorBase::clonePath(this->path) : NULL);
				if(this->offset >= this->rope->cursize)
					this->buildStackFromOffset(static_cast<util::MemorySize>(0u));
				else
					this->forward(static_cast<util::MemorySize>(1u), false);
				typename IteratorBase::PathLink* returnPath = oldPath.path;
				oldPath.path = NULL;
				return returnPath;
			}

			void skipForward(util::MemorySize delta) {
				if(delta && this->rope && this->offset < this->rope->cursize)
					this->backward(delta, true);
			}

			void skipBackward(util::MemorySize delta) {
				if(this->rope && this->rope->cursize) {
					if(this->offset >= this->rope->cursize) {
						if(delta > this->rope->cursize)
							throw error::ListIndexOutOfBoundsError(delta);
						this->buildStackFromOffset(delta - static_cast<util::MemorySize>(1u));
					}
					else
						this->forward(delta, false);
				}
			}

		  public:
			ConstReverseIterator() : IteratorBase(NULL, static_cast<util::MemorySize>(0u)) {}
			ConstReverseIterator(const Rope* rope, util::MemorySize offset) : IteratorBase(rope, offset) {}
			ConstReverseIterator(const ConstReverseIterator& iterator) : IteratorBase(iterator) {}

			const ElementT& operator*() const {
				return *this->deref();
			}

			ConstReverseIterator& operator++() {
				preincrement();
				return *this;
			}

			ConstReverseIterator operator++(int) {
				if(!this->rope || this->offset >= this->rope->cursize)
					return *this;
				util::MemorySize oldOffset = this->offset;
				typename IteratorBase::PathLink* returnPath = postincrement();
				return ConstReverseIterator(this->rope, oldOffset, returnPath);
			}

			ConstReverseIterator& operator--() {
				predecrement();
				return *this;
			}

			ConstReverseIterator operator--(int) {
				if(!this->rope || !this->rope->cursize)
					return *this;
				typename IteratorBase::PathLink* returnPath = postdecrement();
				return ConstReverseIterator(this->rope, this->offset + static_cast<util::MemorySize>(1u), returnPath);
			}

			ConstReverseIterator operator+(util::MemorySize delta) const {
				if(!this->rope || this->offset >= this->rope->cursize)
					return *this;
				return ConstReverseIterator(this->rope, this->offset,
						IteratorBase::clonePath(this->path), delta, false, true);
			}

			ConstReverseIterator operator-(util::MemorySize delta) const {
				if(!this->rope || !this->rope->cursize)
					return *this;
				return ConstReverseIterator(this->rope, this->offset,
						IteratorBase::clonePath(this->path), delta, true, false);
			}

			ConstReverseIterator& operator+=(util::MemorySize delta) {
				skipForward(delta);
				return *this;
			}

			ConstReverseIterator& operator-=(util::MemorySize delta) {
				if(delta)
					skipBackward(delta);
				return *this;
			}

			bool operator==(const ConstReverseIterator& iterator) const {
				return this->rope == iterator.rope && this->offset == iterator.offset;
			}

			bool operator!=(const ConstReverseIterator& iterator) const {
				return this->rope != iterator.rope || this->offset != iterator.offset;
			}

			bool operator<(const ConstReverseIterator& iterator) const {
				return this->offset < iterator.offset;
			}

			bool operator<=(const ConstReverseIterator& iterator) const {
				return this->offset <= iterator.offset;
			}

			bool operator>(const ConstReverseIterator& iterator) const {
				return this->offset > iterator.offset;
			}

			bool operator>=(const ConstReverseIterator& iterator) const {
				return this->offset >= iterator.offset;
			}

			ptrdiff_t operator-(const ConstReverseIterator& iterator) const {
				if(!this->rope || !iterator.rope)
					return static_cast<ptrdiff_t>(0);
				if(this->offset >= this->rope->cursize) {
					if(iterator.offset >= this->rope->cursize)
						return static_cast<ptrdiff_t>(0);
					// | 1 | 0 | - |
					// +---+---+---+
					//           ^--- me
					//   ^----------- them
					return static_cast<ptrdiff_t>(iterator.offset + static_cast<util::MemorySize>(1u));
				}
				else {
					if(iterator.offset >= this->rope->cursize) {
						// | 1 | 0 | - |
						// +---+---+---+
						//           ^--- them
						//   ^----------- me
						return -static_cast<ptrdiff_t>(this->offset + static_cast<util::MemorySize>(1u));
					}
					return this->offset >= iterator.offset
						? -static_cast<ptrdiff_t>(this->offset - iterator.offset)
						: static_cast<ptrdiff_t>(iterator.offset - this->offset);
				}
			}

			bool valid() const {
				return this->rope && this->offset < this->rope->cursize;
			}

		};

		class ReverseIterator : public ConstReverseIterator {

		  private:
			ReverseIterator(const Rope* rope, util::MemorySize offset, typename IteratorBase::PathLink* path)
					: ConstReverseIterator(rope, offset, path) {}
			ReverseIterator(const Rope* rope, util::MemorySize offset, typename IteratorBase::PathLink* path,
					util::MemorySize delta, bool moveForward, bool allowExcess)
					: ConstReverseIterator(rope, offset, path, delta, moveForward, allowExcess) {}

		  public:
			ReverseIterator() {}
			ReverseIterator(const Rope* rope, util::MemorySize offset) : ConstReverseIterator(rope, offset) {}
			ReverseIterator(const ReverseIterator& iterator) : ConstReverseIterator(iterator) {}

			ElementT& operator*() const {
				return *this->deref();
			}

			ReverseIterator& operator++() {
				this->preincrement();
				return *this;
			}

			ReverseIterator operator++(int) {
				if(!this->rope || this->offset >= this->rope->cursize)
					return *this;
				util::MemorySize oldOffset = this->offset;
				typename IteratorBase::PathLink* returnPath = this->postincrement();
				return ReverseIterator(this->rope, oldOffset, returnPath);
			}

			ReverseIterator& operator--() {
				this->predecrement();
				return *this;
			}

			ReverseIterator operator--(int) {
				if(!this->rope || !this->rope->cursize)
					return *this;
				typename IteratorBase::PathLink* returnPath = this->postdecrement();
				return ReverseIterator(this->rope, this->offset + static_cast<util::MemorySize>(1u), returnPath);
			}

			ReverseIterator operator+(util::MemorySize delta) const {
				if(!this->rope || this->offset >= this->rope->cursize)
					return *this;
				return ReverseIterator(this->rope, this->offset,
						IteratorBase::clonePath(this->path), delta, false, true);
			}

			ReverseIterator operator-(util::MemorySize delta) const {
				if(!this->rope || !this->rope->cursize)
					return *this;
				return ReverseIterator(this->rope, this->offset,
						IteratorBase::clonePath(this->path), delta, true, false);
			}

			ReverseIterator& operator+=(util::MemorySize delta) {
				this->skipForward(delta);
				return *this;
			}

			ReverseIterator& operator-=(util::MemorySize delta) {
				if(delta)
					this->skipBackward(delta);
				return *this;
			}

			using ConstReverseIterator::operator-;

		};

	  private:
		static Leaf* newSingletonLeaf(const ElementT& value) {
			size_t size = static_cast<size_t>(1u) + static_cast<size_t>(SPARE_SIZE);
			util::Delete<Leaf> leaf(new(size) Leaf(static_cast<util::MemorySize>(size)));
			new(leaf->getElements()) ElementT(value);
			return leaf.set();
		}

		template<typename IteratorT>
		static Leaf* newIteratedLeaf(IteratorT begin, IteratorT end, util::MemorySize count) {
			DeleteLeafNode<Leaf, Leaf> leaf(new(static_cast<size_t>(count)) Leaf(count));
			ElementT* dest = leaf->getElements();
			for(; begin != end; ++begin) {
				new(dest++) ElementT(*begin);
				++leaf.count;
			}
			return leaf.set();
		}

		/* Takes a tree, 'left', which is a concatenation node
		 * whose right child, left->right, is itsself a
		 * concatenation node whose right child, left->right->right,
		 * has been grown, yielding the modified subtree 'node',
		 * such that its height might have increased by at most
		 * one. Modifies 'left' to reinstate any type invariants
		 * that might be violated due to this change and returns
		 * the root of the modified tree, whose height will have
		 * increased by at most one. Note that the fixup must
		 * have a chance of applying, in that left->right->height
		 * must be >= left->left->height (otherwise, the
		 * type invariant would intrinsically still hold).
		 */
		static Concat* growRightFixup(Concat* left, Node* node) {
			Concat* scat = static_cast<Concat*>(left->right);
			Node* tmp;
			if(node->height < left->height) {
				/* We would have to take great care not to violate
				 * the type invariant if the height of scat->right
				 * increased due to the concatenation to the point
				 * where it equaled scat->height -- since this is not
				 * the case, the tree can remain.
				 */
				scat->right = node;
				return left;
			}
			/* Since 'node' is too high, we need to restructure the
			 * tree using the classic node rotations. The 'left'
			 * argument shall be referred to as 'cat'.
			 * -> Restructure variant #1:
			 *     +------------+                                 +---------------+
			 *     | cat:Concat |                                 | result:Concat |
			 *     +------------+                                 +---------------+
			 *       /       \                                        /       \
			 * +------+   +-------------+         \ \     +-------------+   +-------------+
			 * | cl:? |   | scat:Concat |     -----\ \    | tcat:Concat |   | node:Concat |
			 * +------+   +-------------+     -----/ /    +-------------+   +-------------+
			 *               /       \            / /        /       \           /   \
			 *         +-------+   +-------+            +------+   +-------+    ?     ?
			 *         | scl:? |   | scr:? |            | cl:? |   | scl:? |
			 *         +-------+   +-------+            +------+   +-------+
			 * Essentially, this is:
			 *   tcat = new Concat(cat->left, scat->left, cat->weight);
			 *   return new Concat(tcat, node, cat->weight + scat->weight);
			 * But we'll reuse the existing nodes we would otherwise
			 * throw away: The former 'cat' will become 'tcat' and the
			 * former 'scat' will become the return value.
			 *
			 * Note that this assumes that cat->left ('cl') can be
			 * lowered, i.e. has a height of cat->height - 2; but
			 * also note that if this is not so (i.e. the only other
			 * possibility, cat->left->height = cat->height - 1, is the
			 * case), then we might consider keeping the tree structure
			 * and using the fact that scat->height = cat->height - 1 to
			 * our advantage by simply increasing the height of 'cat'
			 * and 'scat':
			 *   - cat->height' = cat->height + 1
			 *   - cat->left->height = cat->height - 1 = cat->height' - 2
			 *   - scat->height' = scat->height + 1 = cat->height - 1 + 1 = cat->height = cat->height' - 1
			 *   - node->height = scat->height = scat->height' - 1
			 * The only circumstance that might violate the invariants
			 * in doing this is the height of scat->left ('scl'): It
			 * might be scat->height - 1 or scat->height - 2, meaning
			 * scat->height' - 2 or scat->height' - 3; clearly, only the
			 * former is viable.
			 */
			if(left->left->height < left->height - static_cast<util::MemorySize>(1u)) {
				/* restructure variant #1 */
				scat->right = node;
				tmp = scat->left;
				scat->left = left;
				left->right = tmp;
				scat->weight += left->weight;
				left->fixHeight();
				scat->fixHeight();
				return scat;
			}
			if(scat->left->height == left->right->height - static_cast<util::MemorySize>(1u)) {
				/* keep structure, increase heights */
				scat->right = node;
				++scat->height;
				++left->height;
				return left;
			}
			/* If 'scl' is not viable for increasing the heights, we
			 * know that:
			 *   - cl->height = cat->height - 1
			 *   - scat->height = cat->height - 1
			 *   - scl->height = scat->height - 2 = cat->height - 3
			 *   - node->height = scat->height = cat->height - 1
			 * The layout is therefore:
			 *   [ 0]       cat
			 *   [-1]  cl         scat   node
			 *   [-2]                    / \
			 *   [-3]          scl     nl   nr
			 * Where 'nl' and 'nr' might be on level -2 or -3,
			 * but at least one of them must be on level -2.
			 * If nl->height = nr->height, we can concatenate
			 * 'cl', 'scl', 'nl' and 'nr' directly.
			 * -> Restructure variant #2a:
			 *   [ 1]       (t)
			 *             /   \
			 *   [ 0]     /    (c)
			 *           /    /   \
			 *   [-1]  cl   (b)    \
			 *             /   \    \
			 *   [-2]     |     nl   nr
			 *            |
			 *   [-3]    scl
			 * If nl->height < nr->height, the same logic applies.
			 * -> Restructure variant #2b:
			 *   [ 0]      (t)
			 *            /   \
			 *   [-1]   cl    (c)
			 *               /   \
			 *   [-2]      (b)    nr
			 *            /   \
			 *   [-3]  scl     nl
			 * We observe that the structures of variants #2a
			 * and #2b are the same; the heights of the introduced
			 * concatenation nodes are merely one greater in #2a.
			 * Once again, we will reuse the existing concatenation
			 * nodes: The former 'cat' will become 'tcat', the
			 * former 'node' will become 'ccat' and the former
			 * 'scat' will become 'bcat'.
			 */
			Concat* ncat = static_cast<Concat*>(node);
			if(ncat->left->height <= ncat->right->height) {
				/* restructure variant #2 */
				// cat -> t
				// scat -> b
				// node -> c
				scat->right = ncat->left;
				ncat->left = left->right;
				ncat->weight += scat->weight;
				left->right = node;
				static_cast<Concat*>(ncat->left)->fixHeight();
				ncat->fixHeight();
				left->fixHeight();
				return left;
			}
			/* If nl->height > nr->height, we cannot restructure
			 * solely in terms of 'cl', 'scl', 'nl' and 'nr'
			 * to restore the type invariants -- we'll have to
			 * introspect one level deeper still. (Don't worry,
			 * it's the last level...) Since nl->height > nr->height,
			 * surely 'nl' must be a concatenation node. The layout
			 * is therefore:
			 *   [ 0]      cat
			 *   [-1]  cl         scat     node
			 *   [-2]                   nl
			 *   [-3]         scl      /  \     nr
			 *   [-4]               nll    nlr
			 * Where 'nll' and 'nlr' might be on level -3 or -4,
			 * but at least one of them must be on level -3.
			 * Given this, we can concatenate 'cl', 'scl', 'nll',
			 * 'nlr' and 'nr'.
			 * -> Restructure variant #3:
			 *   [ 0]     (t)
			 *           /   \
			 *   [-1]  cl     (c)
			 *               /   \
			 *   [-2]    (bl)     (br)
			 *           |   \    |   \
			 *   [-3]   scl  nll nlr   nr
			 *   [-4]        nll nlr
			 * As we can see, this works regardless of the heights
			 * of 'nll' and 'nlr'. The former 'cat' will become
			 * 'tcat', the former 'scat' will become 'blcat', the
			 * former 'node' will become 'brcat' and the former
			 * 'nl' will become 'ccat'.
			 */
			/* restructure variant #3 */
			// cat -> t
			// scat -> bl
			// node -> br
			// nl -> c
			Concat* nlcat = static_cast<Concat*>(ncat->left);
			ncat->weight -= nlcat->weight; /* br.weight */
			nlcat->weight += scat->weight; /* c.weight */
			scat->right = nlcat->left; /* bl.right */
			nlcat->left = left->right; /* c.left */
			tmp = nlcat->right;
			nlcat->right = node; /* c.right */
			left->right = nlcat; /* t.right */
			ncat->left = tmp; /* br.left */
			scat->fixHeight(); /* bl.height */
			ncat->fixHeight(); /* br.height */
			static_cast<Concat*>(left->right)->fixHeight(); /* c.height */
			return left;
		}

		/* Takes a tree, 'right', which is a concatenation node
		 * whose left child, right->left, is itsself a
		 * concatenation node whose left child, right->left->left,
		 * has been grown, yielding the modified subtree 'node',
		 * such that its height might have increased by at most
		 * one. Modifies 'right' to reinstate any type invariants
		 * that might be violated due to this change and returns
		 * the root of the modified tree, whose height will have
		 * increased by at most one. Note that the fixup must
		 * have a chance of applying, in that right->left->height
		 * must be >= right->right->height (otherwise, the
		 * type invariant would intrinsically still hold).
		 */
		static Concat* growLeftFixup(Node* node, util::MemorySize nodeSize, Concat* right) {
			Concat* scat = static_cast<Concat*>(right->left);
			Node* tmp;
			if(node->height < right->left->height) {
				/* We would have to take great care not to violate
				 * the type invariant if the height of scat->left
				 * increased due to the concatenation to the point
				 * where it equaled scat->height -- since this is
				 * not the case, the tree can remain.
				 */
				scat->left = node;
				return right;
			}
			/* Since 'node' is too high, we need to restructure the
			 * tree using the classic node rotations. The 'right'
			 * argument shall be referred to as 'cat'.
			 * -> Restructure variant #1:
			 *             +------------+                       +---------------+
			 *             | cat:Concat |                       | result:Concat |
			 *             +------------+                       +---------------+
			 *                /       \                             /       \
			 *    +-------------+   +------+      \ \   +-------------+   +-------------+
			 *    | scat:Concat |   | cr:? |  -----\ \  | node:Concat |   | tcat:Concat |
			 *    +-------------+   +------+  -----/ /  +-------------+   +-------------+
			 *       /        \                   / /        /   \           /       \
			 * +-------+    +-------+                       ?     ?    +-------+   +------+
			 * | scl:? |    | scr:? |                                  | scr:? |   | cr:? |
			 * +-------+    +-------+                                  +-------+   +------+
			 * Essentially, this is:
			 *   tcat = new Concat(scat->right, cat->right, cat->weight - scat->weight);
			 *   return new Concat(node, tcat, nodeSize);
			 * But we'll reuse the existing nodes we would otherwise
			 * throw away: The former 'cat' will become 'tcat' and the
			 * former 'scat' will become the return value.
			 *
			 * Note that this assumes that cat->right ('cr') can be
			 * lowered, i.e. has a height of cat->height - 2; but
			 * also note that if this is not so (i.e. the only other
			 * possibility, cat->right->height = cat->height - 1, is the
			 * case), then we might consider keeping the tree structure
			 * and using the fact that scat->height = cat->height - 1 to
			 * our advantage by simply increasing the height of 'cat'
			 * and 'scat':
			 *   - cat->height' = cat->height + 1
			 *   - cat->left->height = cat->height - 1 = cat->height' - 2
			 *   - scat->height' = scat->height + 1 = cat->height - 1 + 1 = cat->height = cat->height' - 1
			 *   - node->height = scat->height = scat->height' - 1
			 * The only circumstance that might violate the invariants
			 * in doing this is the height of scat->right ('scr'): It
			 * might be scat->height - 1 or scat->height - 2, meaning
			 * scat->height' - 2 or scat->height' - 3; clearly, only the
			 * former is viable.
			 */
			if(right->right->height < right->height - static_cast<util::MemorySize>(1u)) {
				/* restructure variant #1 */
				right->weight -= scat->weight;
				scat->left = node;
				tmp = scat->right;
				scat->right = right;
				right->left = tmp;
				scat->weight = nodeSize;
				right->fixHeight();
				scat->fixHeight();
				return scat;
			}
			if(scat->right->height == right->left->height - static_cast<util::MemorySize>(1u)) {
				/* keep structure, increase heights */
				scat->left = node;
				++scat->height;
				++right->height;
				right->weight += nodeSize - scat->weight;
				scat->weight = nodeSize;
				return right;
			}
			/* If 'scr' is not viable for increasing the heights, we
			 * know that:
			 *   - cr->height = cat->height - 1
			 *   - scat->height = cat->height - 1
			 *   - scr->height = scat->height - 2 = cat->height - 3
			 *   - node->height = scat->height = cat->height - 1
			 * The layout is therefore:
			 *   [ 0]                   cat
			 *   [-1]    node    scat        cr
			 *   [-2]    / \
			 *   [-3]  nl   nr       scr
			 * Where 'nl' and 'nr' might be on level -2 or -3,
			 * but at least one of them must be on level -2.
			 * If nl->height = nr->height, we can concatenate
			 * 'nl', 'nr', 'scr' and 'cr' directly.
			 * -> Restructure variant #2a:
			 *   [ 1]          (t)
			 *                /   \
			 *   [ 0]       (c)    \
			 *             /   \    \
			 *   [-1]     /    (b)   cr
			 *           /    /   \
			 *   [-2]  nl   nr    |
			 *                    |
			 *   [-3]            scr
			 * If nl->height > nr->height, the same logic applies.
			 * -> Restructure variant #2b:
			 *   [ 0]        (t)
			 *              /   \
			 *   [-1]     (c)    cr
			 *           /   \
			 *   [-2]  nl    (b)
			 *              /   \
			 *   [-3]     nr     scr
			 * We observe that the structures of variants #2a
			 * and #2b are the same; the heights of the introduced
			 * concatenation nodes are merely one greater in #2a.
			 * Once again, we will reuse the existing concatenation
			 * nodes: The former 'cat' will become 'tcat', the
			 * former 'node' will become 'ccat' and the former
			 * 'scat' will become 'bcat'.
			 */
			Concat* ncat = static_cast<Concat*>(node);
			if(ncat->left->height >= ncat->right->height) {
				/* restructure variant #2 */
				// cat -> t
				// scat -> b
				// node -> c
				scat->left = ncat->right;
				ncat->right = right->left;
				right->left = node;
				Concat* nrcat = static_cast<Concat*>(ncat->right);
				right->weight += nodeSize - nrcat->weight;
				nrcat->weight = nodeSize - ncat->weight;
				nrcat->fixHeight();
				ncat->fixHeight();
				right->fixHeight();
				return right;
			}
			/* If nl->height < nr->height, we cannot restructure
			 * solely in terms of 'nl', 'nr', 'scr' and 'cr'
			 * to restore the type invariants -- we'll have to
			 * introspect one level deeper still. (Don't worry,
			 * it's the last level...) Since nl->height < nr->height,
			 * surely 'nr' must be a concatenation node. The layout
			 * is therefore:
			 *   [ 0]                      cat
			 *   [-1]    node      scat         cr
			 *   [-2]          nr
			 *   [-3]  nl     /  \     scr
			 *   [-4]      nrl    nrr
			 * Where 'nll' and 'nlr' might be on level -3 or -4,
			 * but at least one of them must be on level -3.
			 * Given this, we can concatenate 'nl', 'nrl', 'nrr',
			 * 'scr' and 'cr'.
			 * -> Restructure variant #3:
			 *   [ 0]             (t)
			 *                   /   \
			 *   [-1]         (c)     cr
			 *               /   \
			 *   [-2]    (bl)     (br)
			 *          /   |     |   \
			 *   [-3] nl   nrl   nrr   scr
			 *   [-4]      nrl   nrr
			 * As we can see, this works regardless of the heights
			 * of 'nll' and 'nlr'. The former 'cat' will become
			 * 'tcat', the former 'scat' will become 'brcat', the
			 * former 'node' will become 'blcat' and the former
			 * 'nr' will become 'ccat'.
			 */
			/* restructure variant #3 */
			// cat -> t
			// scat -> br
			// node -> bl
			// nr -> c
			right->weight += nodeSize - scat->weight; /* t.weight */
			Concat* nrcat = static_cast<Concat*>(ncat->right);
			scat->weight = nodeSize - ncat->weight - nrcat->weight; /* br.weight */
			nrcat->weight += ncat->weight; /* c.weight */
			scat->left = nrcat->right; /* br.left */
			nrcat->right = right->left; /* c.right */
			tmp = nrcat->left;
			nrcat->left = node; /* c.left */
			right->left = ncat->right; /* t.left */
			ncat->right = tmp; /* bl.right */
			ncat->fixHeight(); /* bl.height */
			scat->fixHeight(); /* br.height */
			nrcat->fixHeight(); /* c.height */
			return right;
		}

		/*
		 * Takes two trees of any height and returns their
		 * concatenation, whose height is either N or N + 1,
		 * where N = max(left->height, right->height). Both
		 * input trees are incorporated into the result and
		 * may be modified in the process. If the operation
		 * throws an exception (usually out of memory for
		 * node creation), the input trees will remain
		 * unmodified.
		 */
		static Node* concatNodes(Node* left, util::MemorySize leftSize,
				Node* right, util::MemorySize rightSize, Concat* emergencyCat) {
			Node* node;
			Concat *cat, *scat;
			if(left->height > right->height + static_cast<util::MemorySize>(1u)) {
				/* 'Concatenation vs. Leaf' case:
				 *   +-------------+    +---------+
				 *   | left:Concat |    | right:? |
				 *   +-------------+    +---------+
				 *        /   \
				 *       ?     ?
				 */
				cat = static_cast<Concat*>(left);
				if(cat->right->height < cat->left->height) {
					/* We know that:
					 *   (1) cat->height > right->height + 1
					 *       <=> right->height < cat->height - 1,
					 *       as per outer 'if'
					 *   (2) cat->right->height < cat->left->height,
					 *       as per inner 'if'
					 *   (3) node->height <= max(cat->right->height, right->height) + 1,
					 *       as per function invariant
					 *   (4) node->height >= max(cat->right->height, right->height),
					 *       as per function invariant
					 *   (5) max(cat->left->height, cat->right->height) < cat->height,
					 *       as per type invariant
					 *   (6) max(cat->left->height, cat->right->height) + 1 = cat->height
					 *       as per type invariant
					 *   (7) abs(cat->left->height - cat->right->height) <= 1,
					 *       as per type invariant
					 * We can thus conclude that:
					 *   (2)+(5) => cat->right->height < cat->left->height < cat->height
					 *      +(6) => cat->left->height = cat->height - 1
					 *      +(7) => cat->right->height = cat->height - 2
					 *      +(1) => cat->right->height = cat->height - 2 > right->height
					 *      +(3) => node->height <= max(cat->height - 2, cat->height - 3) + 1
					 *          <=> node->height <= cat->height - 2 + 1
					 *          <=> node->height <= cat->height - 1
					 *      +(4) => max(cat->right->height, right->height) <= node->height <= cat->height - 1
					 *          <=> max(cat->height - 2, cat->height - 3) <= node->height <= cat->height - 1
					 *          <=> cat->height - 2 <= node->height <= cat->height - 1
					 * Since
					 *   - cat->left->height = cat->height - 1
					 *   - cat->height - 2 <= node->height <= cat->height - 1
					 * we can use 'node' as the left child of 'cat' without
					 * violating the type invariants. As the height of 'cat'
					 * does not change, this satisfies the function invariant,
					 * as well.
					 */
					node = concatNodes(cat->right, leftSize - cat->weight, right, rightSize, emergencyCat);
					cat->right = node;
					return left;
				}
				/* Observe that:
				 *   (~2) cat->right->height >= cat->left->height,
				 *        as per the preceding 'if'
				 * We can thus conclude that:
				 *   (~2)+(5) => cat->left->height <= cat->right->height < cat->height
				 *       +(6) => cat->right->height = cat->height - 1
				 *       +(1) => right->height < cat->right->height
				 * Since cat->right is higher than 'right', it must be a
				 * concatenation node and we can concatente its own right
				 * child with 'right'.
				 */
				scat = static_cast<Concat*>(cat->right);
				node = concatNodes(scat->right, leftSize - cat->weight - scat->weight,
						right, rightSize, emergencyCat);
				return growRightFixup(cat, node);
			}
			else if(right->height > left->height + static_cast<util::MemorySize>(1u)) {
				/* 'Leaf vs. Concatenation' case:
				 *   +--------+    +--------------+
				 *   | left:? |    | right:Concat |
				 *   +--------+    +--------------+
				 *                       /  \
				 *                      ?    ?
				 * This is the 'Concatenation vs. Leaf' case,
				 * only mirrored. Thus, the same reasoning applies.
				 */
				cat = static_cast<Concat*>(right);
				if(cat->left->height < cat->right->height) {
					node = concatNodes(left, leftSize, cat->left, cat->weight, emergencyCat);
					cat->left = node;
					cat->weight += leftSize;
					return left;
				}
				scat = static_cast<Concat*>(cat->left);
				node = concatNodes(left, leftSize, scat->left, scat->weight, emergencyCat);
				if(node == scat->left)
					return right;
				return growLeftFixup(node, leftSize + scat->weight, cat);
			}
			else if(!left->height && !right->height
					&& rightSize <= static_cast<Leaf*>(left)->size - leftSize && rightSize <= COMBINE_LIMIT) {
				util::Delete<Concat> ecat(emergencyCat);
				const ElementT* src = static_cast<Leaf*>(right)->getElements();
				ElementT* dest = static_cast<Leaf*>(left)->getElements() + leftSize;
				DestroyElements destroyCopied(dest);
				util::MemorySize index;
				try {
					for(index = static_cast<util::MemorySize>(0u); index < rightSize; ++index) {
						new(dest + index) ElementT(src[index]);
						++destroyCopied.count;
					}
				}
				catch(...) {
					if(!emergencyCat)
						throw;
					emergencyCat->left = left;
					emergencyCat->right = right;
					emergencyCat->weight = leftSize;
					emergencyCat->fixHeight();
					return ecat.set();
				}
				right->destroy(rightSize, static_cast<util::MemorySize>(0u), static_cast<util::MemorySize>(0u));
				delete right;
				destroyCopied.elements = NULL;
				return left;
			}
			else {
				if(!emergencyCat)
					return new Concat(left, right, leftSize);
				emergencyCat->left = left;
				emergencyCat->right = right;
				emergencyCat->weight = leftSize;
				emergencyCat->fixHeight();
				return emergencyCat;
			}
		}

		static Node* insertElement(Node* node, util::MemorySize size, util::MemorySize index, const ElementT& value) {
			if(node->height) {
				// insert into concatenation node
				Concat *cat = static_cast<Concat*>(node), *scat;
				Node* tnode;
				if(index <= cat->weight) {
					// insert into left child
					if(cat->left->height <= cat->right->height) {
						// left may increase in height without fixup
						tnode = insertElement(cat->left, cat->weight, index, value);
						cat->left = tnode;
						++cat->weight;
						return cat;
					}
					scat = static_cast<Concat*>(cat->left);
					/*              +------------+
					 *              | cat:Concat |
					 *              +------------+
					 *  0             /        \                size
					 * <-------------------------------------------->
					 *              /            \
					 *       +-------------+   +------+
					 *       | scat:Concat |   | cr:? |
					 *       +-------------+   +------+
					 *           /      \
					 *   0      /cat->weight   cat->weight      size
					 * <--------------------> <--------------------->
					 *        /            \
					 * +-------+          +-------+
					 * | scl:? |          | scr:? |
					 * +-------+          +-------+
					 *  0  scat->weight   scat->weight   cat->weight
					 * <---------------> <-------------------------->
					 */
					if(index <= scat->weight) {
						// growing cat->left->left; can use normal fixup
						tnode = insertElement(scat->left, scat->weight, index, value);
						return growLeftFixup(tnode, scat->weight + static_cast<util::MemorySize>(1u), cat);
					}
					// cannot use fixup; call down and concatenate
					tnode = insertElement(scat, cat->weight, index, value);
					return concatNodes(tnode, cat->weight + static_cast<util::MemorySize>(1u),
							cat->right, size - cat->weight, cat);
				}
				else {
					// insert into right child
					if(cat->right->height <= cat->left->height) {
						// right may increase in height without fixup
						tnode = insertElement(cat->right, size - cat->weight, index - cat->weight, value);
						cat->right = tnode;
						return cat;
					}
					scat = static_cast<Concat*>(cat->right);
					/*                         +------------+
					 *                         | cat:Concat |
					 *                         +------------+
					 *  0                         /      \                                      size
					 * <---------------------------------------------------------------------------->
					 *                          /          \
					 *                     +------+       +-------------+
					 *                     | cl:? |       | scat:Concat |
					 *                     +------+       +-------------+
					 *                                      /         \
					 *  0               cat->weight     cat->weight    \                        size
					 * <--------------------------->   <-------------------------------------------->
					 *                                   /               \
					 *                                 +-------+   +------+
					 *                                 | scl:? |   | scr:?|
					 *                                 +-------+   +------+
					 *  cat->weight   cat->weight + scat->weight   cat->weight + scat->weight   size
					 * <----------------------------------------> <--------------------------------->
					 */
					if(index > cat->weight + scat->weight) {
						// growing cat->right->right; can use normal fixup
						tnode = insertElement(scat->right, size - cat->weight - scat->weight,
								index - cat->weight - scat->weight, value);
						return growRightFixup(cat, tnode);
					}
					// cannot use fixup; call down and concatenate
					tnode = insertElement(scat, size - cat->weight, index - cat->weight, value);
					return concatNodes(cat->left, cat->weight,
							tnode, size - cat->weight + static_cast<util::MemorySize>(1u), cat);
				}
			}
			else {
				Leaf* leaf = static_cast<Leaf*>(node);
				if(index == size && size < leaf->size) {
					// appending to non-full leaf is trivial
					new(leaf->getElements() + size) ElementT(value);
					return leaf;
				}
				else if(size - index > MOVE_LIMIT && (!index || index == size)) {
					// create new node for 'value'
					DeleteLeafNode<Leaf, Leaf> newLeaf(newSingletonLeaf(value));
					++newLeaf.count;
					Concat* cat = index
						? new Concat(leaf, *newLeaf, size)
						: new Concat(*newLeaf, leaf, static_cast<util::MemorySize>(1u));
					newLeaf.set();
					return cat;
				}
				else if(index && size > MOVE_LIMIT) {
					// split node and concatenate
					util::MemorySize newSize = size - index + static_cast<util::MemorySize>(1u) + SPARE_SIZE;
					DeleteLeafNode<Leaf, Leaf> newLeaf(new(static_cast<size_t>(newSize)) Leaf(newSize));
					const ElementT* src = leaf->getElements();
					ElementT* dest = newLeaf->getElements();
					new(dest) ElementT(value);
					++newLeaf.count;
					util::MemorySize u;
					for(u = index; u < size; ++u) {
						new(dest + (u - index + static_cast<util::MemorySize>(1u))) ElementT(src[u]);
						++newLeaf.count;
					}
					Concat* cat = new Concat(leaf, *newLeaf, index);
					for(u = index; u < size; ++u)
						src[u].~ElementT();
					newLeaf.set();
					return cat;
				}
				else {
					// "split" node, then reassemble it into a new one
					util::MemorySize newSize = size + static_cast<util::MemorySize>(1u) + SPARE_SIZE;
					DeleteLeafNode<Leaf, Leaf> newLeaf(new(static_cast<size_t>(newSize)) Leaf(newSize));
					const ElementT* src = leaf->getElements();
					ElementT* dest = newLeaf->getElements();
					util::MemorySize u;
					for(u = static_cast<util::MemorySize>(0u); u < index; ++u, ++newLeaf.count)
						new(dest++) ElementT(src[u]);
					new(dest++) ElementT(value);
					++newLeaf.count;
					for(u = index; u < size; ++u, ++newLeaf.count)
						new(dest++) ElementT(src[u]);
					leaf->destroy(size, static_cast<util::MemorySize>(0u), static_cast<util::MemorySize>(0u));
					delete leaf;
					return newLeaf.set();
				}
			}
		}

		static Node* spliceNodes(Node* node, util::MemorySize size, util::MemorySize offset,
				util::MemorySize count, Node* replacement, util::MemorySize replacementSize) {
			util::MemorySize index;
			if(!node->height) {
				// The "easy" case: We have a leaf.
				Leaf* leaf = static_cast<Leaf*>(node);
				if(!replacement) {
					if(offset + count == size) {
						// we only need a prefix of this leaf
						ElementT* erase = leaf->getElements() + offset;
						for(index = static_cast<util::MemorySize>(0u); index < count; ++index)
							erase[index].~ElementT();
						return leaf;
					}
					// just fumble together the required elements into a new leaf
					util::MemorySize newSize = size - count + SPARE_SIZE;
					DeleteLeafNode<Leaf, Leaf> newLeaf(new(static_cast<size_t>(newSize)) Leaf(newSize));
					const ElementT* src = leaf->getElements();
					ElementT* dest = newLeaf->getElements();
					for(index = static_cast<util::MemorySize>(0u); index < offset; ++index) {
						new(dest + index) ElementT(src[index]);
						++newLeaf.count;
					}
					for(index = offset + count; index < size; ++index) {
						new(dest + (index - count)) ElementT(src[index]);
						++newLeaf.count;
					}
					leaf->destroy(size, offset, offset + count);
					delete leaf;
					return newLeaf.set();
				}
				// If we only need one piece of 'node',
				// we only need one concatenation...
				if(!offset) {
					// need back piece
					if(!count)
						return concatNodes(replacement, replacementSize, leaf, size, NULL);
					util::MemorySize newSize = leaf->size - count;
					util::MemorySize altSize = size - count + SPARE_SIZE;
					if(altSize > newSize)
						newSize = altSize;
					DeleteLeafNode<Leaf, Leaf> newLeaf(new(static_cast<size_t>(newSize)) Leaf(newSize));
					const ElementT* src = leaf->getElements();
					ElementT* dest = newLeaf->getElements();
					for(index = count; index < size; ++index) {
						new(dest + (index - count)) ElementT(src[index]);
						++newLeaf.count;
					}
					Node* cat = concatNodes(replacement, replacementSize, *newLeaf, size - count, NULL);
					leaf->destroy(size, static_cast<util::MemorySize>(0), count);
					delete leaf;
					newLeaf.set();
					return cat;
				}
				if(offset + count == size) {
					// need front piece
					util::Delete<Concat> emergencyCat(new Concat(leaf, replacement, size));
					ElementT* src = leaf->getElements();
					for(index = static_cast<util::MemorySize>(0u); index < count; ++index)
						src[offset + index].~ElementT();
					Node* cat = concatNodes(leaf, size - count, replacement, replacementSize, *emergencyCat);
					emergencyCat.set();
					return cat;
				}
				// If we need two (front and back) pieces of
				// 'node', we'll just have to split it up
				// and concat 'replacement' right into the
				// middle; giving two new concatenation nodes.
				util::MemorySize newSize = size - (offset + count) + SPARE_SIZE;
				DeleteLeafNode<Leaf, Leaf> newLeaf(new(static_cast<size_t>(newSize)) Leaf(newSize));
				ElementT *src = leaf->getElements(), *dest = newLeaf->getElements();
				for(index = offset + count; index < size; ++index) {
					new(dest + (index - offset - count)) ElementT(src[index]);
					++newLeaf.count;
				}
				util::Delete<Concat> emergencyCat1(new Concat(leaf, replacement, offset));
				util::Delete<Concat> emergencyCat2(new Concat(leaf, replacement, offset));
				for(index = offset; index < size; ++index) {
					if(index < offset + count)
						Destructor(src[index]);
					else
						src[index].~ElementT();
				}
				Node* innerCat = concatNodes(leaf, offset, replacement, replacementSize, *emergencyCat1);
				emergencyCat1.set();
				Node* outerCat = concatNodes(innerCat, offset + replacementSize,
						*newLeaf, size - offset - count, *emergencyCat2);
				emergencyCat2.set();
				newLeaf.set();
				return outerCat;
			}
			else {
				// The "hard" case: We have a concatenation node.
				Concat* cat = static_cast<Concat*>(node);
				/*     +------------+
				 *     | cat:Concat |
				 *     +------------+
				 *      /     w    \
				 * +--------+   +---------+
				 * | left:? |   | right:? |
				 * +--------+   +---------+
				 */
				Node* tnode;
				if(!offset && count >= cat->weight) {
					// splice removes left child
					if(!replacement) {
						// return right subtree
						if(count == cat->weight)
							tnode = cat->right;
						else
							tnode = spliceNodes(cat->right, size - cat->weight, static_cast<util::MemorySize>(0u),
									count - cat->weight, replacement, replacementSize);
						cat->left->destroy(cat->weight, static_cast<util::MemorySize>(0u), cat->weight);
						delete cat->left;
						delete cat;
						return tnode;
					}
					// replace left subtree
					if(count == cat->weight)
						return concatNodes(replacement, replacementSize, cat->right, size - cat->weight, cat);
					else {
						tnode = spliceNodes(cat->right, size - cat->weight,
								static_cast<util::MemorySize>(0u), count - cat->weight, NULL,
								static_cast<util::MemorySize>(0u));
						return concatNodes(replacement, replacementSize, tnode, size - count, cat);
					}
				}
				if(offset <= cat->weight && offset + count == size) {
					// splice removes right child
					if(!replacement) {
						// return left subtree
						if(offset == cat->weight)
							tnode = cat->left;
						else
							tnode = spliceNodes(cat->left, cat->weight, offset, cat->weight - offset,
									replacement, replacementSize);
						cat->right->destroy(size - cat->weight, static_cast<util::MemorySize>(0u), size);
						delete cat->right;
						delete cat;
						return tnode;
					}
					// replace right subtree
					if(offset == cat->weight)
						return concatNodes(cat->left, cat->weight, replacement, replacementSize, cat);
					else {
						tnode = spliceNodes(cat->left, cat->weight, offset, cat->weight - offset,
								NULL, static_cast<util::MemorySize>(0u));
						return concatNodes(tnode, offset, replacement, replacementSize, cat);
					}
				}
				if(offset + count <= cat->weight) {
					// splice is completely in left child
					tnode = spliceNodes(cat->left, cat->weight, offset, count, replacement, replacementSize);
					return concatNodes(tnode, cat->weight - count + replacementSize,
							cat->right, size - cat->weight, cat);
				}
				if(offset >= cat->weight) {
					// splice is completely in right child
					tnode = spliceNodes(cat->right, size - cat->weight, offset - cat->weight, count,
							replacement, replacementSize);
					return concatNodes(cat->left, cat->weight,
							tnode, size - cat->weight - count + replacementSize, cat);
				}
				/* This is the tricky case: Part of the splice
				 * is in the left and part is in the right child,
				 * but neither is covered completely. We
				 * therefore need to call spliceNodes() twice:
				 *     +-------+-------+
				 *     | left  | right |
				 *     +-------+-------+
				 *         <------->
				 *      intended splice
				 *         <--> <-->
				 *     performed splices
				 * Since a successful splice cannot be rolled
				 * back, we're doomed if the second splice
				 * fails; we will have irreversibly modified
				 * half of the tree and cannot modify the
				 * other half accordingly.
				 *
				 * What saves us is the fact that spliceNodes()
				 * CANNOT FAIL if the splice covers a suffix
				 * of the subject tree and there is no
				 * replacement. If we arrange for the second
				 * splice to be like that, the modification
				 * will be atomic, as desired: Either the
				 * first splice fails and nothing is changed
				 * or the first splice succeeds, in which
				 * case the second splice WILL succeed as
				 * well and the overall splice thus succeeds.
				 */
				Node* newRight = spliceNodes(cat->right, size - cat->weight,
						static_cast<util::MemorySize>(0u), offset + count - cat->weight,
						replacement, replacementSize);
				Node* newLeft = spliceNodes(cat->left, cat->weight, offset, cat->weight - offset,
						NULL, static_cast<util::MemorySize>(0u));
				return concatNodes(newLeft, offset, newRight, size - offset - count + replacementSize, cat);
			}
		}

		static void subseqNodes(Node* node, util::MemorySize offset, util::MemorySize count,
				util::Appender<ElementT>& sink) {
			if(!node->height) {
				// leaf
				const ElementT* src = static_cast<Leaf*>(node)->getElements();
				util::MemorySize index;
				for(index = static_cast<util::MemorySize>(0u); index < count; ++index)
					sink.append(src[offset + index]);
			}
			else {
				// concatenation
				Concat* cat = static_cast<Concat*>(node);
				if(offset < cat->weight) {
					util::MemorySize available = cat->weight - offset;
					subseqNodes(cat->left, offset, count > available ? available : count, sink);
				}
				if(offset + count > cat->weight) {
					util::MemorySize begin = offset < cat->weight
							? static_cast<util::MemorySize>(0u) : cat->weight - offset;
					subseqNodes(cat->right, begin, offset + count - cat->weight - begin, sink);
				}
			}
		}

		static Node* subseqNodes(Node* node, util::MemorySize size,
				util::MemorySize offset, util::MemorySize count) {
			if(!offset && count == size)
				return node->clone(size);
			if(!node->height) {
				// leaf
				const ElementT* src = static_cast<Leaf*>(node)->getElements();
				return newIteratedLeaf<const ElementT*>(src + offset, src + (offset + count), count);
			}
			else {
				// concatenation
				Concat* cat = static_cast<Concat*>(node);
				if(offset < cat->weight) {
					// need left child
					if(offset + count > cat->weight) {
						// need both children
						DeleteAnyNode newLeft(subseqNodes(cat->left, cat->weight, offset, cat->weight - offset),
								cat->weight - offset);
						DeleteAnyNode newRight(subseqNodes(cat->right, size - cat->weight,
								static_cast<util::MemorySize>(0u), offset + count - cat->weight));
						Node* result = concatNodes(*newLeft, cat->weight - offset,
								*newRight, offset + count - cat->weight, NULL);
						newLeft.set();
						newRight.set();
						return result;
					}
					else {
						// need *only* left child
						return subseqNodes(cat->left, cat->weight, offset, count);
					}
				}
				else {
					// need right child
					return subseqNodes(cat->right, size - cat->weight, offset - cat->weight, count);
				}
			}
		}

	  private:
		Node* root;
		util::MemorySize cursize;

	  private:
		void checkIndex(util::MemorySize index, bool equalAllowed) const {
			if(equalAllowed ? index > cursize : index >= cursize)
				throw error::ListIndexOutOfBoundsError(index);
		}

		void checkIndices(util::MemorySize& begin, util::MemorySize end) const {
			if(begin > end)
				begin = end;
			if(begin > cursize)
				throw error::ListIndexOutOfBoundsError(begin);
			if(end > cursize)
				throw error::ListIndexOutOfBoundsError(end);
		}

		ElementT* findElement(util::MemorySize index) const {
			if(index >= cursize)
				throw error::ListIndexOutOfBoundsError(index);
			Node* node = root;
			while(node->height) {
				Concat* cat = static_cast<Concat*>(node);
				if(index < cat->weight)
					node = cat->left;
				else {
					node = cat->right;
					index -= cat->weight;
				}
			}
			return static_cast<Leaf*>(node)->getElements() + index;
		}

	  public:
		Rope() : root(NULL), cursize(static_cast<util::MemorySize>(0u)) {}

		Rope(const Rope& rope)
				: root(rope.root ? rope.root->clone(rope.cursize) : NULL), cursize(rope.cursize) {}

		~Rope() {
			if(root) {
				root->destroy(cursize, static_cast<util::MemorySize>(0u), cursize);
				delete root;
			}
		}

		inline util::MemorySize size() const {
			return cursize;
		}

		ElementT& operator[](util::MemorySize index) {
			return *findElement(index);
		}

		const ElementT& operator[](util::MemorySize index) const {
			return *findElement(index);
		}

		template<typename IteratorT>
		void append(IteratorT begin, IteratorT end) {
			if(begin == end)
				return;
			util::MemorySize count = static_cast<util::MemorySize>(end - begin);
			DeleteLeafNode<Leaf, Leaf> leaf(newIteratedLeaf<IteratorT>(begin, end, count));
			leaf.count = count;
			if(root)
				root = concatNodes(root, cursize, *leaf, count, NULL);
			else
				root = *leaf;
			cursize += count;
			leaf.set();
		}

		template<typename IteratorT>
		void prepend(IteratorT begin, IteratorT end) {
			if(begin == end)
				return;
			util::MemorySize count = static_cast<util::MemorySize>(end - begin);
			DeleteLeafNode<Leaf, Leaf> leaf(newIteratedLeaf<IteratorT>(begin, end, count));
			leaf.count = count;
			if(root)
				root = concatNodes(*leaf, count, root, cursize, NULL);
			else
				root = *leaf;
			cursize += count;
			leaf.set();
		}

		template<typename IteratorT>
		void insert(util::MemorySize index, IteratorT begin, IteratorT end) {
			if(index > cursize)
				throw error::ListIndexOutOfBoundsError(index);
			if(begin == end)
				return;
			util::MemorySize count = static_cast<util::MemorySize>(end - begin);
			DeleteLeafNode<Leaf, Leaf> leaf(newIteratedLeaf<IteratorT>(begin, end, count));
			leaf.count = count;
			if(root)
				root = spliceNodes(root, cursize, index, static_cast<util::MemorySize>(0u), *leaf, count);
			else
				root = *leaf;
			cursize += count;
			leaf.set();
		}

		template<typename IteratorT>
		void insert(const ConstIterator& index, IteratorT begin, IteratorT end) {
			insert<IteratorT>(index.index(), begin, end);
		}

		template<typename IteratorT>
		void insert(const ConstReverseIterator& index, IteratorT begin, IteratorT end) {
			insert<IteratorT>(index.index(), begin, end);
		}

		void append(const ElementT& value) {
			if(root)
				root = insertElement(root, cursize, cursize, value);
			else
				root = newSingletonLeaf(value);
			++cursize;
		}

		void prepend(const ElementT& value) {
			if(root)
				root = insertElement(root, cursize, static_cast<util::MemorySize>(0u), value);
			else
				root = newSingletonLeaf(value);
			++cursize;
		}

		Rope& operator+=(const ElementT& value) {
			append(value);
			return *this;
		}

		void insert(util::MemorySize index, const ElementT& value) {
			if(index > cursize)
				throw error::ListIndexOutOfBoundsError(index);
			if(root)
				root = insertElement(root, cursize, index, value);
			else
				root = newSingletonLeaf(value);
			++cursize;
		}

		void insert(const ConstIterator& index, const ElementT& value) {
			insert(index.index(), value);
		}

		void insert(const ConstReverseIterator& index, const ElementT& value) {
			insert(index.index(), value);
		}

		void clear() {
			if(!root)
				return;
			root->destroy(cursize, static_cast<util::MemorySize>(0u), cursize);
			delete root;
			root = NULL;
			cursize = static_cast<util::MemorySize>(0u);
		}

		void erase(util::MemorySize index) {
			checkIndex(index, false);
			if(cursize == static_cast<util::MemorySize>(1u))
				clear();
			else {
				root = spliceNodes(root, cursize, index,
						static_cast<util::MemorySize>(1u), NULL, static_cast<util::MemorySize>(0u));
				--cursize;
			}
		}

		void erase(const ConstIterator& index) {
			erase(index.index());
		}

		void erase(const ConstReverseIterator& index) {
			erase(index.index());
		}

		void erase(util::MemorySize begin, util::MemorySize end) {
			checkIndices(begin, end);
			util::MemorySize count = end - begin;
			if(!count)
				return;
			if(count == cursize)
				clear();
			else {
				root = spliceNodes(root, cursize, begin, count, NULL, static_cast<util::MemorySize>(0u));
				cursize -= count;
			}
		}

		void erase(const ConstIterator& begin, const ConstIterator& end) {
			erase(begin.index(), end.index());
		}

		void erase(const ConstReverseIterator& begin, ConstReverseIterator& end) {
			util::MemorySize eidx = end.index();
			erase(eidx >= cursize ? static_cast<util::MemorySize>(0u) : eidx + static_cast<util::MemorySize>(1u),
					begin.index() + static_cast<util::MemorySize>(1u));
		}

		void splice(util::MemorySize begin, util::MemorySize end, const ElementT& value) {
			checkIndices(begin, end);
			util::MemorySize count = end - begin;
			DeleteLeafNode<Leaf, Leaf> leaf(newSingletonLeaf(value));
			++leaf.count;
			if(count == cursize) {
				root->destroy(cursize, static_cast<util::MemorySize>(0u), cursize);
				delete root;
				root = *leaf;
			}
			else
				root = spliceNodes(root, cursize, begin, count, *leaf, static_cast<util::MemorySize>(1u));
			cursize = cursize - count + static_cast<util::MemorySize>(1u);
			leaf.set();
		}

		void splice(const ConstIterator& begin, const ConstIterator& end, const ElementT& value) {
			splice(begin.index(), end.index(), value);
		}

		void splice(const ConstReverseIterator& begin, const ConstReverseIterator& end, const ElementT& value) {
			util::MemorySize eidx = end.index();
			splice(eidx >= cursize ? static_cast<util::MemorySize>(0u) : eidx + static_cast<util::MemorySize>(1u),
					begin.index() + static_cast<util::MemorySize>(1u), value);
		}

		template<typename IteratorT>
		void splice(util::MemorySize beginIndex, util::MemorySize endIndex,
				IteratorT beginIterator, IteratorT endIterator) {
			checkIndices(beginIndex, endIndex);
			util::MemorySize indexCount = endIndex - beginIndex;
			if(!indexCount && beginIterator == endIterator)
				return;
			util::MemorySize iteratorCount = static_cast<util::MemorySize>(endIterator - beginIterator);
			DeleteLeafNode<Leaf, Leaf> leaf(iteratorCount
					? newIteratedLeaf<IteratorT>(beginIterator, endIterator, iteratorCount) : NULL);
			leaf.count = iteratorCount;
			if(indexCount == cursize) {
				if(root) {
					root->destroy(cursize, static_cast<util::MemorySize>(0u), cursize);
					delete root;
				}
				root = *leaf;
			}
			else
				root = spliceNodes(root, cursize, beginIndex, indexCount, *leaf, iteratorCount);
			leaf.set();
			cursize = cursize - indexCount + iteratorCount;
		}

		template<typename IteratorT>
		void splice(const ConstIterator& beginIndex, const ConstIterator& endIndex,
				IteratorT beginIterator, IteratorT endIterator) {
			splice<IteratorT>(beginIndex.index(), endIndex.index(), beginIterator, endIterator);
		}

		template<typename IteratorT>
		void splice(const ConstReverseIterator& beginIndex, const ConstReverseIterator& endIndex,
				IteratorT beginIterator, IteratorT endIterator) {
			util::MemorySize eidx = endIndex.index();
			splice<IteratorT>(eidx >= cursize
					? static_cast<util::MemorySize>(0u) : eidx + static_cast<util::MemorySize>(1u),
					beginIndex.index() + static_cast<util::MemorySize>(1u), beginIterator, endIterator);
		}

		void subseq(util::MemorySize begin, util::MemorySize end, util::Appender<ElementT>& destination) const {
			checkIndices(begin, end);
			if(end > begin)
				subseqNodes(root, begin, end - begin, destination);
			destination.doneAppending();
		}

		void subseq(const ConstIterator& begin, const ConstIterator& end,
				util::Appender<ElementT>& destination) const {
			subseq(begin.index(), end.index(), destination);
		}

		void subseq(const ConstReverseIterator& begin, const ConstReverseIterator& end,
				util::Appender<ElementT>& destination) const {
			util::MemorySize eidx = end.index();
			subseq(eidx >= cursize ? static_cast<util::MemorySize>(0u) : eidx + static_cast<util::MemorySize>(1u),
					begin.index() + static_cast<util::MemorySize>(1u), destination);
		}

		void subseq(util::MemorySize begin, util::MemorySize end, ElementT* destination) const {
			DestroyElements destroy(destination);
			ElementArrayAppender sink(destination, destroy);
			subseq(begin, end, sink);
		}

		void subseq(const ConstIterator& begin, const ConstIterator& end, ElementT* destination) const {
			subseq(begin.index(), end.index(), destination);
		}

		void subseq(const ConstReverseIterator& begin, const ConstReverseIterator& end,
				ElementT* destination) const {
			util::MemorySize eidx = end.index();
			subseq(eidx >= cursize ? static_cast<util::MemorySize>(0u) : eidx + static_cast<util::MemorySize>(1u),
					begin.index() + static_cast<util::MemorySize>(1u), destination);
		}

		void subseq(util::MemorySize begin, util::MemorySize end, Rope& destination) const {
			checkIndices(begin, end);
			if(end <= begin) {
				destination.clear();
				return;
			}
			util::MemorySize count = end - begin;
			Node* newTree = subseqNodes(root, cursize, begin, count);
			if(destination.root) {
				destination.root->destroy(destination.cursize,
						static_cast<util::MemorySize>(0u), destination.cursize);
				delete destination.root;
			}
			destination.root = newTree;
			destination.cursize = count;
		}

		void subseq(const ConstIterator& begin, const ConstIterator& end, Rope& destination) const {
			subseq(begin.index(), end.index(), destination);
		}

		void subseq(const ConstReverseIterator& begin, const ConstReverseIterator& end, Rope& destination) const {
			util::MemorySize eidx = end.index();
			subseq(eidx >= cursize ? static_cast<util::MemorySize>(0u) : eidx + static_cast<util::MemorySize>(1u),
					begin.index() + static_cast<util::MemorySize>(1u), destination);
		}

		ConstIterator cbegin() const {
			return ConstIterator(this, static_cast<util::MemorySize>(0u));
		}

		ConstIterator cend() const {
			return ConstIterator(this, cursize);
		}

		Iterator begin() {
			return Iterator(this, static_cast<util::MemorySize>(0u));
		}

		Iterator end() {
			return Iterator(this, cursize);
		}

		ConstReverseIterator crbegin() const {
			return ConstReverseIterator(this, cursize ? cursize - static_cast<util::MemorySize>(1u) : cursize);
		}

		ConstReverseIterator crend() const {
			return ConstReverseIterator(this, cursize);
		}

		ReverseIterator rbegin() {
			return ReverseIterator(this, cursize ? cursize - static_cast<util::MemorySize>(1u) : cursize);
		}

		ReverseIterator rend() {
			return ReverseIterator(this, cursize);
		}

		template<typename IteratorT>
		void bappend(IteratorT begin, IteratorT end,
				util::MemorySize batchSize = static_cast<util::MemorySize>(0u)) {
			if(begin == end)
				return;
			if(!batchSize)
				batchSize = SPARE_SIZE;
			util::MemorySize count = static_cast<util::MemorySize>(end - begin);
			while(count) {
				util::MemorySize chunk = count;
				if(chunk > batchSize)
					chunk = batchSize;
				DeleteLeafNode<Leaf, Leaf> leaf(newIteratedLeaf<IteratorT>(begin, begin + chunk, chunk));
				leaf.count = chunk;
				if(root)
					root = concatNodes(root, cursize, *leaf, chunk, NULL);
				else
					root = *leaf;
				cursize += chunk;
				leaf.set();
				count -= chunk;
				begin += chunk;
			}
		}

		template<typename IteratorT>
		void bprepend(IteratorT begin, IteratorT end,
				util::MemorySize batchSize = static_cast<util::MemorySize>(0u)) {
			if(begin == end)
				return;
			if(!batchSize)
				batchSize = SPARE_SIZE;
			util::MemorySize count = static_cast<util::MemorySize>(end - begin);
			while(count) {
				util::MemorySize chunk = count;
				if(chunk > batchSize)
					chunk = batchSize;
				DeleteLeafNode<Leaf, Leaf> leaf(newIteratedLeaf<IteratorT>(begin, begin + chunk, chunk));
				leaf.count = chunk;
				if(root)
					root = concatNodes(*leaf, chunk, root, cursize, NULL);
				else
					root = *leaf;
				cursize += chunk;
				leaf.set();
				count -= chunk;
				begin += chunk;
			}
		}

		template<typename IteratorT>
		void binsert(util::MemorySize index, IteratorT begin, IteratorT end,
				util::MemorySize batchSize = static_cast<util::MemorySize>(0u)) {
			if(index > cursize)
				throw error::ListIndexOutOfBoundsError(index);
			if(begin == end)
				return;
			if(!batchSize)
				batchSize = SPARE_SIZE;
			util::MemorySize count = static_cast<util::MemorySize>(end - begin);
			while(count) {
				util::MemorySize chunk = count;
				if(chunk > batchSize)
					chunk = batchSize;
				DeleteLeafNode<Leaf, Leaf> leaf(newIteratedLeaf<IteratorT>(begin, begin + chunk, chunk));
				leaf.count = chunk;
				if(root)
					root = spliceNodes(root, cursize, index, static_cast<util::MemorySize>(0u), *leaf, chunk);
				else
					root = *leaf;
				cursize += chunk;
				leaf.set();
				count -= chunk;
				begin += chunk;
				index += chunk;
			}
		}

		template<typename IteratorT>
		void binsert(const ConstIterator& index, IteratorT begin, IteratorT end,
				util::MemorySize batchSize = static_cast<util::MemorySize>(0u)) {
			binsert<IteratorT>(index.index(), begin, end, batchSize);
		}

		template<typename IteratorT>
		void binsert(const ConstReverseIterator& index, IteratorT begin, IteratorT end,
				util::MemorySize batchSize = static_cast<util::MemorySize>(0u)) {
			binsert<IteratorT>(index.index(), begin, end, batchSize);
		}

		template<typename IteratorT>
		void bsplice(util::MemorySize beginIndex, util::MemorySize endIndex,
				IteratorT beginIterator, IteratorT endIterator,
				util::MemorySize batchSize = static_cast<util::MemorySize>(0u)) {
			checkIndices(beginIndex, endIndex);
			util::MemorySize indexCount = endIndex - beginIndex;
			if(beginIterator == endIterator) {
				if(indexCount)
					splice<IteratorT>(beginIndex, endIndex, beginIterator, endIterator);
				return;
			}
			util::MemorySize iteratorCount = static_cast<util::MemorySize>(endIterator - beginIterator);
			while(iteratorCount) {
				util::MemorySize chunk = iteratorCount;
				if(chunk > batchSize)
					chunk = batchSize;
				DeleteLeafNode<Leaf, Leaf> leaf(newIteratedLeaf<IteratorT>(beginIterator, endIterator, chunk));
				leaf.count = chunk;
				if(indexCount == cursize) {
					if(root) {
						root->destroy(cursize, static_cast<util::MemorySize>(0u), cursize);
						delete root;
					}
					root = *leaf;
				}
				else
					root = spliceNodes(root, cursize, beginIndex, indexCount, *leaf, chunk);
				cursize = cursize - indexCount + chunk;
				leaf.set();
				iteratorCount -= chunk;
				beginIterator += chunk;
				beginIndex += chunk;
				indexCount = static_cast<util::MemorySize>(0u);
			}
		}

		template<typename IteratorT>
		void bsplice(const ConstIterator& beginIndex, const ConstIterator& endIndex,
				IteratorT beginIterator, IteratorT endIterator,
				util::MemorySize batchSize = static_cast<util::MemorySize>(0u)) {
			bsplice<IteratorT>(beginIndex.index(), endIndex.index(), beginIterator, endIterator, batchSize);
		}

		template<typename IteratorT>
		void bsplice(const ConstReverseIterator& beginIndex, const ConstReverseIterator& endIndex,
				IteratorT beginIterator, IteratorT endIterator,
				util::MemorySize batchSize = static_cast<util::MemorySize>(0u)) {
			util::MemorySize eidx = endIndex.index();
			bsplice<IteratorT>(eidx >= cursize
					? static_cast<util::MemorySize>(0u) : eidx + static_cast<util::MemorySize>(1u),
					beginIndex.index() + static_cast<util::MemorySize>(1u), beginIterator, endIterator, batchSize);
		}

		void assign(const Rope& rope) {
			if(root) {
				root->destroy(cursize, static_cast<util::MemorySize>(0u), cursize);
				delete root;
				root = NULL;
				cursize = static_cast<util::MemorySize>(0u);
			}
			if(rope.root) {
				root = rope.root->clone(rope.cursize);
				cursize = rope.cursize;
			}
		}

		void fill(const ElementT& value, util::MemorySize count,
				util::MemorySize batchSize = static_cast<util::MemorySize>(0u)) {
			if(!batchSize)
				batchSize = SPARE_SIZE;
			while(count) {
				util::MemorySize chunk = count;
				if(chunk > batchSize)
					chunk = batchSize;
				util::MemorySize newSize = chunk + SPARE_SIZE;
				DeleteLeafNode<Leaf, Leaf> leaf(new(static_cast<size_t>(newSize)) Leaf(newSize));
				ElementT* dest = leaf->getElements();
				for(; leaf.count < chunk; ++leaf.count)
					new(dest + leaf.count) ElementT(value);
				if(root)
					root = concatNodes(root, cursize, *leaf, chunk, NULL);
				else
					root = *leaf;
				cursize += chunk;
				leaf.set();
				count -= chunk;
			}
		}

		Rope& operator=(const Rope& other) {
			Node* newRoot = other.root ? other.root->clone(other.cursize) : NULL;
			if(root)
				root->destroy(cursize, static_cast<util::MemorySize>(0u), cursize);
			root = newRoot;
			cursize = other.cursize;
			return *this;
		}

	};

}}

#endif /* REDSTRAIN_MOD_ALGORITHM_ROPE_HPP */
