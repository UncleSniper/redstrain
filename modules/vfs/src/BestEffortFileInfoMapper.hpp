#ifndef REDSTRAIN_MOD_VFS_BESTEFFORTFILEINFOMAPPER_HPP
#define REDSTRAIN_MOD_VFS_BESTEFFORTFILEINFOMAPPER_HPP

#include "MapFileInfoMapper.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API BestEffortFileInfoMapper : public MapFileInfoMapper {

	  protected:
		virtual VirtualUserID guessHostUserToVirtual(const HostUserID&) const;
		virtual HostUserID guessVirtualUserToHost(VirtualUserID) const;
		virtual VirtualGroupID guessHostGroupToVirtual(const HostGroupID&) const;
		virtual HostGroupID guessVirtualGroupToHost(VirtualGroupID) const;
		virtual VirtualDeviceID guessHostDeviceToVirtual(const HostDeviceID&) const;
		virtual HostDeviceID guessVirtualDeviceToHost(VirtualDeviceID) const;

	  public:
		BestEffortFileInfoMapper();
		BestEffortFileInfoMapper(const BestEffortFileInfoMapper&);

	};

}}

#endif /* REDSTRAIN_MOD_VFS_BESTEFFORTFILEINFOMAPPER_HPP */
