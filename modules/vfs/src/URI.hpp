#ifndef REDSTRAIN_MOD_VFS_URI_HPP
#define REDSTRAIN_MOD_VFS_URI_HPP

#include <stdint.h>
#include <redstrain/io/InputStream.hpp>
#include <redstrain/text/types.hpp>

#include "URIAcquisition.hpp"
#include "api.hpp"

namespace redengine {
namespace vfs {

	class VFS;

	class REDSTRAIN_VFS_API URI {

	  public:
		URI();
		URI(const URI&);
		virtual ~URI();

		virtual std::string getScheme() const = 0;
		virtual text::String16 getScheme16() const = 0;
		virtual text::String32 getScheme32() const = 0;

		virtual std::string getRawSchemeSpecificPart() const = 0;
		virtual text::String16 getRawSchemeSpecificPart16() const = 0;
		virtual text::String32 getRawSchemeSpecificPart32() const = 0;
		virtual std::string getSchemeSpecificPart() const = 0;
		virtual text::String16 getSchemeSpecificPart16() const = 0;
		virtual text::String32 getSchemeSpecificPart32() const = 0;

		virtual std::string getRawAuthority() const = 0;
		virtual text::String16 getRawAuthority16() const = 0;
		virtual text::String32 getRawAuthority32() const = 0;
		virtual std::string getAuthority() const = 0;
		virtual text::String16 getAuthority16() const = 0;
		virtual text::String32 getAuthority32() const = 0;

		virtual std::string getRawAuthentication() const = 0;
		virtual text::String16 getRawAuthentication16() const = 0;
		virtual text::String32 getRawAuthentication32() const = 0;
		virtual std::string getAuthentication() const = 0;
		virtual text::String16 getAuthentication16() const = 0;
		virtual text::String32 getAuthentication32() const = 0;

		virtual std::string getRawUserName() const = 0;
		virtual text::String16 getRawUserName16() const = 0;
		virtual text::String32 getRawUserName32() const = 0;
		virtual std::string getUserName() const = 0;
		virtual text::String16 getUserName16() const = 0;
		virtual text::String32 getUserName32() const = 0;

		virtual std::string getRawPassword() const = 0;
		virtual text::String16 getRawPassword16() const = 0;
		virtual text::String32 getRawPassword32() const = 0;
		virtual std::string getPassword() const = 0;
		virtual text::String16 getPassword16() const = 0;
		virtual text::String32 getPassword32() const = 0;

		virtual std::string getRawHost() const = 0;
		virtual text::String16 getRawHost16() const = 0;
		virtual text::String32 getRawHost32() const = 0;
		virtual std::string getHost() const = 0;
		virtual text::String16 getHost16() const = 0;
		virtual text::String32 getHost32() const = 0;

		virtual uint16_t getPort() const = 0;

		virtual std::string getRawPath() const = 0;
		virtual text::String16 getRawPath16() const = 0;
		virtual text::String32 getRawPath32() const = 0;
		virtual std::string getPath() const = 0;
		virtual text::String16 getPath16() const = 0;
		virtual text::String32 getPath32() const = 0;

		virtual std::string getRawQuery() const = 0;
		virtual text::String16 getRawQuery16() const = 0;
		virtual text::String32 getRawQuery32() const = 0;
		virtual std::string getQuery() const = 0;
		virtual text::String16 getQuery16() const = 0;
		virtual text::String32 getQuery32() const = 0;

		virtual std::string getRawFragment() const = 0;
		virtual text::String16 getRawFragment16() const = 0;
		virtual text::String32 getRawFragment32() const = 0;
		virtual std::string getFragment() const = 0;
		virtual text::String16 getFragment16() const = 0;
		virtual text::String32 getFragment32() const = 0;

		virtual std::string toString() const = 0;
		virtual text::String16 toString16() const = 0;
		virtual text::String32 toString32() const = 0;

		virtual URI* normalize() const = 0;
		virtual URI* relativize(const URI&) const = 0;
		virtual URI* resolve(const URI&) const = 0;

		virtual URIAcquisition<io::InputStream<char> >* acquireResource() const = 0;
		virtual URIAcquisition<VFS>* acquireVFS() const = 0;

	};

}}

#endif /* REDSTRAIN_MOD_VFS_URI_HPP */
