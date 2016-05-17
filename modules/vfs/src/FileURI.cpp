#include <redstrain/text/Transcode.hpp>
#include <redstrain/text/UTF8Decoder.hpp>
#include <redstrain/text/UTF16Encoder16.hpp>

#include "FileURI.hpp"
#include "HostVFS.hpp"
#include "RootedVFS.hpp"
#include "NotADirectoryError.hpp"
#include "URIResourceNotLocatableError.hpp"

using std::string;
using redengine::text::Char16;
using redengine::text::Char32;
using redengine::text::String16;
using redengine::text::String32;
using redengine::text::Transcode;
using redengine::io::InputStream;
using redengine::io::OutputStream;
using redengine::text::UTF8Decoder16;
using redengine::text::UTF16Encoder16;

namespace redengine {
namespace vfs {

	// ======== AcquisitionBase ========

	FileURI::AcquisitionBase::AcquisitionBase(const FileURI& uri, VFS& vfs) : uri(uri), vfs(vfs) {}

	FileURI::AcquisitionBase::AcquisitionBase(const AcquisitionBase& base) : uri(base.uri), vfs(base.vfs) {}

	FileURI::AcquisitionBase::~AcquisitionBase() {}

	// ======== StreamAcquisition ========

	FileURI::StreamAcquisition::StreamAcquisition(const FileURI& uri, VFS& vfs)
			: AcquisitionBase(uri, vfs), stream(NULL) {}

	FileURI::StreamAcquisition::StreamAcquisition(const StreamAcquisition& acquisition)
			: AcquisitionBase(acquisition), URIAcquisition<InputStream<char> >(acquisition), stream(NULL) {}

	FileURI::StreamAcquisition::~StreamAcquisition() {
		if(stream)
			delete stream;
	}

	void FileURI::StreamAcquisition::requestResource() {
		if(!stream)
			stream = vfs.getInputStream(uri.getPath16());
	}

	OutputStream<char>* FileURI::StreamAcquisition::getRequestStream() {
		requestResource();
		return NULL;
	}

	InputStream<char>* FileURI::StreamAcquisition::getAcquiredResource() {
		return stream;
	}

	void FileURI::StreamAcquisition::releaseAcquiredResource() {
		if(stream) {
			stream->close();
			delete stream;
			stream = NULL;
		}
	}

	// ======== VFSAcquisition ========

	FileURI::VFSAcquisition::VFSAcquisition(const FileURI& uri, VFS& vfs)
			: AcquisitionBase(uri, vfs), rootedVFS(NULL) {}

	FileURI::VFSAcquisition::VFSAcquisition(const VFSAcquisition& acquisition)
			: AcquisitionBase(acquisition), URIAcquisition<VFS>(acquisition), rootedVFS(NULL) {}

	FileURI::VFSAcquisition::~VFSAcquisition() {
		if(rootedVFS)
			delete rootedVFS;
	}

	void FileURI::VFSAcquisition::requestResource() {
		if(!rootedVFS) {
			String16 path(uri.getPath16());
			VFS::Pathname pathname;
			VFS::deconstructPathname(path, pathname);
			Stat info;
			vfs.stat(pathname, info, false);
			if(info.getType() != Stat::DIRECTORY)
				throw NotADirectoryError(pathname);
			rootedVFS = new RootedVFS(vfs, false, pathname);
		}
	}

	OutputStream<char>* FileURI::VFSAcquisition::getRequestStream() {
		requestResource();
		return NULL;
	}

	VFS* FileURI::VFSAcquisition::getAcquiredResource() {
		return rootedVFS;
	}

	void FileURI::VFSAcquisition::releaseAcquiredResource() {
		if(rootedVFS) {
			delete rootedVFS;
			rootedVFS = NULL;
		}
	}

	// ======== FileURI ========

	FileURI::FileURI(const String16& uri, const Decomposition& decomposition)
			: HierarchicalURI(uri, decomposition), vfs(NULL), remoteAcquirer(NULL) {}

	FileURI::FileURI(const Components<Char16, CL_RENDITION>& components)
			: HierarchicalURI(components), vfs(NULL), remoteAcquirer(NULL) {}

	FileURI::FileURI(const Components<char, CL_ORIGINAL>& components)
			: HierarchicalURI(components), vfs(NULL), remoteAcquirer(NULL) {}

	FileURI::FileURI(const Components<Char16, CL_ORIGINAL>& components)
			: HierarchicalURI(components), vfs(NULL), remoteAcquirer(NULL) {}

	FileURI::FileURI(const Components<Char32, CL_ORIGINAL>& components)
			: HierarchicalURI(components), vfs(NULL), remoteAcquirer(NULL) {}

	FileURI::FileURI(const string& path) : HierarchicalURI(0), vfs(NULL), remoteAcquirer(NULL) {
		UTF8Decoder16 utf8;
		Components<Char16, CL_ORIGINAL> components(0);
		Transcode::transcodeString2<char, Char16>(path, components.path, utf8);
		setByOriginal(components);
	}

	FileURI::FileURI(const String16& path) : HierarchicalURI(0), vfs(NULL), remoteAcquirer(NULL) {
		Components<Char16, CL_ORIGINAL> components(0);
		components.path = path;
		setByOriginal(components);
	}

	FileURI::FileURI(const String32& path) : HierarchicalURI(0), vfs(NULL), remoteAcquirer(NULL) {
		UTF16Encoder16 utf16;
		Components<Char16, CL_ORIGINAL> components(0);
		Transcode::transcodeString2<Char32, Char16>(path, components.path, utf16);
		setByOriginal(components);
	}

	FileURI::FileURI(const string& hostname, const string& path)
			: HierarchicalURI(hostname.empty() ? 0 : Decomposition::HAS_AUTHORITY), vfs(NULL), remoteAcquirer(NULL) {
		UTF8Decoder16 utf8;
		Components<Char16, CL_ORIGINAL> components(0);
		if(!hostname.empty()) {
			components.definedComponents |= Decomposition::HAS_AUTHORITY;
			Transcode::transcodeString2<char, Char16>(hostname, components.hostname, utf8);
			utf8.reset();
		}
		Transcode::transcodeString2<char, Char16>(path, components.path, utf8);
		setByOriginal(components);
	}

	FileURI::FileURI(const String16& hostname, const String16& path)
			: HierarchicalURI(hostname.empty() ? 0 : Decomposition::HAS_AUTHORITY), vfs(NULL), remoteAcquirer(NULL) {
		Components<Char16, CL_ORIGINAL> components(0);
		if(!hostname.empty()) {
			components.definedComponents |= Decomposition::HAS_AUTHORITY;
			components.hostname = hostname;
		}
		components.path = path;
		setByOriginal(components);
	}

	FileURI::FileURI(const String32& hostname, const String32& path)
			: HierarchicalURI(hostname.empty() ? 0 : Decomposition::HAS_AUTHORITY), vfs(NULL), remoteAcquirer(NULL) {
		UTF16Encoder16 utf16;
		Components<Char16, CL_ORIGINAL> components(0);
		if(!hostname.empty()) {
			components.definedComponents |= Decomposition::HAS_AUTHORITY;
			Transcode::transcodeString2<Char32, Char16>(hostname, components.hostname, utf16);
			utf16.reset();
		}
		Transcode::transcodeString2<Char32, Char16>(path, components.path, utf16);
		setByOriginal(components);
	}

	FileURI::FileURI(const FileURI& uri)
			: URI(uri), HierarchicalURI(uri), vfs(uri.vfs), remoteAcquirer(uri.remoteAcquirer) {}

	URI* FileURI::newURIOfThisType(int mask, const String16& username, const String16& password,
			const String16& hostname, const String16& port, uint16_t,
			const String16& path, const String16& query, const String16& fragment) const {
		Components<Char16, CL_RENDITION> components(mask);
		components.scheme = scheme;
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
		return new FileURI(components);
	}

	bool FileURI::isLocator() const {
		return !authorityDefined || remoteAcquirer;
	}

	URIAcquisition<InputStream<char> >* FileURI::acquireResource() const {
		if(authorityDefined) {
			if(!remoteAcquirer)
				throw URIResourceNotLocatableError(toString16());
			return remoteAcquirer->acquireResource(*this);
		}
		else
			return new StreamAcquisition(*this, vfs ? *vfs : HostVFS::instance);
	}

	URIAcquisition<VFS>* FileURI::acquireVFS() const {
		if(authorityDefined) {
			if(!remoteAcquirer)
				throw URIResourceNotLocatableError(toString16());
			return remoteAcquirer->acquireVFS(*this);
		}
		else
			return new VFSAcquisition(*this, vfs ? *vfs : HostVFS::instance);
	}

}}
