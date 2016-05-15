#include <redstrain/text/Transcode.hpp>
#include <redstrain/text/UTF8Decoder.hpp>
#include <redstrain/text/UTF16Decoder16.hpp>

#include "HierarchicalURI.hpp"

using std::string;
using redengine::text::Char16;
using redengine::text::Char32;
using redengine::text::String16;
using redengine::text::String32;
using redengine::text::Transcode;
using redengine::text::UTF8Decoder16;
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

	HierarchicalURI::HierarchicalURI(const HierarchicalURI& uri) : URI(uri), AbstractURI(uri),
			AuthoritativeURI(uri), GenericPathedURI(uri), QueriedURI(uri), FragmentedURI(uri) {}

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
