#include <redstrain/text/Transcode.hpp>
#include <redstrain/util/StringUtils.hpp>
#include <redstrain/text/UTF8Encoder.hpp>
#include <redstrain/text/UTF8Decoder.hpp>
#include <redstrain/text/UTF16Encoder16.hpp>
#include <redstrain/text/UTF16Decoder16.hpp>

#include "HierarchicalURI.hpp"

using std::string;
using redengine::text::Char16;
using redengine::text::Char32;
using redengine::text::String16;
using redengine::text::String32;
using redengine::text::Transcode;
using redengine::util::StringUtils;
using redengine::text::UTF8Encoder16;
using redengine::text::UTF8Decoder16;
using redengine::text::UTF16Encoder16;
using redengine::text::UTF16Decoder16;

namespace redengine {
namespace vfs {

	HierarchicalURI::Decomposition::Decomposition() : definedComponents(0) {}

	HierarchicalURI::Decomposition::Decomposition(const Decomposition& decomposition)
			: definedComponents(decomposition.definedComponents),
			schemeEnd(decomposition.schemeEnd),
			authorityStart(decomposition.authorityStart), authorityEnd(decomposition.authorityEnd),
			usernameStart(decomposition.usernameStart), usernameEnd(decomposition.usernameEnd),
			passwordStart(decomposition.passwordStart), passwordEnd(decomposition.passwordEnd),
			pathStart(decomposition.pathStart), pathEnd(decomposition.pathEnd),
			queryStart(decomposition.queryStart), queryEnd(decomposition.queryEnd),
			fragmentStart(decomposition.fragmentStart), fragmentEnd(decomposition.fragmentEnd) {}

	HierarchicalURI::HierarchicalURI(int definedComponents) : AbstractURI(),
			AuthoritativeURI(!!(definedComponents & Decomposition::HAS_AUTHORITY)),
			GenericPathedURI(), QueriedURI(!!(definedComponents & Decomposition::HAS_QUERY)),
			FragmentedURI(!!(definedComponents & Decomposition::HAS_FRAGMENT)) {}

	HierarchicalURI::HierarchicalURI(const String16& uri, const Decomposition& decomposition) : AbstractURI(),
			AuthoritativeURI(!!(decomposition.definedComponents & Decomposition::HAS_AUTHORITY)),
			GenericPathedURI(), QueriedURI(!!(decomposition.definedComponents & Decomposition::HAS_QUERY)),
			FragmentedURI(!!(decomposition.definedComponents & Decomposition::HAS_FRAGMENT)) {
		scheme = Transcode::bmpToUTF8(uri.substr(static_cast<String16::size_type>(0u), decomposition.schemeEnd));
		if(decomposition.definedComponents & Decomposition::HAS_AUTHORITY) {
			if(decomposition.definedComponents & Decomposition::HAS_USERNAME) {
				username = uri.substr(decomposition.usernameStart,
						decomposition.usernameEnd - decomposition.usernameStart);
				if(decomposition.definedComponents & Decomposition::HAS_PASSWORD)
					password = uri.substr(decomposition.passwordStart,
							decomposition.passwordEnd - decomposition.passwordStart);
			}
			hostname = uri.substr(decomposition.hostnameStart,
					decomposition.hostnameEnd - decomposition.hostnameStart);
			if(decomposition.definedComponents & Decomposition::HAS_PORT) {
				port = uri.substr(decomposition.portStart, decomposition.portEnd - decomposition.portStart);
				if(!port.empty())
					parsePort();
			}
			setAuthoritativeOctetsFromRendition(uri, decomposition.usernameStart, decomposition.passwordStart,
					decomposition.hostnameStart, decomposition.portStart, URI::byteizeCharUTF8);
		}
		path = uri.substr(decomposition.pathStart, decomposition.pathEnd - decomposition.pathStart);
		setPathedOctetsFromRendition(uri, decomposition.pathStart, URI::byteizeCharUTF8);
		if(decomposition.definedComponents & Decomposition::HAS_QUERY) {
			query = uri.substr(decomposition.queryStart, decomposition.queryEnd - decomposition.queryStart);
			setQueriedOctetsFromRendition(uri, decomposition.queryStart, URI::byteizeCharUTF8);
		}
		if(decomposition.definedComponents & Decomposition::HAS_FRAGMENT) {
			fragment = uri.substr(decomposition.fragmentStart,
					decomposition.fragmentEnd - decomposition.fragmentStart);
			setFragmentedOctetsFromRendition(uri, decomposition.fragmentStart, URI::byteizeCharUTF8);
		}
	}

	HierarchicalURI::HierarchicalURI(const Components<Char16, CL_RENDITION>& components) : AbstractURI(),
			AuthoritativeURI(!!(components.definedComponents & Decomposition::HAS_AUTHORITY)),
			GenericPathedURI(), QueriedURI(!!(components.definedComponents & Decomposition::HAS_QUERY)),
			FragmentedURI(!!(components.definedComponents & Decomposition::HAS_FRAGMENT)) {
		setByRendition(components);
	}

#define makeOriginalCtor(ctype) \
	HierarchicalURI::HierarchicalURI(const Components<ctype, CL_ORIGINAL>& components) : AbstractURI(), \
			AuthoritativeURI(!!(components.definedComponents & Decomposition::HAS_AUTHORITY)), \
			GenericPathedURI(), QueriedURI(!!(components.definedComponents & Decomposition::HAS_QUERY)), \
			FragmentedURI(!!(components.definedComponents & Decomposition::HAS_FRAGMENT)) { \
		setByOriginal(components); \
	} \
	void HierarchicalURI::setByOriginal(const Components<ctype, CL_ORIGINAL>& components) { \
		scheme = components.scheme; \
		if(components.definedComponents & Decomposition::HAS_AUTHORITY) { \
			if(components.definedComponents & Decomposition::HAS_USERNAME) { \
				hierarchicalEncodeOriginal(components.username, usernameOctets); \
				hierarchicalDecodeRendition(usernameOctets, username); \
				if(components.definedComponents & Decomposition::HAS_PASSWORD) { \
					hierarchicalEncodeOriginal(components.password, passwordOctets); \
					hierarchicalDecodeRendition(passwordOctets, password); \
				} \
			} \
			hierarchicalEncodeOriginal(components.hostname, hostnameOctets); \
			hierarchicalDecodeRendition(hostnameOctets, hostname); \
			if(components.definedComponents & Decomposition::HAS_PORT) { \
				portNumber = components.port; \
				if(portNumber) { \
					portOctets = StringUtils::toString(components.port); \
					hierarchicalDecodeRendition(portOctets, port); \
				} \
			} \
		} \
		hierarchicalEncodeOriginal(components.path, pathOctets); \
		hierarchicalDecodeRendition(pathOctets, path); \
		if(components.definedComponents & Decomposition::HAS_QUERY) { \
			hierarchicalEncodeOriginal(components.query, queryOctets); \
			hierarchicalDecodeRendition(queryOctets, query); \
		} \
		if(components.definedComponents & Decomposition::HAS_FRAGMENT) { \
			hierarchicalEncodeOriginal(components.fragment, fragmentOctets); \
			hierarchicalDecodeRendition(fragmentOctets, fragment); \
		} \
	}

	makeOriginalCtor(char)
	makeOriginalCtor(Char16)
	makeOriginalCtor(Char32)

	HierarchicalURI::HierarchicalURI(const HierarchicalURI& uri) : URI(uri), AbstractURI(uri),
			AuthoritativeURI(uri), GenericPathedURI(uri), QueriedURI(uri), FragmentedURI(uri) {}

	void HierarchicalURI::setByRendition(const Components<Char16, CL_RENDITION>& components) {
		String16 uri;
		Decomposition decomposition;
		decomposition.definedComponents = components.definedComponents;
		components.toURIRendition(uri, URI::renderUTF8, decomposition);
		scheme = components.scheme;
		if(components.definedComponents & Decomposition::HAS_AUTHORITY) {
			if(components.definedComponents & Decomposition::HAS_USERNAME) {
				username = components.username;
				if(components.definedComponents & Decomposition::HAS_PASSWORD)
					password = components.password;
			}
			hostname = components.hostname;
			if(components.definedComponents & Decomposition::HAS_PORT) {
				port = components.port;
				if(!port.empty())
					parsePort();
			}
			setAuthoritativeOctetsFromRendition(uri, decomposition.usernameStart, decomposition.passwordStart,
					decomposition.hostnameStart, decomposition.portStart, URI::byteizeCharUTF8);
		}
		path = components.path;
		setPathedOctetsFromRendition(uri, decomposition.pathStart, URI::byteizeCharUTF8);
		if(components.definedComponents & Decomposition::HAS_QUERY) {
			query = components.query;
			setQueriedOctetsFromRendition(uri, decomposition.queryStart, URI::byteizeCharUTF8);
		}
		if(components.definedComponents & Decomposition::HAS_FRAGMENT) {
			fragment = components.fragment;
			setFragmentedOctetsFromRendition(uri, decomposition.fragmentStart, URI::byteizeCharUTF8);
		}
	}

#define redirectDecodeOriginal(base, stype) \
	void HierarchicalURI::base ## DecodeOriginal(const string& octets, stype& original) const { \
		hierarchicalDecodeOriginal(octets, original); \
	}

#define redirectAllDecodeOriginal(base) \
	redirectDecodeOriginal(base, string) \
	redirectDecodeOriginal(base, String16) \
	redirectDecodeOriginal(base, String32)

	redirectAllDecodeOriginal(authoritative)
	redirectAllDecodeOriginal(pathed)
	redirectAllDecodeOriginal(queried)
	redirectAllDecodeOriginal(fragmented)

	void HierarchicalURI::hierarchicalDecodeOriginal(const string& octets, string& original) const {
		original = octets;
	}

	void HierarchicalURI::hierarchicalDecodeOriginal(const string& octets, String16& original) const {
		UTF8Decoder16 utf8;
		Transcode::transcodeString2<char, Char16>(octets, original, utf8);
	}

	void HierarchicalURI::hierarchicalDecodeOriginal(const string& octets, String32& original) const {
		UTF8Decoder16 utf8;
		UTF16Decoder16 utf16;
		Transcode::transcodeString3<char, Char16, Char32>(octets, original, utf8, utf16);
	}

	void HierarchicalURI::renderRendition(String16& uri, bool withScheme) const {
		typedef String16::size_type Index;
		Index length = !withScheme || scheme.empty() ? static_cast<Index>(0u)
				: static_cast<Index>(scheme.length()) + static_cast<Index>(1u);
		if(authorityDefined) {
			length += static_cast<Index>(2u) + hostname.length();
			if(!username.empty()) {
				length += username.length() + static_cast<Index>(1u);
				if(!password.empty())
					length += static_cast<Index>(1u) + password.length();
			}
			if(!port.empty())
				length += static_cast<Index>(1u) + port.length();
		}
		length += path.length();
		if(queryDefined)
			length += static_cast<Index>(1u) + query.length();
		if(fragmentDefined)
			length += static_cast<Index>(1u) + fragment.length();
		uri.clear();
		uri.reserve(length);
		if(withScheme && !scheme.empty()) {
			URI::renderUTF8(scheme, uri);
			uri += static_cast<Char16>(':');
		}
		if(authorityDefined) {
			uri += static_cast<Char16>('/');
			uri += static_cast<Char16>('/');
			if(!username.empty()) {
				uri += username;
				if(!password.empty()) {
					uri += static_cast<Char16>(':');
					uri += password;
				}
				uri += static_cast<Char16>('@');
			}
			uri += hostname;
			if(!port.empty()) {
				uri += static_cast<Char16>(':');
				uri += port;
			}
		}
		uri += path;
		if(queryDefined) {
			uri += static_cast<Char16>('?');
			uri += query;
		}
		if(fragmentDefined) {
			uri += static_cast<Char16>('#');
			uri += fragment;
		}
	}

	void HierarchicalURI::renderOctets(string& uri, bool withScheme) const {
		typedef string::size_type Index;
		Index length = !withScheme || scheme.empty() ? static_cast<Index>(0u)
				: scheme.length() + static_cast<Index>(1u);
		if(authorityDefined) {
			length += static_cast<Index>(2u) + hostnameOctets.length();
			if(!usernameOctets.empty()) {
				length += usernameOctets.length() + static_cast<Index>(1u);
				if(!passwordOctets.empty())
					length += static_cast<Index>(1u) + passwordOctets.length();
			}
			if(!portOctets.empty())
				length += static_cast<Index>(1u) + portOctets.length();
		}
		length += pathOctets.length();
		if(queryDefined)
			length += static_cast<Index>(1u) + queryOctets.length();
		if(fragmentDefined)
			length += static_cast<Index>(1u) + fragmentOctets.length();
		uri.clear();
		uri.reserve(length);
		if(withScheme && !scheme.empty()) {
			uri += scheme;
			uri += ':';
		}
		if(authorityDefined) {
			uri += "//";
			if(!usernameOctets.empty()) {
				uri += usernameOctets;
				if(!passwordOctets.empty()) {
					uri += ':';
					uri += passwordOctets;
				}
				uri += '@';
			}
			uri += hostnameOctets;
			if(!portOctets.empty()) {
				uri += ':';
				uri += portOctets;
			}
		}
		uri += pathOctets;
		if(queryDefined) {
			uri += '?';
			uri += queryOctets;
		}
		if(fragmentDefined) {
			uri += '#';
			uri += fragmentOctets;
		}
	}

	bool HierarchicalURI::isSchemeAndAuthorityEqual(const URI& uri) const {
		if(scheme != uri.getScheme8())
			return false;
		if(!authorityDefined)
			return !uri.hasAuthority();
		if(!uri.hasAuthority())
			return false;
		return usernameOctets == uri.getUserName() && passwordOctets == uri.getPassword()
				&& hostnameOctets == uri.getHostName() && portOctets == uri.getPort();
	}

	void HierarchicalURI::hierarchicalEncodeOriginal(const string& original, string& octets) const {
		octets = original;
	}

	void HierarchicalURI::hierarchicalEncodeOriginal(const String16& original, string& octets) const {
		UTF8Encoder16 utf8;
		Transcode::transcodeString2<Char16, char>(original, octets, utf8);
	}

	void HierarchicalURI::hierarchicalEncodeOriginal(const String32& original, string& octets) const {
		UTF8Encoder16 utf8;
		UTF16Encoder16 utf16;
		Transcode::transcodeString3<Char32, Char16, char>(original, octets, utf16, utf8);
	}

	void HierarchicalURI::hierarchicalDecodeRendition(const string& octets, String16& rendition) const {
		UTF8Decoder16 utf8;
		Transcode::transcodeString2<char, Char16>(octets, rendition, utf8);
	}

	string HierarchicalURI::getRawSchemeSpecificPart8() const {
		String16 ssp16;
		renderRendition(ssp16, false);
		string ssp8;
		UTF8Encoder16 utf8;
		Transcode::transcodeString2<Char16, char>(ssp16, ssp8, utf8);
		return ssp8;
	}

	String16 HierarchicalURI::getRawSchemeSpecificPart16() const {
		String16 ssp16;
		renderRendition(ssp16, false);
		return ssp16;
	}

	String32 HierarchicalURI::getRawSchemeSpecificPart32() const {
		String16 ssp16;
		renderRendition(ssp16, false);
		String32 ssp32;
		UTF16Decoder16 utf16;
		Transcode::transcodeString2<Char16, Char32>(ssp16, ssp32, utf16);
		return ssp32;
	}

	string HierarchicalURI::getSchemeSpecificPart() const {
		string ssp8;
		renderOctets(ssp8, false);
		return ssp8;
	}

	string HierarchicalURI::getSchemeSpecificPart8() const {
		string ssp8;
		renderOctets(ssp8, false);
		string result;
		hierarchicalDecodeOriginal(ssp8, result);
		return result;
	}

	String16 HierarchicalURI::getSchemeSpecificPart16() const {
		string ssp8;
		renderOctets(ssp8, false);
		String16 result;
		hierarchicalDecodeOriginal(ssp8, result);
		return result;
	}

	String32 HierarchicalURI::getSchemeSpecificPart32() const {
		string ssp8;
		renderOctets(ssp8, false);
		String32 result;
		hierarchicalDecodeOriginal(ssp8, result);
		return result;
	}

	string HierarchicalURI::toString8() const {
		String16 ssp16;
		renderRendition(ssp16, true);
		string ssp8;
		UTF8Encoder16 utf8;
		Transcode::transcodeString2<Char16, char>(ssp16, ssp8, utf8);
		return ssp8;
	}

	String16 HierarchicalURI::toString16() const {
		String16 ssp16;
		renderRendition(ssp16, true);
		return ssp16;
	}

	String32 HierarchicalURI::toString32() const {
		String16 ssp16;
		renderRendition(ssp16, true);
		String32 ssp32;
		UTF16Decoder16 utf16;
		Transcode::transcodeString2<Char16, Char32>(ssp16, ssp32, utf16);
		return ssp32;
	}

#define bitFor(bit) ? Decomposition::HAS_ ## bit : 0
#define haveHere(member, bit) (member ## Defined bitFor(bit))
#define isPresent(member, bit) (!member.empty() bitFor(bit))

	URI* HierarchicalURI::makeNormalizedURI(const String16& newPath) const {
		int mask = haveHere(authority, AUTHORITY)
				| isPresent(username, USERNAME) | isPresent(password, PASSWORD) | isPresent(port, PORT)
				| haveHere(query, QUERY) | haveHere(fragment, FRAGMENT);
		return newURIOfThisType(mask, username, password, hostname, port, portNumber, newPath, query, fragment);
	}

	URI* HierarchicalURI::withFragment(const String16& newFragment) const {
		int mask = haveHere(authority, AUTHORITY)
				| isPresent(username, USERNAME) | isPresent(password, PASSWORD) | isPresent(port, PORT)
				| haveHere(query, QUERY) | Decomposition::HAS_FRAGMENT;
		return newURIOfThisType(mask, username, password, hostname, port, portNumber, path, query, newFragment);
	}

	URI* HierarchicalURI::makeResolvedURI(const URI& uri, bool useOwnAuthority, const String16& newPath) const {
		int mask;
		if(useOwnAuthority) {
			mask = haveHere(authority, AUTHORITY)
					| isPresent(username, USERNAME) | isPresent(password, PASSWORD) | isPresent(port, PORT)
					| (uri.hasQuery() bitFor(QUERY)) | (uri.hasFragment() bitFor(FRAGMENT));
			return newURIOfThisType(mask, username, password, hostname, port, portNumber, newPath,
					uri.getRawQuery16(), uri.getRawFragment16());
		}
		else {
			String16 theirUsername(uri.getRawUserName16()),
					theirPassword(uri.getRawPassword16()), theirPort(uri.getRawPort16());
			mask = Decomposition::HAS_AUTHORITY
					| (!theirUsername.empty() bitFor(USERNAME)) | (!theirPassword.empty() bitFor(PASSWORD))
					| (!theirPort.empty() bitFor(PORT))
					| (uri.hasQuery() bitFor(QUERY)) | (uri.hasFragment() bitFor(FRAGMENT));
			return newURIOfThisType(mask, theirUsername, theirPassword, uri.getRawHostName16(), theirPort,
					uri.getPortNumber(), newPath, uri.getRawQuery16(), uri.getRawFragment16());
		}
	}

	URI* HierarchicalURI::clone() const {
		int mask = haveHere(authority, AUTHORITY)
				| isPresent(username, USERNAME) | isPresent(password, PASSWORD) | isPresent(port, PORT)
				| haveHere(query, QUERY) | haveHere(fragment, FRAGMENT);
		return newURIOfThisType(mask, username, password, hostname, port, portNumber, path, query, fragment);
	}

#undef bitFor
#undef haveHere
#undef isPresent

	URI* HierarchicalURI::makeRelativizedURI(const URI&, const String16&) const {
		//TODO: make RelativeURI
		return NULL;
	}

	bool HierarchicalURI::isOpaque() const {
		return false;
	}

	bool HierarchicalURI::isRemote() const {
		return !hostname.empty();
	}

}}
