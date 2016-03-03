#ifndef REDSTRAIN_MOD_ZWEIGANA_CORE_DEFAULTBINARYDISPATCHER_HPP
#define REDSTRAIN_MOD_ZWEIGANA_CORE_DEFAULTBINARYDISPATCHER_HPP

#include <cstring>
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

			static const util::MemorySize BYTE_COUNT = static_cast<util::MemorySize>(sizeof(T));

			static inline void encodeObject(T value, char* buffer) {
				value = platform::Endianness<T>::convertBig(value);
				memcpy(buffer, &value, sizeof(T));
			}

			static inline void decodeObject(const char* buffer, T& value) {
				memcpy(&value, buffer, sizeof(T));
				value = platform::Endianness<T>::convertBig(value);
			}

			template<typename BaseT, typename ReturnT>
			static inline ReturnT withBinaryKey1(BaseT& base,
					ReturnT (BaseT::*function)(TrieView::BinaryKey&), T key) {
				TrieView::DefaultCopyBinaryKey<T> wrapper(platform::Endianness<T>::convertBig(key));
				return (base.*function)(wrapper);
			}

			template<typename BaseT, typename ReturnT>
			static inline ReturnT withBinaryKey1(const BaseT& base,
					ReturnT (BaseT::*function)(TrieView::BinaryKey&) const, T key) {
				TrieView::DefaultCopyBinaryKey<T> wrapper(platform::Endianness<T>::convertBig(key));
				return (base.*function)(wrapper);
			}

			template<typename BaseT, typename ReturnT, typename ArgumentT>
			static inline ReturnT withBinaryKey2(BaseT& base,
					ReturnT (BaseT::*function)(TrieView::BinaryKey&, ArgumentT), T key, ArgumentT argument) {
				TrieView::DefaultCopyBinaryKey<T> wrapper(platform::Endianness<T>::convertBig(key));
				return (base.*function)(wrapper, argument);
			}

			template<typename BaseT, typename ReturnT, typename ArgumentT>
			static inline ReturnT withBinaryKey2(const BaseT& base,
					ReturnT (BaseT::*function)(TrieView::BinaryKey&, ArgumentT) const, T key, ArgumentT argument) {
				TrieView::DefaultCopyBinaryKey<T> wrapper(platform::Endianness<T>::convertBig(key));
				return (base.*function)(wrapper, argument);
			}

		};

		template<int Dummy, typename T>
		struct DispatcherImpl<Dummy, T, util::TCLS_FLOAT> {

			static const util::MemorySize BYTE_COUNT = static_cast<util::MemorySize>(sizeof(T));

			static inline void encodeObject(T value, char* buffer) {
				memcpy(buffer, &value, sizeof(T));
			}

			static inline void decodeObject(const char* buffer, T& value) {
				memcpy(&value, buffer, sizeof(T));
			}

			template<typename BaseT, typename ReturnT>
			static inline ReturnT withBinaryKey1(BaseT& base,
					ReturnT (BaseT::*function)(TrieView::BinaryKey&), T key) {
				TrieView::DefaultCopyBinaryKey<T> wrapper(key);
				return (base.*function)(wrapper);
			}

			template<typename BaseT, typename ReturnT>
			static inline ReturnT withBinaryKey1(const BaseT& base,
					ReturnT (BaseT::*function)(TrieView::BinaryKey&) const, T key) {
				TrieView::DefaultCopyBinaryKey<T> wrapper(key);
				return (base.*function)(wrapper);
			}

			template<typename BaseT, typename ReturnT, typename ArgumentT>
			static inline ReturnT withBinaryKey2(BaseT& base,
					ReturnT (BaseT::*function)(TrieView::BinaryKey&, ArgumentT), T key, ArgumentT argument) {
				TrieView::DefaultCopyBinaryKey<T> wrapper(key);
				return (base.*function)(wrapper, argument);
			}

			template<typename BaseT, typename ReturnT, typename ArgumentT>
			static inline ReturnT withBinaryKey2(const BaseT& base,
					ReturnT (BaseT::*function)(TrieView::BinaryKey&, ArgumentT) const, T key, ArgumentT argument) {
				TrieView::DefaultCopyBinaryKey<T> wrapper(key);
				return (base.*function)(wrapper, argument);
			}

		};

		template<int Dummy>
		struct DispatcherImpl<Dummy, text::String16, util::TCLS_OTHER> {

			template<typename BaseT, typename ReturnT>
			static inline ReturnT withBinaryKey1(BaseT& base,
					ReturnT (BaseT::*function)(TrieView::BinaryKey&), const text::String16& key) {
				TrieView::String16ReferenceBinaryKey wrapper(key);
				return (base.*function)(wrapper);
			}

			template<typename BaseT, typename ReturnT>
			static inline ReturnT withBinaryKey1(const BaseT& base,
					ReturnT (BaseT::*function)(TrieView::BinaryKey&) const, const text::String16& key) {
				TrieView::String16ReferenceBinaryKey wrapper(key);
				return (base.*function)(wrapper);
			}

			template<typename BaseT, typename ReturnT, typename ArgumentT>
			static inline ReturnT withBinaryKey2(BaseT& base,
					ReturnT (BaseT::*function)(TrieView::BinaryKey&, ArgumentT),
					const text::String16& key, ArgumentT argument) {
				TrieView::String16ReferenceBinaryKey wrapper(key);
				return (base.*function)(wrapper, argument);
			}

			template<typename BaseT, typename ReturnT, typename ArgumentT>
			static inline ReturnT withBinaryKey2(const BaseT& base,
					ReturnT (BaseT::*function)(TrieView::BinaryKey&, ArgumentT) const,
					const text::String16& key, ArgumentT argument) {
				TrieView::String16ReferenceBinaryKey wrapper(key);
				return (base.*function)(wrapper, argument);
			}

		};

	  public:
		typedef DispatcherImpl<0, KeyT, util::ClassifyType<KeyT>::CLASS> Dispatcher;

	};

}}}

#endif /* REDSTRAIN_MOD_ZWEIGANA_CORE_DEFAULTBINARYDISPATCHER_HPP */
