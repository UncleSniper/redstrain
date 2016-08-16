#ifndef REDSTRAIN_MOD_ALGORITHM_RANGEMAP_HPP
#define REDSTRAIN_MOD_ALGORITHM_RANGEMAP_HPP

#include <redstrain/util/types.hpp>

#include "RedBlackTree.hpp"

namespace redengine {
namespace algorithm {

	template<
		typename ValueT,
		typename IndexT = util::MemorySize,
		void (*ValueDestructor)(ValueT&) = explicitDestructor<ValueT>,
		void* (*Allocator)(util::MemorySize) = standardAlloc,
		void (*Deallocator)(void*) = standardFree
	>
	class RangeMap {

	  public:
		class Entry;

		class Edge {

		};

		class Entry {

		};

	  private:
		typedef RedBlackTree<
			Edge*,
			char,
			standardCompare<Edge *const&>,
			deleteDestructor<Edge*>,
			noDestructor<char>,
			Allocator,
			Deallocator
		> Edges;
		typedef typename Edges::Iterator EdgeIterator;
		typedef typename Edges::ConstIterator EdgeConstIterator;

		struct Node {

			util::MemorySize height;

			Node(util::MemorySize height) : height(height) {}
			virtual ~Node() {}

			virtual void destroy() = 0;

		};

		struct Leaf : Node {

			Edges edges;

			Leaf() : Node(static_cast<util::MemorySize>(0u)) {}

			virtual void destroy() {
				//TODO
			}

			static void* operator new(size_t size) {
				return Allocator(static_cast<util::MemorySize>(size));
			}

			static void operator delete(void* address) {
				Deallocator(address);
			}

		};

		struct Concat : Node {

			Node *left, *right;
			util::MemorySize weight, skip;

			Concat(Node* left, Node* right, util::MemorySize weight, util::MemorySize skip)
					: Node((left->height < right->height ? right->height : left->height)
					+ static_cast<util::MemorySize>(1u)),
					left(left), right(right), weight(weight), skip(skip) {}

			void fixHeight() {
				this->height = (left->height < right->height
						? right->height : left->height) + static_cast<util::MemorySize>(1u);
			}

			virtual void destroy() {
				left->destroy();
				delete left;
				right->destroy();
				delete right;
			}

			static void* operator new(size_t size) {
				return Allocator(static_cast<util::MemorySize>(size));
			}

			static void operator delete(void* address) {
				Deallocator(address);
			}

		};

	  private:
		static Concat* rebalanceGrownRight(Concat* cat) {
			if(cat->right->height < cat->height)
				return cat;
			/* Right child of 'cat' is too high:
			 *     +------------+
			 *     | cat:Concat |
			 *     +------------+
			 *       /       \
			 * +------+   +-------------+
			 * | cl:? |   | scat:Concat |
			 * +------+   +-------------+
			 *               /       \
			 *         +-------+   +-------+
			 *         | scl:? |   | scr:? |
			 *         +-------+   +-------+
			 * Such that:
			 *   +-----------+           +------------+            +-----+
			 *   |    cl     |           |    scl     |            | scr |
			 *   +-----------+           +------------+            +-----+
			 *   <-----------><---------><------------><---------->
			 *    cat->weight  cat->skip  scat->weight  scat->skip
			 * In the simplest case, 'cl' is one level below 'cat',
			 * i.e. cat->left->height == cat->height - 1. In this
			 * case, we can just raise 'cat' without violating the
			 * invariant.
			 */
			if(cat->left->height == cat->height - static_cast<util::MemorySize>(1u)) {
				++cat->height;
				return cat;
			}
			Concat* scat = static_cast<Concat*>(cat->right);
			/* Otherwise, the question becomes: Why is 'scat' too
			 * high? If it's because 'scr' has risen, then
			 * 'scl' can remain on its level. We can thus
			 * restructure as follows:
			 *              +---------------+
			 *              | result:Concat |
			 *              +---------------+
			 *                /           \
			 *    +-------------+       +------------+
			 *    | ncat:Concat |       | scr:Concat |
			 *    +-------------+       +------------+
			 *      /        \
			 * +------+   +-------+
			 * | cl:? |   | scl:? |
			 * +------+   +-------+
			 * Here, 'scat' becomes 'result' and 'cat' becomes 'ncat'.
			 *
			 * If both 'scl' and 'scr' are one level below 'cat', the
			 * same structure can be used, except 'ncat' and 'result'
			 * will be one level higher. The levels are:
			 *          | Case A | Case B
			 *   -------+--------+-------
			 *   cat    | 0      | 0
			 *   cl     | -2     | -2
			 *   scat   | 0      | 0
			 *   scl    | -2     | -1
			 *   scr    | -1     | -1
			 *   -------+--------+-------
			 *   ncat   | -1     | 0
			 *   result | 0      | 1
			 */
			if(scat->left->height == cat->height - static_cast<util::MemorySize>(2u)
					|| scat->right->height == cat->height - static_cast<util::MemorySize>(1u)) {
				// scat -> result
				// cat -> ncat
				cat->right = scat->left;
				scat->left = cat;
				cat->fixHeight();
				scat->fixHeight();
				scat->weight += cat->weight;
				return scat;
			}
			/* Otherwise, 'scl' has risen, and it must be a Concat node:
			 *      +------------+
			 *      | cat:Concat |
			 *      +------------+
			 *        /       \
			 *  +------+   +-------------+
			 *  | cl:? |   | scat:Concat |
			 *  +------+   +-------------+
			 *                /       \
			 *      +------------+   +-------+
			 *      | scl:Concat |   | scr:? |
			 *      +------------+   +-------+
			 *        /        \
			 * +--------+   +--------+
			 * | scll:? |   | sclr:? |
			 * +--------+   +--------+
			 * Such that:
			 *   +-----------+           +-----------+           +-------------+            +-----+
			 *   |    cl     |           |   scll    |           |    sclr     |            | scr |
			 *   +-----------+           +-----------+           +-------------+            +-----+
			 *   <-----------><---------><-----------><---------><-------------><---------->
			 *    cat->weight  cat->skip  scl->weight  scl->skip  scat->weight   scat->skip
			 *                                                    - scl->weight
			 *                                                    - scl->skip
			 * The heights are therefore:
			 *   - cat: 0
			 *   - cl: -2
			 *   - scat: 0 <-- the problem
			 *   - scl: -1 <-- the cause
			 *   - scr: -2
			 *   - scll: -2 or -3
			 *   - sclr: -2 or -3
			 * As in:
			 *   [ 0]
			 *   [-1]
			 *   [-2] cl  scll  sclr   scr
			 *   [-3]     scll  sclr
			 * We can thus restructure as follows:
			 *   [ 0]      ___(t)____
			 *            /          \
			 *   [-1]   (l)          (r)
			 *          / \          / \
			 *   [-2] cl   scll  sclr   scr
			 *   [-3]      scll  sclr
			 * Here, 'cat' will become 'l', 'scat' will become 'r'
			 * and 'scl' will become 't'.
			 */
			Concat* scl = static_cast<Concat*>(scat->left);
			// cat -> l
			// scat -> r
			// scl -> t
			cat->right = scl->left;
			scat->left = scl->right;
			scl->left = cat;
			scl->right = scat;
			cat->fixHeight();
			scat->fixHeight();
			scl->fixHeight();
			scat->weight -= scl->weight + scl->skip;
			scl->weight += cat->weight + cat->skip;
			return scl;
		}

		static Concat* rebalanceGrownLeft(Concat* cat) {
			if(cat->left->height < cat->height)
				return cat;
			/* Left child of 'cat' is too high:
			 *              +------------+
			 *              | cat:Concat |
			 *              +------------+
			 *                /        \
			 *    +-------------+    +------+
			 *    | scat:Concat |    | cr:? |
			 *    +-------------+    +------+
			 *       /       \
			 * +-------+   +-------+
			 * | scl:? |   | scr:? |
			 * +-------+   +-------+
			 * Such that:
			 *   +------------+            +--------------+           +----+
			 *   |    scl     |            |     scr      |           | cr |
			 *   +------------+            +--------------+           +----+
			 *   <------------><----------><--------------><--------->
			 *    scat->weight  scat->skip  cat->weight     cat->skip
			 *                              - scat->weight
			 *                              - scat->skip
			 * In the simplest case, 'cr' is one level below 'cat',
			 * i.e. cat->right->height == cat->height - 1. In this
			 * case, we can just raise 'cat' without violating the
			 * invariant.
			 */
			if(cat->right->height == cat->height - static_cast<util::MemorySize>(1u)) {
				++cat->height;
				return cat;
			}
			Concat* scat = static_cast<Concat*>(cat->left);
			/* Otherwise, the question becomes: Why is 'scat' too
			 * high? If it's because 'scl' has risen, then
			 * 'scr' can remain on its level. We can thus
			 * restructure as follows:
			 *              +---------------+
			 *              | result:Concat |
			 *              +---------------+
			 *                /           \
			 *    +------------+       +-------------+
			 *    | scl:Concat |       | ncat:Concat |
			 *    +------------+       +-------------+
			 *                            /        \
			 *                      +-------+    +------+
			 *                      | scr:? |    | cr:? |
			 *                      +-------+    +------+
			 * Here, 'scat' becomes 'result' and 'cat' becomes 'ncat'.
			 *
			 * If both 'scl' and 'scr' are one level below 'cat', the
			 * same structure can be used, except 'ncat' and 'result'
			 * will be one level higher. The levels are:
			 *          | Case A | Case B
			 *   -------+--------+-------
			 *   cat    | 0      | 0
			 *   scat   | 0      | 0
			 *   cr     | -2     | -2
			 *   scl    | -1     | -1
			 *   scr    | -2     | -1
			 *   -------+--------+-------
			 *   ncat   | -1     | 0
			 *   result | 0      | 1
			 */
			if(scat->right->height == cat->height - static_cast<util::MemorySize>(2u)
					|| scat->left->height == cat->height - static_cast<util::MemorySize>(1u)) {
				// scat -> result
				// cat -> ncat
				cat->left = scat->right;
				scat->right = cat;
				cat->fixHeight();
				scat->fixHeight();
				cat->weight -= scat->weight + scat->skip;
				return scat;
			}
			/* Otherwise, 'scr' has risen, and it must be a Concat node:
			 *              +------------+
			 *              | cat:Concat |
			 *              +------------+
			 *                /        \
			 *    +-------------+    +------+
			 *    | scat:Concat |    | cr:? |
			 *    +-------------+    +------+
			 *       /       \
			 * +-------+   +------------+
			 * | scl:? |   | scr:Concat |
			 * +-------+   +------------+
			 *               /        \
			 *        +--------+   +--------+
			 *        | scrl:? |   | scrr:? |
			 *        +--------+   +--------+
			 */
			//TODO
		}

	};

}}

#endif /* REDSTRAIN_MOD_ALGORITHM_RANGEMAP_HPP */
