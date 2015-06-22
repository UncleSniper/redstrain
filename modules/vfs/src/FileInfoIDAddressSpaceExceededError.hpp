#ifndef REDSTRAIN_MOD_VFS_FILEINFOIDADDRESSSPACEEXCEEDEDERROR_HPP
#define REDSTRAIN_MOD_VFS_FILEINFOIDADDRESSSPACEEXCEEDEDERROR_HPP

#include "FileInfoMappingError.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API FileInfoIDAddressSpaceExceededError : public FileInfoMappingError {

	  public:
		FileInfoIDAddressSpaceExceededError(IDType, IDDomain, const std::string&);
		FileInfoIDAddressSpaceExceededError(const FileInfoIDAddressSpaceExceededError&);

		REDSTRAIN_DECLARE_ERROR(FileInfoIDAddressSpaceExceededError)

	};

}}

#endif /* REDSTRAIN_MOD_VFS_FILEINFOIDADDRESSSPACEEXCEEDEDERROR_HPP */
