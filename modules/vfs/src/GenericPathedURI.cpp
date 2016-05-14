#include "GenericPathedURI.hpp"

using redengine::text::Char16;
using redengine::text::String16;

namespace redengine {
namespace vfs {

	GenericPathedURI::GenericPathedURI() {}

	GenericPathedURI::GenericPathedURI(const String16& path) : PathedURI(path) {}

	GenericPathedURI::GenericPathedURI(const GenericPathedURI& uri) : URI(uri), PathedURI(uri) {}

	URI* GenericPathedURI::normalize() const {
		if(isOpaque())
			return clone();
		String16 newPath;
		URI::normalizePath<Char16>(path, newPath);
		return makeNormalizedURI(newPath);
	}

	URI* GenericPathedURI::relativize(const URI& uri) const {
		if(isOpaque() || uri.isOpaque() || !isSchemeAndAuthorityEqual(uri))
			return uri.clone();
		String16 newPath;
		URI::relativizePath<Char16>(path, uri.getRawPath16(), newPath);
		return makeRelativizedURI(uri, newPath);
	}

	URI* GenericPathedURI::resolve(const URI& uri) const {
		if(!uri.isRelativeReference() || isOpaque() || uri.isOpaque())
			return uri.clone();
		if(uri.hasFragment() && uri.getPathMode() == PM_NONE && !uri.hasAuthority() && !uri.hasQuery())
			return withFragment(uri.getRawFragment16());
		if(uri.hasAuthority())
			//this.scheme, that.authority, that.path, that.query, that.fragment
			return makeResolvedURI(uri, false, uri.getRawPath16());
		else if(uri.getPathMode() == PM_ABSOLUTE)
			//this.scheme, this.authority, that.path, that.query, that.fragment
			return makeResolvedURI(uri, true, uri.getRawPath16());
		else {
			//this.scheme, this.authority, cat(this.path, that.path), that.query, that.fragment
			String16 newPath;
			URI::resolvePath<Char16>(path, uri.getRawPath16(), newPath);
			return makeResolvedURI(uri, true, newPath);
		}
	}

}}
