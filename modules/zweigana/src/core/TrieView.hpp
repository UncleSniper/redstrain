#ifndef REDSTRAIN_MOD_ZWEIGANA_CORE_TRIEVIEW_HPP
#define REDSTRAIN_MOD_ZWEIGANA_CORE_TRIEVIEW_HPP

#include <cstddef>
#include <redstrain/error/IndexOutOfBoundsError.hpp>
#include <redstrain/text/types.hpp>

#include "../api.hpp"

namespace redengine {
namespace zweigana {
namespace core {

	class Database;

	class REDSTRAIN_ZWEIGANA_API TrieView {

	  public:
		enum PutResult {
			NOT_MODIFIED,
			CREATED,
			OVERWRITTEN
		};

		class REDSTRAIN_ZWEIGANA_API BinaryKey {

		  public:
			BinaryKey();
			BinaryKey(const BinaryKey&);
			virtual ~BinaryKey();

			virtual util::MemorySize getKeyByteCount() = 0;
			virtual char getKeyByte(util::MemorySize) = 0;

		};

		template<typename T>
		class DefaultReferenceBinaryKey : public BinaryKey {

		  private:
			const char* object;

		  public:
			DefaultReferenceBinaryKey(const T& object) : object(reinterpret_cast<const char*>(&object)) {}

			DefaultReferenceBinaryKey(const DefaultReferenceBinaryKey& key) : BinaryKey(key), object(key.object) {}

			virtual util::MemorySize getKeyByteCount() {
				return static_cast<util::MemorySize>(sizeof(T));
			}

			virtual char getKeyByte(util::MemorySize index) {
				if(index >= static_cast<util::MemorySize>(sizeof(T)))
					throw error::IndexOutOfBoundsError(index);
				return object[index];
			}

		};

		template<typename T>
		class DefaultCopyBinaryKey : public BinaryKey {

		  private:
			const T object;

		  public:
			DefaultCopyBinaryKey(const T& object) : object(object) {}

			DefaultCopyBinaryKey(const DefaultCopyBinaryKey& key) : BinaryKey(key), object(key.object) {}

			virtual ~DefaultCopyBinaryKey() {}

			inline const T& getObject() const {
				return object;
			}

			virtual util::MemorySize getKeyByteCount() {
				return static_cast<util::MemorySize>(sizeof(T));
			}

			virtual char getKeyByte(util::MemorySize index) {
				if(index >= static_cast<util::MemorySize>(sizeof(T)))
					throw error::IndexOutOfBoundsError(index);
				return reinterpret_cast<const char*>(&object)[index];
			}

		};

		class REDSTRAIN_ZWEIGANA_API String16ReferenceBinaryKey : public BinaryKey {

		  private:
			const text::String16& string;

		  public:
			String16ReferenceBinaryKey(const text::String16&);
			String16ReferenceBinaryKey(const String16ReferenceBinaryKey&);

			inline const text::String16& getString() const {
				return string;
			}

			virtual util::MemorySize getKeyByteCount();
			virtual char getKeyByte(util::MemorySize);

		};

		class REDSTRAIN_ZWEIGANA_API String16CopyBinaryKey : public BinaryKey {

		  private:
			const text::String16 string;

		  public:
			String16CopyBinaryKey(const text::String16&);
			String16CopyBinaryKey(const String16CopyBinaryKey&);

			inline const text::String16& getString() const {
				return string;
			}

			virtual util::MemorySize getKeyByteCount();
			virtual char getKeyByte(util::MemorySize);

		};

	  private:
		Database& dbase;
		util::FileSize treeOffset;
		util::MemorySize valueSize;
		util::FileSize rootOffset;
		util::FileSize elementCount;

	  protected:
		const char* getImpl(BinaryKey&, char*) const;
		PutResult putImpl(BinaryKey&, const char*);
		bool removeImpl(BinaryKey&);

	  public:
		TrieView(Database&, util::FileSize, util::MemorySize);
		TrieView(Database&, util::MemorySize);
		TrieView(const TrieView&);
		virtual ~TrieView();

		inline Database& getDatabase() {
			return dbase;
		}

		inline const Database& getDatabase() const {
			return dbase;
		}

		inline util::FileSize getTreeOffset() const {
			return treeOffset;
		}

		inline util::MemorySize getValueSize() const {
			return valueSize;
		}

		inline util::FileSize getElementCount() const {
			return elementCount;
		}

		void clear();

	};

}}}

#endif /* REDSTRAIN_MOD_ZWEIGANA_CORE_TRIEVIEW_HPP */
