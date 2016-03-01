#ifndef REDSTRAIN_MOD_ZWEIGANA_CORE_DEFAULTBINARYDISPATCHER_HPP
#define REDSTRAIN_MOD_ZWEIGANA_CORE_DEFAULTBINARYDISPATCHER_HPP

#include <redstrain/util/ClassifyType.hpp>
#include <redstrain/platform/Endianness.hpp>

#include "TrieView.hpp"

namespace redengine {
namespace zweigana {
namespace core {

	template<typename KeyT>
	class DefaultBinaryDispatcher {

	  private:
		template<int, typename, int>
		struct DispatcherImpl;

		template<int Dummy, typename T>
		struct DispatcherImpl<Dummy, T, util::TCLS_INTEGER> {

			template<typename BaseT, typename ReturnT>
			static inline ReturnT withBinaryKey1(BaseT& base, ReturnT (BaseT::*function)(), T key) {
				TrieView::DefaultCopyBinaryKey<T> wrapper(key);
				return (base.*function)(wrapper);
			}

			template<typename BaseT, typename ReturnT>
			static inline ReturnT withBinaryKey1(const BaseT& base, ReturnT (BaseT::*function)() const, T key) {
				TrieView::DefaultCopyBinaryKey<T> wrapper(key);
				return (base.*function)(wrapper);
			}

			template<typename BaseT, typename ReturnT, typename ArgumentT>
			static inline ReturnT withBinaryKey2(BaseT& base, ReturnT (BaseT::*function)(), T key,
					ArgumentT argument) {
				TrieView::DefaultCopyBinaryKey<T> wrapper(key);
				return (base.*function)(wrapper, argument);
			}

			template<typename BaseT, typename ReturnT, typename ArgumentT>
			static inline ReturnT withBinaryKey2(const BaseT& base, ReturnT (BaseT::*function)() const, T key,
					ArgumentT argument) {
				TrieView::DefaultCopyBinaryKey<T> wrapper(key);
				return (base.*function)(wrapper, argument);
			}

		};

	  public:
		typedef DispatcherImpl<0, KeyT, util::ClassifyType<KeyT>::CLASS> Dispatcher;

	};

}}}

#endif /* REDSTRAIN_MOD_ZWEIGANA_CORE_DEFAULTBINARYDISPATCHER_HPP */
