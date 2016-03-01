#ifndef REDSTRAIN_MOD_ZWEIGANA_CORE_SETVIEW_HPP
#define REDSTRAIN_MOD_ZWEIGANA_CORE_SETVIEW_HPP

#include "DefaultBinaryDispatcher.hpp"

namespace redengine {
namespace zweigana {
namespace core {

	template<
		typename KeyT,
		typename KeyBinaryDispatcherT = typename DefaultBinaryDispatcher<KeyT>::Dispatcher
	>
	class SetView : public TrieView {

	  public:
		SetView(Database& dbase, util::FileSize treeOffset)
				: TrieView(dbase, treeOffset, static_cast<util::MemorySize>(0u)) {}

		SetView(Database& dbase) : TrieView(dbase, static_cast<util::MemorySize>(0u)) {}

		bool contains(const KeyT& key) const {
			char c;
			return !!KeyBinaryDispatcherT::template withBinaryKey2<TrieView, const char*, char*>(*this,
					&TrieView::getImpl, key, &c);
		}

		bool insert(const KeyT& key) {
			return KeyBinaryDispatcherT::template withBinaryKey2<TrieView, PutResult, const char*>(*this,
					&TrieView::putImpl, key, NULL) != NOT_MODIFIED;
		}

		bool remove(const KeyT& key) {
			return KeyBinaryDispatcherT::template withBinaryKey1<TrieView, bool>(*this, &TrieView::removeImpl, key);
		}

	};

}}}

#endif /* REDSTRAIN_MOD_ZWEIGANA_CORE_SETVIEW_HPP */
