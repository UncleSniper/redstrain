#ifndef REDSTRAIN_MOD_UTIL_REFERENCECOUNTED_HPP
#define REDSTRAIN_MOD_UTIL_REFERENCECOUNTED_HPP

/**
 * @file
 * Definition of the @link redengine::util::ReferenceCounted base
 * for reference-counted objects @endlink.
 *
 * @since 0.1.0
 */

namespace redengine {
namespace util {

	/**
	 * Base for reference-counted objects. Any class whose instances
	 * should keep their own reference count may inherit this
	 * behavior from this class. The @c ref() and @c unref() member
	 * functions may be used to increment and decrement, respectively,
	 * the otherwise opaque reference count. When that count reaches
	 * zero, the object is deleted.
	 *
	 * Note that copying a reference-counted object will reset the count
	 * of the copy; it is therefore imperative that copy-constructors
	 * of derived classes call the ReferenceCounted copy-constructor.
	 *
	 * @since 0.1.0
	 */
	class ReferenceCounted {

	  private:
		unsigned refcount;

	  protected:
		/**
		 * Shallow copy-constructor. The reference count is initialized
		 * with 1, as the newly constructed object has exactly one
		 * reference, namely that from the client constructing it.
		 *
		 * @since 0.1.0
		 */
		ReferenceCounted(const ReferenceCounted&) : refcount(1u) {}

	  public:
		/**
		 * Default constructor. The reference count is initialized
		 * with 1, as the newly constructed object has exactly one
		 * reference, namely that from the client constructing it.
		 *
		 * @since 0.1.0
		 */
		ReferenceCounted() : refcount(1u) {}
		/**
		 * No-operation virtual destructor. Derived classes may safely
		 * assume that their own destructors will be called in case
		 * the object is deleted during a call to #unref.
		 *
		 * @since 0.1.0
		 */
		virtual ~ReferenceCounted() {}

		/**
		 * Increment the reference count by one. Note that no checks
		 * for overflow are performed and excessive calls to this
		 * function may cause the object to be deleted prematurely
		 * if the reference count wraps around. The count is kept as
		 * a data member of type <tt>unsigned</tt>; the client must
		 * take precautions to ensure that the address space of that
		 * type is not exceeded.
		 *
		 * @since 0.1.0
		 */
		inline void ref() {
			++refcount;
		}

		/**
		 * Decrement the reference count by one. If this causes the
		 * count to reach zero, the object is deleted by invoking the
		 * C++ @c delete operator on <tt>this</tt>.
		 *
		 * @since 0.1.0
		 */
		inline void unref() {
			if(!--refcount)
				delete this;
		}

	};

}}

#endif /* REDSTRAIN_MOD_UTIL_REFERENCECOUNTED_HPP */
