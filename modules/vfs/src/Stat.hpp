#ifndef REDSTRAIN_MOD_VFS_STAT_HPP
#define REDSTRAIN_MOD_VFS_STAT_HPP

#include <redstrain/platform/Stat.hpp>
#include <redstrain/util/IntegerBits.hpp>

#include "api.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API Stat {

	  public:
#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX
		typedef util::IntegerBits<uid_t>::AsUnsigned UserID;
		typedef util::IntegerBits<gid_t>::AsUnsigned GroupID;
		typedef util::IntegerBits<dev_t>::AsUnsigned DeviceID;
#else /* !UNIX */
		typedef uint32_t UserID;
		typedef uint32_t GroupID;
		typedef uint32_t DeviceID;
#endif /* OS-specific types */

		enum Type {
			REGULAR_FILE,
			DIRECTORY,
			CHARACTER_DEVICE,
			BLOCK_DEVICE,
			NAMED_PIPE,
			SYMBOLIC_LINK,
			LOCAL_SOCKET
		};

		enum {
			OWNER_READ       = 0400,
			OWNER_WRITE      = 0200,
			OWNER_EXECUTE    = 0100,
			OWNER_MASK       = 0700,
			GROUP_READ       = 0040,
			GROUP_WRITE      = 0020,
			GROUP_EXECUTE    = 0010,
			GROUP_MASK       = 0070,
			OTHERS_READ      = 0004,
			OTHERS_WRITE     = 0002,
			OTHERS_EXECUTE   = 0001,
			OTHERS_MASK      = 0007,
			PERMISSIONS_MASK = 0777
		};

	  public:
		static const UserID NO_USER = static_cast<UserID>(0u);
		static const GroupID NO_GROUP = static_cast<GroupID>(0u);
		static const DeviceID NO_DEVICE = static_cast<DeviceID>(0u);

	  public:
		static const int DEFAULT_DIRECTORY_PERMISSIONS
				= OWNER_MASK | GROUP_READ | GROUP_EXECUTE | OTHERS_READ | OTHERS_EXECUTE;

	  private:
		Type type;
		UserID owner;
		GroupID group;
		DeviceID device, special;
		int permissions;
		size_t size;
		time_t atime, mtime, ctime;

	  public:
		Stat();
		Stat(const Stat&);

		inline Type getType() const {
			return type;
		}

		inline void setType(Type newType) {
			type = newType;
		}

		inline UserID getOwner() const {
			return owner;
		}

		inline void setOwner(UserID newOwner) {
			owner = newOwner;
		}

		inline GroupID getGroup() const {
			return group;
		}

		inline void setGroup(GroupID newGroup) {
			group = newGroup;
		}

		inline DeviceID getDevice() const {
			return device;
		}

		inline void setDevice(DeviceID newDevice) {
			device = newDevice;
		}

		inline DeviceID getSpecialSpecifier() const {
			return special;
		}

		inline void setSpecialSpecifier(DeviceID newSpecial) {
			special = newSpecial;
		}

		inline int getPermissions() const {
			return permissions;
		}

		inline void setPermissions(int newPermissions) {
			permissions = newPermissions & PERMISSIONS_MASK;
		}

		inline size_t getSize() const {
			return size;
		}

		inline void setSize(size_t newSize) {
			size = newSize;
		}

		inline time_t getAccessTimestamp() const {
			return atime;
		}

		inline void setAccessTimestamp(time_t newTimestamp) {
			atime = newTimestamp;
		}

		inline time_t getModificationTimestamp() const {
			return mtime;
		}

		inline void setModificationTimestamp(time_t newTimestamp) {
			mtime = newTimestamp;
		}

		inline time_t getStatusChangeTimestamp() const {
			return ctime;
		}

		inline void setStatusChangeTimestamp(time_t newTimestamp) {
			ctime = newTimestamp;
		}

		Stat& operator=(const Stat&);

	};

}}

#endif /* REDSTRAIN_MOD_VFS_STAT_HPP */
