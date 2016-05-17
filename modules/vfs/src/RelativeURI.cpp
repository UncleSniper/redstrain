#include <redstrain/text/Transcode.hpp>
#include <redstrain/text/UTF8Decoder.hpp>
#include <redstrain/text/UTF16Encoder16.hpp>

#include "RelativeURI.hpp"
#include "URIResourceNotLocatableError.hpp"

using std::string;
using redengine::text::Char16;
using redengine::text::Char32;
using redengine::text::String16;
using redengine::text::String32;
using redengine::text::Transcode;
using redengine::io::InputStream;
using redengine::text::UTF8Decoder16;
using redengine::text::UTF16Encoder16;

namespace redengine {
namespace vfs {

	RelativeURI::RelativeURI(const String16& uri, const Decomposition& decomposition)
			: HierarchicalURI(uri, decomposition) {}

	RelativeURI::RelativeURI(const Components<Char16, CL_RENDITION>& components) : HierarchicalURI(components) {}

	RelativeURI::RelativeURI(const Components<char, CL_ORIGINAL>& components) : HierarchicalURI(components) {}

	RelativeURI::RelativeURI(const Components<Char16, CL_ORIGINAL>& components) : HierarchicalURI(components) {}

	RelativeURI::RelativeURI(const Components<Char32, CL_ORIGINAL>& components) : HierarchicalURI(components) {}

	RelativeURI::RelativeURI(const string& path) : HierarchicalURI(0) {
		UTF8Decoder16 utf8;
		Components<Char16, CL_ORIGINAL> components(0);
		Transcode::transcodeString2<char, Char16>(path, components.path, utf8);
		setByOriginal(components);
	}

	RelativeURI::RelativeURI(const String16& path) : HierarchicalURI(0) {
		Components<Char16, CL_ORIGINAL> components(0);
		components.path = path;
		setByOriginal(components);
	}

	RelativeURI::RelativeURI(const String32& path) : HierarchicalURI(0) {
		UTF16Encoder16 utf16;
		Components<Char16, CL_ORIGINAL> components(0);
		Transcode::transcodeString2<Char32, Char16>(path, components.path, utf16);
		setByOriginal(components);
	}

	RelativeURI::RelativeURI(const string& hostname, const string& path, const string& fragment)
			: HierarchicalURI((hostname.empty() ? 0 : Decomposition::HAS_AUTHORITY)
			| (fragment.empty() ? 0 : Decomposition::HAS_FRAGMENT)) {
		UTF8Decoder16 utf8;
		Components<Char16, CL_ORIGINAL> components(0);
		if(!hostname.empty()) {
			components.definedComponents |= Decomposition::HAS_AUTHORITY;
			Transcode::transcodeString2<char, Char16>(hostname, components.hostname, utf8);
			utf8.reset();
		}
		Transcode::transcodeString2<char, Char16>(path, components.path, utf8);
		if(!fragment.empty()) {
			components.definedComponents |= Decomposition::HAS_FRAGMENT;
			utf8.reset();
			Transcode::transcodeString2<char, Char16>(fragment, components.fragment, utf8);
		}
		setByOriginal(components);
	}

	RelativeURI::RelativeURI(const String16& hostname, const String16& path, const String16& fragment)
			: HierarchicalURI((hostname.empty() ? 0 : Decomposition::HAS_AUTHORITY)
			| (fragment.empty() ? 0 : Decomposition::HAS_FRAGMENT)) {
		Components<Char16, CL_ORIGINAL> components(0);
		if(!hostname.empty()) {
			components.definedComponents |= Decomposition::HAS_AUTHORITY;
			components.hostname = hostname;
		}
		components.path = path;
		if(!fragment.empty()) {
			components.definedComponents |= Decomposition::HAS_FRAGMENT;
			components.fragment = fragment;
		}
		setByOriginal(components);
	}

	RelativeURI::RelativeURI(const String32& hostname, const String32& path, const String32& fragment)
			: HierarchicalURI((hostname.empty() ? 0 : Decomposition::HAS_AUTHORITY)
			| (fragment.empty() ? 0 : Decomposition::HAS_FRAGMENT)) {
		UTF16Encoder16 utf16;
		Components<Char16, CL_ORIGINAL> components(0);
		if(!hostname.empty()) {
			components.definedComponents |= Decomposition::HAS_AUTHORITY;
			Transcode::transcodeString2<Char32, Char16>(hostname, components.hostname, utf16);
			utf16.reset();
		}
		Transcode::transcodeString2<Char32, Char16>(path, components.path, utf16);
		if(!fragment.empty()) {
			components.definedComponents |= Decomposition::HAS_FRAGMENT;
			utf16.reset();
			Transcode::transcodeString2<Char32, Char16>(fragment, components.fragment, utf16);
		}
		setByOriginal(components);
	}

	RelativeURI::RelativeURI(const RelativeURI& uri) : URI(uri), HierarchicalURI(uri) {}

	URI* RelativeURI::newURIOfThisType(int mask, const String16& username, const String16& password,
			const String16& hostname, const String16& port, uint16_t,
			const String16& path, const String16& query, const String16& fragment) const {
		Components<Char16, CL_RENDITION> components(mask);
		if(mask & Decomposition::HAS_AUTHORITY) {
			if(mask & Decomposition::HAS_USERNAME) {
				components.username = username;
				if(mask & Decomposition::HAS_PASSWORD)
					components.password = password;
			}
			components.hostname = hostname;
			if(mask & Decomposition::HAS_PORT)
				components.port = port;
		}
		components.path = path;
		if(mask & Decomposition::HAS_QUERY)
			components.query = query;
		if(mask & Decomposition::HAS_FRAGMENT)
			components.fragment = fragment;
		return new RelativeURI(components);
	}

	bool RelativeURI::isRelativeReference() const {
		return true;
	}

	bool RelativeURI::isLocator() const {
		return false;
	}

	URIAcquisition<InputStream<char> >* RelativeURI::acquireResource() const {
		throw URIResourceNotLocatableError(toString16());
	}

	URIAcquisition<VFS>* RelativeURI::acquireVFS() const {
		throw URIResourceNotLocatableError(toString16());
	}

}}
