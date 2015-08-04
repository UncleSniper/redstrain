#ifndef REDSTRAIN_MOD_UTIL_MAPKEYITERATOR_HPP
#define REDSTRAIN_MOD_UTIL_MAPKEYITERATOR_HPP

#include <map>

namespace redengine {
namespace util {

	template<typename KeyT, typename ValueT>
	class MapKeyIterator {

	  public:
		typedef typename std::map<KeyT, ValueT>::const_iterator MapIterator;

	  private:
		MapIterator iterator;

	  public:
		MapKeyIterator() {}
		MapKeyIterator(const MapIterator& iterator) : iterator(iterator) {}
		MapKeyIterator(const MapKeyIterator& iterator) : iterator(iterator.iterator) {}

		inline const MapIterator& getMapIterator() const {
			return iterator;
		}

		const KeyT& operator*() const {
			return iterator->first;
		}

		MapKeyIterator& operator++() {
			++iterator;
			return *this;
		}

		MapKeyIterator operator++(int) {
			MapIterator it(iterator);
			++iterator;
			return MapKeyIterator(it);
		}

		bool operator==(const MapKeyIterator& other) const {
			return iterator == other.iterator;
		}

		bool operator!=(const MapKeyIterator& other) const {
			return iterator != other.iterator;
		}

		MapKeyIterator& operator=(const MapKeyIterator& other) {
			iterator = other.iterator;
			return *this;
		}

	};

}}

#endif /* REDSTRAIN_MOD_UTIL_MAPKEYITERATOR_HPP */
