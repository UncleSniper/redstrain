#ifndef REDSTRAIN_MOD_ALGORITHM_REDBLACKTREE_HPP
#define REDSTRAIN_MOD_ALGORITHM_REDBLACKTREE_HPP

#include <redstrain/util/BareType.hpp>
#include <redstrain/util/WithAlign.hpp>
#include <redstrain/error/ListIndexOutOfBoundsError.hpp>

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

		bool put(const KeyT& key, const ValueT& value) {
			Node* node = new Node(RED);
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

	};

}}

#endif /* REDSTRAIN_MOD_ALGORITHM_REDBLACKTREE_HPP */
