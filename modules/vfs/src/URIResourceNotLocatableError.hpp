#ifndef REDSTRAIN_MOD_VFS_URIRESOURCENOTLOCATABLEERROR_HPP
#define REDSTRAIN_MOD_VFS_URIRESOURCENOTLOCATABLEERROR_HPP

#include "URIAcquisitionError.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API URIResourceNotLocatableError : public URIAcquisitionError {

	  public:
		URIResourceNotLocatableError(const text::String16&);
		URIResourceNotLocatableError(const URIResourceNotLocatableError&);

		REDSTRAIN_DECLARE_ERROR(URIResourceNotLocatableError)

	};

}}

#endif /* REDSTRAIN_MOD_VFS_URIRESOURCENOTLOCATABLEERROR_HPP */
