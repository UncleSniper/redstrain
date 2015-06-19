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

	bool MapFileInfoMapper::getHostUserID(VirtualUserID virtualID, HostUserID& hostID) {
		MutexLocker lock(mutex);
		UserVirtualToHost::const_iterator it(userv2h.find(virtualID));
		if(it == userv2h.end()) {
			lock.release();
			return false;
		}
		hostID = it->second;
		lock.release();
		return true;
	}

	bool MapFileInfoMapper::getVirtualUserID(const HostUserID& hostID, VirtualUserID& virtualID) {
		MutexLocker lock(mutex);
		UserHostToVirtual::const_iterator it(userh2v.find(hostID));
		if(it == userh2v.end()) {
			lock.release();
			return false;
		}
		virtualID = it->second;
		lock.release();
		return true;
	}

	void MapFileInfoMapper::getUserMappings(UserIterator& begin, UserIterator& end) {
		MutexLocker lock(mutex);
		begin = userh2v.begin();
		end = userh2v.end();
		lock.release();
	}

	void MapFileInfoMapper::putGroupMapping(const HostGroupID& hostID, VirtualGroupID virtualID) {
		MutexLocker lock(mutex);
		GroupHostToVirtual::iterator h2vit(grouph2v.find(hostID));
		if(h2vit != grouph2v.end()) {
			groupv2h.erase(h2vit->second);
			grouph2v.erase(h2vit);
		}
		GroupVirtualToHost::iterator v2hit(groupv2h.find(virtualID));
		if(v2hit != groupv2h.end()) {
			grouph2v.erase(v2hit->second);
			groupv2h.erase(v2hit);
		}
		grouph2v[hostID] = virtualID;
		Eraser<HostGroupID, VirtualGroupID> eraser(&grouph2v, hostID);
		groupv2h[virtualID] = hostID;
		eraser.target = NULL;
		lock.release();
	}

	bool MapFileInfoMapper::getHostGroupID(VirtualGroupID virtualID, HostGroupID& hostID) {
		MutexLocker lock(mutex);
		GroupVirtualToHost::const_iterator it(groupv2h.find(virtualID));
		if(it == groupv2h.end()) {
			lock.release();
			return false;
		}
		hostID = it->second;
		lock.release();
		return true;
	}

	bool MapFileInfoMapper::getVirtualGroupID(const HostGroupID& hostID, VirtualGroupID& virtualID) {
		MutexLocker lock(mutex);
		GroupHostToVirtual::const_iterator it(grouph2v.find(hostID));
		if(it == grouph2v.end()) {
			lock.release();
			return false;
		}
		virtualID = it->second;
		lock.release();
		return true;
	}

	void MapFileInfoMapper::getGroupMappings(GroupIterator& begin, GroupIterator& end) {
		MutexLocker lock(mutex);
		begin = grouph2v.begin();
		end = grouph2v.end();
		lock.release();
	}

	void MapFileInfoMapper::putDeviceMapping(const HostDeviceID& hostID, VirtualDeviceID virtualID) {
		MutexLocker lock(mutex);
		DeviceHostToVirtual::iterator h2vit(deviceh2v.find(hostID));
		if(h2vit != deviceh2v.end()) {
			devicev2h.erase(h2vit->second);
			deviceh2v.erase(h2vit);
		}
		DeviceVirtualToHost::iterator v2hit(devicev2h.find(virtualID));
		if(v2hit != devicev2h.end()) {
			deviceh2v.erase(v2hit->second);
			devicev2h.erase(v2hit);
		}
		deviceh2v[hostID] = virtualID;
		Eraser<HostDeviceID, VirtualDeviceID> eraser(&deviceh2v, hostID);
		devicev2h[virtualID] = hostID;
		eraser.target = NULL;
		lock.release();
	}

	bool MapFileInfoMapper::getHostDeviceID(VirtualDeviceID virtualID, HostDeviceID& hostID) {
		MutexLocker lock(mutex);
		DeviceVirtualToHost::const_iterator it(devicev2h.find(virtualID));
		if(it == devicev2h.end()) {
			lock.release();
			return false;
		}
		hostID = it->second;
		lock.release();
		return true;
	}

	bool MapFileInfoMapper::getVirtualDeviceID(const HostDeviceID& hostID, VirtualDeviceID& virtualID) {
		MutexLocker lock(mutex);
		DeviceHostToVirtual::const_iterator it(deviceh2v.find(hostID));
		if(it == deviceh2v.end()) {
			lock.release();
			return false;
		}
		virtualID = it->second;
		lock.release();
		return true;
	}

	void MapFileInfoMapper::getDeviceMappings(DeviceIterator& begin, DeviceIterator& end) {
		MutexLocker lock(mutex);
		begin = deviceh2v.begin();
		end = deviceh2v.end();
		lock.release();
	}

	MapFileInfoMapper::VirtualUserID MapFileInfoMapper::mapHostUserToVirtual(const HostUserID& hostID) {
		VirtualUserID virtualID;
		MutexLocker lock(mutex);
		UserHostToVirtual::const_iterator h2vit(userh2v.find(hostID));
		if(h2vit != userh2v.end()) {
			virtualID = h2vit->second;
			lock.release();
			return virtualID;
		}
		virtualID = guessHostUserToVirtual(hostID);
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
		return virtualID;
	}

	MapFileInfoMapper::HostUserID MapFileInfoMapper::mapVirtualUserToHost(VirtualUserID virtualID) {
		HostUserID hostID;
		MutexLocker lock(mutex);
		UserVirtualToHost::const_iterator v2hit(userv2h.find(virtualID));
		if(v2hit != userv2h.end()) {
			hostID = v2hit->second;
			lock.release();
			return hostID;
		}
		hostID = guessVirtualUserToHost(virtualID);
		UserHostToVirtual::iterator h2vit(userh2v.find(hostID));
		if(h2vit != userh2v.end()) {
			userv2h.erase(h2vit->second);
			userh2v.erase(h2vit);
		}
		userh2v[hostID] = virtualID;
		Eraser<HostUserID, VirtualUserID> eraser(&userh2v, hostID);
		userv2h[virtualID] = hostID;
		eraser.target = NULL;
		lock.release();
		return hostID;
	}

	/*TODO
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
