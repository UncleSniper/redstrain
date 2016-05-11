#ifndef REDSTRAIN_MOD_VFS_INVALIDURIERROR_HPP
#define REDSTRAIN_MOD_VFS_INVALIDURIERROR_HPP

#include <redstrain/error/IllegalArgumentError.hpp>
#include <redstrain/text/types.hpp>

#include "URIError.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API InvalidURIError : public URIError, public error::IllegalArgumentError {

	  private:
		const text::String16 uri;

	  public:
		InvalidURIError(const std::string&);
		InvalidURIError(const text::String16&);
		InvalidURIError(const text::String32&);
		InvalidURIError(const InvalidURIError&);
		virtual ~InvalidURIError();

		inline const text::String16& getURI() const {
			return uri;
		}

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(InvalidURIError)

	};

}}

#endif /* REDSTRAIN_MOD_VFS_INVALIDURIERROR_HPP */
