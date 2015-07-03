#ifndef REDSTRAIN_MOD_ALGORITHM_ROPE_HPP
#define REDSTRAIN_MOD_ALGORITHM_ROPE_HPP

#include <redstrain/util/Delete.hpp>
#include <redstrain/util/WithAlign.hpp>
#include <redstrain/error/IndexOutOfBoundsError.hpp>

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

	  private:
		typedef typename util::WithAlign<util::AlignOf<ElementT>::ALIGNMENT>::Primitive AlignElement;

		template<typename SubjectT, typename LeafT>
		class DeleteLeafNode : public util::Pointer<SubjectT> {

		  public:
			size_t count;

		  public:
			DeleteLeafNode(SubjectT* object = NULL)
					: util::Pointer<SubjectT>(object), count(static_cast<size_t>(0u)) {}

			DeleteLeafNode(const DeleteLeafNode& pointer)
					: util::Pointer<SubjectT>(pointer), count(pointer.count) {}

			~DeleteLeafNode() {
				if(!this->object)
					return;
				if(!this->object->height)
					static_cast<LeafT*>(this->object)->destroyElements(count,
							static_cast<size_t>(0u), static_cast<size_t>(0u));
				delete this->object;
			}

			using util::Pointer<SubjectT>::operator=;

			inline SubjectT* operator=(const DeleteLeafNode& pointer) {
				count = pointer.count;
				return this->object = pointer.object;
			}

		};

		struct Node {

			size_t height;

			Node(size_t height) : height(height) {}
			virtual ~Node() {}

			virtual Node* clone(size_t) const = 0;
			virtual void destroy(size_t, size_t, size_t) = 0;

		};

		struct Leaf : Node {

			size_t size;
			AlignElement elements;

			Leaf(size_t size) : Node(static_cast<size_t>(0u)), size(size) {}

			inline ElementT* getElements() {
				return reinterpret_cast<ElementT*>(&elements);
			}

			void destroyElements(size_t length, size_t finalBegin, size_t finalEnd) {
				ElementT* eptr = reinterpret_cast<ElementT*>(&elements);
				size_t index;
				for(index = static_cast<size_t>(0u); index < length; ++index) {
					if(index >= finalBegin && index < finalEnd)
						Destructor(eptr[index]);
					else
						eptr[index].~ElementT();
				}
			}

			virtual Node* clone(size_t length) const {
				DeleteLeafNode<Leaf, Leaf> newLeaf(new(size) Leaf(size));
				const ElementT* src = reinterpret_cast<const ElementT*>(&elements);
				ElementT* dest = reinterpret_cast<ElementT*>(&newLeaf->elements);
				for(; length; ++src, ++dest, --length) {
					new(dest) ElementT(*src);
					++newLeaf.count;
				}
				return newLeaf.set();
			}

			virtual void destroy(size_t length, size_t finalBegin, size_t finalEnd) {
				destroyElements(length, finalBegin, finalEnd);
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
				DeleteLeafNode<Node, Leaf> newLeft(left->clone(weight));
				DeleteLeafNode<Node, Leaf> newRight(right->clone(length - weight));
				Concat* cat = new Concat(*newLeft, *newRight, weight);
				newLeft.set();
				newRight.set();
				return cat;
			}

			virtual void destroy(size_t length, size_t finalBegin, size_t finalEnd) {
				left->destroy(weight, finalBegin, finalEnd);
				delete left;
				if(finalBegin >= weight)
					right->destroy(length - weight, finalBegin - weight, finalEnd - weight);
				else
					right->destroy(length - weight, static_cast<size_t>(0u), static_cast<size_t>(0u));
				delete right;
			}

		};

		struct DestroyElements {

			ElementT* elements;
			size_t count;

			DestroyElements(ElementT* elements = NULL, size_t count = static_cast<size_t>(0u))
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

	  private:
		static Leaf* newSingletonLeaf(const ElementT& value) {
			size_t size = static_cast<size_t>(1u) + SPARE_SIZE;
			util::Delete<Leaf> leaf(new(size) Leaf(size));
			new(leaf->getElements()) ElementT(value);
			return leaf.set();
		}

		template<typename IteratorT>
		static Leaf* newIteratedLeaf(IteratorT begin, IteratorT end, size_t count) {
			DeleteLeafNode<Leaf, Leaf> leaf(new(count) Leaf(count));
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
			if(left->left->height < left->height - static_cast<size_t>(1u)) {
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
			if(scat->left->height == left->right->height - static_cast<size_t>(1u)) {
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
		static Concat* growLeftFixup(Node* node, size_t nodeSize, Concat* right) {
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
			if(right->right->height < right->height - static_cast<size_t>(1u)) {
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
			if(scat->right->height == right->left->height - static_cast<size_t>(1u)) {
				/* keep structure, increase heights */
				scat->left = node;
				++scat->height;
				++right->height;
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
		static Node* concatNodes(Node* left, size_t leftSize, Node* right, size_t rightSize, Concat* emergencyCat) {
			Node* node;
			Concat *cat, *scat;
			if(left->height > right->height + static_cast<size_t>(1u)) {
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
			else if(right->height > left->height + static_cast<size_t>(1u)) {
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
				size_t index;
				try {
					for(index = static_cast<size_t>(0u); index < rightSize; ++index) {
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
				right->destroy(rightSize, static_cast<size_t>(0u), static_cast<size_t>(0u));
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

		static Node* insertElement(Node* node, size_t size, size_t index, const ElementT& value) {
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
						return growLeftFixup(tnode, scat->weight + static_cast<size_t>(1u), cat);
					}
					// cannot use fixup; call down and concatenate
					tnode = insertElement(scat, cat->weight, index, value);
					return concatNodes(tnode, cat->weight + static_cast<size_t>(1u),
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
							tnode, size - cat->weight + static_cast<size_t>(1u), cat);
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
						: new Concat(*newLeaf, leaf, static_cast<size_t>(1u));
					newLeaf.set();
					return cat;
				}
				else if(index && size > MOVE_LIMIT) {
					// split node and concatenate
					size_t newSize = size - index + static_cast<size_t>(1u) + SPARE_SIZE;
					DeleteLeafNode<Leaf, Leaf> newLeaf(new(newSize) Leaf(newSize));
					const ElementT* src = leaf->getElements();
					ElementT* dest = newLeaf->getElements();
					new(dest) ElementT(value);
					++newLeaf.count;
					size_t u;
					for(u = index; u < size; ++u) {
						new(dest + (u - index + static_cast<size_t>(1u))) ElementT(src[u]);
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
					size_t newSize = size + static_cast<size_t>(1u) + SPARE_SIZE;
					DeleteLeafNode<Leaf, Leaf> newLeaf(new(newSize) Leaf(newSize));
					const ElementT* src = leaf->getElements();
					ElementT* dest = newLeaf->getElements();
					size_t u;
					for(u = static_cast<size_t>(0u); u < index; ++u, ++newLeaf.count)
						new(dest++) ElementT(src[u]);
					new(dest++) ElementT(value);
					++newLeaf.count;
					for(u = index; u < size; ++u, ++newLeaf.count)
						new(dest++) ElementT(src[u]);
					leaf->destroy(size, static_cast<size_t>(0u), static_cast<size_t>(0u));
					delete leaf;
					return newLeaf.set();
				}
			}
		}

		static Node* spliceNodes(Node* node, size_t size, size_t offset, size_t count, Node* replacement,
				size_t replacementSize) {
			size_t index;
			if(!node->height) {
				// The "easy" case: We have a leaf.
				Leaf* leaf = static_cast<Leaf*>(node);
				if(!replacement) {
					if(offset + count == size) {
						// we only need a prefix of this leaf
						ElementT* erase = leaf->getElements() + offset;
						for(index = static_cast<size_t>(0u); index < count; ++index)
							erase[index].~ElementT();
						return leaf;
					}
					// just fumble together the required elements into a new leaf
					size_t newSize = size - count + SPARE_SIZE;
					DeleteLeafNode<Leaf, Leaf> newLeaf(new(newSize) Leaf(newSize));
					const ElementT* src = leaf->getElements();
					ElementT* dest = newLeaf->getElements();
					for(index = static_cast<size_t>(0u); index < offset; ++index) {
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
					size_t newSize = leaf->size - count;
					size_t altSize = size - count + SPARE_SIZE;
					if(altSize > newSize)
						newSize = altSize;
					DeleteLeafNode<Leaf, Leaf> newLeaf(new(newSize) Leaf(newSize));
					const ElementT* src = leaf->getElements();
					ElementT* dest = newLeaf->getElements();
					for(index = count; index < size; ++index) {
						new(dest + (index - count)) ElementT(src[index]);
						++newLeaf.count;
					}
					Node* cat = concatNodes(replacement, replacementSize, *newLeaf, size - count, NULL);
					leaf->destroy(size, static_cast<size_t>(0), count);
					delete leaf;
					newLeaf.set();
					return cat;
				}
				if(offset + count == size) {
					// need front piece
					util::Delete<Concat> emergencyCat(new Concat(leaf, replacement, size));
					ElementT* src = leaf->getElements();
					for(index = static_cast<size_t>(0u); index < count; ++index)
						src[offset + index].~ElementT();
					Node* cat = concatNodes(leaf, size - count, replacement, replacementSize, *emergencyCat);
					emergencyCat.set();
					return cat;
				}
				// If we need two (front and back) pieces of
				// 'node', we'll just have to split it up
				// and concat 'replacement' right into the
				// middle; giving two new concatenation nodes.
				size_t newSize = size - (offset + count) + SPARE_SIZE;
				DeleteLeafNode<Leaf, Leaf> newLeaf(new(newSize) Leaf(newSize));
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
							tnode = spliceNodes(cat->right, size - cat->weight,
									static_cast<size_t>(0u), count - cat->weight, replacement, replacementSize);
						cat->left->destroy(cat->weight, static_cast<size_t>(0u), cat->weight);
						delete cat->left;
						delete cat;
						return tnode;
					}
					// replace left subtree
					if(count == cat->weight)
						return concatNodes(replacement, replacementSize, cat->right, size - cat->weight, cat);
					else {
						tnode = spliceNodes(cat->right, size - cat->weight,
								static_cast<size_t>(0u), count - cat->weight, NULL, static_cast<size_t>(0u));
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
						cat->right->destroy(size - cat->weight, static_cast<size_t>(0u), size);
						delete cat->right;
						delete cat;
						return tnode;
					}
					// replace right subtree
					if(offset == cat->weight)
						return concatNodes(cat->left, cat->weight, replacement, replacementSize, cat);
					else {
						tnode = spliceNodes(cat->left, cat->weight, offset, cat->weight - offset,
								NULL, static_cast<size_t>(0u));
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
						static_cast<size_t>(0u), offset + count - cat->weight, replacement, replacementSize);
				Node* newLeft = spliceNodes(cat->left, cat->weight, offset, cat->weight - offset,
						NULL, static_cast<size_t>(0u));
				return concatNodes(newLeft, offset, newRight, size - offset - count + replacementSize, cat);
			}
		}

	  private:
		Node* root;
		size_t cursize;

	  private:
		ElementT* findElement(size_t index) const {
			if(index >= cursize)
				throw error::IndexOutOfBoundsError("List index out of bounds", index);
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
		Rope() : root(NULL), cursize(static_cast<size_t>(0u)) {}

		Rope(const Rope& rope)
				: root(rope.root ? rope.root->clone(rope.cursize) : NULL), cursize(rope.cursize) {}

		~Rope() {
			if(root) {
				root->destroy(cursize, static_cast<size_t>(0u), cursize);
				delete root;
			}
		}

		inline size_t size() const {
			return cursize;
		}

		ElementT& operator[](size_t index) {
			return *findElement(index);
		}

		const ElementT& operator[](size_t index) const {
			return *findElement(index);
		}

		template<typename IteratorT>
		void append(IteratorT begin, IteratorT end) {
			if(begin == end)
				return;
			size_t count = static_cast<size_t>(end - begin);
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
			size_t count = static_cast<size_t>(end - begin);
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
		void insert(size_t index, IteratorT begin, IteratorT end) {
			if(index > cursize)
				throw error::IndexOutOfBoundsError("List index out of bounds", index);
			if(begin == end)
				return;
			size_t count = static_cast<size_t>(end - begin);
			DeleteLeafNode<Leaf, Leaf> leaf(newIteratedLeaf<IteratorT>(begin, end, count));
			leaf.count = count;
			if(root)
				root = spliceNodes(root, cursize, index, static_cast<size_t>(0u), *leaf, count);
			else
				root = *leaf;
			cursize += count;
			leaf.set();
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
				root = insertElement(root, cursize, static_cast<size_t>(0u), value);
			else
				root = newSingletonLeaf(value);
			++cursize;
		}

		Rope& operator+=(const ElementT& value) {
			append(value);
			return *this;
		}

		void insert(size_t index, const ElementT& value) {
			if(index > cursize)
				throw error::IndexOutOfBoundsError("List index out of bounds", index);
			if(root)
				root = insertElement(root, cursize, index, value);
			else
				root = newSingletonLeaf(value);
			++cursize;
		}

		void clear() {
			if(!root)
				return;
			root->destroy(cursize, static_cast<size_t>(0u), cursize);
			delete root;
			root = NULL;
			cursize = static_cast<size_t>(0u);
		}

		void erase(size_t index) {
			if(index >= cursize)
				throw error::IndexOutOfBoundsError("List index out of bounds", index);
			if(cursize == static_cast<size_t>(1u))
				clear();
			else {
				root = spliceNodes(root, cursize, index, static_cast<size_t>(1u), NULL, static_cast<size_t>(0u));
				--cursize;
			}
		}

		void erase(size_t begin, size_t end) {
			if(begin > cursize)
				throw error::IndexOutOfBoundsError("List index out of bounds", begin);
			if(end > cursize)
				throw error::IndexOutOfBoundsError("List index out of bounds", end);
			size_t count = end - begin;
			if(!count)
				return;
			if(count == cursize)
				clear();
			else {
				root = spliceNodes(root, cursize, begin, count, NULL, static_cast<size_t>(0u));
				cursize -= count;
			}
		}

		void splice(size_t begin, size_t end, const ElementT& value) {
			if(begin > cursize)
				throw error::IndexOutOfBoundsError("List index out of bounds", begin);
			if(end > cursize)
				throw error::IndexOutOfBoundsError("List index out of bounds", end);
			size_t count = end - begin;
			DeleteLeafNode<Leaf, Leaf> leaf(newSingletonLeaf(value));
			++leaf.count;
			if(count == cursize) {
				root->destroy(cursize, static_cast<size_t>(0u), cursize);
				delete root;
				root = *leaf;
			}
			else
				root = spliceNodes(root, cursize, begin, count, *leaf, static_cast<size_t>(1u));
			cursize = cursize - count + static_cast<size_t>(1u);
			leaf.set();
		}

		template<typename IteratorT>
		void splice(size_t beginIndex, size_t endIndex, IteratorT beginIterator, IteratorT endIterator) {
			if(beginIndex > cursize)
				throw error::IndexOutOfBoundsError("List index out of bounds", beginIndex);
			if(endIndex > cursize)
				throw error::IndexOutOfBoundsError("List index out of bounds", endIndex);
			size_t indexCount = endIndex - beginIndex;
			if(!indexCount && beginIterator == endIterator)
				return;
			size_t iteratorCount = static_cast<size_t>(endIterator - beginIterator);
			DeleteLeafNode<Leaf, Leaf> leaf(iteratorCount
					? newIteratedLeaf<IteratorT>(beginIterator, endIterator, iteratorCount) : NULL);
			leaf.count = iteratorCount;
			if(indexCount == cursize) {
				if(root) {
					root->destroy(cursize, static_cast<size_t>(0u), cursize);
					delete root;
				}
				root = *leaf;
			}
			else
				root = spliceNodes(root, cursize, beginIndex, indexCount, *leaf, iteratorCount);
			cursize = cursize - indexCount + iteratorCount;
		}

	};

}}

#endif /* REDSTRAIN_MOD_ALGORITHM_ROPE_HPP */
