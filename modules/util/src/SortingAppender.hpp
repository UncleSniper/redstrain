#ifndef REDSTRAIN_MOD_UTIL_SORTINGAPPENDER_HPP
#define REDSTRAIN_MOD_UTIL_SORTINGAPPENDER_HPP

#include <vector>
#include <algorithm>

#include "Appender.hpp"

namespace redengine {
namespace util {

	template<
		typename ElementT,
		typename CompareT = std::less<ElementT>
	>
	class SortingAppender : public Appender<ElementT> {

	  private:
		std::vector<ElementT> elements;
		Appender<ElementT>& sink;
		CompareT compare;

	  public:
		SortingAppender(Appender<ElementT>& sink, const CompareT& compare = CompareT())
				: sink(sink), compare(compare) {}

		SortingAppender(const SortingAppender& appender)
				: Appender<ElementT>(appender), elements(appender.elements), sink(appender.sink),
				compare(appender.compare) {}

		virtual void append(const ElementT& element) {
			elements.push_back(element);
		}

		virtual void doneAppending() {
			std::sort(elements.begin(), elements.end(), compare);
			typename std::vector<ElementT>::const_iterator begin(elements.begin()), end(elements.end());
			for(; begin != end; ++begin)
				sink.append(*begin);
			sink.doneAppending();
		}

	};

}}

#endif /* REDSTRAIN_MOD_UTIL_SORTINGAPPENDER_HPP */
