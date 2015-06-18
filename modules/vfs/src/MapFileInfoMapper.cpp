#include <redstrain/platform/MutexLocker.hpp>

#include "MapFileInfoMapper.hpp"

using std::map;
using redengine::platform::Mutex;
using redengine::platform::MutexLocker;

namespace redengine {
namespace vfs {

	MapFileInfoMapper::MapFileInfoMapper() {}

	MapFileInfoMapper::MapFileInfoMapper(const MapFileInfoMapper& mapper) : FileInfoMapper(mapper) {
		MutexLocker lock(const_cast<Mutex&>(mapper.mutex));
		userh2v = mapper.userh2v;
		userv2h = mapper.userv2h;
		grouph2v = mapper.grouph2v;
		groupv2h = mapper.groupv2h;
		deviceh2v = mapper.deviceh2v;
		devicev2h = mapper.devicev2h;
		lock.release();
	}

	template<typename KeyT, typename ValueT>
	struct Eraser {

		map<KeyT, ValueT>* target;
		KeyT key;

		Eraser(map<KeyT, ValueT>* target, const KeyT& key) : target(target), key(key) {}

		~Eraser() {
			if(target)
				target->erase(key);
		}

	};

	void MapFileInfoMapper::putUserMapping(const HostUserID& hostID, VirtualUserID virtualID) {
		MutexLocker lock(mutex);
		UserHostToVirtual::iterator h2vit(userh2v.find(hostID));
		if(h2vit != userh2v.end()) {
			userv2h.erase(h2vit->second);
			userh2v.erase(h2vit);
		}
		UserVirtualToHost::iterator v2hit(userv2h.find(virtualID));
		if(v2hit != userv2h.end()) {
			userh2v.erase(v2hit->second);
			userv2h.erase(v2hit);
		}
		userh2v[hostID] = virtualID;
		Eraser<HostUserID, VirtualUserID> eraser(&userh2v, hostID);
		userv2h[virtualID] = hostID;
		eraser.target = NULL;
		lock.release();
	}

	/*TODO
	bool MapFileInfoMapper::getHostUserID(VirtualUserID, HostUserID&) const;
	bool MapFileInfoMapper::getVirtualUserID(const HostUserID&, VirtualUserID&) const;
	void MapFileInfoMapper::getUserMappings(UserIterator&, UserIterator&) const;
	void MapFileInfoMapper::putGroupMapping(const HostGroupID&, VirtualGroupID);
	bool MapFileInfoMapper::getHostGroupID(VirtualGroupID, HostGroupID&) const;
	bool MapFileInfoMapper::getVirtualGroupID(const HostGroupID&, VirtualGroupID&) const;
	void MapFileInfoMapper::getGroupMappings(GroupIterator&, GroupIterator&) const;
	void MapFileInfoMapper::putDeviceMapping(const HostDeviceID&, VirtualDeviceID);
	bool MapFileInfoMapper::getHostDeviceID(VirtualDeviceID, HostDeviceID&) const;
	bool MapFileInfoMapper::getVirtualDeviceID(const HostDeviceID&, VirtualDeviceID&) const;
	void MapFileInfoMapper::getDeviceMappings(DeviceIterator&, DeviceIterator&) const;
	VirtualUserID MapFileInfoMapper::mapHostUserToVirtual(const HostUserID&);
	HostUserID MapFileInfoMapper::mapVirtualUserToHost(VirtualUserID);
	VirtualGroupID MapFileInfoMapper::mapHostGroupToVirtual(const HostGroupID&);
	HostGroupID MapFileInfoMapper::mapVirtualGroupToHost(VirtualGroupID);
	VirtualDeviceID MapFileInfoMapper::mapHostDeviceToVirtual(const HostDeviceID&);
	HostDeviceID MapFileInfoMapper::mapVirtualDeviceToHost(VirtualDeviceID);
	VirtualUserID MapFileInfoMapper::guessHostUserToVirtual(const HostUserID&) const;
	HostUserID MapFileInfoMapper::guessVirtualUserToHost(VirtualUserID) const;
	VirtualGroupID MapFileInfoMapper::guessHostGroupToVirtual(const HostGroupID&) const;
	HostGroupID MapFileInfoMapper::guessVirtualGroupToHost(VirtualGroupID) const;
	VirtualDeviceID MapFileInfoMapper::guessHostDeviceToVirtual(const HostDeviceID&) const;
	HostDeviceID MapFileInfoMapper::guessVirtualDeviceToHost(VirtualDeviceID) const;
	*/

}}
