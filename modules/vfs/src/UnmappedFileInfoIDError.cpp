#include <redstrain/util/StringUtils.hpp>
#include <redstrain/error/ProgrammingError.hpp>

#include "UnmappedFileInfoIDError.hpp"

using std::string;
using redengine::util::StringUtils;
using redengine::error::ProgrammingError;

namespace redengine {
namespace vfs {

	UnmappedFileInfoIDError::UnmappedFileInfoIDError(IDType idType, IDDomain idDomain, const string& id)
			: FileInfoMappingError(idType, idDomain, id) {}

	UnmappedFileInfoIDError::UnmappedFileInfoIDError(const UnmappedFileInfoIDError& error)
			: Error(error), FileInfoMappingError(error) {}

	REDSTRAIN_DEFINE_ERROR(UnmappedFileInfoIDError) {
		out << "Virtual filesystem ";
		switch(idDomain) {
			case HOST_ID:
				out << "host ";
				break;
			case VIRTUAL_ID:
				out << "virtual ";
				break;
			default:
				throw ProgrammingError("Unrecognized file info ID domain: "
						+ StringUtils::toString(static_cast<int>(idDomain)));
		}
		switch(idType) {
			case USER_ID:
				out << "user";
				break;
			case GROUP_ID:
				out << "group";
				break;
			case DEVICE_ID:
				out << "device";
				break;
			default:
				throw ProgrammingError("Unrecognized file info ID type: "
						+ StringUtils::toString(static_cast<int>(idType)));
		}
		out << " ID not mapped";
	}

}}
