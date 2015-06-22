#ifndef REDSTRAIN_MOD_VFS_UNMAPPEDFILEINFOIDERROR_HPP
#define REDSTRAIN_MOD_VFS_UNMAPPEDFILEINFOIDERROR_HPP

#include "FileInfoMappingError.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API UnmappedFileInfoIDError : public FileInfoMappingError {

	  public:
		UnmappedFileInfoIDError(IDType, IDDomain, const std::string&);
		UnmappedFileInfoIDError(const UnmappedFileInfoIDError&);

		REDSTRAIN_DECLARE_ERROR(UnmappedFileInfoIDError)

	};

}}

#endif /* REDSTRAIN_MOD_VFS_UNMAPPEDFILEINFOIDERROR_HPP */
