#ifndef REDSTRAIN_MOD_VFS_UNSUPPORTEDDEVICEFILEOPERATIONERROR_HPP
#define REDSTRAIN_MOD_VFS_UNSUPPORTEDDEVICEFILEOPERATIONERROR_HPP

#include "Stat.hpp"
#include "VFSIOError.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API UnsupportedDeviceFileOperationError : public VFSIOError {

	  public:
		enum Operation {
			TRUNCATE,
			OPEN_FOR_READING,
			OPEN_FOR_WRITING,
			OPEN_FOR_RANDOM_ACCESS
		};

	  private:
		const bool block;
		const Stat::DeviceID device;
		const Operation operation;

	  public:
		UnsupportedDeviceFileOperationError(bool, Stat::DeviceID, Operation);
		UnsupportedDeviceFileOperationError(const UnsupportedDeviceFileOperationError&);

		inline bool isBlockDevice() const {
			return block;
		}

		inline Stat::DeviceID getDevice() const {
			return device;
		}

		inline Operation getOperation() const {
			return operation;
		}

		REDSTRAIN_DECLARE_ERROR(UnsupportedDeviceFileOperationError)

	};

}}

#endif /* REDSTRAIN_MOD_VFS_UNSUPPORTEDDEVICEFILEOPERATIONERROR_HPP */
