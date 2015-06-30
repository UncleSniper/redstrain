#ifndef REDSTRAIN_MOD_ALGORITHM_ROPE_HPP
#define REDSTRAIN_MOD_ALGORITHM_ROPE_HPP

#include <redstrain/util/Pointer.hpp>
#include <redstrain/util/WithAlign.hpp>

#include "destructors.hpp"

namespace redengine {
namespace algorithm {

	template<
		typename ElementT,
		void (*Destructor)(ElementT&) = explicitDestructor<ElementT>,
		size_t DefaultLeafSize = static_cast<size_t>(0u)
	>
	class Rope {

	  private:
		static const size_t COMBINE_LIMIT = static_cast<size_t>(32u);
		static const size_t MOVE_LIMIT = COMBINE_LIMIT * static_cast<size_t>(2u) / static_cast<size_t>(3u);
		static const size_t _SPARE_SIZE = static_cast<size_t>(128u) / sizeof(ElementT);
		static const size_t SPARE_SIZE = _SPARE_SIZE ? _SPARE_SIZE : static_cast<size_t>(1u);
		static const size_t _DEFAULT_LEAF_SIZE = DefaultLeafSize ? DefaultLeafSize
				: static_cast<size_t>(256u) / sizeof(ElementT);
		static const size_t DEFAULT_LEAF_SIZE = _DEFAULT_LEAF_SIZE ? _DEFAULT_LEAF_SIZE : static_cast<size_t>(2u);

	  private:
		typedef typename util::WithAlign<util::AlignOf<ElementT>::ALIGNMENT>::Primitive AlignElement;

		template<typename SubjectT, typename LeafT>
		class DeleteNode : public util::Pointer<SubjectT> {

		  public:
			size_t count;

		  public:
			DeleteNode(SubjectT* object = NULL)
					: util::Pointer<SubjectT>(object), count(static_cast<size_t>(0u)) {}

			DeleteNode(const DeleteNode& pointer)
					: util::Pointer<SubjectT>(pointer), count(pointer.count) {}

			~DeleteNode() {
				if(!this->object)
					return;
				if(!this->object->height)
					static_cast<LeafT*>(this->object)->destroyElements(count);
				delete this->object;
			}

			using util::Pointer<SubjectT>::operator=;

			inline SubjectT* operator=(const DeleteNode& pointer) {
				return this->object = pointer.object;
			}

		};

		struct Node {

			size_t height;

			Node(size_t height) : height(height) {}
			virtual ~Node() {}

			virtual Node* clone(size_t) const = 0;
			virtual void destroy(size_t) = 0;

		};

		struct Leaf : Node {

			size_t size;
			AlignElement elements;

			Leaf(size_t size) : Node(static_cast<size_t>(0u)), size(size) {}

			inline ElementT* getElements() {
				return reinterpret_cast<ElementT*>(&elements);
			}

			void destroyElements(size_t length) {
				ElementT* eptr = reinterpret_cast<ElementT*>(&elements);
				for(; length; ++eptr, --length)
					Destructor(*eptr);
			}

			virtual Node* clone(size_t length) const {
				DeleteNode<Leaf, Leaf> newLeaf(new(size) Leaf(size));
				ElementT* src = reinterpret_cast<ElementT*>(&elements);
				ElementT* dest = reinterpret_cast<ElementT*>(&newLeaf->elements);
				for(; length; ++src, ++dest, --length) {
					new(dest) ElementT(*src);
					++newLeaf.count;
				}
				return newLeaf.set();
			}

			virtual void destroy(size_t length) {
				destroyElements(length);
			}

			static void* operator new(size_t allocSize, size_t nodeSize) {
				return ::operator new(allocSize - sizeof(AlignElement) + nodeSize * sizeof(ElementT));
			}

		};

		struct Concat : Node {

			Node *left, *right;
			size_t weight;

			Concat(Node* left, Node* right, size_t weight)
					: Node((left->height < right->height ? right->height : left->height) + static_cast<size_t>(1u)),
					left(left), right(right), weight(weight) {}

			void fixHeight() {
				this->height = (left->height < right->height
						? right->height : left->height) + static_cast<size_t>(1u);
			}

			virtual Node* clone(size_t length) const {
				DeleteNode<Node, Leaf> newLeft(left->clone(weight));
				DeleteNode<Node, Leaf> newRight(right->clone(length - weight));
				Concat* cat = new Concat(this->height, *newLeft, *newRight, weight);
				newLeft.set();
				newRight.set();
				return cat;
			}

			virtual void destroy(size_t length) {
				left->destroy(weight);
				delete left;
				right->destroy(length - weight);
				delete right;
			}

		};

	  private:
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
			 * might me scat->height - 1 or scat->height - 2, meaning
			 * scat->height' - 2 or scat->height' - 3; clearly, only the
			 * former is viable.
			 */
			if(left->left->height < left->height - (size_t)1u) {
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
			if(scat->left->height == left->right->height - (size_t)1u) {
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
				ncat->left->fixHeight();
				node->fixHeight();
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
			node->fixHeight(); /* br.height */
			left->right->fixHeight(); /* c.height */
			return left;
		}

	  private:
		Node* root;
		size_t size;

	  public:
		Rope() : root(NULL), size(static_cast<size_t>(0u)) {}
		Rope(const Rope& rope) : root(rope.root ? rope.root->clone(rope.size) : NULL), size(rope.size) {}

		~Rope() {
			if(root) {
				root->destroy(size);
				delete root;
			}
		}

	};

}}

#endif /* REDSTRAIN_MOD_ALGORITHM_ROPE_HPP */
