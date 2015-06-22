#include <redstrain/util/StringUtils.hpp>
#include <redstrain/util/ClassifyType.hpp>
#include <redstrain/util/AddressSpace.hpp>

#include "UnmappedFileInfoIDError.hpp"
#include "BestEffortFileInfoMapper.hpp"
#include "FileInfoIDAddressSpaceExceededError.hpp"

using redengine::util::StringUtils;
using redengine::util::ClassifyType;
using redengine::util::AddressSpace;
using redengine::util::TCLS_INTEGER;

namespace redengine {
namespace vfs {

	BestEffortFileInfoMapper::BestEffortFileInfoMapper() {}

	BestEffortFileInfoMapper::BestEffortFileInfoMapper(const BestEffortFileInfoMapper& mapper)
			: MapFileInfoMapper(mapper) {}

	template<
		typename HostID,
		typename VirtualID,
		size_t (MapFileInfoMapper::*SizeFunction)() const,
		FileInfoMappingError::IDType IDType
	>
	struct StrategyDispatcher {

		template<int, int>
		struct ByTypeClass {

			static VirtualID guessHostToVirtual(const BestEffortFileInfoMapper& mapper, const HostID& hostID) {
				size_t count = (mapper.*SizeFunction)();
				if(AddressSpace<VirtualID, size_t>::exceededBy(count))
					throw FileInfoIDAddressSpaceExceededError(IDType, FileInfoMappingError::HOST_ID,
							StringUtils::toString(hostID));
				return static_cast<VirtualID>(count);
			}

			static HostID guessVirtualToHost(VirtualID virtualID) {
				throw UnmappedFileInfoIDError(IDType, FileInfoMappingError::VIRTUAL_ID,
						StringUtils::toString(virtualID));
			}

		};

		template<int Dummy>
		struct ByTypeClass<Dummy, TCLS_INTEGER> {

			static VirtualID guessHostToVirtual(const BestEffortFileInfoMapper&, const HostID& hostID) {
				if(AddressSpace<VirtualID, HostID>::exceededBy(hostID))
					throw FileInfoIDAddressSpaceExceededError(IDType, FileInfoMappingError::HOST_ID,
							StringUtils::toString(hostID));
				return static_cast<VirtualID>(hostID);
			}

			static HostID guessVirtualToHost(VirtualID virtualID) {
				if(AddressSpace<HostID, VirtualID>::exceededBy(virtualID))
					throw FileInfoIDAddressSpaceExceededError(IDType, FileInfoMappingError::VIRTUAL_ID,
							StringUtils::toString(virtualID));
				return static_cast<HostID>(virtualID);
			}

		};

		static VirtualID dispatchHostToVirtual(const BestEffortFileInfoMapper& mapper, const HostID& hostID) {
			return ByTypeClass<0, ClassifyType<HostID>::CLASS>::guessHostToVirtual(mapper, hostID);
		}

		static HostID dispatchVirtualToHost(VirtualID virtualID) {
			return ByTypeClass<0, ClassifyType<HostID>::CLASS>::guessVirtualToHost(virtualID);
		}

	};

	FileInfoMapper::VirtualUserID BestEffortFileInfoMapper::guessHostUserToVirtual(const HostUserID& hostID) const {
		return StrategyDispatcher<
			HostUserID,
			VirtualUserID,
			&MapFileInfoMapper::getUserMappingCount,
			FileInfoMappingError::USER_ID
		>::dispatchHostToVirtual(*this, hostID);
	}

	FileInfoMapper::HostUserID BestEffortFileInfoMapper::guessVirtualUserToHost(VirtualUserID virtualID) const {
		return StrategyDispatcher<
			HostUserID,
			VirtualUserID,
			&MapFileInfoMapper::getUserMappingCount,
			FileInfoMappingError::USER_ID
		>::dispatchVirtualToHost(virtualID);
	}

	FileInfoMapper::VirtualGroupID BestEffortFileInfoMapper::guessHostGroupToVirtual(
			const HostGroupID& hostID) const {
		return StrategyDispatcher<
			HostGroupID,
			VirtualGroupID,
			&MapFileInfoMapper::getGroupMappingCount,
			FileInfoMappingError::GROUP_ID
		>::dispatchHostToVirtual(*this, hostID);
	}

	FileInfoMapper::HostGroupID BestEffortFileInfoMapper::guessVirtualGroupToHost(VirtualGroupID virtualID) const {
		return StrategyDispatcher<
			HostGroupID,
			VirtualGroupID,
			&MapFileInfoMapper::getGroupMappingCount,
			FileInfoMappingError::GROUP_ID
		>::dispatchVirtualToHost(virtualID);
	}

	FileInfoMapper::VirtualDeviceID BestEffortFileInfoMapper::guessHostDeviceToVirtual(
			const HostDeviceID& hostID) const {
		return StrategyDispatcher<
			HostDeviceID,
			VirtualDeviceID,
			&MapFileInfoMapper::getDeviceMappingCount,
			FileInfoMappingError::DEVICE_ID
		>::dispatchHostToVirtual(*this, hostID);
	}

	FileInfoMapper::HostDeviceID BestEffortFileInfoMapper::guessVirtualDeviceToHost(
			VirtualDeviceID virtualID) const {
		return StrategyDispatcher<
			HostDeviceID,
			VirtualDeviceID,
			&MapFileInfoMapper::getDeviceMappingCount,
			FileInfoMappingError::DEVICE_ID
		>::dispatchVirtualToHost(virtualID);
	}

}}
