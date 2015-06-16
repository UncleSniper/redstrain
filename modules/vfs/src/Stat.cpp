#include "Stat.hpp"

namespace redengine {
namespace vfs {

	Stat::Stat() : type(REGULAR_FILE), owner(NO_USER), group(NO_GROUP), device(NO_DEVICE), permissions(0),
			size(static_cast<size_t>(0u)), atime(static_cast<time_t>(0u)),
			mtime(static_cast<time_t>(0u)), ctime(static_cast<time_t>(0u)) {}

	Stat::Stat(const Stat& stat) : type(stat.type), owner(stat.owner), group(stat.group), device(stat.device),
			permissions(stat.permissions), size(stat.size), atime(stat.atime), mtime(stat.mtime),
			ctime(stat.ctime) {}

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
