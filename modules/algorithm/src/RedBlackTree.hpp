#ifndef REDSTRAIN_MOD_ALGORITHM_REDBLACKTREE_HPP
#define REDSTRAIN_MOD_ALGORITHM_REDBLACKTREE_HPP

#include <utility>
#include <redstrain/util/BareType.hpp>
#include <redstrain/util/WithAlign.hpp>
#include <redstrain/error/ListIndexOutOfBoundsError.hpp>

#include "CreateOrModify.hpp"
#include "NoElementInThisStateError.hpp"
#include "ordering.hpp"
#include "allocators.hpp"
#include "destructors.hpp"

namespace redengine {
namespace algorithm {

	template<
		typename KeyT,
		typename ValueT,
		util::OrderRelation (*Comparison)(
			typename util::BareType<KeyT>::ConstRef,
			typename util::BareType<KeyT>::ConstRef
		) = standardCompare<typename util::BareType<KeyT>::ConstRef>,
		void (*KeyDestructor)(KeyT&) = explicitDestructor<KeyT>,
		void (*ValueDestructor)(ValueT&) = explicitDestructor<ValueT>,
		void* (*Allocator)(util::MemorySize) = standardAlloc,
		void (*Deallocator)(void*) = standardFree
	>
	class RedBlackTree {

	  public:
		typedef KeyT Key;
		typedef ValueT Value;

	  public:
		static const int LOWER_BOUND_INCLUSIVE = 001;
		static const int UPPER_BOUND_INCLUSIVE = 002;
		static const int IGNORE_LOWER_BOUND    = 004;
		static const int IGNORE_UPPER_BOUND    = 010;

	  private:
		typedef typename util::WithAlign<util::AlignOf<KeyT>::ALIGNMENT>::Primitive AlignKey;
		typedef typename util::WithAlign<util::AlignOf<ValueT>::ALIGNMENT>::Primitive AlignValue;

		enum Color {
			RED,
			BLACK
		};

		struct Node {

			Color color;
			Node *parent, *left, *right;
			union {
				char buffer[sizeof(KeyT)];
				AlignKey align;
			} key;
			union {
				char buffer[sizeof(ValueT)];
				AlignValue align;
			} value;

			Node() {}

			Node(Color color) : color(color), parent(NULL), left(NULL), right(NULL) {}

			Node(Color color, Node* parent, Node* left, Node* right)
					: color(color), parent(parent), left(left), right(right) {}

			inline KeyT& getKey() {
				return *reinterpret_cast<KeyT*>(key.buffer);
			}

			inline const KeyT& getKey() const {
				return *reinterpret_cast<const KeyT*>(key.buffer);
			}

			inline ValueT& getValue() {
				return *reinterpret_cast<ValueT*>(value.buffer);
			}

			inline const ValueT& getValue() const {
				return *reinterpret_cast<const ValueT*>(value.buffer);
			}

			void assignStructure(const Node& other) {
				color = other.color;
				parent = other.parent;
				left = other.left;
				right = other.right;
			}

			inline Node* grandparent() {
				return this && parent ? parent->parent : NULL;
			}

			Node* uncle() {
				Node* g = grandparent();
				if(!g)
					return NULL;
				return parent == g->left ? g->right : g->left;
			}

			Node* sibling() {
				if(this && parent)
					return this == parent->left ? parent->right : parent->left;
				else
					return NULL;
			}

			void destroy() {
				if(left) {
					left->destroy();
					delete left;
				}
				if(right) {
					right->destroy();
					delete right;
				}
				KeyDestructor(getKey());
				ValueDestructor(getValue());
			}

			void destroyLocal() {
				KeyDestructor(getKey());
				ValueDestructor(getValue());
			}

			static void* operator new(size_t allocSize) {
				return Allocator(static_cast<util::MemorySize>(allocSize));
			}

			static void operator delete(void* address) {
				Deallocator(address);
			}

		};

		struct DeleteSingleNode {

			static const int HAS_KEY   = 01;
			static const int HAS_VALUE = 02;

			Node* node;
			int flags;

			DeleteSingleNode(Node* node, int flags) : node(node), flags(flags) {}

			~DeleteSingleNode() {
				if(node) {
					if(flags & DeleteSingleNode::HAS_KEY)
						KeyDestructor(node->getKey());
					if(flags & DeleteSingleNode::HAS_VALUE)
						ValueDestructor(node->getValue());
					delete node;
				}
			}

		};

		struct DeleteSubtree {

			Node* node;

			DeleteSubtree(Node* node) : node(node) {}

			~DeleteSubtree() {
				if(node) {
					node->destroy();
					delete node;
				}
			}

		};

	  private:
		static Node* clone(const Node* node) {
			Node* c = new Node(node->color);
			{
				new(&c->getKey()) KeyT(node->getKey());
				DeleteSingleNode newNode(c, DeleteSingleNode::HAS_KEY);
				new(&c->getValue()) ValueT(node->getValue());
				newNode.node = NULL;
			}
			DeleteSubtree subtree(c);
			if(node->left) {
				c->left = RedBlackTree::clone(node->left);
				c->left->parent = c;
			}
			if(node->right) {
				c->right = RedBlackTree::clone(node->right);
				c->right->parent = c;
			}
			subtree.node = NULL;
			return c;
		}

	  private:
		Node* root;
		util::MemorySize cursize;

	  private:
		bool findNode(const KeyT& key, Node*& found, util::OrderRelation& relation) const {
			Node *prev = NULL, *current = root;
			while(current) {
				relation = Comparison(key, current->getKey());
				switch(relation) {
					case util::OR_LESS:
						prev = current;
						current = current->left;
						break;
					case util::OR_GREATER:
						prev = current;
						current = current->right;
						break;
					case util::OR_EQUAL:
					default:
						found = current;
						return true;
				}
			}
			found = prev;
			return false;
		}

		void replace(Node& newNode, const Node& oldNode) {
			newNode.assignStructure(oldNode);
			if(oldNode.parent) {
				if(oldNode.parent->left == &oldNode)
					oldNode.parent->left = &newNode;
				else
					oldNode.parent->right = &newNode;
			}
			if(oldNode.left)
				oldNode.left->parent = &newNode;
			if(oldNode.right)
				oldNode.right->parent = &newNode;
			if(root == &oldNode)
				root = &newNode;
		}

		void transplant(Node* newNode, const Node& oldNode) {
			// CLRS:
			//   u -> oldNode
			//   v -> newNode
			if(!oldNode.parent)
				root = newNode;
			else if(&oldNode == oldNode.parent->left)
				oldNode.parent->left = newNode;
			else
				oldNode.parent->right = newNode;
			if(newNode)
				newNode->parent = oldNode.parent;
		}

		void rotateLeft(Node* node) {
			Node* r = node->right;
			node->right = r->left;
			if(r->left)
				r->left->parent = node;
			r->parent = node->parent;
			if(!node->parent)
				root = r;
			else if(node == node->parent->left)
				node->parent->left = r;
			else
				node->parent->right = r;
			r->left = node;
			node->parent = r;
		}

		void rotateRight(Node* node) {
			Node* l = node->left;
			node->left = l->right;
			if(l->right)
				l->right->parent = node;
			l->parent = node->parent;
			if(!node->parent)
				root = l;
			else if(node == node->parent->left)
				node->parent->left = l;
			else
				node->parent->right = l;
			l->right = node;
			node->parent = l;
		}

		void insertFixup(Node* node) {
			// CLRS:
			//   z -> node
			//   y -> uncle
			while(node->parent && node->parent->color == RED) {
				Node* grandparent = node->grandparent();
				if(node->parent == grandparent->left) {
					// direction explicitly in book
					Node* uncle = grandparent->right;
					if(uncle && uncle->color == RED) {
						node->parent->color = BLACK;
						uncle->color = BLACK;
						grandparent->color = RED;
						node = grandparent;
					}
					else {
						if(node == node->parent->right) {
							node = node->parent;
							rotateLeft(node);
						}
						node->parent->color = BLACK;
						grandparent->color = RED;
						rotateRight(grandparent);
					}
				}
				else {
					// reverse direction "with 'right' and 'left' exchanged"
					Node* uncle = grandparent->left;
					if(uncle && uncle->color == RED) {
						node->parent->color = BLACK;
						uncle->color = BLACK;
						grandparent->color = RED;
						node = grandparent;
					}
					else {
						if(node == node->parent->left) {
							node = node->parent;
							rotateRight(node);
						}
						node->parent->color = BLACK;
						grandparent->color = RED;
						rotateLeft(grandparent);
					}
				}
			}
			root->color = BLACK;
		}

		void removeFixup(Node* node, Node* parent, bool isRightChild) {
			// *groan* I guess now the whole 'use NULL in place of sentinels'
			// thing comes back to bite us... This function might be called
			// on a sentinel and we have to take great care to do the right
			// thing in such a case.
			// CLRS:
			//   x -> node
			while(node != root && (!node || node->color == BLACK)) {
				if(isRightChild) {
					// reverse direction "with 'right' and 'left' exchanged"
					Node* w = parent ? parent->left : NULL;
					if(w && w->color == RED) {  // lines 4-8
						w->color = BLACK;
						parent->color = RED;
						rotateRight(parent);
						w = parent->left;
					}
					if(!w || ((!w->left || w->left->color == BLACK) && (!w->right || w->right->color == BLACK))) {
						if(w)
							w->color = RED;
						node = parent;
						if(parent) {
							parent = parent->parent;
							if(parent)
								isRightChild = node == parent->right;
						}
					}
					else {
						if(!w->left || w->left->color == BLACK) {
							w->right->color = BLACK;
							w->color = RED;
							rotateLeft(w);
							w = parent->left;
						}
						w->color = parent->color;
						parent->color = BLACK;
						if(w->left)
							w->left->color = BLACK;
						rotateRight(parent);
						node = root;
					}
				}
				else {
					// direction explicitly in book
					Node* w = parent ? parent->right : NULL;
					if(w && w->color == RED) {  // lines 4-8
						w->color = BLACK;
						parent->color = RED;
						// OK, so w exists, so parent exists and has a right child
						// (namely w), so we can call rotateLeft() on it.
						rotateLeft(parent);
						w = parent->right;
					}
					// lines 9-11
					if(!w || ((!w->left || w->left->color == BLACK) && (!w->right || w->right->color == BLACK))) {
						if(w)
							w->color = RED;
						node = parent;
						if(parent) {
							parent = parent->parent;
							if(parent)
								isRightChild = node == parent->right;
						}
					}
					else {  // lines 12-21
						if(!w->right || w->right->color == BLACK) {  // lines 12-16
							w->left->color = BLACK;  // see comment below
							w->color = RED;
							// We know that w->right is black as per the if above, so w->left
							// cannot be black as well, as per the if above *that*, otherwise
							// we wouldn't be in the 'else' clause of that one. Since that
							// means w->left is real, we can call rotateRight() on w.
							rotateRight(w);
							// This will put w->left as the right child of parent, so w will
							// still be real after this:
							w = parent->right;
						}
						w->color = parent->color;
						parent->color = BLACK;
						if(w->right)
							w->right->color = BLACK;
						rotateLeft(parent);
						node = root;
					}
				}
			}
			if(node)
				node->color = BLACK;
		}

		void removeNode(Node* node) {
			// CLRS:
			//   z -> node
			//   y -> successor
			//   x -> tofix
			Color originalColor = node->color;
			Node *tofix, *fixParent;
			bool isRightChild;
			if(!node->left) {
				tofix = node->right;
				fixParent = node->parent;
				isRightChild = fixParent && node == fixParent->right;
				transplant(tofix, *node);
			}
			else if(!node->right) {
				tofix = node->left;
				fixParent = node->parent;
				isRightChild = fixParent && node == fixParent->right;
				transplant(tofix, *node);
			}
			else {
				Node* successor = node->right;
				while(successor->left)
					successor = successor->left;
				originalColor = successor->color;
				tofix = successor->right;
				fixParent = successor->parent;
				isRightChild = successor == fixParent->right;
				transplant(tofix, *successor);
				successor->right = node->right;
				successor->right->parent = successor;
				transplant(successor, *node);
				successor->left = node->left;
				successor->left->parent = successor;
				successor->color = node->color;
			}
			node->destroyLocal();
			delete node;
			if(originalColor == BLACK)
				removeFixup(tofix, fixParent, isRightChild);
		}

		bool insert(const KeyT& key, const ValueT& value, Node*& node) {
			node = new Node(RED);
			{
				new(&node->getKey()) KeyT(key);
				DeleteSingleNode newNode(node, DeleteSingleNode::HAS_KEY);
				new(&node->getValue()) ValueT(value);
				newNode.node = NULL;
			}
			Node* existing;
			util::OrderRelation relation;
			if(findNode(key, existing, relation)) {
				// entry exists, replace that node
				replace(*node, *existing);
				KeyDestructor(existing->getKey());
				ValueDestructor(existing->getValue());
				delete existing;
				return true;
			}
			// entry does not exist, insert new node
			node->parent = existing;
			if(existing) {
				if(relation == util::OR_LESS)
					existing->left = node;
				else
					existing->right = node;
			}
			else
				root = node;
			insertFixup(node);
			++cursize;
			return false;
		}

		Node* putIntoNode(Node* existing, const KeyT& key, const ValueT& value) {
			Node* node = new Node(RED);
			{
				new(&node->getKey()) KeyT(key);
				DeleteSingleNode newNode(node, DeleteSingleNode::HAS_KEY);
				new(&node->getValue()) ValueT(value);
				newNode.node = NULL;
			}
			replace(*node, *existing);
			KeyDestructor(existing->getKey());
			ValueDestructor(existing->getValue());
			delete existing;
			return node;
		}

	  private:
		template<typename TreeT>
		class IteratorBase {

		  protected:
			TreeT* tree;
			Node* node;
			bool reverse;

		  protected:
			void forward() {
				if(!node)
					return;
				if(node->right) {
					node = node->right;
					while(node->left)
						node = node->left;
				}
				else {
					Node* original;
					do {
						original = node;
						node = node->parent;
					} while(node && original == node->right);
				}
			}

			void backward() {
				if(!node)
					return;
				if(node->left) {
					node = node->left;
					while(node->right)
						node = node->right;
				}
				else {
					Node* original;
					do {
						original = node;
						node = node->parent;
					} while(node && original == node->left);
				}
			}

			void assignIteratorBase(const IteratorBase& iterator) {
				tree = iterator.tree;
				node = iterator.node;
				reverse = iterator.reverse;
			}

			util::OrderRelation compareTo(const IteratorBase& iterator) const {
				if(!this->node)
					return iterator.node ? util::OR_GREATER : util::OR_EQUAL;
				if(!iterator.node)
					return util::OR_LESS;
				if(this->node == iterator.node)
					return util::OR_EQUAL;
				return Comparison(this->node->getKey(), iterator.node->getKey());
			}

		  public:
			IteratorBase(TreeT& tree, bool reverse) : tree(&tree), node(tree.root), reverse(reverse) {}

			IteratorBase(TreeT& tree, Node* node) : tree(&tree), node(node), reverse(false) {}

			IteratorBase(const IteratorBase& iterator)
					: tree(iterator.tree), node(iterator.node), reverse(iterator.reverse) {}

			inline bool valid() const {
				return !!node;
			}

			inline bool isReverse() const {
				return reverse;
			}

			const KeyT& getKey() const {
				if(!node)
					throw NoElementInThisStateError();
				return node->getKey();
			}

			operator bool() const {
				return !!node;
			}

		};

		template<typename TreeT>
		class UnboundedIteratorBase : public virtual IteratorBase<TreeT> {

		  public:
			UnboundedIteratorBase(TreeT& tree, bool reverse) : IteratorBase<TreeT>(tree, reverse) {
				reset();
			}

			UnboundedIteratorBase(TreeT& tree, Node* node) : IteratorBase<TreeT>(tree, node) {}

			UnboundedIteratorBase(const UnboundedIteratorBase& iterator) : IteratorBase<TreeT>(iterator) {}

			void reset() {
				this->node = this->tree->root;
				if(this->node) {
					if(this->reverse) {
						while(this->node->right)
							this->node = this->node->right;
					}
					else {
						while(this->node->left)
							this->node = this->node->left;
					}
				}
			}

			bool moveTo(const KeyT& key) {
				util::OrderRelation relation;
				if(this->tree->findNode(key, this->node, relation))
					return true;
				this->node = NULL;
				return false;
			}

		};

		template<typename TreeT>
		class BoundedIteratorBase : public virtual IteratorBase<TreeT> {

		  private:
			KeyT lowerBound, upperBound;
			int flags;

		  protected:
			bool isWithinBounds(Node* node) const {
				if(!node)
					return false;
				if(!(flags & RedBlackTree::IGNORE_LOWER_BOUND)) {
					switch(Comparison(node->getKey(), lowerBound)) {
						case util::OR_LESS:
							return false;
						case util::OR_GREATER:
							break;
						case util::OR_EQUAL:
						default:
							if(!(flags & RedBlackTree::LOWER_BOUND_INCLUSIVE))
								return false;
							break;
					}
				}
				if(!(flags & RedBlackTree::IGNORE_UPPER_BOUND)) {
					switch(Comparison(node->getKey(), upperBound)) {
						case util::OR_LESS:
							break;
						case util::OR_GREATER:
							return false;
						case util::OR_EQUAL:
						default:
							if(!(flags & RedBlackTree::UPPER_BOUND_INCLUSIVE))
								return false;
							break;
					}
				}
				return true;
			}

			void assignBoundedIterator(const BoundedIteratorBase& iterator) {
				lowerBound = iterator.lowerBound;
				upperBound = iterator.upperBound;
				flags = iterator.flags;
			}

		  public:
			BoundedIteratorBase(TreeT& tree, bool reverse,
					const KeyT& lowerBound, const KeyT& upperBound, int flags)
					: IteratorBase<TreeT>(tree, reverse), lowerBound(lowerBound), upperBound(upperBound),
					flags(flags) {
				reset();
			}

			BoundedIteratorBase(const BoundedIteratorBase& iterator)
					: IteratorBase<TreeT>(iterator), lowerBound(iterator.lowerBound),
					upperBound(iterator.upperBound), flags(iterator.flags) {}

			inline const KeyT& getLowerBound() const {
				return lowerBound;
			}

			inline const KeyT& getUpperBound() const {
				return upperBound;
			}

			inline int getFlags() const {
				return flags;
			}

			void reset() {
				this->node = this->tree->root;
				if(!this->node)
					return;
				if(!isWithinBounds(this->node)) {
					this->node = NULL;
					return;
				}
				if(this->reverse) {
					while(isWithinBounds(this->node->right))
						this->node = this->node->right;
				}
				else {
					while(isWithinBounds(this->node->left))
						this->node = this->node->left;
				}
			}

			bool moveTo(const KeyT& key) {
				util::OrderRelation relation;
				if(this->tree->findNode(key, this->node, relation)) {
					if(isWithinBounds(this->node))
						return true;
				}
				this->node = NULL;
				return false;
			}

		};

		class ConstIteratorBase : public virtual IteratorBase<const RedBlackTree> {

		  public:
			ConstIteratorBase(const RedBlackTree& tree, bool reverse)
					: IteratorBase<const RedBlackTree>(tree, reverse) {}

			ConstIteratorBase(const RedBlackTree& tree, Node* node)
					: IteratorBase<const RedBlackTree>(tree, node) {}

			ConstIteratorBase(const ConstIteratorBase& iterator) : IteratorBase<const RedBlackTree>(iterator) {}

			inline const RedBlackTree& getTree() const {
				return *this->tree;
			}

			const ValueT& getValue() const {
				if(!this->node)
					throw NoElementInThisStateError();
				return this->node->getValue();
			}

			std::pair<const KeyT&, const ValueT&> operator*() const {
				if(!this->node)
					throw NoElementInThisStateError();
				return std::pair<const KeyT&, const ValueT&>(this->node->getKey(), this->node->getValue());
			}

		};

		class MutableIteratorBase : public virtual IteratorBase<RedBlackTree> {

		  public:
			MutableIteratorBase(RedBlackTree& tree, bool reverse)
					: IteratorBase<RedBlackTree>(tree, reverse) {}

			MutableIteratorBase(RedBlackTree& tree, Node* node)
					: IteratorBase<RedBlackTree>(tree, node) {}

			MutableIteratorBase(const MutableIteratorBase& iterator) : IteratorBase<RedBlackTree>(iterator) {}

			inline const RedBlackTree& getTree() const {
				return *this->tree;
			}

			inline RedBlackTree& getTree() {
				return *this->tree;
			}

			const ValueT& getValue() const {
				if(!this->node)
					throw NoElementInThisStateError();
				return this->node->getValue();
			}

			ValueT& getValue() {
				if(!this->node)
					throw NoElementInThisStateError();
				return this->node->getValue();
			}

			std::pair<const KeyT&, const ValueT&> operator*() const {
				if(!this->node)
					throw NoElementInThisStateError();
				return std::pair<const KeyT&, const ValueT&>(this->node->getKey(), this->node->getValue());
			}

			std::pair<const KeyT&, ValueT&> operator*() {
				if(!this->node)
					throw NoElementInThisStateError();
				return std::pair<const KeyT&, ValueT&>(this->node->getKey(), this->node->getValue());
			}

		};

	  public:
		class ConstIterator : public ConstIteratorBase, public UnboundedIteratorBase<const RedBlackTree> {

		  public:
			ConstIterator(const RedBlackTree& tree, bool reverse)
					: IteratorBase<const RedBlackTree>(tree, reverse), ConstIteratorBase(tree, reverse),
					UnboundedIteratorBase<const RedBlackTree>(tree, reverse) {}

			ConstIterator(const RedBlackTree& tree, Node* node)
					: IteratorBase<const RedBlackTree>(tree, node), ConstIteratorBase(tree, node),
					UnboundedIteratorBase<const RedBlackTree>(tree, node) {}

			ConstIterator(const ConstIterator& iterator)
					: IteratorBase<const RedBlackTree>(iterator), ConstIteratorBase(iterator),
					UnboundedIteratorBase<const RedBlackTree>(iterator) {}

			ConstIterator& operator++() {
				if(this->reverse)
					this->backward();
				else
					this->forward();
				return *this;
			}

			ConstIterator operator++(int) {
				ConstIterator iterator(*this);
				if(this->reverse)
					this->backward();
				else
					this->forward();
				return iterator;
			}

			ConstIterator& operator--() {
				if(this->reverse)
					this->forward();
				else
					this->backward();
				return *this;
			}

			ConstIterator operator--(int) {
				ConstIterator iterator(*this);
				if(this->reverse)
					this->forward();
				else
					this->backward();
				return iterator;
			}

			ConstIterator& operator=(const ConstIterator& iterator) {
				this->assignIteratorBase(iterator);
				return *this;
			}

			bool operator==(const ConstIterator& iterator) const {
				return this->compareTo(iterator) == util::OR_EQUAL;
			}

			bool operator!=(const ConstIterator& iterator) const {
				return this->compareTo(iterator) != util::OR_EQUAL;
			}

			bool operator<(const ConstIterator& iterator) const {
				return this->compareTo(iterator) == util::OR_LESS;
			}

			bool operator<=(const ConstIterator& iterator) const {
				return this->compareTo(iterator) != util::OR_GREATER;
			}

			bool operator>(const ConstIterator& iterator) const {
				return this->compareTo(iterator) == util::OR_GREATER;
			}

			bool operator>=(const ConstIterator& iterator) const {
				return this->compareTo(iterator) != util::OR_LESS;
			}

		};

		class ConstBoundedIterator : public ConstIteratorBase, public BoundedIteratorBase<const RedBlackTree> {

		  public:
			ConstBoundedIterator(const RedBlackTree& tree, bool reverse,
					const KeyT& lowerBound, const KeyT& upperBound, int flags)
					: IteratorBase<const RedBlackTree>(tree, reverse), ConstIteratorBase(tree, reverse),
					BoundedIteratorBase<const RedBlackTree>(tree, reverse, lowerBound, upperBound, flags) {}

			ConstBoundedIterator(const ConstBoundedIterator& iterator)
					: IteratorBase<const RedBlackTree>(iterator), ConstIteratorBase(iterator),
					BoundedIteratorBase<const RedBlackTree>(iterator) {}

			ConstBoundedIterator& operator++() {
				if(this->reverse)
					this->backward();
				else
					this->forward();
				if(!this->isWithinBounds(this->node))
					this->node = NULL;
				return *this;
			}

			ConstBoundedIterator operator++(int) {
				ConstBoundedIterator iterator(*this);
				if(this->reverse)
					this->backward();
				else
					this->forward();
				if(!this->isWithinBounds(this->node))
					this->node = NULL;
				return iterator;
			}

			ConstBoundedIterator& operator--() {
				if(this->reverse)
					this->forward();
				else
					this->backward();
				if(!this->isWithinBounds(this->node))
					this->node = NULL;
				return *this;
			}

			ConstBoundedIterator operator--(int) {
				ConstBoundedIterator iterator(*this);
				if(this->reverse)
					this->forward();
				else
					this->backward();
				if(!this->isWithinBounds(this->node))
					this->node = NULL;
				return iterator;
			}

			ConstBoundedIterator& operator=(const ConstBoundedIterator& iterator) {
				this->assignIteratorBase(iterator);
				this->assignBoundedIterator(iterator);
				return *this;
			}

			bool operator==(const ConstBoundedIterator& iterator) const {
				return this->compareTo(iterator) == util::OR_EQUAL;
			}

			bool operator!=(const ConstBoundedIterator& iterator) const {
				return this->compareTo(iterator) != util::OR_EQUAL;
			}

			bool operator<(const ConstBoundedIterator& iterator) const {
				return this->compareTo(iterator) == util::OR_LESS;
			}

			bool operator<=(const ConstBoundedIterator& iterator) const {
				return this->compareTo(iterator) != util::OR_GREATER;
			}

			bool operator>(const ConstBoundedIterator& iterator) const {
				return this->compareTo(iterator) == util::OR_GREATER;
			}

			bool operator>=(const ConstBoundedIterator& iterator) const {
				return this->compareTo(iterator) != util::OR_LESS;
			}

		};

		class Iterator : public MutableIteratorBase, public UnboundedIteratorBase<RedBlackTree> {

		  public:
			Iterator(RedBlackTree& tree, bool reverse)
					: IteratorBase<RedBlackTree>(tree, reverse), MutableIteratorBase(tree, reverse),
					UnboundedIteratorBase<RedBlackTree>(tree, reverse) {}

			Iterator(RedBlackTree& tree, Node* node)
					: IteratorBase<RedBlackTree>(tree, node), MutableIteratorBase(tree, node),
					UnboundedIteratorBase<RedBlackTree>(tree, node) {}

			Iterator(const Iterator& iterator)
					: IteratorBase<RedBlackTree>(iterator), MutableIteratorBase(iterator),
					UnboundedIteratorBase<RedBlackTree>(iterator) {}

			void erase() {
				if(!this->node)
					throw NoElementInThisStateError();
				Node* condemned = this->node;
				if(this->reverse)
					this->backward();
				else
					this->forward();
				this->tree->removeNode(condemned);
				--this->tree->cursize;
			}

			Iterator& operator++() {
				if(this->reverse)
					this->backward();
				else
					this->forward();
				return *this;
			}

			Iterator operator++(int) {
				Iterator iterator(*this);
				if(this->reverse)
					this->backward();
				else
					this->forward();
				return iterator;
			}

			Iterator& operator--() {
				if(this->reverse)
					this->forward();
				else
					this->backward();
				return *this;
			}

			Iterator operator--(int) {
				Iterator iterator(*this);
				if(this->reverse)
					this->forward();
				else
					this->backward();
				return iterator;
			}

			Iterator& operator=(const Iterator& iterator) {
				this->assignIteratorBase(iterator);
				return *this;
			}

			Iterator& operator=(const ValueT& value) {
				if(!this->node)
					throw NoElementInThisStateError();
				this->node->getValue() = value;
				return *this;
			}

			bool operator==(const Iterator& iterator) const {
				return this->compareTo(iterator) == util::OR_EQUAL;
			}

			bool operator!=(const Iterator& iterator) const {
				return this->compareTo(iterator) != util::OR_EQUAL;
			}

			bool operator<(const Iterator& iterator) const {
				return this->compareTo(iterator) == util::OR_LESS;
			}

			bool operator<=(const Iterator& iterator) const {
				return this->compareTo(iterator) != util::OR_GREATER;
			}

			bool operator>(const Iterator& iterator) const {
				return this->compareTo(iterator) == util::OR_GREATER;
			}

			bool operator>=(const Iterator& iterator) const {
				return this->compareTo(iterator) != util::OR_LESS;
			}

		};

		class BoundedIterator : public MutableIteratorBase, public BoundedIteratorBase<RedBlackTree> {

		  public:
			BoundedIterator(RedBlackTree& tree, bool reverse,
					const KeyT& lowerBound, const KeyT& upperBound, int flags)
					: IteratorBase<RedBlackTree>(tree, reverse), MutableIteratorBase(tree, reverse),
					BoundedIteratorBase<RedBlackTree>(tree, reverse, lowerBound, upperBound, flags) {}

			BoundedIterator(const BoundedIterator& iterator)
					: IteratorBase<RedBlackTree>(iterator), MutableIteratorBase(iterator),
					BoundedIteratorBase<RedBlackTree>(iterator) {}

			void erase() {
				if(!this->node)
					throw NoElementInThisStateError();
				Node* condemned = this->node;
				if(this->reverse)
					this->backward();
				else
					this->forward();
				if(!this->isWithinBounds(this->node))
					this->node = NULL;
				this->tree->removeNode(condemned);
				--this->tree->cursize;
			}

			BoundedIterator& operator++() {
				if(this->reverse)
					this->backward();
				else
					this->forward();
				if(!this->isWithinBounds(this->node))
					this->node = NULL;
				return *this;
			}

			BoundedIterator operator++(int) {
				BoundedIterator iterator(*this);
				if(this->reverse)
					this->backward();
				else
					this->forward();
				if(!this->isWithinBounds(this->node))
					this->node = NULL;
				return iterator;
			}

			BoundedIterator& operator--() {
				if(this->reverse)
					this->forward();
				else
					this->backward();
				if(!this->isWithinBounds(this->node))
					this->node = NULL;
				return *this;
			}

			BoundedIterator operator--(int) {
				BoundedIterator iterator(*this);
				if(this->reverse)
					this->forward();
				else
					this->backward();
				if(!this->isWithinBounds(this->node))
					this->node = NULL;
				return iterator;
			}

			BoundedIterator& operator=(const Iterator& iterator) {
				this->assignIteratorBase(iterator);
				this->assignBoundedIterator(iterator);
				return *this;
			}

			BoundedIterator& operator=(const ValueT& value) {
				if(!this->node)
					throw NoElementInThisStateError();
				this->node->getValue() = value;
				return *this;
			}

			bool operator==(const BoundedIterator& iterator) const {
				return this->compareTo(iterator) == util::OR_EQUAL;
			}

			bool operator!=(const BoundedIterator& iterator) const {
				return this->compareTo(iterator) != util::OR_EQUAL;
			}

			bool operator<(const BoundedIterator& iterator) const {
				return this->compareTo(iterator) == util::OR_LESS;
			}

			bool operator<=(const BoundedIterator& iterator) const {
				return this->compareTo(iterator) != util::OR_GREATER;
			}

			bool operator>(const BoundedIterator& iterator) const {
				return this->compareTo(iterator) == util::OR_GREATER;
			}

			bool operator>=(const BoundedIterator& iterator) const {
				return this->compareTo(iterator) != util::OR_LESS;
			}

		};

		class InsertionCookie {

		  private:
			RedBlackTree* tree;
			const KeyT key;
			Node* node;

		  public:
			InsertionCookie(RedBlackTree& tree, const KeyT& key) : tree(&tree), key(key), node(NULL) {}

			InsertionCookie(const InsertionCookie& cookie)
					: tree(cookie.tree), key(cookie.key), node(cookie.node) {}

			inline RedBlackTree& getTree() {
				return *tree;
			}

			inline const RedBlackTree& getTree() const {
				return *tree;
			}

			inline const KeyT& getKey() const {
				return key;
			}

			bool put(const ValueT& value) {
				if(node) {
					node = tree->putIntoNode(node, key, value);
					return true;
				}
				else
					return tree->insert(key, value, node);
			}

			InsertionCookie& operator=(const ValueT& value) {
				put(value);
				return *this;
			}

		};

	  public:
		RedBlackTree() : root(NULL), cursize(static_cast<util::MemorySize>(0u)) {}

		RedBlackTree(const RedBlackTree& tree) : root(tree.root ? RedBlackTree::clone(tree.root) : NULL),
				cursize(tree.cursize) {}

		~RedBlackTree() {
			if(root) {
				root->destroy();
				delete root;
			}
		}

		inline util::MemorySize size() const {
			return cursize;
		}

		bool put(const KeyT& key, const ValueT& value) {
			Node* node;
			return insert(key, value, node);
		}

		bool put(const KeyT& key, CreateOrModify<ValueT>& value) {
			Node* existing;
			util::OrderRelation relation;
			if(findNode(key, existing, relation)) {
				value.modifyElement(existing->getValue());
				return true;
			}
			Node* node = new Node(RED);
			{
				new(&node->getKey()) KeyT(key);
				DeleteSingleNode newNode(node, DeleteSingleNode::HAS_KEY);
				value.createElement(node->getValue());
				newNode.node = NULL;
			}
			node->parent = existing;
			if(existing) {
				if(relation == util::OR_LESS)
					existing->left = node;
				else
					existing->right = node;
			}
			else
				root = node;
			insertFixup(node);
			++cursize;
			return false;
		}

		bool erase(const KeyT& key) {
			Node* existing;
			util::OrderRelation relation;
			if(!findNode(key, existing, relation))
				return false;
			removeNode(existing);
			--cursize;
			return true;
		}

		void clear() {
			if(root) {
				root->destroy();
				delete root;
				root = NULL;
				cursize = static_cast<util::MemorySize>(0u);
			}
		}

		Iterator all() {
			return Iterator(*this, false);
		}

		Iterator rall() {
			return Iterator(*this, true);
		}

		ConstIterator call() const {
			return ConstIterator(*this, false);
		}

		ConstIterator crall() const {
			return ConstIterator(*this, true);
		}

		BoundedIterator range(const KeyT& lowerBound, const KeyT& upperBound, int flags) {
			return BoundedIterator(*this, false, lowerBound, upperBound, flags);
		}

		BoundedIterator rrange(const KeyT& lowerBound, const KeyT& upperBound, int flags) {
			return BoundedIterator(*this, true, lowerBound, upperBound, flags);
		}

		ConstBoundedIterator crange(const KeyT& lowerBound, const KeyT& upperBound, int flags) const {
			return ConstBoundedIterator(*this, false, lowerBound, upperBound, flags);
		}

		ConstBoundedIterator crrange(const KeyT& lowerBound, const KeyT& upperBound, int flags) const {
			return ConstBoundedIterator(*this, true, lowerBound, upperBound, flags);
		}

		Iterator operator[](const KeyT& key) {
			Iterator iterator(*this, false);
			iterator.moveTo(key);
			return iterator;
		}

		ConstIterator operator[](const KeyT& key) const {
			ConstIterator iterator(*this, false);
			iterator.moveTo(key);
			return iterator;
		}

		InsertionCookie operator()(const Key& key) {
			return InsertionCookie(*this, key);
		}

		BoundedIterator operator<(const KeyT& upperBound) {
			return BoundedIterator(*this, false, upperBound, upperBound,
					RedBlackTree::UPPER_BOUND_INCLUSIVE | RedBlackTree::IGNORE_LOWER_BOUND);
		}

		BoundedIterator operator<=(const KeyT& upperBound) {
			return BoundedIterator(*this, false, upperBound, upperBound, RedBlackTree::IGNORE_LOWER_BOUND);
		}

		BoundedIterator operator>(const KeyT& lowerBound) {
			return BoundedIterator(*this, false, lowerBound, lowerBound,
					RedBlackTree::LOWER_BOUND_INCLUSIVE | RedBlackTree::IGNORE_UPPER_BOUND);
		}

		BoundedIterator operator>=(const KeyT& lowerBound) {
			return BoundedIterator(*this, false, lowerBound, lowerBound, RedBlackTree::IGNORE_UPPER_BOUND);
		}

		ConstBoundedIterator operator<(const KeyT& upperBound) const {
			return ConstBoundedIterator(*this, false, upperBound, upperBound,
					RedBlackTree::UPPER_BOUND_INCLUSIVE | RedBlackTree::IGNORE_LOWER_BOUND);
		}

		ConstBoundedIterator operator<=(const KeyT& upperBound) const {
			return ConstBoundedIterator(*this, false, upperBound, upperBound, RedBlackTree::IGNORE_LOWER_BOUND);
		}

		ConstBoundedIterator operator>(const KeyT& lowerBound) const {
			return ConstBoundedIterator(*this, false, lowerBound, lowerBound,
					RedBlackTree::LOWER_BOUND_INCLUSIVE | RedBlackTree::IGNORE_UPPER_BOUND);
		}

		ConstBoundedIterator operator>=(const KeyT& lowerBound) const {
			return ConstBoundedIterator(*this, false, lowerBound, lowerBound, RedBlackTree::IGNORE_UPPER_BOUND);
		}

	};

}}

#endif /* REDSTRAIN_MOD_ALGORITHM_REDBLACKTREE_HPP */
