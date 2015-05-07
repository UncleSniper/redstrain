#ifndef REDSTRAIN_MOD_IO_PATCH_HPP
#define REDSTRAIN_MOD_IO_PATCH_HPP

#include <redstrain/platform/Endianness.hpp>

namespace redengine {
namespace io {

	template<typename ValueT>
	class Patch {

	  private:
		template<int, typename T>
		struct Convert;

#define REDSTRAIN_IO_DEFINE_PATCH_TYPE_ENDIANNESS(type) \
	template<int Dummy> \
	struct Convert<Dummy, type> { \
		static inline type convertBig(type value) { \
			return platform::Endianness<type>::convertBig(value); \
		} \
		static inline type convertLittle(type value) { \
			return platform::Endianness<type>::convertLittle(value); \
		} \
	};
#define REDSTRAIN_IO_DEFINE_PATCH_TYPE_IDENTITY(type) \
	template<int Dummy> \
	struct Convert<Dummy, type> { \
		static inline type convertBig(type value) { \
			return value; \
		} \
		static inline type convertLittle(type value) { \
			return value; \
		} \
	};

		REDSTRAIN_IO_DEFINE_PATCH_TYPE_IDENTITY(int8_t)
		REDSTRAIN_IO_DEFINE_PATCH_TYPE_IDENTITY(uint8_t)
		REDSTRAIN_IO_DEFINE_PATCH_TYPE_ENDIANNESS(int16_t)
		REDSTRAIN_IO_DEFINE_PATCH_TYPE_ENDIANNESS(uint16_t)
		REDSTRAIN_IO_DEFINE_PATCH_TYPE_ENDIANNESS(int32_t)
		REDSTRAIN_IO_DEFINE_PATCH_TYPE_ENDIANNESS(uint32_t)
		REDSTRAIN_IO_DEFINE_PATCH_TYPE_ENDIANNESS(int64_t)
		REDSTRAIN_IO_DEFINE_PATCH_TYPE_ENDIANNESS(uint64_t)
		REDSTRAIN_IO_DEFINE_PATCH_TYPE_IDENTITY(float)
		REDSTRAIN_IO_DEFINE_PATCH_TYPE_IDENTITY(double)

	  private:
		char* address;

	  public:
		Patch(char* address) : address(address) {}
		Patch(const Patch& patch) : address(patch.address) {}

		inline char* getAddress() const {
			return address;
		}

		void patch(ValueT value) {
			*reinterpret_cast<ValueT*>(address) = Convert<0, ValueT>::convertBig(value);
		}

		void patchLE(ValueT value) {
			*reinterpret_cast<ValueT*>(address) = Convert<0, ValueT>::convertLittle(value);
		}

		Patch& operator=(const Patch& patch) {
			address = patch.address;
		}

	};

}}

#endif /* REDSTRAIN_MOD_IO_PATCH_HPP */
