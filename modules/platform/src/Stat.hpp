#ifndef REDSTRAIN_MOD_PLATFORM_STAT_HPP
#define REDSTRAIN_MOD_PLATFORM_STAT_HPP

#include "api.hpp"
#include "platform.hpp"

#include <time.h>
#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS
#include <string>
#include <windows.h>
#endif /* OS-specific includes */

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API Stat {

	  public:
#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX
		typedef uid_t UserID;
		typedef gid_t GroupID;
		typedef dev_t DeviceID;
#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS
		typedef std::string UserID;
		typedef std::string GroupID;
		typedef std::string DeviceID;
#else /* OS not implemented */
#error Platform not supported
#endif /* OS-specific types */

	  public:
#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX
		static const uid_t NO_USER = static_cast<uid_t>(0u);
		static const gid_t NO_GROUP = static_cast<gid_t>(0u);
		static const dev_t NO_DEVICE = static_cast<dev_t>(0u);
#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS
		static const std::string NO_USER;
		static const std::string NO_GROUP;
		static const std::string NO_DEVICE;
#else /* OS not implemented */
#error Platform not supported
#endif /* OS-specific constants */

	  public:
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

		int getAccess() const;
		int getAccess(UserID, GroupID) const;

		Stat& operator=(const Stat&);

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_STAT_HPP */
