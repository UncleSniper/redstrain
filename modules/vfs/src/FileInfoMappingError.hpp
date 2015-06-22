#ifndef REDSTRAIN_MOD_VFS_FILEINFOMAPPINGERROR_HPP
#define REDSTRAIN_MOD_VFS_FILEINFOMAPPINGERROR_HPP

#include <redstrain/error/IOError.hpp>

#include "api.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API FileInfoMappingError : public error::IOError {

	  public:
		enum IDType {
			USER_ID,
			GROUP_ID,
			DEVICE_ID
		};

		enum IDDomain {
			HOST_ID,
			VIRTUAL_ID
		};

	  protected:
		const IDType idType;
		const IDDomain idDomain;
		const std::string id;

	  public:
		FileInfoMappingError(IDType, IDDomain, const std::string&);
		FileInfoMappingError(const FileInfoMappingError&);

		inline IDType getIDType() const {
			return idType;
		}

		inline IDDomain getIDDomain() const {
			return idDomain;
		}

		inline const std::string& getID() const {
			return id;
		}

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(FileInfoMappingError)

	};

}}

#endif /* REDSTRAIN_MOD_VFS_FILEINFOMAPPINGERROR_HPP */
