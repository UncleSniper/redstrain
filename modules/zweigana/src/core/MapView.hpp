#ifndef REDSTRAIN_MOD_ZWEIGANA_CORE_MAPVIEW_HPP
#define REDSTRAIN_MOD_ZWEIGANA_CORE_MAPVIEW_HPP

#include "DefaultBinaryDispatcher.hpp"

namespace redengine {
namespace zweigana {
namespace core {

	template<
		typename KeyT,
		typename ValueT,
		typename KeyBinaryDispatcherT = typename DefaultBinaryDispatcher<KeyT>::Dispatcher,
		typename ValueBinaryDispatcherT = typename DefaultBinaryDispatcher<ValueT>::Dispatcher
	>
	class MapView : public TrieView {

	  public:
		MapView(Database& dbase, util::FileSize treeOffset)
				: TrieView(dbase, treeOffset, ValueBinaryDispatcherT::BYTE_COUNT) {}

		MapView(Database& dbase) : TrieView(dbase, ValueBinaryDispatcherT::BYTE_COUNT) {}

		bool contains(const KeyT& key) const {
			char valueBuffer[ValueBinaryDispatcherT::BYTE_COUNT];
			return !!KeyBinaryDispatcherT::template withBinaryKey2<TrieView, const char*, char*>(*this,
					&TrieView::getImpl, key, valueBuffer);
		}

		bool get(const KeyT& key, ValueT& value) const {
			char valueBuffer[ValueBinaryDispatcherT::BYTE_COUNT];
			const char* read = KeyBinaryDispatcherT::template withBinaryKey2<TrieView, const char*, char*>(*this,
					&TrieView::getImpl, key, valueBuffer);
			if(!read)
				return false;
			ValueBinaryDispatcherT::decodeObject(read, value);
			return true;
		}

		PutResult put(const KeyT& key, const ValueT& value) {
			char valueBuffer[ValueBinaryDispatcherT::BYTE_COUNT];
			ValueBinaryDispatcherT::encodeObject(value, valueBuffer);
			return KeyBinaryDispatcherT::template withBinaryKey2<TrieView, PutResult, const char*>(*this,
					&TrieView::putImpl, key, valueBuffer);
		}

		bool remove(const KeyT& key) {
			return KeyBinaryDispatcherT::template withBinaryKey1<TrieView, bool>(*this, &TrieView::removeImpl, key);
		}

	};

}}}

#endif /* REDSTRAIN_MOD_ZWEIGANA_CORE_MAPVIEW_HPP */
