#ifndef REDSTRAIN_MOD_VFS_FILEINFOMAPPER_HPP
#define REDSTRAIN_MOD_VFS_FILEINFOMAPPER_HPP

#include <redstrain/util/ReferenceCounted.hpp>

#include "Stat.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API FileInfoMapper : public util::ReferenceCounted {

	  public:
		typedef platform::Stat::UserID HostUserID;
		typedef Stat::UserID VirtualUserID;
		typedef platform::Stat::GroupID HostGroupID;
		typedef Stat::GroupID VirtualGroupID;
		typedef platform::Stat::DeviceID HostDeviceID;
		typedef Stat::DeviceID VirtualDeviceID;

	  public:
		FileInfoMapper();
		FileInfoMapper(const FileInfoMapper&);

		virtual VirtualUserID mapHostUserToVirtual(const HostUserID&) = 0;
		virtual HostUserID mapVirtualUserToHost(VirtualUserID) = 0;
		virtual VirtualGroupID mapHostGroupToVirtual(const HostGroupID&) = 0;
		virtual HostGroupID mapVirtualGroupToHost(VirtualGroupID) = 0;
		virtual VirtualDeviceID mapHostDeviceToVirtual(const HostDeviceID&) = 0;
		virtual HostDeviceID mapVirtualDeviceToHost(VirtualDeviceID) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_VFS_FILEINFOMAPPER_HPP */
