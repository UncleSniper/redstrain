#ifndef REDSTRAIN_MOD_VFS_MAPFILEINFOMAPPER_HPP
#define REDSTRAIN_MOD_VFS_MAPFILEINFOMAPPER_HPP

#include <map>
#include <redstrain/platform/Mutex.hpp>

#include "FileInfoMapper.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API MapFileInfoMapper : FileInfoMapper {

	  private:
		// Gnnnnn, technically, we can't rely on operator<() being
		// defined for Host*ID, but we "kind of know" this will
		// work, so...
		typedef std::map<HostUserID, VirtualUserID> UserHostToVirtual;
		typedef std::map<VirtualUserID, HostUserID> UserVirtualToHost;
		typedef std::map<HostGroupID, VirtualGroupID> GroupHostToVirtual;
		typedef std::map<VirtualGroupID, HostGroupID> GroupVirtualToHost;
		typedef std::map<HostDeviceID, VirtualDeviceID> DeviceHostToVirtual;
		typedef std::map<VirtualDeviceID, HostDeviceID> DeviceVirtualToHost;

	  public:
		template<typename HostT, typename VirtualT>
		struct Pair {

			HostT hostID;
			VirtualT virtualID;

			Pair(const HostT& hostID, VirtualT virtualID) : hostID(hostID), virtualID(virtualID) {}
			Pair(const Pair& pair) : hostID(pair.hostID), virtualID(pair.virtualID) {}

			Pair& operator=(const Pair& pair) {
				hostID = pair.hostID;
				virtualID = pair.virtualID;
				return *this;
			}

		};

		template<typename HostT, typename VirtualT>
		class Iterator {

		  public:
			typedef typename std::map<HostT, VirtualT>::const_iterator BaseIterator;

		  private:
			BaseIterator iterator;

		  public:
			Iterator() {}
			Iterator(const BaseIterator& iterator) : iterator(iterator) {}
			Iterator(const Iterator& iterator) : iterator(iterator.iterator) {}

			Iterator& operator=(const Iterator& iterator) {
				this->iterator = iterator.iterator;
				return *this;
			}

			Pair<HostT, VirtualT> operator*() const {
				return Pair<HostT, VirtualT>(iterator->first, iterator->second);
			}

			Iterator& operator++() {
				++iterator;
				return *this;
			}

			Iterator operator++(int) {
				return Iterator(iterator++);
			}

		};

		typedef Iterator<HostUserID, VirtualUserID> UserIterator;
		typedef Iterator<HostGroupID, VirtualGroupID> GroupIterator;
		typedef Iterator<HostDeviceID, VirtualDeviceID> DeviceIterator;

	  private:
		platform::Mutex mutex;
		UserHostToVirtual userh2v;
		UserVirtualToHost userv2h;
		GroupHostToVirtual grouph2v;
		GroupVirtualToHost groupv2h;
		DeviceHostToVirtual deviceh2v;
		DeviceVirtualToHost devicev2h;

	  protected:
		virtual VirtualUserID guessHostUserToVirtual(const HostUserID&) const;
		virtual HostUserID guessVirtualUserToHost(VirtualUserID) const;
		virtual VirtualGroupID guessHostGroupToVirtual(const HostGroupID&) const;
		virtual HostGroupID guessVirtualGroupToHost(VirtualGroupID) const;
		virtual VirtualDeviceID guessHostDeviceToVirtual(const HostDeviceID&) const;
		virtual HostDeviceID guessVirtualDeviceToHost(VirtualDeviceID) const;

	  public:
		MapFileInfoMapper();
		MapFileInfoMapper(const MapFileInfoMapper&);

		void putUserMapping(const HostUserID&, VirtualUserID);
		bool getHostUserID(VirtualUserID, HostUserID&) const;
		bool getVirtualUserID(const HostUserID&, VirtualUserID&) const;
		void getUserMappings(UserIterator&, UserIterator&) const;

		void putGroupMapping(const HostGroupID&, VirtualGroupID);
		bool getHostGroupID(VirtualGroupID, HostGroupID&) const;
		bool getVirtualGroupID(const HostGroupID&, VirtualGroupID&) const;
		void getGroupMappings(GroupIterator&, GroupIterator&) const;

		void putDeviceMapping(const HostDeviceID&, VirtualDeviceID);
		bool getHostDeviceID(VirtualDeviceID, HostDeviceID&) const;
		bool getVirtualDeviceID(const HostDeviceID&, VirtualDeviceID&) const;
		void getDeviceMappings(DeviceIterator&, DeviceIterator&) const;

		virtual VirtualUserID mapHostUserToVirtual(const HostUserID&);
		virtual HostUserID mapVirtualUserToHost(VirtualUserID);
		virtual VirtualGroupID mapHostGroupToVirtual(const HostGroupID&);
		virtual HostGroupID mapVirtualGroupToHost(VirtualGroupID);
		virtual VirtualDeviceID mapHostDeviceToVirtual(const HostDeviceID&);
		virtual HostDeviceID mapVirtualDeviceToHost(VirtualDeviceID);

	};

}}

#endif /* REDSTRAIN_MOD_VFS_MAPFILEINFOMAPPER_HPP */
