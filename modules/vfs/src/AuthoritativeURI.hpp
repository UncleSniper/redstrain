#ifndef REDSTRAIN_MOD_VFS_AUTHORITATIVEURI_HPP
#define REDSTRAIN_MOD_VFS_AUTHORITATIVEURI_HPP

#include "URI.hpp"

namespace redengine {
namespace vfs {

	class REDSTRAIN_VFS_API AuthoritativeURI : public virtual URI {

	  protected:
		bool authorityDefined;
		text::String16 username, password, hostname, port;
		uint16_t portNumber;
		std::string usernameOctets, passwordOctets, hostnameOctets, portOctets;

	  protected:
		void parsePort();
		void renderPort();
		void setAuthoritativeOctetsFromRendition(const text::String16&,
				util::MemorySize, util::MemorySize, util::MemorySize, util::MemorySize,
				void (*byteizeChar)(text::Char16, std::string&));

		virtual void authoritativeDecodeOriginal(const std::string&, std::string&) const;
		virtual void authoritativeDecodeOriginal(const std::string&, text::String16&) const;
		virtual void authoritativeDecodeOriginal(const std::string&, text::String32&) const;

	  public:
		AuthoritativeURI(bool);
		AuthoritativeURI(const text::String16&);
		AuthoritativeURI(const text::String16&, const text::String16&);
		AuthoritativeURI(const text::String16&, uint16_t);
		AuthoritativeURI(const text::String16&, const text::String16&, const text::String16&);
		AuthoritativeURI(const text::String16&, const text::String16&, const text::String16&, const text::String16&);
		AuthoritativeURI(const text::String16&, const text::String16&, const text::String16&, uint16_t);
		AuthoritativeURI(const AuthoritativeURI&);
		virtual ~AuthoritativeURI();

		virtual bool hasAuthority() const;
		virtual std::string getRawAuthority8() const;
		virtual text::String16 getRawAuthority16() const;
		virtual text::String32 getRawAuthority32() const;
		virtual std::string getAuthority() const;
		virtual std::string getAuthority8() const;
		virtual text::String16 getAuthority16() const;
		virtual text::String32 getAuthority32() const;

		virtual std::string getRawAuthentication8() const;
		virtual text::String16 getRawAuthentication16() const;
		virtual text::String32 getRawAuthentication32() const;
		virtual std::string getAuthentication() const;
		virtual std::string getAuthentication8() const;
		virtual text::String16 getAuthentication16() const;
		virtual text::String32 getAuthentication32() const;

		virtual std::string getRawUserName8() const;
		virtual text::String16 getRawUserName16() const;
		virtual text::String32 getRawUserName32() const;
		virtual std::string getUserName() const;
		virtual std::string getUserName8() const;
		virtual text::String16 getUserName16() const;
		virtual text::String32 getUserName32() const;

		virtual std::string getRawPassword8() const;
		virtual text::String16 getRawPassword16() const;
		virtual text::String32 getRawPassword32() const;
		virtual std::string getPassword() const;
		virtual std::string getPassword8() const;
		virtual text::String16 getPassword16() const;
		virtual text::String32 getPassword32() const;

		virtual std::string getRawHost8() const;
		virtual text::String16 getRawHost16() const;
		virtual text::String32 getRawHost32() const;
		virtual std::string getHost() const;
		virtual std::string getHost8() const;
		virtual text::String16 getHost16() const;
		virtual text::String32 getHost32() const;

		virtual std::string getRawHostName8() const;
		virtual text::String16 getRawHostName16() const;
		virtual text::String32 getRawHostName32() const;
		virtual std::string getHostName() const;
		virtual std::string getHostName8() const;
		virtual text::String16 getHostName16() const;
		virtual text::String32 getHostName32() const;

		virtual std::string getRawPort8() const;
		virtual text::String16 getRawPort16() const;
		virtual text::String32 getRawPort32() const;
		virtual std::string getPort() const;
		virtual uint16_t getPortNumber() const;

	};

}}

#endif /* REDSTRAIN_MOD_VFS_AUTHORITATIVEURI_HPP */
