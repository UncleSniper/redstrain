#ifndef REDSTRAIN_MOD_UTIL_APPENDER_HPP
#define REDSTRAIN_MOD_UTIL_APPENDER_HPP

/**
 * @file
 * Definition of the @link redengine::util::Appender collection element
 * adder interface @endlink.
 *
 * @since 0.1.0
 */

namespace redengine {
namespace util {

	/**
	 * Interface for adding elements to collections. An instance of Appender
	 * shall represent a reference to a collection and a means to add elements
	 * to that collection. If the collection is ordered, adding an element in
	 * such a manner should, by convention, add it to the end of the
	 * collection. Classes implementing this interface need provide only the
	 * #append operation, which adds a single element.
	 *
	 * The typical use for this facility is for generator functions to emit
	 * the objects they generate by calling #append on a given Appender for
	 * each such object.
	 *
	 * @tparam ElementT type of all elements of the represented collection
	 *
	 * @since 0.1.0
	 */
	template<typename ElementT>
	class Appender {

	  public:
		/**
		 * Type of all elements of the collection. Present so that the type
		 * parameter can be extracted from a template instance.
		 *
		 * @since 0.1.0
		 */
		typedef ElementT Element;

	  public:
		/**
		 * No-operation constructor.
		 *
		 * @since 0.1.0
		 */
		Appender() {}
		/**
		 * No-operation copy-constructor.
		 *
		 * @since 0.1.0
		 */
		Appender(const Appender&) {}
		/**
		 * No-operation virtual destructor. Thus, derived classes may safely
		 * assume that their own destructors will be called.
		 *
		 * @since 0.1.0
		 */
		virtual ~Appender() {}

		/**
		 * Add a single element to the collection. This is merely a convenience
		 * alias and simply calls #append, then returns <tt>*this</tt>.
		 *
		 * @param element an element to add to the collection
		 * @return a reference to this Appender
		 *
		 * @since 0.1.0
		 */
		Appender& operator+=(const ElementT& element) {
			append(element);
			return *this;
		}

		/**
		 * Add a single element to the collection. That is, modify the
		 * collection represented by this object in a way that it will be
		 * deemed to include an object equivalent to the given @c element
		 * after the call to this function returns. By convention,
		 * - if the collection is ordered, the element should be added to its
		 *   end ("back").
		 * - a value copy of the element given should be added, rather than
		 *   retaining the actual reference (which might refer to a temporary
		 *   object).
		 * - if the collection requires elements to be unique (such as a
		 *   set data structure) and the given element is already present,
		 *   the function should perform no operation and return gracefully.
		 * - if the element cannot be added for any other reason, an exception
		 *   should be thrown.
		 *
		 * @param element an element to add to the collection
		 *
		 * @since 0.1.0
		 */
		virtual void append(const ElementT& element) = 0;

		virtual void doneAppending() {}

	};

}}

#endif /* REDSTRAIN_MOD_UTIL_APPENDER_HPP */
