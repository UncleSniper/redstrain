#ifndef REDSTRAIN_MOD_UTIL_STLOPERATORS_HPP
#define REDSTRAIN_MOD_UTIL_STLOPERATORS_HPP

#include <list>
#include <deque>
#include <vector>

/**
 * @file
 * Definition of the STL collection operators.
 *
 * @since 0.1.0
 */

namespace redengine {
namespace util {

	////////////////////////////////////////////////////////////
	// collection += element
	////////////////////////////////////////////////////////////

	/**
	 * Add an element to a STL list. The object @c element
	 * will be appended to the given <tt>list</tt>, as per
	 * <tt>push_back()</tt>.
	 *
	 * @param list the list to add @c element to
	 * @param element the object to add to @c list
	 * @return @c list
	 *
	 * @tparam ElementT type of all elements in @c list
	 *
	 * @since 0.1.0
	 */
	template<typename ElementT>
	std::list<ElementT>& operator+=(std::list<ElementT>& list, const ElementT& element) {
		list.push_back(element);
		return list;
	}

	/**
	 * Add an element to a STL vector. The object @c element
	 * will be appended to the given <tt>vector</tt>, as per
	 * <tt>push_back()</tt>.
	 *
	 * @param vector the vector to add @c element to
	 * @param element the object to add to @c vector
	 * @return @c vector
	 *
	 * @tparam ElementT type of all elements in @c vector
	 *
	 * @since 0.1.0
	 */
	template<typename ElementT>
	std::vector<ElementT>& operator+=(std::vector<ElementT>& vector, const ElementT& element) {
		vector.push_back(element);
		return vector;
	}

	/**
	 * Add an element to a STL double-ended queue.
	 * The object @c element
	 * will be appended to the given <tt>deque</tt>, as per
	 * <tt>push_back()</tt>.
	 *
	 * @param deque the deque to add @c element to
	 * @param element the object to add to @c deque
	 * @return @c deque
	 *
	 * @tparam ElementT type of all elements in @c deque
	 *
	 * @since 0.1.0
	 */
	template<typename ElementT>
	std::deque<ElementT>& operator+=(std::deque<ElementT>& deque, const ElementT& element) {
		deque.push_back(element);
		return deque;
	}

	////////////////////////////////////////////////////////////
	// list += collection
	////////////////////////////////////////////////////////////

	/**
	 * Add elements of a STL list to another.
	 * The elements in @c inner will be appended
	 * to @c outer as per <tt>push_back()</tt>, in
	 * the order arising from <tt>const_iterator</tt>.
	 *
	 * @param outer the list to add the elements of @c inner to
	 * @param inner the list whose elements to add to @c outer
	 * @return @c outer
	 *
	 * @tparam OuterElementT type of all elements in @c outer
	 * @tparam InnerElementT type of all elements in @c inner
	 *
	 * @since 0.1.0
	 */
	template<typename OuterElementT, typename InnerElementT>
	std::list<OuterElementT>& operator+=(std::list<OuterElementT>& outer,
			const std::list<InnerElementT>& inner) {
		typename std::list<InnerElementT>::const_iterator begin(inner.begin()), end(inner.end());
		for(; begin != end; ++begin)
			outer.push_back(*begin);
		return outer;
	}

	/**
	 * Add elements of a STL vector to a STL list.
	 * The elements in @c inner will be appended
	 * to @c outer as per <tt>push_back()</tt>, in
	 * the order arising from <tt>const_iterator</tt>.
	 *
	 * @param outer the list to add the elements of @c inner to
	 * @param inner the vector whose elements to add to @c outer
	 * @return @c outer
	 *
	 * @tparam OuterElementT type of all elements in @c outer
	 * @tparam InnerElementT type of all elements in @c inner
	 *
	 * @since 0.1.0
	 */
	template<typename OuterElementT, typename InnerElementT>
	std::list<OuterElementT>& operator+=(std::list<OuterElementT>& outer,
			const std::vector<InnerElementT>& inner) {
		typename std::vector<InnerElementT>::const_iterator begin(inner.begin()), end(inner.end());
		for(; begin != end; ++begin)
			outer.push_back(*begin);
		return outer;
	}

	/**
	 * Add elements of a STL double-ended queue to a STL list.
	 * The elements in @c inner will be appended
	 * to @c outer as per <tt>push_back()</tt>, in
	 * the order arising from <tt>const_iterator</tt>.
	 *
	 * @param outer the list to add the elements of @c inner to
	 * @param inner the deque whose elements to add to @c outer
	 * @return @c outer
	 *
	 * @tparam OuterElementT type of all elements in @c outer
	 * @tparam InnerElementT type of all elements in @c inner
	 *
	 * @since 0.1.0
	 */
	template<typename OuterElementT, typename InnerElementT>
	std::list<OuterElementT>& operator+=(std::list<OuterElementT>& outer,
			const std::deque<InnerElementT>& inner) {
		typename std::deque<InnerElementT>::const_iterator begin(inner.begin()), end(inner.end());
		for(; begin != end; ++begin)
			outer.push_back(*begin);
		return outer;
	}

	////////////////////////////////////////////////////////////
	// vector += collection
	////////////////////////////////////////////////////////////

	/**
	 * Add elements of a STL list to a STL vector.
	 * The elements in @c inner will be appended
	 * to @c outer as per <tt>push_back()</tt>, in
	 * the order arising from <tt>const_iterator</tt>.
	 *
	 * @param outer the vector to add the elements of @c inner to
	 * @param inner the list whose elements to add to @c outer
	 * @return @c outer
	 *
	 * @tparam OuterElementT type of all elements in @c outer
	 * @tparam InnerElementT type of all elements in @c inner
	 *
	 * @since 0.1.0
	 */
	template<typename OuterElementT, typename InnerElementT>
	std::vector<OuterElementT>& operator+=(std::vector<OuterElementT>& outer,
			const std::list<InnerElementT>& inner) {
		typename std::list<InnerElementT>::const_iterator begin(inner.begin()), end(inner.end());
		for(; begin != end; ++begin)
			outer.push_back(*begin);
		return outer;
	}

	/**
	 * Add elements of a STL vector to another.
	 * The elements in @c inner will be appended
	 * to @c outer as per <tt>push_back()</tt>, in
	 * the order arising from <tt>const_iterator</tt>.
	 *
	 * @param outer the vector to add the elements of @c inner to
	 * @param inner the vector whose elements to add to @c outer
	 * @return @c outer
	 *
	 * @tparam OuterElementT type of all elements in @c outer
	 * @tparam InnerElementT type of all elements in @c inner
	 *
	 * @since 0.1.0
	 */
	template<typename OuterElementT, typename InnerElementT>
	std::vector<OuterElementT>& operator+=(std::vector<OuterElementT>& outer,
			const std::vector<InnerElementT>& inner) {
		typename std::vector<InnerElementT>::const_iterator begin(inner.begin()), end(inner.end());
		for(; begin != end; ++begin)
			outer.push_back(*begin);
		return outer;
	}

	/**
	 * Add elements of a STL double-ended queue to a STL vector.
	 * The elements in @c inner will be appended
	 * to @c outer as per <tt>push_back()</tt>, in
	 * the order arising from <tt>const_iterator</tt>.
	 *
	 * @param outer the vector to add the elements of @c inner to
	 * @param inner the deque whose elements to add to @c outer
	 * @return @c outer
	 *
	 * @tparam OuterElementT type of all elements in @c outer
	 * @tparam InnerElementT type of all elements in @c inner
	 *
	 * @since 0.1.0
	 */
	template<typename OuterElementT, typename InnerElementT>
	std::vector<OuterElementT>& operator+=(std::vector<OuterElementT>& outer,
			const std::deque<InnerElementT>& inner) {
		typename std::deque<InnerElementT>::const_iterator begin(inner.begin()), end(inner.end());
		for(; begin != end; ++begin)
			outer.push_back(*begin);
		return outer;
	}

	////////////////////////////////////////////////////////////
	// deque += collection
	////////////////////////////////////////////////////////////

	/**
	 * Add elements of a STL list to a STL double-ended queue.
	 * The elements in @c inner will be appended
	 * to @c outer as per <tt>push_back()</tt>, in
	 * the order arising from <tt>const_iterator</tt>.
	 *
	 * @param outer the deque to add the elements of @c inner to
	 * @param inner the list whose elements to add to @c outer
	 * @return @c outer
	 *
	 * @tparam OuterElementT type of all elements in @c outer
	 * @tparam InnerElementT type of all elements in @c inner
	 *
	 * @since 0.1.0
	 */
	template<typename OuterElementT, typename InnerElementT>
	std::deque<OuterElementT>& operator+=(std::deque<OuterElementT>& outer,
			const std::list<InnerElementT>& inner) {
		typename std::list<InnerElementT>::const_iterator begin(inner.begin()), end(inner.end());
		for(; begin != end; ++begin)
			outer.push_back(*begin);
		return outer;
	}

	/**
	 * Add elements of a STL vector to a STL double-ended queue.
	 * The elements in @c inner will be appended
	 * to @c outer as per <tt>push_back()</tt>, in
	 * the order arising from <tt>const_iterator</tt>.
	 *
	 * @param outer the deque to add the elements of @c inner to
	 * @param inner the vector whose elements to add to @c outer
	 * @return @c outer
	 *
	 * @tparam OuterElementT type of all elements in @c outer
	 * @tparam InnerElementT type of all elements in @c inner
	 *
	 * @since 0.1.0
	 */
	template<typename OuterElementT, typename InnerElementT>
	std::deque<OuterElementT>& operator+=(std::deque<OuterElementT>& outer,
			const std::vector<InnerElementT>& inner) {
		typename std::vector<InnerElementT>::const_iterator begin(inner.begin()), end(inner.end());
		for(; begin != end; ++begin)
			outer.push_back(*begin);
		return outer;
	}

	/**
	 * Add elements of a STL double-ended queue to another.
	 * The elements in @c inner will be appended
	 * to @c outer as per <tt>push_back()</tt>, in
	 * the order arising from <tt>const_iterator</tt>.
	 *
	 * @param outer the deque to add the elements of @c inner to
	 * @param inner the deque whose elements to add to @c outer
	 * @return @c outer
	 *
	 * @tparam OuterElementT type of all elements in @c outer
	 * @tparam InnerElementT type of all elements in @c inner
	 *
	 * @since 0.1.0
	 */
	template<typename OuterElementT, typename InnerElementT>
	std::deque<OuterElementT>& operator+=(std::deque<OuterElementT>& outer,
			const std::deque<InnerElementT>& inner) {
		typename std::deque<InnerElementT>::const_iterator begin(inner.begin()), end(inner.end());
		for(; begin != end; ++begin)
			outer.push_back(*begin);
		return outer;
	}

}}

#endif /* REDSTRAIN_MOD_UTIL_STLOPERATORS_HPP */
