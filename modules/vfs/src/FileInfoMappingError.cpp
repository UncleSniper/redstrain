#include "FileInfoMappingError.hpp"

using std::string;

namespace redengine {
namespace vfs {

	FileInfoMappingError::FileInfoMappingError(IDType idType, IDDomain idDomain, const string& id)
			: idType(idType), idDomain(idDomain), id(id) {}

	FileInfoMappingError::FileInfoMappingError(const FileInfoMappingError& error)
			: Error(error), IOError(error), idType(error.idType), idDomain(error.idDomain), id(error.id) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(FileInfoMappingError)

}}
