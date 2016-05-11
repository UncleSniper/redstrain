#ifndef REDSTRAIN_MOD_VFS_UNHANDELEDURISCHEMEERROR_HPP
#define REDSTRAIN_MOD_VFS_UNHANDELEDURISCHEMEERROR_HPP

#include <redstrain/text/types.hpp>

#include "URIError.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API UnhandeledURISchemeError : public URIError {

	  private:
		const text::String16 scheme;

	  public:
		UnhandeledURISchemeError(const std::string&);
		UnhandeledURISchemeError(const text::String16&);
		UnhandeledURISchemeError(const text::String32&);
		UnhandeledURISchemeError(const UnhandeledURISchemeError&);
		virtual ~UnhandeledURISchemeError();

		inline const text::String16& getScheme() const {
			return scheme;
		}

		REDSTRAIN_DECLARE_ERROR(UnhandeledURISchemeError)

	};

}}

#endif /* REDSTRAIN_MOD_VFS_UNHANDELEDURISCHEMEERROR_HPP */
