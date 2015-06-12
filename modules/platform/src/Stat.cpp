#include "Stat.hpp"
#include "Filesystem.hpp"

namespace redengine {
namespace platform {

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS

	const Stat::UserID Stat::NO_USER;
	const Stat::GroupID Stat::NO_GROUP;
	const Stat::DeviceID Stat::NO_DEVICE;

#endif /* OS-specific definitions */

	Stat::Stat() : type(REGULAR_FILE), owner(NO_USER), group(NO_GROUP), device(NO_DEVICE), permissions(0),
			size(static_cast<size_t>(0u)), atime(static_cast<time_t>(0u)),
			mtime(static_cast<time_t>(0u)), ctime(static_cast<time_t>(0u)) {}
	Stat::Stat(const Stat& stat) : type(stat.type), owner(stat.owner), group(stat.group), device(stat.device),
			permissions(stat.permissions), size(stat.size), atime(stat.atime), mtime(stat.mtime),
			ctime(stat.ctime) {}

	int Stat::getAccess() const {
		return getAccess(Filesystem::getCurrentUser(), Filesystem::getCurrentGroup());
	}

	int Stat::getAccess(UserID accessingUser, GroupID accessingGroup) const {
		if(accessingUser == owner)
			return (permissions >> 6) & 7;
		else if(accessingGroup == group)
			return (permissions >> 3) & 7;
		else
			return permissions & 7;
	}

	Stat& Stat::operator=(const Stat& stat) {
		type = stat.type;
		owner = stat.owner;
		group = stat.group;
		device = stat.device;
		permissions = stat.permissions;
		size = stat.size;
		atime = stat.atime;
		mtime = stat.mtime;
		ctime = stat.ctime;
		return *this;
	}

}}
