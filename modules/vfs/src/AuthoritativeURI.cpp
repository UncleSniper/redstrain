#include <redstrain/text/Formatter.hpp>
#include <redstrain/text/Transcode.hpp>
#include <redstrain/text/UTF8Decoder.hpp>
#include <redstrain/text/UTF16Decoder16.hpp>
#include <redstrain/text/DefaultFormattingOptionStringEmitter.hpp>
#include <redstrain/io/parsenum.hpp>

#include "AuthoritativeURI.hpp"
#include "MalformedURIPortError.hpp"
#include "URIPortOutOfRangeError.hpp"

using std::string;
using redengine::text::Char16;
using redengine::text::Char32;
using redengine::text::String16;
using redengine::text::String32;
using redengine::text::Transcode;
using redengine::text::Formatter;
using redengine::util::MemorySize;
using redengine::text::UTF8Decoder16;
using redengine::text::UTF16Decoder16;
using redengine::io::NumberFormatError;
using redengine::text::FormattingOptions;
using redengine::io::NumberAddressSpaceError;
using redengine::text::DefaultIntegerFormatter;
using redengine::io::DefaultNumberErrorRenderer;
using redengine::io::DefaultNumberCharSemantics;
using redengine::text::DefaultFormattingRendition;
using redengine::text::DefaultFormattingOptionStringEmitter;
using redengine::io::parseInteger;

namespace redengine {
namespace vfs {

	AuthoritativeURI::AuthoritativeURI() : portNumber(static_cast<uint16_t>(0u)) {}

	AuthoritativeURI::AuthoritativeURI(const String16& hostname)
			: hostname(hostname), portNumber(static_cast<uint16_t>(0u)) {}

	AuthoritativeURI::AuthoritativeURI(const String16& hostname, const String16& port)
			: hostname(hostname), port(port) {
		parsePort();
	}

	AuthoritativeURI::AuthoritativeURI(const String16& hostname, uint16_t port)
			: hostname(hostname), portNumber(port) {
		renderPort();
	}

	AuthoritativeURI::AuthoritativeURI(const String16& username, const String16& password, const String16& hostname)
			: username(username), password(password), hostname(hostname), portNumber(static_cast<uint16_t>(0u)) {}

	AuthoritativeURI::AuthoritativeURI(const String16& username, const String16& password,
			const String16& hostname, const String16& port)
			: username(username), password(password), hostname(hostname), port(port) {
		parsePort();
	}

	AuthoritativeURI::AuthoritativeURI(const String16& username, const String16& password,
			const String16& hostname, uint16_t port)
			: username(username), password(password), hostname(hostname), portNumber(port) {
		renderPort();
	}

	AuthoritativeURI::AuthoritativeURI(const AuthoritativeURI& uri) : URI(uri), username(uri.username),
			password(uri.password), hostname(uri.hostname), port(uri.port), portNumber(uri.portNumber),
			usernameOctets(uri.usernameOctets), passwordOctets(uri.passwordOctets),
			hostnameOctets(uri.hostnameOctets), portOctets(uri.portOctets) {}

	AuthoritativeURI::~AuthoritativeURI() {}

	void AuthoritativeURI::parsePort() {
		if(port.empty()) {
			portNumber = static_cast<uint16_t>(0u);
			return;
		}
		try {
			portNumber = parseInteger<
				uint16_t,
				Char16,
				DefaultNumberCharSemantics<Char16, uint16_t>,
				DefaultNumberErrorRenderer<Char16>
			>(port);
		}
		catch(const NumberFormatError&) {
			throw MalformedURIPortError(port);
		}
		catch(const NumberAddressSpaceError&) {
			throw URIPortOutOfRangeError(port);
		}
	}

	void AuthoritativeURI::renderPort() {
		if(!portNumber)
			return;
		port = DefaultIntegerFormatter<Char16>::formatInteger<
			uint16_t,
			DefaultFormattingRendition<Char16>
		>(portNumber, FormattingOptions<Char16>(DefaultFormattingOptionStringEmitter<Char16>::instance));
	}

	void AuthoritativeURI::setAuthoritativeOctetsFromRendition(const String16& fullURI,
			MemorySize usernameOffset, MemorySize passwordOffset, MemorySize hostnameOffset, MemorySize portOffset,
			void (*byteizeChar)(Char16, string&)) {
		URI::unescape<Char16>(fullURI, usernameOffset, username, usernameOctets, byteizeChar);
		URI::unescape<Char16>(fullURI, passwordOffset, password, passwordOctets, byteizeChar);
		URI::unescape<Char16>(fullURI, hostnameOffset, hostname, hostnameOctets, byteizeChar);
		URI::unescape<Char16>(fullURI, portOffset, port, portOctets, byteizeChar);
	}

	void AuthoritativeURI::authoritativeDecodeOriginal(const string& octets, string& original) const {
		original = octets;
	}

	void AuthoritativeURI::authoritativeDecodeOriginal(const string& octets, String16& original) const {
		UTF8Decoder16 utf8;
		Transcode::transcodeString2<char, Char16>(octets, original, utf8);
	}

	void AuthoritativeURI::authoritativeDecodeOriginal(const string& octets, String32& original) const {
		UTF8Decoder16 utf8;
		UTF16Decoder16 utf16;
		Transcode::transcodeString3<char, Char16, Char32>(octets, original, utf8, utf16);
	}

	static const Formatter<char> formatter8;
	static const Formatter<Char16> formatter16;
	static const Formatter<Char32> formatter32;

#define makeFormatStrings(name, fstring) \
	static const string name ## Format8(fstring); \
	static const String16 name ## Format16(Transcode::utf8ToBMP(fstring)); \
	static const String32 name ## Format32(Transcode::utf8ToUnicode(fstring));

	makeFormatStrings(authority, "%?ts{%s%?ts{:%s}}%=2:s%?ts{:%s}")

#define makeGetRawField(method, field) \
	string AuthoritativeURI::getRaw ## method ## 8() const { \
		return Transcode::bmpToUTF8(field); \
	} \
	String16 AuthoritativeURI::getRaw ## method ## 16() const { \
		return field; \
	} \
	String32 AuthoritativeURI::getRaw ## method ## 32() const { \
		UTF16Decoder16 utf16; \
		String32 result; \
		Transcode::transcodeString2<Char16, Char32>(field, result, utf16); \
		return result; \
	}

#define makeGetOctets(method, field) \
	string AuthoritativeURI::get ## method() const { \
		return field ## Octets; \
	}

#define makeGetOriginal(method, bits, stype) \
	stype AuthoritativeURI::get ## method ## bits() const { \
		stype result; \
		authoritativeDecodeOriginal(get ## method(), result); \
		return result; \
	}

#define makeGetRawAuthority(bits, stype) \
	stype AuthoritativeURI::getRawAuthority ## bits() const { \
		if(hostname.empty()) \
			return stype(); \
		return formatter ## bits.format(authorityFormat ## bits, \
				getRawUserName ## bits(), getRawPassword ## bits(), \
				getRawHostName ## bits(), getRawPort ## bits()); \
	}

	makeGetRawAuthority(8, string)
	makeGetRawAuthority(16, String16)
	makeGetRawAuthority(32, String32)

	string AuthoritativeURI::getAuthority() const {
		if(hostnameOctets.empty())
			return string();
		return formatter8.format(authorityFormat8, usernameOctets, passwordOctets, hostnameOctets, portOctets);
	}

	makeGetOriginal(Authority, 8, string)
	makeGetOriginal(Authority, 16, String16)
	makeGetOriginal(Authority, 32, String32)

	makeFormatStrings(authentication, "%s%?ts{:%s}")

#define makeGetRawAuthentication(bits, stype) \
	stype AuthoritativeURI::getRawAuthentication ## bits() const { \
		if(username.empty()) \
			return stype(); \
		return formatter ## bits.format(authenticationFormat ## bits, \
				getRawUserName ## bits(), getRawPassword ## bits()); \
	}

	makeGetRawAuthentication(8, string)
	makeGetRawAuthentication(16, String16)
	makeGetRawAuthentication(32, String32)

	string AuthoritativeURI::getAuthentication() const {
		if(usernameOctets.empty())
			return string();
		return formatter8.format(authenticationFormat8, usernameOctets, passwordOctets);
	}

	makeGetOriginal(Authentication, 8, string)
	makeGetOriginal(Authentication, 16, String16)
	makeGetOriginal(Authentication, 32, String32)

	makeGetRawField(UserName, username)
	makeGetOctets(UserName, username)

	makeGetOriginal(UserName, 8, string)
	makeGetOriginal(UserName, 16, String16)
	makeGetOriginal(UserName, 32, String32)

	makeGetRawField(Password, password)
	makeGetOctets(Password, password)

	makeGetOriginal(Password, 8, string)
	makeGetOriginal(Password, 16, String16)
	makeGetOriginal(Password, 32, String32)

	makeFormatStrings(host, "%s%?ts{:%s}")

#define makeGetRawHost(bits, stype) \
	stype AuthoritativeURI::getRawHost ## bits() const { \
		if(hostname.empty()) \
			return stype(); \
		return formatter ## bits.format(hostFormat ## bits, getRawHostName ## bits(), getRawPort ## bits()); \
	}

	makeGetRawHost(8, string)
	makeGetRawHost(16, String16)
	makeGetRawHost(32, String32)

	string AuthoritativeURI::getHost() const {
		if(hostnameOctets.empty())
			return string();
		return formatter8.format(hostFormat8, hostnameOctets, portOctets);
	}

	makeGetOriginal(Host, 8, string)
	makeGetOriginal(Host, 16, String16)
	makeGetOriginal(Host, 32, String32)

	makeGetRawField(HostName, hostname)
	makeGetOctets(HostName, hostname)

	makeGetOriginal(HostName, 8, string)
	makeGetOriginal(HostName, 16, String16)
	makeGetOriginal(HostName, 32, String32)

	makeGetRawField(Port, port)
	makeGetOctets(Port, port)

	uint16_t AuthoritativeURI::getPortNumber() const {
		return portNumber;
	}

}}
