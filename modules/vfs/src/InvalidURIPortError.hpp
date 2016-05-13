#ifndef REDSTRAIN_MOD_VFS_INVALIDURIPORTERROR_HPP
#define REDSTRAIN_MOD_VFS_INVALIDURIPORTERROR_HPP

#include <redstrain/error/IllegalArgumentError.hpp>
#include <redstrain/text/types.hpp>

#include "URIError.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API InvalidURIPortError : public URIError, public error::IllegalArgumentError {

	  private:
		const text::String16 portRendition;

	  public:
		InvalidURIPortError(const std::string&);
		InvalidURIPortError(const text::String16&);
		InvalidURIPortError(const text::String32&);
		InvalidURIPortError(const InvalidURIPortError&);
		virtual ~InvalidURIPortError();

		inline const text::String16& getPortRendition() const {
			return portRendition;
		}

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(InvalidURIPortError)

	};

}}

#endif /* REDSTRAIN_MOD_VFS_INVALIDURIPORTERROR_HPP */
