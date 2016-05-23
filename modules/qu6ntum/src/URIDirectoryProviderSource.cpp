#include <redstrain/vfs/VFS.hpp>
#include <redstrain/util/Delete.hpp>
#include <redstrain/error/Error.hpp>
#include <redstrain/text/Transcode.hpp>
#include <redstrain/vfs/RelativeURI.hpp>
#include <redstrain/text/StringUtils.hpp>
#include <redstrain/platform/Library.hpp>

#include "Q6Engine.hpp"
#include "URIDirectoryProviderSource.hpp"

using redengine::vfs::VFS;
using redengine::vfs::URI;
using redengine::vfs::Stat;
using redengine::text::Char16;
using redengine::util::Delete;
using redengine::error::Error;
using redengine::util::Appender;
using redengine::text::String16;
using redengine::text::Transcode;
using redengine::vfs::RelativeURI;
using redengine::text::StringUtils;
using redengine::platform::Library;

namespace redengine {
namespace qu6ntum {

	// ======== EntrySink ========

	URIDirectoryProviderSource::EntrySink::EntrySink(URIDirectoryProviderSource& providerSource)
			: providerSource(providerSource) {}

	URIDirectoryProviderSource::EntrySink::EntrySink(const EntrySink& sink)
			: Appender<String16>(sink), providerSource(sink.providerSource) {}

	void URIDirectoryProviderSource::EntrySink::append(const String16& filename) {
		VFS* vfs = providerSource.acquisition->getAcquiredResource();
		if(!vfs || !StringUtils<Char16>::endsWith(filename, providerSource.providerSuffix))
			return;
		Stat info;
		try {
			vfs->stat(filename, info, false);
		}
		catch(Error&) {
			return;
		}
		providerSource.polledProvider(filename.substr(static_cast<String16::size_type>(0u),
				filename.length() - providerSource.providerSuffix.length()),
				info.getModificationTimestamp(), info.getSize());
	}

	// ======== URIDirectoryProviderSource ========

	URIDirectoryProviderSource::URIDirectoryProviderSource(const URI& directoryURI)
			: resolvedURI(NULL), acquisition(NULL), providerSuffix(Transcode::utf8ToBMP(Library::FILENAME_SUFFIX)) {
		this->directoryURI = directoryURI.clone();
	}

	URIDirectoryProviderSource::URIDirectoryProviderSource(const URIDirectoryProviderSource& source)
			: PollingProviderSource(source) {
		Delete<URI> newdu(source.directoryURI->clone());
		Delete<URI> newru(source.resolvedURI->clone());
		if(source.acquisition) {
			acquisition = newru->acquireVFS();
			acquisition->requestResource();
		}
		else
			acquisition = NULL;
		directoryURI = newdu.set();
		resolvedURI = newru.set();
	}

	URIDirectoryProviderSource::~URIDirectoryProviderSource() {
		if(acquisition)
			delete acquisition;
		if(resolvedURI)
			delete resolvedURI;
		if(directoryURI)
			delete directoryURI;
	}

	void URIDirectoryProviderSource::startProviderSource(Q6Engine& engine) {
		PollingProviderSource::startProviderSource(engine);
		if(acquisition) {
			acquisition->releaseAcquiredResource();
			delete acquisition;
			acquisition = NULL;
		}
		if(resolvedURI) {
			delete resolvedURI;
			resolvedURI = NULL;
		}
		resolvedURI = engine.getInstanceHome().resolve(*directoryURI);
		acquisition = resolvedURI->acquireVFS();
		acquisition->requestResource();
	}

	void URIDirectoryProviderSource::stopProviderSource(Q6Engine& engine) {
		PollingProviderSource::stopProviderSource(engine);
		if(acquisition) {
			acquisition->releaseAcquiredResource();
			delete acquisition;
			acquisition = NULL;
		}
		if(resolvedURI) {
			delete resolvedURI;
			resolvedURI = NULL;
		}
	}

	String16 URIDirectoryProviderSource::getProviderSourceName() {
		String16 result(Transcode::utf8ToBMP("URIDirectoryProviderSource("));
		result += resolvedURI->toString16();
		result += static_cast<Char16>(')');
		return result;
	}

	void URIDirectoryProviderSource::pollProviders() {
		if(!acquisition)
			return;
		VFS* vfs = acquisition->getAcquiredResource();
		if(!vfs)
			return;
		EntrySink sink(*this);
		vfs->readdir(VFS::Pathname(), sink);
	}

	URI* URIDirectoryProviderSource::getProviderURIByName(const String16& name) {
		if(!resolvedURI)
			return NULL;
		RelativeURI rel(name + providerSuffix);
		return resolvedURI->resolve(rel);
	}

}}
