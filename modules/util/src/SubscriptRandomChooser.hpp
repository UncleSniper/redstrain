#ifndef REDSTRAIN_MOD_UTIL_SUBSCRIPTRANDOMCHOOSER_HPP
#define REDSTRAIN_MOD_UTIL_SUBSCRIPTRANDOMCHOOSER_HPP

namespace redengine {
namespace util {

	template<typename CollectionT, typename ElementT, typename SubscriptT>
	class SubscriptRandomChooser {

	  private:
		CollectionT& collection;

	  public:
		SubscriptRandomChooser(CollectionT& collection) : collection(collection) {}
		SubscriptRandomChooser(const SubscriptRandomChooser& chooser) : collection(chooser.collection) {}

		ElementT operator()(SubscriptT subscript) {
			return collection[subscript];
		}

		ElementT operator()(SubscriptT subscript) const {
			return collection[subscript];
		}

	};

}}

#endif /* REDSTRAIN_MOD_UTIL_SUBSCRIPTRANDOMCHOOSER_HPP */
