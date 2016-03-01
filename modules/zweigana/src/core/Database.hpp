#ifndef REDSTRAIN_MOD_ZWEIGANA_CORE_DATABASE_HPP
#define REDSTRAIN_MOD_ZWEIGANA_CORE_DATABASE_HPP

#include <string>
#include <cstring>
#include <redstrain/platform/File.hpp>
#include <redstrain/platform/Mutex.hpp>
#include <redstrain/util/IntegerBounds.hpp>
#include <redstrain/platform/Endianness.hpp>
#include <redstrain/util/types.hpp>

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX
// only needed in source
#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS
#include <windows.h>
#endif /* OS-specific includes */

#include "../api.hpp"

namespace redengine {
namespace zweigana {
namespace core {

	class DBSemantics;

	class REDSTRAIN_ZWEIGANA_API Database {

	  public:
		enum OpenMode {
			OPEN,
			CREATE,
			OPEN_OR_CREATE
		};

		class REDSTRAIN_ZWEIGANA_API Configurator {

		  public:
			Configurator();
			Configurator(const Configurator&);
			virtual ~Configurator();

			virtual void configureDatabase(Database&) = 0;

		};

	  public:
		static const util::FileSize INVALID_OFFSET = util::IntegerBounds<util::FileSize>::MAX;

	  private:
		platform::File::Handle fileHandle;
		util::FileSize mmapOffset, mmapSize;
		const char* mmapData;
		platform::Mutex writeLock;
		volatile util::FileSize currentSize;

	  public:
		Database(const std::string&, DBSemantics&, OpenMode, Configurator* = NULL);
		Database(const Database&);
		virtual ~Database();

		inline platform::File::Handle getFileHandle() const {
			return fileHandle;
		}

		inline util::FileSize getMMapOffset() const {
			return mmapOffset;
		}

		inline util::FileSize getMMapSize() const {
			return mmapSize;
		}

		inline util::FileSize getCurrentSize() const {
			return currentSize;
		}

		void setMMapRegion(util::FileSize, util::FileSize);

		const char* readBlock(char*, util::FileSize, util::MemorySize) const;
		util::FileSize writeBlock(const char*, util::MemorySize);
		void sync();
		void close();

		template<typename IntT>
		static IntT getInt(const char* buffer) {
			IntT value;
			memcpy(&value, buffer, sizeof(IntT));
			return platform::Endianness<IntT>::convertBig(value);
		}

		template<typename IntT>
		static void putInt(char* buffer, IntT value) {
			value = platform::Endianness<IntT>::convertBig(value);
			memcpy(buffer, &value, sizeof(IntT));
		}

		template<typename FloatT>
		static FloatT getFloat(const char* buffer) {
			FloatT value;
			memcpy(&value, buffer, sizeof(FloatT));
			return value;
		}

		template<typename FloatT>
		static void putFloat(char* buffer, FloatT value) {
			memcpy(buffer, &value, sizeof(FloatT));
		}

	};

}}}

#endif /* REDSTRAIN_MOD_ZWEIGANA_CORE_DATABASE_HPP */
