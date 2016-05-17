#ifndef REDSTRAIN_MOD_VFS_URIACQUISITIONERROR_HPP
#define REDSTRAIN_MOD_VFS_URIACQUISITIONERROR_HPP

#include <redstrain/text/types.hpp>

#include "URIError.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API URIAcquisitionError : public URIError {

	  private:
		const text::String16 uri;

	  public:
		URIAcquisitionError(const text::String16&);
		URIAcquisitionError(const URIAcquisitionError&);
		virtual ~URIAcquisitionError();

		inline const text::String16& getURI() const {
			return uri;
		}

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(URIAcquisitionError)

	};

}}

#endif /* REDSTRAIN_MOD_VFS_URIACQUISITIONERROR_HPP */
