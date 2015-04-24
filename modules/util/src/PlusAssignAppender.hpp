#ifndef REDSTRAIN_MOD_UTIL_PLUSASSIGNAPPENDER_HPP
#define REDSTRAIN_MOD_UTIL_PLUSASSIGNAPPENDER_HPP

#include "Appender.hpp"

/**
 * @file
 * Definition of the @link redengine::util::PlusAssignAppender addition
 * assignment collection element adder @endlink.
 *
 * @since 0.1.0
 */

namespace redengine {
namespace util {

	/**
	 * Addition assignment collection element adder. This Appender
	 * uses the C++ @c += operator to append elements to a
	 * collection.
	 *
	 * @tparam CollectionT type of the represented collection
	 * @tparam ElementT type of all elements of the represented collection
	 * @tparconstr
	 * - @c Addition of a <tt>const ElementT&</tt> to a @c CollectionT&
	 *   via @c += must be implemented: There must exist either
	 *   <tt>operator+=(CollectionT&, const ElementT&)</tt> or
	 *   <tt>CollectionT::operator+=(const ElementT&)</tt>.
	 *
	 * @since 0.1.0
	 */
	template<typename CollectionT, typename ElementT>
	class PlusAssignAppender : public Appender<ElementT> {

	  public:
		/**
		 * Type of the collection. Present so that the type
		 * parameter can be extracted from a template instance.
		 *
		 * @since 0.1.0
		 */
		typedef CollectionT Collection;

	  private:
		CollectionT& collection;

	  public:
		/**
		 * Constructor from collection. That is, this appender will represent
		 * the given @c collection with respect to the appending of elements.
		 *
		 * @param collection the collection this appender represents
		 *
		 * @since 0.1.0
		 */
		PlusAssignAppender(CollectionT& collection) : collection(collection) {}
		/**
		 * Shallow copy-constructor. The appender being constructed will
		 * represent the same collection as the given @c appender in the
		 * sense that both appenders will hold identical references to
		 * that same collection object.
		 *
		 * @param appender the appender to copy the collection reference from
		 *
		 * @since 0.1.0
		 */
		PlusAssignAppender(const PlusAssignAppender& appender)
				: Appender<ElementT>(appender), collection(appender.collection) {}

		/**
		 * Retrieve the underlying collection. That is, return a reference
		 * to the collection object this appender represents.
		 *
		 * @return the collection this appender represents
		 *
		 * @since 0.1.0
		 */
		inline CollectionT& getCollection() {
			return collection;
		}

		/**
		 * Retrieve the underlying collection. That is, return a reference
		 * to the collection object this appender represents.
		 *
		 * @return the collection this appender represents
		 *
		 * @since 0.1.0
		 */
		inline const CollectionT& getCollection() const {
			return collection;
		}

		/**
		 * Add a single element to the collection. That is, invoke the
		 * C++ @c += operator on the collection object this appender
		 * represents on the left-hand side and the given @c element
		 * on the right-hand side. The operation performed is therefore:
		 * @code
		   getCollection() += element;
		 * @endcode
		 *
		 * @param element an element to add to the collection
		 *
		 * @since 0.1.0
		 */
		virtual void append(const ElementT& element) {
			collection += element;
		}

	};

}}

#endif /* REDSTRAIN_MOD_UTIL_PLUSASSIGNAPPENDER_HPP */
